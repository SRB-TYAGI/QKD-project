// #include <iostream>
// #include "PrivAmp.h"
// #include "PrivAmp.cpp"


// int main() {
//     const int keyLength = 100;  // must match PrivAmp constructor

//     // Create a privacy amplification object with key length 100
//     qkdtools::PrivAmp pa(keyLength);

//     // Generate Toeplitz matrix
//     pa.generateToeplitzMat();

//     // Example input key (bool array) with a simple pattern
//     bool inputKey[keyLength];
//     for (int i = 0; i < keyLength; ++i) {
//         inputKey[i] = (i % 2 == 0);  // set even indices to 1, odd to 0
//     }

//     // Calculate privacy amplified key
//     pa.calcPAKey(inputKey);

//     // Get output key
//     char* outputKey = pa.getPAKey();

//     // Print the first 10 bits of output key
//     std::cout << "Privacy amplified key (first 10 bits): ";
//     for (int i = 0; i < 10; ++i) {
//         std::cout << (int)outputKey[i];
//     }
//     std::cout << std::endl;

//     return 0;
// }



// user input

// #include <iostream>
// #include <vector>
// #include <cstdlib>
// #include <ctime>
// #include "PrivAmp.h"

// extern void checkKeyAgreement(char* aliceKey, char* bobKey, int keyLength);

// int main() {
//     const int keyLength = 10;
//     const int paKeyLength = 10;
//     const double errorRate = 0.01;  // 1% errors

//     std::vector<char> aliceKey(keyLength);
//     std::vector<char> bobKey(keyLength);

//     std::srand(static_cast<unsigned>(std::time(nullptr)));

//     // Generate random bits for Alice and copy to Bob
//     for (int i = 0; i < keyLength; ++i) {
//         aliceKey[i] = rand() % 2;
//         bobKey[i] = aliceKey[i];
//     }

//     // Introduce random bit flips in Bob's key based on errorRate
//     int totalErrors = static_cast<int>(keyLength * errorRate);
//     for (int i = 0; i < totalErrors; ++i) {
//         int index = rand() % keyLength;
//         bobKey[index] =!bobKey[index]; // Flip the bit
//     }

//     // Compare keys and calculate QBER
//     checkKeyAgreement(aliceKey.data(), bobKey.data(), keyLength);

//     // Perform privacy amplification
//     qkdtools::PrivAmp pa(paKeyLength);
//     pa.generateToeplitzMat();
//     pa.calcPAKey(bobKey.data());

//     // Output the first 10 bits of the secure key
//     char* outputKey = pa.getPAKey();
//     std::cout << "Privacy amplified key (first 10 bits): ";
//     for (int i = 0; i < 10; ++i) {
//         std::cout << static_cast<int>(outputKey[i]);
//     }
//     std::cout << std::endl;

//     return 0;
// }




















// #include <iostream>
// #include <vector>
// #include <cstdlib>
// #include <ctime>
// #include <iomanip>  // for std::setw

// #define KEY_LENGTH 10  // Keep small for demo; change to 100 or 1000
// #define PA_KEY_LENGTH 10
// #define ERROR_RATE 0.1

// void printStep(const std::string& label, const std::vector<char>& key) {
//     std::cout << "\n" << label << ": ";
//     for (int i = 0; i < key.size(); ++i) {
//         std::cout << (int)key[i];
//     }
//     std::cout << std::endl;
// }

// void siftingSimulation(const std::vector<char>& aliceKey, const std::vector<char>& bobKey) {
//     std::cout << "\n Sifting Process Step-by-Step:\n";
//     for (int i = 0; i < aliceKey.size(); ++i) {
//         std::cout << "Step " << std::setw(2) << i + 1
//                   << " | Alice sends: " << (int)aliceKey[i]
//                   << " | Bob receives: " << (int)bobKey[i] << std::endl;
//     }
// }

// void checkKeyAgreement(const std::vector<char>& aliceKey, const std::vector<char>& bobKey) {
//     int errors = 0;
//     std::cout << "\n Bit-by-bit Comparison:\n";
//     std::cout << "Index |   Alice |  Bob |   Result      | Bit Value\n";
//     std::cout << "-----------------------------------------------\n";
//     for (int i = 0; i < aliceKey.size(); ++i) {
//         std::string result = (aliceKey[i] == bobKey[i]) ? " Match   " : " Mismatch";
//         if (aliceKey[i] != bobKey[i]) errors++;

