extern "C" {
  int add (int a, int b) {
    return a + b;
  }
}
// /opt/homebrew/opt/llvm/bin/clang -S -emit-llvm llvm-ir-add.cc