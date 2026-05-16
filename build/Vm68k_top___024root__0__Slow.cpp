// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vm68k_top.h for the primary calling header

#include "Vm68k_top__pch.h"

VL_ATTR_COLD void Vm68k_top___024root___eval_static(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___eval_static\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rst_n__0 = vlSelfRef.rst_n;
}

VL_ATTR_COLD void Vm68k_top___024root___eval_initial(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___eval_initial\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __Vinline__eval_initial__TOP_m68k_top__DOT__u_bus__DOT__mi;
    __Vinline__eval_initial__TOP_m68k_top__DOT__u_bus__DOT__mi = 0;
    // Body
    __Vinline__eval_initial__TOP_m68k_top__DOT__u_bus__DOT__mi = 0U;
    while (VL_GTS_III(32, 0x00004000U, __Vinline__eval_initial__TOP_m68k_top__DOT__u_bus__DOT__mi)) {
        vlSelfRef.m68k_top__DOT__u_bus__DOT__mem[(0x00003fffU 
                                                  & __Vinline__eval_initial__TOP_m68k_top__DOT__u_bus__DOT__mi)] = 0U;
        __Vinline__eval_initial__TOP_m68k_top__DOT__u_bus__DOT__mi 
            = ((IData)(1U) + __Vinline__eval_initial__TOP_m68k_top__DOT__u_bus__DOT__mi);
    }
    VL_READMEM_N(true, 32, 16384, 0, "program.hex"s
                 ,  &(vlSelfRef.m68k_top__DOT__u_bus__DOT__mem)
                 , 0, ~0ULL);
}

VL_ATTR_COLD void Vm68k_top___024root___eval_initial__TOP(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___eval_initial__TOP\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ m68k_top__DOT__u_bus__DOT__mi;
    m68k_top__DOT__u_bus__DOT__mi = 0;
    // Body
    m68k_top__DOT__u_bus__DOT__mi = 0U;
    while (VL_GTS_III(32, 0x00004000U, m68k_top__DOT__u_bus__DOT__mi)) {
        vlSelfRef.m68k_top__DOT__u_bus__DOT__mem[(0x00003fffU 
                                                  & m68k_top__DOT__u_bus__DOT__mi)] = 0U;
        m68k_top__DOT__u_bus__DOT__mi = ((IData)(1U) 
                                         + m68k_top__DOT__u_bus__DOT__mi);
    }
    VL_READMEM_N(true, 32, 16384, 0, "program.hex"s
                 ,  &(vlSelfRef.m68k_top__DOT__u_bus__DOT__mem)
                 , 0, ~0ULL);
}

VL_ATTR_COLD void Vm68k_top___024root___eval_final(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___eval_final\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vm68k_top___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vm68k_top___024root___eval_phase__stl(Vm68k_top___024root* vlSelf);

VL_ATTR_COLD void Vm68k_top___024root___eval_settle(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___eval_settle\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VstlIterCount;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vm68k_top___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
#endif
            VL_FATAL_MT("rtl/m68k_top.v", 6, "", "DIDNOTCONVERGE: Settle region did not converge after '--converge-limit' of 10000 tries");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        vlSelfRef.__VstlPhaseResult = Vm68k_top___024root___eval_phase__stl(vlSelf);
        vlSelfRef.__VstlFirstIteration = 0U;
    } while (vlSelfRef.__VstlPhaseResult);
}

VL_ATTR_COLD void Vm68k_top___024root___eval_triggers_vec__stl(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___eval_triggers_vec__stl\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VstlTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VstlTriggered[0U]) 
                                     | (IData)((IData)(vlSelfRef.__VstlFirstIteration)));
}

