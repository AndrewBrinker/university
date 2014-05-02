`include "src/fetch/mux.v"
`include "src/fetch/incr.v"
`include "src/fetch/memory.v"
`include "src/fetch/if_id.v"
`include "src/fetch/pc_mod.v"

module i_fetch (
        input  wire        EX_MEM_PCSrc,
        input  wire [31:0] EX_MEM_NPC,

        output wire [31:0] IF_ID_instr,
        output wire [31:0] IF_ID_npc
    );

    wire [31:0] PC,
                dataout,
                npc,
                npc_mux;

    mux mux1 (.y(npc_mux),
              .a(EX_MEM_NPC),
              .b(npc),
              .select(EX_MEM_PCSrc));

    pc_mod pc_mod1 (.PC(PC),
                    .npc(npc_mux));

    memory memory1 (.data(dataout),
                    .addr(PC));

    if_id  if_id1 (.instrout(IF_ID_instr),
                   .npcout(IF_ID_npc),
                   .instr(dataout),
                   .npc(npc));

    incrementer incrementer1 (.pcout(npc),
                              .pcin(PC));
endmodule
