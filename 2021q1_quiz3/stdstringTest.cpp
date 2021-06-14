#include <iostream>
#include <string>
#include <cstdlib>

void *operator new(std::size_t n) {
	std::cout << "Allocating " << n << " bytes";
	return malloc(n);
}

void operator delete(void *p) throw() {
	free(p);
}

int main(int argc, const char *argv[]) {
	std::cout << "sizeof(std::string): " << sizeof(std::string) << std::endl;
	for (int i = 0; i < 32; i++) {
		std::cout << i << ": " << std::string(i, '=') << std::endl;
	}

	return 0;
}
