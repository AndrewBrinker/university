`include "src/mux.v"
`include "src/incr.v"
`include "src/instruction_memory.v"

module mips();
    wire [31:0] PC, IF_ID_IR, IF_ID_NPC, EX_MEM_NPC;
    wire PC_choose;

    assign PC         = 32'h0;
    assign IF_ID_IR   = 32'h0;
    assign IF_ID_NPC  = 32'h0;
    assign EX_MEM_NPC = 32'h0;
    assign PC_choose  =  1'b0;
endmodule
