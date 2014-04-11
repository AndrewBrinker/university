// Adds 1 to the program counter
module add (
        input [31:0] in,
        input [31:0] value,
        output wire [31:0] out
    );
    assign out = in + value;
endmodule
