cmake_minimum_required (VERSION 3.8)

project ("VKBot")

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp Network/NetworkManager.cpp Network/HttpSession.cpp Utils/Uri.cpp Utils/Uri.h)

set(BOOST_COMPONENTS beast)
set(BOOST_VERSION 1.71.0)

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /await")
	
	set(Boost_USE_STATIC_LIBS ON)
	set(OPENSSL_USE_STATIC_LIBS TRUE)

	find_package(Boost ${BOOST_VERSION} COMPONENTS ${BOOST_COMPONENTS} PATHS C:/Boost/lib/cmake/Boost-1.71.0)
elseif (UNIX)
	find_package(Boost ${BOOST_VERSION} COMPONENTS ${BOOST_COMPONENTS})
endif ()

find_package(OpenSSL)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES})

if (MSVC)
	target_link_libraries(${PROJECT_NAME} crypt32)
elseif (UNIX)
	target_link_libraries(${PROJECT_NAME} pthread dl)
endif ()