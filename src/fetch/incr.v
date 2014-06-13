module incrementer (
        input  wire [31:0] pcin,
        output wire [31:0] pcout
    );

    assign pcout = pcin + 1;
endmodule
