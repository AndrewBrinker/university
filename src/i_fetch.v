`include "src/mux.v"
`include "src/incr.v"
`include "src/memory.v"
`include "src/if_id.v"
`include "src/pc_mod.v"

module i_fetch (
        output wire  [31:0] IF_ID_instruction,
        output wire  [31:0] IF_ID_npc,
        input  wire         EX_MEM_PC_source,
        input  wire  [31:0] EX_MEM_NPC
    );

    wire [31:0] PC;
    wire [31:0] data_out;
    wire [31:0] npc, npc_mux;

   //instantiations
   mux mux1 (.out(npc_mux),
             .a(EX_MEM_NPC),
             .b(npc),
             .sel(EX_MEM_PC_Source));

   pc_mod pc_mod1 (.PC(PC),
                   .npc(npc_mux));

   memory memory1 (.data(data_out),
                   .addr(PC));

   if_id  if_id1  (.instruction_out(IF_ID_instruction),
                   .npc_out(IF_ID_npc),
                   .instruction(data_out),
                   .npc(npc));

   incrementer incrementer1 (.out(npc),
                             .in(PC));

   initial begin
      $display("Time\t PC\t npc\t data_out of MEM\t IF_ID_instruction\t IF_ID_npc");
      $monitor("%0d\t %0d\t %0d\t %h\t %h\t %0d", $time, PC, npc, data_out, IF_ID_instruction, IF_ID_npc);
      #20 $finish;
   end

endmodule

