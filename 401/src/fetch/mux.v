module mux #(
        parameter SIZE = 32
    ) (
        input [SIZE - 1:0] a,
        input [SIZE - 1:0] b,
        input select,
        output wire [SIZE - 1:0] y
    );

    assign y = select ? a : b;
endmodule
