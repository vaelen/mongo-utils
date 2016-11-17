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
#include <boost/program_options.hpp>
//#include <GetOpt.h>

using namespace std;
using namespace MongoUtils;
using namespace boost::program_options;

int main(int argc, char** argv) {
  const string usage = "Usage: emptyshard [--url <url>] <shard> [shard...]";
  try {
    string uri_string;
    vector<string> shards;

    options_description options("Options");
    options.add_options()
      ("help,h", "this message")
      ("url,u", value<string>(&uri_string)->default_value("mongodb://localhost"), "MongoDB URL")
      ("shards", value<vector<string>>(&shards)->required(), "Shards to move chunks from");

    positional_options_description p;
    p.add("shards", -1);
  
    variables_map opts;
    store(command_line_parser(argc, argv).options(options).positional(p).run(), opts);

    if (opts.count("help") || !opts.count("shards")) {
      cout << usage << endl << options << endl;
      return 1;
    }

    notify(opts);

    if (opts.count("url")) {
      uri_string = opts["url"].as<string>();
    }

    Client client{uri_string};
    client.log_level(boost::log::trivial::trace);
    client.connect();
    for(auto shard: shards) {
      cout << "Emptying shard " << shard << endl;
      client.empty_shard(shard);
    }
  } catch (const exception &ex) {
    cout << "Error: " << ex.what() << endl;
  }
}

