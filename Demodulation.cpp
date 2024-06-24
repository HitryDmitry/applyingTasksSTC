#include <iostream>
#include <ifstream>


int main(){
    std::ifstream ifs("file.dat", std::ios::binary | std::ios::in);
    std::vector<std::complex<float>> v(3);
    ifs.read(reinterpret_cast<char*>(v.data()), 6*sizeof(float));
    ifs.close();
}