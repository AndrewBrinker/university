module memory (
        output reg  [31:0] data,
        input  wire [31:0] addr
    );

    reg [31:0] MEM[0:127];

    /*
    initial begin
        MEM[0] <= 'h002300AA; // 000000 => RTYPE
        MEM[1] <= 'h10654321; // 000100 => BEQ
        MEM[2] <= 'h00100022; // 000000 => RTYPE
        MEM[3] <= 'h8C123456; // 100011 => LW
        MEM[4] <= 'h8F123456; // 100011 => LW
        MEM[5] <= 'hAD654321; // 101011 => SW
        MEM[6] <= 'h13012345; // 000100 => BEQ
        MEM[7] <= 'hAC654321; // 101011 => SW
        MEM[8] <= 'h12012345; // 000100 => BEQ
    end
    */

    integer i;
    initial begin
        $readmemb("etc/risc.txt", MEM);
        for (i = 0; i < 24; i = i + 1) begin
            $display(MEM[i]);
        end
    end

    always @(addr) data <= MEM[addr];
endmodule
