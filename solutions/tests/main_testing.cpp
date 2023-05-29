#include "../src/common.hpp"
#define DISP_ERROR std::cout << "\033[1;31m" << __FUNCTION__ << " failed on line " << __LINE__ << "\033[0m" << std::endl
#define DISP_SUCCESS std::cout << "\033[1;32m" << __FUNCTION__ << " Success !" << "\033[0m" << std::endl
#define IS_TRUE(x) { if (!(x)) DISP_ERROR; else DISP_SUCCESS; }

bool _is_full_of_true(bool *array, int size) {
    // Use the all_of algorithm to check if all elements in the array are true
    return all_of(array, array + size, [](bool x){ return x; });
}

// ~> stitch <~
int main() {
   cout << "Hello testing ! :) Gl boooy" << endl;
}
