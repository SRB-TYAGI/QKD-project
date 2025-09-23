//`timescale 1ns / 1ps

//module tb_ntt_butterfly_2x2;

//    reg clk_100Mhz;
//    reg [23:0] fi_0, fi_1;
//    wire [23:0] Fi_0, Fi_1;

//    // Instantiate DUT
//    ntt_butterfly_2x2 uut (
//        .clk_100Mhz(clk_100Mhz),
//        .fi_0(fi_0),
//        .fi_1(fi_1),
//        .Fi_0(Fi_0),
//        .Fi_1(Fi_1)
//    );

//    // Generate 100 MHz clock
//    always #5 clk_100Mhz = ~clk_100Mhz;

//    initial begin
//        $display("Time\tfi_0\tfi_1\t=> Fi_0\tFi_1");
//        $monitor("%dns\t%d\t%d\t=> %d\t%d", $time, fi_0, fi_1, Fi_0, Fi_1);

//        clk_100Mhz = 0;

//        // Test Vector 1
//        fi_0 = 24'd123456;
//        fi_1 = 24'd654321;
//        #100;

//        // Test Vector 2
//        fi_0 = 24'd222222;
//        fi_1 = 24'd111111;
//        #100;

//        // Test Vector 3
//        fi_0 = 24'd8380416;  // q - 1
//        fi_1 = 24'd1;
//        #100;

//        $finish;
//    end
//endmodule



`timescale 1ns / 1ps

module tb_ntt_butterfly_2x2;

  // Inputs
  reg clk;
  reg [23:0] fi_0;
  reg [23:0] fi_1;

  // Outputs
  wire [23:0] Fi_0;
  wire [23:0] Fi_1;

  // Instantiate the module under test (MUT)
  ntt_butterfly_2x2 uut (
    .clk_100Mhz(clk),
    .fi_0(fi_0),
    .fi_1(fi_1),
    .Fi_0(Fi_0),
    .Fi_1(Fi_1)
  );

  // Clock generation
  initial clk = 0;
  always #10 clk = ~clk; // 100 MHz clock => 10ns half-period

  // Test inputs
  initial begin
    $display("Time\tfi_0\tfi_1\t=> Fi_0\t\tFi_1");
    $monitor("%t\t%d\t%d\t=> %d\t%d", $time, fi_0, fi_1, Fi_0, Fi_1);

    fi_0 = 0; fi_1 = 0;
    #100;

    fi_0 = 5; fi_1 = 3;
    #100;

    fi_0 = 9; fi_1 = 6;
    #100;

    fi_0 = 8380416; fi_1 = 1;
    #100;

    $finish;
  end

endmodule

