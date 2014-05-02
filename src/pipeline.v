`include "src/fetch/i_fetch.v"
`include "src/decode/i_decode.v"

module pipeline ();
    wire [31:0] IF_ID_instruction_out;
    wire [31:0] IF_ID_npc_out;

    i_fetch i_fetch1 (.IF_ID_instruction(IF_ID_instruction_out),
                      .IF_ID_npc(IF_ID_npc_out));

    i_decode i_decode1 (.IF_ID_instruction(IF_ID_instruction_out),
                        .IF_ID_npc(IF_ID_npc_out));

endmodule
