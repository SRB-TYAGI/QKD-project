#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

void write_int(std::ofstream &out, int value) {
    for (int i = 0; i < 4; ++i) {
        unsigned char byte = value & 0xFF;
        out.put(byte);
        value >>= 8;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: write_pchk <input.txt> <output.pchk>" << std::endl;
        return 1;
    }

    std::ifstream infile(argv[1]);
    std::ofstream outfile(argv[2], std::ios::binary);

    if (!infile) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    if (!outfile) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    std::string line;
    int nrows = 0, ncols = 0;
    std::vector<std::vector<int>> matrix;

    while (std::getline(infile, line)) {
        if (line.empty() || line[0] == 'c') continue;

        std::istringstream iss(line);
        if (line[0] == 'p') {
            std::string tmp;
            iss >> tmp >> nrows >> ncols;
            matrix.resize(nrows);
        } else {
            int deg, val;
            iss >> deg;
            std::vector<int> row;
            for (int i = 0; i < deg; ++i) {
                iss >> val;
                row.push_back(val);
            }
            matrix.push_back(row);
        }
    }

    write_int(outfile, nrows);  // M
    write_int(outfile, ncols);  // N

    for (const auto& row : matrix) {
        write_int(outfile, row.size());
        for (int col : row) {
            write_int(outfile, col);
        }
    }

    std::cout << "Successfully wrote binary parity-check matrix to " << argv[2] << std::endl;
    return 0;
}
