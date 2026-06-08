// Behavioral stub for Altera's `altsyncram` megafunction (the V word above is verboten).
//
// minimig's `denise_colortable_ram_mf.v` and `denise_hamgenerator_ram_mf.v`
// instantiate Altera's `altsyncram` primitive.  Verilator doesn't ship
// the Altera library, so we provide a behavioral dual-port RAM that
// covers the configurations minimig uses (256x32, write port A with
// byte-enable, read port B).  The `defparam` values from the wizard
// output are simply ignored.

module altsyncram #(
    parameter address_aclr_b                       = "NONE",
    parameter address_reg_b                        = "CLOCK0",
    parameter byte_size                            = 8,
    parameter clock_enable_input_a                 = "NORMAL",
    parameter clock_enable_input_b                 = "NORMAL",
    parameter clock_enable_output_b                = "BYPASS",
    parameter intended_device_family               = "Cyclone III",
    parameter lpm_type                             = "altsyncram",
    parameter numwords_a                           = 256,
    parameter numwords_b                           = 256,
    parameter operation_mode                       = "DUAL_PORT",
    parameter outdata_aclr_b                       = "NONE",
    parameter outdata_reg_b                        = "UNREGISTERED",
    parameter power_up_uninitialized               = "FALSE",
    parameter read_during_write_mode_mixed_ports   = "OLD_DATA",
    parameter widthad_a                            = 8,
    parameter widthad_b                            = 8,
    parameter width_a                              = 32,
    parameter width_b                              = 32,
    parameter width_byteena_a                      = 4
) (
    input  wire        clock0,
    input  wire        clock1,
    input  wire        clocken0,
    input  wire        clocken1,
    input  wire        clocken2,
    input  wire        clocken3,
    input  wire        aclr0,
    input  wire        aclr1,
    input  wire        addressstall_a,
    input  wire        addressstall_b,
    input  wire [7:0]  address_a,
    input  wire [7:0]  address_b,
    input  wire [31:0] data_a,
    input  wire [31:0] data_b,
    input  wire [3:0]  byteena_a,
    input  wire        byteena_b,
    input  wire        wren_a,
    input  wire        wren_b,
    input  wire        rden_a,
    input  wire        rden_b,
    output reg  [31:0] q_a,
    output reg  [31:0] q_b,
    output wire        eccstatus
);
    assign eccstatus = 1'b0;

    reg [31:0] mem [0:255];

    // Port A write with byte enable.
    always @(posedge clock0) begin
        if (wren_a) begin
            if (byteena_a[3]) mem[address_a][31:24] <= data_a[31:24];
            if (byteena_a[2]) mem[address_a][23:16] <= data_a[23:16];
            if (byteena_a[1]) mem[address_a][15:8]  <= data_a[15:8];
            if (byteena_a[0]) mem[address_a][7:0]   <= data_a[7:0];
        end
    end

    // Port B read (unregistered output per the defparam).
    always @* begin
        q_b = mem[address_b];
        q_a = 32'd0;
    end

    // Silence unused-input warnings.
    wire _unused = &{clock1, clocken0, clocken1, clocken2, clocken3,
                     aclr0, aclr1, addressstall_a, addressstall_b,
                     data_b, byteena_b, wren_b, rden_a, rden_b, 1'b0};

endmodule
