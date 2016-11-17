#  Copyright 2016 Andrew Young <andrew@vaelen.org>
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
  
project(MONGO_UTILS)

# set( CMAKE_VERBOSE_MAKEFILE on )

set(BOOST_REQUIRED_VERSION 1.58)
find_package(Boost ${BOOST_REQUIRED_VERSION} REQUIRED COMPONENTS log program_options)
add_definitions(-DBOOST_ALL_DYN_LINK)

set(LIBBSONCXX_REQUIRED_VERSION 3.0.2)
find_package(LibBSONCXX ${LIBBSONCXX_REQUIRED_VERSION} REQUIRED)

set(LIBMONGOCXX_REQUIRED_VERSION 3.0.2)
find_package(LibMongoCXX ${LIBMONGOCXX_REQUIRED_VERSION} REQUIRED)

set(MONGO_UTILS_VERSION 0.0.1)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${LIBBSONCXX_INCLUDE_DIRS}
    ${LIBMONGOCXX_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
)

link_directories(
  ${Boost_LIBRARY_DIRS}
  ${LIBBSONCXX_LIBRARY_DIRS}
  ${LIBMONGOCXX_LIBRARY_DIRS}
)

add_library(Client Client.cpp)
target_link_libraries(Client
  ${Boost_LIBRARIES}
  ${Boost_LOG_LIBRARY}
  ${LIBBSONCXX_LIBRARIES}
  ${LIBMONGOCXX_LIBRARIES}
)

add_executable(connect connect.cpp)
target_link_libraries(connect Client)

add_executable(removeshard removeshard.cpp)
target_link_libraries(removeshard Client)

add_executable(status status.cpp)
target_link_libraries(status Client)

add_executable(emptyshard emptyshard.cpp)
target_link_libraries(emptyshard
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  Client
)
