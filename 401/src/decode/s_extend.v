module s_extend (
        input  wire [15:0] nextend,

        output reg [31:0] extend
    );

    always @ (nextend[15:0]) begin
        #1
        extend <= {{16{nextend[15]}}, nextend};
    end
endmodule
