module s_extend (
        input  wire [15:0] nextend,

        output wire [31:0] extend
    );

    assign extend = {{16{nextend[15]}}, nextend};
endmodule
