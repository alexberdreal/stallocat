#include <iostream>
#include <vector>
#include "Allocator.h"
using namespace stalloc;

class Cls {
public:
	Cls() = delete;
	Cls(int a, int b, int c) : a(a), b(b), c(c) {
		std::cout << a << "\t" << b << "\t" << c << "\n";
	}
	~Cls() { std::cout << "dest" << &a << '\n'; }
	int a;
	int b;
	int c;
};

int main() {
	stalloc::StackAllocatorRealloc allocator;
	allocator.allocate(8);                                  // some space for 2 integers           
	int32_t** a = allocator.construct<int32_t>(1);          // (1) &(**a) == 0x013CF418 
	int32_t** b = allocator.construct<int32_t>(2);          // (2) &(**a) == 0x013CF418, &(**b) == 0x013CF41C
	int32_t** c = allocator.construct<int32_t>(3);          // (3) &(**a) == 0x013C59E0, &(**b) == 0x013C59E4, &(**c) == 0x013C59E8
	allocator.destroyDownTo(*a);                            // (4) delete 1, 2, 3  
}