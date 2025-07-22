// #include <iostream>
// #include "E:/QKD/qkd-tools/qkdtools/include/QkdMgr.h"  // or use relative path

// int main() {
//     std::cout << "QKD Tools Test Starting..." << std::endl;

//     qkdtools::QkdMgr qkd;   // ✅ Use namespace here
//     qkd.initQkd();          // ✅ Call correct method name (it's `initQkd()` not `init()`)

//     std::cout << "QKD Tools Test Done." << std::endl;
//     return 0;
// }











#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "rcode.h"  // from ldpclib/include

int main(int argc, char* argv[]) {
    std::cout << "QKD LDPC Test Starting...\n";

    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " pchk.txt gen.txt inputfile.txt\n";
        return 1;
    }

    const char* pchkFile = argv[1];
    const char* genFile = argv[2];
    const char* inputFile = argv[3];

    // Step 1: Load input bits from file
    std::ifstream infile(inputFile);
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open " << inputFile << "\n";
        return 1;
    }

    std::vector<unsigned char> inputBits;
    std::string line;
    while (std::getline(infile, line)) {
        for (char ch : line) {
            if (ch == '0' || ch == '1') {
                inputBits.push_back(ch - '0');
            }
        }
    }

    infile.close();

    if (inputBits.empty()) {
        std::cerr << "Error: No valid binary data found in " << inputFile << "\n";
        return 1;
    }

    // Step 2: Initialize LDPC
    rCode ldpc;

    ldpc.read_pchk((char*)pchkFile);

    // Note: Replace with correct dimensions (n, k) for your code
    int n = 256; // Total codeword length
    int k = 128; // Message bits
    ldpc.read_gen((char*)genFile, k, n);

    // Step 3: Encode
    std::vector<unsigned char> encoded;
    ldpc.encode(inputBits, encoded);

    std::cout << "Encoded Bits: ";
    for (auto bit : encoded) std::cout << (int)bit;
    std::cout << "\n";

    // (Optional) Simulate a bit flip
    if (!encoded.empty()) {
        encoded[0] ^= 1; // flip the first bit to simulate error
    }

    // Step 4: Decode
    std::vector<unsigned char> decoded;
    ldpc.decode(encoded, decoded);

    std::cout << "Decoded Bits: ";
    for (auto bit : decoded) std::cout << (int)bit;
    std::cout << "\n";

    // Step 5: Compare input and decoded
    bool match = true;
    if (inputBits.size() != decoded.size()) {
        match = false;
    } else {
        for (size_t i = 0; i < inputBits.size(); ++i) {
            if (inputBits[i] != decoded[i]) {
                match = false;
                break;
            }
        }
    }

    std::cout << (match ? "Success: Decoded matches input.\n" : "Failure: Decoded does not match input.\n");
    std::cout << "QKD LDPC Test Done.\n";

    return 0;
}

