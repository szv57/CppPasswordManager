#include <iostream>
#include <sodium.h>

int main() 
{
    if (sodium_init() < 0) {
        std::cerr << "ERROR: Failed to initialize Sodium." << std::endl;
        return 1;
    }
    return 0;
}