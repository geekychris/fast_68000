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
    IData/*31:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__old_w;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__old_w = 0;
    IData/*31:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__new_w;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__new_w = 0;
    CData/*3:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__be;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__be = 0;
    CData/*3:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__11__Vfuncout;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__11__Vfuncout = 0;
    CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__11__pos;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__11__pos = 0;
    IData/*31:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__12__w;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__12__w = 0;
    CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__12__pos;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__12__pos = 0;
    IData/*31:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__13__Vfuncout;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__13__Vfuncout = 0;
    CData/*7:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__13__b;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__13__b = 0;
    CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__13__pos;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__13__pos = 0;
    IData/*31:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__old_val;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__old_val = 0;
    IData/*31:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__new_val;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__new_val = 0;
    CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__sz;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__sz = 0;
    CData/*0:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__is_areg;
    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__is_areg = 0;
    IData/*31:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__old_w;
    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__old_w = 0;
    IData/*31:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__new_w;
    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__new_w = 0;
    CData/*3:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__be;
    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__be = 0;
    CData/*3:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__be_for_byte__412__Vfuncout;
    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__be_for_byte__412__Vfuncout = 0;
    CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__be_for_byte__412__pos;
    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__be_for_byte__412__pos = 0;
    IData/*31:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__413__w;
    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__413__w = 0;
    CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__413__pos;
    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__413__pos = 0;
    IData/*31:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_into_word__414__Vfuncout;
    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_into_word__414__Vfuncout = 0;
    CData/*7:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_into_word__414__b;
    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_into_word__414__b = 0;
    CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_into_word__414__pos;
    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_into_word__414__pos = 0;
    IData/*31:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__old_val;
    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__old_val = 0;
    IData/*31:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__new_val;
    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__new_val = 0;
    CData/*1:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__sz;
    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__sz = 0;
    CData/*0:0*/ __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__is_areg;
    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__is_areg = 0;
    CData/*0:0*/ __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain;
    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain = 0;
    CData/*0:0*/ __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy;
    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy = 0;
    CData/*1:0*/ __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state;
    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state = 0;
    CData/*0:0*/ __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_drain;
    __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_drain = 0;
    CData/*0:0*/ __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_busy;
    __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_busy = 0;
    CData/*1:0*/ __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state;
    __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state = 0;
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
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v0;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v0 = 0;
    IData/*21:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v0;
    __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v0 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v0;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v0 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v1;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v1 = 0;
    IData/*31:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data__v0;
    __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data__v0 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data__v0;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data__v0 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v1;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v1 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v2;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v2 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data__v1;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data__v1 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v0;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v0 = 0;
    IData/*21:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v0;
    __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v0 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v0;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v0 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v1;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v1 = 0;
    IData/*31:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v0;
    __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v0 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v0;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v0 = 0;
    IData/*31:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v1;
    __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v1 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v1;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v1 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v1;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v1 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v2;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v2 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v2;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v2 = 0;
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
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v0;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v0 = 0;
    IData/*21:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v0;
    __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v0 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v0;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v0 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v1;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v1 = 0;
    IData/*31:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__data__v0;
    __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__data__v0 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__data__v0;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__data__v0 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v1;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v1 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v2;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v2 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__data__v1;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__data__v1 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v0;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v0 = 0;
    IData/*21:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v0;
    __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v0 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v0;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v0 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v1;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v1 = 0;
    IData/*31:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v0;
    __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v0 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v0;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v0 = 0;
    IData/*31:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v1;
    __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v1 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v1;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v1 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v1;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v1 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v2;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v2 = 0;
    CData/*7:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v2;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v2 = 0;
    SData/*15:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext__v0;
    __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext__v0 = 0;
    CData/*1:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext__v0;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext__v0 = 0;
    CData/*0:0*/ __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext__v0;
    __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext__v0 = 0;
    CData/*0:0*/ __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext__v1;
    __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext__v1 = 0;
    SData/*15:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v0;
    __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v0 = 0;
    CData/*0:0*/ __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v0;
    __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v0 = 0;
    SData/*15:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v1;
    __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v1 = 0;
    SData/*15:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v2;
    __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v2 = 0;
    SData/*15:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v3;
    __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v3 = 0;
    CData/*0:0*/ __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v4;
    __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v4 = 0;
    IData/*31:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0;
    __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0 = 0;
    CData/*3:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0 = 0;
    CData/*0:0*/ __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0;
    __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0 = 0;
    IData/*31:0*/ __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1;
    __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1 = 0;
    CData/*3:0*/ __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1;
    __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1 = 0;
    CData/*0:0*/ __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1;
    __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1 = 0;
    CData/*0:0*/ __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v2;
    __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v2 = 0;
    // Body
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v0 = 0U;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v4 = 0U;
    __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v0 = 0U;
    __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v4 = 0U;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0 = 0U;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1 = 0U;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v2 = 0U;
    __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0 = 0U;
    __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1 = 0U;
    __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v2 = 0U;
    vlSelfRef.__Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_pc 
        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_pc;
    vlSelfRef.__Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_pc 
        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_pc;
    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state 
        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state;
    __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state 
        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state;
    vlSelfRef.__Vdly__m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted 
        = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted;
    vlSelfRef.__Vdly__m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted 
        = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted;
    __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v0 = 0U;
    __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v1 = 0U;
    __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v2 = 0U;
    __VdlySet__m68k_top__DOT__u_bus__DOT__mem__v3 = 0U;
    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain 
        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain;
    __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_drain 
        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_drain;
    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy 
        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy;
    __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_busy 
        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_busy;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v0 = 0U;
    __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v1 = 0U;
    __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext__v0 = 0U;
    __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext__v1 = 0U;
    if ((1U & (~ (IData)(vlSelfRef.rst_n)))) {
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_wdata = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_wdata = 0U;
    }
    vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_we_q 
        = ((IData)(vlSelfRef.rst_n) && ((IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid) 
                                        & ((IData)(vlSelfRef.m68k_top__DOT__p_we) 
                                           >> (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner))));
    vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_valid_q 
        = ((IData)(vlSelfRef.rst_n) && (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid));
    if (vlSelfRef.rst_n) {
        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall)))) {
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
                    vlSelfRef.__Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_pc 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc;
                }
            }
        }
        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__redirect_valid)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stall)))) {
                if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_dispatch_w) {
                    __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v0 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ext_combo[0U];
                    __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v0 = 1U;
                    __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v1 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ext_combo[1U];
                    __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v2 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ext_combo[2U];
                    __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v3 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ext_combo[3U];
                    vlSelfRef.__Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_pc 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_pc;
                }
            }
        }
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
            __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__is_areg 
                = (1U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_widx) 
                         >> 3U));
            __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__sz 
                = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled)
                    ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_size)
                    : 2U);
            __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__new_val 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_wdata;
            __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__old_val 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs
                [vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_widx];
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT____VlemCall_0__sized_write 
                = ((IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__is_areg)
                    ? __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__new_val
                    : ((0U == (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__sz))
                        ? ((0xffffff00U & __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__old_val) 
                           | (0x000000ffU & __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__new_val))
                        : ((1U == (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__sz))
                            ? ((0xffff0000U & __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__old_val) 
                               | (0x0000ffffU & __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__new_val))
                            : __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__400__new_val)));
            __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT____VlemCall_0__sized_write;
            __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_widx;
            __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1 = 1U;
        }
        if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_we) 
             & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_we) 
                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_idx) 
                      == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_widx)))))) {
            __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_data;
            __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_idx;
            __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0 = 1U;
        }
        if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_we) {
            __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__is_areg 
                = (1U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_widx) 
                         >> 3U));
            __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__sz 
                = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_settled)
                    ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_size)
                    : 2U);
            __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__new_val 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_wdata;
            __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__old_val 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs
                [vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_widx];
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT____VlemCall_0__sized_write 
                = ((IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__is_areg)
                    ? __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__new_val
                    : ((0U == (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__sz))
                        ? ((0xffffff00U & __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__old_val) 
                           | (0x000000ffU & __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__new_val))
                        : ((1U == (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__sz))
                            ? ((0xffff0000U & __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__old_val) 
                               | (0x0000ffffU & __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__new_val))
                            : __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__sized_write__801__new_val)));
            __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT____VlemCall_0__sized_write;
            __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_widx;
            __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1 = 1U;
        }
        if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled) 
             & (3U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__retired 
                = ((IData)(1U) + vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__retired);
        }
        if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_settled) 
             & (3U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__retired 
                = ((IData)(1U) + vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__retired);
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
    } else {
        __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext__v4 = 1U;
        __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v4 = 1U;
        __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_rf__DOT__regs__v2 = 1U;
        __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v2 = 1U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__retired = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__retired = 0U;
        vlSelfRef.__Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_pc = 0U;
        vlSelfRef.__Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_pc = 0U;
    }
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
    if (vlSelfRef.rst_n) {
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
        if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_settled) 
             & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_we_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c_c;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v_c;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c;
        }
        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_reg_idx_full 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_reg_idx_full;
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
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg;
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
            }
        }
        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__redirect_valid)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stall)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_reg_idx_full 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_reg_idx_full;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_sp 
                    = (((0x0fU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_widx)) 
                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_we))
                        ? vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_wdata
                        : (((0x0fU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_idx)) 
                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_we))
                            ? vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_data
                            : vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[15U]));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_cc;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_predicted_taken 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_predicted_taken;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_alu_op 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_reg 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_rb 
                    = ((((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_rb_idx) 
                         == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_widx)) 
                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_we))
                        ? vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_wdata
                        : ((((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_rb_idx) 
                             == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_idx)) 
                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_we))
                            ? vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_data
                            : vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs
                           [vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_rb_idx]));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_imm32 
                    = ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_ext_words))
                        ? (((- (IData)((1U & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext
                                              [vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_ext_words] 
                                              >> 0x0000000fU)))) 
                            << 0x00000010U) | vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext
                           [vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_ext_words])
                        : (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext
                                    [vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_ext_words]) 
                            << 0x00000010U) | vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext
                           [(3U & ((IData)(1U) + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_ext_words)))]));
            }
        }
        if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled) 
             & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_x_we_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_x 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_x_c;
        }
        if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_settled) 
             & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_x_we_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_x 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_x_c;
        }
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_reg_idx_full = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_reg_idx_full = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_sp = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_sp = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_x = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_x = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_cc = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_predicted_taken = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_predicted_taken = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_alu_op = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_reg = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_rb = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_imm32 = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_imm32 = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_ra = 0U;
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
    if (vlSelfRef.rst_n) {
        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__redirect_valid)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stall)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_ra 
                    = ((((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_src_base_idx) 
                         == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_widx)) 
                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_we))
                        ? vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_wdata
                        : ((((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_src_base_idx) 
                             == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_idx)) 
                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_we))
                            ? vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_aux_data
                            : vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs
                           [vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_src_base_idx]));
            }
        }
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_ra = 0U;
    }
    if (__VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[__VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0] 
            = __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v0;
    }
    if (__VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[__VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1] 
            = __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v1;
    }
    if (__VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs__v2) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[0U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[1U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[2U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[3U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[4U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[5U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[6U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[7U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[8U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[9U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[10U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[11U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[12U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[13U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[14U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[15U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_rf__DOT__regs[15U] = 0x00005000U;
    }
    if (vlSelfRef.rst_n) {
        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_imm32 
                    = ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_ext_words))
                        ? vlSelfRef.__VdfgRegularize_hebeb780c_0_18
                        : (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[0U]) 
                            << 0x00000010U) | vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_ext[1U]));
            }
        }
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_imm32 = 0U;
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
    if (vlSelfRef.rst_n) {
        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__redirect_valid)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stall)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_imm32 
                    = ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_ext_words))
                        ? vlSelfRef.__VdfgRegularize_hebeb780c_0_19
                        : (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext[0U]) 
                            << 0x00000010U) | vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext[1U]));
            }
        }
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_imm32 = 0U;
    }
    if (__VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v0) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext[0U] 
            = __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v0;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext[1U] 
            = __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v1;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext[2U] 
            = __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v2;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext[3U] 
            = __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v3;
    }
    if (__VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext__v4) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext[0U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext[1U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext[2U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_ext[3U] = 0U;
    }
    if (vlSelfRef.rst_n) {
        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall)))) {
                if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_dispatch_w) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op;
                }
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode;
            }
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted)))) {
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
        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__redirect_valid)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stall)))) {
                if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_dispatch_w) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__effective_op;
                }
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode;
            }
        }
        vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_idx_q 
            = vlSelfRef.m68k_top__DOT__u_bus__DOT__mem_idx;
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg = 0U;
        vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_idx_q = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_op = 0U;
        __VdlySet__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext__v1 = 1U;
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
    if (vlSelfRef.rst_n) {
        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__redirect_valid)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted)))) {
                if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_latching) {
                    if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_slot))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_op 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__half_word_w;
                    } else if ((4U >= (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_slot))) {
                        __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext__v0 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__half_word_w;
                        __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext__v0 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_slot) 
                                     - (IData)(1U)));
                        __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext__v0 = 1U;
                    }
                }
            }
        }
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_op = 0U;
        __VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext__v1 = 1U;
    }
    if (__VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext__v0) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext[__VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext__v0] 
            = __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext__v0;
    }
    if (__VdlySet__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext__v1) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext[0U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext[1U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext[2U] = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_ext[3U] = 0U;
    }
    vlSelfRef.retired_flat = (((QData)((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__retired)) 
                               << 0x00000020U) | (QData)((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__retired)));
    vlSelfRef.m68k_top__DOT__u_bus__DOT__wdata[0U] 
        = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_wdata;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__wdata[2U] 
        = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_wdata;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_cc = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                 >> 3U));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                 >> 6U));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                 >> 9U));
    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                  >> 0x0000000fU)))) {
        if ((0x00004000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
            if ((0x00002000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                              >> 0x0000000cU)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_cc 
                        = (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                          >> 8U));
                }
            } else if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_cc 
                            = (0x0000000fU & ((0x00000020U 
                                               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                               ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                  >> 8U)
                                               : ((0x00000010U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                   >> 8U)
                                                   : 
                                                  ((8U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                    >> 8U)
                                                    : 
                                                   ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                    >> 8U)))));
                    }
                }
            }
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                          >> 0x0000000dU)))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                              >> 0x0000000cU)))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                  >> 0x0000000bU)))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                      >> 0x0000000aU)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                          >> 9U)))) {
                                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                              >> 8U)))) {
                                    if ((0x00000080U 
                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                        if ((0x00000040U 
                                             & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                                = (7U 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                      >> 3U));
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                                = (7U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else if ((0x00002000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
            if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                >> 0x0cU))) | (2U == 
                                               (3U 
                                                & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                   >> 0x0cU)))) 
                 | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 0x0cU))))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 6U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 9U));
            }
        } else if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
            if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                >> 0x0cU))) | (2U == 
                                               (3U 
                                                & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                   >> 0x0cU)))) 
                 | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 0x0cU))))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 6U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 9U));
            }
        } else if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
            if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                  >> 8U)))) {
                        if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                            >> 0x0cU))) 
                              | (2U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                              >> 0x0cU)))) 
                             | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 0x0cU))))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 6U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    }
                } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 8U)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                }
            } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                              >> 8U)))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                                = (7U 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                      >> 3U));
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                                = (7U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                                        } else if (
                                                   (1U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                                = (7U 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                      >> 3U));
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                                = (7U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                                        } else {
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                                = (7U 
                                                   & 6U);
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                                = (7U 
                                                   & 1U);
                                        }
                                    } else {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                            = (7U & 
                                               ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                >> 3U));
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                                    }
                                } else {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 3U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                                }
                            } else {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                            }
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                        }
                    } else if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                    if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                            = (7U & 
                                               ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                >> 3U));
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                                    } else if ((1U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                            = (7U & 
                                               ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                >> 3U));
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                                    } else {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                            = (7U & 6U);
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                            = (7U & 0U);
                                    }
                                } else {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 3U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                                }
                            } else {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                            }
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    }
                }
            } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 8U)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
            }
        } else if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                          >> 8U)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
            }
        } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                          >> 8U)))) {
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                    if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                            = (7U & 
                                               ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                >> 3U));
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                                    } else if ((1U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                            = (7U & 
                                               ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                >> 3U));
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                                    } else {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                            = (7U & 6U);
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                            = (7U & 1U);
                                    }
                                } else {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 3U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                                }
                            } else {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                            }
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    }
                } else if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 3U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                                } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 3U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                                } else {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & 6U);
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & 0U);
                                }
                            } else {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                            }
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 8U)))) {
            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
            } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 3U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                                } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 3U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                                } else {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & 6U);
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & 1U);
                                }
                            } else {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                            }
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                }
            } else if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                            } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                            } else {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                    = (7U & 6U);
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                    = (7U & 0U);
                            }
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                }
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
            }
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                 >> 9U));
    if ((0x00008000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
        if ((0x00004000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
            if ((0x00002000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                              >> 0x0000000cU)))) {
                    if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                            = ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                ? ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                    ? ((0x00000100U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                        ? 0x14U : 0x15U)
                                    : ((0x00000100U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                        ? 0x16U : 0x17U))
                                : ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                    ? ((0x00000100U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                        ? 0x12U : 0x13U)
                                    : ((0x00000100U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                        ? 0x10U : 0x11U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode = 0U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                            = ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                ? ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                    ? 0x14U : 0x16U)
                                : ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                    ? 0x12U : 0x10U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode = 0U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                                = ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                    ? ((0x00000200U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                        ? 0x15U : 0x17U)
                                    : ((0x00000200U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                        ? 0x13U : 0x11U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                                = ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                    ? ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                        ? 0x15U : 0x17U)
                                    : ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                        ? 0x13U : (
                                                   (0x00000100U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 0x10U
                                                    : 0x11U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode = 0U;
                        }
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                      >> 6U)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                            = ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                ? ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                    ? 0x15U : 0x17U)
                                : ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                    ? 0x13U : ((0x00000100U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                ? 0x10U
                                                : 0x11U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode = 0U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                }
            } else {
                if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 1U;
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U));
                            } else {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U));
                            }
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                      >> 6U)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 3U;
                        }
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 3U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                  >> 6U)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 3U;
                    }
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 3U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 9U));
                }
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
            }
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                          >> 0x0000000dU)))) {
                if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                            }
                        }
                    } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                        }
                    }
                }
            }
        } else if ((0x00002000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
            if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 6U;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 5U;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 5U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 6U;
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                }
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
            }
        } else {
            if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 2U;
                if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 9U));
                }
            } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                  >> 6U)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 4U;
                    }
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 4U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 9U));
                }
            } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                              >> 6U)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 4U;
                }
                if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 9U));
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 9U));
                }
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 4U;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 9U));
            }
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
        }
    } else if ((0x00004000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
        if ((0x00002000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
            if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                              >> 8U)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 9U));
                }
            }
        } else {
            if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                  >> 6U)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                            = ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                ? 2U : 1U);
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                        = ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                            ? 2U : 1U);
                }
            } else if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                                }
                            }
                        }
                    } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                            }
                        }
                    }
                } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                            }
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                            = ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                ? ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                    ? 7U : 0x0cU) : 0x0cU);
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                        }
                    }
                } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                      >> 5U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                          >> 4U)))) {
                                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                              >> 3U)))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 0x0eU;
                                }
                            }
                        }
                    } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 5U)))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                      >> 4U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                          >> 3U)))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 0x0dU;
                            }
                        }
                    }
                } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                  >> 5U)))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                      >> 4U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                          >> 3U)))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 0x0fU;
                            }
                        }
                    }
                }
            } else if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                            }
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 0x0aU;
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                        }
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 9U;
                }
            } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                        }
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 0x0bU;
                }
            } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                    }
                }
            }
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                          >> 0x0000000cU)))) {
                if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                        = (7U & ((0x00000040U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                  ? 
                                                 ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                  >> 9U)
                                                  : 
                                                 ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                  >> 9U)));
                                }
                            }
                        } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                    = (7U & ((0x00000040U 
                                              & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                              ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 9U)
                                              : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 9U)));
                            }
                        }
                    } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                    = (7U & ((0x00000040U 
                                              & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                              ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 9U)
                                              : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 9U)));
                            }
                        }
                    } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((0x00000040U 
                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                          ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U)
                                          : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U)));
                        }
                    }
                } else if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                    = (7U & ((0x00000040U 
                                              & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                              ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 9U)
                                              : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 9U)));
                            }
                        }
                    } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((0x00000040U 
                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                          ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U)
                                          : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U)));
                        }
                    }
                } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((0x00000040U 
                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                          ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U)
                                          : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U)));
                        }
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                      ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U) : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                   >> 9U)));
                    }
                }
            }
        }
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                      >> 0x0000000dU)))) {
            if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                        } else if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                        } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                }
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
            } else if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                    = (7U & ((0x00000040U 
                                              & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                              ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 3U)
                                              : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 3U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                            }
                        } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                        } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                              >> 4U)))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                                }
                            } else if ((0x00000010U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                            }
                        }
                    } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((0x00000040U 
                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                          ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U)
                                          : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                        }
                    } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    }
                } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((0x00000040U 
                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                          ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U)
                                          : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                      ? ((0x00000040U 
                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                          ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U)
                                          : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U))
                                      : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                      ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U) : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                   >> 3U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    }
                } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                        } else if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                        } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                        }
                    } else if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                    } else if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                    } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                    }
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                    } else if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                    } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                    }
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                }
            } else if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((0x00000040U 
                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                          ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U)
                                          : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                      ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U) : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                   >> 3U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                }
            } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                      ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U) : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                   >> 3U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                }
            } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                  ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U) : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                               >> 3U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                }
            }
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                          >> 0x0000000cU)))) {
                if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                    if ((0x00000040U 
                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                                    }
                                }
                            }
                        } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                                }
                            }
                        }
                    } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                                }
                            }
                        }
                    } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                            }
                        }
                    }
                } else if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                                }
                            }
                        }
                    } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                            }
                        }
                    }
                } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                            }
                        }
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                        }
                    }
                }
            }
        }
    } else {
        if ((0x00002000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
            if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                >> 0x0cU))) | (2U == 
                                               (3U 
                                                & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                   >> 0x0cU)))) 
                 | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 0x0cU))))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
            }
        } else if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
            if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                >> 0x0cU))) | (2U == 
                                               (3U 
                                                & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                   >> 0x0cU)))) 
                 | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 0x0cU))))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
            }
        } else if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
            if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 6U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    } else if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                               >> 0x0cU))) 
                                 | (2U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                 >> 0x0cU)))) 
                                | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                                >> 0x0cU))))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 6U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                        = ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                            ? ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                ? ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                    ? 0U : 6U) : ((0x00000200U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                   ? 5U
                                                   : 0U))
                            : ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                ? ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                    ? 1U : 2U) : ((0x00000200U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                   ? 3U
                                                   : 4U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode = 7U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg = 4U;
                }
            } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 6U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                        = ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                            ? ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                ? ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                    ? 0U : 5U) : ((0x00000400U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                   ? 1U
                                                   : 3U))
                            : ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                ? ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                    ? ((0x00000010U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                        ? ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                            ? ((4U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (2U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000800U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 6U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 5U)
                                                       : 0U))
                                                     : 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 1U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 2U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 3U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 4U))))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000800U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 6U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 5U)
                                                        : 0U))
                                                      : 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 1U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 2U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 3U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 4U))))
                                                     : 5U))
                                                : (
                                                   (0x00000800U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 6U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 5U)
                                                      : 0U))
                                                    : 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 1U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 2U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 3U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 4U)))))
                                            : ((0x00000800U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 6U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 5U)
                                                     : 0U))
                                                : (
                                                   (0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 1U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 2U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 3U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 4U)))))
                                        : ((0x00000800U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                            ? ((0x00000400U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 0U
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 6U))
                                                : 5U)
                                            : ((0x00000400U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                ? 1U
                                                : (
                                                   (0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 3U)
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 4U)))))
                                    : ((0x00000800U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                        ? ((0x00000400U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                            ? 0U : 5U)
                                        : ((0x00000400U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                            ? 1U : 
                                           ((0x00000200U 
                                             & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                             ? ((0x00000100U 
                                                 & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                 ? 0U
                                                 : 3U)
                                             : ((0x00000100U 
                                                 & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                 ? 0U
                                                 : 4U)))))
                                : ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                    ? ((0x00000010U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                        ? ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                            ? ((4U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (2U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000800U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 6U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 5U)
                                                       : 0U))
                                                     : 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 1U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 2U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 3U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 4U))))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000800U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 6U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 5U)
                                                        : 0U))
                                                      : 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 1U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 2U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 3U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 4U))))
                                                     : 5U))
                                                : (
                                                   (0x00000800U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 6U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 5U)
                                                      : 0U))
                                                    : 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 1U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 2U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 3U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 4U)))))
                                            : ((0x00000800U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 6U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 5U)
                                                     : 0U))
                                                : (
                                                   (0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 1U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 2U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 3U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 4U)))))
                                        : ((0x00000800U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                            ? ((0x00000400U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                ? 0U
                                                : (
                                                   (0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 5U)
                                                    : 0U))
                                            : ((0x00000400U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 1U)
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 2U))
                                                : (
                                                   (0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 3U)
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 4U)))))
                                    : ((0x00000800U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                        ? ((0x00000400U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                            ? 0U : 
                                           ((0x00000200U 
                                             & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                             ? ((0x00000100U 
                                                 & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                 ? 0U
                                                 : 5U)
                                             : 0U))
                                        : ((0x00000400U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                            ? ((0x00000200U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (0x00000100U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 0U
                                                    : 1U)
                                                : (
                                                   (0x00000100U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 0U
                                                    : 2U))
                                            : ((0x00000200U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (0x00000100U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 0U
                                                    : 3U)
                                                : (
                                                   (0x00000100U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 0U
                                                    : 4U)))))));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & 7U);
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & 4U);
                }
            } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 6U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 6U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                    = (7U & 7U);
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & 4U);
            }
        } else if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
            if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 6U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                    = ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                        ? ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                            ? 0U : 6U) : ((0x00000200U 
                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                           ? 1U : 2U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                    = (7U & 7U);
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & 4U);
            }
        } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
            if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 6U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                    = ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                        ? ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                            ? 5U : 3U) : ((0x00000040U 
                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                           ? ((0x00000020U 
                                               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                               ? ((0x00000010U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((8U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((4U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((2U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000800U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000400U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 6U))
                                                        : 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 5U)
                                                         : 0U))
                                                       : 
                                                      ((0x00000400U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 1U)
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 2U))
                                                        : 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 3U)
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 4U))))
                                                      : 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000800U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000400U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000200U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 6U))
                                                         : 
                                                        ((0x00000200U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 5U)
                                                          : 0U))
                                                        : 
                                                       ((0x00000400U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000200U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 1U)
                                                          : 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 2U))
                                                         : 
                                                        ((0x00000200U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 3U)
                                                          : 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 4U))))
                                                       : 3U))
                                                     : 
                                                    ((0x00000800U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 6U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 5U)
                                                        : 0U))
                                                      : 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 1U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 2U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 3U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 4U)))))
                                                    : 
                                                   ((0x00000800U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 6U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 5U)
                                                       : 0U))
                                                     : 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 1U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 2U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 3U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 4U)))))
                                                   : 
                                                  ((0x00000800U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 5U
                                                    : 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 1U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 2U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 3U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 4U)))))
                                               : ((0x00000800U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                   ? 5U
                                                   : 
                                                  ((0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 1U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 2U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 3U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 4U)))))
                                           : ((0x00000020U 
                                               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                               ? ((0x00000010U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((8U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((4U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((2U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000800U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000400U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 6U))
                                                        : 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 5U)
                                                         : 0U))
                                                       : 
                                                      ((0x00000400U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 1U)
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 2U))
                                                        : 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 3U)
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 4U))))
                                                      : 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000800U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000400U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000200U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 6U))
                                                         : 
                                                        ((0x00000200U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 5U)
                                                          : 0U))
                                                        : 
                                                       ((0x00000400U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000200U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 1U)
                                                          : 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 2U))
                                                         : 
                                                        ((0x00000200U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 3U)
                                                          : 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 4U))))
                                                       : 3U))
                                                     : 
                                                    ((0x00000800U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 6U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 5U)
                                                        : 0U))
                                                      : 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 1U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 2U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 3U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 4U)))))
                                                    : 
                                                   ((0x00000800U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 6U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 5U)
                                                       : 0U))
                                                     : 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 1U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 2U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 3U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 4U)))))
                                                   : 
                                                  ((0x00000800U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 6U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 5U)
                                                      : 0U))
                                                    : 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 1U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 2U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 3U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 4U)))))
                                               : ((0x00000800U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 6U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 5U)
                                                     : 0U))
                                                   : 
                                                  ((0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 1U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 2U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 3U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 4U)))))));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                    = (7U & 7U);
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & 4U);
            }
        } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                         >> 6U));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                         >> 3U));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op));
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_alu_op 
                = ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                    ? ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                        ? 0U : 4U) : ((0x00000040U 
                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                       ? ((0x00000020U 
                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                           ? ((0x00000010U 
                                               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                               ? ((8U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((4U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((2U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000800U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 6U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 5U)
                                                        : 0U))
                                                      : 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 1U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 2U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 3U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 4U))))
                                                     : 
                                                    ((1U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000800U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000400U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 6U))
                                                        : 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 5U)
                                                         : 0U))
                                                       : 
                                                      ((0x00000400U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 1U)
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 2U))
                                                        : 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 3U)
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 4U))))
                                                      : 4U))
                                                    : 
                                                   ((0x00000800U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 6U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 5U)
                                                       : 0U))
                                                     : 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 1U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 2U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 3U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 4U)))))
                                                   : 
                                                  ((0x00000800U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 6U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 5U)
                                                      : 0U))
                                                    : 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 1U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 2U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 3U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 4U)))))
                                               : ((0x00000800U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 6U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 5U)
                                                     : 0U))
                                                   : 
                                                  ((0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 1U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 2U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 3U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 4U)))))
                                           : ((0x00000800U 
                                               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                               ? ((0x00000400U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 0U
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 6U))
                                                   : 
                                                  ((0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 5U)
                                                    : 0U))
                                               : ((0x00000400U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 1U)
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 2U))
                                                   : 
                                                  ((0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 3U)
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 4U)))))
                                       : ((0x00000020U 
                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                           ? ((0x00000010U 
                                               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                               ? ((8U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((4U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((2U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000800U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 6U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 5U)
                                                        : 0U))
                                                      : 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 1U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 2U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 3U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 4U))))
                                                     : 
                                                    ((1U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000800U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000400U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 6U))
                                                        : 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 5U)
                                                         : 0U))
                                                       : 
                                                      ((0x00000400U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 1U)
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 2U))
                                                        : 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 3U)
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 4U))))
                                                      : 4U))
                                                    : 
                                                   ((0x00000800U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 6U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 5U)
                                                       : 0U))
                                                     : 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 1U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 2U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 3U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 4U)))))
                                                   : 
                                                  ((0x00000800U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 6U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 5U)
                                                      : 0U))
                                                    : 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 1U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 2U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 3U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 4U)))))
                                               : ((0x00000800U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 6U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 5U)
                                                     : 0U))
                                                   : 
                                                  ((0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 1U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 2U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 3U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 4U)))))
                                           : ((0x00000800U 
                                               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                               ? ((0x00000400U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 0U
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 6U))
                                                   : 
                                                  ((0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 5U)
                                                    : 0U))
                                               : ((0x00000400U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 1U)
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 2U))
                                                   : 
                                                  ((0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 3U)
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 4U)))))));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_mode 
                = (7U & 7U);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_src_reg 
                = (7U & 4U);
        }
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                      >> 0x0000000dU)))) {
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                          >> 0x0000000cU)))) {
                if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U));
                            }
                        } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_op) 
                                 >> 9U));
                }
            }
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_cc = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                 >> 3U));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                 >> 6U));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                 >> 9U));
    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                  >> 0x0000000fU)))) {
        if ((0x00004000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
            if ((0x00002000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                              >> 0x0000000cU)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_cc 
                        = (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                          >> 8U));
                }
            } else if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_cc 
                            = (0x0000000fU & ((0x00000020U 
                                               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                               ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                  >> 8U)
                                               : ((0x00000010U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                   >> 8U)
                                                   : 
                                                  ((8U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                    >> 8U)
                                                    : 
                                                   ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                    >> 8U)))));
                    }
                }
            }
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                          >> 0x0000000dU)))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                              >> 0x0000000cU)))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                  >> 0x0000000bU)))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                      >> 0x0000000aU)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                          >> 9U)))) {
                                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                              >> 8U)))) {
                                    if ((0x00000080U 
                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                        if ((0x00000040U 
                                             & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                                = (7U 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                      >> 3U));
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                                = (7U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else if ((0x00002000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
            if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                >> 0x0cU))) | (2U == 
                                               (3U 
                                                & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                   >> 0x0cU)))) 
                 | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 0x0cU))))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 6U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 9U));
            }
        } else if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
            if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                >> 0x0cU))) | (2U == 
                                               (3U 
                                                & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                   >> 0x0cU)))) 
                 | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 0x0cU))))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 6U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 9U));
            }
        } else if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
            if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                  >> 8U)))) {
                        if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                            >> 0x0cU))) 
                              | (2U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                              >> 0x0cU)))) 
                             | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 0x0cU))))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 6U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    }
                } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 8U)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                }
            } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                              >> 8U)))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                    } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                                = (7U 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                      >> 3U));
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                                = (7U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                                        } else if (
                                                   (1U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                                = (7U 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                      >> 3U));
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                                = (7U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                                        } else {
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                                = (7U 
                                                   & 6U);
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                                = (7U 
                                                   & 1U);
                                        }
                                    } else {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                            = (7U & 
                                               ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                >> 3U));
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                                    }
                                } else {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 3U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                                }
                            } else {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                            }
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                        }
                    } else if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                    if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                            = (7U & 
                                               ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                >> 3U));
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                                    } else if ((1U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                            = (7U & 
                                               ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                >> 3U));
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                                    } else {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                            = (7U & 6U);
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                            = (7U & 0U);
                                    }
                                } else {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 3U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                                }
                            } else {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                            }
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                    }
                }
            } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 8U)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
            }
        } else if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                          >> 8U)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
            }
        } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                          >> 8U)))) {
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                    if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                            = (7U & 
                                               ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                >> 3U));
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                                    } else if ((1U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                            = (7U & 
                                               ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                >> 3U));
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                                    } else {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                            = (7U & 6U);
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                            = (7U & 1U);
                                    }
                                } else {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 3U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                                }
                            } else {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                            }
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                    }
                } else if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 3U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                                } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 3U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                                } else {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & 6U);
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & 0U);
                                }
                            } else {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                            }
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 8U)))) {
            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
            } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 3U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                                } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 3U));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                                } else {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                        = (7U & 6U);
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                        = (7U & 1U);
                                }
                            } else {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                            }
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                }
            } else if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                            } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                            } else {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                    = (7U & 6U);
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                    = (7U & 0U);
                            }
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                }
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_dst_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
            }
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                 >> 9U));
    if ((0x00008000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
        if ((0x00004000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
            if ((0x00002000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                              >> 0x0000000cU)))) {
                    if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                            = ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                ? ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                    ? ((0x00000100U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                        ? 0x14U : 0x15U)
                                    : ((0x00000100U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                        ? 0x16U : 0x17U))
                                : ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                    ? ((0x00000100U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                        ? 0x12U : 0x13U)
                                    : ((0x00000100U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                        ? 0x10U : 0x11U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode = 0U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                            = ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                ? ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                    ? 0x14U : 0x16U)
                                : ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                    ? 0x12U : 0x10U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode = 0U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                                = ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                    ? ((0x00000200U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                        ? 0x15U : 0x17U)
                                    : ((0x00000200U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                        ? 0x13U : 0x11U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                                = ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                    ? ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                        ? 0x15U : 0x17U)
                                    : ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                        ? 0x13U : (
                                                   (0x00000100U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 0x10U
                                                    : 0x11U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode = 0U;
                        }
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                      >> 6U)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                            = ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                ? ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                    ? 0x15U : 0x17U)
                                : ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                    ? 0x13U : ((0x00000100U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                ? 0x10U
                                                : 0x11U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode = 0U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                }
            } else {
                if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 1U;
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U));
                            } else {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U));
                            }
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                      >> 6U)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 3U;
                        }
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 3U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                  >> 6U)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 3U;
                    }
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 3U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 9U));
                }
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
            }
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                          >> 0x0000000dU)))) {
                if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                            }
                        }
                    } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                        }
                    }
                }
            }
        } else if ((0x00002000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
            if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 6U;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 5U;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 5U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 6U;
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                }
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
            }
        } else {
            if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 2U;
                if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 9U));
                }
            } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                  >> 6U)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 4U;
                    }
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 4U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 9U));
                }
            } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                              >> 6U)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 4U;
                }
                if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 9U));
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 9U));
                }
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 4U;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 9U));
            }
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
        }
    } else if ((0x00004000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
        if ((0x00002000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
            if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                              >> 8U)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 9U));
                }
            }
        } else {
            if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                  >> 6U)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                            = ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                ? 2U : 1U);
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                        = ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                            ? 2U : 1U);
                }
            } else if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                                }
                            }
                        }
                    } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                            }
                        }
                    }
                } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                            }
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                            = ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                ? ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                    ? 7U : 0x0cU) : 0x0cU);
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                        }
                    }
                } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                      >> 5U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                          >> 4U)))) {
                                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                              >> 3U)))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 0x0eU;
                                }
                            }
                        }
                    } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 5U)))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                      >> 4U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                          >> 3U)))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 0x0dU;
                            }
                        }
                    }
                } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                  >> 5U)))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                      >> 4U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                          >> 3U)))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 0x0fU;
                            }
                        }
                    }
                }
            } else if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                            }
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 0x0aU;
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                        }
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 9U;
                }
            } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                        }
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 0x0bU;
                }
            } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                    }
                }
            }
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                          >> 0x0000000cU)))) {
                if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                        = (7U & ((0x00000040U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                  ? 
                                                 ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                  >> 9U)
                                                  : 
                                                 ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                  >> 9U)));
                                }
                            }
                        } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                    = (7U & ((0x00000040U 
                                              & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                              ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 9U)
                                              : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 9U)));
                            }
                        }
                    } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                    = (7U & ((0x00000040U 
                                              & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                              ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 9U)
                                              : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 9U)));
                            }
                        }
                    } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((0x00000040U 
                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                          ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U)
                                          : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U)));
                        }
                    }
                } else if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                    = (7U & ((0x00000040U 
                                              & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                              ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 9U)
                                              : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 9U)));
                            }
                        }
                    } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((0x00000040U 
                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                          ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U)
                                          : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U)));
                        }
                    }
                } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((0x00000040U 
                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                          ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U)
                                          : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U)));
                        }
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                      ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U) : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                   >> 9U)));
                    }
                }
            }
        }
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                      >> 0x0000000dU)))) {
            if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                        } else if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                        } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                }
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
            } else if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                    = (7U & ((0x00000040U 
                                              & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                              ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 3U)
                                              : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 3U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                            }
                        } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                        } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                              >> 4U)))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                                }
                            } else if ((0x00000010U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                            }
                        }
                    } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((0x00000040U 
                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                          ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U)
                                          : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                        }
                    } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                    }
                } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((0x00000040U 
                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                          ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U)
                                          : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                      ? ((0x00000040U 
                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                          ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U)
                                          : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U))
                                      : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                      ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U) : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                   >> 3U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                    }
                } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                        } else if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                        } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U));
                        }
                    } else if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                    } else if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                    } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                    }
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                    } else if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                    } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                    }
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                }
            } else if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                                = (7U & ((0x00000040U 
                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                          ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U)
                                          : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 3U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                      ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U) : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                   >> 3U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                }
            } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                      ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 3U) : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                   >> 3U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                }
            } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                  ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U) : ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                               >> 3U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                }
            }
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                          >> 0x0000000cU)))) {
                if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                    if ((0x00000040U 
                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                                    }
                                }
                            }
                        } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                                }
                            }
                        }
                    } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                                }
                            }
                        }
                    } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                            }
                        }
                    }
                } else if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                                }
                            }
                        }
                    } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                            }
                        }
                    }
                } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                            }
                        }
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 1U;
                        }
                    }
                }
            }
        }
    } else {
        if ((0x00002000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
            if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                >> 0x0cU))) | (2U == 
                                               (3U 
                                                & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                   >> 0x0cU)))) 
                 | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 0x0cU))))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
            }
        } else if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
            if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                >> 0x0cU))) | (2U == 
                                               (3U 
                                                & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                   >> 0x0cU)))) 
                 | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 0x0cU))))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
            }
        } else if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
            if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 6U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                    } else if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                               >> 0x0cU))) 
                                 | (2U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                 >> 0x0cU)))) 
                                | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                                >> 0x0cU))))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op = 8U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 3U));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 6U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                        = ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                            ? ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                ? ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                    ? 0U : 6U) : ((0x00000200U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                   ? 5U
                                                   : 0U))
                            : ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                ? ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                    ? 1U : 2U) : ((0x00000200U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                   ? 3U
                                                   : 4U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode = 7U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg = 4U;
                }
            } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 6U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 3U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                        = ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                            ? ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                ? ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                    ? 0U : 5U) : ((0x00000400U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                   ? 1U
                                                   : 3U))
                            : ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                ? ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                    ? ((0x00000010U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                        ? ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                            ? ((4U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (2U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000800U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 6U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 5U)
                                                       : 0U))
                                                     : 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 1U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 2U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 3U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 4U))))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000800U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 6U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 5U)
                                                        : 0U))
                                                      : 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 1U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 2U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 3U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 4U))))
                                                     : 5U))
                                                : (
                                                   (0x00000800U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 6U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 5U)
                                                      : 0U))
                                                    : 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 1U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 2U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 3U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 4U)))))
                                            : ((0x00000800U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 6U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 5U)
                                                     : 0U))
                                                : (
                                                   (0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 1U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 2U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 3U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 4U)))))
                                        : ((0x00000800U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                            ? ((0x00000400U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 0U
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 6U))
                                                : 5U)
                                            : ((0x00000400U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                ? 1U
                                                : (
                                                   (0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 3U)
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 4U)))))
                                    : ((0x00000800U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                        ? ((0x00000400U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                            ? 0U : 5U)
                                        : ((0x00000400U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                            ? 1U : 
                                           ((0x00000200U 
                                             & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                             ? ((0x00000100U 
                                                 & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                 ? 0U
                                                 : 3U)
                                             : ((0x00000100U 
                                                 & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                 ? 0U
                                                 : 4U)))))
                                : ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                    ? ((0x00000010U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                        ? ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                            ? ((4U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (2U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000800U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 6U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 5U)
                                                       : 0U))
                                                     : 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 1U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 2U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 3U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 4U))))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000800U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 6U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 5U)
                                                        : 0U))
                                                      : 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 1U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 2U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 3U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 4U))))
                                                     : 5U))
                                                : (
                                                   (0x00000800U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 6U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 5U)
                                                      : 0U))
                                                    : 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 1U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 2U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 3U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 4U)))))
                                            : ((0x00000800U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 6U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 5U)
                                                     : 0U))
                                                : (
                                                   (0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 1U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 2U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 3U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 4U)))))
                                        : ((0x00000800U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                            ? ((0x00000400U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                ? 0U
                                                : (
                                                   (0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 5U)
                                                    : 0U))
                                            : ((0x00000400U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 1U)
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 2U))
                                                : (
                                                   (0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 3U)
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 4U)))))
                                    : ((0x00000800U 
                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                        ? ((0x00000400U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                            ? 0U : 
                                           ((0x00000200U 
                                             & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                             ? ((0x00000100U 
                                                 & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                 ? 0U
                                                 : 5U)
                                             : 0U))
                                        : ((0x00000400U 
                                            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                            ? ((0x00000200U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (0x00000100U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 0U
                                                    : 1U)
                                                : (
                                                   (0x00000100U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 0U
                                                    : 2U))
                                            : ((0x00000200U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                ? (
                                                   (0x00000100U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 0U
                                                    : 3U)
                                                : (
                                                   (0x00000100U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 0U
                                                    : 4U)))))));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                        = (7U & 7U);
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                        = (7U & 4U);
                }
            } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 6U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 6U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                    = (7U & 7U);
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & 4U);
            }
        } else if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
            if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 6U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                    = ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                        ? ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                            ? 0U : 6U) : ((0x00000200U 
                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                           ? 1U : 2U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                    = (7U & 7U);
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & 4U);
            }
        } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
            if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 6U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                             >> 3U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                    = ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                        ? ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                            ? 5U : 3U) : ((0x00000040U 
                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                           ? ((0x00000020U 
                                               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                               ? ((0x00000010U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((8U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((4U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((2U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000800U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000400U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 6U))
                                                        : 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 5U)
                                                         : 0U))
                                                       : 
                                                      ((0x00000400U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 1U)
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 2U))
                                                        : 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 3U)
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 4U))))
                                                      : 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000800U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000400U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000200U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 6U))
                                                         : 
                                                        ((0x00000200U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 5U)
                                                          : 0U))
                                                        : 
                                                       ((0x00000400U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000200U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 1U)
                                                          : 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 2U))
                                                         : 
                                                        ((0x00000200U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 3U)
                                                          : 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 4U))))
                                                       : 3U))
                                                     : 
                                                    ((0x00000800U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 6U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 5U)
                                                        : 0U))
                                                      : 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 1U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 2U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 3U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 4U)))))
                                                    : 
                                                   ((0x00000800U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 6U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 5U)
                                                       : 0U))
                                                     : 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 1U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 2U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 3U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 4U)))))
                                                   : 
                                                  ((0x00000800U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 5U
                                                    : 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 1U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 2U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 3U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 4U)))))
                                               : ((0x00000800U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                   ? 5U
                                                   : 
                                                  ((0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 1U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 2U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 3U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 4U)))))
                                           : ((0x00000020U 
                                               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                               ? ((0x00000010U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((8U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((4U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((2U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000800U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000400U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 6U))
                                                        : 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 5U)
                                                         : 0U))
                                                       : 
                                                      ((0x00000400U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 1U)
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 2U))
                                                        : 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 3U)
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 4U))))
                                                      : 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000800U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000400U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000200U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 6U))
                                                         : 
                                                        ((0x00000200U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 5U)
                                                          : 0U))
                                                        : 
                                                       ((0x00000400U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000200U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 1U)
                                                          : 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 2U))
                                                         : 
                                                        ((0x00000200U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 3U)
                                                          : 
                                                         ((0x00000100U 
                                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                           ? 0U
                                                           : 4U))))
                                                       : 3U))
                                                     : 
                                                    ((0x00000800U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 6U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 5U)
                                                        : 0U))
                                                      : 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 1U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 2U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 3U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 4U)))))
                                                    : 
                                                   ((0x00000800U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 6U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 5U)
                                                       : 0U))
                                                     : 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 1U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 2U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 3U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 4U)))))
                                                   : 
                                                  ((0x00000800U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 6U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 5U)
                                                      : 0U))
                                                    : 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 1U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 2U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 3U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 4U)))))
                                               : ((0x00000800U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 6U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 5U)
                                                     : 0U))
                                                   : 
                                                  ((0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 1U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 2U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 3U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 4U)))))));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                    = (7U & 7U);
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                    = (7U & 4U);
            }
        } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                         >> 6U));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                         >> 3U));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op));
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_alu_op 
                = ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                    ? ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                        ? 0U : 4U) : ((0x00000040U 
                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                       ? ((0x00000020U 
                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                           ? ((0x00000010U 
                                               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                               ? ((8U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((4U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((2U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000800U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 6U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 5U)
                                                        : 0U))
                                                      : 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 1U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 2U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 3U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 4U))))
                                                     : 
                                                    ((1U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000800U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000400U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 6U))
                                                        : 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 5U)
                                                         : 0U))
                                                       : 
                                                      ((0x00000400U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 1U)
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 2U))
                                                        : 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 3U)
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 4U))))
                                                      : 4U))
                                                    : 
                                                   ((0x00000800U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 6U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 5U)
                                                       : 0U))
                                                     : 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 1U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 2U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 3U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 4U)))))
                                                   : 
                                                  ((0x00000800U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 6U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 5U)
                                                      : 0U))
                                                    : 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 1U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 2U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 3U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 4U)))))
                                               : ((0x00000800U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 6U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 5U)
                                                     : 0U))
                                                   : 
                                                  ((0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 1U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 2U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 3U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 4U)))))
                                           : ((0x00000800U 
                                               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                               ? ((0x00000400U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 0U
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 6U))
                                                   : 
                                                  ((0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 5U)
                                                    : 0U))
                                               : ((0x00000400U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 1U)
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 2U))
                                                   : 
                                                  ((0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 3U)
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 4U)))))
                                       : ((0x00000020U 
                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                           ? ((0x00000010U 
                                               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                               ? ((8U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((4U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((2U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000800U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 6U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 5U)
                                                        : 0U))
                                                      : 
                                                     ((0x00000400U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 1U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 2U))
                                                       : 
                                                      ((0x00000200U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 3U)
                                                        : 
                                                       ((0x00000100U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 4U))))
                                                     : 
                                                    ((1U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000800U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000400U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 0U
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 6U))
                                                        : 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 5U)
                                                         : 0U))
                                                       : 
                                                      ((0x00000400U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 1U)
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 2U))
                                                        : 
                                                       ((0x00000200U 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                         ? 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 3U)
                                                         : 
                                                        ((0x00000100U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                          ? 0U
                                                          : 4U))))
                                                      : 4U))
                                                    : 
                                                   ((0x00000800U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 6U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 5U)
                                                       : 0U))
                                                     : 
                                                    ((0x00000400U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 1U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 2U))
                                                      : 
                                                     ((0x00000200U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 3U)
                                                       : 
                                                      ((0x00000100U 
                                                        & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                        ? 0U
                                                        : 4U)))))
                                                   : 
                                                  ((0x00000800U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 6U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 5U)
                                                      : 0U))
                                                    : 
                                                   ((0x00000400U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 1U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 2U))
                                                     : 
                                                    ((0x00000200U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 3U)
                                                      : 
                                                     ((0x00000100U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                       ? 0U
                                                       : 4U)))))
                                               : ((0x00000800U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 6U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 5U)
                                                     : 0U))
                                                   : 
                                                  ((0x00000400U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 1U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 2U))
                                                    : 
                                                   ((0x00000200U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 3U)
                                                     : 
                                                    ((0x00000100U 
                                                      & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                      ? 0U
                                                      : 4U)))))
                                           : ((0x00000800U 
                                               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                               ? ((0x00000400U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 0U
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 6U))
                                                   : 
                                                  ((0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 5U)
                                                    : 0U))
                                               : ((0x00000400U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                   ? 
                                                  ((0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 1U)
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 2U))
                                                   : 
                                                  ((0x00000200U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                    ? 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 3U)
                                                    : 
                                                   ((0x00000100U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))
                                                     ? 0U
                                                     : 4U)))))));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode 
                = (7U & 7U);
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg 
                = (7U & 4U);
        }
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                      >> 0x0000000dU)))) {
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                          >> 0x0000000cU)))) {
                if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                             >> 9U));
                            }
                        } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                         >> 9U));
                        }
                    } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                     >> 9U));
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a = 0U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_op) 
                                 >> 9U));
                }
            }
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_update = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_an_update = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_is_mem = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_update = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_update = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem = 0U;
    if (vlSelfRef.rst_n) {
        if (((IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid) 
             & (~ (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_pending)))) {
            vlSelfRef.m68k_top__DOT__u_bus__DOT__rrobin 
                = ((3U == (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner))
                    ? 0U : (3U & ((IData)(1U) + (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner))));
        }
        if (vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid) {
            if (vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_pending) {
                vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_pending = 0U;
            } else if ((1U & (((((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_lock) 
                                 << 3U) | ((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_lock) 
                                           << 2U)) 
                               | (((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_lock) 
                                   << 1U) | (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_lock))) 
                              >> (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner)))) {
                vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_pending = 1U;
                vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_holder 
                    = vlSelfRef.m68k_top__DOT__u_bus__DOT__winner;
            }
        }
        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_size 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_size;
                if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_dispatch_w) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_predicted_taken 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__predict_taken_w;
                }
            }
        }
        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__redirect_valid)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stall)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_size 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_size;
                if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_dispatch_w) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_predicted_taken 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__predict_taken_w;
                }
            }
        }
    } else {
        vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_pending = 0U;
        vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_holder = 0U;
        vlSelfRef.m68k_top__DOT__u_bus__DOT__rrobin = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_size = 2U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_size = 2U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_predicted_taken = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_predicted_taken = 0U;
    }
    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))) {
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_update = 1U;
            }
        }
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))) {
                if (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg)) 
                     || (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem = 1U;
                }
            }
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem = 1U;
        }
    } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))) {
        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_update = 1U;
        }
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem = 1U;
    }
    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_update = 1U;
            }
        }
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
                if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg) 
                                  >> 1U)))) {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_imm32;
                        }
                    }
                }
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg) 
                              >> 2U)))) {
                    if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg))) {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem = 1U;
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem = 1U;
                    }
                }
            }
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem = 1U;
        }
    } else {
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_update = 1U;
            }
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem = 1U;
        }
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode) 
                      >> 1U)))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_ra;
        }
    }
    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))) {
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_an_update = 1U;
            }
        }
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))) {
                if (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_reg)) 
                     || (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_reg)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_is_mem = 1U;
                }
            }
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_is_mem = 1U;
        }
    } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))) {
        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_an_update = 1U;
        }
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_is_mem = 1U;
    }
    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_update = 1U;
            }
        }
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
                if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg) 
                                  >> 1U)))) {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_imm32;
                        }
                    }
                }
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg) 
                              >> 2U)))) {
                    if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg))) {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem = 1U;
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem = 1U;
                    }
                }
            }
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem = 1U;
        }
    } else {
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_update = 1U;
            }
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem = 1U;
        }
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode) 
                      >> 1U)))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_ra;
        }
    }
    if (vlSelfRef.rst_n) {
        if (((IData)(vlSelfRef.m68k_top__DOT__snoop_valid) 
             & ((2U != (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_port_q)) 
                & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids
                   [(0x000000ffU & (vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_addr_q 
                                    >> 2U))] & ((vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_addr_q 
                                                 >> 0x0000000aU) 
                                                == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags
                                                [(0x000000ffU 
                                                  & (vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_addr_q 
                                                     >> 2U))]))))) {
            __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v0 
                = (0x000000ffU & (vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_addr_q 
                                  >> 2U));
            vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids.enqueue(0U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v0));
        }
        if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__state) {
            if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__state) {
                if ((4U & (IData)(vlSelfRef.m68k_top__DOT__p_resp_valid))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__saved_we)))) {
                        __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v0 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__saved_tag;
                        __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v0 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__saved_idx;
                        vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags.enqueue(__VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v0, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v0));
                        __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v1 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__saved_idx;
                        vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids.enqueue(1U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v1));
                        __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__data__v0 
                            = vlSelfRef.m68k_top__DOT__p_resp_data;
                        __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__data__v0 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__saved_idx;
                        vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__data.enqueue(__VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__data__v0, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__data__v0));
                    }
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__bus_req_r = 0U;
                    vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_we = 0U;
                    vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_lock = 0U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__state = 0U;
                }
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__state = 0U;
            }
        } else if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_req) 
                    & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__hit)))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__bus_req_r = 1U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_we = 0U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_lock = 0U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_addr 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_addr;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_be = 0x0fU;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__saved_we = 0U;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__saved_idx 
                = (0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_addr 
                                  >> 2U));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__saved_tag 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_addr 
                   >> 0x0000000aU);
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__state = 1U;
        }
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__i = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__state = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__bus_req_r = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_we = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_lock = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_addr = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_be = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__saved_we = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__saved_idx = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__saved_tag = 0U;
        while (VL_GTS_III(32, 0x00000100U, vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__i)) {
            __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v1 
                = (0x000000ffU & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__i);
            vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags.enqueue(0U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v1));
            __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v2 
                = (0x000000ffU & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__i);
            vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids.enqueue(0U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v2));
            __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__data__v1 
                = (0x000000ffU & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__i);
            vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__data.enqueue(0U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__data__v1));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__i 
                = ((IData)(1U) + vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__i);
        }
    }
    if (vlSelfRef.rst_n) {
        if (((IData)(vlSelfRef.m68k_top__DOT__snoop_valid) 
             & ((0U != (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_port_q)) 
                & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids
                   [(0x000000ffU & (vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_addr_q 
                                    >> 2U))] & ((vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_addr_q 
                                                 >> 0x0000000aU) 
                                                == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags
                                                [(0x000000ffU 
                                                  & (vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_addr_q 
                                                     >> 2U))]))))) {
            __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v0 
                = (0x000000ffU & (vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_addr_q 
                                  >> 2U));
            vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids.enqueue(0U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v0));
        }
        if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__state) {
            if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__state) {
                if ((1U & (IData)(vlSelfRef.m68k_top__DOT__p_resp_valid))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__saved_we)))) {
                        __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v0 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__saved_tag;
                        __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v0 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__saved_idx;
                        vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags.enqueue(__VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v0, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v0));
                        __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v1 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__saved_idx;
                        vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids.enqueue(1U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v1));
                        __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data__v0 
                            = vlSelfRef.m68k_top__DOT__p_resp_data;
                        __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data__v0 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__saved_idx;
                        vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data.enqueue(__VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data__v0, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data__v0));
                    }
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__bus_req_r = 0U;
                    vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_we = 0U;
                    vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_lock = 0U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__state = 0U;
                }
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__state = 0U;
            }
        } else if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_req) 
                    & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__hit)))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__bus_req_r = 1U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_we = 0U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_lock = 0U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_addr 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_be = 0x0fU;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__saved_we = 0U;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__saved_idx 
                = (0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr 
                                  >> 2U));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__saved_tag 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr 
                   >> 0x0000000aU);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__state = 1U;
        }
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__i = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__state = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__bus_req_r = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_we = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_lock = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_addr = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_be = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__saved_we = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__saved_idx = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__saved_tag = 0U;
        while (VL_GTS_III(32, 0x00000100U, vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__i)) {
            __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v1 
                = (0x000000ffU & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__i);
            vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags.enqueue(0U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags__v1));
            __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v2 
                = (0x000000ffU & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__i);
            vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids.enqueue(0U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids__v2));
            __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data__v1 
                = (0x000000ffU & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__i);
            vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data.enqueue(0U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data__v1));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__i 
                = ((IData)(1U) + vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__i);
        }
    }
    if (vlSelfRef.rst_n) {
        if (((IData)(vlSelfRef.m68k_top__DOT__snoop_valid) 
             & ((3U != (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_port_q)) 
                & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids
                   [(0x000000ffU & (vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_addr_q 
                                    >> 2U))] & ((vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_addr_q 
                                                 >> 0x0000000aU) 
                                                == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags
                                                [(0x000000ffU 
                                                  & (vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_addr_q 
                                                     >> 2U))]))))) {
            __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v0 
                = (0x000000ffU & (vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_addr_q 
                                  >> 2U));
            vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids.enqueue(0U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v0));
        }
        if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__state) {
            if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__state) {
                if ((8U & (IData)(vlSelfRef.m68k_top__DOT__p_resp_valid))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__saved_we)))) {
                        __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v0 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__saved_tag;
                        __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v0 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__saved_idx;
                        vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags.enqueue(__VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v0, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v0));
                        __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v1 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__saved_idx;
                        vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids.enqueue(1U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v1));
                        __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v0 
                            = vlSelfRef.m68k_top__DOT__p_resp_data;
                        __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v0 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__saved_idx;
                        vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data.enqueue(__VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v0, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v0));
                    }
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__bus_req_r = 0U;
                    vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_we = 0U;
                    vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_lock = 0U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__state = 0U;
                }
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__state = 0U;
            }
        } else if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_req) 
                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_we))) {
            if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__hit) {
                __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__be 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_be;
                __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__new_w 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_wdata;
                __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__old_w 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data
                    [(0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr 
                                     >> 2U))];
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT____VlemCall_0__merge_be 
                    = ((0x0000ffffU & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT____VlemCall_0__merge_be) 
                       | (((0x0000ff00U & (((8U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__be))
                                             ? (__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__new_w 
                                                >> 0x18U)
                                             : (__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__old_w 
                                                >> 0x18U)) 
                                           << 8U)) 
                           | (0x000000ffU & ((4U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__be))
                                              ? (__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__new_w 
                                                 >> 0x10U)
                                              : (__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__old_w 
                                                 >> 0x10U)))) 
                          << 0x00000010U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT____VlemCall_0__merge_be 
                    = ((0xffff0000U & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT____VlemCall_0__merge_be) 
                       | ((0x0000ff00U & (((2U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__be))
                                            ? (__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__new_w 
                                               >> 8U)
                                            : (__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__old_w 
                                               >> 8U)) 
                                          << 8U)) | 
                          (0x000000ffU & ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__be))
                                           ? __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__new_w
                                           : __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__401__old_w))));
                __VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v1 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT____VlemCall_0__merge_be;
                __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v1 
                    = (0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr 
                                      >> 2U));
                vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data.enqueue(__VdlyVal__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v1, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v1));
            }
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__bus_req_r = 1U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_we = 1U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_lock 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_lock;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_addr 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_wdata 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_wdata;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_be 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_be;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__saved_we = 1U;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__saved_idx 
                = (0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr 
                                  >> 2U));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__saved_tag 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr 
                   >> 0x0000000aU);
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__state = 1U;
        } else if ((((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_req) 
                     & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_we))) 
                    & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__hit)) 
                       | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_lock)))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__bus_req_r = 1U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_we = 0U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_lock 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_lock;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_addr 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_be = 0x0fU;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__saved_we = 0U;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__saved_idx 
                = (0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr 
                                  >> 2U));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__saved_tag 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr 
                   >> 0x0000000aU);
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__state = 1U;
        }
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__i = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__state = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__bus_req_r = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_we = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_lock = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_addr = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_wdata = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_be = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__saved_we = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__saved_idx = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__saved_tag = 0U;
        while (VL_GTS_III(32, 0x00000100U, vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__i)) {
            __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v1 
                = (0x000000ffU & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__i);
            vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags.enqueue(0U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v1));
            __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v2 
                = (0x000000ffU & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__i);
            vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids.enqueue(0U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v2));
            __VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v2 
                = (0x000000ffU & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__i);
            vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data.enqueue(0U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data__v2));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__i 
                = ((IData)(1U) + vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__i);
        }
    }
    if (vlSelfRef.rst_n) {
        if (((IData)(vlSelfRef.m68k_top__DOT__snoop_valid) 
             & ((1U != (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_port_q)) 
                & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids
                   [(0x000000ffU & (vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_addr_q 
                                    >> 2U))] & ((vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_addr_q 
                                                 >> 0x0000000aU) 
                                                == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags
                                                [(0x000000ffU 
                                                  & (vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_addr_q 
                                                     >> 2U))]))))) {
            __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v0 
                = (0x000000ffU & (vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_addr_q 
                                  >> 2U));
            vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids.enqueue(0U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v0));
        }
        if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__state) {
            if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__state) {
                if ((2U & (IData)(vlSelfRef.m68k_top__DOT__p_resp_valid))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__saved_we)))) {
                        __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v0 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__saved_tag;
                        __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v0 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__saved_idx;
                        vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags.enqueue(__VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v0, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v0));
                        __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v1 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__saved_idx;
                        vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids.enqueue(1U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v1));
                        __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v0 
                            = vlSelfRef.m68k_top__DOT__p_resp_data;
                        __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v0 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__saved_idx;
                        vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data.enqueue(__VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v0, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v0));
                    }
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__bus_req_r = 0U;
                    vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_we = 0U;
                    vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_lock = 0U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__state = 0U;
                }
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__state = 0U;
            }
        } else if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_req) 
                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_we))) {
            if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__hit) {
                __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__be 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_be;
                __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__new_w 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_wdata;
                __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__old_w 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data
                    [(0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr 
                                     >> 2U))];
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT____VlemCall_0__merge_be 
                    = ((0x0000ffffU & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT____VlemCall_0__merge_be) 
                       | (((0x0000ff00U & (((8U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__be))
                                             ? (__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__new_w 
                                                >> 0x18U)
                                             : (__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__old_w 
                                                >> 0x18U)) 
                                           << 8U)) 
                           | (0x000000ffU & ((4U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__be))
                                              ? (__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__new_w 
                                                 >> 0x10U)
                                              : (__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__old_w 
                                                 >> 0x10U)))) 
                          << 0x00000010U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT____VlemCall_0__merge_be 
                    = ((0xffff0000U & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT____VlemCall_0__merge_be) 
                       | ((0x0000ff00U & (((2U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__be))
                                            ? (__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__new_w 
                                               >> 8U)
                                            : (__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__old_w 
                                               >> 8U)) 
                                          << 8U)) | 
                          (0x000000ffU & ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__be))
                                           ? __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__new_w
                                           : __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__merge_be__0__old_w))));
                __VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v1 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT____VlemCall_0__merge_be;
                __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v1 
                    = (0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr 
                                      >> 2U));
                vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data.enqueue(__VdlyVal__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v1, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v1));
            }
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__bus_req_r = 1U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_we = 1U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_lock 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_addr 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_wdata 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_wdata;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_be 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_be;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__saved_we = 1U;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__saved_idx 
                = (0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr 
                                  >> 2U));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__saved_tag 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr 
                   >> 0x0000000aU);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__state = 1U;
        } else if ((((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_req) 
                     & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_we))) 
                    & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__hit)) 
                       | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock)))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__bus_req_r = 1U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_we = 0U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_lock 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_addr 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_be = 0x0fU;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__saved_we = 0U;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__saved_idx 
                = (0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr 
                                  >> 2U));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__saved_tag 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr 
                   >> 0x0000000aU);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__state = 1U;
        }
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__i = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__state = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__bus_req_r = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_we = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_lock = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_addr = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_wdata = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_be = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__saved_we = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__saved_idx = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__saved_tag = 0U;
        while (VL_GTS_III(32, 0x00000100U, vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__i)) {
            __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v1 
                = (0x000000ffU & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__i);
            vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags.enqueue(0U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags__v1));
            __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v2 
                = (0x000000ffU & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__i);
            vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids.enqueue(0U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids__v2));
            __VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v2 
                = (0x000000ffU & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__i);
            vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data.enqueue(0U, (IData)(__VdlyDim0__m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data__v2));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__i 
                = ((IData)(1U) + vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__i);
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
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_reg_idx_full 
        = (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_is_a) 
            << 3U) | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_reg_idx));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_src_base_idx 
        = ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode))
            ? ((- (IData)((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode))))) 
               & ((8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg)) 
                  & (- (IData)((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode) 
                                         >> 1U)))))))
            : (((0U != (3U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_mode))) 
                << 3U) | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_src_reg)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_reg_idx_full 
        = (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_is_a) 
            << 3U) | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_reg_idx));
    vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__data.commit(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__data);
    vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids.commit(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids);
    vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags.commit(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags);
    vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data.commit(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data);
    vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids.commit(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids);
    vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags.commit(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags);
    vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data.commit(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__data);
    vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids.commit(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__valids);
    vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags.commit(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__DOT__tags);
    vlSelfRef.m68k_top__DOT__snoop_valid = ((IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_valid_q) 
                                            & (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_we_q));
    vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data.commit(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data);
    vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids.commit(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__valids);
    vlSelfRef.__VdlyCommitQueuem68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags.commit(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__tags);
    if (vlSelfRef.rst_n) {
        if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot = 0U;
            if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy) 
                 & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_ack)))) {
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
            if (vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted) {
                __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy = 0U;
            } else {
                if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain) 
                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_ack))) {
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
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted)))) {
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
        if (((((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_valid) 
               & (3U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) 
              & (0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state))) 
             & (~ (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted)))) {
            vlSelfRef.__Vdly__m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted = 1U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halt_code 
                = (0x0000ffffU & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_imm_raw);
        }
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state))) {
                if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_ack) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dc_req_r = 0U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_lock = 0U;
                    __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state = 0U;
                }
            } else if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_ack) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dc_req_r = 0U;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_lock = 0U;
                __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state = 0U;
            }
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state))) {
            if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_ack) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dc_req_r = 0U;
                if ((0x0bU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__be_for_byte__412__pos 
                        = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr);
                    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__413__pos 
                        = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr);
                    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__be_for_byte__412__Vfuncout 
                        = ((2U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__be_for_byte__412__pos))
                            ? ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__be_for_byte__412__pos))
                                ? 1U : 2U) : ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__be_for_byte__412__pos))
                                               ? 4U
                                               : 8U));
                    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__413__w 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_rdata;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_tas_word 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_rdata;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dc_req_r = 1U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_we = 1U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_lock = 1U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_be 
                        = __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__be_for_byte__412__Vfuncout;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT____VlemCall_2__byte_at 
                        = (0x000000ffU & ((2U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__413__pos))
                                           ? ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__413__pos))
                                               ? __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__413__w
                                               : (__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__413__w 
                                                  >> 8U))
                                           : ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__413__pos))
                                               ? (__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__413__w 
                                                  >> 0x10U)
                                               : (__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__413__w 
                                                  >> 0x18U))));
                    __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state = 3U;
                    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_into_word__414__pos 
                        = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr);
                    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_into_word__414__b 
                        = (0x80U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT____VlemCall_2__byte_at));
                    __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_into_word__414__Vfuncout 
                        = ((2U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_into_word__414__pos))
                            ? ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_into_word__414__pos))
                                ? (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_into_word__414__b)
                                : ((IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_into_word__414__b) 
                                   << 8U)) : ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_into_word__414__pos))
                                               ? ((IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_into_word__414__b) 
                                                  << 0x00000010U)
                                               : ((IData)(__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_into_word__414__b) 
                                                  << 0x00000018U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_wdata 
                        = __Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_into_word__414__Vfuncout;
                } else {
                    __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state = 0U;
                }
            }
        } else if ((((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_valid) 
                     & (~ (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted))) 
                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_mem))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dc_req_r = 1U;
            if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_we) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_we = 1U;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_lock 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_lock;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_addr;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_wdata 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_wdata;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_be 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_be;
                __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state = 2U;
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_we = 0U;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_lock 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_lock;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_addr;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_wdata 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_wdata;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_be 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__want_be;
                __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state = 1U;
            }
        }
        if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__redirect_valid) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_slot = 0U;
            if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_busy) 
                 & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_ack)))) {
                __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_drain = 1U;
            }
            __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_busy = 0U;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_words_done = 0U;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_pc 
                = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stop_now)
                    ? vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_pc
                    : ((0x0cU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))
                        ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_c)
                            ? ((IData)(2U) + (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_pc 
                                              + vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_branch_imm))
                            : vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_pc_next)
                        : vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__take_branch_target_c));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_valid = 0U;
        } else {
            if (vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted) {
                __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_busy = 0U;
            } else {
                if (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_drain) 
                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_ack))) {
                    __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_drain = 0U;
                }
                if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__would_be_complete) {
                    if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stall) {
                        __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_busy = 0U;
                    } else {
                        __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_busy = 1U;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_addr 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_fetch_target;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_fetch_addr 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_fetch_target;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_slot = 0U;
                    }
                } else if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_latching) {
                    __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_busy = 1U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_addr 
                        = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_pc 
                           + VL_SHIFTL_III(32,32,32, (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__new_words_done), 1U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_fetch_addr 
                        = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_pc 
                           + VL_SHIFTL_III(32,32,32, (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__new_words_done), 1U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_slot 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__new_words_done;
                } else if ((1U & (((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_busy)) 
                                   & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stall))) 
                                  & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_drain))))) {
                    __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_busy = 1U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_addr 
                        = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_pc 
                           + VL_SHIFTL_III(32,32,32, (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_words_done), 1U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_fetch_addr 
                        = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_pc 
                           + VL_SHIFTL_III(32,32,32, (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_words_done), 1U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_slot 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_words_done;
                }
            }
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted)))) {
                if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__would_be_complete) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_words_done 
                        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stall)
                            ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__new_words_done)
                            : 0U);
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stall)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_pc 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_fetch_target;
                    }
                } else if (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_latching) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_words_done 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__new_words_done;
                }
            }
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stall)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_valid 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__id_valid;
            }
        }
        vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_addr_q 
            = vlSelfRef.m68k_top__DOT__u_bus__DOT__addr
            [vlSelfRef.m68k_top__DOT__u_bus__DOT__winner];
        vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_port_q 
            = vlSelfRef.m68k_top__DOT__u_bus__DOT__winner;
        if (((((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid) 
               & (3U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) 
              & (0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) 
             & (~ (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted)))) {
            vlSelfRef.__Vdly__m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted = 1U;
            vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halt_code 
                = (0x0000ffffU & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_imm_raw);
        }
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
                if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_ack) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dc_req_r = 0U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock = 0U;
                    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state = 0U;
                }
            } else if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_ack) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dc_req_r = 0U;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock = 0U;
                __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state = 0U;
            }
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
            if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_ack) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dc_req_r = 0U;
                if ((0x0bU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__11__pos 
                        = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr);
                    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__12__pos 
                        = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr);
                    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__11__Vfuncout 
                        = ((2U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__11__pos))
                            ? ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__11__pos))
                                ? 1U : 2U) : ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__11__pos))
                                               ? 4U
                                               : 8U));
                    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__12__w 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_rdata;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_tas_word 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_rdata;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dc_req_r = 1U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_we = 1U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock = 1U;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_be 
                        = __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__be_for_byte__11__Vfuncout;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT____VlemCall_2__byte_at 
                        = (0x000000ffU & ((2U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__12__pos))
                                           ? ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__12__pos))
                                               ? __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__12__w
                                               : (__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__12__w 
                                                  >> 8U))
                                           : ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__12__pos))
                                               ? (__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__12__w 
                                                  >> 0x10U)
                                               : (__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__12__w 
                                                  >> 0x18U))));
                    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state = 3U;
                    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__13__pos 
                        = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr);
                    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__13__b 
                        = (0x80U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT____VlemCall_2__byte_at));
                    __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__13__Vfuncout 
                        = ((2U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__13__pos))
                            ? ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__13__pos))
                                ? (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__13__b)
                                : ((IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__13__b) 
                                   << 8U)) : ((1U & (IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__13__pos))
                                               ? ((IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__13__b) 
                                                  << 0x00000010U)
                                               : ((IData)(__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__13__b) 
                                                  << 0x00000018U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_wdata 
                        = __Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_into_word__13__Vfuncout;
                } else {
                    __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state = 0U;
                }
            }
        } else if ((((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid) 
                     & (~ (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted))) 
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
        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__redirect_valid)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stall)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_imm_raw 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__idec_imm;
            }
        }
    } else {
        __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_busy = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_slot = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_fetch_addr = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_addr = 0U;
        __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_drain = 0U;
        __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_addr = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr = 0U;
        __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain = 0U;
        __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_tas_word = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dc_req_r = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_we = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_lock = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_wdata = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_be = 0x0fU;
        vlSelfRef.__Vdly__m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halt_code = 0U;
        vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_addr_q = 0U;
        vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_port_q = 0U;
        __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_tas_word = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dc_req_r = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_we = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_wdata = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_be = 0x0fU;
        vlSelfRef.__Vdly__m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halted = 0U;
        vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__u_core__halt_code = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_words_done = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_pc = 0x00000400U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_words_done = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc = 0x00000400U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_imm_raw = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_valid = 0U;
    }
    vlSelfRef.m68k_top__DOT__p_resp_data = vlSelfRef.m68k_top__DOT__u_bus__DOT__mem
        [vlSelfRef.m68k_top__DOT__u_bus__DOT__granted_idx_q];
    if ((2U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_size))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_step = 4U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_step = 4U;
    } else if ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_size))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_step = 2U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_step = 2U;
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_step 
            = ((7U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg))
                ? 2U : 1U);
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_step 
            = ((7U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg))
                ? 2U : 1U);
    }
    if ((2U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_size))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_step = 4U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_step = 4U;
    } else if ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_size))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_step = 2U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_step = 2U;
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_step 
            = ((7U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_reg))
                ? 2U : 1U);
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_step 
            = ((7U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_reg))
                ? 2U : 1U);
    }
    vlSelfRef.m68k_top__DOT__u_bus__DOT__be[2U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_be;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__be[0U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_be;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__wdata[3U] 
        = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_wdata;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__be[3U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_be;
    vlSelfRef.m68k_top__DOT__p_we = ((((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_we) 
                                       << 3U) | ((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_we) 
                                                 << 2U)) 
                                     | (((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_we) 
                                         << 1U) | (IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_we)));
    vlSelfRef.m68k_top__DOT__u_bus__DOT__wdata[1U] 
        = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_wdata;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__be[1U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_be;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_ea = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_next = 0U;
    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))) {
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))) {
                if (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg)) 
                     || (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_ea 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_imm32;
                }
            }
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_ea 
                = ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))
                    ? (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb 
                       + vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_imm32)
                    : (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb 
                       - vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_step));
        }
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_next 
                    = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb 
                       - vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_step);
            }
        }
    } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_ea 
            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb;
        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_next 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb 
                   + vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_step);
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_next = 0U;
    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_next 
                    = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_ra 
                       - vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_step);
            }
        }
    } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_next 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_ra 
                   + vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_step);
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_ea = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_an_next = 0U;
    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))) {
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))) {
                if (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_reg)) 
                     || (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_reg)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_ea 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_imm32;
                }
            }
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_ea 
                = ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))
                    ? (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_rb 
                       + vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_imm32)
                    : (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_rb 
                       - vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_step));
        }
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_an_next 
                    = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_rb 
                       - vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_step);
            }
        }
    } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_ea 
            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_rb;
        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_an_next 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_rb 
                   + vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_step);
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_next = 0U;
    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode)))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_next 
                    = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_ra 
                       - vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_step);
            }
        }
    } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_an_next 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_ra 
                   + vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_step);
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_drain 
        = __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_drain;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_busy 
        = __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_busy;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain 
        = __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy 
        = __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state 
        = __Vdly__m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__addr[0U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__bus_addr;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__addr[1U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__bus_addr;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__addr[2U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__bus_addr;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__addr[3U] = vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__g_cache__DOT__u_dc__bus_addr;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state 
        = __Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__hit 
        = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__valids
           [(0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_addr 
                            >> 2U))] & ((vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_addr 
                                         >> 0x0000000aU) 
                                        == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__g_cache__DOT__u_ic__DOT__tags
                                        [(0x000000ffU 
                                          & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__ic_addr 
                                             >> 2U))]));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__hit 
        = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__valids
           [(0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr 
                            >> 2U))] & ((vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr 
                                         >> 0x0000000aU) 
                                        == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__tags
                                        [(0x000000ffU 
                                          & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr 
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
}
