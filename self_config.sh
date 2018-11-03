make distclean 2>&1 > /dev/null

./configure --prefix=$(pwd)/objs/

mkdir -p $(pwd)/objs/man/man1/

sed -i 's/gcc/powerpc-linux-gcc/g' $(find ./ -name Makefile)


