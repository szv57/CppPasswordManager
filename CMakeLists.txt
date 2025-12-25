cmake_minimum_required(VERSION "3.31.6")
project("CppPasswordManager")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Catch2 REQUIRED)

find_library(SODIUM_LIB NAMES sodium libsodium REQUIRED)
find_path(SODIUM_INCLUDE_DIR NAMES sodium.h REQUIRED)


add_executable("CppPasswordManager" 
    "src/main.cpp" 
    "src/crypto/Crypto.cpp" 
    "src/core/Entry.cpp" 
    "src/storage/Storage.cpp" 
    "src/cli/CommandParser.cpp" 
    "src/core/Vault.cpp"
    "src/PasswordGenerator/PasswordGenerator.cpp"
)
target_include_directories("CppPasswordManager" PRIVATE "src/core" "src/crypto" "src/storage" "src/cli" "src/PasswordGenerator" "includes" ${SODIUM_INCLUDE_DIR})
target_link_libraries("CppPasswordManager" PRIVATE ${SODIUM_LIB})

add_executable("test" 
    "test/src/crypto/test_Crypto.cpp" 
    "src/crypto/Crypto.cpp"
)
target_compile_definitions("test" PRIVATE SODIUM_STATIC=1)

target_include_directories("test" PRIVATE 
    "src/crypto" 
    "include" 
    ${SODIUM_INCLUDE_DIR}
)

target_link_libraries("test" PRIVATE ${SODIUM_LIB})

if(MSVC)
    target_compile_options("CppPasswordManager" PRIVATE /W4)
    target_compile_options("test" PRIVATE /W4)
endif()
