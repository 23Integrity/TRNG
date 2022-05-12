#include <cstdio>
#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <unistd.h>
#include "src/MurmurHash1.h"

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int getFreq() {
    std::ifstream file {"/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"};
    std::string freq;
    std::getline(file, freq);
    return std::stoi(freq);
}

std::string Rando(int amountOfBits) {
    int i = 0;
    unsigned int bits[8];
    std::string word = "";

    while (i < amountOfBits) {
        std::string k = std::to_string(getFreq());
        int b = getFreq();
        int l = k.length();
        uint32_t d = MurmurHash1(&k, l, b);
        bits[i] = (d & 1);
        i++;
        usleep(10 * 1000);
    }
    for (int j = 0; j < amountOfBits; j++) {
        word += std::to_string(bits[j]);
    }
    return word;
}

int main() {
    std::ofstream file { "results.txt" };
    if (file.is_open()) { 
        for (int i = 0; i < 250000; i++) {
            file << Rando(8) << std::endl;
        }
    }
    else { std::cout << "Unable to open the file\n";}
    file.close();
    return 0;
}
