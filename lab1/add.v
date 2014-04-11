// Adds 1 to the program counter
module add(in, value, out);
    input  [31:0] in, value;
    output [31:0] out;
    assign out = in + value;
endmodule