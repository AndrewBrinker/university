module mux (
        input [31:0] a,
        input [31:0] b,
        input select,
        output wire [31:0] y
    );

    assign y = select ? a : b;
endmodule
