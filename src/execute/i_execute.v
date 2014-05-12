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
endmodule
