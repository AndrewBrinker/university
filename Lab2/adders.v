module adders(
		input wire clock, issue,
		input wire signed [31:0] A, B,
		input wire A_invalid, B_invalid,
		input wire [5:0] opcode,
		input wire CDB_xmit,
		inout wire signed [31:0] CDB_data,
		inout wire [5:0] CDB_course,
		inout wire CDB_write,
		output reg CDB_rts,
		output wire available,
		output wire [5:0] RS_available,
		output reg [5:0] issued,
		output wire [5:0] RS_executing,
		output reg error
	);
	
	//constants
	
	//disconnected buses have high impedance
	parameter disconnected = 32'bz;
	//calculation delay
	parameter delay = 20;
	//updating delay
	parameter update_delay = 5;
	//no unit is calculating
	parameter none = 2'b00;
	//clear Qj/Qk, thus Vj/Vk valid
	parameter valid = 6'b000000;
	//error code
	parameter no_error = 1'b0;
	parameter all_rs_bus = 1'b1;
	//command not issued
	parameter not_issued = 6'b000000;
	//status of reservation station or alu
	parameter not_busy = 1'b0;
	parameter in_use = 1'b1;
	//is the value ready to send or not
	parameter ready = 1'b1;
	parameter not_ready = 1'b0;
	//used to clear a data value
	parameter clear = 0;
	//reservation station
	parameter no_rs = 6'b000000;
	parameter adder_1 = 6'b000001;
	parameter adder_2 = 6'b000010;
	parameter adder_3 = 6'b000011;
	//operation codes used by alu
	parameter alu_add = 3'b000;
	parameter alu_sub = 3'b001;
	parameter alu_or  = 3'b100;
	parameter alu_and = 3'b101;
	parameter alu_not = 3'b110;
	parameter alu_xor = 3'b111;
	
	//the result being written
	reg signed [31:0] CDB_data_out;
	//which rs is finished
	reg [5:0] CDB_source_out;
	//ready to write to CDB
	reg CDB_write_out;
	//operation to do
	reg [5:0] operation [2:0];
	//if values aren't available here is where to get them
	reg [5:0] Qj [2:0], Qk [2:0];
	//values to calculate
	reg signed [31:0] Vj [2:0], Vk [2:0];
	//reservation station full indicators
	reg Busy [2:0];
	//alu calculating
	reg Unit_Busy;
	reg [1:0] adder_calculating;
	//Reservation Station (RS) number of each station in this unit
	reg [5:0] RS_num_of [2:0];
	//we want to cycle the order the reservation stations are selected
	//   to go to execution to prevent starvation
	reg [1:0] Priority_Station;
	wire [1:0] Second_Station;
	wire [1:0] Last_Station;
	//Intermediate wires in calculation of available RS
	wire [5:0] RS_availability_of_Second_or_Last;
	wire [5:0] RS_availability_of_Last;
	
	//use tri state gates to connect the CDB output registers to
	//   the CDB, with CDB_xmit as control, thus handling writes
	assign CDB_data = CDB_xmit ? CDB_data_out : disconnected;
	assign CDB_source = CDB_xmit ? CDB_source_out : disconnected;
	assign CDB_write = CDB_xmit ? CDB_write_out : disconnected;
	
	//available indicates that at least one RS is not busy
	assign available = ~(Busy[0] & Busy[1] & Busy[2]);
	//assign RS_availability_of_Last = 
	//	finish this line
	//assing RS_availability_of_Second_or_Last = 
	//	finish this line
	assign RS_available = ~Busy[Priority_Station] ? 
		RS_num_of[Priority_Station] : 
		RS_availability_of_Second_or_Last;
	
	//RS executing, so issue unit can track
	//assign RS_exectuing = //finish this line
	
	//cycle the order of the stations
	mod3counter mod3count1(
		.num(Priority_Station),
		.mod3num(Second_Station)
	);
	//calculate the last station
	
	initial
	begin
		CDB_rts <= not_ready;
		CDB_data_out <= clear;
		CDB_source_out <= no_rs;
		CDB_write_out <= not_ready;
		Priority_Station <= no_rs;
		Unit_Busy <= not_busy;
		Busy[0] <= not_busy;
		Busy[1] <= not_busy;
		Busy[2] <= not_busy;
		issued <= not_issued;
		error <= no_error;
		RS_num_of[0] <= adder_1;
		RS_num_of[1] <= adder_2;
		RS_num_of[2] <= adder_3;
		adder_calculating <= none;
	end
	
	always @ (posedge(clock))
	begin
		//clean messages out each cycle
		issued <= not_issued;
		error <= no_error;
	end
	
	always @ (negedge(CDB_xmit))
	begin
		//clean up write when finished
		CDB_rts <= not_ready;
		//CDB_write_out // finish this line
		//Unit_Busy // finish this line
		//Busy[adder_calculating] // finish this line
		//adder_calculating // finish this line
	end
	
	always @ (posedge(clock))
	begin
		//each cycle handle execution
	end
	
	always @ (negedge(clock))
	begin
		//handle updates
	end
	
	always @ (negedge(clock))
	begin
		//handle issue
	end
endmodule
