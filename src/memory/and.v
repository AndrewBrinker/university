module AND (
        input  wire BRANCH,
        input  wire zero,

        output wire out
    );

    assign out = BRANCH && zero;
endmodule
