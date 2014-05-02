`include "src/fetch/i_fetch.v"
`include "src/decode/i_decode.v"
// `include "src/execute/i_execute.v"

`timescale 1ns / 1ps

module pipeline ();
    wire [31:0] IF_ID_instrout;
    wire [31:0] IF_ID_npcout;

    i_fetch i_fetch1 (.EX_MEM_PCSrc(1'b0),
                      .EX_MEM_NPC(32'b0),
                      .IF_ID_instr(IF_ID_instrout),
                      .IF_ID_npc(IF_ID_npcout));

    i_decode i_decode1 (.IF_ID_instruction(IF_ID_instrout),
                        .IF_ID_npc(IF_ID_npcout),
                        .MEM_WB_rd(5'b0),
                        .MEM_WB_reg_write(1'b0),
                        .WB_mux5_write_data(32'b0));

endmodule