//         // Prepare bit context (5 bits around)
//         std::string context = "";
//         for (int j = i - 2; j <= i + 2; ++j) {
//             if (j >= 0 && j < aliceKey.size()) {
//                 context += std::to_string(aliceKey[j]);
//             } else {
//                 context += "-";
//             }
//         }

//         std::cout << std::setw(5) << i
//                   << " |   " << (int)aliceKey[i]
//                   << "   |  " << (int)bobKey[i]
//                   << " | " << result
//                   << " | " << context << "\n";
//     }

//     double qber = static_cast<double>(errors) / aliceKey.size() * 100.0;
//     std::cout << "\n Total Errors: " << errors
//               << " / " << aliceKey.size()
//               << "  |  QBER: " << std::fixed << std::setprecision(2) << qber << "%\n";

//     if (errors == 0)
//         std::cout << " Keys match perfectly. Proceed to privacy amplification.\n";
//     else
//         std::cout << "  Keys contain mismatches. Apply error correction or discard.\n";
// }

// std::vector<char> performPrivacyAmplification(const std::vector<char>& inputKey, int outputLength) {
//     std::vector<char> paKey(outputLength);
//     std::srand(std::time(nullptr) + 100);
//     for (int i = 0; i < outputLength; ++i) {
//         int bit = 0;
//         for (int j = 0; j < inputKey.size(); ++j) {
//             bit ^= (inputKey[j] & (std::rand() % 2));
//         }
//         paKey[i] = bit;
//     }
//     return paKey;
// }

// int main() {
//     std::srand(static_cast<unsigned>(std::time(nullptr)));

//     std::vector<char> aliceKey(KEY_LENGTH);
//     std::vector<char> bobKey(KEY_LENGTH);

//     // Step 1: Generate raw keys
//     for (int i = 0; i < KEY_LENGTH; ++i) {
//         aliceKey[i] = rand() % 2;
//         bobKey[i] = aliceKey[i];
//     }

//     printStep(" Alice Key", aliceKey);
//     printStep(" Bob Key (before errors)", bobKey);

//     // Step 2: Simulate errors in Bob's key
//     int totalErrors = static_cast<int>(KEY_LENGTH * ERROR_RATE);
//     for (int i = 0; i < totalErrors; ++i) {
//         int index = rand() % KEY_LENGTH;
//         bobKey[index] = !bobKey[index];
//     }

//     printStep(" Bob Key (after errors)", bobKey);

//     // Step 3: Sifting
//     siftingSimulation(aliceKey, bobKey);

//     // Step 4: Compare keys and calculate QBER
//     checkKeyAgreement(aliceKey, bobKey);

//     // Step 5: Privacy Amplification
//     std::cout << "\n Final Step: Privacy Amplification\n";
//     std::vector<char> finalKey = performPrivacyAmplification(bobKey, PA_KEY_LENGTH);
//     printStep(" Final Secret Key", finalKey);

//     return 0;
// }





// main code 

// #include <iostream>
// #include <vector>
// #include <cstdlib>
// #include <ctime>
// #include <iomanip>
// #include "PrivAmp.h" // Include your actual privacy amplification class

// #define KEY_LENGTH 100 // Keep small for readable output
// #define PA_KEY_LENGTH 2
// #define ERROR_RATE 0.1 // 10% errors for demo

// void printStep(const std::string& label, const std::vector<char>& key) {
// std::cout << "\n" << label << ": ";
// for (int i = 0; i < key.size(); ++i) {
// std::cout << (int)key[i];
// }
// std::cout << std::endl;
// }

// void siftingSimulation(const std::vector<char>& aliceKey, const std::vector<char>& bobKey) {
// std::cout << "\nSifting Process Step-by-Step:\n";
// for (int i = 0; i < aliceKey.size(); ++i) {
// std::cout << "Step " << std::setw(2) << i + 1
// << " | Alice sends: " << (int)aliceKey[i]
// << " | Bob receives: " << (int)bobKey[i] << std::endl;
// }
// }

// void checkKeyAgreement(const std::vector<char>& aliceKey, const std::vector<char>& bobKey) {
// int errors = 0;
// std::cout << "\nBit-by-bit Comparison:\n";
// std::cout << "Index | Alice | Bob | Result | Bit Value\n";
// std::cout << "----------------------------------------------\n";


// for (int i = 0; i < aliceKey.size(); ++i) {
//     std::string result = (aliceKey[i] == bobKey[i]) ? "Match   " : "Mismatch";
//     if (aliceKey[i] != bobKey[i]) errors++;

