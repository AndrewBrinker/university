`include "adders.v"

module adders_test;
	parameter clock_period = 10;
	
	//operation codes used by alu
	parameter alu_add = 3'b000;
	parameter alu_sub = 3'b001;
	parameter alu_or  = 3'b100;
	parameter alu_and = 3'b101;
	parameter alu_not = 3'b110;
	parameter alu_xor = 3'b111;
	
	//inputs
	reg clk;
	reg issue_command;
	reg [5:0] command;
	reg signed [31:0] A, B;
	reg A_invalid, B_invalid;
	reg CDB_xmit;
	//i/o used just as output
	wire signed [31:0] data_out;
	wire [5:0] data_from_rs_num;
	wire data_valid;
	//outputs
	wire CDB_rts;
	wire available;
	wire [5:0] issued_to_rs_num, RS_available;
	wire [5:0] RS_executing;
	wire error;
	
	initial
	begin
		clk <= 0;
		issue_command <= 0;
		command <= 0;
		A <= 0;
		B <= 0;
		A_invalid <= 0;
		B_invalid <= 0;
		CDB_xmit <= 0;
		
		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
		
		issue_command <= 1;
		command <= alu_add;
		A <= 5;
		B <= 5;
		A_invalid <= 0;
		B_invalid <= 0;
		CDB_xmit <= 0;
		
		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
		
		issue_command <= 1;
		command <= alu_sub;
		A <= 3;
		B <= 13;
		A_invalid <= 1;
		B_invalid <= 0;
		CDB_xmit <= 0;
		
		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
		
		issue_command <= 1;
		command <= alu_and;
		A <= 15;
		B <= 60;
		A_invalid <= 0;
		B_invalid <= 0;
		CDB_xmit <= 0;
		
		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
		
		issue_command <= 1;
		command <= alu_add;
		A <= 25;
		B <= 35;
		A_invalid <= 0;
		B_invalid <= 0;
		CDB_xmit <= 0;
		
		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
		
		issue_command <= 0;
		command <= 0;
		A <= 0;
		B <= 0;
		A_invalid <= 0;
		B_invalid <= 0;
		CDB_xmit <= 0;
		#clock_period clk = ~clk;
		
		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
		
		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
		
		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
		
		CDB_xmit <= 1;
		
		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
		
		CDB_xmit <= 0;
		
		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
		
		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
		
		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
		
		CDB_xmit <= 1;
		
		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
		
		CDB_xmit <= 0;
		
		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
		
		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
		
		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
		
		CDB_xmit <= 1;
		
		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
		
		CDB_xmit <= 0;
		
		#clock_period clk = ~clk;
		#clock_period clk = ~clk;
	end
	
	adders my_adders(
		.clock(clk),
		.issue(issue_command),
		.A(A),
		.B(B),
		.A_invalid(A_invalid),
		.B_invalid(B_invalid),
		.opcode(command),
		.CDB_xmit(CDB_xmit),
		.CDB_data(data_out),
		.CDB_source(data_from_rs_num),
		.CDB_write(data_valid),
		.CDB_rts(CDB_rts),
		.available(available),
		.RS_available(RS_available),
		.issued(issued_to_rs_num),
		.RS_executing(RS_exucting),
		.error(error
	);
endmodule
