#include "Client.hpp"

using namespace bsoncxx;
using namespace mongocxx;

namespace MongoUtils {
  void Client::connect() {
    BOOST_LOG_TRIVIAL(debug) << "Connecting to " << uri.to_string();
    database admin = client["admin"];
    document::value ping_cmd = builder::stream::document{} << "ping" << 1 << builder::stream::finalize;
    BOOST_LOG_TRIVIAL(trace) << "Command: " << to_json(ping_cmd.view());
    document::value result = admin.run_command(ping_cmd.view());
    BOOST_LOG_TRIVIAL(debug) << "Successfully Connected";
  }

  std::vector<std::string> Client::list_shards() {
    database admin = client["admin"];
    BOOST_LOG_TRIVIAL(debug) << "Listing Shards";
    document::value list_shards_cmd = builder::stream::document{} << "listShards" << 1 << builder::stream::finalize;
    BOOST_LOG_TRIVIAL(trace) << "Command: " << to_json(list_shards_cmd.view());
    document::value result = admin.run_command(list_shards_cmd.view());
    BOOST_LOG_TRIVIAL(trace) << "Command Returned: " << bsoncxx::to_json(result);
    array::view shards = result.view()["shards"].get_array();
    std::vector<std::string> shard_vector{};
    for(auto shard : shards) {
      BOOST_LOG_TRIVIAL(debug) << "Shard: " << bsoncxx::to_json(shard.get_document().view());
      bsoncxx::stdx::string_view id = shard["_id"].get_utf8();
      shard_vector.push_back(id.to_string());
    }
    return shard_vector;
  }

  void Client::drain_shard(std::string &shard_name) {
  }
}
