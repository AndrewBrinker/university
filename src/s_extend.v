`timescale 1ns / 1ps

module s_extend (
        input  wire [15:0] nextend,
        output reg  [31:0] extend
    );

    always@ * begin
        // Replicate signed bit 16 times then cancatinate
    end
endmodule
