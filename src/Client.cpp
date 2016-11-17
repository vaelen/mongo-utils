/*
  Copyright 2016 Andrew Young <andrew@vaelen.org>

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#include "Client.hpp"

#include <sstream>

#include <mongocxx/cursor.hpp>

#include "client-logging.cpp"

using namespace bsoncxx;
using namespace mongocxx;

namespace MongoUtils {
  Chunk::Chunk(document::view chunk):
    _id((std::string)(bsoncxx::stdx::string_view)chunk["_id"].get_utf8()),
    ns((std::string)(bsoncxx::stdx::string_view)chunk["ns"].get_utf8()),
    min(document::value(chunk["min"].get_document())),
    max(document::value(chunk["max"].get_document())),
    shard((std::string)(bsoncxx::stdx::string_view)chunk["shard"].get_utf8()) {}

  std::string Chunk::to_string() {
    std::stringstream s;
    s << "{ ";
    s << "_id: \"" << _id << "\"";
    s << ", ns: \"" << ns << "\"";
    s << ", shard: \"" << shard << "\"";
    s << ", min: " << to_json(min.view());
    s << ", max: " << to_json(max.view());
    s << " }";
    return s.str();
  }

  Shard::Shard(document::view shard):
    name((std::string)(bsoncxx::stdx::string_view)shard["_id"].get_utf8()),
    host((shard["host"]) ? (std::string)(bsoncxx::stdx::string_view)shard["host"].get_utf8() : ""),
    is_draining((shard["is_draining"]) ? (bool)shard["is_draining"].get_bool() : false) {}
  
  std::string Shard::to_string() {
    std::stringstream s;
    s << "{ ";
    s << "name: \"" << name << "\"";
    s << ", host: \"" << host << "\"";
    s << ", draining: " << is_draining;
    s << " }";
    return s.str();    
  }
  
  void Client::connect() {
    BOOST_LOG_FUNCTION();
    BOOST_LOG_TRIVIAL(debug) << "Connecting to " << uri.to_string();
    database admin = client["admin"];
    document::value ping_cmd = builder::stream::document{} << "ping" << 1 << builder::stream::finalize;
    BOOST_LOG_TRIVIAL(trace) << "Command: " << to_json(ping_cmd.view());
    document::value result = admin.run_command(ping_cmd.view());
    BOOST_LOG_TRIVIAL(debug) << "Successfully Connected";
  }

  std::vector<Shard> Client::shards() {
    BOOST_LOG_FUNCTION();
    database admin = client["admin"];
    BOOST_LOG_TRIVIAL(debug) << "Listing Shards";
    document::value list_shards_cmd = builder::stream::document{} << "listShards" << 1 << builder::stream::finalize;
    BOOST_LOG_TRIVIAL(trace) << "Command: " << to_json(list_shards_cmd.view());
    document::value result = admin.run_command(list_shards_cmd.view());
    BOOST_LOG_TRIVIAL(trace) << "Command Returned: " << bsoncxx::to_json(result);
    array::view shards = result.view()["shards"].get_array();
    std::vector<Shard> shard_vector{};
    for(auto shard : shards) {
      BOOST_LOG_TRIVIAL(debug) << "Shard: " << bsoncxx::to_json(shard.get_document().view());
      shard_vector.push_back(Shard(shard.get_document().view()));
    }
    return shard_vector;
  }

  std::vector<Chunk> Client::chunks(const std::string &shard_name) {
    BOOST_LOG_FUNCTION();
    database config = client["config"];
    BOOST_LOG_TRIVIAL(debug) << "Listing Chunks for Shard : " << shard_name;
    document::value find_chunks = builder::stream::document{} << "shard" << shard_name << builder::stream::finalize;
    cursor chunks = config["chunks"].find(find_chunks.view());
    std::vector<Chunk> chunk_vector{};
    for(auto chunk : chunks) {
      BOOST_LOG_TRIVIAL(debug) << "Chunk: " << to_json(chunk);
      chunk_vector.push_back(Chunk(chunk));
    }
    return chunk_vector;
  }

  void Client::empty_shard(const std::string &shard_name) {
    BOOST_LOG_FUNCTION();
    BOOST_LOG_TRIVIAL(debug) << "Emptying Shard " << shard_name;
    std::vector<std::string> shard_names;
    for(auto shard: shards()) {
      if (shard_name != shard.name) {
        shard_names.push_back(shard.name);
      }
    }
      
    auto it = shard_names.begin();
    for(auto chunk: chunks(shard_name)) {
      std::string shard_name = *it;
      move_chunk(chunk, shard_name);
      it++;
      if (it == shard_names.end()) {
        it = shard_names.begin();
      }
    }
  }
  
  void Client::move_chunk(const Chunk &chunk, const std::string &to_shard_name) {
    BOOST_LOG_FUNCTION();
    BOOST_LOG_TRIVIAL(debug) << "Moving Chunk " << chunk._id << " from " << chunk.shard << " to " << to_shard_name;

    database admin = client["admin"];
    
    document::value move_chunk_cmd = builder::stream::document{}
    << "moveChunk" << chunk.ns
    << "bounds" << builder::stream::open_array << chunk.min << chunk.max << builder::stream::close_array
    << "to" << to_shard_name
    << builder::stream::finalize;
      
    BOOST_LOG_TRIVIAL(trace) << "Command: " << to_json(move_chunk_cmd.view());

    document::value result = admin.run_command(move_chunk_cmd.view());

    BOOST_LOG_TRIVIAL(trace) << "Command Returned: " << bsoncxx::to_json(result);
  }
  
  void Client::remove_shard(const std::string &shard_name) {
    BOOST_LOG_FUNCTION();
    _verify_remove_shard(shard_name);
  }

  void Client::_verify_remove_shard(const std::string &shard_name) {
    BOOST_LOG_FUNCTION();
    BOOST_LOG_TRIVIAL(debug) << "Removing Shard: " << shard_name;
    database admin = client["admin"];
    document::value remove_shard_cmd = builder::stream::document{} << "removeShard" << shard_name << builder::stream::finalize;
    BOOST_LOG_TRIVIAL(trace) << "Command: " << to_json(remove_shard_cmd.view());
    document::value result = admin.run_command(remove_shard_cmd.view());
    BOOST_LOG_TRIVIAL(trace) << "Command Returned: " << bsoncxx::to_json(result);
  }
}
