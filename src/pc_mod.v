module pc_mod (
        output reg  [31:0] PC,
        input  wire [31:0] npc
    );

    initial begin
        PC <= 0;
    end

    always @ (npc) begin
        #1 PC <= npc;
    end
endmodule