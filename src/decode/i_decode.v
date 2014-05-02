`include "src/decode/reg.v"
`include "src/decode/control.v"
`include "src/decode/id_ex.v"
`include "src/decode/s_extend.v"

module i_decode (
        input   wire [31:0] IF_ID_instrout,
        input   wire [31:0] IF_ID_npcout,
        input   wire [4:0]  MEM_WB_rd,
        input   wire        MEM_WB_regwrite,
        input   wire [31:0] WB_mux5_writedata,

        output  wire [1:0]  wb_ctlout,
        output  wire [2:0]  m_ctlout,
        output  wire        regdst,
                            alusrc,
        output  wire [1:0]  aluop,
        output  wire [31:0] npcout,
                            rdata1out,
                            rdata2out,
                            s_extendout,
        output  wire [4:0]  instrout_2016,
                            instrout_1511
    );

    wire [3:0]  ctlex_out;
    wire [2:0]  ctlm_out;
    wire [1:0]  ctlwb_out;
    wire [31:0] readdat1,
                readdat2,
                signext_out;

    control control1 (.opcode(IF_ID_instrout[31:26]),
                      .EX(ctlex_out),
                      .M(ctlm_out),
                      .WB(ctlwb_out));

    register register1 (.rs(IF_ID_instrout[25:21]),
                        .rt(IF_ID_instrout[20:16]),
                        .rd(MEM_WB_rd),
                        .writedata(WB_mux5_writedata),
                        .regwrite(MEM_WB_regwrite),
                        .A(readdat1),
                        .B(readdat2));

    s_extend s_extend1 (.nextend(IF_ID_instrout[15:0]),
                        .extend(signext_out));

    id_ex id_ex1 (.ctlwb_out(ctlwb_out),
                  .ctlm_out(ctlm_out),
                  .ctlex_out(ctlex_out),
                  .npc(IF_ID_npcout),
                  .readdat1(readdat1),
                  .readdat2(readdat2),
                  .signext_out(signext_out),
                  .instr_2016(IF_ID_instrout[20:16]),
                  .instr_1511(IF_ID_instrout[15:11]),
                  .wb_ctlout(wb_ctlout),
                  .m_ctlout(m_ctlout),
                  .regdst(regdst),
                  .alusrc(alusrc),
                  .aluop(aluop),
                  .npcout(npcout),
                  .rdata1out(rdata1out),
                  .rdata2out(rdata2out),
                  .s_extendout(s_extendout),
                  .instrout_2016(instrout_2016),
                  .instrout_1511(instrout_1511));

    always @(IF_ID_instrout) begin
        $display("clock: %0d", $time);
        $display("wb_ctlout: %b", wb_ctlout);
        $display("m_ctlout: %b", m_ctlout);
        $display("regdst: %b", regdst);
        $display("alusrc: %b", alusrc);
        $display("aluop: %b", aluop);
        $display("npcout: %b", npcout);
        $display("rdata1out: %b", rdata1out);
        $display("rdata2out: %b", rdata2out);
        $display("signext_out: %b", signext_out);
        $display("instrout_2016: %b", instrout_2016);
        $display("instrout_1511: %b", instrout_1511);
        $display("");
    end
endmodule
