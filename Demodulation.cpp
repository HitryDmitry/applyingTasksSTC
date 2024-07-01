#include <fstream>
#include <vector>
#include <complex>
#include <iostream>


int main(){
    std::ifstream ifs("DemodulationTask/am_sound.dat", std::ios::binary | std::ios::in);
    std::vector<std::complex<float>> v(3);
    ifs.read(reinterpret_cast<char*>(v.data()), 6*sizeof(__int32));
    ifs.close();

    std::cout << v[0] << " " << v[1] << " " << v[2] << std::endl;
    // for (auto c_value: v){
    //     std::cout << c_value.real() << " " << c_value.imag() << std::endl;
    // }

}