//     std::string context = "";
//     for (int j = i - 2; j <= i + 2; ++j) {
//         if (j >= 0 && j < aliceKey.size())
//             context += std::to_string(aliceKey[j]);
//         else
//             context += "-";
//     }

//     std::cout << std::setw(5) << i
//               << " |   " << (int)aliceKey[i]
//               << "   |  " << (int)bobKey[i]
//               << " | " << result
//               << " | " << context << "\n";
// }

// double qber = static_cast<double>(errors) / aliceKey.size() * 100.0;
// std::cout << "\nTotal Errors: " << errors
//           << " / " << aliceKey.size()
//           << "  |  QBER: " << std::fixed << std::setprecision(2) << qber << "%\n";

// if (errors == 0)
//     std::cout << "Keys match perfectly. Proceed to privacy amplification.\n";
// else
//     std::cout << " Keys contain mismatches. Apply error correction or discard.\n";
// }

// int main() {
// std::srand(static_cast<unsigned>(std::time(nullptr)));


// std::vector<char> aliceKey(KEY_LENGTH);
// std::vector<char> bobKey(KEY_LENGTH);

// for (int i = 0; i < KEY_LENGTH; ++i) {
//     aliceKey[i] = rand() % 2;
//     bobKey[i] = aliceKey[i];
// }

// printStep("Alice Key", aliceKey);
// printStep("Bob Key (before errors)", bobKey);

// int totalErrors = static_cast<int>(KEY_LENGTH * ERROR_RATE);
// for (int i = 0; i < totalErrors; ++i) {
//     int index = rand() % KEY_LENGTH;
//     bobKey[index] = !bobKey[index];
// }

// printStep("Bob Key (after errors)", bobKey);

// siftingSimulation(aliceKey, bobKey);
// checkKeyAgreement(aliceKey, bobKey);

// std::cout << "\nFinal Step: Privacy Amplification\n";

// // Use your actual Privacy Amplification class
// qkdtools::PrivAmp pa(PA_KEY_LENGTH);
// pa.generateToeplitzMat();
// pa.calcPAKey(bobKey.data());  // char* required

// char* outputKey = pa.getPAKey();
// std::cout << "\nFinal Key: ";
// for (int i = 0; i < PA_KEY_LENGTH; ++i) {
//     std::cout << (int)outputKey[i];
// }
// std::cout << std::endl;

// return 0;
// }















































// dynamic key length code

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include "PrivAmp.h"

#define KEY_LENGTH 10
#define PA_KEY_LENGTH 10
#define ERROR_RATE 0.1

void printStep(const std::string& label, const std::vector<char>& key) {
std::cout << "\n" << label << ": ";
for (char bit : key) std::cout << (int)bit;
std::cout << "\n";
}

void siftingSimulation(const std::vector<char>& aliceKey, const std::vector<char>& bobKey) {
std::cout << "\n Step-2 Sifting Process Step-by-Step: \n";
std::cout << "----------------------------------------------\n";
for (size_t i = 0; i < aliceKey.size(); ++i) {
std::cout << "Step " << std::setw(2) << i + 1
<< " | Alice sends: " << (int)aliceKey[i]
<< " | Bob receives: " << (int)bobKey[i] << "\n";

}
printStep("Bob recieving  key (After errors)", bobKey);
// printStep("Bob recieving  key (after errors)", bobKey);
}

void checkKeyAgreement(const std::vector<char>& aliceKey, const std::vector<char>& bobKey) {
int errors = 0;
std::cout << "\n Step-3:-Information Reconciliation 3 LDPC: \n";
std::cout << "----------------------------------------------";
std::cout << "\n Bit-by-bit Comparison: \n";
std::cout << "Index | Alice | Bob | Result | Bit Value\n";
std::cout << "----------------------------------------------\n";


for (size_t i = 0; i < aliceKey.size(); ++i) {
    std::string result = (aliceKey[i] == bobKey[i]) ? "Match   " : "Mismatch";
    if (aliceKey[i] != bobKey[i]) errors++;

    std::string context = "";
    for (int j = static_cast<int>(i) - 2; j <= static_cast<int>(i) + 2; ++j) {
        if (j >= 0 && j < (int)aliceKey.size())
            context += std::to_string(aliceKey[j]);
        else
            context += "-";
    }

    std::cout << std::setw(5) << i
              << " |   " << (int)aliceKey[i]
              << "   |  " << (int)bobKey[i]
              << " | " << result
              << " | " << context << "\n";
}

double qber = static_cast<double>(errors) / aliceKey.size() * 100.0;
std::cout << "\nTotal Errors: " << errors
          << " / " << aliceKey.size()
          << "  |  QBER: " << std::fixed << std::setprecision(2) << qber << "%\n";

if (errors == 0)
    std::cout << "\nKeys match perfectly. Proceed to privacy amplification.\n";
else
    std::cout << "\nKeys contain mismatches. Apply error correction or discard.\n";
}

