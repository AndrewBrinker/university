module alu (
        input  wire [31:0] a,
        input  wire [31:0] b,
        input  wire [2:0]  control,

        output reg  [31:0] result,
        output reg         zero
    );

    parameter ALUlwsw = 3'b010,
              ALUbeq  = 3'b110,
              ALUadd  = 3'b010,
              ALUsub  = 3'b110,
              ALUand  = 3'b000,
              ALUor   = 3'b001,
              ALUslt  = 3'b111,
              ALUx    = 3'b011;

    initial begin
        result <= 0;
    end

    reg sign_mismatch;
    always @* begin
        #1
        sign_mismatch <= 0;
        if ((a != $unsigned(a)) || (b != $unsigned(b))) begin
            sign_mismatch <= 1;
        end
    end

    always @* begin
        case (control)
            ALUadd:  result <= a + b;
            ALUsub:  result <= a - b;
            ALUand:  result <= a && b;
            ALUor:   result <= a || b;
            ALUslt:  result <= a < b ? (1 - sign_mismatch) : (0 + sign_mismatch);
            default: result <= 32'bx;
        endcase
        zero <= result ? 0 : 1;
    end
endmodule
