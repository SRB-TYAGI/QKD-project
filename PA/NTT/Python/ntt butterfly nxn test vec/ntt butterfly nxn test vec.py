# -*- coding: utf-8 -*-
"""
Created on Sun Apr 2 01:06:59 2023
@author: bvoc5
Generates test vectors for ntt_butterfly_nxn unit testbench
in CRYSTALS-Dilithium NTT FPGA Implementation
"""

import random
import math

q = 8380417
r = 1753
N = 8  # Size of the butterfly
test = "U"  # "U" -> Unit Test, "R" -> Regression Test
test_length = 8  # Number of test cases

# Butterfly instance: computes N-point butterfly
def ntt_nxn_butterfly_instance(fi, N_local):
    Fi = [0] * N_local
    f1 = fi[0 : int(N_local / 2)]
    f2 = fi[int(N_local / 2) : N_local]
    for k in range(int(N_local / 2)):
        twiddle = pow(r, int((256 / N_local) * (2 * k + 1)), q)
        Fi[k] = (f1[k] + twiddle * f2[k]) % q
        Fi[k + int(N_local / 2)] = (f1[k] - twiddle * f2[k]) % q
    return Fi

# Full butterfly wrapper
def ntt_nxn_butterfly(fi):
    Fi = [0] * N
    if test == "U":
        Fi = ntt_nxn_butterfly_instance(fi, N)
    elif test == "R":
        Fi_temp = fi
        for i in range(int(math.log(N, 2))):
            M1 = 2 ** (i + 1)
            M2 = int(N / M1)
            Fi_temp2 = []
            for j in range(M2):
                sub_fi = Fi_temp[M1 * j : M1 + M1 * j]
                Fi_temp2 += ntt_nxn_butterfly_instance(sub_fi, M1)
            Fi_temp = Fi_temp2
        Fi = Fi_temp
    return Fi

# Open file to write test vectors
filename = f"ntt_butterfly_{N}x{N}_test_vec.txt"
butterfly_test_vec_file = open(filename, "w")

butterfly_test_vec_file.write("// Testbench signal declarations\n\n")

# Write reg declarations for inputs
for i in range(N):
    butterfly_test_vec_file.write(f"\treg [23:0] fi_{i};\n")
butterfly_test_vec_file.write("\n")

# Write wire declarations for outputs
for i in range(N):
    butterfly_test_vec_file.write(f"\twire [23:0] Fi_{i};\n")
butterfly_test_vec_file.write("\n")

# Input test vector memory arrays
for i in range(N):
    butterfly_test_vec_file.write(f"\t// Input test vectors: fi_{i}\n")
    butterfly_test_vec_file.write(f"\treg [23:0] test_vec_in_fi_{i} [0:{test_length - 1}];\n")
butterfly_test_vec_file.write("\n")

# Expected output vector memory arrays
for i in range(N):
    butterfly_test_vec_file.write(f"\t// Expected output vectors: Fi_{i}\n")
    butterfly_test_vec_file.write(f"\treg [23:0] test_vec_exp_out_Fi_{i} [0:{test_length - 1}];\n")
butterfly_test_vec_file.write("\n")

# Write values to vector memory
for i in range(test_length):
    fi = [int((q - 1) * random.random()) for _ in range(N)]
    Fi = ntt_nxn_butterfly(fi)

    butterfly_test_vec_file.write(f"\tinitial begin // Test vector {i}\n")
    for j in range(N):
        butterfly_test_vec_file.write(f"\t\ttest_vec_in_fi_{j}[{i}] = 24'd{fi[j]};\n")
    butterfly_test_vec_file.write("\n")
    for j in range(N):
        butterfly_test_vec_file.write(f"\t\ttest_vec_exp_out_Fi_{j}[{i}] = 24'd{Fi[j]};\n")
    butterfly_test_vec_file.write("\tend\n\n")

# Close the file
butterfly_test_vec_file.close()
