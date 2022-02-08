#include "bits/stdc++.h"
using namespace std;

#define MAX_TREE_HT 100
  
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

// A Huffman tree node
struct MinHeapNode {
  
    // One of the input characters
    char data;
  
    // Frequency of the character
    unsigned freq;
  
    // Left and right child of this node
    struct MinHeapNode *left, *right;
};
  
// A Min Heap:  Collection of
// min-heap (or Huffman tree) nodes
struct MinHeap {
  
    // Current size of min heap
    unsigned size;
  
    // capacity of min heap
    unsigned capacity;
  
    // Array of minheap node pointers
    struct MinHeapNode** array;
};
  
// A utility function allocate a new
// min heap node with given character
// and frequency of the character
struct MinHeapNode* newNode(char data, unsigned freq)
{
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(
        sizeof(struct MinHeapNode));
  
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
  
    return temp;
}
  
// A utility function to create
// a min heap of given capacity
struct MinHeap* createMinHeap(unsigned capacity)
  
{
  
    struct MinHeap* minHeap
        = (struct MinHeap*)malloc(sizeof(struct MinHeap));
  
    // current size is 0
    minHeap->size = 0;
  
    minHeap->capacity = capacity;
  
    minHeap->array = (struct MinHeapNode**)malloc(
        minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}
  
// A utility function to
// swap two min heap nodes
void swapMinHeapNode(struct MinHeapNode** a,
                     struct MinHeapNode** b)
  
{
  
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}
  
// The standard minHeapify function.
void minHeapify(struct MinHeap* minHeap, int idx)
  
{
  
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
  
    if (left < minHeap->size
        && minHeap->array[left]->freq
               < minHeap->array[smallest]->freq)
        smallest = left;
  
    if (right < minHeap->size
        && minHeap->array[right]->freq
               < minHeap->array[smallest]->freq)
        smallest = right;
  
    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest],
                        &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}
  
// A utility function to check
// if size of heap is 1 or not
int isSizeOne(struct MinHeap* minHeap)
{
  
    return (minHeap->size == 1);
}
  
// A standard function to extract
// minimum value node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
  
{
  
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
  
    --minHeap->size;
    minHeapify(minHeap, 0);
  
    return temp;
}
  
// A utility function to insert
// a new node to Min Heap
void insertMinHeap(struct MinHeap* minHeap,
                   struct MinHeapNode* minHeapNode)
  
{
  
    ++minHeap->size;
    int i = minHeap->size - 1;
  
