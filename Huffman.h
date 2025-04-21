#include <string>
#include <queue>
#include <fstream>
#include <unordered_map>

#include "Node.h"
#include "Storage/Storage.h"

#ifndef HUFFMAN_H
#define HUFFMAN_H

/**
 * Simulates Huffman algorithm
 */
class Huffman
{
private:
    /**
     * Tree root Node
     */
    Node* root;
public:
    /**
     * default constructor
     */
    Huffman();
    /**
     * destructor
     */
    ~Huffman();
    /**
     * Stores the frequency of each character
     * @param inputText content in the file
     * @return unordered map that stores (char, frequency)
     */
    std::unordered_map<char, int> countFreq(std::string inputText);
    /**
     * Build tree out of the frequency map
     * @param freq frequency map
     * @return the root node of the tree
     */
    Node* buildTree(const std::unordered_map<char, int>& freq);
    /**
     * Compress the input file and store the compressed content in output file
     * @param in_file input file
     * @param out_file output file
     */
    void compress(std::string& in_file, const std::string& out_file);
    /**
     * Decompress the input file and store the decompressed content in output file
     * @param in_file input file
     * @param out_file output file
     */
    void decompress(const std::string& in_file, const std::string& out_file);

    /**
     * Recursive function to generate binary bit
     * @param convertMap unordered map to store (ascii of character, frequency in binary bits)
     * @param node Huffman tree
     * @param stringPath reference to string path of binary strings conversion
     */
    void generateCode(std::unordered_map<char, std::string>& convertMap, Node* node, const std::string& stringPath);
    /**
     * Encode the frequency map and the content of the input file
     * @param inFile input File
     * @param headerOutput variable for header output
     * @param outFile output File
     */
    void encode(std::string& inFile, std::string& headerOutput, const std::string& outFile);
    /**
     * Decoding the character
     * @param node root Node
     * @param freqBinary the binary bit of the frequency from the header
     * @param c decoded character
     */
    void decode(Node* node, const std::string& freqBinary, char c);

    /**
     * Deleting the tree after compression and decompression
     * @param node root Node
     */
    void deleteTree(Node* node);
};

#endif //HUFFMAN_H
