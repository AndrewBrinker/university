module incrementer (
        input  [31:0] pcin,
        output [31:0] pcout
    );

    assign pcout = pcin + 1;
endmodule