int main() {
std::srand(static_cast<unsigned>(std::time(nullptr)));


std::vector<char> aliceKey(KEY_LENGTH);
std::vector<char> bobKey(KEY_LENGTH);

for (int i = 0; i < KEY_LENGTH; ++i) {
    aliceKey[i] = rand() % 2;
    bobKey[i] = aliceKey[i];
}
std::cout << "\n-----------------------START-------------------------------\n";
std::cout <<"\n Step-1:- Parameter Estimation:\n";
printStep("Alice generating Key", aliceKey);
// printStep("Bob recieving  key (before errors)", bobKey);

int totalErrors = static_cast<int>(KEY_LENGTH * ERROR_RATE);
for (int i = 0; i < totalErrors; ++i) {
    int index = rand() % KEY_LENGTH;
    bobKey[index] = !bobKey[index];
}

// printStep("Bob recieving  key (after errors)", bobKey);
siftingSimulation(aliceKey, bobKey);
checkKeyAgreement(aliceKey, bobKey);

std::cout << "\nStep-4:-Final Step: Privacy Amplification\n";
std::cout << "----------------------------------------------";
qkdtools::PrivAmp pa(PA_KEY_LENGTH);
pa.setPAKeyLength(PA_KEY_LENGTH, KEY_LENGTH);
pa.generateToeplitzMat();
pa.calcPAKey(bobKey.data(), KEY_LENGTH);

char* outputKey = pa.getPAKey();
std::cout << "\nFinal  Key:  ";

for (int i = 0; i < PA_KEY_LENGTH; ++i) {
    std::cout << (int)outputKey[i];
}
std::cout << std::endl;
std::cout << "\n-----------------------END-------------------------------\n";

return 0;
}























// #include <iostream>
// #include <vector>
// #include <cstdlib>
// #include <ctime>
// #include <iomanip>

// #include "PrivAmp.h"               // ✅ Use only the header
// #include "E:\QKD_PROJECT\ldpclib\include\ldpcMgr.h"
// #include "checkKey.cpp"
// #include "E:\QKD_PROJECT\ldpclib\include\check.h"

// #include "E:\QKD_PROJECT\ldpclib\include\dec.h"
// #include "E:\QKD_PROJECT\ldpclib\include\enc.h"
// #include "E:\QKD_PROJECT\ldpclib\include\mod2dense.h"
// #include "E:\QKD_PROJECT\ldpclib\include\mod2dense.h"
// #include "E:\QKD_PROJECT\ldpclib\include\rcode.h"
// #include "E:\QKD_PROJECT\ldpclib\include\alloc.h"
// #include "E:\QKD_PROJECT\ldpclib\include\intio.h"

// #define KEY_LENGTH 10
// #define PA_KEY_LENGTH 5
// #define ERROR_RATE 0.1

// void printStep(const std::string& label, const std::vector<char>& key) {
//     std::cout << "\n" << label << ": ";
//     for (char bit : key) {
//         std::cout << static_cast<int>(bit);
//     }
//     std::cout << std::endl;
// }

// void siftingSimulation(const std::vector<char>& aliceKey, const std::vector<char>& bobKey) {
//     std::cout << "\nSifting Process Step-by-Step:\n";
//     for (int i = 0; i < aliceKey.size(); ++i) {
//         std::cout << "Step " << std::setw(2) << i + 1
//                   << " | Alice sends: " << static_cast<int>(aliceKey[i])
//                   << " | Bob receives: " << static_cast<int>(bobKey[i]) << "\n";
//     }
// }

// void checkKeyAgreement(const std::vector<char>& aliceKey, const std::vector<char>& bobKey) {
//     int errors = 0;
//     std::cout << "\nBit-by-bit Comparison:\n";
//     std::cout << "Index | Alice | Bob | Result | Bit Context\n";
//     std::cout << "----------------------------------------------\n";
//     for (int i = 0; i < aliceKey.size(); ++i) {
//         std::string result = (aliceKey[i] == bobKey[i]) ? "Match " : "Mismatch";
//         if (aliceKey[i] != bobKey[i]) errors++;

