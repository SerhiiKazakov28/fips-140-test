#include <iostream>
#include <iomanip>
#include "src/FIPS140_test.h"

using namespace std;

int main()
{
    uint8_t sequence[2500]{};
    sequenceGenerator(sequence);
    cout << "Generated sequence:\n";
    for (uint8_t c : sequence) {
        cout << setw(2) << setfill('0') << hex << (int)c;
    }
    cout << endl << "\n*-------------*\nSequence test:\n";
    cout << "The Monobit Test:\t\t";
    if (monobitTest(sequence)) cout << "completed\n";
    else "failed\n";
    cout << "The Maximum run length Test:\t";
    if (maxRunsLengthTest(sequence)) cout << "completed\n";
    else " failed\n";
    cout << "The Poker Test:\t\t\t";
    if (pokerTest(sequence)) cout << "completed\n";
    else "failed\n";
    cout << "The Runs length Test:\t\t";
    if (runsLengthTest(sequence)) cout << "completed\n";
    else "failed\n";

    return 0;
}