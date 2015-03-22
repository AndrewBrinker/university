`timescale 1ns / 1ps
`include "adders"
`include "instruction_queue"
`include "Regsters"

module tomasulo ();
	reg clk;
	wire issue_command;
	wire [5:0] command;
	wire signed [31:0] A, B;
	wire A_invalid, B_invalid;
	wire CDB_xmit;

	wire signed [31:0] data_out;
	wire [5:0] data_from_rs_num;
	wire data_valid;

	wire CDB_rts;
	wire available;
	wire [5:0] issued_to_rs_num, RS_available;
	wire [5:0] RS_executing;
	wire error;

	wire [2:0] exec_unit;
	wire [5:0] RS_finished;
	wire [4:0] Dest_address;
	wire [4:0] A_address;
	wire [4:0] B_address;

	adders adder (
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
		.RS_executing(RS_executing),
		.error(error)
	);

	Registers registers (
		.clock(clk),
		.issue(issue_command),
		.A_address(A_address),
		.B_address(B_address),
		.dest(Dest_address),
		.In_data(data_out),
		.In_source(data_from_rs_num),
		.RS_calculating_value(RS_executing),
		.write(data_valid),
		.A_out(A),
		.B_out(B),
		.A_invalid(A_invalid),
		.B_invalid(B_invalid)
	);

	instruction_queue instructions (
		.clock(clk),
		.issue_error(error),
		.adder_available(available),
		.adder_RS_available(RS_available),
		.RS_issued(issued_to_rs_num),
		.RS_executing_adder(RS_executing),
		.adder_rts(CDB_rts),
		.RS_finished(RS_finished),
		.operation(command),
		.execution_unit(exec_unit),
		.Dest_address(Dest_address),
		.A_address(A_address),
		.B_address(B_address),
		.issue(issue_command)
	);

	initial begin
		clk = 0;
		forever begin
			#10 clk = ~clk;
		end
	end

endmodule
