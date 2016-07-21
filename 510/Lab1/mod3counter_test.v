`include "mod3counter.v"

module mod3counter_test;
	reg [1:0] num_in;
	wire [1:0] num_out;
	
	mod3counter mod3inc(
		.num(num_in),
		.mod3num(num_out)
	);
	
	initial begin
		$dumpfile("mod3.vcd");
		$dumpvars;
		
		num_in <= 2;
		#10;
		num_in <= 1;
		#10;
		num_in <= 0;
		#10;
		num_in <= 5;
		#10;
	end
endmodule
