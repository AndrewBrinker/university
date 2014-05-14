`include "src/execute/adder.v"
`include "src/execute/alu.v"
`include "src/execute/alu_control.v"
`include "src/execute/ex_mem.v"
`include "src/execute/five_bit_mux.v"

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

    five_bit_mux five_bit_mux1(.pc(instrout_2016),
                               .jump(instrout_1511),
                               .select(regdst),
                               .out(muxout));

    mux mux1(.a(rdata2),
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

    always @(instrout_2016 or instrout_1511) begin
        $display("clock: %0d", $time);
        $display("wb_ctl: %b", wb_ctl);
        $display("m_ctl: %b", m_ctl);
        $display("regdst: %b", regdst);
        $display("alusrc: %b", alusrc);
        $display("aluop: %b", aluop);
        $display("npcout: %b", npcout);
        $display("rdata1: %b", rdata1);
        $display("rdata2: %b", rdata2);
        $display("s_extendout: %b", s_extendout);
        $display("instrout_2016: %b", instrout_2016);
        $display("instrout_1511: %b", instrout_1511);
        $display("wb_ctlout: %b", wb_ctlout);
        $display("branch: %b", branch);
        $display("memread: %b", memread);
        $display("memwrite: %b", memwrite);
        $display("EX_MEM_NPC: %b", EX_MEM_NPC);
        $display("zero: %b", zero);
        $display("alu_result: %b", alu_result);
        $display("rdata2out: %b", rdata2out);
        $display("five_bit_muxout: %b", five_bit_muxout);
        $display("");
    end
endmodule
