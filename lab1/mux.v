// The multiplexer picks between the program
// counter and the jump address, according
// to the select.
module mux(pc, jump, select, out);
    input  [31:0] pc, jump;
    input  select;
    output [31:0] out;
    assign out = select ? pc : jump;
endmodule