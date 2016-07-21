module i_writeback (
        input  wire        memtoreg,
        input  wire [31:0] read_data,
                           mem_alu_result,

        output wire [31:0] wb_data
    );

    mux mux4(.a(mem_alu_result),
             .b(read_data),
             .select(memtoreg),
             .y(wb_data));
endmodule
