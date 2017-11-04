#include <iostream>
#include <iomanip>

using namespace std;

void hex_dump(void* data, size_t size){
    char* p = (char*) data;

    std::cout << std::hex << std::setfill('0');

    int i = 0;
    while (i < size) {
        std::cout << (void*) (p + i) << ' ';

        int j = 0;
        for (j = 0; j < 16 && (i + j < size); ++j) {
            if (j % 8 == 0) {
                std::cout << ' ';
            }
            std::cout << std::setw(2) << ((unsigned int) (p[j + i]) & 0xff) << ' ';
        }

        if (j < 16) {
            for (int k = 0; k < 16 - j; ++k) {
                if ((j + k) % 8 == 0) {
                    std::cout << ' ';
                }
                std::cout << std::setw(2) << "xx" << ' ';
            }
            std::cout << ' ';
        }

        for (int k = 0; k < j; ++k) {
            if (std::isprint(p[k + i])) {
                std::cout << p[k + i];
            }
            else {
                std::cout << '.';
            }
        }

        std::cout << std::endl;

        i += j;
    }
}


int main() {
    hex_dump((void*)main, sizeof(void*));
    return 0;
}