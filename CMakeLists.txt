cmake_minimum_required (VERSION 3.8)

project ("VKBot")

set(CMAKE_CXX_STANDARD 20)

include_directories(uri/include)

add_subdirectory(uri/src)

add_compile_definitions(BOOST_BEAST_USE_STD_STRING_VIEW)

add_executable(${PROJECT_NAME} main.cpp Network/NetworkManager.cpp Network/HttpSession.cpp)

set(BOOST_COMPONENTS beast)
set(BOOST_VERSION 1.71.0)

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /await")
	
	set(Boost_USE_STATIC_LIBS ON)
	set(OPENSSL_USE_STATIC_LIBS TRUE)

	find_package(Boost ${BOOST_VERSION} COMPONENTS ${BOOST_COMPONENTS} PATHS C:/Boost/lib/cmake/Boost-1.71.0)
elseif (UNIX)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")
	set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -O3 -g0 -DNDEBUG -s")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -mtune=skylake -fPIE")

	find_package(Boost ${BOOST_VERSION} COMPONENTS ${BOOST_COMPONENTS})
endif ()

find_package(OpenSSL)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} network-uri)

if (MSVC)
	target_link_libraries(${PROJECT_NAME} crypt32)
elseif (UNIX)
	target_link_libraries(${PROJECT_NAME} pthread dl)
endif ()