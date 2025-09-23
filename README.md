# 🔐 Digital VLSI Circuit Design, ASIC Chip Design and FPGA Prototyping of Post-Processing tasks for Quantum Key Distribution Systems

Classical post-processing simulation for **Quantum Key Distribution (QKD)** based on the **BB84 protocol**.  
This project is implemented in **C++** is used to simulate classical post-quantum steps, while hardware modules are implemented in Verilog.” and simulates the **classical steps after quantum transmission**:

---
# Introduction
This project focuses on **Quantum Key Distribution (QKD)**, a method for secure key sharing immune to quantum attacks. The raw key from a QKD system contains errors and redundancy, requiring post-processing steps: **Sifting**, **Error Estimation**, **Error Correction**, and **Privacy Amplification (PA)**.

The algorithms for **Sifting** and **PA** were first implemented and tested in **C/C++**, allowing rapid prototyping and functional simulation. After verifying correctness, the design was migrated to hardware using **Verilog HDL** and tested on **Xilinx Vivado**, achieving real-time performance with low latency.

This work demonstrates the transition from **software simulation** to **digital VLSI implementation**, providing an efficient and scalable solution for secure quantum communication.

# Introduction to QKD

## What is QKD?

**Quantum Key Distribution (QKD)** is a method for generating and securely sharing a cryptographic key between two parties, traditionally called **Alice** (sender) and **Bob** (receiver), using the principles of quantum mechanics.  

Unlike classical key exchange protocols (e.g., RSA or Diffie-Hellman), QKD is theoretically unbreakable due to the **no-cloning theorem** and **quantum uncertainty**.

The most common protocol is **BB84**, where photons are encoded with quantum states (polarizations), and measurement outcomes depend on the chosen basis.

**Example:**
- Alice sends quantum bits (**qubits**) encoded in random bases.  
- Bob measures these qubits in random bases.  
- Only when their bases match do they keep the result, forming the **sifted key**.

## II. Major Steps in QKD Post-Processing

The standard post-processing pipeline in most QKD protocols consists of:

1. **Sifting**  
   Keep only the bits where Alice's and Bob's measurement bases match, forming the **sifted key**.

2. **Parameter Estimation / QBER Calculation**  
   Estimate the **Quantum Bit Error Rate (QBER)** to assess the level of eavesdropping or channel noise.

3. **Error Correction (Information Reconciliation)**  
   Correct errors in the sifted key due to channel noise or imperfections in photon transmission.

4. **Privacy Amplification (Toeplitz matrix)**  
   Compress the corrected key using universal hashing techniques to produce a **final secure key**, removing any partial information an eavesdropper may have.

## QKD Post-Processing Workflow

The following figure illustrates the post-processing steps in a typical QKD protocol, showing the flow from quantum transmission to the final secure key:

<p align="center">
  <img src="https://raw.githubusercontent.com/SRB-TYAGI/QKD-project/main/Images/QKD%20Post-Processing%20Workflow.png" alt="QKD Post-Processing Workflow" width="600"/>
</p>


## ✨ Features

- ✅ Random raw key generation (Alice & Bob)  
- ✅ Step-by-step **sifting output**  
- ✅ **QBER calculation** with mismatch reporting  
- ✅ **Privacy Amplification** via Toeplitz matrix  
- ⚠️ LDPC-based error correction (scaffold exists, integration needed)  
- ✅ Thread-based architecture (`ThreadMgr`) for real-time pipeline  

---


## 📂 Project Structure

