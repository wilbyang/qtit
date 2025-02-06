#include <iostream>
#include <concepts>


class AnimalTag {};

template <typename T>
concept Animal = requires (T animal) {
  animal.make_sound();
} && std::derived_from<T, AnimalTag>;

template <Animal T>
void print(T const& animal) {
  animal.make_sound();
}

class Dog : public AnimalTag {
public:
  void make_sound() const {
    std::cout << "Woof!" << std::endl;
  }
};

int main() {
  Dog dog;
  print(dog);
  return 0;
}

// /opt/homebrew/opt/llvm/bin/clang++ -Wall -Werror -std=c++20 concept.cc -o concept