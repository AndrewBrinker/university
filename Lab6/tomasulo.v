`timescale 1ns / 1ps
`include "adders"
`include "instruction_queue"
`include "Registers"

module tomasulo ();
	parameter CLEAR = 0;
	reg cdb_xmit;
	reg clk;
	wire issue_error;
	wire adder_available;
	wire [5:0] adder_rs_available;
	wire [5:0] rs_issued;
	wire [5:0] rs_executing_adder;
	wire [5:0] opcode;
	wire [2:0] execution_unit;
	wire [4:0] dest_address;
	wire [4:0] a_address;
	wire [4:0] b_address;
	wire issue;
	wire [31:0] a;
	wire [31:0] b;
	wire a_invalid;
	wire b_invalid;
	wire cdb_rts;
	wire cdb_write;
	wire [5:0] cdb_source;
	wire [31:0] cdb_data;

	initial begin
		clk <= 0;
		forever begin
			#15 clk <= ~clk;
		end
	end

	initial begin
		cdb_xmit <= CLEAR;
	end

	always @(cdb_rts) begin
		cdb_xmit <= cdb_rts;
	end

	always @(negedge clk) begin
		cdb_xmit <= CLEAR;
	end

	adders adder (
		.clock(clk),
		.issue(issue),
		.A(a),
		.B(b),
		.A_invalid(a_invalid),
		.B_invalid(b_invalid),
		.opcode(opcode),
		.CDB_xmit(cdb_xmit),
		.CDB_data(cdb_data),
		.CDB_source(cdb_source),
		.CDB_write(cdb_write),
		.CDB_rts(cdb_rts),
		.available(adder_available),
		.RS_available(adder_rs_available),
		.issued(rs_issued),
		.RS_executing(rs_executing_adder),
		.error(issue_error)
	);

	Registers registers (
		.clock(clk),
		.issue(issue),
		.A_address(a_address),
		.B_address(b_address),
		.dest(dest_address),
		.In_data(cdb_data),
		.In_source(cdb_source),

		.RS_calculating_value(adder_rs_available),

		.write(cdb_write),
		.A_out(a),
		.B_out(b),
		.A_invalid(a_invalid),
		.B_invalid(b_invalid)
	);

	instruction_queue instructions (
		.clock(clk),
		.issue(issue)
		.adder_available(adder_available),
		.adder_RS_available(adder_rs_available),
		.RS_issued(rs_issued),
		.RS_executing_adder(rs_executing_adder),
		.adder_rts(cdb_rts),
		.RS_finished(cdb_source),
		.operation(opcode),
		.execution_unit(execution_unit),
		.A_address(a_address),
		.B_address(b_address),
		.Dest_address(dest_address),
		.issue_error(issue_error),
	);

endmodule

