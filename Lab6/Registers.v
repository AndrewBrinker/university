module Registers (
        input wire clock, issue,
        input wire [4:0]  A_address,
                          B_address,
                          dest,
        input wire [31:0] In_data,
        input wire [5:0]  In_source,
                          RS_calculating_value,
        input wire write,
        output reg [31:0] A_out,
                          B_out,
        output reg A_invalid,
                   B_invalid
    );

    parameter not_redirected = 0;
    parameter true           = 1'b1;
    parameter false          = 1'b0;
    parameter num_of_regs    = 32;

    reg [31:0] reg_file    [31:0];
    reg  [5:0] redirection [31:0];

    integer i;

    initial begin
        // Initialize register file
        reg_file[0] = 1;
        redirection[0] = not_redirected;
        for(i = 1; i < num_of_regs; i = i + 1) begin
                reg_file[i] = i;
                redirection[i] = not_redirected;
        end
    end

    always @ (negedge clock) begin
        // Handle issues
        if (issue) begin
            if (redirection[A_address] == not_redirected) begin
                A_out = reg_file[A_address];
                A_invalid = false;
            end
            else begin
                A_out = redirection[A_address];
                A_invalid = true;
            end
            if (redirection[B_address] == not_redirected) begin
                B_out = reg_file[B_address];
                B_invalid = false;
            end
            else begin
                B_out = redirection[B_address];
                B_invalid = true;
            end
            redirection[dest] = RS_calculating_value;
        end
    end

    always @ (posedge clock) begin
        // Handle updates
        if (write) begin
            for (i = 0; i < num_of_regs; i = i + 1) begin
                if (redirection[i] == In_source) begin
                    redirection[i] = not_redirected;
                    reg_file[i] = In_data;
                end
            end
        end
    end
endmodule
