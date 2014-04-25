`include "src/fetch/mux.v"
`include "src/fetch/incr.v"
`include "src/fetch/memory.v"
`include "src/fetch/if_id.v"
`include "src/fetch/pc_mod.v"

module i_fetch (
        output wire [31:0] IF_ID_instruction;
        output wire [31:0] IF_ID_npc;
    );

    reg         EX_MEM_PC_Source;
    reg  [31:0] EX_MEM_NPC;

    wire [31:0] PC,
                data_out,
                npc,
                npc_mux;

    //instantiations
    mux mux1 (.out(npc_mux),
              .pc(EX_MEM_NPC),
              .jump(npc),
              .select(EX_MEM_PC_Source));

    pc_mod pc_mod1 (.PC(PC),
                    .npc(npc_mux));

    memory memory1 (.data(data_out),
                    .addr(PC));

    if_id  if_id1 (.instruction_out(IF_ID_instruction),
                   .npc_out(IF_ID_npc),
                   .instruction(data_out),
                   .npc(npc));

    incrementer incrementer1 (.out(npc),
                              .in(PC));

    initial begin
        EX_MEM_PC_Source <= 1'b0;
        EX_MEM_NPC <= 32'b0;
        // $display("Time\t PC\t npc\t data of MEM\t IF_ID_instr\t IF_ID_npc");
        // $monitor("%0d\t %0d\t %0d\t %h\t %h\t %0d", $time, PC, npc, data_out, IF_ID_instruction, IF_ID_npc);
        // #20 $finish;
    end
endmodule
