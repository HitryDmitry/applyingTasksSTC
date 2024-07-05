#include <fstream>
#include <vector>
#include <complex>
#include <iostream>
#include <cmath>
#include <numeric>
#include <algorithm>
#include "tinywav/tinywav.h"

#define NUM_CHANNELS 1
#define SAMPLE_RATE 48000
#define BLOCK_SIZE 480

std::vector<float> downsample(std::vector<float> inputSignal, int sampleRate){
    std::vector<float> downsampledSignal(0);
    for (int i = 0; i < inputSignal.size(); i += sampleRate){
        downsampledSignal.push_back(inputSignal[i]);
    }
    return downsampledSignal;
}

std::vector<std::complex<float>> downsample(std::vector<std::complex<float>> inputSignal, int sampleRate){
    std::vector<std::complex<float>> downsampledSignal(0);
    for (int i = 0; i < inputSignal.size(); i += sampleRate){
        downsampledSignal.push_back(inputSignal[i]);
    }
    return downsampledSignal;
}

float getCoefficientsOfButterworthFilter(std::vector<float>& B, std::vector<float>& A, float fc, float fs){
    float cyclicCutOffFreq = 2*M_PI*fc/fs;
    float alpha = tan(cyclicCutOffFreq/2);

    float gain = alpha*alpha/(1 + sqrt(2)*alpha + alpha*alpha);
    A[0] = 2*(alpha*alpha - 1)/(1+sqrt(2)*alpha + alpha*alpha);
    A[1] = (1+sqrt(2)*alpha + alpha*alpha)/(1+sqrt(2)*alpha + alpha*alpha);
    B[0] = 2*gain;
    B[1] = gain;

    return gain;
}


// direct form II transposed
std::vector<float> filter(std::vector<float> B, std::vector<float> A, std::vector<int>& inputSignal){
    int lengthOfA = A.size();
    int filterState[lengthOfA] = {};

    for (auto elemOfB:B) elemOfB /= A[0];
    for (auto elemOfA:B) elemOfA /= A[0];

    std::vector<float> Out(inputSignal.size());
    for (int i = 0; i < Out.size(); ++i){
        Out[i] = B[0] * inputSignal[i] + filterState[0];
        for(int j = 1; j < lengthOfA; ++j){
            filterState[j-1] = B[j] * inputSignal[i] + filterState[j] - A[j] * Out[i];
        }
    }
    return Out;
}

// std::complex<float> operator*(std::complex<int> complexNumber, float number){
//     return std::complex<float> ((float)complexNumber.real()*number, (float)complexNumber.real()*number)
// }


int main(){
    // std::ifstream ifs("DemodulationTask/file1EuropaPlus.bin", std::ios::binary | std::ios::in);
    std::ifstream ifs("DemodulationTask/file1EuropaPlus.bin", std::ios::binary | std::ios::in);

    int lengthInBytes = 0;
    if (ifs) {
    // get length of file:
    ifs.seekg (0, ifs.end);
    lengthInBytes = ifs.tellg();
    ifs.seekg (0, ifs.beg);
  }
    std::vector<std::complex<float>> complexNumbers(lengthInBytes/sizeof(float)/2);
    ifs.read(reinterpret_cast<char*>(complexNumbers.data()), 2*lengthInBytes/sizeof(float));
    ifs.close();

    // // создаем такой же вектор complexNumbers, но с float
    // std::vector<std::complex<float>> cNumbersFloat(complexNumbers.size());
    // for (auto complexNumber: complexNumbers) cNumbersFloat.push_back(std::complex<float> (complexNumber.real(), complexNumber.imag()));

    // Получаем коэффициенты и фильтруем
    std::vector<float> B(2);
    std::vector<float> A(2);
    float gain = getCoefficientsOfButterworthFilter(B, A, 50000, 500000);
    std::vector<std::complex<float>> filteredSignal(complexNumbers.size());
    for (int i = 2; i < complexNumbers.size(); ++i){
        filteredSignal[i] = gain*complexNumbers[i] + B[0]*complexNumbers[i - 1] + B[1]*complexNumbers[i - 2] - A[0]*filteredSignal[i - 1] - A[1]*filteredSignal[i - 2];
    }
    std::cout << *std::max_element(filteredSignal.begin(), filteredSignal.end()) << std::endl;

    // Осуществляем downsampling
    complexNumbers = downsample(complexNumbers, 5);

    // Процесс демодуляции
    std::vector<float> modulatingSignal(complexNumbers.size());
    for (int i = 0; i < complexNumbers.size() - 1; ++i){
        std::complex<int> multiplication = complexNumbers[i]*std::conj(complexNumbers[i + 1]);
        modulatingSignal[i] = atan2(multiplication.imag(), multiplication.real());
    }

    // // Получаем коэффициенты и фильтруем
    // std::vector<float> B(2);
    // std::vector<float> A(2);
    // float gain = getCoefficientsOfButterworthFilter(B, A, 22728, 45455);
    // std::vector<float> filteredSignal(complexNumbers.size());
    // for (int i = 2; i < modulatingSignal.size(); ++i){
    //     filteredSignal[i] = gain*modulatingSignal[i] + B[0]*modulatingSignal[i - 1] + B[1]*modulatingSignal[i - 2] - A[0]*filteredSignal[i - 1] - A[1]*filteredSignal[i - 2];
    // }
    // std::cout << *std::max_element(filteredSignal.begin(), filteredSignal.end()) << std::endl;
    
    // // Выполняем downsampling еще раз
    // std::vector<float> downsampledSignal = downsample(filteredSignal, 5);


    // std::complex<int> c1 = {1, 1};
    // std::complex<int> c2 = {1, 1};
    // std::complex<int> c3 = c1*std::conj(c2);
    // std::cout << atan2(c3.imag(), c3.real());

    // // вычисляем среднее
    // float average = std::accumulate(modulatingSignal.begin(), modulatingSignal.end(), 0.0) / modulatingSignal.size();

    // // Проходим по массиву и вычитаем среднее из каждого элемента
    // for (int i = 0; i < modulatingSignal.size(); ++i){
    //     modulatingSignal[i] /= average;
    // }


    // добавляем фильтр скользящего среднего
    // int windowSize = 10;
    // std::vector<float> a = {1};
    // // создаем массив коэффициентов b
    // std::vector<float> b = {};
    // // заполняем его единицами
    // for (int index = 0; index < windowSize; ++index) b[index] = 1;
    // // применяем фильтр
    // std::vector<float> Out = filter(b, a, modulatingSignal);
}