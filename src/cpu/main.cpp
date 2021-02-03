#include <iostream>
#include "stack.h"

int main(int argc, char** argv) {
    stack stack;
    std::cout << stack.size() << ' ' << stack.empty() << '\n';
    for (int i = 0; i < 123; i++) {
        stack.push(i % 10 + '0');
        std::cout << stack.top() << '\n';
    }
    std::cout << stack.size() << ' ' << stack.empty() << ' ' << stack.pop() << ' ' << stack.top() << '\n';
    return 0;
}
