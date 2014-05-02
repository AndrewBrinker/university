module control (
        input  wire [5:0] opcode,

        output reg  [3:0] EX,
        output reg  [2:0] M,
        output reg  [1:0] WB
    );

    parameter RTYPE = 6'b000000;
    parameter LW    = 6'b100011;
    parameter SW    = 6'b101011;
    parameter BEQ   = 6'b000100;
    parameter NOP   = 6'b100000;

    initial begin
        #0
        EX <= 0;
        M  <= 0;
        WB <= 0;
    end

    always@* begin
        case (opcode)
            RTYPE: begin
                EX <= 4'b1100;
                M  <= 3'b000;
                WB <= 2'b10;
            end
            LW: begin
                EX <= 4'b0001;
                M  <= 3'b010;
                WB <= 2'b11;
            end
            SW: begin
                EX <= 4'bx001;
                M  <= 3'b001;
                WB <= 2'b0x;
            end
            BEQ: begin
                EX <= 4'bx010;
                M  <= 3'b100;
                WB <= 2'b0x;
            end
            NOP: begin
                EX <= 4'b0;
                M  <= 3'b0;
                WB <= 2'b0;
            end
            default: $display ("Opcode not recognized.");
        endcase
    end
endmodule
