// #include <iostream>

// void checkKeyAgreement(bool* aliceKey, bool* bobKey, int keyLength) {
//     int errorCount = 0;

//     std::cout << "Comparing keys...\n";
//     for (int i = 0; i < keyLength; i++) {
//         if (aliceKey[i] != bobKey[i]) {
//             errorCount++;
//             std::cout << "Mismatch at bit " << i
//                       << " | Alice: " << (int)aliceKey[i]
//                       << " Bob: " << (int)bobKey[i] << "\n";
//         }
//     }

//     float qber = (float)errorCount / keyLength;
//     std::cout << "\nTotal Errors: " << errorCount
//               << " / " << keyLength
//               << " | QBER: " << qber * 100 << "%\n";

//     if (errorCount > 0) {
//         std::cout << "⚠️ Error correction failed. Keys are not synchronized.\n";
//     } else {
//         std::cout << "✅ Keys match perfectly. Proceed to privacy amplification.\n";
//     }
// }


#include <iostream>

void checkKeyAgreement(char* aliceKey, char* bobKey, int keyLength) {
    int errorCount = 0;

    std::cout << "Comparing keys...\n";
    for (int i = 0; i < keyLength; ++i) {
        if (aliceKey[i] != bobKey[i]) {
            if (errorCount < 10) {  // Only show first 10 mismatches
                std::cout << "Mismatch at bit " << i
                          << " | Alice: " << static_cast<int>(aliceKey[i])
                          << " Bob: " << static_cast<int>(bobKey[i]) << "\n";
            }
            errorCount++;
        }
    }

    float qber = static_cast<float>(errorCount) / keyLength;
    std::cout << "\nTotal Errors: " << errorCount
              << " / " << keyLength
              << " | QBER: " << qber * 100 << "%\n";

    if (errorCount > 0)
        std::cout << "⚠️ Error correction failed. Keys are not synchronized.\n";
    else
        std::cout << "✅ Keys match perfectly. Proceed to privacy amplification.\n";
}

