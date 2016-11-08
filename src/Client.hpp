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
