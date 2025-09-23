module counter (
    input wire clk,
    input wire rst,
    input wire enable,
    output reg [9:0] count
);
    always @(posedge clk or posedge rst) begin
        if (rst)
            count <= 0;
        else if (enable)
            count <= count + 1;
    end
endmodule