VL_ATTR_COLD bool Vm68k_top___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vm68k_top___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(Vm68k_top___024root___trigger_anySet__stl(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD bool Vm68k_top___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___trigger_anySet__stl\n"); );
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

VL_ATTR_COLD void Vm68k_top___024root___stl_sequent__TOP__0(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___stl_sequent__TOP__0\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*3:0*/ m68k_top__DOT__p_req;
    m68k_top__DOT__p_req = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_rdata;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_rdata = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_rdata;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_rdata = 0;
    CData/*2:0*/ m68k_top__DOT__u_bus__DOT__cand_ext;
    m68k_top__DOT__u_bus__DOT__cand_ext = 0;
    CData/*1:0*/ m68k_top__DOT__u_bus__DOT__cand;
    m68k_top__DOT__u_bus__DOT__cand = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__comb_read_hit;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__comb_read_hit = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__bus_resp_now;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__bus_resp_now = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__comb_read_hit;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__comb_read_hit = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__bus_resp_now;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__bus_resp_now = 0;
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
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__comb_read_hit;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__comb_read_hit = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__bus_resp_now;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__bus_resp_now = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__comb_read_hit;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__comb_read_hit = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__bus_resp_now;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__bus_resp_now = 0;
    CData/*3:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_kind;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_kind = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_imm;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_imm = 0;
    CData/*3:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_cc;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_cc = 0;
    CData/*1:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_src_ext_words;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_src_ext_words = 0;
    CData/*1:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_dst_ext_words;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_dst_ext_words = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_needs_disp_word;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_needs_disp_word = 0;
    CData/*2:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__total_words_eff;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__total_words_eff = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_bcc_uncond;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_bcc_uncond = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__disp32;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__disp32 = 0;
    CData/*2:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_ea;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_ea = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_next;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_next = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_update;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_update = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_ea;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_ea = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_an_next;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_an_next = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_an_update;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_an_update = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_is_mem;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_is_mem = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_we_c;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_we_c = 0;
    CData/*3:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_idx_c;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_idx_c = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_we_c;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_we_c = 0;
    CData/*3:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_idx_c;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_idx_c = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_data_c;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_data_c = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b = 0;
    CData/*3:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_movq;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_movq = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_addq;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_addq = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_disp;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_disp = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_movq;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_movq = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_addq;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_addq = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_disp;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_disp = 0;
    QData/*32:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum = 0;
    SData/*15:0*/ __VdfgRegularize_hebeb780c_0_2;
    __VdfgRegularize_hebeb780c_0_2 = 0;
    SData/*15:0*/ __VdfgRegularize_hebeb780c_0_3;
    __VdfgRegularize_hebeb780c_0_3 = 0;
    // Body
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_cc = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_needs_disp_word = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_cc = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_needs_disp_word = 0U;
    vlSelfRef.__VdfgRegularize_hebeb780c_0_10 = (((- (IData)(
                                                             (1U 
                                                              & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[0U] 
                                                                 >> 0x0000000fU)))) 
                                                  << 0x00000010U) 
                                                 | vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[0U]);
    vlSelfRef.__VdfgRegularize_hebeb780c_0_11 = (((- (IData)(
                                                             (1U 
                                                              & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext[0U] 
                                                                 >> 0x0000000fU)))) 
                                                  << 0x00000010U) 
                                                 | vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext[0U]);
    vlSelfRef.halted = (((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted) 
                         << 1U) | (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted));
    vlSelfRef.m68k_top__DOT__snoop_valid = ((IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_valid_q) 
                                            & (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_we_q));
    vlSelfRef.halt_code_flat = (((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halt_code) 
                                 << 0x00000010U) | (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halt_code));
    vlSelfRef.retired_flat = (((QData)((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__retired)) 
                               << 0x00000020U) | (QData)((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__retired)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_lock = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_lock = 0U;
    vlSelfRef.m68k_top__DOT__p_we = ((((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_we) 
                                       << 3U) | ((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_we) 
                                                 << 2U)) 
                                     | (((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_we) 
                                         << 1U) | (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_we)));
    vlSelfRef.m68k_top__DOT__u_bus__DOT__wdata[0U] 
        = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_wdata;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__wdata[1U] 
        = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_wdata;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__wdata[2U] 
        = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_wdata;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__wdata[3U] 
        = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_wdata;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__be[0U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_be;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__be[1U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_be;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__be[2U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_be;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__be[3U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_be;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                 >> 3U));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                 >> 3U));
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
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                 >> 6U));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                 >> 9U));
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
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_ext_words = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind = 0U;
    if ((0x4e71U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind = 1U;
    } else if ((0x4e75U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind = 2U;
    } else if ((0x4e72U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind = 3U;
    } else if ((0x009dU == (0x000001ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                           >> 7U)))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind 
            = ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                ? 4U : 5U);
    } else if ((IData)((0x41c0U == (0xf1c0U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind = 6U;
    } else if ((0x012bU == (0x000003ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                           >> 6U)))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind = 0x0bU;
    } else if ((IData)((0x7000U == (0xf100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind = 7U;
    } else if ((2U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                      >> 0x0cU)))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind = 8U;
    } else if ((IData)((0x5080U == (0xf0c0U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind = 9U;
    } else if ((6U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                      >> 0x0cU)))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind = 0x0cU;
    } else if ((IData)(((0x0080U == (0x00c0U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) 
                        & (((((0x0dU == (0x0000000fU 
                                         & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                            >> 0x0cU))) 
                              | (9U == (0x0000000fU 
                                        & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                           >> 0x0cU)))) 
                             | (0x0cU == (0x0000000fU 
                                          & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 0x0cU)))) 
                            | (8U == (0x0000000fU & 
                                      ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                       >> 0x0cU)))) 
                           | (0x0bU == (0x0000000fU 
                                        & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                           >> 0x0cU))))))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind = 0x0aU;
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                 >> 6U));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                 >> 9U));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_imm = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_movq 
        = (((- (IData)((1U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                              >> 7U)))) << 8U) | (0x000000ffU 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op)));
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_addq 
        = ((0U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                         >> 9U))) ? 8U : (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                >> 9U)));
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_disp 
        = (((- (IData)((1U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                              >> 7U)))) << 8U) | (0x000000ffU 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_ext_words = 0U;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__addr[0U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_addr;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__addr[1U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_addr;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__addr[2U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_addr;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__addr[3U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_addr;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                 >> 9U));
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                 >> 9U));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_ea = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_ea = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_next = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_next = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_next = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_an_next = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stop_now 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid) 
           & ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)) 
              & ((~ (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted)) 
                 & (3U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stop_now 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_valid) 
           & ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state)) 
              & ((~ (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted)) 
                 & (3U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_ea = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_ea = 0U;
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
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_cc 
                                                = (0x0000000fU 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                      >> 8U));
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
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 6U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 9U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__r 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 9U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__m 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 6U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__Vfuncout 
                                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__m))
                                            ? ((0U 
                                                == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__r))
                                                ? 1U
                                                : (
                                                   (1U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__r))
                                                    ? 2U
                                                    : 
                                                   ((4U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__r))
                                                     ? 2U
                                                     : 0U)))
                                            : 0U);
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_ext_words 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__Vfuncout;
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
                    if ((IData)((0x41c0U == (0xf1c0U 
                                             & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    } else {
                        if ((0x012bU == (0x000003ffU 
                                         & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                            >> 6U)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 7U;
                        } else if ((IData)((0x7000U 
                                            == (0xf100U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                        } else if ((2U == (0x0000000fU 
                                           & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                              >> 0x0cU)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                        } else if ((IData)((0x5080U 
                                            == (0xf0c0U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                                = ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                    ? 2U : 1U);
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
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                                    = ((0x00008000U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                        ? ((0x00004000U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                            ? ((0x00002000U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                ? 0U
                                                : (
                                                   (0x00001000U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 1U
                                                    : 3U))
                                            : ((0x00002000U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (0x00001000U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 5U
                                                     : 6U)
                                                    : 0U)
                                                : (
                                                   (0x00001000U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 2U
                                                    : 4U)))
                                        : 0U);
                            }
                        }
                        if ((0x012bU != (0x000003ffU 
                                         & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                            >> 6U)))) {
                            if ((IData)((0x7000U == 
                                         (0xf100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U));
                            } else if ((2U != (0x0000000fU 
                                               & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                  >> 0x0cU)))) {
                                if ((1U & (~ (IData)(
                                                     (0x5080U 
                                                      == 
                                                      (0xf0c0U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))))) {
                                    if ((6U != (0x0000000fU 
                                                & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                   >> 0x0cU)))) {
                                        if ((IData)(
                                                    ((0x0080U 
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
                                                = (7U 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                      >> 9U));
                                        }
                                    }
                                }
                            }
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
            if ((0x4e72U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words = 1U;
            } else if ((0x009dU == (0x000001ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                   >> 7U)))) {
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
            } else if ((IData)((0x41c0U == (0xf1c0U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
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
            } else if ((0x012bU == (0x000003ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                   >> 6U)))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__Vfuncout 
                    = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__m))
                        ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__r))
                            ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__r))
                                     ? 2U : ((4U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__r))
                                              ? 2U : 0U)))
                        : 0U);
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__Vfuncout;
            } else if ((1U & (~ (IData)((0x7000U == 
                                         (0xf100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))))) {
                if ((2U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                           >> 0x0cU)))) {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__Vfuncout 
                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__m))
                            ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__r))
                                ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__r))
                                         ? 2U : ((4U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__r))
                                                  ? 2U
                                                  : 0U)))
                            : 0U);
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__Vfuncout;
                } else if ((IData)((0x5080U == (0xf0c0U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))))) {
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
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__24__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__24__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__24__Vfuncout 
                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__24__m))
                            ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__24__r))
                                ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__24__r))
                                         ? 2U : ((4U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__24__r))
                                                  ? 2U
                                                  : 0U)))
                            : 0U);
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__24__Vfuncout;
                }
            }
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_ext_words = 0U;
    if ((0x4e71U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
        if ((0x4e75U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
            if ((0x4e72U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x009dU != (0x000001ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                >> 7U)))) {
                    if ((1U & (~ (IData)((0x41c0U == 
                                          (0xf1c0U 
                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))))) {
                        if ((0x012bU != (0x000003ffU 
                                         & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                            >> 6U)))) {
                            if ((1U & (~ (IData)((0x7000U 
                                                  == 
                                                  (0xf100U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))))) {
                                if ((2U != (0x0000000fU 
                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                               >> 0x0cU)))) {
                                    if ((1U & (~ (IData)(
                                                         (0x5080U 
                                                          == 
                                                          (0xf0c0U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))))) {
                                        if ((6U == 
                                             (0x0000000fU 
                                              & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 0x0cU)))) {
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_cc 
                                                = (0x0000000fU 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                      >> 8U));
                                            if ((0U 
                                                 == 
                                                 (0x000000ffU 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op)))) {
                                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_needs_disp_word = 1U;
                                            }
                                        }
                                    }
                                }
                                if ((2U == (0x0000000fU 
                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                               >> 0x0cU)))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 6U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 9U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__47__r 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 9U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__47__m 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 6U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__47__Vfuncout 
                                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__47__m))
                                            ? ((0U 
                                                == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__47__r))
                                                ? 1U
                                                : (
                                                   (1U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__47__r))
                                                    ? 2U
                                                    : 
                                                   ((4U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__47__r))
                                                     ? 2U
                                                     : 0U)))
                                            : 0U);
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_ext_words 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__47__Vfuncout;
                                }
                            }
                            if ((IData)((0x7000U == 
                                         (0xf100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_imm 
                                    = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_movq;
                            } else if ((2U != (0x0000000fU 
                                               & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                  >> 0x0cU)))) {
                                if ((IData)((0x5080U 
                                             == (0xf0c0U 
                                                 & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_imm 
                                        = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_addq;
                                } else if ((6U == (0x0000000fU 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                      >> 0x0cU)))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_imm 
                                        = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__imm_disp;
                                }
                            }
                        }
                    }
                    if ((IData)((0x41c0U == (0xf1c0U 
                                             & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    } else {
                        if ((0x012bU == (0x000003ffU 
                                         & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                            >> 6U)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 7U;
                        } else if ((IData)((0x7000U 
                                            == (0xf100U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                        } else if ((2U == (0x0000000fU 
                                           & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                              >> 0x0cU)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                        } else if ((IData)((0x5080U 
                                            == (0xf0c0U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                                = ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                    ? 2U : 1U);
                        } else if ((6U != (0x0000000fU 
                                           & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                              >> 0x0cU)))) {
                            if ((IData)(((0x0080U == 
                                          (0x00c0U 
                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) 
                                         & (((((0x0dU 
                                                == 
                                                (0x0000000fU 
                                                 & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                    >> 0x0cU))) 
                                               | (9U 
                                                  == 
                                                  (0x0000000fU 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                      >> 0x0cU)))) 
                                              | (0x0cU 
                                                 == 
                                                 (0x0000000fU 
                                                  & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                     >> 0x0cU)))) 
                                             | (8U 
                                                == 
                                                (0x0000000fU 
                                                 & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                    >> 0x0cU)))) 
                                            | (0x0bU 
                                               == (0x0000000fU 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                      >> 0x0cU))))))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                                    = ((0x00008000U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                        ? ((0x00004000U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                            ? ((0x00002000U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                ? 0U
                                                : (
                                                   (0x00001000U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 1U
                                                    : 3U))
                                            : ((0x00002000U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (0x00001000U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 5U
                                                     : 6U)
                                                    : 0U)
                                                : (
                                                   (0x00001000U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 2U
                                                    : 4U)))
                                        : 0U);
                            }
                        }
                        if ((0x012bU != (0x000003ffU 
                                         & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                            >> 6U)))) {
                            if ((IData)((0x7000U == 
                                         (0xf100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))) {
                                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U));
                            } else if ((2U != (0x0000000fU 
                                               & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                  >> 0x0cU)))) {
                                if ((1U & (~ (IData)(
                                                     (0x5080U 
                                                      == 
                                                      (0xf0c0U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))))) {
                                    if ((6U != (0x0000000fU 
                                                & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                   >> 0x0cU)))) {
                                        if ((IData)(
                                                    ((0x0080U 
                                                      == 
                                                      (0x00c0U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) 
                                                     & (((((0x0dU 
                                                            == 
                                                            (0x0000000fU 
                                                             & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                                >> 0x0cU))) 
                                                           | (9U 
                                                              == 
                                                              (0x0000000fU 
                                                               & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                                  >> 0x0cU)))) 
                                                          | (0x0cU 
                                                             == 
                                                             (0x0000000fU 
                                                              & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                                 >> 0x0cU)))) 
                                                         | (8U 
                                                            == 
                                                            (0x0000000fU 
                                                             & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                                >> 0x0cU)))) 
                                                        | (0x0bU 
                                                           == 
                                                           (0x0000000fU 
                                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                               >> 0x0cU))))))) {
                                            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                                = (7U 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                      >> 9U));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if ((0x009dU == (0x000001ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                >> 7U)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                } else if ((1U & (~ (IData)((0x41c0U 
                                             == (0xf1c0U 
                                                 & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))))) {
                    if ((0x012bU == (0x000003ffU & 
                                     ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                      >> 6U)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                    } else if ((1U & (~ (IData)((0x7000U 
                                                 == 
                                                 (0xf100U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))))) {
                        if ((2U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                   >> 0x0cU)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                        } else if ((IData)((0x5080U 
                                            == (0xf0c0U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                        } else if ((6U != (0x0000000fU 
                                           & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                              >> 0x0cU)))) {
                            if ((IData)(((0x0080U == 
                                          (0x00c0U 
                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) 
                                         & (((((0x0dU 
                                                == 
                                                (0x0000000fU 
                                                 & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                    >> 0x0cU))) 
                                               | (9U 
                                                  == 
                                                  (0x0000000fU 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                      >> 0x0cU)))) 
                                              | (0x0cU 
                                                 == 
                                                 (0x0000000fU 
                                                  & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                     >> 0x0cU)))) 
                                             | (8U 
                                                == 
                                                (0x0000000fU 
                                                 & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                    >> 0x0cU)))) 
                                            | (0x0bU 
                                               == (0x0000000fU 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                      >> 0x0cU))))))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                            }
                        }
                    }
                }
            }
            if ((0x4e72U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_ext_words = 1U;
            } else if ((0x009dU == (0x000001ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                   >> 7U)))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__43__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__43__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__43__Vfuncout 
                    = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__43__m))
                        ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__43__r))
                            ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__43__r))
                                     ? 2U : ((4U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__43__r))
                                              ? 2U : 0U)))
                        : 0U);
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__43__Vfuncout;
            } else if ((IData)((0x41c0U == (0xf1c0U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__44__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__44__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__44__Vfuncout 
                    = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__44__m))
                        ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__44__r))
                            ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__44__r))
                                     ? 2U : ((4U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__44__r))
                                              ? 2U : 0U)))
                        : 0U);
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__44__Vfuncout;
            } else if ((0x012bU == (0x000003ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                   >> 6U)))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__45__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__45__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__45__Vfuncout 
                    = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__45__m))
                        ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__45__r))
                            ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__45__r))
                                     ? 2U : ((4U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__45__r))
                                              ? 2U : 0U)))
                        : 0U);
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__45__Vfuncout;
            } else if ((1U & (~ (IData)((0x7000U == 
                                         (0xf100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))))) {
                if ((2U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                           >> 0x0cU)))) {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__46__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__46__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__46__Vfuncout 
                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__46__m))
                            ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__46__r))
                                ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__46__r))
                                         ? 2U : ((4U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__46__r))
                                                  ? 2U
                                                  : 0U)))
                            : 0U);
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__46__Vfuncout;
                } else if ((IData)((0x5080U == (0xf0c0U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))))) {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__48__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__48__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__48__Vfuncout 
                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__48__m))
                            ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__48__r))
                                ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__48__r))
                                         ? 2U : ((4U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__48__r))
                                                  ? 2U
                                                  : 0U)))
                            : 0U);
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__48__Vfuncout;
                } else if ((6U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                  >> 0x0cU)))) {
                    if ((0U == (0x000000ffU & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_ext_words = 1U;
                    }
                } else if ((IData)(((0x0080U == (0x00c0U 
                                                 & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) 
                                    & (((((0x0dU == 
                                           (0x0000000fU 
                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                               >> 0x0cU))) 
                                          | (9U == 
                                             (0x0000000fU 
                                              & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 0x0cU)))) 
                                         | (0x0cU == 
                                            (0x0000000fU 
                                             & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                >> 0x0cU)))) 
                                        | (8U == (0x0000000fU 
                                                  & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                     >> 0x0cU)))) 
                                       | (0x0bU == 
                                          (0x0000000fU 
                                           & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                              >> 0x0cU))))))) {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__49__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__49__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__49__Vfuncout 
                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__49__m))
                            ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__49__r))
                                ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__49__r))
                                         ? 2U : ((4U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__49__r))
                                                  ? 2U
                                                  : 0U)))
                            : 0U);
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__49__Vfuncout;
                }
            }
        }
    }
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_update = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_update = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_update = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_an_update = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c = 0U;
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
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_is_mem = 0U;
    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))) {
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))) {
                if (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_reg)) 
                     || (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_reg)))) {
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_ea 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_imm32;
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_is_mem = 1U;
                }
            }
        } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode)))) {
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_ea 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_rb 
                   - (IData)(4U));
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_is_mem = 1U;
        }
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode)))) {
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_an_next 
                    = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_rb 
                       - (IData)(4U));
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_an_update = 1U;
            }
        }
    } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))) {
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_ea 
            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_rb;
        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))) {
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_an_next 
                = ((IData)(4U) + vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_rb);
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_an_update = 1U;
        }
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_is_mem = 1U;
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_req 
        = ((~ (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted)) 
           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dc_req_r));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_req 
        = ((~ (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted)) 
           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dc_req_r));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__hit 
        = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids
           [(0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr 
                            >> 2U))] & ((vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr 
                                         >> 0x0000000aU) 
                                        == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags
                                        [(0x000000ffU 
                                          & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr 
                                             >> 2U))]));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__hit 
        = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids
           [(0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr 
                            >> 2U))] & ((vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr 
                                         >> 0x0000000aU) 
                                        == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags
                                        [(0x000000ffU 
                                          & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr 
                                             >> 2U))]));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand = 0U;
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
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem = 0U;
    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode)))) {
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_next 
                    = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_ra 
                       - (IData)(4U));
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_update = 1U;
            }
        }
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
                if (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg)) 
                     || (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg)))) {
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_ea 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_imm32;
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem = 1U;
                }
                if ((1U & (~ ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg)) 
                              || (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg)))))) {
                    if ((4U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg))) {
                        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_imm32;
                    }
                }
            }
        } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode)))) {
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_ea 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_ra 
                   - (IData)(4U));
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem = 1U;
        }
    } else {
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_next 
                    = ((IData)(4U) + vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_ra);
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_update = 1U;
            }
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_ea 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_ra;
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem = 1U;
        }
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode) 
                      >> 1U)))) {
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_ra;
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_req 
        = ((~ (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted)) 
           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_req 
        = ((~ (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted)) 
           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_busy));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__hit 
        = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids
           [(0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr 
                            >> 2U))] & ((vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr 
                                         >> 0x0000000aU) 
                                        == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags
                                        [(0x000000ffU 
                                          & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr 
                                             >> 2U))]));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__hit 
        = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids
           [(0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_addr 
                            >> 2U))] & ((vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_addr 
                                         >> 0x0000000aU) 
                                        == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags
                                        [(0x000000ffU 
                                          & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_addr 
                                             >> 2U))]));
    vlSelfRef.m68k_top__DOT__p_resp_data = vlSelfRef.m68k_top__DOT__u_bus__DOT__mem
        [vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_idx_q];
    vlSelfRef.m68k_top__DOT__p_resp_valid = 0U;
    if (vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_valid_q) {
        vlSelfRef.m68k_top__DOT__p_resp_valid = ((IData)(vlSelfRef.m68k_top__DOT__p_resp_valid) 
                                                 | (0x0fU 
                                                    & ((IData)(1U) 
                                                       << (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_port_q))));
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_src_base_idx 
        = ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode))
            ? ((- (IData)((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode))))) 
               & ((8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg)) 
                  & (- (IData)((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode) 
                                         >> 1U)))))))
            : (((0U != (3U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode))) 
                << 3U) | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_src_base_idx 
        = ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode))
            ? ((- (IData)((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode))))) 
               & ((8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg)) 
                  & (- (IData)((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode) 
                                         >> 1U)))))))
            : (((0U != (3U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode))) 
                << 3U) | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_reg_idx_full 
        = (((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a) 
            << 3U) | (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_reg_idx_full 
        = (((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a) 
            << 3U) | (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data_c = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_we_c = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_idx_c = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_data_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__comb_read_hit 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_req) 
           & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__state)) 
              & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_we)) 
                 & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock)) 
                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__hit)))));
    m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__comb_read_hit 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_req) 
           & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__state)) 
              & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_we)) 
                 & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_lock)) 
                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__hit)))));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_we = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_be = 0x0fU;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_wdata = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_addr = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_we_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_idx_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_we = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_be = 0x0fU;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_we_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_wdata = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_addr = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_we_c = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_idx_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_mem = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__comb_read_hit 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_req) 
           & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__state)) 
              & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__hit)));
    m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__comb_read_hit 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_req) 
           & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__state)) 
              & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__hit)));
    m68k_top__DOT__p_req = (((((~ ((IData)(vlSelfRef.m68k_top__DOT__p_resp_valid) 
                                   >> 3U)) & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__bus_req_r)) 
                              << 3U) | (((~ ((IData)(vlSelfRef.m68k_top__DOT__p_resp_valid) 
                                             >> 2U)) 
                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__bus_req_r)) 
                                        << 2U)) | (
                                                   (((~ 
                                                      ((IData)(vlSelfRef.m68k_top__DOT__p_resp_valid) 
                                                       >> 1U)) 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__bus_req_r)) 
                                                    << 1U) 
                                                   | ((~ (IData)(vlSelfRef.m68k_top__DOT__p_resp_valid)) 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__bus_req_r))));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__bus_resp_now 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__state) 
           & ((IData)(vlSelfRef.m68k_top__DOT__p_resp_valid) 
              >> 1U));
    m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__bus_resp_now 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__state) 
           & ((IData)(vlSelfRef.m68k_top__DOT__p_resp_valid) 
              >> 3U));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__bus_resp_now 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__state) 
           & (IData)(vlSelfRef.m68k_top__DOT__p_resp_valid));
    m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__bus_resp_now 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__state) 
           & ((IData)(vlSelfRef.m68k_top__DOT__p_resp_valid) 
              >> 2U));
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
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_rb_idx 
        = ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind))
            ? (((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind))
                 ? ((- (IData)((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind))))) 
                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_reg_idx_full))
                 : (((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode))
                      ? ((- (IData)((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode))))) 
                         & ((8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg)) 
                            & (- (IData)((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode) 
                                                   >> 1U)))))))
                      : (((0U != (3U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode))) 
                          << 3U) | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg))) 
                    & (- (IData)((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind))))))) 
               & (- (IData)((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind) 
                                      >> 2U)))))) : 
           ((- (IData)((3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind) 
                                     >> 1U))))) & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_reg_idx_full)));
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
        m68k_top__DOT__u_bus__DOT__cand = (3U & (IData)(m68k_top__DOT__u_bus__DOT__cand_ext));
        m68k_top__DOT__u_bus__DOT__cand_ext = (7U & 
                                               ((IData)(1U) 
                                                + (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__rrobin)));
        if ((1U & ((~ (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid)) 
                   & ((IData)(m68k_top__DOT__p_req) 
                      >> (IData)(m68k_top__DOT__u_bus__DOT__cand))))) {
            vlSelfRef.m68k_top__DOT__u_bus__DOT__winner 
                = m68k_top__DOT__u_bus__DOT__cand;
            vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid = 1U;
        }
        m68k_top__DOT__u_bus__DOT__cand = (3U & (IData)(m68k_top__DOT__u_bus__DOT__cand_ext));
        m68k_top__DOT__u_bus__DOT__cand_ext = (7U & 
                                               ((IData)(2U) 
                                                + (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__rrobin)));
        if ((1U & ((~ (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid)) 
                   & ((IData)(m68k_top__DOT__p_req) 
                      >> (IData)(m68k_top__DOT__u_bus__DOT__cand))))) {
            vlSelfRef.m68k_top__DOT__u_bus__DOT__winner 
                = m68k_top__DOT__u_bus__DOT__cand;
            vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid = 1U;
        }
        m68k_top__DOT__u_bus__DOT__cand = (3U & (IData)(m68k_top__DOT__u_bus__DOT__cand_ext));
        m68k_top__DOT__u_bus__DOT__cand_ext = (7U & 
                                               ((IData)(3U) 
                                                + (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__rrobin)));
        if ((1U & ((~ (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid)) 
                   & ((IData)(m68k_top__DOT__p_req) 
                      >> (IData)(m68k_top__DOT__u_bus__DOT__cand))))) {
            vlSelfRef.m68k_top__DOT__u_bus__DOT__winner 
                = m68k_top__DOT__u_bus__DOT__cand;
            vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid = 1U;
        }
        m68k_top__DOT__u_bus__DOT__cand = (3U & (IData)(m68k_top__DOT__u_bus__DOT__cand_ext));
        if ((1U & ((~ (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid)) 
                   & ((IData)(m68k_top__DOT__p_req) 
                      >> (IData)(m68k_top__DOT__u_bus__DOT__cand))))) {
            vlSelfRef.m68k_top__DOT__u_bus__DOT__winner 
                = m68k_top__DOT__u_bus__DOT__cand;
            vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid = 1U;
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_ack 
        = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__bus_resp_now) 
           | (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__comb_read_hit));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_rdata 
        = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__comb_read_hit)
            ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data
           [(0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr 
                            >> 2U))] : (vlSelfRef.m68k_top__DOT__p_resp_data 
                                        & (- (IData)((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__bus_resp_now)))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_ack 
        = ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__bus_resp_now) 
           | (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__comb_read_hit));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_rdata 
        = ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__comb_read_hit)
            ? vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data
           [(0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr 
                            >> 2U))] : (vlSelfRef.m68k_top__DOT__p_resp_data 
                                        & (- (IData)((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__bus_resp_now)))));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_rdata 
        = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__comb_read_hit)
            ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data
           [(0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr 
                            >> 2U))] : (vlSelfRef.m68k_top__DOT__p_resp_data 
                                        & (- (IData)((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__bus_resp_now)))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_ack 
        = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__bus_resp_now) 
           | (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__comb_read_hit));
    m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_rdata 
        = ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__comb_read_hit)
            ? vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__data
           [(0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_addr 
                            >> 2U))] : (vlSelfRef.m68k_top__DOT__p_resp_data 
                                        & (- (IData)((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__bus_resp_now)))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_ack 
        = ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__bus_resp_now) 
           | (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__comb_read_hit));
    vlSelfRef.m68k_top__DOT__u_bus__DOT__mem_idx = 
        (0x00003fffU & (vlSelfRef.m68k_top__DOT__u_bus__DOT__addr
                        [vlSelfRef.m68k_top__DOT__u_bus__DOT__winner] 
                        >> 2U));
    if ((1U & (~ ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid)) 
                  | (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted))))) {
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
                if ((8U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((0x0aU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op;
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb;
                    }
                }
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
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op;
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb;
                            if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                if ((6U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op))) {
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_we_c = 1U;
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_idx_c 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_reg_idx_full;
                                }
                            }
                        }
                    } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op;
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand;
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
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)))) {
                if ((8U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((5U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c = 1U;
                    } else if ((0x0cU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c = 1U;
                    }
                }
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
            }
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c = 1U;
            } else if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c = 1U;
            }
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
            if ((8U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if ((0x0aU != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((2U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c = 1U;
                    }
                }
            }
            if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_update) {
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c = 1U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx_c 
                        = (8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data_c 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_next;
                }
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c 
                    = (0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode));
            } else if ((0x0aU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_update) {
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c = 1U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx_c 
                        = (8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data_c 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_next;
                }
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c = 1U;
            } else if ((2U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c = 1U;
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx_c = 0x0fU;
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data_c 
                    = ((IData)(4U) + vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_sp);
            }
        } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
            if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
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
        } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                          >> 1U)))) {
                if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c = 1U;
                }
            }
            if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c = 1U;
                }
            }
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled 
        = (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)) 
            & ((~ ((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted) 
                   | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem))) 
               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid))) 
           | ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_ack) 
              & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid) 
                 & ((~ (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted)) 
                    & (((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)) 
                        & (0x0bU != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) 
                       | ((2U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)) 
                          | (3U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))))))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_target_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b = 0U;
    if ((1U & (~ ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_valid)) 
                  | (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted))))) {
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state)))) {
                if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                                  >> 2U)))) {
                        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_lock = 1U;
                            }
                        }
                    }
                    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                                      >> 1U)))) {
                            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                                if ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_we = 1U;
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_be = 0x0fU;
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_wdata 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_pc_next;
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_addr 
                                        = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_sp 
                                           - (IData)(4U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_mem = 1U;
                                }
                            }
                        }
                    } else {
                        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_we = 0U;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_be = 0x0fU;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_addr 
                                    = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_ea;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_mem = 1U;
                            } else if (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_we = 0U;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_be = 0x0fU;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_addr 
                                    = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_ea;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_mem = 1U;
                            }
                        } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                            if (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_we = 0U;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_be = 0x0fU;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_addr 
                                    = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_ea;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_mem = 1U;
                            } else if (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_is_mem) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_we = 1U;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_be = 0x0fU;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_addr 
                                    = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_ea;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_mem = 1U;
                            }
                        }
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                                      >> 1U)))) {
                            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                                if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                    if (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_is_mem) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_wdata 
                                            = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand;
                                    }
                                }
                            }
                        }
                    }
                } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                                  >> 1U)))) {
                        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_we = 1U;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_be = 0x0fU;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_wdata 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_pc_next;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_addr 
                                = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_sp 
                                   - (IData)(4U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_mem = 1U;
                        }
                    }
                } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_we = 0U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_be = 0x0fU;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_addr 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_sp;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_mem = 1U;
                    }
                }
            }
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state))) {
                if ((8U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((0x0aU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_alu_op;
                        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_rb;
                    }
                }
                if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_we_c = 1U;
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_idx_c 
                        = ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))
                            ? (8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_reg))
                            : (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_reg));
                } else if ((0x0aU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((6U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_alu_op))) {
                        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_we_c = 1U;
                        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_idx_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_reg_idx_full;
                    }
                }
            } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                              >> 2U)))) {
                    if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_alu_op;
                            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_rb;
                            if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                if ((6U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_alu_op))) {
                                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_we_c = 1U;
                                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_idx_c 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_reg_idx_full;
                                }
                            }
                        }
                    } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_alu_op;
                        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a 
                            = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand;
                        if (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode)) 
                             | (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode)))) {
                            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_we_c = 1U;
                            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_idx_c 
                                = ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))
                                    ? (8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg))
                                    : (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg));
                        }
                    } else if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem)))) {
                        if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_is_mem)))) {
                            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_we_c = 1U;
                            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_idx_c 
                                = ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))
                                    ? (8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_reg))
                                    : (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_reg));
                        }
                    }
                }
            } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_we_c = 1U;
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_idx_c 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_reg_idx_full;
                }
            }
        }
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state)))) {
                if ((8U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((5U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_c = 1U;
                    } else if ((0x0cU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_c = 1U;
                    }
                }
                if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_an_update) {
                        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_we_c = 1U;
                        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_idx_c 
                            = (8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_reg));
                        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_data_c 
                            = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_an_next;
                    }
                } else if ((5U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_we_c = 1U;
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_idx_c = 0x0fU;
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_data_c 
                        = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_sp 
                           - (IData)(4U));
                } else if ((0x0cU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_we_c = 1U;
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_idx_c = 0x0fU;
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_data_c 
                        = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_sp 
                           - (IData)(4U));
                }
            }
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_we_c = 1U;
            } else if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_we_c = 1U;
            }
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state))) {
            if ((8U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                if ((0x0aU != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((2U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_c = 1U;
                    }
                }
            }
            if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                if (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_update) {
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_we_c = 1U;
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_idx_c 
                        = (8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg));
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_data_c 
                        = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_next;
                }
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_we_c 
                    = (0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode));
            } else if ((0x0aU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                if (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_update) {
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_we_c = 1U;
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_idx_c 
                        = (8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg));
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_data_c 
                        = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_next;
                }
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_we_c = 1U;
            } else if ((2U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_we_c = 1U;
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_idx_c = 0x0fU;
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_data_c 
                    = ((IData)(4U) + vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_sp);
            }
        } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
            if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_c 
                            = (1U & ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc))
                                      ? ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc))
                                          ? ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc))
                                              ? ((1U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc))
                                                  ? 
                                                 (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z) 
                                                   | ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n) 
                                                      & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v)))) 
                                                  | ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n)) 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v)))
                                                  : 
                                                 ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z)) 
                                                  & (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n) 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v)) 
                                                     | ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n)) 
                                                        & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v))))))
                                              : ((1U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc))
                                                  ? 
                                                 (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n) 
                                                   & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v))) 
                                                  | ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n)) 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v)))
                                                  : 
                                                 (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n) 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v)) 
                                                  | ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n)) 
                                                     & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v))))))
                                          : ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc))
                                              ? ((1U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc))
                                                  ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n)
                                                  : 
                                                 (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n)))
                                              : ((1U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc))
                                                  ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v)
                                                  : 
                                                 (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v)))))
                                      : ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc))
                                          ? ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc))
                                              ? ((1U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc))
                                                  ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z)
                                                  : 
                                                 (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z)))
                                              : ((1U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc))
                                                  ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c)
                                                  : 
                                                 (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c))))
                                          : ((1U & 
                                              (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc) 
                                                  >> 1U))) 
                                             || (1U 
                                                 & ((1U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc))
                                                     ? 
                                                    ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c) 
                                                     | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z))
                                                     : 
                                                    ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c)) 
                                                     & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z)))))))));
                        if ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_c = 0U;
                        }
                    }
                }
            }
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                          >> 2U)))) {
                if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                        if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_we_c = 1U;
                        }
                    }
                } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_we_c = 1U;
                } else if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem)))) {
                    if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_is_mem)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_we_c 
                            = (0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode));
                    }
                }
            }
        } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                          >> 1U)))) {
                if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_c = 1U;
                }
            }
            if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_we_c = 1U;
                }
            }
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_settled 
        = (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state)) 
            & ((~ ((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted) 
                   | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_mem))) 
               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_valid))) 
           | ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_ack) 
              & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_valid) 
                 & ((~ (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted)) 
                    & (((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state)) 
                        & (0x0bU != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) 
                       | ((2U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state)) 
                          | (3U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state))))))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_target_c = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w 
        = (0x0000ffffU & ((2U & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_addr)
                           ? m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_rdata
                           : (m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_rdata 
                              >> 0x00000010U)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy) 
           & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain)) 
              & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_ack)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__half_word_w 
        = (0x0000ffffU & ((2U & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_fetch_addr)
                           ? m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_rdata
                           : (m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_rdata 
                              >> 0x00000010U)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_latching 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_busy) 
           & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_drain)) 
              & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_ack)));
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
        = ((~ ((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted) 
               | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled))) 
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
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum = 0ULL;
    if ((1U & (~ ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid)) 
                  | (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted))))) {
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)))) {
                if ((8U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((5U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_target_c 
                            = ((7U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))
                                ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_imm32
                                : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_ra);
                    } else if ((0x0cU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_target_c 
                            = ((IData)(2U) + (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_pc 
                                              + vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_branch_imm));
                    }
                }
            }
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
            if ((8U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if ((0x0aU != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((2U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_target_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_rdata;
                    }
                }
            }
        } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
            if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_target_c 
                            = ((IData)(2U) + (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_pc 
                                              + vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_branch_imm));
                    }
                }
            }
        } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                          >> 1U)))) {
                if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_target_c 
                        = ((7U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))
                            ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_imm32
                            : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_ra);
                }
            }
        }
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state) 
                      >> 1U)))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
                if ((8U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((0x0aU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_rdata;
                    }
                }
            } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                              >> 2U)))) {
                    if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                            if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand;
                            }
                        }
                    } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
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
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_we 
        = ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_we_c) 
           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_settled));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_widx 
        = ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_idx_c) 
           & (- (IData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_settled))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_we 
        = ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_we_c) 
           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_settled));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_idx 
        = ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_idx_c) 
           & (- (IData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_settled))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_data 
        = (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_data_c 
           & (- (IData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_settled))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stall 
        = ((~ ((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted) 
               | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_settled))) 
           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_valid));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__redirect_valid 
        = (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_settled) 
            & ((0x0cU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)) 
               & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_predicted_taken) 
                  != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_c)))) 
           | (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_settled) 
               & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_c) 
                  & (0x0cU != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) 
              | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stop_now)));
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum = 0ULL;
    if ((1U & (~ ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_valid)) 
                  | (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted))))) {
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state)))) {
                if ((8U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((5U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_target_c 
                            = ((7U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))
                                ? vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_imm32
                                : vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_ra);
                    } else if ((0x0cU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_target_c 
                            = ((IData)(2U) + (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_pc 
                                              + vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_branch_imm));
                    }
                }
            }
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state))) {
            if ((8U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                if ((0x0aU != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((2U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_target_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_rdata;
                    }
                }
            }
        } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
            if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_target_c 
                            = ((IData)(2U) + (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_pc 
                                              + vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_branch_imm));
                    }
                }
            }
        } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                          >> 1U)))) {
                if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_target_c 
                        = ((7U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))
                            ? vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_imm32
                            : vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_ra);
                }
            }
        }
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state) 
                      >> 1U)))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state))) {
                if ((8U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((0x0aU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_rdata;
                    }
                }
            } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                              >> 2U)))) {
                    if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                            if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
                                    = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand;
                            }
                        }
                    } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_imm_raw;
                    }
                }
            }
        }
    }
    if ((8U & (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
        if ((4U & (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y = 0U;
        } else if ((2U & (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y = 0U;
        } else if ((1U & (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y = 0U;
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b;
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                = (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
                   >> 0x1fU);
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                = (0U == m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b);
        }
    } else if ((4U & (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
        if ((2U & (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
            if ((1U & (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                    = (0x00000080U | m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b);
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                    = (1U & (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
                             >> 7U));
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                    = (0U == (0x000000ffU & m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b));
            } else {
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                    = (0x00000001ffffffffULL & ((QData)((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a)) 
                                                - (QData)((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b))));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                    = ((6U == (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))
                        ? m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a
                        : (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum));
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                    = (1U & (IData)((m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                     >> 0x1fU)));
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                    = (0U == (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum));
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c 
                    = (1U & (IData)((m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                     >> 0x20U)));
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v 
                    = (((m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a 
                         >> 0x1fU) != (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
                                       >> 0x1fU)) & 
                       ((1U & (IData)((m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                       >> 0x1fU))) 
                        != (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a 
                            >> 0x1fU)));
            }
        } else if ((1U & (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                = (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a 
                   ^ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b);
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                   >> 0x1fU);
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y);
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                = (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a 
                   | m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b);
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                   >> 0x1fU);
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y);
        }
    } else if ((2U & (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
        if ((1U & (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                = (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a 
                   & m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b);
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                   >> 0x1fU);
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y);
        } else {
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                = (0x00000001ffffffffULL & ((QData)((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a)) 
                                            - (QData)((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b))));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                = ((6U == (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))
                    ? m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a
                    : (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum));
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                = (1U & (IData)((m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                 >> 0x1fU)));
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                = (0U == (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum));
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c 
                = (1U & (IData)((m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                 >> 0x20U)));
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v 
                = (((m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a 
                     >> 0x1fU) != (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
                                   >> 0x1fU)) & ((1U 
                                                  & (IData)(
                                                            (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                                             >> 0x1fU))) 
                                                 != 
                                                 (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a 
                                                  >> 0x1fU)));
        }
    } else if ((1U & (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
            = (0x00000001ffffffffULL & ((QData)((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a)) 
                                        + (QData)((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b))));
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
            = (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum);
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
            = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
               >> 0x1fU);
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
            = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y);
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c 
            = (1U & (IData)((m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                             >> 0x20U)));
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v 
            = (((m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a 
                 >> 0x1fU) == (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
                               >> 0x1fU)) & ((vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                                              >> 0x1fU) 
                                             != (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a 
                                                 >> 0x1fU)));
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
            = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b;
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
            = (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
               >> 0x1fU);
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
            = (0U == m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b);
    }
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
    __VdfgRegularize_hebeb780c_0_2 = (((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot)) 
                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching))
                                       ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w)
                                       : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext[0U]);
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op 
        = (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot)) 
            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching))
            ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w)
            : (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_op));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ext_combo[1U] 
        = (((2U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_slot)) 
            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_latching))
            ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__half_word_w)
            : vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext[1U]);
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ext_combo[2U] 
        = (((3U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_slot)) 
            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_latching))
            ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__half_word_w)
            : vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext[2U]);
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ext_combo[3U] 
        = (((4U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_slot)) 
            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_latching))
            ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__half_word_w)
            : vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext[3U]);
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__new_words_done 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_words_done) 
                 + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_latching)));
    __VdfgRegularize_hebeb780c_0_3 = (((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_slot)) 
                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_latching))
                                       ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__half_word_w)
                                       : vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext[0U]);
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op 
        = (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_slot)) 
            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_latching))
            ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__half_word_w)
            : (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_op));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c = 0U;
    if ((1U & (~ ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid)) 
                  | (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted))))) {
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
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__6__pos 
                    = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr);
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__6__w 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_tas_word;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT____VlemCall_0__byte_at 
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
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                    = (1U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT____VlemCall_0__byte_at) 
                             >> 7U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__7__pos 
                    = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr);
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__7__w 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_tas_word;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT____VlemCall_1__byte_at 
                    = (0x000000ffU & ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__7__pos))
                                       ? ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__7__pos))
                                           ? vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__7__w
                                           : (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__7__w 
                                              >> 8U))
                                       : ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__7__pos))
                                           ? (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__7__w 
                                              >> 0x10U)
                                           : (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__7__w 
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
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c = 0U;
    if ((1U & (~ ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_valid)) 
                  | (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted))))) {
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state) 
                      >> 1U)))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state))) {
                if ((8U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((0x0aU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v_c 
                            = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c_c 
                            = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c;
                    }
                }
                if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_rdata;
                } else if ((0x0aU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((6U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_alu_op))) {
                        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y;
                    }
                }
            } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                              >> 2U)))) {
                    if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                            if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v_c 
                                    = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c_c 
                                    = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c;
                                if ((6U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_alu_op))) {
                                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y;
                                }
                            }
                        }
                    } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v_c 
                            = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c_c 
                            = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c;
                        if (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode)) 
                             | (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode)))) {
                            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y;
                        }
                    } else if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem)))) {
                        if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_is_mem)))) {
                            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c 
                                = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand;
                        }
                    }
                }
            } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c 
                        = ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))
                            ? vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_imm_raw
                            : ((7U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))
                                ? vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_imm32
                                : m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_ea));
                }
            }
        }
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__31__pos 
                    = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr);
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__31__w 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_tas_word;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT____VlemCall_0__byte_at 
                    = (0x000000ffU & ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__31__pos))
                                       ? ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__31__pos))
                                           ? vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__31__w
                                           : (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__31__w 
                                              >> 8U))
                                       : ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__31__pos))
                                           ? (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__31__w 
                                              >> 0x10U)
                                           : (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__31__w 
                                              >> 0x18U))));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                    = (1U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT____VlemCall_0__byte_at) 
                             >> 7U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__32__pos 
                    = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr);
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__32__w 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_tas_word;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT____VlemCall_1__byte_at 
                    = (0x000000ffU & ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__32__pos))
                                       ? ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__32__pos))
                                           ? vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__32__w
                                           : (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__32__w 
                                              >> 8U))
                                       : ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__32__pos))
                                           ? (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__32__w 
                                              >> 0x10U)
                                           : (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__32__w 
                                              >> 0x18U))));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                    = (0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT____VlemCall_1__byte_at));
            } else if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                    = (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand 
                       >> 0x1fU);
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                    = (0U == m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand);
            }
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state))) {
            if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                    = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_rdata 
                       >> 0x1fU);
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                    = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_rdata);
            } else if ((0x0aU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                    = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                    = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z;
            }
        } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                          >> 2U)))) {
                if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                        if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                                = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                                = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z;
                        }
                    }
                } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                        = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                        = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z;
                } else if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem)))) {
                    if ((1U & (~ (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_is_mem)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                            = (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand 
                               >> 0x1fU);
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                            = (0U == m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand);
                    }
                }
            }
        } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
            if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                        = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_imm_raw 
                           >> 0x1fU);
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                        = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_imm_raw);
                }
            }
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ext_combo[0U] 
        = __VdfgRegularize_hebeb780c_0_2;
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
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__r 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 9U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__m 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 6U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__Vfuncout 
                                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__m))
                                            ? ((0U 
                                                == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__r))
                                                ? 1U
                                                : (
                                                   (1U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__r))
                                                    ? 2U
                                                    : 
                                                   ((4U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__r))
                                                     ? 2U
                                                     : 0U)))
                                            : 0U);
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__Vfuncout;
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
            } else if ((IData)((0x41c0U == (0xf1c0U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))))) {
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
            } else if ((0x012bU == (0x000003ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                   >> 6U)))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__Vfuncout 
                    = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__m))
                        ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__r))
                            ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__r))
                                     ? 2U : ((4U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__r))
                                              ? 2U : 0U)))
                        : 0U);
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__Vfuncout;
            } else if ((1U & (~ (IData)((0x7000U == 
                                         (0xf100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))))))) {
                if ((2U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                           >> 0x0cU)))) {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__Vfuncout 
                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__m))
                            ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__r))
                                ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__r))
                                         ? 2U : ((4U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__r))
                                                  ? 2U
                                                  : 0U)))
                            : 0U);
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__Vfuncout;
                } else if ((IData)((0x5080U == (0xf0c0U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))))) {
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
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__Vfuncout 
                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__m))
                            ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__r))
                                ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__r))
                                         ? 2U : ((4U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__r))
                                                  ? 2U
                                                  : 0U)))
                            : 0U);
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__Vfuncout;
                }
            }
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ext_combo[0U] 
        = __VdfgRegularize_hebeb780c_0_3;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_cc = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_needs_disp_word = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_kind = 0U;
    if ((0x4e71U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))) {
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_kind = 1U;
    } else if ((0x4e75U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))) {
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_kind = 2U;
    } else if ((0x4e72U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))) {
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_kind = 3U;
    } else if ((0x009dU == (0x000001ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                           >> 7U)))) {
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_kind 
            = ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))
                ? 4U : 5U);
    } else if ((IData)((0x41c0U == (0xf1c0U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))))) {
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_kind = 6U;
    } else if ((0x012bU == (0x000003ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                           >> 6U)))) {
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_kind = 0x0bU;
    } else if ((IData)((0x7000U == (0xf100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))))) {
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_kind = 7U;
    } else if ((2U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                      >> 0x0cU)))) {
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_kind = 8U;
    } else if ((IData)((0x5080U == (0xf0c0U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))))) {
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_kind = 9U;
    } else if ((6U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                      >> 0x0cU)))) {
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_kind = 0x0cU;
    } else if ((IData)(((0x0080U == (0x00c0U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))) 
                        & (((((0x0dU == (0x0000000fU 
                                         & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                            >> 0x0cU))) 
                              | (9U == (0x0000000fU 
                                        & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                           >> 0x0cU)))) 
                             | (0x0cU == (0x0000000fU 
                                          & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                             >> 0x0cU)))) 
                            | (8U == (0x0000000fU & 
                                      ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                       >> 0x0cU)))) 
                           | (0x0bU == (0x0000000fU 
                                        & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                           >> 0x0cU))))))) {
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
    }
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_dst_ext_words = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_imm = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_movq 
        = (((- (IData)((1U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                              >> 7U)))) << 8U) | (0x000000ffU 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op)));
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_addq 
        = ((0U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                         >> 9U))) ? 8U : (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                                >> 9U)));
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_disp 
        = (((- (IData)((1U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                              >> 7U)))) << 8U) | (0x000000ffU 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op)));
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_src_ext_words = 0U;
    if ((0x4e71U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))) {
        if ((0x4e75U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))) {
            if ((0x4e72U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))) {
                if ((0x009dU != (0x000001ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                                >> 7U)))) {
                    if ((1U & (~ (IData)((0x41c0U == 
                                          (0xf1c0U 
                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))))))) {
                        if ((0x012bU != (0x000003ffU 
                                         & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                            >> 6U)))) {
                            if ((1U & (~ (IData)((0x7000U 
                                                  == 
                                                  (0xf100U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))))))) {
                                if ((2U != (0x0000000fU 
                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                               >> 0x0cU)))) {
                                    if ((1U & (~ (IData)(
                                                         (0x5080U 
                                                          == 
                                                          (0xf0c0U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))))))) {
                                        if ((6U == 
                                             (0x0000000fU 
                                              & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 0x0cU)))) {
                                            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_cc 
                                                = (0x0000000fU 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                                      >> 8U));
                                            if ((0U 
                                                 == 
                                                 (0x000000ffU 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op)))) {
                                                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_needs_disp_word = 1U;
                                            }
                                        }
                                    }
                                }
                                if ((2U == (0x0000000fU 
                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                               >> 0x0cU)))) {
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__40__r 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 9U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__40__m 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 6U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__40__Vfuncout 
                                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__40__m))
                                            ? ((0U 
                                                == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__40__r))
                                                ? 1U
                                                : (
                                                   (1U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__40__r))
                                                    ? 2U
                                                    : 
                                                   ((4U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__40__r))
                                                     ? 2U
                                                     : 0U)))
                                            : 0U);
                                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__40__Vfuncout;
                                }
                            }
                            if ((IData)((0x7000U == 
                                         (0xf100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))))) {
                                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_imm 
                                    = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_movq;
                            } else if ((2U != (0x0000000fU 
                                               & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                                  >> 0x0cU)))) {
                                if ((IData)((0x5080U 
                                             == (0xf0c0U 
                                                 & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))))) {
                                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_imm 
                                        = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_addq;
                                } else if ((6U == (0x0000000fU 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                                      >> 0x0cU)))) {
                                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_imm 
                                        = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_disp;
                                }
                            }
                        }
                    }
                }
            }
            if ((0x4e72U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))) {
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
            } else if ((0x009dU == (0x000001ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                                   >> 7U)))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__Vfuncout 
                    = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__m))
                        ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__r))
                            ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__r))
                                     ? 2U : ((4U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__r))
                                              ? 2U : 0U)))
                        : 0U);
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__Vfuncout;
            } else if ((IData)((0x41c0U == (0xf1c0U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__Vfuncout 
                    = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__m))
                        ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__r))
                            ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__r))
                                     ? 2U : ((4U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__r))
                                              ? 2U : 0U)))
                        : 0U);
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__Vfuncout;
            } else if ((0x012bU == (0x000003ffU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                                   >> 6U)))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__Vfuncout 
                    = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__m))
                        ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__r))
                            ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__r))
                                     ? 2U : ((4U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__r))
                                              ? 2U : 0U)))
                        : 0U);
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__Vfuncout;
            } else if ((1U & (~ (IData)((0x7000U == 
                                         (0xf100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))))))) {
                if ((2U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                           >> 0x0cU)))) {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__Vfuncout 
                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__m))
                            ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__r))
                                ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__r))
                                         ? 2U : ((4U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__r))
                                                  ? 2U
                                                  : 0U)))
                            : 0U);
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__Vfuncout;
                } else if ((IData)((0x5080U == (0xf0c0U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))))) {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__Vfuncout 
                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__m))
                            ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__r))
                                ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__r))
                                         ? 2U : ((4U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__r))
                                                  ? 2U
                                                  : 0U)))
                            : 0U);
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__Vfuncout;
                } else if ((6U == (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                                  >> 0x0cU)))) {
                    if ((0U == (0x000000ffU & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op)))) {
                        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                    }
                } else if ((IData)(((0x0080U == (0x00c0U 
                                                 & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op))) 
                                    & (((((0x0dU == 
                                           (0x0000000fU 
                                            & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                               >> 0x0cU))) 
                                          | (9U == 
                                             (0x0000000fU 
                                              & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 0x0cU)))) 
                                         | (0x0cU == 
                                            (0x0000000fU 
                                             & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                                >> 0x0cU)))) 
                                        | (8U == (0x0000000fU 
                                                  & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                                     >> 0x0cU)))) 
                                       | (0x0bU == 
                                          (0x0000000fU 
                                           & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                              >> 0x0cU))))))) {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__Vfuncout 
                        = ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__m))
                            ? ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__r))
                                ? 1U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__r))
                                         ? 2U : ((4U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__r))
                                                  ? 2U
                                                  : 0U)))
                            : 0U);
                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__Vfuncout;
                }
            }
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_wdata 
        = (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
           & (- (IData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_wdata 
        = (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c 
           & (- (IData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_settled))));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_bcc_uncond 
        = ((0x0cU == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind)) 
           & ((0U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_cc)) 
              | (1U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_cc))));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__disp32 
        = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_needs_disp_word)
            ? (((- (IData)((1U & ((IData)(__VdfgRegularize_hebeb780c_0_2) 
                                  >> 0x0000000fU)))) 
                << 0x00000010U) | (IData)(__VdfgRegularize_hebeb780c_0_2))
            : m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_imm);
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__total_words_eff 
        = (7U & ((IData)(1U) + ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words) 
                                + (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words))));
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_bcc_uncond 
        = ((0x0cU == (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_kind)) 
           & ((0U == (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_cc)) 
              | (1U == (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_cc))));
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__disp32 
        = ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_needs_disp_word)
            ? (((- (IData)((1U & ((IData)(__VdfgRegularize_hebeb780c_0_3) 
                                  >> 0x0000000fU)))) 
                << 0x00000010U) | (IData)(__VdfgRegularize_hebeb780c_0_3))
            : m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_imm);
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__total_words_eff 
        = (7U & ((IData)(1U) + ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_dst_ext_words) 
                                + (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_src_ext_words))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__predict_taken_w 
        = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_bcc_uncond) 
           | ((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__disp32 
               >> 0x0000001fU) & ((~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_bcc_uncond)) 
                                  & (0x0cU == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind)))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__would_be_complete 
        = (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__new_words_done) 
            >= (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__total_words_eff)) 
           & (0U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__new_words_done)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__predict_taken_w 
        = ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_bcc_uncond) 
           | ((m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__disp32 
               >> 0x0000001fU) & ((~ (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_bcc_uncond)) 
                                  & (0x0cU == (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_kind)))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__would_be_complete 
        = (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__new_words_done) 
            >= (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__total_words_eff)) 
           & (0U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__new_words_done)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_target 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__predict_taken_w)
            ? ((IData)(2U) + (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc 
                              + m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__disp32))
            : (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc 
               + ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__total_words_eff) 
                  << 1U)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_dispatch_w 
        = ((~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall) 
               | ((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted) 
                  | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid)))) 
           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__would_be_complete));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_fetch_target 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__predict_taken_w)
            ? ((IData)(2U) + (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_pc 
                              + m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__disp32))
            : (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_pc 
               + ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__total_words_eff) 
                  << 1U)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_dispatch_w 
        = ((~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stall) 
               | ((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted) 
                  | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__redirect_valid)))) 
           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__would_be_complete));
}

