// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vm68k_top.h for the primary calling header

#ifndef VERILATED_VM68K_TOP___024ROOT_H_
#define VERILATED_VM68K_TOP___024ROOT_H_  // guard

#include "verilated.h"


class Vm68k_top__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vm68k_top___024root final {
  public:

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        VL_IN8(clk,0,0);
        VL_IN8(rst_n,0,0);
        VL_OUT8(halted,0,0);
        CData/*1:0*/ m68k_top__DOT__p_we;
        CData/*1:0*/ m68k_top__DOT__p_resp_valid;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_we;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock;
        CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_be;
        CData/*3:0*/ m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_be;
        CData/*0:0*/ m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_lock;
        CData/*0:0*/ m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_we;
        CData/*3:0*/ m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_be;
        CData/*0:0*/ m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_lock;
        CData/*0:0*/ m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_we;
        CData/*0:0*/ m68k_top__DOT__u_bus__DOT__lock_pending;
        CData/*0:0*/ m68k_top__DOT__u_bus__DOT__lock_holder;
        CData/*0:0*/ m68k_top__DOT__u_bus__DOT__rrobin;
        CData/*0:0*/ m68k_top__DOT__u_bus__DOT__winner;
        CData/*0:0*/ m68k_top__DOT__u_bus__DOT__winner_valid;
        CData/*0:0*/ m68k_top__DOT__u_bus__DOT__granted_valid_q;
        CData/*0:0*/ m68k_top__DOT__u_bus__DOT__granted_port_q;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__state;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__bus_req_r;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__bus_resp_now;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__state;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__bus_req_r;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__bus_resp_now;
        CData/*7:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT____VlemCall_2__byte_at;
        CData/*7:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT____VlemCall_1__byte_at;
        CData/*7:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT____VlemCall_0__byte_at;
        CData/*2:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_words_done;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy;
        CData/*2:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching;
        CData/*2:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__new_words_done;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__would_be_complete;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__predict_taken_w;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_dispatch_w;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_valid;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_predicted_taken;
        CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind;
        CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op;
        CData/*2:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode;
        CData/*2:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg;
        CData/*2:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode;
        CData/*2:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg;
        CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_cc;
        CData/*1:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words;
        CData/*1:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_ext_words;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_needs_disp_word;
        CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_src_base_idx;
        CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_reg_idx_full;
        CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_rb_idx;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_we;
        CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_widx;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we;
        CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid;
        CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind;
        CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op;
        CData/*2:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode;
    };
    struct {
        CData/*2:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg;
        CData/*2:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode;
        CData/*2:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg;
        CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc;
        CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_reg_idx_full;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_predicted_taken;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c;
        CData/*1:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_we;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_lock;
        CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_be;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v_c;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c_c;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dc_req_r;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled;
        CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stop_now;
        CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__5__pos;
        CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__6__pos;
        CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__10__Vfuncout;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__10__m;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__10__r;
        CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__11__Vfuncout;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__11__m;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__11__r;
        CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__Vfuncout;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__m;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__r;
        CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__Vfuncout;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__m;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__r;
        CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__Vfuncout;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__m;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__r;
        CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__Vfuncout;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__m;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__r;
        CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__16__Vfuncout;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__16__m;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__16__r;
        CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__17__Vfuncout;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__17__m;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__17__r;
        CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__18__Vfuncout;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__18__m;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__18__r;
        CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__19__Vfuncout;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__19__m;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__19__r;
        CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__Vfuncout;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__m;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__r;
        CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__Vfuncout;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__m;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__r;
        CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__Vfuncout;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__m;
    };
    struct {
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__r;
        CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__23__Vfuncout;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__23__m;
        CData/*2:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__23__r;
        CData/*0:0*/ __VstlFirstIteration;
        CData/*0:0*/ __VstlPhaseResult;
        CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__rst_n__0;
        CData/*0:0*/ __VactPhaseResult;
        CData/*0:0*/ __VnbaPhaseResult;
        VL_OUT16(halt_code_flat,15,0);
        SData/*13:0*/ m68k_top__DOT__u_bus__DOT__granted_idx_q;
        SData/*13:0*/ m68k_top__DOT__u_bus__DOT__mem_idx;
        SData/*15:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_op;
        SData/*15:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w;
        SData/*15:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op;
        SData/*15:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op;
        VL_OUT(retired_flat,31,0);
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_wdata;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_rdata;
        IData/*31:0*/ m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_wdata;
        IData/*31:0*/ m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_addr;
        IData/*31:0*/ m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_wdata;
        IData/*31:0*/ m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_addr;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_addr;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_target;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_pc;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_imm;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_wdata;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_pc;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_pc_next;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_imm32;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_imm32;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_branch_imm;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_imm_raw;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_ra;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_sp;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_tas_word;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_addr;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_wdata;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_target_c;
        IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y;
        IData/*31:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__5__w;
        IData/*31:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__6__w;
        IData/*31:0*/ __VdfgRegularize_hebeb780c_0_5;
        IData/*31:0*/ __VactIterCount;
        VlUnpacked<IData/*31:0*/, 16384> m68k_top__DOT__u_bus__DOT__mem;
        VlUnpacked<IData/*31:0*/, 2> m68k_top__DOT__u_bus__DOT__addr;
        VlUnpacked<IData/*31:0*/, 2> m68k_top__DOT__u_bus__DOT__wdata;
        VlUnpacked<CData/*3:0*/, 2> m68k_top__DOT__u_bus__DOT__be;
        VlUnpacked<SData/*15:0*/, 4> m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext;
        VlUnpacked<SData/*15:0*/, 4> m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ext_combo;
        VlUnpacked<SData/*15:0*/, 4> m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext;
        VlUnpacked<IData/*31:0*/, 16> m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs;
        VlUnpacked<QData/*63:0*/, 1> __VstlTriggered;
        VlUnpacked<QData/*63:0*/, 1> __VactTriggered;
        VlUnpacked<QData/*63:0*/, 1> __VnbaTriggered;
    };

    // INTERNAL VARIABLES
    Vm68k_top__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    Vm68k_top___024root(Vm68k_top__Syms* symsp, const char* namep);
    ~Vm68k_top___024root();
    VL_UNCOPYABLE(Vm68k_top___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
