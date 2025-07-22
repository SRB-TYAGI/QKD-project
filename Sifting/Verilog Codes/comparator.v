module comparator (
    input wire a,
    input wire b,
    output wire eq
);
    assign eq = (a == b);
endmodule