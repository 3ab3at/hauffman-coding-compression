#include<iostream>
#include<fstream>
#include<bits/stdc++.h>
#include"binary1.cpp"

using namespace std;


int binaryToDecimal(string n)
{
    // Initializing base value to 1, i.e 2^0
    int sum = 0;

    int p = 0;
    for(int i = 7; i >= 0; i--)
    {
        sum += ((int)n[i] - (int)'0') * pow(2, p);
        p++;
    }

    return sum;
}

string decimalToBinary(int n)
{
    // Initializing base value to 1, i.e 2^0
    string sum = "";

    for(int i = 0; i < 8; i++)
    {
        sum += to_string(n%2);
        n /= 2;
    }

    return sum;
}

void write_to_binary(string byte, ofstream &fp) {
    int8_t b = binaryToDecimal(byte);
    fp.write(reinterpret_cast<const char *>(&b), sizeof(b));
}
/*
void read_from_binary()
{
    char buffer[100];
    ifstream myFile ("data.bin", ios::in | ios::binary);
    myFile.read (buffer, 100);
    if (!myFile) {
        // An error occurred!
        // myFile.gcount() returns the number of bytes read.
        // calling myFile.clear() will reset the stream state
        // so it is usable again.
    }
    if (!myFile.read (buffer, 100)) {
        // Same effect as above
    }
}
*/
int main() {
    ofstream fp;
    fp.open("output.bin",ios::out | ios :: binary | ios::app);
    write_to_binary("01010101", fp);
    write_to_binary("10101010", fp);
    fp.close();

    ifstream in;
	in.open("output.bin", ios::in | ios::binary);
    if (!in.is_open()) {
		cerr << "Error in open file './in.bin'" << endl;
		return 1;
	}

	// read integer - '111' in hex is '6f'
	int num;
    while(in.read((char*)&num, 1))
    {
        cout << "Integer: " << decimalToBinary(num) << endl;
    }

	// close
	in.close();

	return 0;
}