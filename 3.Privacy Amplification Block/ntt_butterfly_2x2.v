module ntt_butterfly_2x2 (
    input clk_100Mhz,
    input [23:0] fi_0,
    input [23:0] fi_1,
    output [23:0] Fi_0,
    output [23:0] Fi_1
);

    // Twiddle factors
    parameter C_POS_R_128 = 24'd4808194;
    parameter C_NEG_R_128 = 24'd3572223;

    // Internal wires
    wire [48:0] sum0;
    wire [48:0] sum1;
    wire [47:0] prod0;
    wire [47:0] prod1;

    // Multipliers
    mult_gen_0 mult_gen_0i (
        .CLK(clk_100Mhz),
        .A(fi_1),
        .B(C_POS_R_128),
        .P(prod0)
    );

    mult_gen_0 mult_gen_1i (
        .CLK(clk_100Mhz),
        .A(fi_1),
        .B(C_NEG_R_128),
        .P(prod1)
    );

    // Adders
    c_add_0 c_add_0i (
        .A(prod0),
        .B(fi_0),
        .CLK(clk_100Mhz),
        .CE(1'b1),
        .S(sum0)
    );

    c_add_0 c_add_1i (
        .A(prod1),
        .B(fi_0),
        .CLK(clk_100Mhz),
        .CE(1'b1),
        .S(sum1)
    );

    // Barrett reducers
    barret barret_0i (
        .clk_100Mhz(clk_100Mhz),
        .a_in(sum0),
        .a_out(Fi_0)
    );

    barret barret_1i (
        .clk_100Mhz(clk_100Mhz),
        .a_in(sum1),
        .a_out(Fi_1)
    );

endmodule