    while (i
           && minHeapNode->freq
                  < minHeap->array[(i - 1) / 2]->freq) {
  
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
  
    minHeap->array[i] = minHeapNode;
}
  
// A standard function to build min heap
void buildMinHeap(struct MinHeap* minHeap)
  
{
  
    int n = minHeap->size - 1;
    int i;
  
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}
  
// A utility function to print an array of size n
void printArr(int arr[], int n, char data, unordered_map<char, string> &R, unordered_map<string, char> &V)
{
    int i;
    string code = "";
    for (i = 0; i < n; ++i)
    {
        printf("%d", arr[i]);
        code +=  to_string(arr[i]);
    }

    R[data] = code;
    V[code] = data;
    printf("\n");
}
  
// Utility function to check if this node is leaf
int isLeaf(struct MinHeapNode* root)
{
    return !(root->left) && !(root->right);
}
  
// Creates a min heap of capacity
// equal to size and inserts all character of
// data[] in min heap. Initially size of
// min heap is equal to capacity
struct MinHeap* createAndBuildMinHeap(char data[],
                                      int freq[], int size)
  
{
  
    struct MinHeap* minHeap = createMinHeap(size);
  
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
  
    minHeap->size = size;
    buildMinHeap(minHeap);
  
    return minHeap;
}
  
// The main function that builds Huffman tree
struct MinHeapNode* buildHuffmanTree(char data[],
                                     int freq[], int size)
  
{
    struct MinHeapNode *left, *right, *top;
  
    // Step 1: Create a min heap of capacity
    // equal to size.  Initially, there are
    // modes equal to size.
    struct MinHeap* minHeap
        = createAndBuildMinHeap(data, freq, size);
  
    // Iterate while size of heap doesn't become 1
    while (!isSizeOne(minHeap)) {
  
        // Step 2: Extract the two minimum
        // freq items from min heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);
  
        // Step 3:  Create a new internal
        // node with frequency equal to the
        // sum of the two nodes frequencies.
        // Make the two extracted node as
        // left and right children of this new node.
        // Add this node to the min heap
        // '$' is a special value for internal nodes, not
        // used
        top = newNode('$', left->freq + right->freq);
  
        top->left = left;
        top->right = right;
  
        insertMinHeap(minHeap, top);
    }
  
    // Step 4: The remaining node is the
    // root node and the tree is complete.
    return extractMin(minHeap);
}
  
// Prints huffman codes from the root of Huffman Tree.
// It uses arr[] to store codes
void printCodes(struct MinHeapNode* root, int arr[],
                int top, unordered_map<char, string> &R, unordered_map<string, char> &V)
  
{
  
    // Assign 0 to left edge and recur
    if (root->left) {
  
        arr[top] = 0;
        printCodes(root->left, arr, top + 1, R, V);
    }
  
    // Assign 1 to right edge and recur
    if (root->right) {
  
        arr[top] = 1;
        printCodes(root->right, arr, top + 1, R, V);
    }
    // If this is a leaf node, then
    // it contains one of the input
    // characters, print the character
    // and its code from arr[]
    if (isLeaf(root)) {
  
        printf("%c: ", root->data);
        printArr(arr, top, root->data, R, V);
    }
}
  
// The main function that builds a
// Huffman Tree and print codes by traversing
// the built Huffman Tree
void HuffmanCodes(char data[], int freq[], int size, unordered_map<char, string> &R, unordered_map<string, char> &V)
  
{
    // Construct Huffman Tree
    struct MinHeapNode* root
        = buildHuffmanTree(data, freq, size);
  
    // Print Huffman codes using
    // the Huffman tree built above
    int arr[MAX_TREE_HT], top = 0;
  
    printCodes(root, arr, top, R, V);
}

bool cmp(pair<char, float>& a,
         pair<char, float>& b)
{
    return a.second < b.second;
}

vector<pair<char, float>> sort(unordered_map<char, float> M)
{
  
    vector<pair<char, float>> A;

    for (auto& it : M) {
        A.push_back(it);
    }
  
    sort(A.begin(), A.end(), cmp);

    return A;
}

vector<pair<char, float>> read ()
{
    unordered_map <char, float> chars;
    string path ("input.txt");
    ifstream in(path);
    if (!in.is_open())
    {
        cout<< "Error loading the text file. \n";
    }
    char ch;
    int n=0;
    while (in.get(ch)) 
    {
        chars[ch] ++;
        n++;
    }
    for(auto &it:chars)
    {
        //it.second /= n;
    }

    in.close();
    vector<pair<char, float>> rslt = sort(chars);
    return rslt;
}

void encoder(unordered_map <char, string> codes)
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
    string code = "";
    while (in.read((char*)&num, 1)) 
    {
        string byte = decimalToBinary(num);
        cout << byte << endl;
        
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

 int main()
 {
    unordered_map<char, string> R;
    unordered_map<string, char> V;
    vector<pair<char, float>> chars = read();
    char arr[chars.size()];
    int freq[chars.size()];
    for(int i = 0; i < chars.size(); i++)
    {
        arr[i] = chars[i].first;
        freq[i] = int(chars[i].second);
    }
    HuffmanCodes(arr, freq, chars.size(), R, V);
    encoder(R);
    decoder(V);
    cout << "-------------------------------------------\n";
    for(auto &rana: R)
    {
        cout << rana.first << " " << rana.second << endl;
    }
    
    return 0;
 }
