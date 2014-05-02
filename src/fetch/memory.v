`define MEMORY_FILE_NAME "etc/memory.list"

module memory (
        output reg  [31:0] data,
        input  wire [31:0] addr
    );

    reg [31:0] mem[0:127];

    initial begin
        #0
        $readmemh(`MEMORY_FILE_NAME, mem);
    end

    always@* begin
        data <= mem[addr];
    end
endmodule
