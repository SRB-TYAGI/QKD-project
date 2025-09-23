//// File: fixed_point_multiplier.v
//module fixed_point_multiplier (
//    input  wire signed [31:0] a,       // Q4.28 input
//    input  wire signed [31:0] b,       // Q4.28 input
//    output wire signed [63:0] result   // Q4.28 output (after shifting)
//);

//    wire signed [63:0] mult_full;

//    assign mult_full = a * b;         // Q4.28 * Q4.28 = Q8.56
//    assign result = mult_full >>> 28; // Convert back to Q4.28 (preserve sign)

//endmodule



// File: fft8_with_multiplier.v
// 8-point FFT (real input only) using Q4.28 fixed-point arithmetic

module fixed_point_multiplier (
    input  wire signed [31:0] a,       // Q4.28 input
    input  wire signed [31:0] b,       // Q4.28 input
    output wire signed [63:0] result   // Q4.28 output (after scaling)
);

    wire signed [63:0] mult_full;
    assign mult_full = a * b;             // Q4.28 * Q4.28 = Q8.56
    assign result = mult_full >>> 28;     // Scale back to Q4.28

endmodule


module fft8_fixed_q4_28 (
    input  wire signed [31:0] x0_real,
    input  wire signed [31:0] x1_real,
    input  wire signed [31:0] x2_real,
    input  wire signed [31:0] x3_real,
    input  wire signed [31:0] x4_real,
    input  wire signed [31:0] x5_real,
    input  wire signed [31:0] x6_real,
    input  wire signed [31:0] x7_real,
    output wire signed [63:0] X0_real,
    output wire signed [63:0] X1_real,
    output wire signed [63:0] X2_real,
    output wire signed [63:0] X3_real,
    output wire signed [63:0] X4_real,
    output wire signed [63:0] X5_real,
    output wire signed [63:0] X6_real,
    output wire signed [63:0] X7_real
);

    wire signed [31:0] stage0 [0:7];
    assign stage0[0] = x0_real;
    assign stage0[1] = x1_real;
    assign stage0[2] = x2_real;
    assign stage0[3] = x3_real;
    assign stage0[4] = x4_real;
    assign stage0[5] = x5_real;
    assign stage0[6] = x6_real;
    assign stage0[7] = x7_real;

    // Stage 1: Butterfly + Twiddle multiply
    wire signed [31:0] bf0 = stage0[0] + stage0[4];
    wire signed [31:0] bf1 = stage0[1] + stage0[5];
    wire signed [31:0] bf2 = stage0[2] + stage0[6];
    wire signed [31:0] bf3 = stage0[3] + stage0[7];
    wire signed [31:0] bf4 = stage0[0] - stage0[4];
    wire signed [31:0] bf5 = stage0[1] - stage0[5];
    wire signed [31:0] bf6 = stage0[2] - stage0[6];
    wire signed [31:0] bf7 = stage0[3] - stage0[7];

    // Simplified FFT: Only real part, assume twiddle = 1 (for example purposes)
    wire signed [63:0] temp0, temp1, temp2, temp3, temp4, temp5, temp6, temp7;

    fixed_point_multiplier m0 (.a(bf0), .b(32'sd268435456), .result(temp0)); // *1.0
    fixed_point_multiplier m1 (.a(bf1), .b(32'sd268435456), .result(temp1));
    fixed_point_multiplier m2 (.a(bf2), .b(32'sd268435456), .result(temp2));
    fixed_point_multiplier m3 (.a(bf3), .b(32'sd268435456), .result(temp3));
    fixed_point_multiplier m4 (.a(bf4), .b(32'sd268435456), .result(temp4));
    fixed_point_multiplier m5 (.a(bf5), .b(32'sd268435456), .result(temp5));
    fixed_point_multiplier m6 (.a(bf6), .b(32'sd268435456), .result(temp6));
    fixed_point_multiplier m7 (.a(bf7), .b(32'sd268435456), .result(temp7));

    assign X0_real = temp0;
    assign X1_real = temp1;
    assign X2_real = temp2;
    assign X3_real = temp3;
    assign X4_real = temp4;
    assign X5_real = temp5;
    assign X6_real = temp6;
    assign X7_real = temp7;

endmodule