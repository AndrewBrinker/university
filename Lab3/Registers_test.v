`include "Registers.v"

module Registers_test;

	parameter clock_period = 10;

	//inputs
	reg clk;
	reg issue_command;
	reg [4:0] A_address, B_address, dest;
	reg signed [31:0] CDB_data;
	reg [5:0] CDB_source, RS_producing_result;
	reg CDB_write;

	wire signed [31:0] A, B;
	wire A_invalid, B_invalid;

	initial
	begin
        $dumpfile("test.vcd");
        $dumpvars;

		clk <= 0;

		//reading variables
		issue_command <= 0;
		A_address <= 0;
		B_address <= 0;
		dest <= 0;
		RS_producing_result <= 0;

		//update variables
		CDB_data <= 0;
		CDB_source <= 0;
		CDB_write <= 0;

		#clock_period clk = ~clk;
		#clock_period clk = ~clk;

		//reading variables
		issue_command <= 1;
		A_address <= 1;
		B_address <= 2;
		dest <= 3;
		RS_producing_result <= 1;

		//update variables
		CDB_data <= 0;
		CDB_source <= 0;
		CDB_write <= 0;

		#clock_period clk = ~clk;
		#clock_period clk = ~clk;

		//reading variables
		issue_command <= 0;
		A_address <= 0;
		B_address <= 0;
		dest <= 0;
		RS_producing_result <= 0;

		//update variables
		CDB_data <= 0;
		CDB_source <= 0;
		CDB_write <= 0;

		#clock_period clk = ~clk;
		#clock_period clk = ~clk;

		//reading variables
		issue_command <= 1;
		A_address <= 3;
		B_address <= 7;
		dest <= 7;
		RS_producing_result <= 2;

		//update variables
		CDB_data <= 0;
		CDB_source <= 0;
		CDB_write <= 0;

		#clock_period clk = ~clk;
		#clock_period clk = ~clk;

		//reading variables
		issue_command <= 0;
		A_address <= 0;
		B_address <= 0;
		dest <= 0;
		RS_producing_result <= 0;

		//update variables
		CDB_data <= 25;
		CDB_source <= 1;
		CDB_write <= 1;

		#clock_period clk = ~clk;
		#clock_period clk = ~clk;

		//reading variables
		issue_command <= 0;
		A_address <= 0;
		B_address <= 0;
		dest <= 0;
		RS_producing_result <= 0;

		//update variables
		CDB_data <= 0;
		CDB_source <= 0;
		CDB_write <= 0;

		#clock_period clk = ~clk;
		#clock_period clk = ~clk;

		//reading variables
		issue_command <= 1;
		A_address <= 12;
		B_address <= 13;
		dest <= 15;
		RS_producing_result <= 3;

		//update variables
		CDB_data <= 21;
		CDB_source <= 2;
		CDB_write <= 1;

		#clock_period clk = ~clk;
		#clock_period clk = ~clk;

		//reading variables
		issue_command <= 0;
		A_address <= 0;
		B_address <= 0;
		dest <= 0;
		RS_producing_result <= 0;

		//update variables
		CDB_data <= 0;
		CDB_source <= 0;
		CDB_write <= 0;

		#clock_period clk = ~clk;
		#clock_period clk = ~clk;

		//reading variables
		issue_command <= 1;
		A_address <= 3;
		B_address <= 7;
		dest <= 12;
		RS_producing_result <= 1;

		//update variables
		CDB_data <= 0;
		CDB_source <= 0;
		CDB_write <= 0;

		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
	end

	Registers myregs(
		.clock(clk),
		.issue(issue_command),
		.A_address(A_address),
		.B_address(B_address),
		.dest(dest),
		.In_data(CDB_data),
		.In_source(CDB_source),
		.RS_calculating_value(RS_producing_result),
		.write(CDB_write),
		.A_out(A),
		.B_out(B),
		.A_invalid(A_invalid),
		.B_invalid(B_invalid)
	);
endmodule
