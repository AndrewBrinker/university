module d_mem (
        input  wire [31:0] addr,
        input  wire [31:0] write_data,
        input  wire        memwrite,
                           memread,

        output reg  [31:0] read_data
    );

    // 256 words of 32-bit memory
    reg [31:0] DMEM[0:255];

    initial begin
        $readmemb("etc/data.txt", DMEM);
    end

    always @ (addr) begin
        #1
        if (memwrite == 1) begin
            // Write stuff
            DMEM[addr] <= write_data;
            read_data  <= 32'bx;
        end else if (memread == 1) begin
            // Read stuff
            read_data <= DMEM[addr];
        end else begin
            // No idea!
            read_data <= 32'bx;
        end
    end
endmodule
