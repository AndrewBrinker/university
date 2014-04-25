`timescale 1ns / 1ps

module id_ex (
        input  wire [1:0]  ctl_wb_out,
        input  wire [2:0]  ctl_m_out,
        input  wire [3:0]  ctl_ex_out,
        input  wire [31:0] npc, read_data_1, read_data_2, sign_ext_out,
        input  wire [4:0]  instruction_2016, instruction_1511,
        output reg  [1:0]  wb_ctl_out,
        output reg  [2:0]  m_ctl_out,
        output reg         reg_dest, alu_src,
        output reg  [1:0]  alo_op,
        output reg  [31:0] npcout, r_data_1_out, r_data_2_out, sign_extend_out,
        output reg  [4:0]  instruction_out_2016, instruction_out_1511
    );

    initial begin
        wb_ctl_out           <= 0
        m_ctl_out            <= 0
        reg_dest             <= 0
        alo_op               <= 0
        alu_src              <= 0
        npcout               <= 0
        r_data_1_out         <= 0
        r_data_2_out         <= 0
        sign_extend_out      <= 0
        instruction_out_2016 <= 0
        instruction_out_1511 <= 0
    end

    always @ * begin
        // Wire the inputs to the corresponding outputs
        #1
        wb_ctl_out           <=
        m_ctl_out            <=
        reg_dest             <=
        alo_op               <=
        alu_src              <=
        npcout               <=
        r_data_1_out         <=
        r_data_2_out         <=
        sign_extend_out      <=
        instruction_out_2016 <=
        instruction_out_1511 <=
    end
endmodule
