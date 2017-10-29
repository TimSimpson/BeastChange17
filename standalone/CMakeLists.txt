
# A parent project which tests that the main CMakeLists.txt can be used
# as a subproject.
#
# This is used only for CI; in your own project, ignore this and add lp3_core as
# a subproject (nested directory) using Git submodules, ExternalProject_Add or
# something else.
#
# Note that you will need to define the libraries below as seen here yourself.

project(BeastChange17-Parent CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(MSVC)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    add_compile_options(/WX)
    add_compile_options(/bigobj)
else()
    add_definitions("-Wall -pedantic -Werror -Wshadow -Wno-long-long -Wextra")
endif()

set (CMAKE_CXX_STANDARD 14)

enable_testing()

include(ExternalProject)

###############################################################################
#  Third Party libraries
###############################################################################

# If you are using this as a subproject, your super project CMakeLists.txt
# must define the following interface import libraries as seen here.

### OpenSSL

find_package(OpenSSL )

if ("${OPENSSL_FOUND}")
    add_library(lp3_deps::openssl INTERFACE IMPORTED)
    set_property(TARGET lp3_deps::openssl PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}")
    set_property(TARGET lp3_deps::openssl PROPERTY
        INTERFACE_LINK_LIBRARIES "${OPENSSL_LIBRARIES}")
    #message(FATAL_ERROR "${OPENSSL_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "OpenSSL not found - set OPENSSL_ROOT_DIR.")
endif()



### Boost

find_package(Boost 1.65.1 COMPONENTS context coroutine system REQUIRED )

# Disable autolinking - comes from this Stack Overflow question:
# http://stackoverflow.com/questions/6646405/how-do-you-add-boost-libraries-in-cmakelists-txt
# Without this Boost Context / Coroutine would not work on good 'ole Windows.

add_definitions( -DBOOST_ALL_NO_LIB )

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)


add_library(lp3_deps::boost-system INTERFACE IMPORTED)
set_property(TARGET lp3_deps::boost-system PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}")
set_property(TARGET lp3_deps::boost-system PROPERTY
    INTERFACE_LINK_LIBRARIES $<$<CONFIG:Debug>:${Boost_SYSTEM_LIBRARY_DEBUG}> $<$<CONFIG:Release>:${Boost_SYSTEM_LIBRARY_RELEASE}>)

add_library(lp3_deps::boost-context INTERFACE IMPORTED)
set_property(TARGET lp3_deps::boost-context PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}")
set_property(TARGET lp3_deps::boost-context PROPERTY
    INTERFACE_LINK_LIBRARIES $<$<CONFIG:Debug>:${Boost_CONTEXT_LIBRARY_DEBUG}> $<$<CONFIG:Release>:${Boost_CONTEXT_LIBRARY_RELEASE}>)


add_library(lp3_deps::boost-coroutine INTERFACE IMPORTED)
set_property(TARGET lp3_deps::boost-coroutine PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}")
set_property(TARGET lp3_deps::boost-coroutine PROPERTY
    INTERFACE_LINK_LIBRARIES $<$<CONFIG:Debug>:${Boost_COROUTINE_LIBRARY_DEBUG}> $<$<CONFIG:Release>:${Boost_COROUTINE_LIBRARY_RELEASE}>)

message(INFO "MARIO\n\n***${Boost_INCLUDE_DIRS}\n\n***\n")
# Boost Beast

set(DEPS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../build/deps")

add_library(lp3_deps::boost-beast INTERFACE IMPORTED)
set_property(TARGET lp3_deps::boost-beast PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES "${DEPS_PATH}/beast/include")

###############################################################################
#  Subproject
###############################################################################

set(LP3_BEAST_CHANGE_17 true)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/..
                 ${CMAKE_CURRENT_BINARY_DIR}/bc17)
