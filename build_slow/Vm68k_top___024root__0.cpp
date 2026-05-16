// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vm68k_top.h for the primary calling header

#include "Vm68k_top__pch.h"

void Vm68k_top___024root___eval_triggers_vec__act(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___eval_triggers_vec__act\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered[0U] = (QData)((IData)(
                                                    ((((~ (IData)(vlSelfRef.rst_n)) 
                                                       & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rst_n__0)) 
                                                      << 1U) 
                                                     | ((IData)(vlSelfRef.clk) 
                                                        & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__clk__0))))));
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rst_n__0 = vlSelfRef.rst_n;
}

bool Vm68k_top___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___trigger_anySet__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

void Vm68k_top___024root___nba_sequent__TOP__0(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___nba_sequent__TOP__0\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*1:0*/ m68k_top__DOT__p_req;
    m68k_top__DOT__p_req = 0;
    IData/*31:0*/ m68k_top__DOT__p_resp_data;
    m68k_top__DOT__p_resp_data = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_rdata;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_rdata = 0;
    CData/*1:0*/ m68k_top__DOT__u_bus__DOT__cand_ext;
    m68k_top__DOT__u_bus__DOT__cand_ext = 0;
    CData/*0:0*/ m68k_top__DOT__u_bus__DOT__cand;
    m68k_top__DOT__u_bus__DOT__cand = 0;
    CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_imm;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_imm = 0;
    CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_cc;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_cc = 0;
    CData/*1:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 0;
    CData/*1:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_needs_disp_word;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_needs_disp_word = 0;
    CData/*2:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__total_words_eff;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__total_words_eff = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_bcc_uncond;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_bcc_uncond = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__disp32;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__disp32 = 0;
    CData/*2:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_ea;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_ea = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_next;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_next = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_update;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_update = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_ea;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_ea = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_next;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_next = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_update;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_update = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_we_c;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_we_c = 0;
    CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_idx_c;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_idx_c = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c = 0;
    CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx_c;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx_c = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data_c;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data_c = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b = 0;
    CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_movq;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_movq = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_addq;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_addq = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_disp;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_disp = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_movq;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_movq = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_addq;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_addq = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_disp;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_disp = 0;
    QData/*32:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum = 0;
    CData/*3:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__7__Vfuncout;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__7__Vfuncout = 0;
    CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__7__pos;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__7__pos = 0;
    IData/*31:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__w;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__w = 0;
    CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__pos;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__pos = 0;
    IData/*31:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__9__Vfuncout;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__9__Vfuncout = 0;
    CData/*7:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__9__b;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__9__b = 0;
    CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__9__pos;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__9__pos = 0;
    SData/*15:0*/ __VdfgRegularize_hebeb780c_0_1;
    __VdfgRegularize_hebeb780c_0_1 = 0;
    CData/*0:0*/ __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain;
    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain = 0;
    CData/*0:0*/ __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy;
    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy = 0;
    IData/*31:0*/ __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_pc;
    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_pc = 0;
    CData/*0:0*/ __Vdly__halted;
    __Vdly__halted = 0;
    CData/*1:0*/ __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state;
    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state = 0;
    CData/*7:0*/ __VdlyVal__m68k_top__DOT__u_bus__DOT__mem__v0;
    __VdlyVal__m68k_top__DOT__u_bus__DOT__mem__v0 = 0;
    SData/*13:0*/ __VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v0;
    __VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v0 = 0;
    CData/*0:0*/ __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v0;
    __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v0 = 0;
    CData/*7:0*/ __VdlyVal__m68k_top__DOT__u_bus__DOT__mem__v1;
    __VdlyVal__m68k_top__DOT__u_bus__DOT__mem__v1 = 0;
    SData/*13:0*/ __VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v1;
    __VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v1 = 0;
    CData/*0:0*/ __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v1;
    __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v1 = 0;
    CData/*7:0*/ __VdlyVal__m68k_top__DOT__u_bus__DOT__mem__v2;
    __VdlyVal__m68k_top__DOT__u_bus__DOT__mem__v2 = 0;
    SData/*13:0*/ __VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v2;
    __VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v2 = 0;
    CData/*0:0*/ __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v2;
    __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v2 = 0;
    CData/*7:0*/ __VdlyVal__m68k_top__DOT__u_bus__DOT__mem__v3;
    __VdlyVal__m68k_top__DOT__u_bus__DOT__mem__v3 = 0;
    SData/*13:0*/ __VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v3;
    __VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v3 = 0;
    CData/*0:0*/ __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v3;
    __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v3 = 0;
    SData/*15:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v0;
    __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v0 = 0;
    CData/*1:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v0;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v0 = 0;
    CData/*0:0*/ __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v0;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v0 = 0;
    CData/*0:0*/ __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v1;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v1 = 0;
    SData/*15:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v0;
    __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v0 = 0;
    CData/*0:0*/ __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v0;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v0 = 0;
    SData/*15:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v1;
    __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v1 = 0;
    SData/*15:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v2;
    __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v2 = 0;
    SData/*15:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v3;
    __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v3 = 0;
    CData/*0:0*/ __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v4;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v4 = 0;
    IData/*31:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0;
    __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0 = 0;
    CData/*3:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0 = 0;
    CData/*0:0*/ __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0 = 0;
    IData/*31:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1;
    __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1 = 0;
    CData/*3:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1 = 0;
    CData/*0:0*/ __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1 = 0;
    CData/*0:0*/ __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v2;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v2 = 0;
    // Body
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0 = 0U;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1 = 0U;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v2 = 0U;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v0 = 0U;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v4 = 0U;
    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_pc 
        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_pc;
    __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v0 = 0U;
    __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v1 = 0U;
    __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v2 = 0U;
    __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v3 = 0U;
    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain 
        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain;
    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy 
        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy;
    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state 
        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state;
    __Vdly__halted = vlSelfRef.halted;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v0 = 0U;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v1 = 0U;
    if (vlSelfRef.rst_n) {
        if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we) 
             & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_we) 
                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx) 
                      == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_widx)))))) {
            __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data;
            __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx;
            __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0 = 1U;
        }
        if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_we) {
            __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_wdata;
            __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_widx;
            __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1 = 1U;
        }
        if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled) 
             & (3U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
            vlSelfRef.retired_flat = ((IData)(1U) + vlSelfRef.retired_flat);
        }
        if (((IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid) 
             & ((IData)(vlSelfRef.m68k_top__DOT__p_we) 
                >> (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner)))) {
            if ((8U & vlSelfRef.m68k_top__DOT__u_bus__DOT__be
                 [vlSelfRef.m68k_top__DOT__u_bus__DOT__winner])) {
                __VdlyVal__m68k_top__DOT__u_bus__DOT__mem__v0 
                    = (vlSelfRef.m68k_top__DOT__u_bus__DOT__wdata
                       [vlSelfRef.m68k_top__DOT__u_bus__DOT__winner] 
                       >> 0x18U);
                __VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v0 
                    = vlSelfRef.m68k_top__DOT__u_bus__DOT__mem_idx;
                __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v0 = 1U;
            }
            if ((4U & vlSelfRef.m68k_top__DOT__u_bus__DOT__be
                 [vlSelfRef.m68k_top__DOT__u_bus__DOT__winner])) {
                __VdlyVal__m68k_top__DOT__u_bus__DOT__mem__v1 
                    = (0x000000ffU & (vlSelfRef.m68k_top__DOT__u_bus__DOT__wdata
                                      [vlSelfRef.m68k_top__DOT__u_bus__DOT__winner] 
                                      >> 0x10U));
                __VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v1 
                    = vlSelfRef.m68k_top__DOT__u_bus__DOT__mem_idx;
                __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v1 = 1U;
            }
            if ((2U & vlSelfRef.m68k_top__DOT__u_bus__DOT__be
                 [vlSelfRef.m68k_top__DOT__u_bus__DOT__winner])) {
                __VdlyVal__m68k_top__DOT__u_bus__DOT__mem__v2 
                    = (0x000000ffU & (vlSelfRef.m68k_top__DOT__u_bus__DOT__wdata
                                      [vlSelfRef.m68k_top__DOT__u_bus__DOT__winner] 
                                      >> 8U));
                __VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v2 
                    = vlSelfRef.m68k_top__DOT__u_bus__DOT__mem_idx;
                __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v2 = 1U;
            }
            if ((1U & vlSelfRef.m68k_top__DOT__u_bus__DOT__be
                 [vlSelfRef.m68k_top__DOT__u_bus__DOT__winner])) {
                __VdlyVal__m68k_top__DOT__u_bus__DOT__mem__v3 
                    = (0x000000ffU & vlSelfRef.m68k_top__DOT__u_bus__DOT__wdata
                       [vlSelfRef.m68k_top__DOT__u_bus__DOT__winner]);
                __VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v3 
                    = vlSelfRef.m68k_top__DOT__u_bus__DOT__mem_idx;
                __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v3 = 1U;
            }
        }
        if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled) 
             & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c_c;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v_c;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c;
        }
        vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_idx_q 
            = vlSelfRef.m68k_top__DOT__u_bus__DOT__mem_idx;
        if (((IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid) 
             & (~ (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_pending)))) {
            vlSelfRef.m68k_top__DOT__u_bus__DOT__rrobin 
                = (1U & ((~ (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner)) 
                         & ((IData)(1U) + (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner))));
        }
        if (vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid) {
            if (vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_pending) {
                vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_pending = 0U;
            } else if ((1U & ((((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_lock) 
                                << 1U) | (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_lock)) 
                              >> (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner)))) {
                vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_pending = 1U;
                vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_holder 
                    = vlSelfRef.m68k_top__DOT__u_bus__DOT__winner;
            }
        }
        if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__state) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__p_resp_valid))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__bus_req_r = 0U;
                vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_we = 0U;
                vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_lock = 0U;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__state = 0U;
            }
        } else if (((~ (IData)(vlSelfRef.halted)) & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__bus_req_r = 1U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_we = 0U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_lock = 0U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_addr 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_wdata = 0U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_be = 0x0fU;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__state = 1U;
        }
        if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__state) {
            if ((2U & (IData)(vlSelfRef.m68k_top__DOT__p_resp_valid))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__bus_req_r = 0U;
                vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_we = 0U;
                vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_lock = 0U;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__state = 0U;
            }
        } else if (((~ (IData)(vlSelfRef.halted)) & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dc_req_r))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__bus_req_r = 1U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_we 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_we;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_lock 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_addr 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_wdata 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_wdata;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_be 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_be;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__state = 1U;
        }
        if (((((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid) 
               & (3U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) 
              & (0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) 
             & (~ (IData)(vlSelfRef.halted)))) {
            __Vdly__halted = 1U;
            vlSelfRef.halt_code_flat = (0x0000ffffU 
                                        & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_imm_raw);
        }
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
                if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__bus_resp_now) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dc_req_r = 0U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock = 0U;
                    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state = 0U;
                }
            } else if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__bus_resp_now) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dc_req_r = 0U;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock = 0U;
                __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state = 0U;
            }
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
            if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__bus_resp_now) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dc_req_r = 0U;
                if ((0x0bU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__7__pos 
                        = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr);
                    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__pos 
                        = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr);
                    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__7__Vfuncout 
                        = ((2U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__7__pos))
                            ? ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__7__pos))
                                ? 1U : 2U) : ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__7__pos))
                                               ? 4U
                                               : 8U));
                    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__w 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_rdata;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_tas_word 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_rdata;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dc_req_r = 1U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_we = 1U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock = 1U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_be 
                        = __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__7__Vfuncout;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT____VlemCall_2__byte_at 
                        = (0x000000ffU & ((2U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__pos))
                                           ? ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__pos))
                                               ? __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__w
                                               : (__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__w 
                                                  >> 8U))
                                           : ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__pos))
                                               ? (__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__w 
                                                  >> 0x10U)
                                               : (__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__w 
                                                  >> 0x18U))));
                    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state = 3U;
                    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__9__pos 
                        = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr);
                    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__9__b 
                        = (0x80U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT____VlemCall_2__byte_at));
                    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__9__Vfuncout 
                        = ((2U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__9__pos))
                            ? ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__9__pos))
                                ? (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__9__b)
                                : ((IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__9__b) 
                                   << 8U)) : ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__9__pos))
                                               ? ((IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__9__b) 
                                                  << 0x00000010U)
                                               : ((IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__9__b) 
                                                  << 0x00000018U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_wdata 
                        = __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__9__Vfuncout;
                } else {
                    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state = 0U;
                }
            }
        } else if ((((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid) 
                     & (~ (IData)(vlSelfRef.halted))) 
                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dc_req_r = 1U;
            if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_we) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_we = 1U;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_lock;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_addr;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_wdata 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_wdata;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_be 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_be;
                __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state = 2U;
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_we = 0U;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_lock;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_addr;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_wdata 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_wdata;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_be 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_be;
                __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state = 1U;
            }
        }
        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_reg_idx_full 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_reg_idx_full;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_imm32 
                    = ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_ext_words))
                        ? (((- (IData)((1U & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext
                                              [vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words] 
                                              >> 0x0000000fU)))) 
                            << 0x00000010U) | vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext
                           [vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words])
                        : (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext
                                    [vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words]) 
                            << 0x00000010U) | vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext
                           [(3U & ((IData)(1U) + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words)))]));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_sp 
                    = (((0x0fU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_widx)) 
                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_we))
                        ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_wdata
                        : (((0x0fU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx)) 
                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we))
                            ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data
                            : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[15U]));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_cc;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_predicted_taken 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_predicted_taken;
                if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_dispatch_w) {
                    __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v0 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ext_combo[0U];
                    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v0 = 1U;
                    __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v1 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ext_combo[1U];
                    __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v2 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ext_combo[2U];
                    __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v3 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ext_combo[3U];
                    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_pc 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_predicted_taken 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__predict_taken_w;
                }
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb 
                    = ((((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_rb_idx) 
                         == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_widx)) 
                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_we))
                        ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_wdata
                        : ((((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_rb_idx) 
                             == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx)) 
                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we))
                            ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data
                            : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs
                           [vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_rb_idx]));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_imm32 
                    = ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words))
                        ? vlSelfRef.__VdfgRegularize_hebeb780c_0_5
                        : (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[0U]) 
                            << 0x00000010U) | vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[1U]));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_ra 
                    = ((((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_src_base_idx) 
                         == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_widx)) 
                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_we))
                        ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_wdata
                        : ((((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_src_base_idx) 
                             == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx)) 
                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we))
                            ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data
                            : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs
                           [vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_src_base_idx]));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_imm_raw 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_imm;
            }
            if ((1U & (~ (IData)(vlSelfRef.halted)))) {
                if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching) {
                    if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_op 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w;
                    } else if ((4U >= (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot))) {
                        __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v0 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w;
                        __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v0 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot) 
                                     - (IData)(1U)));
                        __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v0 = 1U;
                    }
                }
            }
        }
        if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot = 0U;
            if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy) 
                 & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__bus_resp_now)))) {
                __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain = 1U;
            }
            __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy = 0U;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_words_done = 0U;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc 
                = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stop_now)
                    ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_pc
                    : ((0x0cU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))
                        ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c)
                            ? ((IData)(2U) + (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_pc 
                                              + vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_branch_imm))
                            : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_pc_next)
                        : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_target_c));
        } else {
            if (vlSelfRef.halted) {
                __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy = 0U;
            } else {
                if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain) 
                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__bus_resp_now))) {
                    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain = 0U;
                }
                if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__would_be_complete) {
                    if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall) {
                        __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy = 0U;
                    } else {
                        __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy = 1U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_target;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_addr 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_target;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot = 0U;
                    }
                } else if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching) {
                    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy = 1U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr 
                        = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc 
                           + VL_SHIFTL_III(32,32,32, (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__new_words_done), 1U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_addr 
                        = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc 
                           + VL_SHIFTL_III(32,32,32, (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__new_words_done), 1U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__new_words_done;
                } else if ((1U & (((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy)) 
                                   & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall))) 
                                  & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain))))) {
                    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy = 1U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr 
                        = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc 
                           + VL_SHIFTL_III(32,32,32, (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_words_done), 1U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_addr 
                        = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc 
                           + VL_SHIFTL_III(32,32,32, (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_words_done), 1U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_words_done;
                }
            }
            if ((1U & (~ (IData)(vlSelfRef.halted)))) {
                if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__would_be_complete) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_words_done 
                        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall)
                            ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__new_words_done)
                            : 0U);
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_target;
                    }
                } else if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_words_done 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__new_words_done;
                }
            }
        }
        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_pc 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_pc;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_branch_imm 
                    = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_needs_disp_word)
                        ? vlSelfRef.__VdfgRegularize_hebeb780c_0_5
                        : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_imm);
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_pc_next 
                    = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_pc 
                       + (0x0000000eU & (((IData)(1U) 
                                          + ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_ext_words) 
                                             + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words))) 
                                         << 1U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind;
            }
        }
        if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid = 0U;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_valid = 0U;
        } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall)))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_valid;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_valid 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_dispatch_w;
        }
    } else {
        __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v2 = 1U;
        __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v4 = 1U;
        vlSelfRef.retired_flat = 0U;
        __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_pc = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_reg_idx_full = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_imm32 = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_sp = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_predicted_taken = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_imm32 = 0U;
        vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_idx_q = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_ra = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_op = 0U;
        __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v1 = 1U;
        vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_pending = 0U;
        vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_holder = 0U;
        vlSelfRef.m68k_top__DOT__u_bus__DOT__rrobin = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_predicted_taken = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__state = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__bus_req_r = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_we = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_lock = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_addr = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_wdata = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_be = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__state = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__bus_req_r = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_we = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_lock = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_addr = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_wdata = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_be = 0U;
        __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_addr = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr = 0U;
        __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain = 0U;
        __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_tas_word = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dc_req_r = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_we = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_wdata = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_be = 0x0fU;
        __Vdly__halted = 0U;
        vlSelfRef.halt_code_flat = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_words_done = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc = 0x00000400U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_imm_raw = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_pc = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_branch_imm = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_pc_next = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_valid = 0U;
    }
    vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_port_q 
        = ((IData)(vlSelfRef.rst_n) && (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner));
    vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_valid_q 
        = ((IData)(vlSelfRef.rst_n) && (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid));
    if (__VdlySet__m68k_top__DOT__u_bus__DOT__mem__v0) {
        vlSelfRef.m68k_top__DOT__u_bus__DOT__mem[__VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v0] 
            = ((0x00ffffffU & vlSelfRef.m68k_top__DOT__u_bus__DOT__mem
                [__VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v0]) 
               | ((IData)(__VdlyVal__m68k_top__DOT__u_bus__DOT__mem__v0) 
                  << 0x00000018U));
    }
    if (__VdlySet__m68k_top__DOT__u_bus__DOT__mem__v1) {
        vlSelfRef.m68k_top__DOT__u_bus__DOT__mem[__VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v1] 
            = ((0xff00ffffU & vlSelfRef.m68k_top__DOT__u_bus__DOT__mem
                [__VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v1]) 
               | ((IData)(__VdlyVal__m68k_top__DOT__u_bus__DOT__mem__v1) 
                  << 0x00000010U));
    }
    if (__VdlySet__m68k_top__DOT__u_bus__DOT__mem__v2) {
        vlSelfRef.m68k_top__DOT__u_bus__DOT__mem[__VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v2] 
            = ((0xffff00ffU & vlSelfRef.m68k_top__DOT__u_bus__DOT__mem
                [__VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v2]) 
               | ((IData)(__VdlyVal__m68k_top__DOT__u_bus__DOT__mem__v2) 
                  << 8U));
    }
    if (__VdlySet__m68k_top__DOT__u_bus__DOT__mem__v3) {
        vlSelfRef.m68k_top__DOT__u_bus__DOT__mem[__VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v3] 
            = ((0xffffff00U & vlSelfRef.m68k_top__DOT__u_bus__DOT__mem
                [__VdlyDim0__m68k_top__DOT__u_bus__DOT__mem__v3]) 
               | (IData)(__VdlyVal__m68k_top__DOT__u_bus__DOT__mem__v3));
    }
    if (__VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v0) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[0U] 
            = __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v0;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[1U] 
            = __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v1;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[2U] 
            = __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v2;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[3U] 
            = __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v3;
    }
    if (__VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v4) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[0U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[1U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[2U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[3U] = 0U;
    }
    if (__VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[__VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0] 
            = __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0;
    }
    if (__VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[__VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1] 
            = __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1;
    }
    if (__VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v2) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[0U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[1U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[2U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[3U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[4U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[5U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[6U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[7U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[8U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[9U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[10U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[11U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[12U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[13U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[14U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[15U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[15U] = 0x00004000U;
    }
    if (__VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v0) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext[__VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v0] 
            = __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v0;
    }
    if (__VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v1) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext[0U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext[1U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext[2U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext[3U] = 0U;
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_cc = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                 >> 3U));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                 >> 6U));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                 >> 9U));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                 >> 9U));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_ea = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_next = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_update = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem = 0U;
    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))) {
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))) {
                if (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg)) 
                     || (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg)))) {
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_ea 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_imm32;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem = 1U;
                }
            }
        } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode)))) {
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_ea 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb 
                   - (IData)(4U));
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem = 1U;
        }
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode)))) {
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_next 
                    = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb 
                       - (IData)(4U));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_update = 1U;
            }
        }
    } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))) {
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_ea 
            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb;
        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))) {
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_next 
                = ((IData)(4U) + vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb);
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_update = 1U;
        }
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem = 1U;
    }
    m68k_top__DOT__p_resp_data = vlSelfRef.m68k_top__DOT__u_bus__DOT__mem
        [vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_idx_q];
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_next = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_ea = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_update = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem = 0U;
    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode)))) {
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_next 
                    = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_ra 
                       - (IData)(4U));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_update = 1U;
            }
        }
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
                if (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg)) 
                     || (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg)))) {
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_ea 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_imm32;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem = 1U;
                }
                if ((1U & (~ ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg)) 
                              || (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg)))))) {
                    if ((4U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg))) {
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_imm32;
                    }
                }
            }
        } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode)))) {
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_ea 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_ra 
                   - (IData)(4U));
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem = 1U;
        }
    } else {
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_next 
                    = ((IData)(4U) + vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_ra);
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_update = 1U;
            }
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_ea 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_ra;
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem = 1U;
        }
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode) 
                      >> 1U)))) {
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_ra;
        }
    }
    if ((0x4e71U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
        if ((0x4e75U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
            if ((0x4e72U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x009dU != (0x000001ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                >> 7U)))) {
                    if ((1U & (~ (IData)((0x41c0U == 
                                          (0xf1c0U 
                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))))) {
                        if ((0x012bU != (0x000003ffU 
                                         & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                            >> 6U)))) {
                            if ((1U & (~ (IData)((0x7000U 
                                                  == 
                                                  (0xf100U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))))) {
                                if ((2U != (0x0000000fU 
                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                               >> 0x0cU)))) {
                                    if ((1U & (~ (IData)(
                                                         (0x5080U 
                                                          == 
                                                          (0xf0c0U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))))) {
                                        if ((6U == 
                                             (0x0000000fU 
                                              & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 0x0cU)))) {
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_cc 
                                                = (0x0000000fU 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                      >> 8U));
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if ((IData)((0x41c0U == (0xf1c0U 
                                             & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                    } else if ((0x012bU == (0x000003ffU 
                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                               >> 6U)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 7U;
                    } else if ((IData)((0x7000U == 
                                        (0xf100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                    } else if ((2U == (0x0000000fU 
                                       & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                          >> 0x0cU)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                    } else if ((IData)((0x5080U == 
                                        (0xf0c0U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                            = ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                ? 2U : 1U);
                    } else if ((6U != (0x0000000fU 
                                       & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                          >> 0x0cU)))) {
                        if ((IData)(((0x0080U == (0x00c0U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) 
                                     & (((((0x0dU == 
                                            (0x0000000fU 
                                             & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                >> 0x0cU))) 
                                           | (9U == 
                                              (0x0000000fU 
                                               & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                  >> 0x0cU)))) 
                                          | (0x0cU 
                                             == (0x0000000fU 
                                                 & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                    >> 0x0cU)))) 
                                         | (8U == (0x0000000fU 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                      >> 0x0cU)))) 
                                        | (0x0bU == 
                                           (0x0000000fU 
                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                               >> 0x0cU))))))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                                = ((0x00008000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                    ? ((0x00004000U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                        ? ((0x00002000U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                            ? 0U : 
                                           ((0x00001000U 
                                             & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                             ? 1U : 3U))
                                        : ((0x00002000U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                            ? ((0x00001000U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (0x00000100U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 5U
                                                    : 6U)
                                                : 0U)
                                            : ((0x00001000U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                ? 2U
                                                : 4U)))
                                    : 0U);
                        }
                    }
                }
                if ((0x009dU == (0x000001ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                >> 7U)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                } else if ((1U & (~ (IData)((0x41c0U 
                                             == (0xf1c0U 
                                                 & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))))) {
                    if ((0x012bU == (0x000003ffU & 
                                     ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                      >> 6U)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                    } else if ((1U & (~ (IData)((0x7000U 
                                                 == 
                                                 (0xf100U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))))) {
                        if ((2U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                   >> 0x0cU)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                        } else if ((IData)((0x5080U 
                                            == (0xf0c0U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                        } else if ((6U != (0x0000000fU 
                                           & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                              >> 0x0cU)))) {
                            if ((IData)(((0x0080U == 
                                          (0x00c0U 
                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) 
                                         & (((((0x0dU 
                                                == 
                                                (0x0000000fU 
                                                 & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                    >> 0x0cU))) 
                                               | (9U 
                                                  == 
                                                  (0x0000000fU 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                      >> 0x0cU)))) 
                                              | (0x0cU 
                                                 == 
                                                 (0x0000000fU 
                                                  & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                     >> 0x0cU)))) 
                                             | (8U 
                                                == 
                                                (0x0000000fU 
                                                 & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                    >> 0x0cU)))) 
                                            | (0x0bU 
                                               == (0x0000000fU 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                      >> 0x0cU))))))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_src_base_idx 
        = ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode))
            ? ((- (IData)((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode))))) 
               & ((8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg)) 
                  & (- (IData)((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode) 
                                         >> 1U)))))))
            : (((0U != (3U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode))) 
                << 3U) | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg)));
    if ((0x4e71U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
        if ((0x4e75U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
            if ((0x4e72U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x009dU != (0x000001ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                >> 7U)))) {
                    if ((1U & (~ (IData)((0x41c0U == 
                                          (0xf1c0U 
                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))))) {
                        if ((0x012bU != (0x000003ffU 
                                         & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                            >> 6U)))) {
                            if ((1U & (~ (IData)((0x7000U 
                                                  == 
                                                  (0xf100U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))))) {
                                if ((2U == (0x0000000fU 
                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                               >> 0x0cU)))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 6U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 9U));
                                }
                            }
                        }
                    }
                    if ((IData)((0x41c0U == (0xf1c0U 
                                             & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    } else if ((0x012bU != (0x000003ffU 
                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                               >> 6U)))) {
                        if ((IData)((0x7000U == (0xf100U 
                                                 & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        } else if ((2U != (0x0000000fU 
                                           & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                              >> 0x0cU)))) {
                            if ((1U & (~ (IData)((0x5080U 
                                                  == 
                                                  (0xf0c0U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))))) {
                                if ((6U != (0x0000000fU 
                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                               >> 0x0cU)))) {
                                    if ((IData)(((0x0080U 
                                                  == 
                                                  (0x00c0U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) 
                                                 & (((((0x0dU 
                                                        == 
                                                        (0x0000000fU 
                                                         & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                            >> 0x0cU))) 
                                                       | (9U 
                                                          == 
                                                          (0x0000000fU 
                                                           & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                              >> 0x0cU)))) 
                                                      | (0x0cU 
                                                         == 
                                                         (0x0000000fU 
                                                          & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                             >> 0x0cU)))) 
                                                     | (8U 
                                                        == 
                                                        (0x0000000fU 
                                                         & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                            >> 0x0cU)))) 
                                                    | (0x0bU 
                                                       == 
                                                       (0x0000000fU 
                                                        & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                           >> 0x0cU))))))) {
                                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                            = (7U & 
                                               ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                >> 9U));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_reg_idx_full 
        = (((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a) 
            << 3U) | (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx));
    vlSelfRef.m68k_top__DOT__p_resp_valid = 0U;
    if (vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_valid_q) {
        vlSelfRef.m68k_top__DOT__p_resp_valid = ((IData)(vlSelfRef.m68k_top__DOT__p_resp_valid) 
                                                 | (3U 
                                                    & ((IData)(1U) 
                                                       << (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_port_q))));
    }
    vlSelfRef.m68k_top__DOT__u_bus__DOT__wdata[0U] 
        = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_wdata;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__be[0U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_be;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__addr[0U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_addr;
    vlSelfRef.m68k_top__DOT__p_we = (((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_we) 
                                      << 1U) | (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_we));
    vlSelfRef.m68k_top__DOT__u_bus__DOT__wdata[1U] 
        = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_wdata;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__be[1U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_be;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__addr[1U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_addr;
    m68k_top__DOT__p_req = ((((~ ((IData)(vlSelfRef.m68k_top__DOT__p_resp_valid) 
                                  >> 1U)) & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__bus_req_r)) 
                             << 1U) | ((~ (IData)(vlSelfRef.m68k_top__DOT__p_resp_valid)) 
                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__bus_req_r)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain 
        = __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy 
        = __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__bus_resp_now 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__state) 
           & (IData)(vlSelfRef.m68k_top__DOT__p_resp_valid));
    vlSelfRef.m68k_top__DOT__u_bus__DOT__winner = 0U;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid = 0U;
    m68k_top__DOT__u_bus__DOT__cand = 0U;
    m68k_top__DOT__u_bus__DOT__cand_ext = 0U;
    if (vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_pending) {
        vlSelfRef.m68k_top__DOT__u_bus__DOT__winner 
            = vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_holder;
        vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid 
            = (1U & ((IData)(m68k_top__DOT__p_req) 
                     >> (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_holder)));
    } else {
        m68k_top__DOT__u_bus__DOT__cand_ext = vlSelfRef.m68k_top__DOT__u_bus__DOT__rrobin;
        m68k_top__DOT__u_bus__DOT__cand = (1U & (IData)(m68k_top__DOT__u_bus__DOT__cand_ext));
        m68k_top__DOT__u_bus__DOT__cand_ext = (3U & 
                                               ((IData)(1U) 
                                                + (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__rrobin)));
        if ((1U & ((~ (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid)) 
                   & ((IData)(m68k_top__DOT__p_req) 
                      >> (IData)(m68k_top__DOT__u_bus__DOT__cand))))) {
            vlSelfRef.m68k_top__DOT__u_bus__DOT__winner 
                = m68k_top__DOT__u_bus__DOT__cand;
            vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid = 1U;
        }
        m68k_top__DOT__u_bus__DOT__cand = (1U & (IData)(m68k_top__DOT__u_bus__DOT__cand_ext));
        if ((1U & ((~ (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid)) 
                   & ((IData)(m68k_top__DOT__p_req) 
                      >> (IData)(m68k_top__DOT__u_bus__DOT__cand))))) {
            vlSelfRef.m68k_top__DOT__u_bus__DOT__winner 
                = m68k_top__DOT__u_bus__DOT__cand;
            vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid = 1U;
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__bus_resp_now 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__state) 
           & ((IData)(vlSelfRef.m68k_top__DOT__p_resp_valid) 
              >> 1U));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state 
        = __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_rdata 
        = (m68k_top__DOT__p_resp_data & (- (IData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__bus_resp_now))));
    vlSelfRef.m68k_top__DOT__u_bus__DOT__mem_idx = 
        (0x00003fffU & (vlSelfRef.m68k_top__DOT__u_bus__DOT__addr
                        [vlSelfRef.m68k_top__DOT__u_bus__DOT__winner] 
                        >> 2U));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_rdata 
        = (m68k_top__DOT__p_resp_data & (- (IData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__bus_resp_now))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w 
        = (0x0000ffffU & ((2U & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_addr)
                           ? m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_rdata
                           : (m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_rdata 
                              >> 0x00000010U)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy) 
           & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain)) 
              & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__bus_resp_now)));
    vlSelfRef.halted = __Vdly__halted;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ext_combo[1U] 
        = (((2U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot)) 
            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching))
            ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w)
            : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext[1U]);
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ext_combo[2U] 
        = (((3U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot)) 
            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching))
            ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w)
            : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext[2U]);
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ext_combo[3U] 
        = (((4U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot)) 
            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching))
            ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w)
            : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext[3U]);
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__new_words_done 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_words_done) 
                 + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching)));
    __VdfgRegularize_hebeb780c_0_1 = (((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot)) 
                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching))
                                       ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w)
                                       : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext[0U]);
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op 
        = (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot)) 
            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching))
            ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w)
            : (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_op));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ext_combo[0U] 
        = __VdfgRegularize_hebeb780c_0_1;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_cc = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_needs_disp_word = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0U;
    if ((0x4e71U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 1U;
    } else if ((0x4e75U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 2U;
    } else if ((0x4e72U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 3U;
    } else if ((0x009dU == (0x000001ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                           >> 7U)))) {
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
            = ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                ? 4U : 5U);
    } else if ((IData)((0x41c0U == (0xf1c0U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))))) {
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 6U;
    } else if ((0x012bU == (0x000003ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                           >> 6U)))) {
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0bU;
    } else if ((IData)((0x7000U == (0xf100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))))) {
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 7U;
    } else if ((2U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                      >> 0x0cU)))) {
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 8U;
    } else if ((IData)((0x5080U == (0xf0c0U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))))) {
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 9U;
    } else if ((6U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                      >> 0x0cU)))) {
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0cU;
    } else if ((IData)(((0x0080U == (0x00c0U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) 
                        & (((((0x0dU == (0x0000000fU 
                                         & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                            >> 0x0cU))) 
                              | (9U == (0x0000000fU 
                                        & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                           >> 0x0cU)))) 
                             | (0x0cU == (0x0000000fU 
                                          & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 0x0cU)))) 
                            | (8U == (0x0000000fU & 
                                      ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                       >> 0x0cU)))) 
                           | (0x0bU == (0x0000000fU 
                                        & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                           >> 0x0cU))))))) {
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
    }
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_imm = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_movq 
        = (((- (IData)((1U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                              >> 7U)))) << 8U) | (0x000000ffU 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_addq 
        = ((0U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 9U))) ? 8U : (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                >> 9U)));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_disp 
        = (((- (IData)((1U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                              >> 7U)))) << 8U) | (0x000000ffU 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 0U;
    if ((0x4e71U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
        if ((0x4e75U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            if ((0x4e72U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((0x009dU != (0x000001ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                >> 7U)))) {
                    if ((1U & (~ (IData)((0x41c0U == 
                                          (0xf1c0U 
                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))))))) {
                        if ((0x012bU != (0x000003ffU 
                                         & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                            >> 6U)))) {
                            if ((1U & (~ (IData)((0x7000U 
                                                  == 
                                                  (0xf100U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))))))) {
                                if ((2U != (0x0000000fU 
                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                               >> 0x0cU)))) {
                                    if ((1U & (~ (IData)(
                                                         (0x5080U 
                                                          == 
                                                          (0xf0c0U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))))))) {
                                        if ((6U == 
                                             (0x0000000fU 
                                              & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 0x0cU)))) {
                                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_cc 
                                                = (0x0000000fU 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                      >> 8U));
                                            if ((0U 
                                                 == 
                                                 (0x000000ffU 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))) {
                                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_needs_disp_word = 1U;
                                            }
                                        }
                                    }
                                }
                                if ((2U == (0x0000000fU 
                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                               >> 0x0cU)))) {
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__r 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 9U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__m 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 6U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__Vfuncout 
                                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__m))
                                            ? ((0U 
                                                == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__r))
                                                ? 1U
                                                : (
                                                   (1U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__r))
                                                    ? 2U
                                                    : 
                                                   ((4U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__r))
                                                     ? 2U
                                                     : 0U)))
                                            : 0U);
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__Vfuncout;
                                }
                            }
                            if ((IData)((0x7000U == 
                                         (0xf100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))))) {
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_imm 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_movq;
                            } else if ((2U != (0x0000000fU 
                                               & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                  >> 0x0cU)))) {
                                if ((IData)((0x5080U 
                                             == (0xf0c0U 
                                                 & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))))) {
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_imm 
                                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_addq;
                                } else if ((6U == (0x0000000fU 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                      >> 0x0cU)))) {
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_imm 
                                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_disp;
                                }
                            }
                        }
                    }
                }
            }
            if ((0x4e72U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
            } else if ((0x009dU == (0x000001ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                   >> 7U)))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__10__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__10__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__10__Vfuncout 
                    = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__10__m))
                        ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__10__r))
                            ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__10__r))
                                     ? 2U : ((4U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__10__r))
                                              ? 2U : 0U)))
                        : 0U);
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__10__Vfuncout;
            } else if ((IData)((0x41c0U == (0xf1c0U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__11__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__11__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__11__Vfuncout 
                    = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__11__m))
                        ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__11__r))
                            ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__11__r))
                                     ? 2U : ((4U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__11__r))
                                              ? 2U : 0U)))
                        : 0U);
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__11__Vfuncout;
            } else if ((0x012bU == (0x000003ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                   >> 6U)))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__Vfuncout 
                    = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__m))
                        ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__r))
                            ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__r))
                                     ? 2U : ((4U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__r))
                                              ? 2U : 0U)))
                        : 0U);
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__Vfuncout;
            } else if ((1U & (~ (IData)((0x7000U == 
                                         (0xf100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))))))) {
                if ((2U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                           >> 0x0cU)))) {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__Vfuncout 
                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__m))
                            ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__r))
                                ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__r))
                                         ? 2U : ((4U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__r))
                                                  ? 2U
                                                  : 0U)))
                            : 0U);
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__Vfuncout;
                } else if ((IData)((0x5080U == (0xf0c0U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))))) {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__Vfuncout 
                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__m))
                            ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__r))
                                ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__r))
                                         ? 2U : ((4U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__r))
                                                  ? 2U
                                                  : 0U)))
                            : 0U);
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__Vfuncout;
                } else if ((6U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                  >> 0x0cU)))) {
                    if ((0U == (0x000000ffU & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))) {
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                    }
                } else if ((IData)(((0x0080U == (0x00c0U 
                                                 & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) 
                                    & (((((0x0dU == 
                                           (0x0000000fU 
                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                               >> 0x0cU))) 
                                          | (9U == 
                                             (0x0000000fU 
                                              & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 0x0cU)))) 
                                         | (0x0cU == 
                                            (0x0000000fU 
                                             & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                >> 0x0cU)))) 
                                        | (8U == (0x0000000fU 
                                                  & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                     >> 0x0cU)))) 
                                       | (0x0bU == 
                                          (0x0000000fU 
                                           & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                              >> 0x0cU))))))) {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__16__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__16__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__16__Vfuncout 
                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__16__m))
                            ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__16__r))
                                ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__16__r))
                                         ? 2U : ((4U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__16__r))
                                                  ? 2U
                                                  : 0U)))
                            : 0U);
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__16__Vfuncout;
                }
            }
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_needs_disp_word = 0U;
    vlSelfRef.__VdfgRegularize_hebeb780c_0_5 = (((- (IData)(
                                                            (1U 
                                                             & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[0U] 
                                                                >> 0x0000000fU)))) 
                                                 << 0x00000010U) 
                                                | vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[0U]);
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_imm = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_movq 
        = (((- (IData)((1U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                              >> 7U)))) << 8U) | (0x000000ffU 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op)));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_addq 
        = ((0U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                         >> 9U))) ? 8U : (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                >> 9U)));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_disp 
        = (((- (IData)((1U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                              >> 7U)))) << 8U) | (0x000000ffU 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_pc 
        = __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_pc;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_ext_words = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words = 0U;
    if ((0x4e71U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
        if ((0x4e75U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
            if ((0x4e72U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x009dU != (0x000001ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                >> 7U)))) {
                    if ((1U & (~ (IData)((0x41c0U == 
                                          (0xf1c0U 
                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))))) {
                        if ((0x012bU != (0x000003ffU 
                                         & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                            >> 6U)))) {
                            if ((1U & (~ (IData)((0x7000U 
                                                  == 
                                                  (0xf100U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))))) {
                                if ((2U != (0x0000000fU 
                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                               >> 0x0cU)))) {
                                    if ((1U & (~ (IData)(
                                                         (0x5080U 
                                                          == 
                                                          (0xf0c0U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))))) {
                                        if ((6U == 
                                             (0x0000000fU 
                                              & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 0x0cU)))) {
                                            if ((0U 
                                                 == 
                                                 (0x000000ffU 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op)))) {
                                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_needs_disp_word = 1U;
                                            }
                                        }
                                    }
                                }
                                if ((2U == (0x0000000fU 
                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                               >> 0x0cU)))) {
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__r 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 9U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__m 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 6U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__Vfuncout 
                                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__m))
                                            ? ((0U 
                                                == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__r))
                                                ? 1U
                                                : (
                                                   (1U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__r))
                                                    ? 2U
                                                    : 
                                                   ((4U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__r))
                                                     ? 2U
                                                     : 0U)))
                                            : 0U);
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_ext_words 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__Vfuncout;
                                }
                            }
                            if ((IData)((0x7000U == 
                                         (0xf100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_imm 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_movq;
                            } else if ((2U != (0x0000000fU 
                                               & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                  >> 0x0cU)))) {
                                if ((IData)((0x5080U 
                                             == (0xf0c0U 
                                                 & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_imm 
                                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_addq;
                                } else if ((6U == (0x0000000fU 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                      >> 0x0cU)))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_imm 
                                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_disp;
                                }
                            }
                        }
                    }
                }
            }
            if ((0x4e72U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words = 1U;
            } else if ((0x009dU == (0x000001ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                   >> 7U)))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__17__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__17__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__17__Vfuncout 
                    = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__17__m))
                        ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__17__r))
                            ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__17__r))
                                     ? 2U : ((4U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__17__r))
                                              ? 2U : 0U)))
                        : 0U);
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__17__Vfuncout;
            } else if ((IData)((0x41c0U == (0xf1c0U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__18__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__18__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__18__Vfuncout 
                    = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__18__m))
                        ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__18__r))
                            ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__18__r))
                                     ? 2U : ((4U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__18__r))
                                              ? 2U : 0U)))
                        : 0U);
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__18__Vfuncout;
            } else if ((0x012bU == (0x000003ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                   >> 6U)))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__19__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__19__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__19__Vfuncout 
                    = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__19__m))
                        ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__19__r))
                            ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__19__r))
                                     ? 2U : ((4U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__19__r))
                                              ? 2U : 0U)))
                        : 0U);
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__19__Vfuncout;
            } else if ((1U & (~ (IData)((0x7000U == 
                                         (0xf100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))))) {
                if ((2U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                           >> 0x0cU)))) {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__Vfuncout 
                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__m))
                            ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__r))
                                ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__r))
                                         ? 2U : ((4U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__r))
                                                  ? 2U
                                                  : 0U)))
                            : 0U);
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__Vfuncout;
                } else if ((IData)((0x5080U == (0xf0c0U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__Vfuncout 
                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__m))
                            ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__r))
                                ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__r))
                                         ? 2U : ((4U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__r))
                                                  ? 2U
                                                  : 0U)))
                            : 0U);
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__Vfuncout;
                } else if ((6U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                  >> 0x0cU)))) {
                    if ((0U == (0x000000ffU & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words = 1U;
                    }
                } else if ((IData)(((0x0080U == (0x00c0U 
                                                 & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) 
                                    & (((((0x0dU == 
                                           (0x0000000fU 
                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                               >> 0x0cU))) 
                                          | (9U == 
                                             (0x0000000fU 
                                              & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 0x0cU)))) 
                                         | (0x0cU == 
                                            (0x0000000fU 
                                             & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                >> 0x0cU)))) 
                                        | (8U == (0x0000000fU 
                                                  & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                     >> 0x0cU)))) 
                                       | (0x0bU == 
                                          (0x0000000fU 
                                           & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                              >> 0x0cU))))))) {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__23__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__23__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__23__Vfuncout 
                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__23__m))
                            ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__23__r))
                                ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__23__r))
                                         ? 2U : ((4U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__23__r))
                                                  ? 2U
                                                  : 0U)))
                            : 0U);
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__23__Vfuncout;
                }
            }
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind = 0U;
    if ((0x4e71U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind = 1U;
    } else if ((0x4e75U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind = 2U;
    } else if ((0x4e72U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind = 3U;
    } else if ((0x009dU == (0x000001ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                           >> 7U)))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind 
            = ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                ? 4U : 5U);
    } else if ((IData)((0x41c0U == (0xf1c0U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind = 6U;
    } else if ((0x012bU == (0x000003ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                           >> 6U)))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind = 0x0bU;
    } else if ((IData)((0x7000U == (0xf100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind = 7U;
    } else if ((2U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                      >> 0x0cU)))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind = 8U;
    } else if ((IData)((0x5080U == (0xf0c0U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind = 9U;
    } else if ((6U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                      >> 0x0cU)))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind = 0x0cU;
    } else if ((IData)(((0x0080U == (0x00c0U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) 
                        & (((((0x0dU == (0x0000000fU 
                                         & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                            >> 0x0cU))) 
                              | (9U == (0x0000000fU 
                                        & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                           >> 0x0cU)))) 
                             | (0x0cU == (0x0000000fU 
                                          & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 0x0cU)))) 
                            | (8U == (0x0000000fU & 
                                      ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                       >> 0x0cU)))) 
                           | (0x0bU == (0x0000000fU 
                                        & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                           >> 0x0cU))))))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind = 0x0aU;
    }
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_bcc_uncond 
        = ((0x0cU == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind)) 
           & ((0U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_cc)) 
              | (1U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_cc))));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__disp32 
        = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_needs_disp_word)
            ? (((- (IData)((1U & ((IData)(__VdfgRegularize_hebeb780c_0_1) 
                                  >> 0x0000000fU)))) 
                << 0x00000010U) | (IData)(__VdfgRegularize_hebeb780c_0_1))
            : m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_imm);
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__total_words_eff 
        = (7U & ((IData)(1U) + ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words) 
                                + (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_rb_idx 
        = ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind))
            ? (((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind))
                 ? ((- (IData)((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind))))) 
                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_reg_idx_full))
                 : (((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode))
                      ? ((- (IData)((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode))))) 
                         & ((8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg)) 
                            & (- (IData)((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode) 
                                                   >> 1U)))))))
                      : (((0U != (3U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode))) 
                          << 3U) | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg))) 
                    & (- (IData)((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind))))))) 
               & (- (IData)((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind) 
                                      >> 2U)))))) : 
           ((- (IData)((3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind) 
                                     >> 1U))))) & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_reg_idx_full)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_lock = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_we = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_be = 0x0fU;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_wdata = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_addr = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_target_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_we_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_idx_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stop_now 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid) 
           & ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)) 
              & ((~ (IData)(vlSelfRef.halted)) & (3U 
                                                  == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__predict_taken_w 
        = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_bcc_uncond) 
           | ((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__disp32 
               >> 0x0000001fU) & ((~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_bcc_uncond)) 
                                  & (0x0cU == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind)))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__would_be_complete 
        = (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__new_words_done) 
            >= (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__total_words_eff)) 
           & (0U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__new_words_done)));
    if ((1U & (~ ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid)) 
                  | (IData)(vlSelfRef.halted))))) {
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)))) {
                if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                  >> 2U)))) {
                        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_lock = 1U;
                            }
                        }
                    }
                    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                      >> 1U)))) {
                            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                                if ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_we = 1U;
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_be = 0x0fU;
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_wdata 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_pc_next;
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_addr 
                                        = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_sp 
                                           - (IData)(4U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem = 1U;
                                }
                            }
                        }
                    } else {
                        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_we = 0U;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_be = 0x0fU;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_addr 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_ea;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem = 1U;
                            } else if (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_we = 0U;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_be = 0x0fU;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_addr 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_ea;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem = 1U;
                            }
                        } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                            if (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_we = 0U;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_be = 0x0fU;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_addr 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_ea;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem = 1U;
                            } else if (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_we = 1U;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_be = 0x0fU;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_addr 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_ea;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem = 1U;
                            }
                        }
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                      >> 1U)))) {
                            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                                if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                    if (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_wdata 
                                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand;
                                    }
                                }
                            }
                        }
                    }
                } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                  >> 1U)))) {
                        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_we = 1U;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_be = 0x0fU;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_wdata 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_pc_next;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_addr 
                                = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_sp 
                                   - (IData)(4U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem = 1U;
                        }
                    }
                } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_we = 0U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_be = 0x0fU;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_addr 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_sp;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem = 1U;
                    }
                }
            }
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
                if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_we_c = 1U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_idx_c 
                        = ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))
                            ? (8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg))
                            : (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg));
                } else if ((0x0aU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((6U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op))) {
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_we_c = 1U;
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_idx_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_reg_idx_full;
                    }
                }
            } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                              >> 2U)))) {
                    if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                            if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                if ((6U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op))) {
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_we_c = 1U;
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_idx_c 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_reg_idx_full;
                                }
                            }
                        }
                    } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode)) 
                             | (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode)))) {
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_we_c = 1U;
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_idx_c 
                                = ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))
                                    ? (8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg))
                                    : (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg));
                        }
                    } else if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                        if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem)))) {
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_we_c = 1U;
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_idx_c 
                                = ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))
                                    ? (8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg))
                                    : (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg));
                        }
                    }
                }
            } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_we_c = 1U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_idx_c 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_reg_idx_full;
                }
            }
        }
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c = 1U;
            } else if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c = 1U;
            }
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)))) {
                if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_update) {
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c = 1U;
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx_c 
                            = (8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data_c 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_next;
                    }
                } else if ((5U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c = 1U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx_c = 0x0fU;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data_c 
                        = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_sp 
                           - (IData)(4U));
                } else if ((0x0cU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c = 1U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx_c = 0x0fU;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data_c 
                        = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_sp 
                           - (IData)(4U));
                }
                if ((8U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((5U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_target_c 
                            = ((7U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))
                                ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_imm32
                                : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_ra);
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c = 1U;
                    } else if ((0x0cU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_target_c 
                            = ((IData)(2U) + (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_pc 
                                              + vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_branch_imm));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c = 1U;
                    }
                }
            }
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
            if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c 
                    = (0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode));
                if (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_update) {
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c = 1U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx_c 
                        = (8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data_c 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_next;
                }
            } else if ((0x0aU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c = 1U;
                if (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_update) {
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c = 1U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx_c 
                        = (8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data_c 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_next;
                }
            } else if ((2U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c = 1U;
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx_c = 0x0fU;
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data_c 
                    = ((IData)(4U) + vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_sp);
            }
            if ((8U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if ((0x0aU != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((2U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_target_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_rdata;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c = 1U;
                    }
                }
            }
        } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                          >> 2U)))) {
                if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                        if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c = 1U;
                        }
                    }
                } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c = 1U;
                } else if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                    if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c 
                            = (0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode));
                    }
                }
            }
            if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_target_c 
                            = ((IData)(2U) + (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_pc 
                                              + vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_branch_imm));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c 
                            = (1U & ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                      ? ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                          ? ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                              ? ((1U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                                  ? 
                                                 (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z) 
                                                   | ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n) 
                                                      & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v)))) 
                                                  | ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n)) 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v)))
                                                  : 
                                                 ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z)) 
                                                  & (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n) 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v)) 
                                                     | ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n)) 
                                                        & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v))))))
                                              : ((1U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                                  ? 
                                                 (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n) 
                                                   & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v))) 
                                                  | ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n)) 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v)))
                                                  : 
                                                 (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n) 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v)) 
                                                  | ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n)) 
                                                     & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v))))))
                                          : ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                              ? ((1U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                                  ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n)
                                                  : 
                                                 (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n)))
                                              : ((1U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                                  ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v)
                                                  : 
                                                 (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v)))))
                                      : ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                          ? ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                              ? ((1U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                                  ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z)
                                                  : 
                                                 (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z)))
                                              : ((1U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                                  ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c)
                                                  : 
                                                 (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c))))
                                          : ((1U & 
                                              (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc) 
                                                  >> 1U))) 
                                             || (1U 
                                                 & ((1U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                                     ? 
                                                    ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c) 
                                                     | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z))
                                                     : 
                                                    ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c)) 
                                                     & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z)))))))));
                        if ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c = 0U;
                        }
                    }
                }
            }
        } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
            if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c = 1U;
                }
            }
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                          >> 1U)))) {
                if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_target_c 
                        = ((7U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))
                            ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_imm32
                            : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_ra);
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c = 1U;
                }
            }
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled 
        = (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)) 
            & ((~ ((IData)(vlSelfRef.halted) | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem))) 
               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid))) 
           | ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid) 
              & ((~ (IData)(vlSelfRef.halted)) & ((
                                                   ((1U 
                                                     == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)) 
                                                    & (0x0bU 
                                                       != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) 
                                                   | ((2U 
                                                       == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)) 
                                                      | (3U 
                                                         == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)))) 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__bus_resp_now)))));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum = 0ULL;
    if ((1U & (~ ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid)) 
                  | (IData)(vlSelfRef.halted))))) {
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state) 
                      >> 1U)))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
                if ((8U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((0x0aU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op;
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb;
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_rdata;
                    }
                }
            } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                              >> 2U)))) {
                    if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op;
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb;
                            if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand;
                            }
                        }
                    } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op;
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand;
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_imm_raw;
                    }
                }
            }
        }
    }
    if ((8U & (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
        if ((4U & (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y = 0U;
        } else if ((2U & (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y = 0U;
        } else if ((1U & (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y = 0U;
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b;
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                = (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                   >> 0x1fU);
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                = (0U == m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b);
        }
    } else if ((4U & (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
        if ((2U & (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            if ((1U & (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                    = (0x00000080U | m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b);
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                    = (1U & (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                             >> 7U));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                    = (0U == (0x000000ffU & m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b));
            } else {
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                    = (0x00000001ffffffffULL & ((QData)((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a)) 
                                                - (QData)((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b))));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                    = ((6U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))
                        ? m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a
                        : (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                    = (1U & (IData)((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                     >> 0x1fU)));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                    = (0U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c 
                    = (1U & (IData)((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                     >> 0x20U)));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v 
                    = (((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
                         >> 0x1fU) != (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                                       >> 0x1fU)) & 
                       ((1U & (IData)((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                       >> 0x1fU))) 
                        != (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
                            >> 0x1fU)));
            }
        } else if ((1U & (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
                   ^ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b);
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                   >> 0x1fU);
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y);
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
                   | m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b);
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                   >> 0x1fU);
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y);
        }
    } else if ((2U & (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
        if ((1U & (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
                   & m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b);
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                   >> 0x1fU);
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y);
        } else {
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                = (0x00000001ffffffffULL & ((QData)((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a)) 
                                            - (QData)((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b))));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = ((6U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))
                    ? m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a
                    : (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum));
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                = (1U & (IData)((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                 >> 0x1fU)));
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                = (0U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum));
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c 
                = (1U & (IData)((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                 >> 0x20U)));
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v 
                = (((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
                     >> 0x1fU) != (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                                   >> 0x1fU)) & ((1U 
                                                  & (IData)(
                                                            (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                                             >> 0x1fU))) 
                                                 != 
                                                 (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
                                                  >> 0x1fU)));
        }
    } else if ((1U & (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
            = (0x00000001ffffffffULL & ((QData)((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a)) 
                                        + (QData)((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b))));
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
            = (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum);
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
            = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
               >> 0x1fU);
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
            = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y);
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c 
            = (1U & (IData)((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                             >> 0x20U)));
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v 
            = (((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
                 >> 0x1fU) == (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                               >> 0x1fU)) & ((vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                                              >> 0x1fU) 
                                             != (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
                                                 >> 0x1fU)));
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b;
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
            = (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
               >> 0x1fU);
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
            = (0U == m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b);
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_target 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__predict_taken_w)
            ? ((IData)(2U) + (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc 
                              + m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__disp32))
            : (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc 
               + ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__total_words_eff) 
                  << 1U)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_we 
        = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_we_c) 
           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_widx 
        = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_idx_c) 
           & (- (IData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we 
        = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c) 
           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx 
        = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx_c) 
           & (- (IData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data 
        = (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data_c 
           & (- (IData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall 
        = ((~ ((IData)(vlSelfRef.halted) | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled))) 
           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid 
        = (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled) 
            & ((0x0cU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)) 
               & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_predicted_taken) 
                  != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c)))) 
           | (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled) 
               & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c) 
                  & (0x0cU != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) 
              | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stop_now)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c = 0U;
    if ((1U & (~ ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid)) 
                  | (IData)(vlSelfRef.halted))))) {
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state) 
                      >> 1U)))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
                if ((8U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((0x0aU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v_c 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c_c 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c;
                    }
                }
                if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_rdata;
                } else if ((0x0aU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((6U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op))) {
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y;
                    }
                }
            } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                              >> 2U)))) {
                    if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                            if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v_c 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c_c 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c;
                                if ((6U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op))) {
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y;
                                }
                            }
                        }
                    } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v_c 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c_c 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c;
                        if (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode)) 
                             | (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode)))) {
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y;
                        }
                    } else if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                        if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem)))) {
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand;
                        }
                    }
                }
            } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
                        = ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))
                            ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_imm_raw
                            : ((7U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))
                                ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_imm32
                                : m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_ea));
                }
            }
        }
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__5__pos 
                    = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr);
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__5__w 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_tas_word;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT____VlemCall_0__byte_at 
                    = (0x000000ffU & ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__5__pos))
                                       ? ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__5__pos))
                                           ? vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__5__w
                                           : (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__5__w 
                                              >> 8U))
                                       : ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__5__pos))
                                           ? (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__5__w 
                                              >> 0x10U)
                                           : (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__5__w 
                                              >> 0x18U))));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                    = (1U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT____VlemCall_0__byte_at) 
                             >> 7U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__6__pos 
                    = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr);
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__6__w 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_tas_word;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT____VlemCall_1__byte_at 
                    = (0x000000ffU & ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__6__pos))
                                       ? ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__6__pos))
                                           ? vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__6__w
                                           : (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__6__w 
                                              >> 8U))
                                       : ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__6__pos))
                                           ? (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__6__w 
                                              >> 0x10U)
                                           : (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__6__w 
                                              >> 0x18U))));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                    = (0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT____VlemCall_1__byte_at));
            } else if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                    = (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand 
                       >> 0x1fU);
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                    = (0U == m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand);
            }
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
            if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                    = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_rdata 
                       >> 0x1fU);
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                    = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_rdata);
            } else if ((0x0aU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z;
            }
        } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                          >> 2U)))) {
                if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                        if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z;
                        }
                    }
                } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z;
                } else if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                    if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                            = (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand 
                               >> 0x1fU);
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                            = (0U == m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand);
                    }
                }
            }
        } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
            if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                        = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_imm_raw 
                           >> 0x1fU);
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                        = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_imm_raw);
                }
            }
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_dispatch_w 
        = ((~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall) 
               | ((IData)(vlSelfRef.halted) | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid)))) 
           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__would_be_complete));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_wdata 
        = (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
           & (- (IData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled))));
}

