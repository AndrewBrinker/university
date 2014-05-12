module alu (
        input   wire  [31:0] a,
        input   wire  [31:0] b,
        input   wire  [2:0]  control,

        output  reg   [31:0] result,
        output  wire         zero
    );

    parameter ALUadd =
    /*
        Get these values from Figure 3.2 in Lab Manual:
        ALUsub      =
        ALUand      =
        ALUor       =
        ALUslt      =
    */


    // Handles negative inputs
    wire sign_mismatch;
    assign sign_mismatch = ;

    initial begin
        result <= 0;
    end

    always@* begin
        case (control)
            ALUadd:   result = a + b;
            ALUsub:   result = a - b;
            ALUand:   result = a && b;
            ALUor:    result = a || b;
            ALUslt:   result = a < b ? 1 - sign_mismatch : 0 + sign_mismatch;
            default:  result = 32'bx;
        endcase

        assign zero = ;
        /*
            Check to see if result is equal to zero. if it is assign it true (1). if it isn't assign it false (0)
            Complete this line using a turnary operator. If you don't know what that is then Google it.
            P.S. I tabbed all of these lines, so you might want to delete these comments after you implement this line or
            replace the tabs with white space for when you import this into your LaTeX write up.
        */
    end
endmodule