//         std::string context = "";
//         for (int j = i - 2; j <= i + 2; ++j) {
//             if (j >= 0 && j < aliceKey.size())
//                 context += std::to_string(aliceKey[j]);
//             else
//                 context += "-";
//         }

//         std::cout << std::setw(5) << i
//                   << " |   " << static_cast<int>(aliceKey[i])
//                   << "   |  " << static_cast<int>(bobKey[i])
//                   << " | " << result
//                   << " | " << context << "\n";
//     }

//     double qber = static_cast<double>(errors) / aliceKey.size() * 100.0;
//     std::cout << "\nTotal Errors: " << errors << " / " << aliceKey.size()
//               << "  |  QBER: " << std::fixed << std::setprecision(2) << qber << "%\n";

//     if (errors == 0)
//         std::cout << "Keys match perfectly. Proceed to privacy amplification.\n";
//     else
//         std::cout << "Keys contain mismatches. Proceeding to LDPC-based error correction...\n";
// }

// int main() {
//     std::srand(static_cast<unsigned>(std::time(nullptr)));

//     std::vector<char> aliceKey(KEY_LENGTH);
//     std::vector<char> bobKey(KEY_LENGTH);

//     // Step 1: Generate Alice key and copy to Bob
//     for (int i = 0; i < KEY_LENGTH; ++i) {
//         aliceKey[i] = rand() % 2;
//         bobKey[i] = aliceKey[i];
//     }

//     // Step 2: Inject errors in Bob's key
//     int totalErrors = static_cast<int>(KEY_LENGTH * ERROR_RATE);
//     for (int i = 0; i < totalErrors; ++i) {
//         int index = rand() % KEY_LENGTH;
//         bobKey[index] = !bobKey[index];
//     }

//     printStep("Alice Key", aliceKey);
//     printStep("Bob Key (Before Errors)", bobKey);

//     siftingSimulation(aliceKey, bobKey);
//     checkKeyAgreement(aliceKey, bobKey);

//     // Step 3: LDPC Error Correction
//     std::cout << "\nStep-3: Applying LDPC Error Correction...\n";

//     const char* pchkFile = "pchk.txt";
//     const char* genFile  = "gen.txt";
//     qkdtools::ldpcMgr decoder((char*)pchkFile, (char*)genFile);

//     char* parity = new char[KEY_LENGTH]();
//     char* data   = new char[KEY_LENGTH];

//     // Copy Bob's key to data buffer for decoding
//     for (int i = 0; i < KEY_LENGTH; ++i)
//         data[i] = bobKey[i];

//     if (decoder.decodeParityAndData(parity, data)) {
//         char* correctedData = decoder.getDecodedData();

//         std::cout << "LDPC Decoding successful.\n";

//         // Step 4: Privacy Amplification
//         std::cout << "\nStep-4: Privacy Amplification\n";
//         qkdtools::PrivAmp pa(PA_KEY_LENGTH);
//         pa.generateToeplitzMat();
//         pa.calcPAKey(correctedData, KEY_LENGTH);

//         char* outputKey = pa.getPAKey();
//         std::cout << "Final Secret Key: ";
//         for (int i = 0; i < PA_KEY_LENGTH; ++i) {
//             std::cout << static_cast<int>(outputKey[i]);
//         }
//         std::cout << std::endl;
//     } else {
//         std::cerr << "LDPC decoding failed. Cannot proceed to privacy amplification.\n";
//     }

//     delete[] parity;
//     delete[] data;

//     return 0;
// }


























// #include <iostream>
// #include <vector>
// #include <cstdlib>
// #include <ctime>
// #include <iomanip>

// #include "PrivAmp.h"
// #include "E:\QKD_PROJECT\ldpclib\include\ldpcMgr.h"
// #include "checkKey.cpp"
// #include "E:\QKD_PROJECT\ldpclib\include\check.h"
// #include "E:\QKD_PROJECT\ldpclib\include\dec.h"
// #include "E:\QKD_PROJECT\ldpclib\include\enc.h"
// #include "E:\QKD_PROJECT\ldpclib\include\mod2dense.h"
// #include "E:\QKD_PROJECT\ldpclib\include\rcode.h"
// #include "E:\QKD_PROJECT\ldpclib\include\alloc.h"
// #include "E:\QKD_PROJECT\ldpclib\include\intio.h"

// #define KEY_LENGTH 10
// #define PA_KEY_LENGTH 5
// #define ERROR_RATE 0.1

