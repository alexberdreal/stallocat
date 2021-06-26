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
	Allocator allocator;
	allocator.alloc(8);
	int32_t** a = allocator.construct<int32_t>(1);
	int32_t** b = allocator.construct<int32_t>(2);
	std::cout << &(**a) << '\t' << &(**b) << std::endl;
	allocator.destroyDownTo(*b);
	int32_t** c = allocator.construct<int32_t>(3);
	std::cout << &(**a) << '\t' << &(**b) << '\t' << &(**c) << std::endl;
	system("pause");
}