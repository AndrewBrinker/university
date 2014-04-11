`define MEMORY_FILE_NAME "etc/memory.list"

module instruction_memory (
        input [31:0] pc,
        output wire [31:0] out
    );
    reg [31:0] mem[0:127];
    initial begin
        $readmemh(`MEMORY_FILE_NAME, mem);
    end
    assign out = mem[pc[6:0]];
endmodule
