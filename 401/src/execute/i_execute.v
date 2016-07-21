`include "src/execute/adder.v"
`include "src/execute/alu.v"
`include "src/execute/alu_control.v"
`include "src/execute/ex_mem.v"

module i_execute (
        input   wire [1:0]  wb_ctl,
        input   wire [2:0]  m_ctl,
        input   wire        regdst,
                            alusrc,
        input   wire [1:0]  aluop,
        input   wire [31:0] npcout,
                            rdata1,
                            rdata2,
                            s_extendout,
        input   wire [4:0]  instrout_2016,
                            instrout_1511,

        output  wire [1:0]  wb_ctlout,
        output  wire        branch,
                            memread,
                            memwrite,
        output  wire [31:0] EX_MEM_NPC,
        output  wire        zero,
        output  wire [31:0] alu_result,
                            rdata2out,
        output  wire [4:0]  five_bit_muxout
    );

    wire [31:0] adder_out,
                b,
                aluout;
    wire [4:0]  muxout;
    wire [2:0]  control;
    wire        aluzero;

    // Wire everything up.

    adder adder1(.add_in1(npcout),
                 .add_in2(s_extendout),
                 .add_out(adder_out));

    alu_control alu_control1(.funct(s_extendout[5:0]),
                             .aluop(aluop),
                             .select(control));

    mux #(.SIZE(5)) mux2(.a(instrout_2016),
                         .b(instrout_1511),
                         .select(regdst),
                         .y(muxout));

    mux mux3(.a(rdata2),
             .b(rdata1),
             .select(alusrc),
             .y(b));

    alu alu1(.a(rdata1),
             .b(b),
             .control(control),
             .result(aluout),
             .zero(aluzero));

    ex_mem ex_mem1(.ctlwb_out(wb_ctl),
                   .ctlm_out(m_ctl),
                   .adder_out(adder_out),
                   .aluzero(aluzero),
                   .aluout(aluout),
                   .readdat2(rdata2),
                   .muxout(muxout),
                   .wb_ctlout(wb_ctlout),
                   .branch(branch),
                   .memread(memread),
                   .memwrite(memwrite),
                   .add_result(EX_MEM_NPC),
                   .zero(zero),
                   .alu_result(alu_result),
                   .rdata2out(rdata2out),
                   .five_bit_muxout(five_bit_muxout));

endmodule
