module alu_control (
        input  wire [5:0] funct,
        input  wire [1:0] aluop,

        output reg  [2:0] select
    );


    parameter Rtype =   ,
              Radd  =   ,
              Rsub  =   ,
              Rand  =   ,
              Ror   =   ,
              Rslt  =   ;

    parameter lwsw  = 2'b00,
              Itype = 2'b01,
              xis   = 6'bx;

    parameter ALUadd = 3'b010,
              ALUsub = 3'b110,
              ALUand = 3'b000,
              ALUor  = 3'b001,
              ALUslt = 3'b111;

    parameter unknown = 2'b11,
              ALUx    = 3'b011;

    initial begin
        select <= 0;
    end

    always@* begin
        if (aluop == Rtype) begin
            case (funct)
                Radd:    select <= ;
                Rsub:    select <= ;
                Rand:    select <= ;
                Ror:     select <= ;
                Rslt:    select <= ;
                default: select <= ALUx;
            endcase
        end
        else if (aluop == lwsw) begin
            select <= ;
        end
        else if (aluop == Itype) begin
           select <= ;
        end
        else if (aluop == unknown) begin
            select <= ALUx;
        end
        else begin
            select <= select;
        end
    end
endmodule
