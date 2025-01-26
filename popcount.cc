#include <iostream>
#include <bitset> // 仅用于演示

int countOnes(int n) {
    return __builtin_popcount(n); // GCC/Clang 内置函数
}

int main() {
    int n = 29; // 二进制: 11101
    std::cout << "Number of 1s in " << n << " is: " << countOnes(n) << std::endl;
    return 0;
}
// /opt/homebrew/opt/llvm/bin/clang++ -Wall -Werror -std=c++20 popcount.cc