module mod3counter(
        input wire [1:0] num,
        output reg [1:0] mod3num
    );

    always @ * begin
        mod3num = (num + 1) % 3;
    end
endmodule
