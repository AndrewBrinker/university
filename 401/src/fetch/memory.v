module memory (
        input  wire [31:0] addr,

        output reg  [31:0] data
    );

    reg [31:0] MEM[0:127];

    initial begin
        $readmemb("etc/risc.txt", MEM);
    end

    always @(addr) begin
        data <= MEM[addr];
    end
endmodule
