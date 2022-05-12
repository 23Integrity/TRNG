#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
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
    std::string result = (exec("sudo /usr/bin/powermetrics -s cpu_power -n 1 | grep \"CPU 1 frequency\"")).substr(17, 4);
    return std::stoi(result) * 1000;
}

std::string Rando(int amountOfBits) {
    int i = 1;
    unsigned int bits[8];
    std::string word = "";

    while (i < amountOfBits) {
        std::string k = std::to_string(getFreq());
        int b = getFreq();
        int l = k.length();
        uint32_t d = MurmurHash1(&k, l, b);
        bits[i] = (d & 1);
        i++;
    }
    for (int j = 0; j < amountOfBits; j++) {
        word += std::to_string(bits[j]);
    }
    return word;
}

int main() {
    std::cout << Rando(8) << std::endl;
    return 0;
}
