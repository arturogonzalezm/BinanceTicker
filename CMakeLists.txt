cmake_minimum_required(VERSION 3.28)
project(BinanceTicker)

set(CMAKE_CXX_STANDARD 20)

# Find Boost libraries
find_package(Boost REQUIRED system)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

# Add include directory for websocketpp, libpqxx, and nlohmann/json
include_directories(/opt/homebrew/include /usr/local/include include)

# Find OpenSSL
find_package(OpenSSL REQUIRED)
if(OpenSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

# Find and link libpqxx
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQXX REQUIRED libpqxx)
include_directories(${LIBPQXX_INCLUDE_DIRS})
link_directories(${LIBPQXX_LIBRARY_DIRS})

# Add the main executable
add_executable(BinanceTicker
        main.cpp
        include/BinanceTicker.hpp
        include/WebSocketClient.hpp
        src/WebSocketClient.cpp
        src/BinanceTicker.cpp
        include/json.hpp
)

# Link the necessary libraries
target_link_libraries(BinanceTicker
        ${Boost_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        /opt/homebrew/opt/openssl/lib/libssl.dylib
        /opt/homebrew/opt/openssl/lib/libcrypto.dylib
        ${LIBPQXX_LIBRARIES}
        pthread
)