// void printStep(const std::string& label, const std::vector<char>& key) {
//     std::cout << "\n" << label << ": ";
//     for (char bit : key) {
//         std::cout << static_cast<int>(bit);
//     }
//     std::cout << std::endl;
// }

// void siftingSimulation(const std::vector<char>& aliceKey, const std::vector<char>& bobKey) {
//     std::cout << "\nSifting Process Step-by-Step:\n";
//     for (int i = 0; i < aliceKey.size(); ++i) {
//         std::cout << "Step " << std::setw(2) << i + 1
//                   << " | Alice sends: " << static_cast<int>(aliceKey[i])
//                   << " | Bob receives: " << static_cast<int>(bobKey[i]) << "\n";
//     }
// }

// void checkKeyAgreement(const std::vector<char>& aliceKey, const std::vector<char>& bobKey) {
//     int errors = 0;
//     std::cout << "\nBit-by-bit Comparison:\n";
//     std::cout << "Index | Alice | Bob | Result | Bit Context\n";
//     std::cout << "----------------------------------------------\n";
//     for (int i = 0; i < aliceKey.size(); ++i) {
//         std::string result = (aliceKey[i] == bobKey[i]) ? "Match " : "Mismatch";
//         if (aliceKey[i] != bobKey[i]) errors++;

//         std::string context = "";
//         for (int j = i - 2; j <= i + 2; ++j) {
//             if (j >= 0 && j < aliceKey.size())
//                 context += std::to_string(aliceKey[j]);
//             else
//                 context += "-";
//         }

//         std::cout << std::setw(5) << i
//                   << " |   " << static_cast<int>(aliceKey[i])
//                   << "   |  " << static_cast<int>(bobKey[i])
//                   << " | " << result
//                   << " | " << context << "\n";
//     }

//     double qber = static_cast<double>(errors) / aliceKey.size() * 100.0;
//     std::cout << "\nTotal Errors: " << errors << " / " << aliceKey.size()
//               << "  |  QBER: " << std::fixed << std::setprecision(2) << qber << "%\n";

//     if (errors == 0)
//         std::cout << "Keys match perfectly. Proceed to privacy amplification.\n";
//     else
//         std::cout << "Keys contain mismatches. Proceeding to LDPC-based error correction...\n";
// }

// int main() {
//     std::srand(static_cast<unsigned>(std::time(nullptr)));

//     std::vector<char> aliceKey(KEY_LENGTH);
//     std::vector<char> bobKey(KEY_LENGTH);

//     for (int i = 0; i < KEY_LENGTH; ++i) {
//         aliceKey[i] = rand() % 2;
//         bobKey[i] = aliceKey[i];
//     }

//     int totalErrors = static_cast<int>(KEY_LENGTH * ERROR_RATE);
//     for (int i = 0; i < totalErrors; ++i) {
//         int index = rand() % KEY_LENGTH;
//         bobKey[index] = !bobKey[index];
//     }

//     printStep("Alice Key", aliceKey);
//     printStep("Bob Key (Before Errors)", bobKey);

//     siftingSimulation(aliceKey, bobKey);
//     checkKeyAgreement(aliceKey, bobKey);

//     std::cout << "\nStep-3: Applying LDPC Error Correction...\n";

//     const char* pchkFile = "pchk.txt";
//     const char* genFile  = "gen.txt";
//     qkdtools::ldpcMgr decoder((char*)pchkFile, (char*)genFile);

//     char* parity = new char[KEY_LENGTH]();
//     char* data   = new char[KEY_LENGTH];

//     for (int i = 0; i < KEY_LENGTH; ++i)
//         data[i] = bobKey[i];

//     if (decoder.decodeParityAndData(parity, data)) {
//         char* correctedData = decoder.getDecodedData();

//         std::cout << "LDPC Decoding successful.\n";

//         std::cout << "\nStep-4: Privacy Amplification\n";
//         qkdtools::PrivAmp pa(PA_KEY_LENGTH);
//         pa.generateToeplitzMat();
//         pa.calcPAKey(correctedData, KEY_LENGTH);

//         char* outputKey = pa.getPAKey();
//         std::cout << "Final Secret Key: ";
//         for (int i = 0; i < PA_KEY_LENGTH; ++i) {
//             std::cout << static_cast<int>(outputKey[i]);
//         }
//         std::cout << std::endl;
//     } else {
//         std::cerr << "LDPC decoding failed. Cannot proceed to privacy amplification.\n";
//     }

//     delete[] parity;
//     delete[] data;

//     return 0;
// }
