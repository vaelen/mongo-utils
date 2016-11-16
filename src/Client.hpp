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

#include <iostream>
#include <vector>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/exception/exception.hpp>

#include <boost/log/trivial.hpp>

namespace MongoUtils {
  class Shard {
  public:
    const std::string name;
    const std::string host;
    const bool is_draining;
    Shard(std::string name): name(name), host(""), is_draining(false) {}
    Shard(std::string name, std::string host): name(name), host(host), is_draining(false) {}
    Shard(std::string name, std::string host, bool is_draining): name(name), host(host), is_draining(is_draining) {}
    Shard(bsoncxx::document::view shard);
    std::string to_string();
  };

  class Chunk {
  public:
    const std::string _id;
    const std::string ns;
    const bsoncxx::document::value min;
    const bsoncxx::document::value max;
    const std::string shard;
    Chunk(bsoncxx::document::view chunk);
    std::string to_string();
  };
  
  class Client {
  public:
    Client(std::string &uri_string): inst(), uri(uri_string), client(uri) { init_logging(); }
    void connect();
    std::vector<Shard> shards();
    std::vector<Chunk> chunks(const std::string &shard_name);
    void move_chunks(const std::string &from_shard_name, const std::string &to_shard_name);
    void remove_shard(const std::string &shard_name);
    void log_level(const boost::log::trivial::severity_level level);
  private:
    mongocxx::instance inst;
    mongocxx::uri uri;
    mongocxx::client client;
    void init_logging();
    void _verify_remove_shard(const std::string &shard_name);
  };
}
