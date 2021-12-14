#pragma once

#include <string>
#include <iostream>
#include <cstdlib>

// ret can take in a pointer of a boolean
// nullptr is interpreted as false and anything else is true
void check_init(bool ret, const std::string& event)
{
    if (ret)
        return;

    std::cout << "[INIT ERROR]: " << event << '\n';
    exit(EXIT_FAILURE);
}