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

#pragma warning(push)
#pragma warning(disable:4819)
#   include <boost/shared_ptr.hpp>
#   include <boost/date_time/posix_time/posix_time_types.hpp>
#   include <boost/log/trivial.hpp>
#   include <boost/log/core.hpp>
#   include <boost/log/expressions.hpp>
#   include <boost/log/sources/logger.hpp>
#   include <boost/log/utility/setup/file.hpp>
#   include <boost/log/utility/setup/console.hpp>
#   include <boost/log/utility/setup/common_attributes.hpp>
#   include <boost/log/support/date_time.hpp>
#   include <boost/log/sinks/sync_frontend.hpp>
#   include <boost/log/sinks/text_file_backend.hpp>
#   include <boost/log/sinks/text_ostream_backend.hpp>
#   include <boost/log/attributes/named_scope.hpp>
#pragma warning(pop)
#pragma warning(disable:4503)

namespace MongoUtils {
  
  void Client::init_logging() {
    // Adapted from https://gist.github.com/xiongjia/e23b9572d3fc3d677e3d
    boost::log::add_common_attributes();
    boost::log::core::get()->add_global_attribute("Scope", boost::log::attributes::named_scope());
    log_level(boost::log::trivial::debug);

    // [TimeStamp] [ThreadId] [Severity Level] [Scope] Log message
    auto fmtTimeStamp = boost::log::expressions::
      format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f");
    auto fmtThreadId = boost::log::expressions::
      attr<boost::log::attributes::current_thread_id::value_type>("ThreadID");
    auto fmtSeverity = boost::log::expressions::
      attr<boost::log::trivial::severity_level>("Severity");
    auto fmtScope =
      boost::log::expressions::format_named_scope("Scope",
                                                  boost::log::keywords::format = "%n(%f:%l)",
                                                  boost::log::keywords::iteration = boost::log::expressions::reverse,
                                                  boost::log::keywords::depth = 2);
    boost::log::formatter logFmt =
      boost::log::expressions::format("[%1%] (%2%) [%3%] [%4%] %5%")
      % fmtTimeStamp % fmtThreadId % fmtSeverity % fmtScope
      % boost::log::expressions::smessage;

    // Console
    // auto consoleSink = boost::log::add_console_log(std::clog);
    // consoleSink->set_formatter(logFmt);

    // Log File
    auto fsSink =
      boost::log::add_file_log(boost::log::keywords::file_name = "client_%Y-%m-%d_%H-%M-%S.%N.log",
                               boost::log::keywords::rotation_size = 10 * 1024 * 1024,
                               boost::log::keywords::min_free_space = 30 * 1024 * 1024,
                               boost::log::keywords::open_mode = std::ios_base::app);

    fsSink->set_formatter(logFmt);
    fsSink->locked_backend()->auto_flush(true);
  }

  void Client::log_level(const boost::log::trivial::severity_level level) {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= level);
  }
}
