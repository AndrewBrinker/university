`timescale 1ns / 1ps

module id_ex (
        input  wire [1:0]  ctl_wb_out,
        input  wire [2:0]  ctl_m_out,
        input  wire [3:0]  ctl_ex_out,
        input  wire [31:0] npc,
                           read_data_1,
                           read_data_2,
                           sign_ext_out,
        input  wire [4:0]  instruction_2016,
                           instruction_1511,

        output reg  [1:0]  wb_ctl_out,
        output reg  [2:0]  m_ctl_out,
        output reg         reg_dest,
                           alu_src,
        output reg  [1:0]  alu_op,
        output reg  [31:0] npc_out,
                           r_data_1_out,
                           r_data_2_out,
                           sign_extend_out,
        output reg  [4:0]  instruction_out_2016,
                           instruction_out_1511
    );

    initial begin
        wb_ctl_out           <= 0;
        m_ctl_out            <= 0;
        reg_dest             <= 0;
        alu_op               <= 0;
        alu_src              <= 0;
        npc_out              <= 0;
        r_data_1_out         <= 0;
        r_data_2_out         <= 0;
        sign_extend_out      <= 0;
        instruction_out_2016 <= 0;
        instruction_out_1511 <= 0;
    end

    always @ * begin
        #1
        wb_ctl_out           <= ctl_wb_out;
        m_ctl_out            <= ctl_m_out;
        reg_dest             <= ctl_ex_out[3];
        alu_op               <= ctl_ex_out[2:1];
        alu_src              <= ctl_ex_out[0];
        npc_out              <= npc;
        r_data_1_out         <= read_data_1;
        r_data_2_out         <= read_data_2;
        sign_extend_out      <= sign_ext_out;
        instruction_out_2016 <= instruction_2016;
        instruction_out_1511 <= instruction_1511;
    end
endmodule
