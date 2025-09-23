//// File: tb_fixed_point_multiplier.v
//`timescale 1ns / 1ps

//module tb_fixed_point_multiplier;

//    reg signed [31:0] a, b;               // Q4.28 inputs
//    wire signed [63:0] result;            // Q4.28 result

//    real a_real, b_real, result_real;     // For printing

//    // Instantiate multiplier
//    fixed_point_multiplier uut (
//        .a(a),
//        .b(b),
//        .result(result)
//    );

//    initial begin
//        // Q4.28 representation: 5.1125 * 2^28 = 137741403.2 ? 137741403
//        a = 32'sd137741403;  // 5.1125
//        b = 32'sd137741403;  // 5.1125

//        #10; // Wait for multiplier to settle

//        // Convert fixed-point to real for display
//        a_real = $itor(a) / (1 << 28);
//        b_real = $itor(b) / (1 << 28);
//        result_real = $itor(result) / (1 << 28);

//        $display("Input A (Q4.28) = %0d --> %f", a, a_real);
//        $display("Input B (Q4.28) = %0d --> %f", b, b_real);
//        $display("Result  (Q4.28) = %0d --> %f", result, result_real);
//        $display("Result Binary   = %b", result);

//        $finish;
//    end

//endmodule


module tb_fft8_fixed_q4_28;

    reg signed [31:0] x_real [0:7];
    wire signed [63:0] X_real [0:7];

    fft8_fixed_q4_28 uut (
        .x0_real(x_real[0]), .x1_real(x_real[1]), .x2_real(x_real[2]), .x3_real(x_real[3]),
        .x4_real(x_real[4]), .x5_real(x_real[5]), .x6_real(x_real[6]), .x7_real(x_real[7]),
        .X0_real(X_real[0]), .X1_real(X_real[1]), .X2_real(X_real[2]), .X3_real(X_real[3]),
        .X4_real(X_real[4]), .X5_real(X_real[5]), .X6_real(X_real[6]), .X7_real(X_real[7])
    );

    integer i;
    real x_float, x_scaled;

    initial begin
        $display("==== FFT 8-point Testbench (Q4.28) ====");

        // Initialize input: 4 ones followed by 4 zeros
        for (i = 0; i < 8; i = i + 1) begin
            if (i < 4)
                x_real[i] = 32'sd268435456; // 1.0 in Q4.28
            else
                x_real[i] = 32'sd0;
        end

        #20;

        for (i = 0; i < 8; i = i + 1) begin
            $display("X[%0d]_real = %0d (%.6f)", i, X_real[i], $itor(X_real[i]) / 268435456.0);
        end

        $finish;
    end

endmodule