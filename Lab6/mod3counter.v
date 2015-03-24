module mod3counter(
        input wire [1:0] num,
        output reg [1:0] mod3num
    );

    always @ * begin
        if (num == 2'b11) begin
            mod3num <= 2'b01;
        end
        else begin
            mod3num <= (num + 1) % 3;
        end
    end
endmodule
