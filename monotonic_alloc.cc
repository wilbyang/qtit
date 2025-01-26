#include <iostream>
#include <cstdlib>  // For std::malloc, std::free

class MonoAllocator {
public:
    // 构造函数：分配一块大内存
    MonoAllocator(size_t size) : buffer_size(size), offset(0) {
        buffer = (char*) std::malloc(buffer_size);
    }

    // 析构函数：释放分配的内存
    ~MonoAllocator() {
        std::free(buffer);
    }

    // 分配内存
    void* allocate(size_t size) {
        if (offset + size > buffer_size) {
            throw std::bad_alloc();
        }
        void* ptr = buffer + offset;
        offset += size;
        return ptr;
    }

    // 重置分配器（释放所有内存，但不回收）
    void reset() {
        offset = 0;
    }

private:
    char* buffer;     // 指向分配的内存块
    size_t buffer_size;  // 内存块的大小
    size_t offset;     // 当前的分配位置
};

// 使用示例
int main() {
    MonoAllocator allocator(1024);  // 分配1KB的内存池

    // 分配整数数组
    int* arr = (int*)allocator.allocate(10 * sizeof(int));
    for (int i = 0; i < 10; ++i) {
        arr[i] = i;
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    // 重置分配器以复用内存
    allocator.reset();

    // 再次分配
    int* arr2 = (int*)allocator.allocate(5 * sizeof(int));
    for (int i = 0; i < 5; ++i) {
        arr2[i] = i * 2;
        std::cout << arr2[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
//g++ -std=c++17 monotonic_alloc.cc
