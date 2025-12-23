cmake_minimum_required(VERSION "3.31.6")

project("CppPasswordManager")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSODIUM REQUIRED libsodium)

add_executable("CppPasswordManager" 
    "src/main.cpp" 
    "src/crypto/Crypto.cpp" 
    "src/core/Entry.cpp" 
    "src/storage/Storage.cpp" 
    "src/cli/CommandParser.cpp" 
    "src/core/Vault.cpp"
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSODIUM REQUIRED libsodium)

target_include_directories("CppPasswordManager" PRIVATE 
    "src/core"
    "src/crypto"
    "src/storage"
    "include"
    "${LIBSODIUM_INCLUDE_DIRS}"
    "${sodium_INCLUDE_DIRS}")


target_link_libraries("CppPasswordManager" PRIVATE ${LIBSODIUM_LIBRARIES})
target_link_directories("CppPasswordManager" PRIVATE ${LIBSODIUM_LIBRARY_DIRS})


if(MSVC)
    target_compile_options("CppPasswordManager" PRIVATE /W4)
endif()
