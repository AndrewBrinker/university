module mux (
        input [31:0] pc,
        input [31:0] jump,
        input select,
        output wire [31:0] out
    );
    assign out = select ? pc : jump;
endmodule
