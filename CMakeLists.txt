cmake_minimum_required (VERSION 3.8)

project ("VKBot")

set(CMAKE_CXX_STANDARD 20)

include_directories(uri/include)
include_directories(json/single_include)

add_subdirectory(uri/src)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_definitions(BOOST_BEAST_USE_STD_STRING_VIEW)

add_executable(${PROJECT_NAME} main.cpp Network/NetworkManager.cpp Network/HttpSession.cpp VK/Objects/VKUser.hpp VK/API/VKLongPoll.hpp)

set(BOOST_COMPONENTS beast)
set(BOOST_VERSION)

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /await")
	
	set(Boost_USE_STATIC_LIBS ON)
	set(OPENSSL_USE_STATIC_LIBS TRUE)

	find_package(Boost ${BOOST_VERSION} COMPONENTS ${BOOST_COMPONENTS} PATHS C:/Boost/lib/cmake/Boost-1.71.0)
elseif (UNIX)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")
	set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -O3 -g0 -DNDEBUG -s")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -mtune=skylake -fPIE")

	find_package(Boost)
endif ()
#set(OPENSSL_ROOT_DIR openssl)
#set(OPENSSL_SSL_LIBRARY "/usr/lib/x86_64-linux-gnu/")
#set(OPENSSL_CRYPTO_LIBRARY /usr/lib/x86_64-linux-gnu/libcrypt.so)
find_package(OpenSSL)

#message(${OPENSSL_LIBRARIES})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIR})

#target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} network-uri)

if (MSVC)
	target_link_libraries(${PROJECT_NAME} crypt32)
elseif (UNIX)
	target_link_libraries(${PROJECT_NAME} pthread dl ssl crypto network-uri)
endif ()