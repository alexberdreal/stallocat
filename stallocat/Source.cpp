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
	allocator.alloc(12);
	Cls** cls1(allocator.construct<Cls>(1, 2, 3));
	std::cout << *cls1 << std::endl;
	Cls** cls2 = allocator.construct<Cls>(2, 4, 5);
	std::cout << *cls1 << '\t' << *cls2 << std::endl;
	Cls** cls3 = allocator.construct<Cls>(3, 8, 8);
	Cls** cls4 = allocator.construct<Cls>(4, 1, 1);
	std::cout << *cls1 << '\t' << *cls2 << '\t' << *cls3 << '\t' << *cls4 << std::endl;
	Cls** cls5 = allocator.construct<Cls>(5, 1, 1);
	//allocator.destroyDownTo(cls3);
	std::cout << "HERE";
}