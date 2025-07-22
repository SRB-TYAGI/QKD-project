module qkd_sifting_bram_fsm (
    input wire clk,
    input wire rst,
    input wire valid,
    input wire manual_mode,
    input wire [0:0] alice_basis_in,
    input wire [0:0] bob_basis_in,
    input wire [0:0] alice_bit_in,
    input wire [0:0] bob_bit_in,
    output reg write_enable,
    output wire [9:0] addr,
    output reg done,
    output reg [0:0] sifted_key_a,
    output reg [0:0] sifted_key_b,
    output reg [9:0] qber_errors,
    output reg [9:0] qber_total
);

    localparam IDLE = 2'b00, CHECK = 2'b01, STORE = 2'b10, DONE = 2'b11;
    reg [1:0] current_state, next_state;

    reg [9:0] error_count;
    reg [9:0] count;
    reg [9:0] addr_reg;
    assign addr = addr_reg;

    // BRAM-style memory
    reg [0:0] alice_key_mem [0:1023];
    reg [0:0] bob_key_mem   [0:1023];

    // File handles
    integer file_a, file_b;

    // Random generators
    wire rand_ab, rand_bb, rand_abit, rand_bbit;
    lfsr8 alice_gen (.clk(clk), .rst(rst), .rand_bit(rand_abit), .rand_bit2(rand_ab));
    lfsr8 bob_gen   (.clk(clk), .rst(rst), .rand_bit(rand_bbit), .rand_bit2(rand_bb));

    wire [0:0] alice_basis, bob_basis, alice_bit, bob_bit;
    mux2 #(1) mux_ab (.sel(manual_mode), .a(alice_basis_in), .b(rand_ab), .y(alice_basis));
    mux2 #(1) mux_bb (.sel(manual_mode), .a(bob_basis_in), .b(rand_bb), .y(bob_basis));
    mux2 #(1) mux_abit(.sel(manual_mode), .a(alice_bit_in), .b(rand_abit), .y(alice_bit));
    mux2 #(1) mux_bbit(.sel(manual_mode), .a(bob_bit_in), .b(rand_bbit), .y(bob_bit));

    wire match;
    comparator comp_inst (.a(alice_basis), .b(bob_basis), .eq(match));

    always @(posedge clk or posedge rst) begin
        if (rst)
            current_state <= IDLE;
        else
            current_state <= next_state;
    end

    always @(*) begin
        case (current_state)
            IDLE:  next_state = valid ? CHECK : IDLE;
            CHECK: next_state = match ? STORE : IDLE;
            STORE: next_state = (count >= 10) ? DONE : IDLE;  //////////////////
            DONE:  next_state = DONE;
            default: next_state = IDLE;
        endcase
    end

    initial begin
file_a = $fopen("E:/VIVADO/project_1/alice_keys.txt", "w");
file_b = $fopen("E:/VIVADO/project_1/bob_keys.txt", "w");


    end

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            addr_reg <= 0;
            count <= 0;
            error_count <= 0;
            write_enable <= 0;
            done <= 0;
            qber_errors <= 0;
            qber_total <= 0;
        end else begin
            write_enable <= 0;

//            case (current_state)
//                STORE: begin
//                    write_enable <= 1;
//                    sifted_key_a <= alice_bit;
//                    sifted_key_b <= bob_bit;
//                    alice_key_mem[addr_reg] <= alice_bit;
//                    bob_key_mem[addr_reg]   <= bob_bit;
//                    $fwrite(file_a, "%b\n", alice_bit);
//                    $fwrite(file_b, "%b\n", bob_bit);
//                    if (alice_bit != bob_bit)
//                        error_count <= error_count + 1;
//                    addr_reg <= addr_reg + 1;
//                    count <= count + 1;

//                    $display(" SIFTED at Addr=%0d | ABit=%b BBit=%b | AB=%b BB=%b", addr_reg, alice_bit, bob_bit, alice_basis, bob_basis);
//                end

//                DONE: begin
//                    done <= 1;
//                    qber_errors <= error_count;
//                    qber_total <= count;
//                    $fclose(file_a);
//                    $fclose(file_b);
//                    $display(" FSM reached DONE at time = %0t", $time);
//                end
//            endcase
//        end

    case (current_state)
        STORE: begin
            if (match) begin
                write_enable <= 1;
                sifted_key_a <= alice_bit;
                sifted_key_b <= bob_bit;
                alice_key_mem[addr_reg] <= alice_bit;
                bob_key_mem[addr_reg]   <= bob_bit;
                $fwrite(file_a, "%b\n", alice_bit);
                $fwrite(file_b, "%b\n", bob_bit);

                if (alice_bit != bob_bit)
                    error_count <= error_count + 1;

                $display("SIFTED at Addr=%0d | ABit=%b BBit=%b | AB=%b BB=%b", addr_reg, alice_bit, bob_bit, alice_basis, bob_basis);

                addr_reg <= addr_reg + 1;
                count <= count + 1;
            end else begin
                $display("DISCARDED         | ABit=%b BBit=%b | AB=%b BB=%b (basis mismatch)", alice_bit, bob_bit, alice_basis, bob_basis);
            end
        end

        DONE: begin
            done <= 1;
            qber_errors <= error_count;
            qber_total <= count;
            $fclose(file_a);
            $fclose(file_b);
            $display("FSM reached DONE at time = %0t", $time);
        end
    endcase
end

    end
endmodule
