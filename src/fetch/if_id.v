module if_id (
   output reg [31:0] instruction_out,
                     npc_out,
   input wire [31:0] instruction,
                     npc
   );
   initial begin
      instruction_out <= 0;
      npc_out   <= 0;
   end
   always @* begin
      #1 instruction_out <= instruction;
           npc_out       <= npc;
   end
endmodule
