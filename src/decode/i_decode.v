`include "src/decode/reg.v"
`include "src/decode/control.v"
`include "src/decode/id_ex.v"
`include "src/decode/s_extend.v"

module i_decode (
        input   wire [31:0] IF_ID_instruction,
        input   wire [31:0] IF_ID_npc,

        input   wire [4:0]  MEM_WB_rd,
        input   wire        MEM_WB_reg_write,
        input   wire [31:0] WB_mux5_write_data,

        output  wire [1:0]  wb_ctl_out,
        output  wire [2:0]  m_ctl_out,
        output  wire        reg_dst,
                            alu_src,
        output  wire [1:0]  alu_op,
        output  wire [31:0] npc_out,
                            r_data1_out,
                            r_data2_out,
                            sign_extend_out,
        output  wire [4:0]  instruction_out_2016,
                            instruction_out_1511
    );

    wire [3:0]  ctl_ex_out;
    wire [2:0]  ctl_m_out;
    wire [1:0]  ctl_wb_out;
    wire [31:0] read_data_1, read_data_2, sign_ext_out;

    control control1 (.op_code(IF_ID_instruction[31:26]),
                      .EX(ctl_ex_out),
                      .M(ctl_m_out),
                      .WB(ctl_wb_out));

    register register1 (.rs(IF_ID_instruction[25:21]),
                        .rt(IF_ID_instruction[20:16]),
                        .write_reg(MEM_WB_rd),
                        .write_data(WB_mux5_write_data),
                        .reg_write(MEM_WB_reg_write),
                        .A(read_data_1),
                        .B(read_data_2));

    s_extend s_extend1 (.original(IF_ID_instruction[15:0]),
                        .extended(sign_ext_out));

    id_ex id_ex1 (.ctl_wb_out(ctl_wb_out),
                  .ctl_m_out(ctl_m_out),
                  .ctl_ex_out(ctl_ex_out),
                  .npc(IF_ID_npc),
                  .read_data_1(read_data_1),
                  .read_data_2(read_data_2),
                  .sign_ext_out(sign_ext_out),
                  .instruction_2016(IF_ID_instruction[20:16]),
                  .instruction_1511(IF_ID_instruction[15:11]),
                  .wb_ctl_out(wb_ctl_out),
                  .m_ctl_out(m_ctl_out),
                  .reg_dest(reg_dst),
                  .alu_src(alu_src),
                  .alu_op(alu_op),
                  .npc_out(npc_out),
                  .r_data_1_out(r_data1_out),
                  .r_data_2_out(r_data2_out),
                  .sign_extend_out(sign_extend_out),
                  .instruction_out_2016(instruction_out_2016),
                  .instruction_out_1511(instruction_out_1511));

    always @(IF_ID_instruction) begin
        $display("wb_ctlout: %b", wb_ctl_out);
        $display("m_ctlout: %b", m_ctl_out);
        $display("reg_dst: %b", reg_dst);
        $display("alu_src: %b", alu_src);
        $display("alu_op: %b", alu_op);
        $display("npc_out: %b", npc_out);
        $display("r_data1_out: %b", r_data1_out);
        $display("r_data2_out: %b", r_data2_out);
        $display("sign_extend_out: %b", sign_extend_out);
        $display("instruction_out_2016: %b", instruction_out_2016);
        $display("instruction_out_1511: %b", instruction_out_1511);
        $display("");
    end
endmodule
