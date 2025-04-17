#include <string>
#include <queue>
#include <fstream>
#include <unordered_map>

#include "Node.h"
#include "Storage/Storage.h"

#ifndef HUFFMAN_H
#define HUFFMAN_H


class Huffman
{
private:
    Node* root;
public:
    Huffman();
    ~Huffman();
    std::unordered_map<char, int> countFreq(std::string inputText);
    Node* buildTree(const std::unordered_map<char, int>& freq);
    void compress(std::string& in_file, const std::string& out_file);
    void decompress(const std::string& in_file, const std::string& out_file);

    void generateCode(std::unordered_map<char, std::string>& convertMap, Node* node, const std::string& stringPath);
    std::string encode(std::string& inFile, std::string& headerOutput, const std::string& outFile);
    void decode(Node* node, const std::string& freqBinary, char c);

    void deleteTree(Node* node);
};

#endif //HUFFMAN_H
