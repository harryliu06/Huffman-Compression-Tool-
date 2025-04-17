#include "Huffman.h"

#include <sstream>

Huffman::Huffman()
{
    root = nullptr;
}

Huffman::~Huffman()
{
    delete root;
}


std::unordered_map<char, int> Huffman::countFreq(std::string inputText)
{
    std::unordered_map<char, int> freqs;
    char curr;
    for (char ch: inputText)
    {
        freqs[ch]++;
    }
    return freqs;
}

Node* Huffman::buildTree(const std::unordered_map<char, int>& freq)

{
    std::priority_queue<Node*, std::vector<Node*>, compareWeights> pq;

    //push the (key, value) pair into priority queue
    for (const std::pair<const char, int>p: freq)
    {
        pq.push(new Node(p.first, p.second));
    }

    //Build a tree from bottom to top
    //pq stores the chars from the smallest weight to biggest
    //Alternatively it creates left n right Nodes as it goes through the queue
    //Every 2 nodes, it creates a parent node with sum of left n right weights, then having left as zero n right as one
    //then push it into the pq

    while (pq.size() > 1)
    {
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();

        Node* parent = new Node('\0', left->weight + right->weight, left, right);

        pq.push(parent);
    }

    if (pq.empty())
    {
        return nullptr;
    }
    else
    {
        this->root = pq.top();
        pq.pop();
        return this->root;
    }
}

void Huffman::generateCode(std::unordered_map<char, std::string>& convertMap, Node* node, const std::string& stringPath)
{
    if (!node)
    {
        return;
    }

    if (!node->zero && !node->one)
    {
        convertMap[node->letter] = stringPath;
        return;
    }

    generateCode(convertMap, node->zero, stringPath + "0");
    generateCode(convertMap, node->one, stringPath + "1");
}

std::string Huffman::encode(std::string& inFile, std::string& headerOutput, const std::string& outFile)
{
    std::ifstream in(inFile, std::ios::binary);
    std::string inputText;
    char ch;
    while (in.get(ch)) {
        inputText += ch;
    }
    in.close();

    std::unordered_map<char, int> freqs = countFreq(inputText);

    Node* root = buildTree(freqs);
    std::unordered_map<char, std::string> convertMap;
    generateCode(convertMap, root, "");


    std::ostringstream headerOut;
    const char SEPARATOR = char(31);
    for (std::unordered_map<char, int>::const_iterator it = freqs.begin(); it != freqs.end(); it++)
    {
        headerOut << (int)(unsigned char)it->first << SEPARATOR << convertMap[it->first] << "|";
    }

    headerOutput = headerOut.str();
    Storage s;

    int totalBits = 0;

    for (char c: inputText)
    {
        totalBits += convertMap[c].length();
    }

    if (s.open(outFile, "write"))
    {
        s.setHeader(std::to_string(totalBits) +"#"+ headerOutput);
        for (char ch: inputText)
        {
            std::string bits = convertMap[ch];
            s.insert(bits);
        }
        s.close();
    } else
    {
        std::cerr << "Error: can't open file" << std::endl;
    }

    deleteTree(root);
}


void Huffman::compress(std::string& inFile, const std::string& outFile)
{
    std::string headerOutput = "";
    encode(inFile, headerOutput, outFile);
}

void Huffman::decode(Node* node, const std::string& freqBinary, char c)
{
    Node* current = root;

    for (char bit: freqBinary)
    {
        if (bit == '0') {
            if (!current->zero) current->zero = new Node('\0', 0);
            current = current->zero;
        } else if (bit == '1') {
            if (!current->one) current->one = new Node('\0', 0);
            current = current->one;
        }
    }
    current->letter = c;
}

void Huffman::decompress(const std::string& inFile, const std::string& outFile)
{
    Storage s;
    if (!s.open(inFile, "read"))
    {
        std::cerr << "Can't open file" << std::endl;
    }

    std::string header = s.getHeader();
    int sep = header.find("#");
    if (sep == std::string::npos)
    {
        std::cerr << "can't find the separator" << std::endl;
    }

    int validBits = std::stoi(header.substr(0, sep));
    std::string freq = header.substr(sep+1);

    std::unordered_map<char, int> freqs;
    std::istringstream inputStream(freq);
    std::string pair;
    const char SEPARATOR = char(31);

    this->root = new Node('\0', 0);

    while (std::getline(inputStream, pair, '|'))
    {
        int pos = pair.find(SEPARATOR);
        if (pos != std::string::npos)
        {
            int ascii = std::stoi(pair.substr(0, pos));
            std::string freqBinary = pair.substr(pos+1);
            decode(root, freqBinary, char(ascii));
        }
    }

    std::ostringstream decodedOut;
    std::string byteChunk;
    int counter = 0;
    Node* curr = root;

    while (s.extract(byteChunk) && counter < validBits)
    {
        for (char bit: byteChunk)
        {
            if (counter >= validBits) break;

            if (bit == '0')
            {
                curr = curr->zero;
            } else
            {
                curr = curr->one;
            }

            counter++;
            if (!curr->zero && !curr->one)
            {
                decodedOut << curr->letter;
                curr = root;
            }
        }
    }


    std::ofstream out(outFile, std::ios::binary);

    out << decodedOut.str();

    s.close();
    out.close();

    deleteTree(root);
}


void Huffman::deleteTree(Node* node)
{
    if (!node) return;
    deleteTree(node->zero);
    deleteTree(node->one);
    delete node;
}






