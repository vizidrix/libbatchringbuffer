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