project(MOVESHARD)

set( CMAKE_VERBOSE_MAKEFILE on )

set(LIBBSONCXX_REQUIRED_VERSION 3.0.2)
find_package(LibBSONCXX ${LIBBSONCXX_REQUIRED_VERSION} REQUIRED)

set(LIBMONGOCXX_REQUIRED_VERSION 3.0.2)
find_package(LibMongoCXX ${LIBMONGOCXX_REQUIRED_VERSION} REQUIRED)

set(MOVESHARD_VERSION 0.0.1)

include_directories(
    ${LIBBSONCXX_INCLUDE_DIRS}
    ${LIBMONGOCXX_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
)

link_directories(
  ${LIBBSONCXX_LIBRARY_DIRS}
  ${LIBMONGOCXX_LIBRARY_DIRS}
)

add_library(Client Client.cpp)
target_link_libraries(Client ${LIBBSONCXX_LIBRARIES} ${LIBMONGOCXX_LIBRARIES})

add_executable(connect connect.cpp)
target_link_libraries(connect Client)

add_executable(moveshard moveshard.cpp)
target_link_libraries(moveshard Client)