void Vm68k_top___024root___eval_nba(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___eval_nba\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((3ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vm68k_top___024root___nba_sequent__TOP__0(vlSelf);
    }
}

void Vm68k_top___024root___trigger_orInto__act_vec_vec(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___trigger_orInto__act_vec_vec\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((0U >= n));
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vm68k_top___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

bool Vm68k_top___024root___eval_phase__act(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___eval_phase__act\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vm68k_top___024root___eval_triggers_vec__act(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vm68k_top___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
    Vm68k_top___024root___trigger_orInto__act_vec_vec(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    return (0U);
}

void Vm68k_top___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vm68k_top___024root___eval_phase__nba(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___eval_phase__nba\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vm68k_top___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vm68k_top___024root___eval_nba(vlSelf);
        Vm68k_top___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

void Vm68k_top___024root___eval(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___eval\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vm68k_top___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("rtl/m68k_top.v", 6, "", "DIDNOTCONVERGE: NBA region did not converge after '--converge-limit' of 10000 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00002710U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vm68k_top___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("rtl/m68k_top.v", 6, "", "DIDNOTCONVERGE: Active region did not converge after '--converge-limit' of 10000 tries");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactPhaseResult = Vm68k_top___024root___eval_phase__act(vlSelf);
        } while (vlSelfRef.__VactPhaseResult);
        vlSelfRef.__VnbaPhaseResult = Vm68k_top___024root___eval_phase__nba(vlSelf);
    } while (vlSelfRef.__VnbaPhaseResult);
}

#ifdef VL_DEBUG
void Vm68k_top___024root___eval_debug_assertions(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___eval_debug_assertions\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.clk & 0xfeU)))) {
        Verilated::overWidthError("clk");
    }
    if (VL_UNLIKELY(((vlSelfRef.rst_n & 0xfeU)))) {
        Verilated::overWidthError("rst_n");
    }
}
#endif  // VL_DEBUG
