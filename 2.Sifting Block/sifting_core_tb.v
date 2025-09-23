`timescale 1ns / 1ps

module qkd_sifting_bram_fsm_tb;

    reg clk;
    reg rst;
    reg valid;
    reg manual_mode;
    reg [0:0] alice_basis_in;
    reg [0:0] bob_basis_in;
    reg [0:0] alice_bit_in;
    reg [0:0] bob_bit_in;

    wire write_enable;
    wire [9:0] addr;
    wire done;
    wire [0:0] sifted_key_a;
    wire [0:0] sifted_key_b;
    wire [9:0] qber_errors;
    wire [9:0] qber_total;

    qkd_sifting_bram_fsm dut (
        .clk(clk),
        .rst(rst),
        .valid(valid),
        .manual_mode(manual_mode),
        .alice_basis_in(alice_basis_in),
        .bob_basis_in(bob_basis_in),
        .alice_bit_in(alice_bit_in),
        .bob_bit_in(bob_bit_in),
        .write_enable(write_enable),
        .addr(addr),
        .done(done),
        .sifted_key_a(sifted_key_a),
        .sifted_key_b(sifted_key_b),
        .qber_errors(qber_errors),
        .qber_total(qber_total)
    );

    always #5 clk = ~clk;

    initial begin
        clk = 0;
        rst = 1;
        valid = 0;
        manual_mode = 0;
        alice_basis_in = 0;
        bob_basis_in   = 0;
        alice_bit_in   = 0;
        bob_bit_in     = 0;

        $display(" Starting Simulation...");

        #20 rst = 0;

        repeat (40) begin
            @(negedge clk);
            valid = 1;
            alice_basis_in = $random % 2;
            bob_basis_in   = $random % 2;
            alice_bit_in   = $random % 2;
            bob_bit_in     = $random % 2;
            @(negedge clk);
            valid = 0;
        end
    end

    initial begin
        wait(done == 1);

        $display("\n==== QKD Sifting Result ====");
        $display("Total sifted bits : %0d", qber_total);
        $display("Error bits        : %0d", qber_errors);
        if (qber_total > 0)
            $display("QBER (%%)          : %0.2f%%", (qber_errors * 100.0) / qber_total);
        else
            $display("QBER              : N/A (no sifted bits)");

        $finish;
    end

    initial begin
        #200000;
        $display("\n Timeout reached at %0t ns - ending simulation forcefully", $time);
        $finish;
    end

endmodule
