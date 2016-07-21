module register (
        input  wire [4:0]  rs,
        input  wire [4:0]  rt,
        input  wire [4:0]  rd,
        input  wire [31:0] writedata,
        input  wire        regwrite,

        output reg  [31:0] A,
        output reg  [31:0] B
    );

    reg [31:0] REG [0:31];

    integer i;
    initial begin
        A <= 0;
        B <= 0;
        for (i = 0; i < 32; i = i + 1) begin
            REG[i] <= 0;
        end
    end

    always @ (REG[rs] || REG[rt]) begin
        A <= REG[rs];
        B <= REG[rt];
        if (rd != 0 && regwrite) begin
            REG[rd] = writedata;
        end
    end

    always @ (REG[0] || REG[1] || REG[2]) begin
        #1
        $display("%d %d %d", REG[0], REG[1], REG[2]);
    end
endmodule
