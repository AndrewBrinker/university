`timescale 1ns / 1ps

module s_extend (
        input  wire [15:0] original,

        output wire [31:0] extended
    );

    assign extended = {{16{original[15]}}, original};
endmodule
