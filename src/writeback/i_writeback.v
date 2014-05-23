`include "src/fetch/mux.v"

module i_writeback (
        input  reg         regwrite,
                           memtoreg,
        input  reg  [31:0] read_data,
                           mem_alu_result,
        input  reg  [4:0]  mem_write_reg,

        output reg [31:0] wb_data
    );

    mux mux4(.a(mem_alu_result),
             .b(read_data),
             .select(memtoreg),
             .y(wb_data));

endmodule
