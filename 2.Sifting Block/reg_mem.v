module reg_mem (
    input wire clk,
    input wire we,
    input wire [9:0] addr,
    input wire [0:0] data_a,
    input wire [0:0] data_b,
    output reg [0:0] out_a,
    output reg [0:0] out_b
);
    reg [0:0] bram_a [0:1023];
    reg [0:0] bram_b [0:1023];

    always @(posedge clk) begin
        if (we) begin
            bram_a[addr] <= data_a;
            bram_b[addr] <= data_b;
        end
        out_a <= bram_a[addr];
        out_b <= bram_b[addr];
    end
endmodule