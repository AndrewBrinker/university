module memory (
        output reg  [31:0] data,
        input  wire [31:0] addr
    );

    reg [31:0] MEM[0:127];

    initial begin
        #0
        MEM[0] <= 'h002300AA;
        MEM[1] <= 'h10654321;
        MEM[2] <= 'h00100022;
        MEM[3] <= 'h8C123456;
        MEM[4] <= 'h8F123456;
        MEM[5] <= 'hAD654321;
        MEM[6] <= 'h13012345;
        MEM[7] <= 'hAC654321;
        MEM[8] <= 'h12012345;
   end

    always@(addr) begin
        data <= MEM[addr];
    end
endmodule
