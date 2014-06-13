module memory (
        output reg  [31:0] data,
        input  wire [31:0] addr
    );

    reg [31:0] MEM[0:127];

    integer i;
    initial begin
        $readmemb("etc/risc.txt", MEM);
    end

    always @(addr) data <= MEM[addr];
endmodule
