#include <iostream>
#include <sodium.h>

int main() 
{
    if (sodium_init() < 0) {
        std::cout << "Hiba: A libsodiumot nem sikerult inicializalni!" << std::endl;
        return 1;
    }
    std::cout << "Siker! A libsodium elindult, verzio: " << sodium_version_string() << std::endl;
    return 0;
}