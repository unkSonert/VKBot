cmake_minimum_required (VERSION 3.8)

project ("Test-Boost")

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} "main.cpp")

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /await")
endif(MSVC)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.71 COMPONENTS system)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} OpenSSL::SSL)