VL_ATTR_COLD void Vm68k_top___024root___eval_stl(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___eval_stl\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
        Vm68k_top___024root___stl_sequent__TOP__0(vlSelf);
    }
}

VL_ATTR_COLD bool Vm68k_top___024root___eval_phase__stl(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___eval_phase__stl\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VstlExecute;
    // Body
    Vm68k_top___024root___eval_triggers_vec__stl(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vm68k_top___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
    }
#endif
    __VstlExecute = Vm68k_top___024root___trigger_anySet__stl(vlSelfRef.__VstlTriggered);
    if (__VstlExecute) {
        Vm68k_top___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

bool Vm68k_top___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vm68k_top___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vm68k_top___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @(posedge clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @(negedge rst_n)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vm68k_top___024root___ctor_var_reset(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___ctor_var_reset\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1638864771569018232ull);
    vlSelf->halted = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 18300898478658917723ull);
    vlSelf->halt_code_flat = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 837389137883315161ull);
    vlSelf->retired_flat = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 9534046761757830170ull);
    vlSelf->m68k_top__DOT__p_we = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 10923809041123312772ull);
    vlSelf->m68k_top__DOT__p_resp_valid = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 15558741892508150942ull);
    vlSelf->m68k_top__DOT__p_resp_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5412736511730414642ull);
    vlSelf->m68k_top__DOT__snoop_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3562705624101463728ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_req = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10234964172446696401ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6156770161601447316ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_ack = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16481721190156875360ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_req = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14258306902057112167ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_we = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12903487258171689060ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1069044267686987633ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13027223206962246240ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5359165127817150354ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_be = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 17937453946068425088ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_ack = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2026714863689158972ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15481669047499850498ull);
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_be = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_wdata = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_addr = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_lock = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_we = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_be = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_wdata = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_addr = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_lock = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_we = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__retired = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halt_code = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted = 0;
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_req = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7591483035115508797ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3861274457758713237ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_ack = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 187591933561538096ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_req = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 229135936245166270ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_we = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7464954700227352088ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_lock = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3076235904902541208ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8716902559346911695ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17496261318352541927ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_be = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 4857152785024684862ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_ack = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12825300558179236117ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17002970790377766754ull);
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_be = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_wdata = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_addr = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_lock = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_we = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_be = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_wdata = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_addr = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_lock = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_we = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__retired = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halt_code = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted = 0;
    for (int __Vi0 = 0; __Vi0 < 16384; ++__Vi0) {
        vlSelf->m68k_top__DOT__u_bus__DOT__mem[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8405838060570058610ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->m68k_top__DOT__u_bus__DOT__addr[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12576531322975491979ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->m68k_top__DOT__u_bus__DOT__wdata[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10067475970339129730ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->m68k_top__DOT__u_bus__DOT__be[__Vi0] = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 13240584956950382430ull);
    }
    vlSelf->m68k_top__DOT__u_bus__DOT__lock_pending = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15138474193592913356ull);
    vlSelf->m68k_top__DOT__u_bus__DOT__lock_holder = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 1981839175001340901ull);
    vlSelf->m68k_top__DOT__u_bus__DOT__rrobin = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 3322787106678699999ull);
    vlSelf->m68k_top__DOT__u_bus__DOT__winner = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 2062466938622668690ull);
    vlSelf->m68k_top__DOT__u_bus__DOT__winner_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15951889756196056155ull);
    vlSelf->m68k_top__DOT__u_bus__DOT__granted_valid_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17413588287435569584ull);
    vlSelf->m68k_top__DOT__u_bus__DOT__granted_port_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 16457900450580502902ull);
    vlSelf->m68k_top__DOT__u_bus__DOT__granted_we_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10557294399411994428ull);
    vlSelf->m68k_top__DOT__u_bus__DOT__granted_idx_q = VL_SCOPED_RAND_RESET_I(14, __VscopeHash, 1880420011290920756ull);
    vlSelf->m68k_top__DOT__u_bus__DOT__granted_addr_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5183053770658980584ull);
    vlSelf->m68k_top__DOT__u_bus__DOT__mem_idx = VL_SCOPED_RAND_RESET_I(14, __VscopeHash, 12581664221319265516ull);
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags[__Vi0] = VL_SCOPED_RAND_RESET_I(22, __VscopeHash, 15331459640114581283ull);
    }
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 270301625157390892ull);
    }
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16855472018160228005ull);
    }
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__i = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13601891629530239857ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__hit = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3653208530836844376ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__state = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15775879530670620747ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__saved_we = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4867221686177912054ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__saved_idx = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 1746982036656357867ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__saved_tag = VL_SCOPED_RAND_RESET_I(22, __VscopeHash, 12341969642835784730ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__bus_req_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5408542344127889630ull);
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags[__Vi0] = VL_SCOPED_RAND_RESET_I(22, __VscopeHash, 7809326739462195369ull);
    }
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5990270662109287146ull);
    }
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11653600304805318916ull);
    }
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__i = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7053026826515199119ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__hit = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5016283490496015393ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__state = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13774195019644267643ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__saved_we = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5328580802015836874ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__saved_idx = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 1552866960447858478ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__saved_tag = VL_SCOPED_RAND_RESET_I(22, __VscopeHash, 14303821781672512747ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__bus_req_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2010229896821036312ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16020481905674281019ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_op = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 3509570302857165984ull);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext[__Vi0] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 16685319302389617597ull);
    }
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_words_done = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 16059229529619093326ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12311094860263250824ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 2082399839803365115ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15771592267191603404ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13611496000344173340ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16297860386467870289ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7159414519456822064ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 4175441821923934544ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17235400327796119555ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 16638807308794929269ull);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ext_combo[__Vi0] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 3447384361110267549ull);
    }
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__new_words_done = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 7430928928763589752ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__would_be_complete = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9690520326130498974ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__predict_taken_w = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3848111202111916695ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_target = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1553513897889996578ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_dispatch_w = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8579480039841003181ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1775301411452863763ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2848516405184957319ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 2459172402846325209ull);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[__Vi0] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 15340439289285929389ull);
    }
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_predicted_taken = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7692547426385746850ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 8695887700889890245ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 857000748935301131ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 7428861302836778720ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 16871841364035532441ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 1763647791946249634ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 9704154021560667283ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_imm = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15337542268989636552ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_cc = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 8871667647769019311ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 4849115242436074477ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_ext_words = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 849851043884998480ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_needs_disp_word = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10465052797118471154ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_src_base_idx = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 2347650265680284424ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_reg_idx_full = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 1178045281853077878ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_rb_idx = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 15277536400178923795ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_we = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 491266378947547134ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_widx = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14466124650296700909ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11229283788358663723ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13134890227687567300ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 5684754073221503328ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 180337638647794388ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12225696437036570054ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13312655958631723937ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_pc_next = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9197111273441319170ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 13627531710897571606ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 17748277181462389688ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 1788096212691950293ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 10281980494568481280ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 9580300238786415485ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 3974499783076269100ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_imm32 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11599336987200093469ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_imm32 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1128076741615651586ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_branch_imm = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12098331655749407829ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_imm_raw = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9579368024323132386ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 5351932330032017624ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_ra = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3103529932728462833ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14678010429224696857ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_sp = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4888878959377605902ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_reg_idx_full = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 5650679793980144002ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_predicted_taken = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9296660778998658353ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12460333139019170701ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16220831622657162733ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17124735315975610666ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 95678028887465949ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 7693129251713760457ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_tas_word = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 493022265051438789ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5986269167039382594ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_we = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18078657406370335705ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_lock = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15066506828287034671ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16257046106527848284ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5532382294199559525ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_be = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 1521354702889269228ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9984521577060331944ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15458994363309905600ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9834879594966805944ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v_c = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8824215882897188118ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c_c = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15557264910658247795ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4937768677635872881ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_target_c = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17100606378228139576ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15660870328671667512ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dc_req_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15366467301209315769ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9556978321605845477ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stop_now = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4170067220346013366ull);
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16755703805127666494ull);
    }
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags[__Vi0] = VL_SCOPED_RAND_RESET_I(22, __VscopeHash, 13928500365470587894ull);
    }
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9446295738537720071ull);
    }
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__data[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1650730233674854964ull);
    }
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__i = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12104727819762550413ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__hit = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2186136467594318707ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__state = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7354215942217184852ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__saved_we = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1128450487064343334ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__saved_idx = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 4195787137889750226ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__saved_tag = VL_SCOPED_RAND_RESET_I(22, __VscopeHash, 16306815377045395209ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__bus_req_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9410972839934526363ull);
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags[__Vi0] = VL_SCOPED_RAND_RESET_I(22, __VscopeHash, 5667400804160213795ull);
    }
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9244655406307714458ull);
    }
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10574716576418954518ull);
    }
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__i = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 598278756511275916ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__hit = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6076150988106781723ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__state = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5561499591101304490ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__saved_we = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7829996867010114833ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__saved_idx = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 10789292066938791073ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__saved_tag = VL_SCOPED_RAND_RESET_I(22, __VscopeHash, 3190062297059958672ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__bus_req_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4591789270573786920ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18156314456397220123ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_op = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 11748410581833902187ull);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext[__Vi0] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 2236478983634941459ull);
    }
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_words_done = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 4935074271633872795ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_busy = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9956763139638870010ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_slot = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 15422135636579250966ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_fetch_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12591689790335702973ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_drain = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14456537877687762182ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__redirect_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10057259860613650141ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stall = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5335919798699355251ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__half_word_w = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 3035214453230930484ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_latching = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9689591678738059993ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 13280120219284188916ull);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ext_combo[__Vi0] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 12289675064807724702ull);
    }
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__new_words_done = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 4171697506882046804ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__would_be_complete = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13599157637922473905ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__predict_taken_w = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12598098420429989119ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_fetch_target = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12286787480479704879ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_dispatch_w = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5202597433871533310ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9437543715798565983ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8412602186765959012ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 16877413549216761503ull);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext[__Vi0] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 2536162707529878768ull);
    }
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_predicted_taken = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1478594015558922428ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_kind = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 16697764046214340992ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 1136248919830894486ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 12232054608642476798ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 6123858708898282910ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 894678322286522483ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 3725276958001446753ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_imm = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1949142978139711724ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_cc = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 7186169786924373397ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_ext_words = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 16792927868027322985ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_ext_words = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 15653520556725575973ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_needs_disp_word = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16983681287149674376ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_src_base_idx = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 11524777062208869983ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_reg_idx_full = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 5935223098219215476ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_rb_idx = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 7291330451898195873ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_we = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16570463968268097637ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_widx = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 738011537955289152ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5570403020155543665ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_we = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17404980891112493019ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_idx = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 18035925646410554927ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5334592874669223242ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10695508550545139294ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15849277257909824940ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_pc_next = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8051331626389649661ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 1594371476895597860ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_alu_op = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 9986142904247545462ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 908083765494306994ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 10430347446066714102ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 1677315651825913534ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_reg = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 14067940517596884356ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_imm32 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9618705942299206523ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_imm32 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7465334178926278211ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_branch_imm = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3732647681342387560ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_imm_raw = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13143560627803669104ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 9600298458169002296ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_ra = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 906671123542340003ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_rb = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9225531962653829584ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_sp = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3911940932162243318ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_reg_idx_full = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 17052234181450286118ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_predicted_taken = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6114205517213303606ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6027298066581434907ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5437215414907495381ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8543218398531691025ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18308057117444526103ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 17186517214886757921ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_tas_word = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1951345259329892408ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_mem = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18198386281921804077ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_we = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5421423326666792150ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_lock = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7478017143125726893ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12691698806328291529ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4520180521843995696ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_be = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 9781568645214062862ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_we_c = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3892371602725234503ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4395911331255173041ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6342233956254157824ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v_c = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3297103149049380536ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c_c = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3412678349282696874ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_c = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1636672776919391899ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_target_c = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14058208275848039303ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7138633417207376878ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dc_req_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 561875245692777409ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_settled = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3366342318374970968ull);
    vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stop_now = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4464205672971209913ull);
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15866888831645203832ull);
    }
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__6__w = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__6__pos = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__7__w = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__7__pos = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__11__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__11__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__11__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__16__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__16__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__16__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__18__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__18__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__18__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__19__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__19__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__19__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__20__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__21__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__22__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__23__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__23__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__23__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__24__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__24__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__24__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__31__w = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__31__pos = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__32__w = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__32__pos = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__40__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__40__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__40__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__43__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__43__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__43__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__44__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__44__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__44__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__45__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__45__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__45__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__46__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__46__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__46__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__47__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__47__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__47__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__48__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__48__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__48__r = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__49__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__49__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__49__r = 0;
    vlSelf->__VdfgRegularize_hebeb780c_0_10 = 0;
    vlSelf->__VdfgRegularize_hebeb780c_0_11 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
}
