cmake_minimum_required(VERSION 3.15)
project(PeerClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable vcpkg toolchain
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

find_package(Poco REQUIRED COMPONENTS Foundation Net)
find_package(OpenCV REQUIRED)

add_executable(PeerClient main.cpp)
target_link_libraries(PeerClient PRIVATE Poco::Foundation Poco::Net ${OpenCV_LIBS}) 