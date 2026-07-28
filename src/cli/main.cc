#include "main.hh"
#include <iostream>
#include "mystarceiling/core/foo.hh"

int main(int argc, char** argv)
{
    if (argc > 1)
        std::cout << argv[0];
    std::cout << "No args";
    return mystarceiling::core::foo();
}
