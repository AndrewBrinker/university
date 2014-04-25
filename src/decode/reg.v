module register (
        input       [4:0]  rs,
        input       [4:0]  rt,
        input       [4:0]  write_reg,
        input       [31:0] write_data,
        input              reg_write,

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

        $display("From Register Memory:");
        for (i = 0; i < 9; i = i + 1) begin
            $display("\tREG[%0d] = %0d", i, REG[i]);
        end

        $display("\t...");
        $display("\tREG[%0d] = %0d", 31, REG[31]);
    end

    always @ * begin
        A <= REG[rs];
        B <= REG[rt];

        if (write_reg != 0 && reg_write) begin
            REG[write_reg] = write_data;
        end
    end
endmodule
