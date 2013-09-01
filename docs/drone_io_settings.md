mkdir gtest
pushd gtest
wget http://googletest.googlecode.com/files/gtest-1.6.0.zip
unzip gtest-1.6.0.zip
pushd gtest-1.6.0
cmake -DCMAKE_BUILD_TYPE=Release
make
sudo cp libgtest* /usr/lib
sudo mkdir /usr/include/gtest
sudo cp -r include/gtest/* /usr/include/gtest
popd
popd


cmake -DCMAKE_BUILD_TYPE=Release ./
make
make test
./bin/libbatchringbuffer_test

echo "Valgrind - libbatchringbuffer tests"

sudo apt-get install valgrind
valgrind ./bin/libbatchringbuffer_test
valgrind --tool=helgrind ./bin/libbatchringbuffer_test
valgrind --tool=cachegrind ./bin/libbatchringbuffer_test

echo "Valgrind - example.c"

valgrind ./bin/example_c
valgrind --tool=helgrind ./bin/example_c
valgrind --tool=cachegrind ./bin/example_c

valgrind --tool=callgrind --dsymutil=yes ./bin/example_c
for outfile in callgrind.out.*; do
    callgrind_annotate --auto=yes $outfile
done