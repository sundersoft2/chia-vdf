#include "include.h"

#define COMPILE_ASM

#include "headers.h"

int main(int argc, char** argv) {
    using namespace simd_integer_namespace;

    set_rounding_mode();

    compile_asm();
}