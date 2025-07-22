# Enable cycle counter if using start/done signals
clocked_en = 0

def butterfly_gen(n):
    r = 1753
    q = 8380417
    bit_width = 24
    num_cycles = 15
    cycles_width = 4
    barret_count = 0

    clk_signal = "clk_100Mhz"
    rst_signal = "rst_n"
    str_signal = "start"
    dne_signal = "done"

    exp_factor_list = []

    # Open file for writing
    butterfly_file_name = f"ntt_butterfly_{n}x{n}"
    butterfly_file = open(butterfly_file_name + ".txt", "w")

    # Module Declaration
    butterfly_file.write(f"module {butterfly_file_name}(\n")
    butterfly_file.write(f"\tinput {clk_signal},\n")
    if clocked_en:
        butterfly_file.write(f"\tinput {rst_signal},\n")
        butterfly_file.write(f"\tinput {str_signal},\n")

    # Inputs
    for i in range(n):
        butterfly_file.write(f"\tinput [{bit_width - 1}:0] fi_{i},\n")

    # Outputs
    for i in range(n):
        line_end = ",\n" if i != n - 1 else "\n"
        butterfly_file.write(f"\toutput [{bit_width - 1}:0] Fi_{i}{line_end}")

    if clocked_en:
        butterfly_file.write(f"\toutput {dne_signal}\n")
    butterfly_file.write("\t);\n\n")

    # Twiddle factor parameters
    for k in range(int(n / 2)):
        exp_factor = int((256 / n) * (2 * k + 1))
        pos_twid_factor = pow(r, exp_factor, q)
        neg_twid_factor = q - pos_twid_factor

        butterfly_file.write(f"\tparameter C_POS_R_{exp_factor} = 24'd{pos_twid_factor};\n")
        butterfly_file.write(f"\tparameter C_NEG_R_{exp_factor} = 24'd{neg_twid_factor};\n")
        exp_factor_list.append(exp_factor)

    if clocked_en:
        butterfly_file.write(f"\tparameter C_CYCLES_TO_WAIT = {cycles_width}'d{num_cycles};\n\n")

    # Sum wire declarations
    for i in range(n):
        butterfly_file.write(f"\twire [48:0] sum{i};\n")
    butterfly_file.write("\n")

    # Product wire declarations
    for i in range(n):
        butterfly_file.write(f"\twire [{2 * bit_width - 1}:0] prod{i};\n")
    butterfly_file.write("\n")

    # Clock counter register (if clocked)
    if clocked_en:
        butterfly_file.write(f"\treg [{cycles_width - 1}:0] clk_count;\n\n")

    # Multiplier instantiations
    for i in range(n):
        butterfly_file.write(f"\tmult_gen_0 mult_gen_{i} (\n")
        butterfly_file.write(f"\t\t.CLK({clk_signal}),\n")
        a_input = f"fi_{i + int(n / 2)}" if i < int(n / 2) else f"fi_{i - int(n / 2)}"
        b_input = f"C_POS_R_{exp_factor_list[i]}" if i < int(n / 2) else f"C_NEG_R_{exp_factor_list[i - int(n / 2)]}"
        butterfly_file.write(f"\t\t.A({a_input}),\n")
        butterfly_file.write(f"\t\t.B({b_input}),\n")
        butterfly_file.write(f"\t\t.P(prod{i})\n")
        butterfly_file.write("\t);\n\n")

    # Adders
    for i in range(n):
        a_input = f"prod{i}"
        b_input = f"fi_{i}" if i < int(n / 2) else f"fi_{i - int(n / 2)}"
        butterfly_file.write(f"\tc_add_0 c_add_{i} (\n")
        butterfly_file.write(f"\t\t.A({a_input}),\n")
        butterfly_file.write(f"\t\t.B({b_input}),\n")
        butterfly_file.write(f"\t\t.CLK({clk_signal}),\n")
        butterfly_file.write(f"\t\t.CE(1'b1),\n")
        butterfly_file.write(f"\t\t.S(sum{i})\n")
        butterfly_file.write("\t);\n\n")

    # Barrett reduction instantiations
    for i in range(n):
        butterfly_file.write(f"\tbarret barret_{barret_count} (\n")
        butterfly_file.write(f"\t\t.clk_100Mhz({clk_signal}),\n")
        butterfly_file.write(f"\t\t.a_in(sum{i}),\n")
        butterfly_file.write(f"\t\t.a_out(Fi_{i})\n")
        butterfly_file.write("\t);\n\n")
        barret_count += 1

    # Done signal generation logic
    if clocked_en:
        butterfly_file.write(f"\tassign {dne_signal} = (clk_count == C_CYCLES_TO_WAIT) ? 1'b1 : 1'b0;\n\n")

        # Clock counter logic
        butterfly_file.write(f"\talways @(posedge {clk_signal}) begin\n")
        butterfly_file.write(f"\t\tif ({rst_signal} == 1'b1) begin\n")
        butterfly_file.write(f"\t\t\tclk_count <= {cycles_width}'d0;\n")
        butterfly_file.write(f"\t\tend else begin\n")
        butterfly_file.write(f"\t\t\tif (clk_count == C_CYCLES_TO_WAIT) begin\n")
        butterfly_file.write(f"\t\t\t\tclk_count <= {cycles_width}'d0;\n")
        butterfly_file.write(f"\t\t\tend else if ({str_signal} == 1'b1) begin\n")
        butterfly_file.write(f"\t\t\t\tclk_count <= clk_count + {cycles_width}'d1;\n")
        butterfly_file.write(f"\t\t\tend\n")
        butterfly_file.write(f"\t\tend\n")
        butterfly_file.write(f"\tend\n\n")

    # Endmodule
    butterfly_file.write("endmodule\n")
    butterfly_file.close()

# Generate butterfly modules for various sizes
for size in [2, 4, 8, 16, 32, 64, 128, 256]:
    butterfly_gen(size)
