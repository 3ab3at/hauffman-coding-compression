#include "bits/stdc++.h"

using namespace std;

//convert binary to decimal
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

//convert decimal to binary
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

//write binary byte to a file
void write_to_binary(string byte, ofstream &fp) {
    int8_t b = binaryToDecimal(byte);
    fp.write(reinterpret_cast<const char *>(&b), sizeof(b));
}

//get the probability table
unordered_map <char, float> getProb(string path)
{
    unordered_map <char, float> prob;

    ifstream in(path);
    if (!in.is_open())
    {
        cout<< "Error loading the text file. \n";
    }

    char ch;
    int n=0;
    while (in.get(ch)) 
    {
        prob[ch]++;
        n++;
    }
    for(auto &it:prob)
    {
        it.second /= n;
    }

    in.close();

    return prob;
}

//data structure for a node in huffman tree
struct hNode
{
    string code;
    string data;
    float prob;
    bool leaf;
    hNode *right;
    hNode *left;
    hNode()
    {
        code = "";
        left = right = nullptr;
        data = "";
        prob = 0;
        leaf = false;
    }
    hNode(string d, float p)
    {
        code = "";
        left = right = nullptr;
        data = d;
        prob = p;
        leaf = false;
    }
};

//for comaring two nodes in the priority queue with their probabilities
struct node_cmp
{
   bool operator()( const hNode a, const hNode b ) const 
   {
    return a.prob > b.prob;
   }
};

//get the codes from the tree
void get_codes(hNode* node, char x, string prevCode, unordered_map <char, string> &codes, unordered_map <string, char> &chars)
{

    if (node == NULL)
        return;

    if (x == 'r')
        node->code = prevCode + '1';
    else if (x=='l')
        node->code = prevCode + '0';


    get_codes(node->left, 'l', node->code, codes, chars);

    if (node->leaf == true)
    {
        codes[node->data[0]] = node->code;
        chars[node->code] = node->data[0];
    }

    get_codes(node->right, 'r', node -> code, codes, chars);

}

//encode the input file to binary file
void encoder(unordered_map <char, string> codes, string path)
{
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

    if(s.length() > 0)
    {
        int i = 0;
        while(s.length() < 8)
        {
            if(i == codes[' '].size())
            {
                i = 0;
            }
            s += codes[' '][i];
            i++;
        }
        write_to_binary(s, fp);
    }


    in.close();
    fp.close();
}

//decode the binary file into a text file
void decoder(unordered_map<string, char> V)
{
    string path ("output.bin");
    ifstream in(path);
    ofstream out("outstring.txt");
    if (!in.is_open())
    {
        cout<< "Error loading the text file. \n";
    }

    int num;
    char c;
    string code = "";
    while (in.read((char*)&num, 1)) 
    {
        string byte = decimalToBinary(num);
        
        for(int i = 7; i >= 0; i--)
        {
            code += byte[i];
            if(V.find(code) != V.end())
            {
                out << V[code];
                code = "";
            }
        }
    }

    in.close();
    out.close();
}

//creation of huffman tree & extract the codes (i.e. encode an decode included)
void huffmanCoding(string path)
{
    unordered_map <char, float> prob = getProb(path);
    priority_queue<hNode, vector<hNode>, node_cmp> huffmanTree;
    unordered_map<string, hNode> mytree;

    for(auto &it: prob)
    {
        hNode tmp(string(1, it.first), it.second);
        mytree[string(1, it.first)] = tmp;
        tmp.leaf = true;
        huffmanTree.push(tmp);
    }

    priority_queue<hNode, vector<hNode>, node_cmp> g = huffmanTree;
    while(g.size() > 1)
    {
        hNode tmp;
        string s = "";
        float cost = 0;
        hNode *left = new hNode;
        *left = g.top();
        tmp.left = left;
        s += g.top().data;
        cost += g.top().prob;
        g.pop();
        hNode *right = new hNode;
        *right = g.top();
        tmp.right = right;
        s += g.top().data;
        cost += g.top().prob;
        g.pop();
        tmp.data = s;
        tmp.prob = cost;
        g.push(tmp);
    }

    hNode root;
    root = g.top();

    unordered_map <char, string> codes;
    unordered_map <string, char> chars;
    get_codes(&root, 'x', "", codes, chars);

    encoder(codes, path);
    decoder(chars);
}


int main()
{
    remove("output.bin");
    remove("outstring.txt");
    huffmanCoding("input.txt");
}