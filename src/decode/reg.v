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

    always @ * begin
        #1
        A <= REG[rs];
        B <= REG[rt];
        if (rd != 0 && regwrite) begin
            REG[rd] = writedata;
        end
    end

    always @ (REG[0]) begin
        #1  $display("%d %d %d", REG[0], REG[1], REG[2]);
        #2  $display("%d %d %d", REG[0], REG[1], REG[2]);
        #3  $display("%d %d %d", REG[0], REG[1], REG[2]);
        #4  $display("%d %d %d", REG[0], REG[1], REG[2]);
        #5  $display("%d %d %d", REG[0], REG[1], REG[2]);
        #6  $display("%d %d %d", REG[0], REG[1], REG[2]);
        #7  $display("%d %d %d", REG[0], REG[1], REG[2]);
        #8  $display("%d %d %d", REG[0], REG[1], REG[2]);
        #9  $display("%d %d %d", REG[0], REG[1], REG[2]);
        #10 $display("%d %d %d", REG[0], REG[1], REG[2]);
        #11 $display("%d %d %d", REG[0], REG[1], REG[2]);
        #12 $display("%d %d %d", REG[0], REG[1], REG[2]);
        #13 $display("%d %d %d", REG[0], REG[1], REG[2]);
        #14 $display("%d %d %d", REG[0], REG[1], REG[2]);
        #15 $display("%d %d %d", REG[0], REG[1], REG[2]);
        #16 $display("%d %d %d", REG[0], REG[1], REG[2]);
        #17 $display("%d %d %d", REG[0], REG[1], REG[2]);
        #18 $display("%d %d %d", REG[0], REG[1], REG[2]);
        #19 $display("%d %d %d", REG[0], REG[1], REG[2]);
        #20 $display("%d %d %d", REG[0], REG[1], REG[2]);
        #21 $display("%d %d %d", REG[0], REG[1], REG[2]);
        #22 $display("%d %d %d", REG[0], REG[1], REG[2]);
        #23 $display("%d %d %d", REG[0], REG[1], REG[2]);
        #24
        $finish;
    end
endmodule
