module and (
        input  reg branch,
        input  reg zero,

        output reg out
    );

    assign output = branch && zero;
endmodule
