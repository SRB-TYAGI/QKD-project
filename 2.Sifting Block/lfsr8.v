module lfsr8 (
    input wire clk,
    input wire rst,
    output wire rand_bit,
    output wire rand_bit2
);
    reg [7:0] lfsr;
    always @(posedge clk or posedge rst) begin
        if (rst)
            lfsr <= 8'b10101010;
        else
            lfsr <= {lfsr[6:0], lfsr[7] ^ lfsr[5]};
    end
    assign rand_bit = lfsr[0];
    assign rand_bit2 = lfsr[1];
endmodule