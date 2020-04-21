cmake_minimum_required (VERSION 3.8)

project ("Test-Boost")

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} "echo_server.cpp")

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /await")
	
	set(Boost_USE_STATIC_LIBS ON)
	set(OPENSSL_USE_STATIC_LIBS TRUE)
endif ()

find_package(Boost 1.71.0 COMPONENTS system date_time PATHS C:/Boost/lib/cmake/Boost-1.71.0)

find_package(OpenSSL)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIR})

#add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES})

if (MSVC)
	target_link_libraries(${PROJECT_NAME} crypt32)
elseif (UNIX)
	target_link_libraries(${PROJECT_NAME} pthread dl)
endif ()