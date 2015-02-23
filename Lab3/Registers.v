module Registers(
	input wire clock, issue,
	input wire [4:0] A_address, B_address, dest,
	input wire [31:0] In_data,
	input wire [5:0] In_source, RS_calculating_value,
	input wire write,
	output reg [31:0] A_out, B_out,
	output reg A_invalid, B_invalid
	);
	
	parameter not_redirected = 0;
	parameter true = 1'b1;
	parameter false = 1'b0;
	parameter num_of_regs = 32;
	
	reg [31:0] reg_file [31:0];
	reg [5:0] redirection [31:0];
	integer i;
	
	initial
	begin
		for(i = 0; i < num_of_regs; i = i + 1)
		begin
				reg_file[i] = i;
				redirection[i] = not_redirected;
		end
	end
	
	always @ (negedge clock)
	begin
		//handle issues
	end
	
	always @ (posedge clock)
	begin
		//handle updates
	end
endmodule
