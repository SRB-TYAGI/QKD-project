// Dummy 49-bit multiplier
module mult_gen_49_bit(input CLK, input [48:0] A, output reg [72:0] P);
    always @ (posedge CLK)
        P <= A;
endmodule

// Dummy 27-bit multiplier
module mult_gen_27_bit(input CLK, input [26:0] A, output reg [49:0] P);
    always @ (posedge CLK)
        P <= A;
endmodule

// Dummy subtractor
module c_sub_51_bit(input [50:0] A, input [50:0] B, input CLK, input CE, output reg [50:0] S);
    always @ (posedge CLK)
        if (CE)
            S <= A - B;
endmodule
