#pragma once
#include <bitset>
#include <random>

//Function to create own key sequence
void sequenceGenerator(uint8_t* sequence);

//4 tests of FIPS-140
bool monobitTest(uint8_t* sequence);
bool maxRunsLengthTest(uint8_t* sequence);
bool pokerTest(uint8_t* sequence);
bool runsLengthTest(uint8_t* sequence);