`include "../src/incr.v"

module incrTest();
    wire  [31:0] out;
    reg   [31:0] a;

    incr incr1 (
        .in(a),
        .out(out)
    );

    initial begin
        #10
        a = 3;
        #10
        a = 15;
        #10
        a = 64;
        #5;
    end

    always @ (a)
    #1 $display("Time = %0d\ta = %0d\tout = %0d", $time, a, out);
endmodule
