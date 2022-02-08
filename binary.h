#ifndef BINARY_H
#define BINARY_H

#include "bits/stdc++.h"

using namespace std;

class binary
{
    public:
        int binaryToDecimal(string n);
        string decimalToBinary(int n);
        void write_to_binary(string byte, ofstream &fp);
        void encoder(unordered_map <char, string> codes);
        void decoder(unordered_map<string, char> chars);
};

#endif