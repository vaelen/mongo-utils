project(MONGO_UTILS)

set( CMAKE_VERBOSE_MAKEFILE on )

set(BOOST_REQUIRED_VERSION 1.58)
find_package(Boost ${BOOST_REQUIRED_VERSION} REQUIRED COMPONENTS log)
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

add_executable(moveshard moveshard.cpp)
target_link_libraries(moveshard Client)
