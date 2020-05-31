#include <iostream>

#include "tensor.hpp"

int main() {
    tensor<double> hoge;

    hoge() = 10;

    std::cout << hoge() << std::endl;

    return 0;
}

