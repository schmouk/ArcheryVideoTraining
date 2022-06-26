// avt_cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int main()
{
#if defined(_UNIT_TESTS)
    std::cout << "--- UNIT TESTS - Archery Video Training ---\n\n";
#else
    std::cout << "--- ARCHERY VIDEO TRAINING ---\n\n";
#endif
}
