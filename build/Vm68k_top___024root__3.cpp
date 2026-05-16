// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vm68k_top.h for the primary calling header

#include "Vm68k_top__pch.h"

void Vm68k_top___024root___nba_sequent__TOP__3(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___nba_sequent__TOP__3\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    QData/*32:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sub_res;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sub_res = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__res_neg;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__res_neg = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_val;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_val = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_y;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_y = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_v;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_v = 0;
    QData/*32:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sub_res;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sub_res = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__res_neg;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__res_neg = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_val;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_val = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_y;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_y = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_v;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_v = 0;
    // Body
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_x 
        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_x;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum = 0ULL;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sub_res = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__res_neg = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_val = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_y = 0U;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_v = 0U;
    if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
        if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y = 0U;
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_val 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_sized;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v = 0U;
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_y 
                = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_val;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                = ((vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
                    & (~ vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__mask)) 
                   | (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_y 
                      & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                = (0U != (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                = (0U == (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c = 0U;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_x 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_x;
        } else {
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_val 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_sized;
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_y 
                = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_val;
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_v = 0U;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                = ((vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
                    & (~ vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__mask)) 
                   | (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_y 
                      & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                = (0U != (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                = (0U == (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c = 0U;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v 
                = ((0x10U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c)) 
                   & (IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__shift_v));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_x 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_x;
        }
    } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
        if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
            if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
                if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                        = ((vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
                            << 0x00000010U) | (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
                                               >> 0x10U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                        = (1U & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
                                 >> 0x0fU));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                        = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y);
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                        = (((- (IData)((1U & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
                                              >> 0x0fU)))) 
                            << 0x00000010U) | (0x0000ffffU 
                                               & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                        = (1U & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
                                 >> 0x0fU));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                        = (0U == (0x0000ffffU & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b));
                }
            } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                    = ((0xffff0000U & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b) 
                       | ((0x0000ff00U & ((- (IData)(
                                                     (1U 
                                                      & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
                                                         >> 7U)))) 
                                          << 8U)) | 
                          (0x000000ffU & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                    = (1U & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
                             >> 7U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                    = (0U == (0x000000ffU & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b));
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_sized;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_neg;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                    = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
            }
        } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y = 0U;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z = 1U;
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                    = ((~ vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_sized) 
                       & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__mask);
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                    = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                              & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                    = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y);
            }
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                = (0x00000001ffffffffULL & (- (QData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_sized))));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                = ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum) 
                   & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__mask);
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c 
                = (0U != vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_x 
                = (0U != vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y);
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v 
                = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_neg) 
                   & (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                             & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask)));
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_sized;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_neg;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
        }
    } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                    = (0x00000080U | vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b);
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                    = (1U & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b 
                             >> 7U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                    = (0U == (0x000000ffU & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_b));
            } else {
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                    = (0x00000001ffffffffULL & ((QData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__a_sized)) 
                                                - (QData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_sized))));
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sub_res 
                    = ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum) 
                       & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__mask);
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                    = ((6U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))
                        ? vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a
                        : m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sub_res);
                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__res_neg 
                    = (0U != (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sub_res 
                              & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                    = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__res_neg;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                    = (0U == m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sub_res);
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c 
                    = ((0x20U >= (0x0000003fU & ((IData)(1U) 
                                                 + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))) 
                       && (1U & (IData)((m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                         >> (0x0000003fU 
                                             & ((IData)(1U) 
                                                + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))))));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v 
                    = (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__a_neg) 
                        != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_neg)) 
                       & ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__res_neg) 
                          != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__a_neg)));
                if ((6U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_x 
                        = ((0x20U >= (0x0000003fU & 
                                      ((IData)(1U) 
                                       + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))) 
                           && (1U & (IData)((m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                             >> (0x0000003fU 
                                                 & ((IData)(1U) 
                                                    + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))))));
                }
            }
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__a_sized 
                   ^ vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y);
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__a_sized 
                   | vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y);
        }
    } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__a_sized 
                   & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y);
        } else {
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                = (0x00000001ffffffffULL & ((QData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__a_sized)) 
                                            - (QData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_sized))));
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sub_res 
                = ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum) 
                   & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__mask);
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                = ((6U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))
                    ? vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_a
                    : m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sub_res);
            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__res_neg 
                = (0U != (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sub_res 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
                = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__res_neg;
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
                = (0U == m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sub_res);
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c 
                = ((0x20U >= (0x0000003fU & ((IData)(1U) 
                                             + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))) 
                   && (1U & (IData)((m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                     >> (0x0000003fU 
                                         & ((IData)(1U) 
                                            + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))))));
            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v 
                = (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__a_neg) 
                    != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_neg)) 
                   & ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__res_neg) 
                      != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__a_neg)));
            if ((6U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_x 
                    = ((0x20U >= (0x0000003fU & ((IData)(1U) 
                                                 + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))) 
                       && (1U & (IData)((m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                         >> (0x0000003fU 
                                             & ((IData)(1U) 
                                                + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))))));
            }
        }
    } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_op_c))) {
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
            = (0x00000001ffffffffULL & ((QData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__a_sized)) 
                                        + (QData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_sized))));
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
            = ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum) 
               & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__mask);
        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__res_neg 
            = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
                      & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
            = m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__res_neg;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
            = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y);
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c 
            = ((0x20U >= (0x0000003fU & ((IData)(1U) 
                                         + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))) 
               && (1U & (IData)((m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                 >> (0x0000003fU & 
                                     ((IData)(1U) + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))))));
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_x 
            = ((0x20U >= (0x0000003fU & ((IData)(1U) 
                                         + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))) 
               && (1U & (IData)((m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                 >> (0x0000003fU & 
                                     ((IData)(1U) + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))))));
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v 
            = (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__a_neg) 
                == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_neg)) 
               & ((IData)(m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__res_neg) 
                  != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__a_neg)));
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y 
            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_sized;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n 
            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_neg;
        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z 
            = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_bcc_uncond 
        = ((0x0cU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_kind)) 
           & ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_cc)) 
              | (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_cc))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__disp32 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_needs_disp_word)
            ? (((- (IData)((1U & ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_3) 
                                  >> 0x0000000fU)))) 
                << 0x00000010U) | (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_3))
            : vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_imm);
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__total_words_eff 
        = (7U & ((IData)(1U) + ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_dst_ext_words) 
                                + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_src_ext_words))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_x 
        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_x;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum = 0ULL;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sub_res = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__res_neg = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_val = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_y = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_v = 0U;
    if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
        if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y = 0U;
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_val 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v = 0U;
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_y 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_val;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = ((vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                    & (~ vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask)) 
                   | (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_y 
                      & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                = (0U != (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                = (0U == (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c = 0U;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_x 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_x;
        } else {
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_val 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized;
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_y 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_val;
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_v = 0U;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = ((vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                    & (~ vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask)) 
                   | (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_y 
                      & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                = (0U != (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                = (0U == (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c = 0U;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v 
                = ((0x10U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c)) 
                   & (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_v));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_x 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_x;
        }
    } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
        if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
                if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                        = ((vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                            << 0x00000010U) | (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                                               >> 0x10U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                        = (1U & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                                 >> 0x0fU));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                        = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y);
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                        = (((- (IData)((1U & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                                              >> 0x0fU)))) 
                            << 0x00000010U) | (0x0000ffffU 
                                               & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                        = (1U & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                                 >> 0x0fU));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                        = (0U == (0x0000ffffU & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b));
                }
            } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                    = ((0xffff0000U & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b) 
                       | ((0x0000ff00U & ((- (IData)(
                                                     (1U 
                                                      & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                                                         >> 7U)))) 
                                          << 8U)) | 
                          (0x000000ffU & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                    = (1U & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                             >> 7U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                    = (0U == (0x000000ffU & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b));
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_neg;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                    = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
            }
        } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y = 0U;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z = 1U;
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                    = ((~ vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized) 
                       & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask);
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                    = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                              & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                    = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y);
            }
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                = (0x00000001ffffffffULL & (- (QData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized))));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum) 
                   & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c 
                = (0U != vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_x 
                = (0U != vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v 
                = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_neg) 
                   & (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                             & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask)));
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_neg;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
        }
    } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                    = (0x00000080U | vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b);
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                    = (1U & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                             >> 7U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                    = (0U == (0x000000ffU & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b));
            } else {
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                    = (0x00000001ffffffffULL & ((QData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_sized)) 
                                                - (QData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized))));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sub_res 
                    = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum) 
                       & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask);
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                    = ((6U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))
                        ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a
                        : m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sub_res);
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__res_neg 
                    = (0U != (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sub_res 
                              & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__res_neg;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                    = (0U == m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sub_res);
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c 
                    = ((0x20U >= (0x0000003fU & ((IData)(1U) 
                                                 + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))) 
                       && (1U & (IData)((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                         >> (0x0000003fU 
                                             & ((IData)(1U) 
                                                + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))))));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v 
                    = (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_neg) 
                        != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_neg)) 
                       & ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__res_neg) 
                          != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_neg)));
                if ((6U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_x 
                        = ((0x20U >= (0x0000003fU & 
                                      ((IData)(1U) 
                                       + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))) 
                           && (1U & (IData)((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                             >> (0x0000003fU 
                                                 & ((IData)(1U) 
                                                    + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))))));
                }
            }
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_sized 
                   ^ vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y);
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_sized 
                   | vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y);
        }
    } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_sized 
                   & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y);
        } else {
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                = (0x00000001ffffffffULL & ((QData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_sized)) 
                                            - (QData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized))));
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sub_res 
                = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum) 
                   & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = ((6U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))
                    ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a
                    : m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sub_res);
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__res_neg 
                = (0U != (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sub_res 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__res_neg;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                = (0U == m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sub_res);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c 
                = ((0x20U >= (0x0000003fU & ((IData)(1U) 
                                             + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))) 
                   && (1U & (IData)((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                     >> (0x0000003fU 
                                         & ((IData)(1U) 
                                            + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))))));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v 
                = (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_neg) 
                    != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_neg)) 
                   & ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__res_neg) 
                      != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_neg)));
            if ((6U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_x 
                    = ((0x20U >= (0x0000003fU & ((IData)(1U) 
                                                 + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))) 
                       && (1U & (IData)((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                         >> (0x0000003fU 
                                             & ((IData)(1U) 
                                                + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))))));
            }
        }
    } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
            = (0x00000001ffffffffULL & ((QData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_sized)) 
                                        + (QData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized))));
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
            = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum) 
               & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask);
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__res_neg 
            = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                      & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__res_neg;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
            = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y);
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c 
            = ((0x20U >= (0x0000003fU & ((IData)(1U) 
                                         + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))) 
               && (1U & (IData)((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                 >> (0x0000003fU & 
                                     ((IData)(1U) + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))))));
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_x 
            = ((0x20U >= (0x0000003fU & ((IData)(1U) 
                                         + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))) 
               && (1U & (IData)((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                                 >> (0x0000003fU & 
                                     ((IData)(1U) + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos)))))));
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v 
            = (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_neg) 
                == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_neg)) 
               & ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__res_neg) 
                  != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_neg)));
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_neg;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
            = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
    }
}

void Vm68k_top___024root___nba_sequent__TOP__4(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___nba_sequent__TOP__4\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*2:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__total_words_eff;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__total_words_eff = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_bcc_uncond;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_bcc_uncond = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__disp32;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__disp32 = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c;
    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c = 0;
    // Body
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_bcc_uncond 
        = ((0x0cU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind)) 
           & ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_cc)) 
              | (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_cc))));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__disp32 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_needs_disp_word)
            ? (((- (IData)((1U & ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_2) 
                                  >> 0x0000000fU)))) 
                << 0x00000010U) | (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_2))
            : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_imm);
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__total_words_eff 
        = (7U & ((IData)(1U) + ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words) 
                                + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_x_c 
        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_x;
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
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c;
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
            } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                                 >> 5U)))) {
                if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                                  >> 3U)))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                                      >> 2U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                                          >> 1U)))) {
                                if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                                    if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v_c 
                                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v;
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c_c 
                                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c;
                                    }
                                }
                            }
                        }
                        if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                                          >> 1U)))) {
                                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c 
                                    = ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))
                                        ? vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_ra
                                        : vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y);
                            }
                        } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y;
                            }
                        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
                                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c = 0U;
                            }
                        } else if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
                            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y;
                        }
                    }
                } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                                if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v_c 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v;
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c_c 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c;
                                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y;
                                }
                            } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c 
                                    = ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_size))
                                        ? (((- (IData)(
                                                       (1U 
                                                        & (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand 
                                                           >> 0x0fU)))) 
                                            << 0x00000010U) 
                                           | (0x0000ffffU 
                                              & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand))
                                        : vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand);
                            }
                        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c;
                                if ((6U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_alu_op))) {
                                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y;
                                }
                            }
                        }
                    } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c;
                                if ((6U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_alu_op))) {
                                    m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y;
                                }
                            }
                        }
                    } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_v_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_v;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_c_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_c;
                        if (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode)) 
                             | (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode)))) {
                            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_y;
                        }
                    } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem)))) {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_is_mem)))) {
                            m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand;
                        }
                    }
                } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c 
                            = ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))
                                ? vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_imm_raw
                                : ((7U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))
                                    ? vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_imm32
                                    : vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_ea));
                    }
                }
            }
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state)))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                              >> 5U)))) {
                    if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                                      >> 3U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                                          >> 2U)))) {
                                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                                              >> 1U)))) {
                                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                                        if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_x_c 
                                                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_x;
                                        }
                                    }
                                }
                            }
                        }
                    } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                                if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                                    if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_x_c 
                                            = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_x;
                                    }
                                }
                            } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                                if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_x_c 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_x;
                                }
                            }
                        }
                    }
                }
            }
        }
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_state))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__409__pos 
                    = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr);
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__409__w 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_tas_word;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT____VlemCall_0__byte_at 
                    = (0x000000ffU & ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__409__pos))
                                       ? ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__409__pos))
                                           ? vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__409__w
                                           : (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__409__w 
                                              >> 8U))
                                       : ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__409__pos))
                                           ? (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__409__w 
                                              >> 0x10U)
                                           : (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__409__w 
                                              >> 0x18U))));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                    = (1U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT____VlemCall_0__byte_at) 
                             >> 7U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__410__pos 
                    = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__dc_addr);
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__410__w 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_tas_word;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT____VlemCall_1__byte_at 
                    = (0x000000ffU & ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__410__pos))
                                       ? ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__410__pos))
                                           ? vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__410__w
                                           : (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__410__w 
                                              >> 8U))
                                       : ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__410__pos))
                                           ? (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__410__w 
                                              >> 0x10U)
                                           : (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__byte_at__410__w 
                                              >> 0x18U))));
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                    = (0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT____VlemCall_1__byte_at));
            } else if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                    = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand 
                       >> 0x1fU);
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                    = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand);
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
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n;
                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z;
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                             >> 5U)))) {
            if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                              >> 3U)))) {
                    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind) 
                                      >> 1U)))) {
                            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z;
                            }
                        }
                    } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z;
                        } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z;
                        }
                    } else {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                            if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n;
                            }
                        }
                        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c = 1U;
                            }
                        } else if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z;
                        }
                    }
                }
            } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_src_mode))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z;
                            }
                        }
                    } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_dst_mode))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z;
                        }
                    }
                } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind)))) {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z;
                        }
                    }
                } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__ex_kind))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_n;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__alu_z;
                } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_needs_mem)))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dst_is_mem)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_n_c 
                            = (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand 
                               >> 0x1fU);
                        vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__cc_z_c 
                            = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__src_operand);
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
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__predict_taken_w 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_bcc_uncond) 
           | ((vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__disp32 
               >> 0x0000001fU) & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_bcc_uncond)) 
                                  & (0x0cU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__dec_kind)))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__would_be_complete 
        = (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__new_words_done) 
            >= (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__total_words_eff)) 
           & (0U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__new_words_done)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_x_c 
        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_x;
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
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c;
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
            } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                 >> 5U)))) {
                if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                  >> 3U)))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                      >> 2U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                          >> 1U)))) {
                                if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                                    if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v_c 
                                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v;
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c_c 
                                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c;
                                    }
                                }
                            }
                        }
                        if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                          >> 1U)))) {
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
                                    = ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))
                                        ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_ra
                                        : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y);
                            }
                        } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y;
                            }
                        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c = 0U;
                            }
                        } else if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y;
                        }
                    }
                } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                                if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v_c 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v;
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c_c 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c;
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y;
                                }
                            } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
                                    = ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_size))
                                        ? (((- (IData)(
                                                       (1U 
                                                        & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand 
                                                           >> 0x0fU)))) 
                                            << 0x00000010U) 
                                           | (0x0000ffffU 
                                              & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand))
                                        : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand);
                            }
                        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c;
                                if ((6U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op))) {
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y;
                                }
                            }
                        }
                    } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c;
                                if ((6U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op))) {
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y;
                                }
                            }
                        }
                    } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c;
                        if (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode)) 
                             | (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode)))) {
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y;
                        }
                    } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem)))) {
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand;
                        }
                    }
                } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
                            = ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))
                                ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_imm_raw
                                : ((7U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))
                                    ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_imm32
                                    : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_ea));
                    }
                }
            }
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                              >> 5U)))) {
                    if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                      >> 3U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                          >> 2U)))) {
                                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                              >> 1U)))) {
                                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                                        if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_x_c 
                                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_x;
                                        }
                                    }
                                }
                            }
                        }
                    } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                                if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                                    if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_x_c 
                                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_x;
                                    }
                                }
                            } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                                if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_x_c 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_x;
                                }
                            }
                        }
                    }
                }
            }
        }
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__pos 
                    = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr);
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__w 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_tas_word;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT____VlemCall_0__byte_at 
                    = (0x000000ffU & ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__pos))
                                       ? ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__pos))
                                           ? vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__w
                                           : (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__w 
                                              >> 8U))
                                       : ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__pos))
                                           ? (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__w 
                                              >> 0x10U)
                                           : (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__8__w 
                                              >> 0x18U))));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                    = (1U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT____VlemCall_0__byte_at) 
                             >> 7U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__9__pos 
                    = (3U & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr);
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__9__w 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_tas_word;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT____VlemCall_1__byte_at 
                    = (0x000000ffU & ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__9__pos))
                                       ? ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__9__pos))
                                           ? vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__9__w
                                           : (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__9__w 
                                              >> 8U))
                                       : ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__9__pos))
                                           ? (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__9__w 
                                              >> 0x10U)
                                           : (vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__byte_at__9__w 
                                              >> 0x18U))));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                    = (0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT____VlemCall_1__byte_at));
            } else if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                    = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand 
                       >> 0x1fU);
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                    = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand);
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
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z;
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                             >> 5U)))) {
            if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                              >> 3U)))) {
                    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                      >> 1U)))) {
                            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z;
                            }
                        }
                    } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z;
                        } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z;
                        }
                    } else {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                            if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n;
                            }
                        }
                        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c = 1U;
                            }
                        } else if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z;
                        }
                    }
                }
            } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_mode))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z;
                            }
                        }
                    } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z;
                        }
                    }
                } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z;
                        }
                    }
                } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z;
                } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c 
                            = (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand 
                               >> 0x1fU);
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c 
                            = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand);
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
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__predict_taken_w 
        = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_bcc_uncond) 
           | ((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__disp32 
               >> 0x0000001fU) & ((~ (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_bcc_uncond)) 
                                  & (0x0cU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind)))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__would_be_complete 
        = (((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__new_words_done) 
            >= (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__total_words_eff)) 
           & (0U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__new_words_done)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_wdata 
        = (m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__wb_main_data_c 
           & (- (IData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__is_settled))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_fetch_target 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__predict_taken_w)
            ? ((IData)(2U) + (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_pc 
                              + vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__disp32))
            : (vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_pc 
               + ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__total_words_eff) 
                  << 1U)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__if_dispatch_w 
        = ((~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__stall) 
               | ((IData)(vlSelfRef.m68k_top__DOT____Vcellout__g_core__BRA__1__KET____DOT__u_core__halted) 
                  | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__redirect_valid)))) 
           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__1__KET____DOT__u_core__DOT__would_be_complete));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_wdata 
        = (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c 
           & (- (IData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled))));
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
}

void Vm68k_top___024root___nba_sequent__TOP__0(Vm68k_top___024root* vlSelf);
void Vm68k_top___024root___nba_sequent__TOP__1(Vm68k_top___024root* vlSelf);
void Vm68k_top___024root___nba_sequent__TOP__2(Vm68k_top___024root* vlSelf);

void Vm68k_top___024root___eval_nba(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___eval_nba\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((3ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vm68k_top___024root___nba_sequent__TOP__0(vlSelf);
        Vm68k_top___024root___nba_sequent__TOP__1(vlSelf);
        Vm68k_top___024root___nba_sequent__TOP__2(vlSelf);
        Vm68k_top___024root___nba_sequent__TOP__3(vlSelf);
        Vm68k_top___024root___nba_sequent__TOP__4(vlSelf);
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

void Vm68k_top___024root___eval_triggers_vec__act(Vm68k_top___024root* vlSelf);
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

bool Vm68k_top___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

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
