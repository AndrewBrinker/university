module pc_mod (
        input  wire [31:0] npc,

        output reg  [31:0] PC
    );

    initial begin
        PC <= 0;
    end

    always @ (npc) begin
        #1
        PC <= npc;
    end
endmodule
