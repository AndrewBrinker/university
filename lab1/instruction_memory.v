// 128 32-bit word memory. It takes in the program counter
// and returns the contents of that memory location
module instruction_memory(pc, out);
    input  [31:0] pc;
    output [31:0] out;
    reg [31:0] mem[0:127];
    out = mem[pc];
endmodule