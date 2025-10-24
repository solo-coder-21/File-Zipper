# C++ Huffman Compressor

This is a simple, command-line file compression and decompression tool written in C++. It uses the classic *Huffman coding* algorithm for lossless data compression.

## Features

* *Compression*: Compresses any file into a smaller .huf (or any other extension) file.
* *Decompression*: Decompresses files created by this tool back to their original state.
* *Lossless*: No data is lost during the compression/decompression cycle.
* *Header-based*: The compressed file stores a header containing the frequency map, which allows the decompressor to rebuild the exact Huffman tree.

## How It Works

### Compression

1.  *Build Frequency Map*: The program reads the input file once to count the frequency of every byte (0-255).
2.  *Build Huffman Tree*: A priority queue (min-heap) is used to build the Huffman tree. Characters with lower frequencies are placed deeper in the tree.
3.  *Generate Codes*: The tree is traversed to assign a unique binary code (e.g., 01, 101) to each character. More frequent characters get shorter codes.
4.  *Write File*:
    * *Header*: The program first writes a header to the output file. This header contains the frequency map, which is essential for decompression. It also includes padding information.
    * *Data*: The input file is read a second time. Each byte is replaced with its corresponding Huffman code. These codes are packed into new bytes (8 bits at a time) and written to the output file.

### Decompression

1.  *Read Header*: The decompressor reads the frequency map from the header of the compressed file.
2.  *Rebuild Tree: It uses this map to reconstruct the *exact same Huffman tree that was built during compression.
3.  *Decode Data*: The program reads the compressed data one bit at a time, traversing the Huffman tree from the root.
    * A 0 bit means "go to the left child."
    * A 1 bit means "go to the right child."
4.  When a leaf node is reached, the byte stored in that node is written to the output file, and the traversal resets to the root. This continues until all bits are decoded.

## How to Build

You need a C++ compiler that supports C++11 or newer (e.g., g++ or clang++).

```bash
# Compile the program
g++ huffman.cpp -o huffman -std=c++11

# (Optional) For optimization
# g++ huffman.cpp -o huffman -std=c++11 -O3
