cmake_minimum_required(VERSION 3.15)
project(PeerClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Qt6 path (adjust if needed)
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.1/msvc2022_64/lib/cmake")

find_package(Qt6 COMPONENTS Widgets Network REQUIRED)
find_package(Poco REQUIRED COMPONENTS Foundation Net)
find_package(OpenCV REQUIRED)

set(SOURCES main.cpp)

add_executable(PeerClient ${SOURCES})
target_link_libraries(PeerClient PRIVATE Qt6::Widgets Qt6::Network Poco::Foundation Poco::Net ${OpenCV_LIBS})

# Enable automoc for Qt
set_target_properties(PeerClient PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
) 