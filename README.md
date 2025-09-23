# 🔐 Quantum Key Distribution (QKD) Post-Processing Project

Classical post-processing simulation for **Quantum Key Distribution (QKD)** based on the **BB84 protocol**.  
This project is implemented in **C++** and simulates the **classical steps after quantum transmission**:

1. **Sifting**  
2. **Parameter Estimation**  
3. **Information Reconciliation (LDPC planned)**  
4. **Privacy Amplification (Toeplitz matrix)**  

---

## 📌 Overview

In QKD, Alice and Bob generate raw keys using quantum states (e.g., BB84).  
Due to noise and eavesdropping, errors exist. Classical post-processing is needed to:

- Remove bits where measurement bases differ (**sifting**)  
- Estimate error rate (**parameter estimation**)  
- Correct mismatches using error-correcting codes (**information reconciliation**)  
- Compress the key to a secure shorter one (**privacy amplification**)  

This repo simulates that entire pipeline.

---

## ✨ Features

- ✅ Random raw key generation (Alice & Bob)  
- ✅ Step-by-step **sifting output**  
- ✅ **QBER calculation** with mismatch reporting  
- ✅ **Privacy Amplification** via Toeplitz matrix  
- ⚠️ LDPC-based error correction (scaffold exists, integration needed)  
- ✅ Thread-based architecture (`ThreadMgr`) for real-time pipeline  

---

## 📂 Project Structure

