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

using namespace std;
using namespace MongoUtils;

int main(int argc, char** argv) {
  string uri_string = "mongodb://localhost";
  if (argc > 1) {
    uri_string = argv[1];
  }
  try {
    Client client{uri_string};
    client.connect();
    vector<Shard> shards = client.shards();
    if(shards.empty()) {
      cout << "No Shards" << endl;
    } else {
      string shard_name = "";
      bool found_shard = false;
      for(Shard shard : shards) {
        cout << "Shard: " << shard.name << "\tHost: " << shard.host << "\tDraining: " << (shard.is_draining ? "Yes" : "No") << endl;
        if(!found_shard && shard.is_draining) {
          cout << "\tFound Draining Shard" << endl;
          shard_name = shard.name;
          found_shard = true;
        }
      }
      if(!found_shard) {
        // TODO: Remove this debug line later
        shard_name = shards[0].name;
        cout << "No Draining Shards.  Removing Shard " << shard_name << endl;
      }
      client.remove_shard(shard_name);
    }
  } catch (const exception &ex) {
    cout << "Exception: " << ex.what() << endl;
  }
}
