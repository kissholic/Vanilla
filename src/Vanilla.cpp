/*
 * File: Vanilla.cpp
 * Author: kissholic
 * Copyright (c) 2023 kissholic. All wrongs reserved.
 */


#include "Core/Coroutine/generator.hpp"

#include <iostream>

co_generator<int> test()
{
    int i = 0;
    for (int i = 0; i < 10; i++)
        co_yield i;
}


int main(int argc, char* argv[]) {
    for (auto i : test())
        std::cout << i << std::endl;
    return 0;
}