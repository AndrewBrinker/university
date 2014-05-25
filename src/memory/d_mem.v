module d_mem (
        input  wire [31:0] addr,
        input  wire [31:0] write_data,
        input  wire        memwrite,
                           memread,

        output reg  [31:0] read_data
    );

    // 256 words of 32-bit memory
    reg [31:0] DMEM[0:255];

    integer i;
    initial begin
        read_data <= 0;

        // Initialize DMEM[0-5] from data.txt
        DMEM[0] = 32'b00000000000000000000000000000000;
        DMEM[1] = 32'b00000000000000000000000000000001;
        DMEM[2] = 32'b00000000000000000000000000000010;
        DMEM[3] = 32'b00000000000000000000000000000011;
        DMEM[4] = 32'b00000000000000000000000000000100;
        DMEM[5] = 32'b00000000000000000000000000000101;


        // Initialize DMEM[6-255] to 6-255
        for (i = 6; i < 256; i = i + 1) begin
            DMEM[i] = i;
        end
    end

    always@(addr) begin
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
