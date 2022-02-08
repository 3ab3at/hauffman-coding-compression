#include "binary.h"

int binary::binaryToDecimal(string n)
{
    int sum = 0;

    int p = 0;
    for(int i = 7; i >= 0; i--)
    {
        sum += ((int)n[i] - (int)'0') * pow(2, p);
        p++;
    }

    return sum;
}

string binary::decimalToBinary(int n)
{
    string sum = "";

    for(int i = 0; i < 8; i++)
    {
        sum += to_string(n%2);
        n /= 2;
    }

    return sum;
}

void binary::write_to_binary(string byte, ofstream &fp) {
    int8_t b = binaryToDecimal(byte);
    fp.write(reinterpret_cast<const char *>(&b), sizeof(b));
}

void binary::encoder(unordered_map <char, string> codes)
{
    string path ("input.txt");
    ifstream in(path);

    ofstream fp;
    fp.open("output.bin",ios::out | ios :: binary | ios::app);

    if (!in.is_open())
    {
        cout<< "Error loading the text file. \n";
    }
    char ch;
    string s = "";
    while (in.get(ch)) 
    {
        for(int i = 0; i < codes[ch].length(); i++)
        {
            s += codes[ch][i];
            if(s.length() == 8)
            {
                write_to_binary(s, fp);
                s = "";
            }
        }
    }

    in.close();
    fp.close();
}

void binary::decoder(unordered_map<string, char> chars)
{
    string path ("output.bin");
    ifstream in(path);
    ofstream out("outstring.txt");
    if (!in.is_open())
    {
        cout<< "Error loading the text file. \n";
    }
    int num;
    string code = "";
    while (in.read((char*)&num, 1)) 
    {
        string byte = decimalToBinary(num);
        cout << byte << endl;
        
        for(int i = 7; i >= 0; i--)
        {
            code += byte[i];
            if(chars.find(code) != chars.end())
            {
                out << chars[code];
                code = "";
            }
        }
    }

    in.close();
    out.close();
}