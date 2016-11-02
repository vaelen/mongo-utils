project(MOVESHARD)

set( CMAKE_VERBOSE_MAKEFILE on )

set(LIBBSONCXX_REQUIRED_VERSION 3.0.2)
find_package(LibBSONCXX ${LIBBSONCXX_REQUIRED_VERSION} REQUIRED)

set(LIBMONGOCXX_REQUIRED_VERSION 3.0.2)
find_package(LibMongoCXX ${LIBMONGOCXX_REQUIRED_VERSION} REQUIRED)

set(MOVESHARD_VERSION 0.0.1)

set(moveshard_sources
  test.cpp
)

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

add_executable(testapp test.cpp)
target_link_libraries(testapp ${LIBBSONCXX_LIBRARIES} ${LIBMONGOCXX_LIBRARIES})

#add_executable(moveshard
 # ${moveshard_sources}
#)
