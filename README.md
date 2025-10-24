#  Huffman Compression Tool (C++)

A lightweight command-line **compressor/decompressor** built entirely from scratch using **Huffman coding**.  
This program compresses files into a custom `.huf` format and restores them exactly as the original by reconstructing the Huffman tree from stored `(byte, code)` pairs.

---

##  Features

- **Full Compression & Decompression**  
  Converts any file (text or binary) into a smaller `.huf` archive and restores it bit-perfectly.
-  **Deterministic Decode**  
  The decoder rebuilds the Huffman tree directly from `(byte, code)` pairs, avoiding tie-break ambiguities.  
-  **Bit-Level I/O**  
  Uses a custom `Storage` class to write and read variable-length codes efficiently.
-  **Binary Safe**  
  Handles all byte values (0–255) and reads/writes in binary mode.
-  **Automatic Cleanup**  
  Recursive tree deletion prevents memory leaks.

---

##  How It Works

### 1️. Frequency Counting
`countFreq()` scans the input and counts occurrences of every character.

### 2️. Tree Building
`buildTree()` inserts all characters into a **priority queue**, repeatedly combining the two lowest-weight nodes into a parent until one root remains.

### 3️. Code Generation
`generateCode()` traverses the tree recursively:
- Left edge → add `'0'`
- Right edge → add `'1'`  
Resulting codes form a prefix-free mapping for all characters.

### 4️. Encoding
`encode()`:
- Creates a header storing `(ASCII int, code)` pairs separated by a **Unit Separator (`char(31)`)**.  
- Prepends the header with the **total number of valid bits** followed by `'#'`.  
- Writes the Huffman bitstream using the `Storage` class.

### 5️. Decoding
`decompress()`:
- Reads and parses the header.  
- Rebuilds the tree by walking code paths via `decode()`.  
- Reads bits from `Storage`, traversing the tree until a leaf node is reached, then writes the decoded byte.

---

## Time Complexity 

- Compression: O(n log σ) time, O(σ) space

- Decompression: O(n) time, O(σ) space

- Works efficiently for all byte data (0–255).

- Typical compression ratio: 30–60% for natural language text.

---

