module instruction_queue (
        input wire clock,
        input wire issue_error,
        input wire adder_available,
        input wire [5:0] adder_RS_available,
        input wire [5:0] RS_issued,
        input wire [5:0] RS_executing_adder,
        input wire adder_rts,
        input wire [5:0] RS_finished,

        output reg [5:0] operation,
        output reg [2:0] execution_unit,
        output reg [4:0] Dest_address,
        output reg [4:0] A_address,
        output reg [4:0] B_address,
        output reg issue
    );

    parameter TRUE                     = 1'b1;
    parameter FALSE                    = 1'b0;
    parameter LAST_INSTRUCTION_ELEMENT = 1023;
    parameter INSTRUCTION_ADDRESS_BITS = 10;
    parameter LAST_QUEUE_ELEMENT       = 7;
    parameter QUEUE_ADDRESS_BITS       = 3;
    parameter CLEAR                    = 0;
    parameter OPCODE_HIGH              = 31;
    parameter OPCODE_LOW               = 26;
    parameter EXEC_UNIT_HIGH           = 31;
    parameter EXEC_UNIT_LOW            = 29;
    parameter SOURCE1_HIGH             = 25;
    parameter SOURCE1_LOW              = 21;
    parameter SOURCE2_HIGH             = 20;
    parameter SOURCE2_LOW              = 16;
    parameter DESTINATION_HIGH         = 15;
    parameter DESTINATION_LOW          = 11;
    parameter BUSY_MASK                = 4'b0001;
    parameter ISSUE_MASK               = 4'b0010;
    parameter EXECUTE_MASK             = 4'b0100;
    parameter WRITE_BACK_MASK          = 4'b1000;
    parameter ADDER                    = 3'b000;
    parameter alu_add                  = 3'b000;
    parameter alu_sub                  = 3'b001;
    parameter alu_or                   = 3'b100;
    parameter alu_and                  = 3'b101;
    parameter alu_not                  = 3'b110;
    parameter alu_xor                  = 3'b111;

    integer i, j;

    reg issued_this_clock = 0;
    reg [31:0] Instruction_Memory [LAST_INSTRUCTION_ELEMENT:0];
    reg [INSTRUCTION_ADDRESS_BITS:0] PC = 0;
    reg next_queue_location;
    reg [31:0] Instruction [LAST_QUEUE_ELEMENT:0];
    reg [5:0] RS_Holding [LAST_QUEUE_ELEMENT:0];
    reg [3:0] Status [LAST_QUEUE_ELEMENT:0];
    reg [QUEUE_ADDRESS_BITS-1:0] Queue_End = CLEAR;

    // Initialize values and fill up instruction memory
    initial begin
        issue <= FALSE;
        for (i = 0; i <= LAST_QUEUE_ELEMENT; i = i + 1) begin
            Instruction[i] <= CLEAR;
            RS_Holding[i]  <= CLEAR;
            Status[i]      <= CLEAR;
        end
        Instruction_Memory[0] <= {ADDER, alu_add, 5'b00001, 5'b00100, 5'b00010, 11'b0}; // R1 + R4  => R2
        Instruction_Memory[1] <= {ADDER, alu_add, 5'b00010, 5'b00111, 5'b00011, 11'b0}; // R2 + R7  => R3
        Instruction_Memory[2] <= {ADDER, alu_add, 5'b00011, 5'b01010, 5'b00100, 11'b0}; // R3 + R10 => R4
        Instruction_Memory[3] <= {ADDER, alu_add, 5'b00100, 5'b01101, 5'b00101, 11'b0}; // R4 + R13 => R5
        Instruction_Memory[4] <= {ADDER, alu_add, 5'b00101, 5'b10000, 5'b00110, 11'b0}; // R5 + R16 => R6
        for (i = 5; i < 32; i = i + 1) begin
            Instruction_Memory[i] <= {ADDER, alu_add, 5'b00000, 5'b00000, 5'b00000, 11'b0};
        end
    end

    always @(posedge clock) begin
        // If there are still items in the queue, and there's no issue error
        if ((PC == 0 || Queue_End != CLEAR) &&
           !issue_error) begin
            // Issue the instruction
            Instruction[Queue_End] <= Instruction_Memory[PC];
            Status[Queue_End]      <= Status[Queue_End] | BUSY_MASK;
            RS_Holding[Queue_End]  <= CLEAR;
            // Update the program counter and queue
            PC        <= PC + 1;
            Queue_End <= Queue_End + 1;
        end
    end

    always @(posedge clock) begin
        // Check that the adder is available for new inputs
        if (adder_available) begin
            // For each item in the queue
            for (i = 0; i <= Queue_End - 1; i = i + 1) begin
                // If everything is ready and we haven't issued yet, issue
                if (!issued_this_clock &&
                   (Status[i] & BUSY_MASK) == 1 &&
                   (Status[i] & ISSUE_MASK) == 0) begin
                    // The relevant reservation station
                    RS_Holding[i] <= adder_RS_available;

                    // Pull out all the parts of the current instruction
                    operation      <= Instruction[i][OPCODE_HIGH:OPCODE_LOW];
                    execution_unit <= Instruction[i][EXEC_UNIT_HIGH:EXEC_UNIT_LOW];
                    Dest_address   <= Instruction[i][DESTINATION_HIGH:DESTINATION_LOW];
                    A_address      <= Instruction[i][SOURCE1_HIGH:SOURCE1_LOW];
                    B_address      <= Instruction[i][SOURCE2_HIGH:SOURCE2_LOW];

                    // Set the issue flags to true
                    issue             <= TRUE;
                    issued_this_clock <= TRUE;

                    // Block for one time unit to make sure issues happen on
                    // the correct timesclae
                    #1;
                end
            end
        end
    end

    // Whenever the reservation station status changes
    always @(RS_issued or RS_executing_adder) begin
        // If either is true (they might both be false, so we have to check)
        if (RS_issued || RS_executing_adder) begin
            // For each item in the queue...
            for (i = 0; i <= Queue_End - 1; i = i + 1) begin
                // If it's been issued
                if (RS_issued != CLEAR &&
                   (Status[i] & BUSY_MASK) &&
                   RS_Holding[i] == RS_issued) begin
                    // Update the current status value to indicate we've issued
                    Status[i] = Status[i] | ISSUE_MASK;
                end
                // If it's been executed
                if (RS_executing_adder != CLEAR &&
                   (Status[i] & BUSY_MASK) &&
                   RS_Holding[i] == RS_executing_adder) begin
                    // Update the current status value to indicate execution
                    Status[i] = Status[i] | EXECUTE_MASK;
                end
            end
        end
    end

    // Whenever the reservation station is done
    always @(RS_finished) begin
        // If the reservation station is done
        if (RS_finished) begin
            // For each item in the queue
            for (i = 0; i <= Queue_End - 1; i = i + 1) begin
                // If the item is busy and the reservation station is finished
                if ((Status[i] & BUSY_MASK) &&
                   RS_Holding[i] == RS_finished) begin
                    // For each item in the queue
                    for (j = i; j < Queue_End; j = j + 1) begin
                        // Move the item up one step
                        Instruction[j] = Instruction[j + 1];
                        Status[j]      = Status[j + 1];
                        RS_Holding[j]  = RS_Holding[j + 1];
                    end

                    // Clear out the old head of the queue.
                    Instruction[Queue_End - 1] = CLEAR;
                    Status[Queue_End - 1]      = CLEAR;
                    RS_Holding[Queue_End - 1]  = CLEAR;

                    // Move to the next item.
                    Queue_End = Queue_End - 1;
                end
            end
        end
    end
endmodule
