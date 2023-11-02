#include "FIPS140_test.h"

//Genetating 32-bit random values and paste them to sequence
void sequenceGenerator(uint8_t* sequence) {
    unsigned random4byte;
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<unsigned> distribution(0, 0xFFFFFFFF);
    for (size_t i = 0; i < 625; i++) {
        random4byte = distribution(generator);
        sequence[i * 4] = random4byte >> 24;
        sequence[i * 4 + 1] = random4byte >> 16 & 0xFF;
        sequence[i * 4 + 2] = random4byte >> 8 & 0xFF;
        sequence[i * 4 + 3] = random4byte & 0xFF;
    }
}

//Count zero bits in sequence and compare with acceptance region
bool monobitTest(uint8_t* sequence) {
    std::bitset<8> byte;
    size_t countZeros = 0;
    for (size_t i = 0; i < 2500; i++) {
        byte = sequence[i];
        countZeros += byte.count();
    }
    if (countZeros > 9654 && countZeros < 10346) return true;
    return false;
}

//Check if sequence contains runs of 0 or 1 longer than 36 bits
bool maxRunsLengthTest(uint8_t* sequence) {
    short runsLength = 0;
    std::bitset<8> byte;
    bool currentBit = false;
    for (size_t i = 0; i < 2500; i++) {
        byte = sequence[i];
        for (int j = 7; j >= 0; j--) {
            if (byte[j] != currentBit) {
                currentBit = !currentBit;
                runsLength = 1;
            }
            else runsLength++;
        }
        if (runsLength > 36) return false;
    }
    return true;
}

/*Count the number of occurencies of each possible 4-bit pattterns
  in sequence, calculating the value using the formula and 
  compare with acceptance region*/
bool pokerTest(uint8_t* sequence) {
    size_t sequences4b[16]{};
    int seq;
    double result = 0;
    for (size_t i = 0; i < 2500; i++) {
        seq = sequence[i] >> 4;
        sequences4b[seq]++;
        seq = sequence[i] & 0b1111;
        sequences4b[seq]++;
    }
    for (size_t s : sequences4b) {
        result += s * s;
    }
    result = result * 16.0 / 5000.0 - 5000.0;
    if (result > 1.03 && result < 57.4) return true;
    return false;
}

/*Tests for each run length of 1, 2, 3, 4, 5 and 6+
  in sequence and compare with acceptance region table*/
bool runsLengthTest(uint8_t* sequence) {
    size_t sequences0[6]{}, sequences1[6]{}, runsLength = 0;
    std::bitset<8> byte;
    bool currentBit = false;
    for (size_t i = 0; i < 2500; i++) {
        byte = sequence[i];
        for (int j = 7; j >= 0; j--) {
            if (byte[j] != currentBit) {
                if (currentBit) sequences1[(runsLength > 5) ? 5 : runsLength - 1]++;
                else sequences0[(runsLength > 5) ? 5 : runsLength - 1]++;
                currentBit = !currentBit;
                runsLength = 1;
            }
            else runsLength++;
        }
    }
    if (currentBit) sequences1[(runsLength > 5) ? 5 : runsLength - 1]++;
    else sequences0[(runsLength > 5) ? 5 : runsLength - 1]++;
    if (sequences0[0] <= 2267 || sequences0[0] >= 2733 || sequences1[0] <= 2267 || sequences1[0] >= 2733) return false;
    if (sequences0[1] <= 1079 || sequences0[1] >= 1421 || sequences1[1] <= 1079 || sequences1[1] >= 1421) return false;
    if (sequences0[2] <= 502 || sequences0[2] >= 748 || sequences1[2] <= 502 || sequences1[2] >= 748) return false;
    if (sequences0[3] <= 223 || sequences0[3] >= 402 || sequences1[3] <= 223 || sequences1[3] >= 402) return false;
    if (sequences0[4] <= 90 || sequences0[4] >= 223 || sequences1[4] <= 90 || sequences1[4] >= 223) return false;
    if (sequences0[5] <= 90 || sequences0[5] >= 223 || sequences1[5] <= 90 || sequences1[5] >= 223) return false;
    return true;
}