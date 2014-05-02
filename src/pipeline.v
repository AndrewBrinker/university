`include "src/fetch/i_fetch.v"
`include "src/decode/i_decode.v"

module pipeline ();
    wire [31:0] IF_ID_instrout;
    wire [31:0] IF_ID_npcout;
    reg         EX_MEM_PCSrc;
    reg  [31:0] EX_MEM_NPC;
    reg  [4:0]  regwrite;

    initial begin
        EX_MEM_PCSrc <= 1'b0;
        EX_MEM_NPC   <= 32'b0;
        regwrite     <= 5'b0;
    end

    i_fetch i_fetch1 (.EX_MEM_PCSrc(EX_MEM_PCSrc),
                      .EX_MEM_NPC(EX_MEM_NPC),
                      .IF_ID_instr(IF_ID_instrout),
                      .IF_ID_npc(IF_ID_npcout));

    i_decode i_decode1 (.IF_ID_instrout(IF_ID_instrout),
                        .IF_ID_npcout(IF_ID_npcout),
                        .MEM_WB_rd(regwrite),
                        .MEM_WB_regwrite(EX_MEM_PCSrc),
                        .WB_mux5_writedata(EX_MEM_NPC));

endmodule
