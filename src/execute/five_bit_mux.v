module five_bit_mux (
        input [4:0] pc,
        input [4:0] jump,
        input select,
        output wire [4:0] out
    );
    assign out = select ? pc : jump;
endmodule
