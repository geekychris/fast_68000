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
    SData/*15:0*/ __VdfgRegularize_hebeb780c_0_1;
    __VdfgRegularize_hebeb780c_0_1 = 0;
    // Body
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_cc = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_needs_disp_word = 0U;
    vlSelfRef.__VdfgRegularize_hebeb780c_0_5 = (((- (IData)(
                                                            (1U 
                                                             & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[0U] 
                                                                >> 0x0000000fU)))) 
                                                 << 0x00000010U) 
                                                | vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[0U]);
    vlSelfRef.m68k_top__DOT__p_we = (((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_we) 
                                      << 1U) | (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_we));
    vlSelfRef.m68k_top__DOT__u_bus__DOT__wdata[0U] 
        = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_wdata;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__wdata[1U] 
        = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_wdata;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__be[0U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_be;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__be[1U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_be;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_lock = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                 >> 3U));
    vlSelfRef.m68k_top__DOT__u_bus__DOT__addr[0U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_addr;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__addr[1U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_addr;
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
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                 >> 9U));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_ea = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_next = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_next = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stop_now 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid) 
           & ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)) 
              & ((~ (IData)(vlSelfRef.halted)) & (3U 
                                                  == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_ea = 0U;
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
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_update = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_update = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c = 0U;
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
    m68k_top__DOT__p_resp_data = vlSelfRef.m68k_top__DOT__u_bus__DOT__mem
        [vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_idx_q];
    vlSelfRef.m68k_top__DOT__p_resp_valid = 0U;
    if (vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_valid_q) {
        vlSelfRef.m68k_top__DOT__p_resp_valid = ((IData)(vlSelfRef.m68k_top__DOT__p_resp_valid) 
                                                 | (3U 
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
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_reg_idx_full 
        = (((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a) 
            << 3U) | (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_we = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_be = 0x0fU;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_wdata = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_addr = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_we_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_idx_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem = 0U;
    m68k_top__DOT__p_req = ((((~ ((IData)(vlSelfRef.m68k_top__DOT__p_resp_valid) 
                                  >> 1U)) & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__bus_req_r)) 
                             << 1U) | ((~ (IData)(vlSelfRef.m68k_top__DOT__p_resp_valid)) 
                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__bus_req_r)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__bus_resp_now 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__state) 
           & ((IData)(vlSelfRef.m68k_top__DOT__p_resp_valid) 
              >> 1U));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__bus_resp_now 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__state) 
           & (IData)(vlSelfRef.m68k_top__DOT__p_resp_valid));
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
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_rdata 
        = (m68k_top__DOT__p_resp_data & (- (IData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__bus_resp_now))));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_rdata 
        = (m68k_top__DOT__p_resp_data & (- (IData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__bus_resp_now))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy) 
           & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain)) 
              & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__bus_resp_now)));
    vlSelfRef.m68k_top__DOT__u_bus__DOT__mem_idx = 
        (0x00003fffU & (vlSelfRef.m68k_top__DOT__u_bus__DOT__addr
                        [vlSelfRef.m68k_top__DOT__u_bus__DOT__winner] 
                        >> 2U));
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
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_target_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w 
        = (0x0000ffffU & ((2U & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_addr)
                           ? m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_rdata
                           : (m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_rdata 
                              >> 0x00000010U)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__new_words_done 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_words_done) 
                 + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching)));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum = 0ULL;
    if ((1U & (~ ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid)) 
                  | (IData)(vlSelfRef.halted))))) {
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
    __VdfgRegularize_hebeb780c_0_1 = (((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot)) 
                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching))
                                       ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w)
                                       : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext[0U]);
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op 
        = (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot)) 
            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching))
            ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w)
            : (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_op));
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
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_wdata 
        = (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
           & (- (IData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled))));
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
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__predict_taken_w 
        = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_bcc_uncond) 
           | ((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__disp32 
               >> 0x0000001fU) & ((~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_bcc_uncond)) 
                                  & (0x0cU == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind)))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__would_be_complete 
        = (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__new_words_done) 
            >= (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__total_words_eff)) 
           & (0U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__new_words_done)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_target 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__predict_taken_w)
            ? ((IData)(2U) + (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc 
                              + m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__disp32))
            : (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc 
               + ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__total_words_eff) 
                  << 1U)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_dispatch_w 
        = ((~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall) 
               | ((IData)(vlSelfRef.halted) | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid)))) 
           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__would_be_complete));
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
    vlSelf->halted = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18300898478658917723ull);
    vlSelf->halt_code_flat = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 837389137883315161ull);
    vlSelf->retired_flat = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9534046761757830170ull);
    vlSelf->m68k_top__DOT__p_we = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 10923809041123312772ull);
    vlSelf->m68k_top__DOT__p_resp_valid = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 15558741892508150942ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6156770161601447316ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_we = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12903487258171689060ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1069044267686987633ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13027223206962246240ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5359165127817150354ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_be = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 17937453946068425088ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15481669047499850498ull);
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_be = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_wdata = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_addr = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_lock = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__bus_we = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_be = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_wdata = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_addr = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_lock = 0;
    vlSelf->m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__bus_we = 0;
    for (int __Vi0 = 0; __Vi0 < 16384; ++__Vi0) {
        vlSelf->m68k_top__DOT__u_bus__DOT__mem[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8405838060570058610ull);
    }
    for (int __Vi0 = 0; __Vi0 < 2; ++__Vi0) {
        vlSelf->m68k_top__DOT__u_bus__DOT__addr[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12576531322975491979ull);
    }
    for (int __Vi0 = 0; __Vi0 < 2; ++__Vi0) {
        vlSelf->m68k_top__DOT__u_bus__DOT__wdata[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10067475970339129730ull);
    }
    for (int __Vi0 = 0; __Vi0 < 2; ++__Vi0) {
        vlSelf->m68k_top__DOT__u_bus__DOT__be[__Vi0] = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 13240584956950382430ull);
    }
    vlSelf->m68k_top__DOT__u_bus__DOT__lock_pending = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15138474193592913356ull);
    vlSelf->m68k_top__DOT__u_bus__DOT__lock_holder = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1981839175001340901ull);
    vlSelf->m68k_top__DOT__u_bus__DOT__rrobin = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3322787106678699999ull);
    vlSelf->m68k_top__DOT__u_bus__DOT__winner = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2062466938622668690ull);
    vlSelf->m68k_top__DOT__u_bus__DOT__winner_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15951889756196056155ull);
    vlSelf->m68k_top__DOT__u_bus__DOT__granted_valid_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17413588287435569584ull);
    vlSelf->m68k_top__DOT__u_bus__DOT__granted_port_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16457900450580502902ull);
    vlSelf->m68k_top__DOT__u_bus__DOT__granted_idx_q = VL_SCOPED_RAND_RESET_I(14, __VscopeHash, 1880420011290920756ull);
    vlSelf->m68k_top__DOT__u_bus__DOT__mem_idx = VL_SCOPED_RAND_RESET_I(14, __VscopeHash, 12581664221319265516ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__state = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17518024137640730380ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__bus_req_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18156036509064856705ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_ic__DOT__bus_resp_now = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12924416066569193321ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__state = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17381672416326318707ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__bus_req_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2043381549482322003ull);
    vlSelf->m68k_top__DOT__g_core__BRA__0__KET____DOT__g_passthrough__DOT__u_dc__DOT__bus_resp_now = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3548537493704750ull);
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
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__5__w = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__5__pos = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__6__w = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__6__pos = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__10__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__10__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__10__r = 0;
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
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__17__Vfuncout = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__17__m = 0;
    vlSelf->__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_id__DOT__ea_ext__17__r = 0;
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
    vlSelf->__VdfgRegularize_hebeb780c_0_5 = 0;
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
