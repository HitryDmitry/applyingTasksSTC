#include <fstream>
#include <vector>
#include <complex>
#include <iostream>
#include <cmath>
#include "tinywav/tinywav.h"

#define NUM_CHANNELS 1
#define SAMPLE_RATE 48000
#define BLOCK_SIZE 480

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


int main(){
    std::ifstream ifs("DemodulationTask/am_sound.dat", std::ios::binary | std::ios::in);
    std::vector<std::complex<__int32>> complexNumbers(409600);
    ifs.read(reinterpret_cast<char*>(complexNumbers.data()), 2*complexNumbers.size()*sizeof(__int32));
    ifs.close();

    std::vector<int> modulatingSignal(0);
    for (auto complexNumber: complexNumbers){
        // добавляем в вектор, содержащий отсчеты сигнала, модуль комплексного отсчета, 
        // умноженный на 10^-5 и вычитаем постоянную составляющую (3.2)
        modulatingSignal.push_back(abs(complexNumber)*pow(10, -5)-3.2);
    }

    // добавляем фильтр скользящего среднего
    int windowSize = 10;
    std::vector<float> a = {1};
    // создаем массив коэффициентов b
    std::vector<float> b = {};
    // заполняем его единицами
    for (int index = 0; index < windowSize; ++index) b[index] = 1;
    // применяем фильтр
    std::vector<float> Out = filter(b, a, modulatingSignal);
     
    // записываем в файл "Demodulated_AM.wav"
    TinyWav tw;
    tinywav_open_write(&tw,
        NUM_CHANNELS,
        SAMPLE_RATE,
        TW_FLOAT32, // the output samples will be 32-bit floats. TW_INT16 is also supported
        TW_INLINE,  // the samples to be written will be assumed to be inlined in a single buffer.
                    // Other options include TW_INTERLEAVED and TW_SPLIT
        "AM_demodulation_output.wav" // the output path
    );

    for (int i = 0; i < 1000; i++) {
    // NOTE: samples are always expected in float32 format, 
    // regardless of file sample format

    float samples[BLOCK_SIZE * NUM_CHANNELS];
    for (int j = 0; j < BLOCK_SIZE * NUM_CHANNELS; ++j) samples[i] = Out[i*1000 + j];
    tinywav_write_f(&tw, samples, BLOCK_SIZE);
    }
    
    tinywav_close_write(&tw);
}