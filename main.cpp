#include <iostream>
#include <string>
#include <queue>          // For priority_queue
#include <unordered_map>  // For frequency map and code map
#include <memory>         // For std::shared_ptr (easy memory management)
#include <vector>

// Use a shared_ptr for easy, automatic memory management of the tree
// This way, we don't need to manually write 'delete'
using namespace std;

/**
 * @struct HuffmanNode
 * @brief Represents a node in the Huffman tree.
 */
struct HuffmanNode {
    char data; // The character
    int freq;  // Frequency of the character
    shared_ptr<HuffmanNode> left, right;

    // Constructor for leaf nodes (they have a character)
    HuffmanNode(char data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}

    // Constructor for internal nodes (they don't have a character)
    HuffmanNode(shared_ptr<HuffmanNode> l, shared_ptr<HuffmanNode> r)
        : data('\0'), freq(l->freq + r->freq), left(l), right(r) {}

    // Helper function to check if a node is a leaf
    bool isLeaf() const {
        return !left && !right;
    }
};

/**
 * @struct Compare
 * @brief A comparison object for our priority queue (min-heap).
 * It puts the nodes with the lowest frequency at the top.
 */
struct Compare {
    bool operator()(const shared_ptr<HuffmanNode>& a, const shared_ptr<HuffmanNode>& b) {
        // 'a' > 'b' means 'a' has lower priority (it's "bigger")
        return a->freq > b->freq;
    }
};

/**
 * @brief Recursively traverses the tree to build the Huffman codes.
 * @param node The current node in the tree.
 * @param code The binary code built so far.
 * @param huffmanCodes The map where we store the final codes.
 */
void generateCodes(shared_ptr<HuffmanNode> node, string code, unordered_map<char, string>& huffmanCodes) {
    if (!node) {
        return;
    }

    // If it's a leaf node, we have a complete code for a character
    if (node->isLeaf()) {
        // Handle the edge case of a string with only one character (e.g., "aaaa")
        if (code.empty()) {
            huffmanCodes[node->data] = "0";
        } else {
            huffmanCodes[node->data] = code;
        }
        return;
    }

    // Go left, append '0'
    generateCodes(node->left, code + "0", huffmanCodes);
    // Go right, append '1'
    generateCodes(node->right, code + "1", huffmanCodes);
}

int main() {
    // The text we want to compress
    string text = "huffman coding is simple";
    
    if (text.empty()) {
        cout << "String is empty. Nothing to do." << endl;
        return 0;
    }

    cout << "## Huffman Coding Algorithm ##" << endl;
    cout << "Original Text: \"" << text << "\"" << endl;
    cout << "---------------------------------" << endl;

    // 1. Count character frequencies
    unordered_map<char, int> freqMap;
    for (char c : text) {
        freqMap[c]++;
    }

    // 2. Build the priority queue (min-heap)
    //    We store shared_ptr's to nodes, and use our 'Compare' struct
    priority_queue<shared_ptr<HuffmanNode>, 
                   vector<shared_ptr<HuffmanNode>>, 
                   Compare> pq;

    // Create a leaf node for each character and add it to the heap
    for (auto pair : freqMap) {
        pq.push(make_shared<HuffmanNode>(pair.first, pair.second));
    }

    // 3. Build the Huffman Tree
    //    Loop until only one node (the root) is left in the queue
    while (pq.size() > 1) {
        // Extract the two nodes with the lowest frequencies
        auto left = pq.top();
        pq.pop();
        auto right = pq.top();
        pq.pop();

        // Create a new internal node that combines them
        // The new node's frequency is the sum of its children's frequencies
        auto parent = make_shared<HuffmanNode>(left, right);
        pq.push(parent);
    }

    // The last node in the queue is the root of our tree
    shared_ptr<HuffmanNode> root = pq.top();

    // 4. Generate Codes
    unordered_map<char, string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);

    cout << "## Generated Codes ##" << endl;
    for (auto pair : huffmanCodes) {
        cout << "'" << pair.first << "' : " << pair.second << endl;
    }
    cout << "---------------------------------" << endl;

    // 5. Encode the original text
    string encodedText = "";
    for (char c : text) {
        encodedText += huffmanCodes[c];
    }

    cout << "## Encoded Text ##" << endl;
    cout << encodedText << endl;
    cout << "---------------------------------" << endl;

    // 6. Decode the text
    //    We just need the 'root' of the tree and the encoded string
    string decodedText = "";
    auto currentNode = root; // Start at the root

    for (char bit : encodedText) {
        // Move left for '0', right for '1'
        if (bit == '0') {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }

        // If we've reached a leaf node, we found a character
        if (currentNode->isLeaf()) {
            decodedText += currentNode->data; // Add the character
            currentNode = root;               // Go back to the root for the next char
        }
    }

    cout << "## Decoded Text ##" << endl;
    cout << decodedText << endl;
    cout << "---------------------------------" << endl;

    // Verify
    if (text == decodedText) {
        cout << "Success! Original and decoded text match. 👍" << endl;
    } else {
        cout << "Failure! Text does not match. 👎" << endl;
    }

    return 0; // All memory is cleaned up automatically by shared_ptr
}
