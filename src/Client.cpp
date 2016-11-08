#include "Client.hpp"

namespace MongoUtils {
  void Client::connect() {
    std::cout << "Connecting to " << uri.to_string() << std::endl;

    // Get a reference to the admin database
    mongocxx::database admin = client["admin"];

    bsoncxx::document::value ping_cmd = bsoncxx::builder::stream::document{} << "ping" << 1 << bsoncxx::builder::stream::finalize;

    std::cout << "Command: " << bsoncxx::to_json(ping_cmd.view()) << std::endl;
    
    // Run a command to verify our connection
    bsoncxx::document::value result = admin.run_command(ping_cmd.view());
    
    std::cout << "Successfully Connected" << std::endl;
  }
}
