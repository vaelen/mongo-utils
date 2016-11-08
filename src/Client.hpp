#include <iostream>
#include <vector>

#include <boost/log/trivial.hpp>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/exception/exception.hpp>

namespace MongoUtils {
  class Client {
  public:
    Client(std::string &uri_string): inst(), uri(uri_string), client(uri) {}
    void connect();
    std::vector<std::string> list_shards();
    void drain_shard(std::string &shard_name);
  private:
    mongocxx::instance inst;
    mongocxx::uri uri;
    mongocxx::client client;
  };
}
