module adders (
        input  wire clock,
                    issue,

        input  wire signed [31:0] A,
                                  B,
        input  wire A_invalid,
                    B_invalid,
        input  wire [5:0] opcode,

        input  wire CDB_xmit,
        inout  wire signed [31:0] CDB_data,
        inout  wire [5:0] CDB_source,
        inout  wire CDB_write,
        output reg  CDB_rts,

        output wire available,
        output wire [5:0] RS_available,
        output reg  [5:0] issued,
        output wire [5:0] RS_executing,
        output reg  error
    );

    parameter disconnected = 32'bz;
    parameter delay        = 20;
    parameter update_delay = 5;
    parameter none         = 2'b00;
    parameter valid        = 6'b000000;
    parameter no_error     = 1'b0;
    parameter all_rs_bus   = 1'b1;
    parameter not_issued   = 6'b000000;
    parameter not_busy     = 1'b0;
    parameter in_use       = 1'b1;
    parameter ready        = 1'b1;
    parameter not_ready    = 1'b0;
    parameter clear        = 0;
    parameter no_rs        = 6'b000000;
    parameter adder_1      = 6'b000001;
    parameter adder_2      = 6'b000010;
    parameter adder_3      = 6'b000011;
    parameter alu_add      = 3'b000;
    parameter alu_sub      = 3'b001;
    parameter alu_or       = 3'b100;
    parameter alu_and      = 3'b101;
    parameter alu_not      = 3'b110;
    parameter alu_xor      = 3'b111;

    reg  signed [31:0] CDB_data_out;
    reg          [5:0] CDB_source_out;
    reg                CDB_write_out;
    reg  [5:0] operation [2:0];
    reg  [5:0] Qj [2:0],
               Qk [2:0];
    reg  signed [31:0] Vj [2:0],
                       Vk [2:0];
    reg  Busy [2:0];
    reg  Unit_Busy;
    reg  [1:0] adder_calculating;
    reg  [5:0] RS_num_of [2:0];
    reg  [1:0] Priority_Station;
    wire [1:0] Second_Station;
    wire [1:0] Last_Station;
    wire [5:0] RS_availability_of_Second_or_Last;
    wire [5:0] RS_availability_of_Last;

    assign CDB_data   = CDB_xmit ? CDB_data_out   : disconnected;
    assign CDB_source = CDB_xmit ? CDB_source_out : disconnected;
    assign CDB_write  = CDB_xmit ? CDB_write_out  : disconnected;

    assign available = ~(Busy[0] & Busy[1] & Busy[2]);
    assign RS_avalaibility_of_Last = ~Busy[Last_Station] ?
                RS_num_of[Last_Station] : no_rs;
    assign RS_availability_of_Second_or_Last = ~Busy[Second_Station] ?
                RS_num_of[Second_Station] : RS_num_of[Last_Station];
    assign RS_available = ~Busy[Priority_Station] ?
                RS_num_of[Priority_Station] : RS_availability_of_Second_or_Last;
    assign RS_executing = Unit_Busy ?
                RS_num_of[adder_calculating] : no_rs;

    mod3counter mod1 (
        .num(Priority_Station),
        .mod3num(Second_Station)
    );

    mod3counter mod2 (
        .num(Second_Station),
        .mod3num(Last_Station)
    );

    mod3counter mod3 (
        .num(Last_Station),
        .mod3num(Priority_Station)
    );

    initial begin
        CDB_rts           <= not_ready;
        CDB_data_out      <= clear;
        CDB_source_out    <= no_rs;
        CDB_write_out     <= not_ready;
        Priority_Station  <= no_rs;
        Unit_Busy         <= not_busy;
        Busy[0]           <= not_busy;
        Busy[1]           <= not_busy;
        Busy[2]           <= not_busy;
        issued            <= not_issued;
        error             <= no_error;
        RS_num_of[0]      <= adder_1;
        RS_num_of[1]      <= adder_2;
        RS_num_of[2]      <= adder_3;
        adder_calculating <= none;
    end

    always @ (posedge(clock)) begin
        issued <= not_issued;
        error  <= no_error;
    end

    always @ (negedge(CDB_xmit)) begin
        CDB_rts                 <= not_ready;
        CDB_write_out           <= not_ready;
        Unit_Busy               <= not_busy;
        Busy[adder_calculating] <= not_busy;
        adder_calculating       <= none;
    end

    always @ (posedge(clock)) begin
        if (~Unit_Busy) begin
            if (Busy[Priority_Station] &&
                Qj[Priority_Station] == valid &&
                Qk[Priority_Station] == valid) begin
                case (operation[Priority_Station])
                    alu_add: CDB_data_out <= Vj[Priority_Station] + Vk[Priority_Station];
                    alu_sub: CDB_data_out <= Vj[Priority_Station] - Vk[Priority_Station];
                    alu_or:  CDB_data_out <= Vj[Priority_Station] | Vk[Priority_Station];
                    alu_and: CDB_data_out <= Vj[Priority_Station] & Vk[Priority_Station];
                    alu_xor: CDB_data_out <= Vj[Priority_Station] ^ Vk[Priority_Station];
                    alu_not: CDB_data_out <= ~Vj[Priority_Station];
                endcase

                #delay;
                Unit_Busy         <= in_use;
                CDB_source_out    <= RS_num_of[Priority_Station];
                CDB_write_out     <= ready;
                CDB_rts           <= 1;
                adder_calculating <= Priority_Station;
            end
            else if (Busy[Second_Station] &&
                     Qj[Second_Station] == valid &&
                     Qk[Second_Station] == valid) begin
                case (operation[Second_Station])
                    alu_add: CDB_data_out <= Vj[Second_Station] + Vk[Second_Station];
                    alu_sub: CDB_data_out <= Vj[Second_Station] - Vk[Second_Station];
                    alu_or:  CDB_data_out <= Vj[Second_Station] | Vk[Second_Station];
                    alu_and: CDB_data_out <= Vj[Second_Station] & Vk[Second_Station];
                    alu_xor: CDB_data_out <= Vj[Second_Station] ^ Vk[Second_Station];
                    alu_not: CDB_data_out <= ~Vj[Second_Station];
                endcase

                #delay;
                Unit_Busy         <= in_use;
                CDB_source_out    <= RS_num_of[Second_Station];
                CDB_write_out     <= ready;
                CDB_rts           <= 1;
                adder_calculating <= Second_Station;
            end
            else if (Busy[Last_Station] &&
                     Qj[Last_Station] == valid &&
                     Qk[Last_Station] == valid) begin
                case (operation[Last_Station])
                    alu_add: CDB_data_out <= Vj[Last_Station] + Vk[Last_Station];
                    alu_sub: CDB_data_out <= Vj[Last_Station] - Vk[Last_Station];
                    alu_or:  CDB_data_out <= Vj[Last_Station] | Vk[Last_Station];
                    alu_and: CDB_data_out <= Vj[Last_Station] & Vk[Last_Station];
                    alu_xor: CDB_data_out <= Vj[Last_Station] ^ Vk[Last_Station];
                    alu_not: CDB_data_out <= ~Vj[Last_Station];
                endcase

                #delay;
                Unit_Busy         <= in_use;
                CDB_source_out    <= RS_num_of[Last_Station];
                CDB_write_out     <= ready;
                CDB_rts           <= 1;
                adder_calculating <= Last_Station;
            end
        end
    end

    always @ (negedge(clock)) begin
        if (CDB_write) begin
            #update_delay;
            if (Qj[Priority_Station] == CDB_source ||
                Qk[Priority_Station] == CDB_source) begin
                if (Qj[Priority_Station] == CDB_source) begin
                    Vj[Priority_Station] <= CDB_data;
                    Qj[Priority_Station] <= valid;
                end
                if (Qk[Priority_Station] == CDB_source) begin
                    Vk[Priority_Station] <= CDB_data;
                    Qk[Priority_Station] <= valid;
                end
            end
            else if (Qj[Second_Station] == CDB_source ||
                     Qk[Second_Station] == CDB_source) begin
                if (Qj[Second_Station] == CDB_source) begin
                    Vj[Second_Station] <= CDB_data;
                    Qj[Second_Station] <= valid;
                end
                if (Qk[Second_Station] == CDB_source) begin
                    Vk[Second_Station] <= CDB_data;
                    Qk[Second_Station] <= valid;
                end
            end
            else begin
                if(Qj[Last_Station] == CDB_source) begin
                    Vj[Last_Station] <= CDB_data;
                    Qj[Last_Station] <= valid;
                end
                if(Qk[Last_Station] == CDB_source) begin
                    Vk[Last_Station] <= CDB_data;
                    Qk[Last_Station] <= valid;
                end
            end
        end
    end

    always @ (negedge(clock)) begin
        if (issue) begin
            if (RS_available) begin

                if (~Busy[Priority_Station]) begin
                    Busy[Priority_Station] <= in_use;
                    operation[Priority_Station] <= opcode;
                    issued <= RS_num_of[Priority_Station];
                    if (A_invalid) begin
                        Qj[Priority_Station] <= A[5:0];
                    end
                    else begin
                        Qj[Priority_Station] <= valid;
                        Vj[Priority_Station] <= A;
                    end
                    if (B_invalid) begin
                        Qk[Priority_Station] <= B[5:0];
                    end
                    else begin
                        Qk[Priority_Station] <= valid;
                        Vk[Priority_Station] <= B;
                    end
                    error <= no_error;
                end

                else if (~Busy[Second_Station]) begin
                    Busy[Second_Station] <= in_use;
                    issued <= RS_num_of[Second_Station];
                    operation[Second_Station] <= opcode;
                    if (A_invalid) begin
                        Qj[Second_Station] <= A[5:0];
                    end
                    else begin
                        Qj[Second_Station] <= valid;
                        Vj[Second_Station] <= A;
                    end
                    if (B_invalid) begin
                        Qk[Second_Station] <= B[5:0];
                    end
                    else begin
                        Qk[Second_Station] <= valid;
                        Vk[Second_Station] <= B;
                    end
                    error <= no_error;
                end

                else if (~Busy[Last_Station]) begin
                    Busy[Last_Station] <= in_use;
                    issued <= RS_num_of[Last_Station];
                    operation[Last_Station] <= opcode;
                    if (A_invalid) begin
                        Qj[Last_Station] <= A[5:0];
                    end
                    else begin
                        Qj[Last_Station] <= valid;
                        Vj[Last_Station] <= A;
                    end
                    if (B_invalid) begin
                        Qk[Last_Station] <= B[5:0];
                    end
                    else begin
                        Qk[Last_Station] <= valid;
                        Vk[Last_Station] <= B;
                    end
                    error <= no_error;
                end

                else begin
                    error <= all_rs_busy;
                end

            end
        end
    end
endmodule
