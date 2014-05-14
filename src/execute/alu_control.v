module alu_control (
                input  wire [5:0] funct, // 6 bit function identifier
                input  wire [1:0] aluop, // 2 bit alu operation identifier

                output reg  [2:0] select // the chosen operation identifier
        );

        parameter Mtype   = 2'b00,
                  Itype   = 2'b01,
                  Rtype   = 2'b10,
                  unknown = 2'b11;

        parameter Rlw     = 6'bxxxxxx,
                  Rsw     = 6'bxxxxxx,
                  Rbeq    = 6'bxxxxxx,
                  Radd    = 6'b100000,
                  Rsub    = 6'b100010,
                  Rand    = 6'b100100,
                  Ror     = 6'b100101,
                  Rslt    = 6'b101010;

        parameter ALUlwsw = 3'b010,
                  ALUbeq  = 3'b110,
                  ALUadd  = 3'b010,
                  ALUsub  = 3'b110,
                  ALUand  = 3'b000,
                  ALUor   = 3'b001,
                  ALUslt  = 3'b111,
                  ALUx    = 3'b011;

        initial begin
            select <= 0;
        end

        always@* begin
            #1
            if (aluop == Rtype) begin
                case (funct)
                    Radd:    select <= ALUadd;
                    Rsub:    select <= ALUsub;
                    Rand:    select <= ALUand;
                    Ror:     select <= ALUor;
                    Rslt:    select <= ALUslt;
                    default: select <= ALUx;
                endcase
            end
            else if (aluop == Mtype) begin
                select <= ALUlwsw;
            end
            else if (aluop == Itype) begin
                select <= ALUbeq;
            end
            else if (aluop == unknown) begin
                select <= ALUx;
            end
            else begin
                select <= select;
            end
        end
endmodule
