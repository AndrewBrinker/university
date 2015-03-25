`include "instruction_queue.v"

module Instruction_queue_test;

	reg clk = 0;
	reg error = 0;
	reg available = 0;
	reg [5:0] RS_available = 0;
	reg [5:0] RS_issue;
	reg [5:0] RS_exec = 0;
	reg adder_rts = 0;
	reg [5:0] RS_xmit = 0;

	wire [5:0] opcode;
	wire [2:0] exec_unit;
	wire [4:0] A_address;
	wire [5:0] B_address;
	wire [4:0] Dest_address;
	wire issue;

	parameter clk_period = 10;

	initial
	begin
		#clk_period clk = ~clk;
		RS_issue <= RS_available;
		#clk_period clk = ~clk;

		error <= 0;
		available <= 1;
		RS_available <= 1;
		RS_issue <= 0;
		RS_exec <= 0;
		adder_rts <= 0;
		RS_xmit <= 0;

		#clk_period clk = ~clk;
		RS_issue <= RS_available;
		#clk_period clk = ~clk;

		error <= 0;
		available <= 1;
		RS_available <= 2;
		RS_issue <= 0;
		RS_exec <= 0;
		adder_rts <= 0;
		RS_xmit <= 0;

		#clk_period clk = ~clk;
		RS_issue <= RS_available;
		#clk_period clk = ~clk;

		error <= 0;
		available <= 1;
		RS_available <= 3;
		RS_issue <= 0;
		RS_exec <= 0;
		adder_rts <= 0;
		RS_xmit <= 0;

		#clk_period clk = ~clk;
		RS_issue <= RS_available;
		#clk_period clk = ~clk;

		error <= 0;
		available <= 0;
		RS_available <= 0;
		RS_issue <= 0;
		RS_exec <= 3;
		adder_rts <= 0;
		RS_xmit <= 0;

		#clk_period clk = ~clk;
		RS_issue <= RS_available;
		#clk_period clk = ~clk;

		error <= 0;
		available <= 0;
		RS_available <= 0;
		RS_issue <= 0;
		RS_exec <= 0;
		adder_rts <= 1;
		RS_xmit <= 0;

		#clk_period clk = ~clk;
		RS_issue <= RS_available;
		#clk_period clk = ~clk;

		error <= 0;
		available <= 0;
		RS_available <= 0;
		RS_issue <= 0;
		RS_exec <= 0;
		adder_rts <= 0;
		RS_xmit <= 3;

		#clk_period clk = ~clk;
		RS_issue <= RS_available;
		#clk_period clk = ~clk;

		error <= 0;
		available <= 1;
		RS_available <= 3;
		RS_issue <= 0;
		RS_exec <= 2;
		adder_rts <= 0;
		RS_xmit <= 0;

		#clk_period clk = ~clk;
		RS_issue <= RS_available;
		#clk_period clk = ~clk;

		error <= 0;
		available <= 0;
		RS_available <= 0;
		RS_issue <= 0;
		RS_exec <= 0;
		adder_rts <= 1;
		RS_xmit <= 0;

		#clk_period clk = ~clk;
		RS_issue <= RS_available;
		#clk_period clk = ~clk;
	end

	instruction_queue the_iq(
		.clock(clk),
		.issue_error(error),
		.adder_available(available),
		.adder_RS_available(RS_available),
		.RS_issued(RS_issue),
		.RS_executing_adder(RS_exec),
		.adder_rts(adder_rts),
		.RS_finished(RS_xmit),
		.operation(opcode),
		.execution_unit(exec_unit),
		.Dest_address(Dest_address),
		.A_address(A_address),
		.B_address(B_address),
		.issue(issue)
	);
endmodule
