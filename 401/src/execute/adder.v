module adder (
        input   wire [31:0] add_in1,
        input   wire [31:0] add_in2,

        output  wire [31:0] add_out
    );

    assign add_out = add_in1 + add_in2;
endmodule
