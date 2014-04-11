`include "mux.v"
`include "add.v"
`include "instruction_memory.v"

module mips();
    // Declare the major registers.
    wire [31:0] IF_ID_IR, IF_ID_NPC, EX_MEM_NPC;
    wire PC_choose;

    // Initialize the things.
    assign IF_ID_IR   = 32'h0;
    assign IF_ID_NPC  = 32'h0;
    assign EX_MEM_NPC = 32'h0;
    assign PC_choose  =  1'b0;


endmodule
