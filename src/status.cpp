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
#include "Client.hpp"
#include <boost/log/trivial.hpp>

using namespace std;
using namespace MongoUtils;

int main(int argc, char** argv) {
  string uri_string = "mongodb://localhost";
  if (argc > 1) {
    uri_string = argv[1];
  }
  try {
    Client client{uri_string};
    client.log_level(boost::log::trivial::trace);
    client.connect();
    vector<Shard> shards = client.shards();
    if(shards.empty()) {
      cout << "No Shards" << endl;
    } else {
      for(Shard shard : shards) {
        cout << "Shard: " << shard.to_string() << endl;
        vector<Chunk> chunks = client.chunks(shard.name);
        for(Chunk chunk: chunks) {
          cout << "    Chunk: " << chunk.to_string() << endl;
        }
      }
    }
  } catch (const exception &ex) {
    cout << "Exception: " << ex.what() << endl;
  }
}
