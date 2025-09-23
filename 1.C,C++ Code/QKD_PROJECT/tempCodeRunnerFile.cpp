#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include "PrivAmp.h"
// #include "checkKey.cpp"
#include "E:\QKD_PROJECT\ldpclib\source\check.cpp"
#include "E:\QKD_PROJECT\ldpclib\include\ldpcMgr.h"
#include "E:\QKD_PROJECT\ldpclib\include\check.h"
#include "E:/QKD_PROJECT/ldpclib/include/dec.h"
#include "E:/QKD_PROJECT/ldpclib/include/enc.h"
#include "E:/QKD_PROJECT/ldpclib/include/ldpcMgr.h"
#include "E:/QKD_PROJECT/ldpclib/include/mod2dense.h"
#include "E:/QKD_PROJECT/ldpclib/include/mod2sparse.h"
#include "E:/QKD_PROJECT/ldpclib/include/rcode.h"

const char* pchkFile = "E:/QKD_PROJECT/pchk.txt";
const char* genFile = "E:/QKD_PROJECT/gen.txt";




#define KEY_LENGTH 100
#define PA_KEY_LENGTH 10
#define ERROR_RATE 0.1

void printStep(const std::string& label, const std::vector<char>& key) {
std::cout << "\n" << label << ": ";
for (char bit : key) {
std::cout << static_cast<int>(bit);
}
std::cout << std::endl;
}

void siftingSimulation(const std::vector<char>& aliceKey, const std::vector<char>& bobKey) {
std::cout << "\nSifting Process Step-by-Step:\n";
for (int i = 0; i < aliceKey.size(); ++i) {
std::cout << "Step " << std::setw(2) << i + 1
<< " | Alice sends: " << static_cast<int>(aliceKey[i])
<< " | Bob receives: " << static_cast<int>(bobKey[i]) << "\n";
}
}

void checkKeyAgreement(const std::vector<char>& aliceKey, const std::vector<char>& bobKey) {
int errors = 0;
std::cout << "\nBit-by-bit Comparison:\n";
std::cout << "Index | Alice | Bob | Result | Bit Context\n";
std::cout << "----------------------------------------------\n";
for (int i = 0; i < aliceKey.size(); ++i) {
std::string result = (aliceKey[i] == bobKey[i]) ? "Match " : "Mismatch";
if (aliceKey[i] != bobKey[i]) errors++;


    std::string context = "";
    for (int j = i - 2; j <= i + 2; ++j) {
        if (j >= 0 && j < aliceKey.size())
            context += std::to_string(aliceKey[j]);
        else
            context += "-";
    }

    std::cout << std::setw(5) << i
              << " |   " << static_cast<int>(aliceKey[i])
              << "   |  " << static_cast<int>(bobKey[i])
              << " | " << result
              << " | " << context << "\n";
}

double qber = static_cast<double>(errors) / aliceKey.size() * 100.0;
std::cout << "\nTotal Errors: " << errors << " / " << aliceKey.size()
          << "  |  QBER: " << std::fixed << std::setprecision(2) << qber << "%\n";

if (errors == 0)
    std::cout << "Keys match perfectly. Proceed to privacy amplification.\n";
else
    std::cout << "Keys contain mismatches. Proceeding to LDPC-based error correction...\n";
}

int main() {
std::srand(static_cast<unsigned>(std::time(nullptr)));


std::vector<char> aliceKey(KEY_LENGTH);
std::vector<char> bobKey(KEY_LENGTH);

// Step 1: Generate Alice key and copy to Bob
for (int i = 0; i < KEY_LENGTH; ++i) {
    aliceKey[i] = rand() % 2;
    bobKey[i] = aliceKey[i];
}

// Step 2: Inject errors in Bob's key
int totalErrors = static_cast<int>(KEY_LENGTH * ERROR_RATE);
for (int i = 0; i < totalErrors; ++i) {
    int index = rand() % KEY_LENGTH;
    bobKey[index] = !bobKey[index];
}

printStep("Alice Key", aliceKey);
printStep("Bob Key (Before Errors)", bobKey);

siftingSimulation(aliceKey, bobKey);
checkKeyAgreement(aliceKey, bobKey);

// Step 3: LDPC Error Correction
std::cout << "\nStep-3: Applying LDPC Error Correction...\n";

const char* pchkFile = "pchk.txt";  // You must have these LDPC matrix files
const char* genFile  = "gen.txt";
qkdtools::ldpcMgr decoder(const_cast<char*>(pchkFile), const_cast<char*>(genFile));

char* parity = new char[KEY_LENGTH](); // Simulated parity input
char* data   = new char[KEY_LENGTH];

// Copy Bob's key to data buffer for decoding
for (int i = 0; i < KEY_LENGTH; ++i)
    data[i] = bobKey[i];

if (decoder.decodeParityAndData(parity, data)) {
    char* correctedData = decoder.getDecodedData();

    std::cout << "LDPC Decoding successful.\n";

    // Step 4: Privacy Amplification
    std::cout << "\nStep-4: Privacy Amplification\n";
    qkdtools::PrivAmp pa(PA_KEY_LENGTH);
    pa.generateToeplitzMat();
    pa.calcPAKey(correctedData, KEY_LENGTH); // FIXED: now pass key length

    char* outputKey = pa.getPAKey();
    std::cout << "Final Secret Key: ";
    for (int i = 0; i < PA_KEY_LENGTH; ++i) {
        std::cout << static_cast<int>(outputKey[i]);
    }
    std::cout << std::endl;
} else {
    std::cerr << "LDPC decoding failed. Cannot proceed to privacy amplification.\n";
}

delete[] parity;
delete[] data;

return 0;
}

