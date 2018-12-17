sudo apt-get install libgmp3-dev
g++ -o compile_asm.o -c -std=c++1z -ggdb -Wnon-virtual-dtor -Wreturn-type -Wno-deprecated -Winit-self -Wuninitialized -Winvalid-pch -march=native -O3 -I. -I/usr/include -I3-shared compile_asm.cpp
g++ -o compile_asm compile_asm.o -L. -L/lib -lgmpxx -lgmp
./compile_asm
as -o asm_compiled.o asm_compiled.s
g++ -o vdf.o -c -std=c++1z -ggdb -Wnon-virtual-dtor -Wreturn-type -Wno-deprecated -Winit-self -Wuninitialized -Winvalid-pch -march=native -O3 -I. -I/usr/include -I3-shared -I/opt/cuda/include vdf.cpp
g++ -o vdf vdf.o asm_compiled.o -L. -L/lib -lgmpxx -lgmp
