`timescale 1ns / 1ps

module i_decode (
        input   wire [31:0] IF_ID_instruction_out,
        input   wire [31:0] IF_ID_npc_out,
        input   wire [4:0]  MEM_WB_rd,
        input   wire        MEM_WB_reg_write,
        input   wire [31:0] WB_mux5_write_data,
        output  wire [1:0]  wb_ctl_out,
        output  wire [2:0]  m_ctl_out,
        output  wire        reg_dst, alu_src,
        output  wire [1:0]  alu_op,
        output  wire [31:0] npc_out, r_data1_out, r_data2_out, sign_extend_out,
        output  wire [4:0]  instruction_out_2016, instruction_out_1511
    );

    wire [3:0]  ctl_ex_out;
    wire [2:0]  ctl_m_out;
    wire [1:0]  ctl_wb_out;
    wire [31:0] read_data_1, read_data_2, sign_ext_out;

    control control1 (.op_code(IF_ID_instruction_out[31:26]),
                      .EX(ctl_ex_out),
                      .M(ctl_m_out),
                      .WB(ctl_wb_out));

    register register1 (.rs(IF_ID_instruction_out[25:21]),
                        .rt(IF_ID_instruction_out[20:16]),
                        .rd(MEM_WB_rd),
                        .write_data(WB_mux5_write_data),
                        .reg_write(MEM_WB_reg_write),
                        .A(read_data_1),
                        .B(read_data_2));

    s_extend s_extend1 (.nextend(IF_ID_instruction_out[15:0]),
                        .extend(sign_ext_out));

    id_ex id_ex1 (.ctl_wb_out(ctl_wb_out),
                  .ctl_m_out(ctl_m_out),
                  .ctl_ex_out(ctl_ex_out),
                  .npc(IF_ID_npc_out),
                  .read_data_1(read_data_1),
                  .read_data_2(read_data_2),
                  .sign_ext_out(sign_ext_out),
                  .instr_2016(IF_ID_instruction_out[20:16]),
                  .instr_1511(IF_ID_instruction_out[15:11]),
                  .wb_ctl_out(wb_ctl_out),
                  .m_ctl_out(m_ctl_out),
                  .reg_dst(reg_dst),
                  .alu_src(alu_src),
                  .alu_op(alu_op),
                  .npc_out(npc_out),
                  .r_data1_out(r_data1_out),
                  .r_data2_out(r_data2_out),
                  .sign_extend_out(sign_extend_out),
                  .instruction_out_2016(instruction_out_2016),
                  .instruction_out_1511(instruction_out_1511));

endmodule
