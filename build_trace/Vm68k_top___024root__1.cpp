// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vm68k_top.h for the primary calling header

#include "Vm68k_top__pch.h"

void Vm68k_top___024root___nba_sequent__TOP__1(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___nba_sequent__TOP__1\n"); );
    Vm68k_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_rdata;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_rdata = 0;
    CData/*1:0*/ m68k_top__DOT__u_bus__DOT__cand_ext;
    m68k_top__DOT__u_bus__DOT__cand_ext = 0;
    CData/*0:0*/ m68k_top__DOT__u_bus__DOT__cand;
    m68k_top__DOT__u_bus__DOT__cand = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__comb_read_hit;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__comb_read_hit = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__comb_read_hit;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__comb_read_hit = 0;
    CData/*1:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_we_c;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_we_c = 0;
    CData/*3:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_idx_c;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_idx_c = 0;
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
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_movq;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_movq = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_disp;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_disp = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_sized;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_sized = 0;
    IData/*31:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_neg;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_neg = 0;
    CData/*0:0*/ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_neg;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_neg = 0;
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
    // Body
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_req 
        = ((~ (IData)(vlSelfRef.halted)) & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dc_req_r));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_req 
        = ((~ (IData)(vlSelfRef.halted)) & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy));
    vlSelfRef.m68k_top__DOT__u_bus__DOT__winner = 0U;
    vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid = 0U;
    m68k_top__DOT__u_bus__DOT__cand = 0U;
    m68k_top__DOT__u_bus__DOT__cand_ext = 0U;
    if (vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_pending) {
        vlSelfRef.m68k_top__DOT__u_bus__DOT__winner 
            = vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_holder;
        vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid 
            = (1U & ((IData)(vlSelfRef.m68k_top__DOT__p_req) 
                     >> (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__lock_holder)));
    } else {
        m68k_top__DOT__u_bus__DOT__cand_ext = vlSelfRef.m68k_top__DOT__u_bus__DOT__rrobin;
        m68k_top__DOT__u_bus__DOT__cand = (1U & (IData)(m68k_top__DOT__u_bus__DOT__cand_ext));
        m68k_top__DOT__u_bus__DOT__cand_ext = (3U & 
                                               ((IData)(1U) 
                                                + (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__rrobin)));
        if ((1U & ((~ (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid)) 
                   & ((IData)(vlSelfRef.m68k_top__DOT__p_req) 
                      >> (IData)(m68k_top__DOT__u_bus__DOT__cand))))) {
            vlSelfRef.m68k_top__DOT__u_bus__DOT__winner 
                = m68k_top__DOT__u_bus__DOT__cand;
            vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid = 1U;
        }
        m68k_top__DOT__u_bus__DOT__cand = (1U & (IData)(m68k_top__DOT__u_bus__DOT__cand_ext));
        if ((1U & ((~ (IData)(vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid)) 
                   & ((IData)(vlSelfRef.m68k_top__DOT__p_req) 
                      >> (IData)(m68k_top__DOT__u_bus__DOT__cand))))) {
            vlSelfRef.m68k_top__DOT__u_bus__DOT__winner 
                = m68k_top__DOT__u_bus__DOT__cand;
            vlSelfRef.m68k_top__DOT__u_bus__DOT__winner_valid = 1U;
        }
    }
    if (vlSelfRef.rst_n) {
        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall)))) {
                if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_dispatch_w) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_pc 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc;
                }
            }
        }
        if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_valid = 0U;
        } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__stall)))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_valid 
                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_dispatch_w;
        }
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_pc = 0U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_valid = 0U;
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_rb_idx 
        = ((- (IData)((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind) 
                                >> 4U))))) & (((8U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind))
                                                ? (
                                                   ((2U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind))
                                                     ? 
                                                    ((- (IData)(
                                                                (1U 
                                                                 & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind))))) 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_reg_idx_full))
                                                     : 
                                                    (((4U 
                                                       & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode))
                                                       ? 
                                                      ((- (IData)(
                                                                  (1U 
                                                                   & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode))))) 
                                                       & ((8U 
                                                           | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg)) 
                                                          & (- (IData)(
                                                                       (1U 
                                                                        & (~ 
                                                                           ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode) 
                                                                            >> 1U)))))))
                                                       : 
                                                      (((0U 
                                                         != 
                                                         (3U 
                                                          & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_mode))) 
                                                        << 3U) 
                                                       | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_dst_reg))) 
                                                     & (- (IData)(
                                                                  (1U 
                                                                   & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind))))))) 
                                                   & (- (IData)(
                                                                (1U 
                                                                 & (~ 
                                                                    ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind) 
                                                                     >> 2U))))))
                                                : (
                                                   (- (IData)(
                                                              (3U 
                                                               == 
                                                               (3U 
                                                                & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind) 
                                                                   >> 1U))))) 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__id_reg_idx_full))) 
                                              & (- (IData)(
                                                           (1U 
                                                            & (~ 
                                                               ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__idec_kind) 
                                                                >> 5U)))))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_lock = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_we = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_be = 0x0fU;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_wdata = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_addr = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c = 0U;
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
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a = 0U;
    if ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_size))) {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos = 7U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask = 0x00000080U;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask = 0x000000ffU;
    } else {
        if ((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_size))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos = 0x0fU;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask = 0x00008000U;
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_pos = 0x1fU;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask = 0x80000000U;
        }
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask 
            = (0x0000ffffU | (- (IData)((1U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_size)))));
    }
    m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__comb_read_hit 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_req) 
           & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__state)) 
              & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_we)) 
                 & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_lock)) 
                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__hit)))));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__comb_read_hit 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_req) 
           & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__state)) 
              & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__hit)));
    vlSelfRef.m68k_top__DOT__u_bus__DOT__mem_idx = 
        (0x00003fffU & (vlSelfRef.m68k_top__DOT__u_bus__DOT__addr
                        [vlSelfRef.m68k_top__DOT__u_bus__DOT__winner] 
                        >> 2U));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc 
        = vlSelfRef.__Vdly__m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_pc;
    if ((1U & (~ ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid)) 
                  | (IData)(vlSelfRef.halted))))) {
        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                              >> 5U)))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                  >> 4U)))) {
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
                                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_ea;
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem = 1U;
                                    } else if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_we = 0U;
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_be = 0x0fU;
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_addr 
                                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_ea;
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem = 1U;
                                    }
                                } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                                    if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_we = 0U;
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_be = 0x0fU;
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_addr 
                                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_ea;
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem = 1U;
                                    } else if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_we = 1U;
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_be = 0x0fU;
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_addr 
                                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_ea;
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem = 1U;
                                    }
                                }
                                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                              >> 1U)))) {
                                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                            if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem) {
                                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_wdata 
                                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand;
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
                if ((8U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((0x0aU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op;
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb;
                    }
                }
            } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                 >> 5U)))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                              >> 4U)))) {
                    if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                      >> 2U)))) {
                            if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                                if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                        if ((6U != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op))) {
                                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_we_c = 1U;
                                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_idx_c 
                                                = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_reg_idx_full;
                                        }
                                    }
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op;
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
                                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_rb;
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
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_alu_op;
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
                                    = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand;
                            } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem)))) {
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
                    if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_update) {
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c = 1U;
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx_c 
                            = (8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_reg));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data_c 
                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_an_next;
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
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c = 1U;
                    } else if ((0x0cU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c = 1U;
                    }
                }
            }
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))) {
            if ((8U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c 
                    = (0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode));
                if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_update) {
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c = 1U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx_c 
                        = (8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data_c 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_next;
                }
            } else if ((0x0aU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c = 1U;
                if (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_update) {
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_we_c = 1U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_idx_c 
                        = (8U | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_src_reg));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_aux_data_c 
                        = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_an_next;
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
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c = 1U;
                    }
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                             >> 5U)))) {
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                          >> 4U)))) {
                if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                  >> 2U)))) {
                        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                                if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c = 1U;
                                }
                            }
                        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c = 1U;
                        } else if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dst_is_mem)))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_we_c 
                                    = (0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_dst_mode));
                            }
                        }
                    }
                    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                      >> 1U)))) {
                            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c 
                                    = (1U & ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                                   ? 
                                                  ((1U 
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
                                                   : 
                                                  ((1U 
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
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                                    ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n)
                                                    : 
                                                   (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n)))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                                    ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v)
                                                    : 
                                                   (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v)))))
                                              : ((4U 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                                    ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z)
                                                    : 
                                                   (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z)))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc))
                                                    ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c)
                                                    : 
                                                   (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c))))
                                                  : 
                                                 ((1U 
                                                   & (~ 
                                                      ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_cc) 
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
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_c = 1U;
                        }
                    }
                }
            }
        }
    }
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_sized 
        = (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a 
           & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask);
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_ack 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__bus_resp_now) 
           | (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__comb_read_hit));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_rdata 
        = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__comb_read_hit)
            ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__data
           [(0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_addr 
                            >> 2U))] : (vlSelfRef.m68k_top__DOT__p_resp_data 
                                        & (- (IData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_dc__DOT__bus_resp_now)))));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_rdata 
        = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__comb_read_hit)
            ? vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__data
           [(0x000000ffU & (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_addr 
                            >> 2U))] : (vlSelfRef.m68k_top__DOT__p_resp_data 
                                        & (- (IData)((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__bus_resp_now)))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_ack 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__bus_resp_now) 
           | (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__g_cache__DOT__u_ic__DOT__comb_read_hit));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_neg 
        = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask 
                  & m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_sized));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_settled 
        = (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)) 
            & ((~ ((IData)(vlSelfRef.halted) | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__want_mem))) 
               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid))) 
           | ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__dc_ack) 
              & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid) 
                 & ((~ (IData)(vlSelfRef.halted)) & 
                    (((1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)) 
                      & (0x0bU != (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) 
                     | ((2U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state)) 
                        | (3U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_state))))))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_target_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b = 0U;
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
        } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                             >> 5U)))) {
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                          >> 4U)))) {
                if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                      >> 1U)))) {
                            if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__take_branch_target_c 
                                    = ((IData)(2U) 
                                       + (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_pc 
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
            } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                 >> 5U)))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                              >> 4U)))) {
                    if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                      >> 2U)))) {
                            if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                                if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind)))) {
                                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_needs_mem)))) {
                                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                                            = vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__src_operand;
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
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w 
        = (0x0000ffffU & ((2U & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_fetch_addr)
                           ? m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_rdata
                           : (m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_rdata 
                              >> 0x00000010U)));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_busy) 
           & ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_drain)) 
              & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__ic_ack)));
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
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized 
        = (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
           & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask);
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
    vlSelfRef.__VdfgRegularize_hebeb780c_0_1 = (((1U 
                                                  == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot)) 
                                                 & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching))
                                                 ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w)
                                                 : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_ext[0U]);
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op 
        = (((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_slot)) 
            & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_latching))
            ? (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__half_word_w)
            : (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__if_op));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_neg 
        = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask 
                  & m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ext_combo[0U] 
        = vlSelfRef.__VdfgRegularize_hebeb780c_0_1;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_cc = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_needs_disp_word = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_imm = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_movq 
        = (((- (IData)((1U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                              >> 7U)))) << 8U) | (0x000000ffU 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_disp 
        = (((- (IData)((1U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                              >> 7U)))) << 8U) | (0x000000ffU 
                                                  & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)));
    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                  >> 0x0000000fU)))) {
        if ((0x00004000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            if ((0x00002000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                              >> 0x0000000cU)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_cc 
                        = (0x0000000fU & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                          >> 8U));
                    if ((0U == (0x000000ffU & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_needs_disp_word = 1U;
                    }
                }
                if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                      >> 8U)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_imm 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_movq;
                        }
                    } else if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                      >> 8U)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_imm 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_movq;
                        }
                    } else if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 8U)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_imm 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_movq;
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_imm 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__imm_disp;
                }
            } else {
                if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_cc 
                                    = (0x0000000fU 
                                       & ((0x00000020U 
                                           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                                           ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                              >> 8U)
                                           : ((0x00000010U 
                                               & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                                               ? ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                  >> 8U)
                                               : ((8U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                                                   ? 
                                                  ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                   >> 8U)
                                                   : 
                                                  ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                   >> 8U)))));
                            }
                        }
                    } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_cc 
                                = (0x0000000fU & ((0x00000020U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                                                   ? 
                                                  ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                   >> 8U)
                                                   : 
                                                  ((0x00000010U 
                                                    & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                                                    ? 
                                                   ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                    >> 8U)
                                                    : 
                                                   ((8U 
                                                     & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                                                     ? 
                                                    ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                     >> 8U)
                                                     : 
                                                    ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                     >> 8U)))));
                        }
                    }
                }
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                              >> 0x0000000cU)))) {
                    if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                              >> 8U)))) {
                                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                  >> 7U)))) {
                                        if ((0x00000040U 
                                             & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                            if ((1U 
                                                 & (~ 
                                                    ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                     >> 5U)))) {
                                                if (
                                                    (1U 
                                                     & (~ 
                                                        ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                         >> 4U)))) {
                                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_imm 
                                                        = 
                                                        (0x0000000fU 
                                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words = 0U;
    if ((0x00008000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
        if ((0x00004000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            if ((0x00002000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
                    = ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                        ? 0U : 0x1aU);
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                              >> 0x0000000cU)))) {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__11__s 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__11__Vfuncout 
                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__11__s))
                            ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__11__s))
                                     ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__11__s))
                                              ? 2U : 1U)));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__11__Vfuncout;
                }
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0U;
            }
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0U;
        }
    } else if ((0x00004000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
        if ((0x00002000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
                        = ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                            ? 0U : 7U);
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                  >> 8U)))) {
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 2U;
                    }
                } else if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
                        = ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                            ? 0U : 7U);
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                  >> 8U)))) {
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 2U;
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
                        = ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                            ? 0U : ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                                     ? ((0x00000020U 
                                         & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                                         ? 0x1aU : 0U)
                                     : 0U));
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                  >> 7U)))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__r 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__m 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__Vfuncout 
                                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__m))
                                        ? 1U : ((6U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__m))
                                                 ? 1U
                                                 : 
                                                ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__r))
                                                     ? 0U
                                                     : 1U))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__12__Vfuncout;
                            }
                        }
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 7U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 2U;
                }
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0cU;
                if ((0U == (0x000000ffU & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                }
            }
        } else if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x1dU;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 0U;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__r))
                                                    ? 0U
                                                    : 1U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__13__Vfuncout;
                        } else if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x1dU;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 0U;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__r))
                                                    ? 0U
                                                    : 1U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__14__Vfuncout;
                        } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x1cU;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x1dU;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 0U;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__r))
                                                    ? 0U
                                                    : 1U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__15__Vfuncout;
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__16__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__16__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__16__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__16__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__16__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__16__Vfuncout;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__17__Vfuncout;
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__18__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__18__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__18__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__18__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__18__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__18__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__18__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__18__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__18__r))
                                                    ? 0U
                                                    : 1U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__18__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__18__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__18__Vfuncout;
                        } else {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__19__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__19__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__19__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__19__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__19__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__19__Vfuncout;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__20__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__20__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__20__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__20__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__20__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__20__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__20__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__20__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__20__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__20__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__20__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__20__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__20__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__20__Vfuncout;
                        }
                    } else {
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__21__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__21__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__21__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__21__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__21__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__21__Vfuncout;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__22__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__22__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__22__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__22__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__22__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__22__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__22__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__22__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__22__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__22__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__22__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__22__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__22__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__22__Vfuncout;
                    }
                }
            } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x1dU;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__23__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 0U;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__23__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__23__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__23__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__23__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__23__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__23__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__23__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__23__r))
                                                     ? 0U
                                                     : 1U))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__23__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__23__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__23__Vfuncout;
                    } else if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x1dU;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__24__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 0U;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__24__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__24__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__24__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__24__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__24__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__24__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__24__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__24__r))
                                                     ? 0U
                                                     : 1U))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__24__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__24__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__24__Vfuncout;
                    } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x1cU;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x1dU;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__25__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 0U;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__25__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__25__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__25__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__25__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__25__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__25__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__25__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__25__r))
                                                     ? 0U
                                                     : 1U))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__25__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__25__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__25__Vfuncout;
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0U;
                }
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0U;
            }
        } else if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__26__r 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 2U;
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__26__m 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 3U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__26__Vfuncout 
                                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__26__m))
                                            ? 1U : 
                                           ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__26__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__26__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__26__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__26__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__26__r))
                                                    ? 0U
                                                    : 2U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__26__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__26__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__26__Vfuncout;
                                } else {
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__27__s 
                                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 6U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__27__Vfuncout 
                                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__27__s))
                                            ? 0U : 
                                           ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__27__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__27__s))
                                              ? 2U : 1U)));
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__27__Vfuncout;
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__28__sz 
                                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__28__r 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__28__m 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 3U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__28__Vfuncout 
                                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__28__m))
                                            ? 1U : 
                                           ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__28__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__28__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__28__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__28__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__28__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__28__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__28__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__28__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__28__Vfuncout;
                                }
                            } else {
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__29__s 
                                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 6U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__29__Vfuncout 
                                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__29__s))
                                        ? 0U : ((1U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__29__s))
                                                 ? 1U
                                                 : 
                                                ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__29__s))
                                                  ? 2U
                                                  : 1U)));
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__29__Vfuncout;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__30__sz 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__30__r 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__30__m 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__30__Vfuncout 
                                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__30__m))
                                        ? 1U : ((6U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__30__m))
                                                 ? 1U
                                                 : 
                                                ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__30__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__30__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__30__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__30__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__30__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__30__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__30__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__30__Vfuncout;
                            }
                        } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__31__r 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__31__m 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__31__Vfuncout 
                                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__31__m))
                                        ? 1U : ((6U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__31__m))
                                                 ? 1U
                                                 : 
                                                ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__31__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__31__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__31__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__31__r))
                                                     ? 0U
                                                     : 1U))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__31__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__31__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__31__Vfuncout;
                            } else {
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__32__s 
                                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 6U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__32__Vfuncout 
                                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__32__s))
                                        ? 0U : ((1U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__32__s))
                                                 ? 1U
                                                 : 
                                                ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__32__s))
                                                  ? 2U
                                                  : 1U)));
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__32__Vfuncout;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__33__sz 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__33__r 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__33__m 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__33__Vfuncout 
                                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__33__m))
                                        ? 1U : ((6U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__33__m))
                                                 ? 1U
                                                 : 
                                                ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__33__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__33__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__33__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__33__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__33__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__33__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__33__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__33__Vfuncout;
                            }
                        } else {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__34__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__34__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__34__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__34__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__34__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__34__Vfuncout;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__35__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__35__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__35__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__35__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__35__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__35__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__35__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__35__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__35__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__35__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__35__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__35__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__35__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__35__Vfuncout;
                        }
                    } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
                            = ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                                ? 4U : 5U);
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__r))
                                                     ? 0U
                                                     : 2U))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__36__Vfuncout;
                    } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__r 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__m 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 3U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__Vfuncout 
                                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__m))
                                            ? 1U : 
                                           ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__r))
                                                    ? 0U
                                                    : 1U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__37__Vfuncout;
                                } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
                                        = ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                                            ? ((1U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                                                ? 0x21U
                                                : 0x1fU)
                                            : ((1U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                                                ? 2U
                                                : 0x0aU));
                                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                  >> 1U)))) {
                                        if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))) {
                                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__r 
                                                = (7U 
                                                   & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__m 
                                                = (7U 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                      >> 3U));
                                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__Vfuncout 
                                                = (
                                                   (5U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__m))
                                                    ? 1U
                                                    : 
                                                   ((6U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__m))
                                                     ? 1U
                                                     : 
                                                    ((7U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__m))
                                                      ? 
                                                     ((4U 
                                                       & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__r))
                                                       ? 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__r))
                                                        ? 0U
                                                        : 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__r))
                                                         ? 0U
                                                         : 1U))
                                                       : 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__r))
                                                        ? 1U
                                                        : 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__r))
                                                         ? 2U
                                                         : 1U)))
                                                      : 0U)));
                                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__38__Vfuncout;
                                        }
                                    }
                                } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
                                        = ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                                            ? 0x20U
                                            : 3U);
                                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))) {
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                                    }
                                } else {
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
                                        = ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                                            ? 1U : 0x0aU);
                                    if ((1U & (~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))) {
                                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__r 
                                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__m 
                                            = (7U & 
                                               ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                >> 3U));
                                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__Vfuncout 
                                            = ((5U 
                                                == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__m))
                                                ? 1U
                                                : (
                                                   (6U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__m))
                                                    ? 1U
                                                    : 
                                                   ((7U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__m))
                                                     ? 
                                                    ((4U 
                                                      & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__r))
                                                      ? 
                                                     ((2U 
                                                       & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__r))
                                                       ? 0U
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__r))
                                                        ? 0U
                                                        : 1U))
                                                      : 
                                                     ((2U 
                                                       & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__r))
                                                       ? 1U
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__r))
                                                        ? 2U
                                                        : 1U)))
                                                     : 0U)));
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__39__Vfuncout;
                                    }
                                }
                            } else {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x29U;
                            }
                        } else if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
                                = ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                                    ? 0x17U : 0x16U);
                            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                          >> 3U)))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                            }
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x1eU;
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__40__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__40__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__40__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__40__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__40__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__40__Vfuncout;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__41__Vfuncout;
                    }
                } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__r 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 2U;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__m 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__Vfuncout 
                                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__m))
                                        ? 1U : ((6U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__m))
                                                 ? 1U
                                                 : 
                                                ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__r))
                                                     ? 0U
                                                     : 2U))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__42__Vfuncout;
                            } else {
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__43__s 
                                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 6U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__43__Vfuncout 
                                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__43__s))
                                        ? 0U : ((1U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__43__s))
                                                 ? 1U
                                                 : 
                                                ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__43__s))
                                                  ? 2U
                                                  : 1U)));
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__43__Vfuncout;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__44__sz 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__44__r 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__44__m 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__44__Vfuncout 
                                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__44__m))
                                        ? 1U : ((6U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__44__m))
                                                 ? 1U
                                                 : 
                                                ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__44__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__44__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__44__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__44__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__44__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__44__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__44__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__44__Vfuncout;
                            }
                        } else {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__45__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__45__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__45__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__45__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__45__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__45__Vfuncout;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__46__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__46__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__46__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__46__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__46__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__46__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__46__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__46__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__46__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__46__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__46__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__46__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__46__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__46__Vfuncout;
                        }
                    } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__47__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__47__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__47__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__47__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__47__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__47__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__47__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__47__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__47__r))
                                                    ? 0U
                                                    : 1U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__47__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__47__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__47__Vfuncout;
                        } else {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__48__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__48__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__48__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__48__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__48__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__48__Vfuncout;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__49__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__49__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__49__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__49__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__49__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__49__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__49__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__49__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__49__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__49__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__49__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__49__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__49__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__49__Vfuncout;
                        }
                    } else {
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__50__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__50__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__50__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__50__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__50__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__50__Vfuncout;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__51__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__51__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__51__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__51__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__51__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__51__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__51__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__51__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__51__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__51__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__51__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__51__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__51__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__51__Vfuncout;
                    }
                } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x19U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                            ? 2U : 1U);
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                    if (((5U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                       >> 3U))) | (6U 
                                                   == 
                                                   (7U 
                                                    & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                       >> 3U))))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 2U;
                    } else if ((7U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = ((0U == (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))
                                ? 2U : ((1U == (7U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))
                                         ? 3U : 2U));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__52__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__52__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__52__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__52__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__52__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__52__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__52__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__52__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__52__r))
                                                    ? 0U
                                                    : 1U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__52__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__52__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__52__Vfuncout;
                        } else {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__53__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__53__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__53__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__53__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__53__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__53__Vfuncout;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__54__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__54__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__54__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__54__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__54__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__54__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__54__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__54__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__54__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__54__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__54__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__54__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__54__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__54__Vfuncout;
                        }
                    } else {
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__55__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__55__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__55__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__55__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__55__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__55__Vfuncout;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__56__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__56__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__56__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__56__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__56__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__56__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__56__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__56__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__56__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__56__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__56__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__56__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__56__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__56__Vfuncout;
                    }
                }
            } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__57__r 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 2U;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__57__m 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__57__Vfuncout 
                                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__57__m))
                                        ? 1U : ((6U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__57__m))
                                                 ? 1U
                                                 : 
                                                ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__57__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__57__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__57__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__57__r))
                                                     ? 0U
                                                     : 2U))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__57__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__57__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__57__Vfuncout;
                            } else {
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__58__s 
                                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 6U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__58__Vfuncout 
                                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__58__s))
                                        ? 0U : ((1U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__58__s))
                                                 ? 1U
                                                 : 
                                                ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__58__s))
                                                  ? 2U
                                                  : 1U)));
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__58__Vfuncout;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__59__sz 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__59__r 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__59__m 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__59__Vfuncout 
                                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__59__m))
                                        ? 1U : ((6U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__59__m))
                                                 ? 1U
                                                 : 
                                                ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__59__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__59__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__59__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__59__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__59__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__59__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__59__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__59__Vfuncout;
                            }
                        } else {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__60__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__60__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__60__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__60__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__60__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__60__Vfuncout;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__61__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__61__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__61__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__61__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__61__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__61__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__61__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__61__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__61__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__61__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__61__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__61__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__61__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__61__Vfuncout;
                        }
                    } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__62__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__62__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__62__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__62__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__62__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__62__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__62__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__62__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__62__r))
                                                    ? 0U
                                                    : 1U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__62__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__62__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__62__Vfuncout;
                        } else {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__63__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__63__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__63__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__63__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__63__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__63__Vfuncout;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__64__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__64__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__64__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__64__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__64__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__64__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__64__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__64__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__64__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__64__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__64__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__64__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__64__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__64__Vfuncout;
                        }
                    } else {
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__65__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__65__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__65__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__65__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__65__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__65__Vfuncout;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__66__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__66__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__66__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__66__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__66__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__66__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__66__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__66__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__66__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__66__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__66__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__66__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__66__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__66__Vfuncout;
                    }
                } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0bU;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__67__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 0U;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__67__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__67__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__67__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__67__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__67__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__67__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__67__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__67__r))
                                                     ? 0U
                                                     : 1U))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__67__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__67__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__67__Vfuncout;
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x12U;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__68__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__68__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__68__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__68__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__68__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__68__Vfuncout;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__69__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__69__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__69__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__69__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__69__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__69__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__69__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__69__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__69__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__69__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__69__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__69__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__69__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__69__Vfuncout;
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x12U;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__70__s 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__70__Vfuncout 
                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__70__s))
                            ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__70__s))
                                     ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__70__s))
                                              ? 2U : 1U)));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__70__Vfuncout;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__71__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__71__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__71__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__71__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__71__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__71__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__71__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__71__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__71__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__71__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__71__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__71__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__71__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__71__Vfuncout;
                }
            } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__72__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 2U;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__72__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__72__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__72__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__72__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__72__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__72__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__72__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__72__r))
                                                    ? 0U
                                                    : 2U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__72__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__72__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__72__Vfuncout;
                        } else {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__73__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__73__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__73__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__73__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__73__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__73__Vfuncout;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__74__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__74__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__74__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__74__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__74__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__74__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__74__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__74__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__74__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__74__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__74__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__74__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__74__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__74__Vfuncout;
                        }
                    } else {
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__75__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__75__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__75__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__75__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__75__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__75__Vfuncout;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__76__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__76__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__76__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__76__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__76__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__76__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__76__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__76__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__76__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__76__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__76__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__76__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__76__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__76__Vfuncout;
                    }
                } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__77__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__77__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__77__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__77__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__77__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__77__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__77__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__77__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__77__r))
                                                     ? 0U
                                                     : 1U))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__77__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__77__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__77__Vfuncout;
                    } else {
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__78__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__78__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__78__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__78__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__78__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__78__Vfuncout;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__79__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__79__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__79__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__79__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__79__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__79__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__79__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__79__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__79__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__79__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__79__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__79__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__79__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__79__Vfuncout;
                    }
                } else {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__80__s 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__80__Vfuncout 
                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__80__s))
                            ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__80__s))
                                     ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__80__s))
                                              ? 2U : 1U)));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__80__Vfuncout;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__81__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__81__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__81__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__81__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__81__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__81__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__81__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__81__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__81__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__81__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__81__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__81__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__81__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__81__Vfuncout;
                }
            } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x19U;
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                                ? 2U : 1U);
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                        if (((5U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                           >> 3U))) 
                             | (6U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U))))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 2U;
                        } else if ((7U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 3U)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = ((0U == (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))
                                    ? 2U : ((1U == 
                                             (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))
                                             ? 3U : 2U));
                        }
                    } else if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x19U;
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                                ? 2U : 1U);
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                        if (((5U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                           >> 3U))) 
                             | (6U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U))))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 2U;
                        } else if ((7U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 3U)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = ((0U == (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))
                                    ? 2U : ((1U == 
                                             (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))
                                             ? 3U : 2U));
                        }
                    } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x19U;
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                                ? 2U : 1U);
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                        if (((5U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                           >> 3U))) 
                             | (6U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U))))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 2U;
                        } else if ((7U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 3U)))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = ((0U == (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))
                                    ? 2U : ((1U == 
                                             (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))
                                             ? 3U : 2U));
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x13U;
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 2U;
                    }
                } else if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x19U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                            ? 2U : 1U);
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                    if (((5U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                       >> 3U))) | (6U 
                                                   == 
                                                   (7U 
                                                    & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                       >> 3U))))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 2U;
                    } else if ((7U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = ((0U == (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))
                                ? 2U : ((1U == (7U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))
                                         ? 3U : 2U));
                    }
                } else if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x19U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                            ? 2U : 1U);
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                    if (((5U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                       >> 3U))) | (6U 
                                                   == 
                                                   (7U 
                                                    & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                       >> 3U))))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 2U;
                    } else if ((7U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = ((0U == (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))
                                ? 2U : ((1U == (7U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))
                                         ? 3U : 2U));
                    }
                } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x19U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))
                            ? 2U : 1U);
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                    if (((5U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                       >> 3U))) | (6U 
                                                   == 
                                                   (7U 
                                                    & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                       >> 3U))))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 2U;
                    } else if ((7U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U)))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = ((0U == (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))
                                ? 2U : ((1U == (7U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op)))
                                         ? 3U : 2U));
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x13U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                }
            } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x18U;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__82__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 2U;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__82__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__82__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__82__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__82__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__82__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__82__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__82__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__82__r))
                                                    ? 0U
                                                    : 2U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__82__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__82__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__82__Vfuncout;
                } else if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x18U;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__83__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 2U;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__83__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__83__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__83__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__83__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__83__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__83__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__83__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__83__r))
                                                    ? 0U
                                                    : 2U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__83__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__83__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__83__Vfuncout;
                } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x18U;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__84__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 2U;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__84__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__84__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__84__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__84__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__84__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__84__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__84__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__84__r))
                                                    ? 0U
                                                    : 2U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__84__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__84__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__84__Vfuncout;
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x14U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 2U;
                }
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__85__s 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 6U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__85__Vfuncout 
                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__85__s))
                        ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__85__s))
                                 ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__85__s))
                                          ? 2U : 1U)));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__85__Vfuncout;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__86__sz 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__86__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__86__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__86__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__86__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__86__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__86__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__86__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__86__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__86__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__86__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__86__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__86__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__86__Vfuncout;
            }
        } else if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x26U;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__87__r 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__87__m 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__87__Vfuncout 
                                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__87__m))
                                        ? 1U : ((6U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__87__m))
                                                 ? 1U
                                                 : 
                                                ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__87__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__87__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__87__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__87__r))
                                                     ? 0U
                                                     : 1U))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__87__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__87__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__87__Vfuncout;
                            } else {
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__88__s 
                                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 6U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__88__Vfuncout 
                                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__88__s))
                                        ? 0U : ((1U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__88__s))
                                                 ? 1U
                                                 : 
                                                ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__88__s))
                                                  ? 2U
                                                  : 1U)));
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__88__Vfuncout;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__89__sz 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__89__r 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__89__m 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__89__Vfuncout 
                                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__89__m))
                                        ? 1U : ((6U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__89__m))
                                                 ? 1U
                                                 : 
                                                ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__89__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__89__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__89__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__89__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__89__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__89__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__89__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__89__Vfuncout;
                            }
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__90__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__90__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__90__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__90__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__90__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__90__Vfuncout;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__91__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__91__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__91__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__91__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__91__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__91__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__91__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__91__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__91__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__91__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__91__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__91__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__91__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__91__Vfuncout;
                        }
                    } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x25U;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__92__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__92__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__92__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__92__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__92__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__92__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__92__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__92__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__92__r))
                                                    ? 0U
                                                    : 1U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__92__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__92__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__92__Vfuncout;
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__93__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__93__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__93__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__93__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__93__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__93__Vfuncout;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__94__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__94__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__94__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__94__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__94__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__94__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__94__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__94__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__94__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__94__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__94__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__94__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__94__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__94__Vfuncout;
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__95__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__95__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__95__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__95__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__95__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__95__Vfuncout;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__96__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__96__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__96__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__96__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__96__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__96__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__96__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__96__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__96__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__96__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__96__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__96__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__96__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__96__Vfuncout;
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x10U;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__97__s 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__97__Vfuncout 
                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__97__s))
                            ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__97__s))
                                     ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__97__s))
                                              ? 2U : 1U)));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__97__Vfuncout;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__98__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__98__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__98__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__98__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__98__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__98__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__98__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__98__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__98__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__98__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__98__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__98__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__98__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__98__Vfuncout;
                }
            } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x26U;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__99__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__99__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__99__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__99__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__99__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__99__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__99__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__99__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__99__r))
                                                    ? 0U
                                                    : 1U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__99__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__99__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__99__Vfuncout;
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__100__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__100__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__100__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__100__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__100__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__100__Vfuncout;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__101__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__101__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__101__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__101__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__101__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__101__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__101__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__101__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__101__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__101__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__101__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__101__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__101__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__101__Vfuncout;
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__102__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__102__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__102__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__102__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__102__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__102__Vfuncout;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__103__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__103__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__103__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__103__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__103__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__103__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__103__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__103__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__103__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__103__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__103__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__103__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__103__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__103__Vfuncout;
                    }
                } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x25U;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__104__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__104__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__104__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__104__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__104__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__104__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__104__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__104__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__104__r))
                                                     ? 0U
                                                     : 1U))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__104__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__104__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__104__Vfuncout;
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__105__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__105__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__105__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__105__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__105__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__105__Vfuncout;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__106__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__106__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__106__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__106__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__106__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__106__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__106__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__106__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__106__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__106__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__106__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__106__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__106__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__106__Vfuncout;
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__107__s 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__107__Vfuncout 
                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__107__s))
                            ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__107__s))
                                     ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__107__s))
                                              ? 2U : 1U)));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__107__Vfuncout;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__108__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__108__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__108__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__108__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__108__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__108__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__108__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__108__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__108__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__108__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__108__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__108__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__108__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__108__Vfuncout;
                }
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0fU;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__109__s 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 6U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__109__Vfuncout 
                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__109__s))
                        ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__109__s))
                                 ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__109__s))
                                          ? 2U : 1U)));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__109__Vfuncout;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__110__sz 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__110__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__110__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__110__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__110__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__110__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__110__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__110__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__110__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__110__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__110__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__110__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__110__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__110__Vfuncout;
            }
        } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x26U;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__111__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__111__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__111__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__111__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__111__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__111__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__111__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__111__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__111__r))
                                                    ? 0U
                                                    : 1U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__111__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__111__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__111__Vfuncout;
                        } else {
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__112__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__112__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__112__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__112__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__112__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__112__Vfuncout;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__113__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__113__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__113__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__113__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__113__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__113__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__113__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__113__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__113__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__113__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__113__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__113__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__113__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__113__Vfuncout;
                        }
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__114__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__114__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__114__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__114__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__114__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__114__Vfuncout;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__115__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__115__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__115__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__115__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__115__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__115__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__115__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__115__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__115__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__115__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__115__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__115__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__115__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__115__Vfuncout;
                    }
                } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x25U;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__116__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__116__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__116__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__116__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__116__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__116__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__116__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__116__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__116__r))
                                                     ? 0U
                                                     : 1U))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__116__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__116__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__116__Vfuncout;
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__117__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__117__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__117__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__117__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__117__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__117__Vfuncout;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__118__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__118__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__118__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__118__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__118__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__118__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__118__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__118__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__118__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__118__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__118__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__118__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__118__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__118__Vfuncout;
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__119__s 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__119__Vfuncout 
                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__119__s))
                            ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__119__s))
                                     ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__119__s))
                                              ? 2U : 1U)));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__119__Vfuncout;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__120__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__120__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__120__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__120__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__120__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__120__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__120__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__120__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__120__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__120__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__120__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__120__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__120__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__120__Vfuncout;
                }
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x11U;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__121__s 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 6U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__121__Vfuncout 
                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__121__s))
                        ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__121__s))
                                 ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__121__s))
                                          ? 2U : 1U)));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__121__Vfuncout;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__122__sz 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__122__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__122__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__122__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__122__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__122__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__122__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__122__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__122__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__122__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__122__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__122__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__122__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__122__Vfuncout;
            }
        } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x26U;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__123__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__123__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__123__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__123__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__123__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__123__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__123__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__123__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__123__r))
                                                     ? 0U
                                                     : 1U))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__123__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__123__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__123__Vfuncout;
                    } else {
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__124__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__124__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__124__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__124__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__124__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__124__Vfuncout;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__125__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__125__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__125__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__125__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__125__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__125__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__125__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__125__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__125__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__125__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__125__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__125__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__125__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__125__Vfuncout;
                    }
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__126__s 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__126__Vfuncout 
                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__126__s))
                            ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__126__s))
                                     ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__126__s))
                                              ? 2U : 1U)));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__126__Vfuncout;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__127__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__127__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__127__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__127__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__127__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__127__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__127__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__127__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__127__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__127__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__127__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__127__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__127__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__127__Vfuncout;
                }
            } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x25U;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__128__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__128__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__128__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__128__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__128__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__128__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__128__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__128__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__128__r))
                                                    ? 0U
                                                    : 1U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__128__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__128__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__128__Vfuncout;
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__129__s 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__129__Vfuncout 
                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__129__s))
                            ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__129__s))
                                     ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__129__s))
                                              ? 2U : 1U)));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__129__Vfuncout;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__130__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__130__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__130__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__130__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__130__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__130__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__130__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__130__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__130__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__130__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__130__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__130__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__130__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__130__Vfuncout;
                }
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__131__s 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 6U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__131__Vfuncout 
                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__131__s))
                        ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__131__s))
                                 ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__131__s))
                                          ? 2U : 1U)));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__131__Vfuncout;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__132__sz 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__132__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__132__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__132__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__132__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__132__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__132__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__132__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__132__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__132__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__132__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__132__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__132__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__132__Vfuncout;
            }
        } else if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x27U;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__133__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__133__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__133__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__133__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__133__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__133__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__133__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__133__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__133__r))
                                                   ? 0U
                                                   : 1U))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__133__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__133__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__133__Vfuncout;
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__134__s 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 6U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__134__Vfuncout 
                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__134__s))
                        ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__134__s))
                                 ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__134__s))
                                          ? 2U : 1U)));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__134__Vfuncout;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__135__sz 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__135__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__135__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__135__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__135__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__135__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__135__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__135__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__135__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__135__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__135__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__135__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__135__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__135__Vfuncout;
            }
        } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x25U;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__136__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__136__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__136__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__136__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__136__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__136__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__136__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__136__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__136__r))
                                                   ? 0U
                                                   : 1U))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__136__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__136__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__136__Vfuncout;
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__137__s 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 6U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__137__Vfuncout 
                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__137__s))
                        ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__137__s))
                                 ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__137__s))
                                          ? 2U : 1U)));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__137__Vfuncout;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__138__sz 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__138__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__138__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__138__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__138__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__138__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__138__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__138__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__138__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__138__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__138__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__138__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__138__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__138__Vfuncout;
            }
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0aU;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__139__s 
                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 6U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__139__Vfuncout 
                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__139__s))
                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__139__s))
                             ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__139__s))
                                      ? 2U : 1U)));
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__139__Vfuncout;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__140__sz 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__140__r 
                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__140__m 
                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 3U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__140__Vfuncout 
                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__140__m))
                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__140__m))
                             ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__140__m))
                                      ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__140__r))
                                          ? ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__140__r))
                                              ? 0U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__140__r))
                                               ? 0U
                                               : ((2U 
                                                   == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__140__sz))
                                                   ? 2U
                                                   : 1U)))
                                          : ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__140__r))
                                              ? 1U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__140__r))
                                               ? 2U
                                               : 1U)))
                                      : 0U)));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__140__Vfuncout;
        }
    } else if ((0x00002000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
        if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                            >> 0x0cU))) | (2U == (3U 
                                                  & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                     >> 0x0cU)))) 
             | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 0x0cU))))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
                = ((1U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U))) ? 0x0eU : 8U);
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__141__s 
                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 0x0cU));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__141__Vfuncout 
                = ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__141__s))
                    ? 0U : ((3U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__141__s))
                             ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__141__s))
                                      ? 2U : 1U)));
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__141__Vfuncout;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__142__sz 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__142__r 
                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__142__m 
                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 3U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__142__Vfuncout 
                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__142__m))
                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__142__m))
                             ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__142__m))
                                      ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__142__r))
                                          ? ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__142__r))
                                              ? 0U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__142__r))
                                               ? 0U
                                               : ((2U 
                                                   == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__142__sz))
                                                   ? 2U
                                                   : 1U)))
                                          : ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__142__r))
                                              ? 1U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__142__r))
                                               ? 2U
                                               : 1U)))
                                      : 0U)));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__142__Vfuncout;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__143__sz 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__143__r 
                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 9U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__143__m 
                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 6U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__143__Vfuncout 
                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__143__m))
                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__143__m))
                             ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__143__m))
                                      ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__143__r))
                                          ? ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__143__r))
                                              ? 0U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__143__r))
                                               ? 0U
                                               : ((2U 
                                                   == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__143__sz))
                                                   ? 2U
                                                   : 1U)))
                                          : ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__143__r))
                                              ? 1U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__143__r))
                                               ? 2U
                                               : 1U)))
                                      : 0U)));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__143__Vfuncout;
        }
    } else if ((0x00001000U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
        if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                            >> 0x0cU))) | (2U == (3U 
                                                  & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                     >> 0x0cU)))) 
             | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 0x0cU))))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
                = ((1U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U))) ? 0x0eU : 8U);
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__144__s 
                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 0x0cU));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__144__Vfuncout 
                = ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__144__s))
                    ? 0U : ((3U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__144__s))
                             ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__144__s))
                                      ? 2U : 1U)));
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__144__Vfuncout;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__145__sz 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__145__r 
                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__145__m 
                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 3U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__145__Vfuncout 
                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__145__m))
                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__145__m))
                             ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__145__m))
                                      ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__145__r))
                                          ? ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__145__r))
                                              ? 0U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__145__r))
                                               ? 0U
                                               : ((2U 
                                                   == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__145__sz))
                                                   ? 2U
                                                   : 1U)))
                                          : ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__145__r))
                                              ? 1U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__145__r))
                                               ? 2U
                                               : 1U)))
                                      : 0U)));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__145__Vfuncout;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__146__sz 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__146__r 
                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 9U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__146__m 
                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 6U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__146__Vfuncout 
                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__146__m))
                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__146__m))
                             ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__146__m))
                                      ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__146__r))
                                          ? ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__146__r))
                                              ? 0U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__146__r))
                                               ? 0U
                                               : ((2U 
                                                   == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__146__sz))
                                                   ? 2U
                                                   : 1U)))
                                          : ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__146__r))
                                              ? 1U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__146__r))
                                               ? 2U
                                               : 1U)))
                                      : 0U)));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__146__Vfuncout;
        }
    } else if ((0x00000800U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
        if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                    >> 0x0cU))) | (2U 
                                                   == 
                                                   (3U 
                                                    & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                       >> 0x0cU)))) 
                     | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 0x0cU))))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
                        = ((1U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U))) ? 0x0eU
                            : 8U);
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__147__s 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 0x0cU));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__147__Vfuncout 
                        = ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__147__s))
                            ? 0U : ((3U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__147__s))
                                     ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__147__s))
                                              ? 2U : 1U)));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__147__Vfuncout;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__148__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__148__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__148__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__148__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__148__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__148__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__148__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__148__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__148__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__148__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__148__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__148__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__148__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__148__Vfuncout;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__149__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__149__r 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 9U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__149__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__149__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__149__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__149__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__149__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__149__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__149__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__149__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__149__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__149__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__149__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__149__Vfuncout;
                }
            } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                    >> 0x0cU))) | (2U 
                                                   == 
                                                   (3U 
                                                    & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                       >> 0x0cU)))) 
                     | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 0x0cU))))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
                        = ((1U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U))) ? 0x0eU
                            : 8U);
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__150__s 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 0x0cU));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__150__Vfuncout 
                        = ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__150__s))
                            ? 0U : ((3U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__150__s))
                                     ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__150__s))
                                              ? 2U : 1U)));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__150__Vfuncout;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__151__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__151__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__151__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__151__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__151__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__151__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__151__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__151__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__151__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__151__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__151__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__151__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__151__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__151__Vfuncout;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__152__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__152__r 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 9U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__152__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__152__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__152__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__152__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__152__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__152__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__152__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__152__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__152__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__152__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__152__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__152__Vfuncout;
                }
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0dU;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__153__s 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 6U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__153__Vfuncout 
                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__153__s))
                        ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__153__s))
                                 ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__153__s))
                                          ? 2U : 1U)));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__153__Vfuncout;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                        ? 2U : 1U);
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__154__sz 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__154__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__154__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__154__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__154__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__154__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__154__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__154__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__154__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__154__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__154__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__154__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__154__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__154__Vfuncout;
            }
        } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                    >> 0x0cU))) | (2U 
                                                   == 
                                                   (3U 
                                                    & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                       >> 0x0cU)))) 
                     | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 0x0cU))))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
                        = ((1U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U))) ? 0x0eU
                            : 8U);
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__155__s 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 0x0cU));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__155__Vfuncout 
                        = ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__155__s))
                            ? 0U : ((3U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__155__s))
                                     ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__155__s))
                                              ? 2U : 1U)));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__155__Vfuncout;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__156__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__156__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__156__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__156__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__156__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__156__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__156__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__156__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__156__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__156__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__156__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__156__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__156__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__156__Vfuncout;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__157__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__157__r 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 9U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__157__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__157__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__157__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__157__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__157__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__157__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__157__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__157__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__157__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__157__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__157__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__157__Vfuncout;
                }
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0dU;
                if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__158__s 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__158__Vfuncout 
                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__158__s))
                            ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__158__s))
                                     ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__158__s))
                                              ? 2U : 1U)));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__158__Vfuncout;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                            ? 2U : 1U);
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__159__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__159__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__159__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__159__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__159__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__159__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__159__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__159__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__159__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__159__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__159__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__159__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__159__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__159__Vfuncout;
                } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                    if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__160__s 
                                            = (3U & 
                                               ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                >> 6U));
                                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__160__Vfuncout 
                                            = ((0U 
                                                == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__160__s))
                                                ? 0U
                                                : (
                                                   (1U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__160__s))
                                                    ? 1U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__160__s))
                                                     ? 2U
                                                     : 1U)));
                                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__160__Vfuncout;
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                            = ((2U 
                                                == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                                ? 2U
                                                : 1U);
                                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__161__sz 
                                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__161__r 
                                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__161__m 
                                            = (7U & 
                                               ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                >> 3U));
                                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__161__Vfuncout 
                                            = ((5U 
                                                == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__161__m))
                                                ? 1U
                                                : (
                                                   (6U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__161__m))
                                                    ? 1U
                                                    : 
                                                   ((7U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__161__m))
                                                     ? 
                                                    ((4U 
                                                      & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__161__r))
                                                      ? 
                                                     ((2U 
                                                       & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__161__r))
                                                       ? 0U
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__161__r))
                                                        ? 0U
                                                        : 
                                                       ((2U 
                                                         == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__161__sz))
                                                         ? 2U
                                                         : 1U)))
                                                      : 
                                                     ((2U 
                                                       & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__161__r))
                                                       ? 1U
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__161__r))
                                                        ? 2U
                                                        : 1U)))
                                                     : 0U)));
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__161__Vfuncout;
                                    } else if ((1U 
                                                & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__162__s 
                                            = (3U & 
                                               ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                >> 6U));
                                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__162__Vfuncout 
                                            = ((0U 
                                                == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__162__s))
                                                ? 0U
                                                : (
                                                   (1U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__162__s))
                                                    ? 1U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__162__s))
                                                     ? 2U
                                                     : 1U)));
                                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__162__Vfuncout;
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                            = ((2U 
                                                == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                                ? 2U
                                                : 1U);
                                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__163__sz 
                                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__163__r 
                                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__163__m 
                                            = (7U & 
                                               ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                >> 3U));
                                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__163__Vfuncout 
                                            = ((5U 
                                                == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__163__m))
                                                ? 1U
                                                : (
                                                   (6U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__163__m))
                                                    ? 1U
                                                    : 
                                                   ((7U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__163__m))
                                                     ? 
                                                    ((4U 
                                                      & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__163__r))
                                                      ? 
                                                     ((2U 
                                                       & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__163__r))
                                                       ? 0U
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__163__r))
                                                        ? 0U
                                                        : 
                                                       ((2U 
                                                         == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__163__sz))
                                                         ? 2U
                                                         : 1U)))
                                                      : 
                                                     ((2U 
                                                       & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__163__r))
                                                       ? 1U
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__163__r))
                                                        ? 2U
                                                        : 1U)))
                                                     : 0U)));
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__163__Vfuncout;
                                    } else {
                                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                                    }
                                } else {
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__164__s 
                                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 6U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__164__Vfuncout 
                                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__164__s))
                                            ? 0U : 
                                           ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__164__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__164__s))
                                              ? 2U : 1U)));
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__164__Vfuncout;
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                        = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                            ? 2U : 1U);
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__165__sz 
                                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__165__r 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__165__m 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 3U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__165__Vfuncout 
                                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__165__m))
                                            ? 1U : 
                                           ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__165__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__165__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__165__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__165__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__165__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__165__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__165__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__165__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__165__Vfuncout;
                                }
                            } else {
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__166__s 
                                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 6U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__166__Vfuncout 
                                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__166__s))
                                        ? 0U : ((1U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__166__s))
                                                 ? 1U
                                                 : 
                                                ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__166__s))
                                                  ? 2U
                                                  : 1U)));
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__166__Vfuncout;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                    = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                        ? 2U : 1U);
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__167__sz 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__167__r 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__167__m 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__167__Vfuncout 
                                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__167__m))
                                        ? 1U : ((6U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__167__m))
                                                 ? 1U
                                                 : 
                                                ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__167__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__167__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__167__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__167__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__167__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__167__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__167__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__167__Vfuncout;
                            }
                        } else {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__168__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__168__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__168__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__168__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__168__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__168__Vfuncout;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                    ? 2U : 1U);
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__169__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__169__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__169__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__169__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__169__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__169__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__169__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__169__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__169__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__169__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__169__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__169__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__169__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__169__Vfuncout;
                        }
                    } else {
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__170__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__170__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__170__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__170__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__170__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__170__Vfuncout;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                ? 2U : 1U);
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__171__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__171__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__171__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__171__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__171__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__171__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__171__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__171__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__171__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__171__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__171__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__171__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__171__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__171__Vfuncout;
                    }
                } else if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__172__s 
                                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 6U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__172__Vfuncout 
                                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__172__s))
                                            ? 0U : 
                                           ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__172__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__172__s))
                                              ? 2U : 1U)));
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__172__Vfuncout;
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                        = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                            ? 2U : 1U);
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__173__sz 
                                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__173__r 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__173__m 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 3U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__173__Vfuncout 
                                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__173__m))
                                            ? 1U : 
                                           ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__173__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__173__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__173__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__173__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__173__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__173__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__173__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__173__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__173__Vfuncout;
                                } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__174__s 
                                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 6U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__174__Vfuncout 
                                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__174__s))
                                            ? 0U : 
                                           ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__174__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__174__s))
                                              ? 2U : 1U)));
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__174__Vfuncout;
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                        = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                            ? 2U : 1U);
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__175__sz 
                                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__175__r 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__175__m 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 3U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__175__Vfuncout 
                                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__175__m))
                                            ? 1U : 
                                           ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__175__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__175__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__175__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__175__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__175__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__175__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__175__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__175__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__175__Vfuncout;
                                } else {
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 0U;
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                                }
                            } else {
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__176__s 
                                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 6U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__176__Vfuncout 
                                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__176__s))
                                        ? 0U : ((1U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__176__s))
                                                 ? 1U
                                                 : 
                                                ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__176__s))
                                                  ? 2U
                                                  : 1U)));
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__176__Vfuncout;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                    = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                        ? 2U : 1U);
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__177__sz 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__177__r 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__177__m 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__177__Vfuncout 
                                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__177__m))
                                        ? 1U : ((6U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__177__m))
                                                 ? 1U
                                                 : 
                                                ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__177__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__177__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__177__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__177__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__177__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__177__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__177__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__177__Vfuncout;
                            }
                        } else {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__178__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__178__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__178__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__178__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__178__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__178__Vfuncout;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                    ? 2U : 1U);
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__179__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__179__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__179__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__179__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__179__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__179__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__179__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__179__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__179__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__179__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__179__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__179__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__179__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__179__Vfuncout;
                        }
                    } else {
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__180__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__180__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__180__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__180__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__180__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__180__Vfuncout;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                ? 2U : 1U);
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__181__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__181__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__181__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__181__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__181__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__181__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__181__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__181__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__181__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__181__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__181__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__181__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__181__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__181__Vfuncout;
                    }
                } else {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__182__s 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__182__Vfuncout 
                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__182__s))
                            ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__182__s))
                                     ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__182__s))
                                              ? 2U : 1U)));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__182__Vfuncout;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                            ? 2U : 1U);
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__183__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__183__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__183__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__183__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__183__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__183__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__183__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__183__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__183__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__183__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__183__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__183__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__183__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__183__Vfuncout;
                }
            }
        } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                >> 0x0cU))) | (2U == 
                                               (3U 
                                                & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                   >> 0x0cU)))) 
                 | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 0x0cU))))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
                    = ((1U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U))) ? 0x0eU
                        : 8U);
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__184__s 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 0x0cU));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__184__Vfuncout 
                    = ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__184__s))
                        ? 0U : ((3U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__184__s))
                                 ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__184__s))
                                          ? 2U : 1U)));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__184__Vfuncout;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__185__sz 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__185__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__185__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__185__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__185__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__185__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__185__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__185__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__185__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__185__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__185__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__185__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__185__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__185__Vfuncout;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__186__sz 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__186__r 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 9U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__186__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 6U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__186__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__186__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__186__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__186__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__186__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__186__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__186__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__186__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__186__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__186__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__186__Vfuncout;
            }
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x1bU;
            if ((0U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                              >> 3U)))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__187__sz = 2U;
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 2U;
            } else {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__187__sz = 0U;
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 0U;
            }
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__187__r 
                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__187__m 
                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 3U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__187__Vfuncout 
                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__187__m))
                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__187__m))
                             ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__187__m))
                                      ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__187__r))
                                          ? ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__187__r))
                                              ? 0U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__187__r))
                                               ? 0U
                                               : ((2U 
                                                   == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__187__sz))
                                                   ? 2U
                                                   : 1U)))
                                          : ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__187__r))
                                              ? 1U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__187__r))
                                               ? 2U
                                               : 1U)))
                                      : 0U)));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__187__Vfuncout;
        }
    } else if ((0x00000400U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
        if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x1bU;
                if ((0U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                  >> 3U)))) {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__188__sz = 2U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 2U;
                } else {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__188__sz = 0U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 0U;
                }
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__188__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__188__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__188__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__188__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__188__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__188__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__188__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__188__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__188__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__188__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__188__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__188__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__188__Vfuncout;
            } else if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                       >> 0x0cU))) 
                         | (2U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 0x0cU)))) 
                        | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                        >> 0x0cU))))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
                    = ((1U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U))) ? 0x0eU
                        : 8U);
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__189__s 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 0x0cU));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__189__Vfuncout 
                    = ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__189__s))
                        ? 0U : ((3U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__189__s))
                                 ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__189__s))
                                          ? 2U : 1U)));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__189__Vfuncout;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__190__sz 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__190__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__190__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__190__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__190__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__190__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__190__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__190__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__190__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__190__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__190__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__190__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__190__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__190__Vfuncout;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__191__sz 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__191__r 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 9U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__191__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 6U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__191__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__191__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__191__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__191__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__191__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__191__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__191__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__191__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__191__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__191__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__191__Vfuncout;
            }
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0dU;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__192__s 
                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 6U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__192__Vfuncout 
                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__192__s))
                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__192__s))
                             ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__192__s))
                                      ? 2U : 1U)));
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__192__Vfuncout;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                    ? 2U : 1U);
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__193__sz 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__193__r 
                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__193__m 
                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 3U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__193__Vfuncout 
                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__193__m))
                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__193__m))
                             ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__193__m))
                                      ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__193__r))
                                          ? ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__193__r))
                                              ? 0U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__193__r))
                                               ? 0U
                                               : ((2U 
                                                   == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__193__sz))
                                                   ? 2U
                                                   : 1U)))
                                          : ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__193__r))
                                              ? 1U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__193__r))
                                               ? 2U
                                               : 1U)))
                                      : 0U)));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__193__Vfuncout;
        }
    } else if ((0x00000200U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
        if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x1bU;
                if ((0U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                  >> 3U)))) {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__194__sz = 2U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 2U;
                } else {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__194__sz = 0U;
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 0U;
                }
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__194__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__194__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__194__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__194__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__194__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__194__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__194__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__194__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__194__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__194__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__194__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__194__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__194__Vfuncout;
            } else if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                       >> 0x0cU))) 
                         | (2U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 0x0cU)))) 
                        | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                        >> 0x0cU))))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
                    = ((1U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U))) ? 0x0eU
                        : 8U);
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__195__s 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 0x0cU));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__195__Vfuncout 
                    = ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__195__s))
                        ? 0U : ((3U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__195__s))
                                 ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__195__s))
                                          ? 2U : 1U)));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__195__Vfuncout;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__196__sz 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__196__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__196__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__196__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__196__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__196__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__196__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__196__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__196__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__196__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__196__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__196__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__196__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__196__Vfuncout;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__197__sz 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__197__r 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 9U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__197__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 6U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__197__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__197__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__197__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__197__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__197__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__197__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__197__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__197__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__197__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__197__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__197__Vfuncout;
            }
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0dU;
            if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__198__s 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 6U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__198__Vfuncout 
                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__198__s))
                        ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__198__s))
                                 ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__198__s))
                                          ? 2U : 1U)));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__198__Vfuncout;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                        ? 2U : 1U);
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__199__sz 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__199__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__199__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__199__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__199__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__199__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__199__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__199__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__199__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__199__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__199__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__199__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__199__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__199__Vfuncout;
            } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__200__s 
                                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 6U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__200__Vfuncout 
                                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__200__s))
                                            ? 0U : 
                                           ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__200__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__200__s))
                                              ? 2U : 1U)));
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__200__Vfuncout;
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                        = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                            ? 2U : 1U);
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__201__sz 
                                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__201__r 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__201__m 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 3U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__201__Vfuncout 
                                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__201__m))
                                            ? 1U : 
                                           ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__201__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__201__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__201__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__201__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__201__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__201__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__201__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__201__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__201__Vfuncout;
                                } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__202__s 
                                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 6U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__202__Vfuncout 
                                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__202__s))
                                            ? 0U : 
                                           ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__202__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__202__s))
                                              ? 2U : 1U)));
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__202__Vfuncout;
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                        = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                            ? 2U : 1U);
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__203__sz 
                                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__203__r 
                                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__203__m 
                                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                 >> 3U));
                                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__203__Vfuncout 
                                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__203__m))
                                            ? 1U : 
                                           ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__203__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__203__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__203__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__203__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__203__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__203__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__203__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__203__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__203__Vfuncout;
                                } else {
                                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                                }
                            } else {
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__204__s 
                                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 6U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__204__Vfuncout 
                                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__204__s))
                                        ? 0U : ((1U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__204__s))
                                                 ? 1U
                                                 : 
                                                ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__204__s))
                                                  ? 2U
                                                  : 1U)));
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__204__Vfuncout;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                    = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                        ? 2U : 1U);
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__205__sz 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__205__r 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__205__m 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__205__Vfuncout 
                                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__205__m))
                                        ? 1U : ((6U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__205__m))
                                                 ? 1U
                                                 : 
                                                ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__205__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__205__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__205__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__205__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__205__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__205__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__205__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__205__Vfuncout;
                            }
                        } else {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__206__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__206__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__206__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__206__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__206__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__206__Vfuncout;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                    ? 2U : 1U);
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__207__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__207__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__207__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__207__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__207__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__207__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__207__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__207__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__207__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__207__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__207__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__207__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__207__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__207__Vfuncout;
                        }
                    } else {
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__208__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__208__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__208__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__208__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__208__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__208__Vfuncout;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                ? 2U : 1U);
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__209__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__209__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__209__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__209__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__209__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__209__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__209__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__209__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__209__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__209__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__209__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__209__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__209__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__209__Vfuncout;
                    }
                } else {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__210__s 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__210__Vfuncout 
                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__210__s))
                            ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__210__s))
                                     ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__210__s))
                                              ? 2U : 1U)));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__210__Vfuncout;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                            ? 2U : 1U);
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__211__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__211__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__211__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__211__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__211__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__211__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__211__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__211__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__211__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__211__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__211__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__211__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__211__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__211__Vfuncout;
                }
            } else if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__212__s 
                                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 6U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__212__Vfuncout 
                                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__212__s))
                                        ? 0U : ((1U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__212__s))
                                                 ? 1U
                                                 : 
                                                ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__212__s))
                                                  ? 2U
                                                  : 1U)));
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__212__Vfuncout;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                    = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                        ? 2U : 1U);
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__213__sz 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__213__r 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__213__m 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__213__Vfuncout 
                                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__213__m))
                                        ? 1U : ((6U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__213__m))
                                                 ? 1U
                                                 : 
                                                ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__213__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__213__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__213__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__213__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__213__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__213__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__213__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__213__Vfuncout;
                            } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__214__s 
                                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 6U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__214__Vfuncout 
                                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__214__s))
                                        ? 0U : ((1U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__214__s))
                                                 ? 1U
                                                 : 
                                                ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__214__s))
                                                  ? 2U
                                                  : 1U)));
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__214__Vfuncout;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                    = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                        ? 2U : 1U);
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__215__sz 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__215__r 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__215__m 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__215__Vfuncout 
                                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__215__m))
                                        ? 1U : ((6U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__215__m))
                                                 ? 1U
                                                 : 
                                                ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__215__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__215__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__215__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__215__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__215__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__215__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__215__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__215__Vfuncout;
                            } else {
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 0U;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                            }
                        } else {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__216__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__216__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__216__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__216__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__216__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__216__Vfuncout;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                    ? 2U : 1U);
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__217__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__217__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__217__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__217__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__217__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__217__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__217__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__217__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__217__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__217__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__217__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__217__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__217__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__217__Vfuncout;
                        }
                    } else {
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__218__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__218__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__218__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__218__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__218__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__218__Vfuncout;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                ? 2U : 1U);
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__219__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__219__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__219__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__219__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__219__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__219__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__219__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__219__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__219__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__219__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__219__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__219__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__219__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__219__Vfuncout;
                    }
                } else {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__220__s 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__220__Vfuncout 
                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__220__s))
                            ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__220__s))
                                     ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__220__s))
                                              ? 2U : 1U)));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__220__Vfuncout;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                            ? 2U : 1U);
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__221__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__221__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__221__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__221__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__221__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__221__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__221__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__221__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__221__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__221__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__221__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__221__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__221__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__221__Vfuncout;
                }
            } else {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__222__s 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 6U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__222__Vfuncout 
                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__222__s))
                        ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__222__s))
                                 ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__222__s))
                                          ? 2U : 1U)));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__222__Vfuncout;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                        ? 2U : 1U);
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__223__sz 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__223__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__223__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__223__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__223__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__223__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__223__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__223__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__223__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__223__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__223__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__223__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__223__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__223__Vfuncout;
            }
        }
    } else if ((0x00000100U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x1bU;
            if ((0U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                              >> 3U)))) {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__224__sz = 2U;
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 2U;
            } else {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__224__sz = 0U;
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 0U;
            }
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__224__r 
                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__224__m 
                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 3U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__224__Vfuncout 
                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__224__m))
                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__224__m))
                             ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__224__m))
                                      ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__224__r))
                                          ? ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__224__r))
                                              ? 0U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__224__r))
                                               ? 0U
                                               : ((2U 
                                                   == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__224__sz))
                                                   ? 2U
                                                   : 1U)))
                                          : ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__224__r))
                                              ? 1U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__224__r))
                                               ? 2U
                                               : 1U)))
                                      : 0U)));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__224__Vfuncout;
        } else if ((((1U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                   >> 0x0cU))) | (2U 
                                                  == 
                                                  (3U 
                                                   & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                                      >> 0x0cU)))) 
                    | (3U == (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                    >> 0x0cU))))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind 
                = ((1U == (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U))) ? 0x0eU : 8U);
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__225__s 
                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 0x0cU));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__225__Vfuncout 
                = ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__225__s))
                    ? 0U : ((3U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__225__s))
                             ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__225__s))
                                      ? 2U : 1U)));
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size_move__225__Vfuncout;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__226__sz 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__226__r 
                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__226__m 
                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 3U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__226__Vfuncout 
                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__226__m))
                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__226__m))
                             ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__226__m))
                                      ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__226__r))
                                          ? ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__226__r))
                                              ? 0U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__226__r))
                                               ? 0U
                                               : ((2U 
                                                   == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__226__sz))
                                                   ? 2U
                                                   : 1U)))
                                          : ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__226__r))
                                              ? 1U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__226__r))
                                               ? 2U
                                               : 1U)))
                                      : 0U)));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__226__Vfuncout;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__227__sz 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__227__r 
                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 9U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__227__m 
                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 6U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__227__Vfuncout 
                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__227__m))
                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__227__m))
                             ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__227__m))
                                      ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__227__r))
                                          ? ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__227__r))
                                              ? 0U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__227__r))
                                               ? 0U
                                               : ((2U 
                                                   == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__227__sz))
                                                   ? 2U
                                                   : 1U)))
                                          : ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__227__r))
                                              ? 1U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__227__r))
                                               ? 2U
                                               : 1U)))
                                      : 0U)));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__227__Vfuncout;
        }
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind = 0x0dU;
        if ((0x00000080U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__228__s 
                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 6U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__228__Vfuncout 
                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__228__s))
                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__228__s))
                             ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__228__s))
                                      ? 2U : 1U)));
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__228__Vfuncout;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                    ? 2U : 1U);
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__229__sz 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__229__r 
                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__229__m 
                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 3U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__229__Vfuncout 
                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__229__m))
                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__229__m))
                             ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__229__m))
                                      ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__229__r))
                                          ? ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__229__r))
                                              ? 0U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__229__r))
                                               ? 0U
                                               : ((2U 
                                                   == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__229__sz))
                                                   ? 2U
                                                   : 1U)))
                                          : ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__229__r))
                                              ? 1U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__229__r))
                                               ? 2U
                                               : 1U)))
                                      : 0U)));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__229__Vfuncout;
        } else if ((0x00000040U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__230__s 
                                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 6U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__230__Vfuncout 
                                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__230__s))
                                        ? 0U : ((1U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__230__s))
                                                 ? 1U
                                                 : 
                                                ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__230__s))
                                                  ? 2U
                                                  : 1U)));
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__230__Vfuncout;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                    = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                        ? 2U : 1U);
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__231__sz 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__231__r 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__231__m 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__231__Vfuncout 
                                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__231__m))
                                        ? 1U : ((6U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__231__m))
                                                 ? 1U
                                                 : 
                                                ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__231__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__231__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__231__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__231__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__231__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__231__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__231__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__231__Vfuncout;
                            } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__232__s 
                                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 6U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__232__Vfuncout 
                                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__232__s))
                                        ? 0U : ((1U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__232__s))
                                                 ? 1U
                                                 : 
                                                ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__232__s))
                                                  ? 2U
                                                  : 1U)));
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__232__Vfuncout;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                    = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                        ? 2U : 1U);
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__233__sz 
                                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__233__r 
                                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__233__m 
                                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                             >> 3U));
                                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__233__Vfuncout 
                                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__233__m))
                                        ? 1U : ((6U 
                                                 == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__233__m))
                                                 ? 1U
                                                 : 
                                                ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__233__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__233__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__233__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__233__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__233__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__233__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__233__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__233__Vfuncout;
                            } else {
                                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 1U;
                                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                            }
                        } else {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__234__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__234__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__234__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__234__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__234__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__234__Vfuncout;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                    ? 2U : 1U);
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__235__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__235__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__235__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__235__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__235__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__235__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__235__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__235__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__235__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__235__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__235__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__235__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__235__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__235__Vfuncout;
                        }
                    } else {
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__236__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__236__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__236__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__236__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__236__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__236__Vfuncout;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                ? 2U : 1U);
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__237__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__237__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__237__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__237__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__237__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__237__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__237__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__237__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__237__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__237__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__237__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__237__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__237__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__237__Vfuncout;
                    }
                } else {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__238__s 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__238__Vfuncout 
                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__238__s))
                            ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__238__s))
                                     ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__238__s))
                                              ? 2U : 1U)));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__238__Vfuncout;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                            ? 2U : 1U);
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__239__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__239__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__239__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__239__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__239__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__239__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__239__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__239__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__239__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__239__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__239__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__239__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__239__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__239__Vfuncout;
                }
            } else {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__240__s 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 6U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__240__Vfuncout 
                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__240__s))
                        ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__240__s))
                                 ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__240__s))
                                          ? 2U : 1U)));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__240__Vfuncout;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                        ? 2U : 1U);
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__241__sz 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__241__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__241__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__241__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__241__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__241__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__241__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__241__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__241__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__241__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__241__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__241__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__241__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__241__Vfuncout;
            }
        } else if ((0x00000020U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
            if ((0x00000010U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                    if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__242__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__242__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__242__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__242__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__242__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__242__Vfuncout;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                    ? 2U : 1U);
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__243__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__243__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__243__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__243__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__243__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__243__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__243__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__243__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__243__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__243__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__243__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__243__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__243__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__243__Vfuncout;
                        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op))) {
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__244__s 
                                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 6U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__244__Vfuncout 
                                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__244__s))
                                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__244__s))
                                             ? 1U : 
                                            ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__244__s))
                                              ? 2U : 1U)));
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__244__Vfuncout;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                                = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                    ? 2U : 1U);
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__245__sz 
                                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__245__r 
                                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__245__m 
                                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                         >> 3U));
                            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__245__Vfuncout 
                                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__245__m))
                                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__245__m))
                                             ? 1U : 
                                            ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__245__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__245__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__245__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__245__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__245__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__245__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__245__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__245__Vfuncout;
                        } else {
                            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size = 0U;
                            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words = 1U;
                        }
                    } else {
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__246__s 
                            = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 6U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__246__Vfuncout 
                            = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__246__s))
                                ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__246__s))
                                         ? 1U : ((2U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__246__s))
                                                  ? 2U
                                                  : 1U)));
                        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__246__Vfuncout;
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                            = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                                ? 2U : 1U);
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__247__sz 
                            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__247__r 
                            = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__247__m 
                            = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                     >> 3U));
                        vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__247__Vfuncout 
                            = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__247__m))
                                ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__247__m))
                                         ? 1U : ((7U 
                                                  == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__247__m))
                                                  ? 
                                                 ((4U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__247__r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__247__r))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__247__r))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__247__sz))
                                                      ? 2U
                                                      : 1U)))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__247__r))
                                                    ? 1U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__247__r))
                                                     ? 2U
                                                     : 1U)))
                                                  : 0U)));
                        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                            = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__247__Vfuncout;
                    }
                } else {
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__248__s 
                        = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 6U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__248__Vfuncout 
                        = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__248__s))
                            ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__248__s))
                                     ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__248__s))
                                              ? 2U : 1U)));
                    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__248__Vfuncout;
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                        = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                            ? 2U : 1U);
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__249__sz 
                        = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__249__r 
                        = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__249__m 
                        = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                                 >> 3U));
                    vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__249__Vfuncout 
                        = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__249__m))
                            ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__249__m))
                                     ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__249__m))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__249__r))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__249__r))
                                                   ? 0U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__249__r))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__249__sz))
                                                     ? 2U
                                                     : 1U)))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__249__r))
                                                   ? 1U
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__249__r))
                                                    ? 2U
                                                    : 1U)))
                                              : 0U)));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                        = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__249__Vfuncout;
                }
            } else {
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__250__s 
                    = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 6U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__250__Vfuncout 
                    = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__250__s))
                        ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__250__s))
                                 ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__250__s))
                                          ? 2U : 1U)));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__250__Vfuncout;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                    = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                        ? 2U : 1U);
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__251__sz 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__251__r 
                    = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__251__m 
                    = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                             >> 3U));
                vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__251__Vfuncout 
                    = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__251__m))
                        ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__251__m))
                                 ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__251__m))
                                          ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__251__r))
                                              ? ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__251__r))
                                                  ? 0U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__251__r))
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__251__sz))
                                                    ? 2U
                                                    : 1U)))
                                              : ((2U 
                                                  & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__251__r))
                                                  ? 1U
                                                  : 
                                                 ((1U 
                                                   & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__251__r))
                                                   ? 2U
                                                   : 1U)))
                                          : 0U)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                    = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__251__Vfuncout;
            }
        } else {
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__252__s 
                = (3U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 6U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__252__Vfuncout 
                = ((0U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__252__s))
                    ? 0U : ((1U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__252__s))
                             ? 1U : ((2U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__252__s))
                                      ? 2U : 1U)));
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__size76__252__Vfuncout;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words 
                = ((2U == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size))
                    ? 2U : 1U);
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__253__sz 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_size;
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__253__r 
                = (7U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__253__m 
                = (7U & ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__effective_op) 
                         >> 3U));
            vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__253__Vfuncout 
                = ((5U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__253__m))
                    ? 1U : ((6U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__253__m))
                             ? 1U : ((7U == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__253__m))
                                      ? ((4U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__253__r))
                                          ? ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__253__r))
                                              ? 0U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__253__r))
                                               ? 0U
                                               : ((2U 
                                                   == (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__253__sz))
                                                   ? 2U
                                                   : 1U)))
                                          : ((2U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__253__r))
                                              ? 1U : 
                                             ((1U & (IData)(vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__253__r))
                                               ? 2U
                                               : 1U)))
                                      : 0U)));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words 
                = vlSelfRef.__Vfunc_m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_dec_if__DOT__ea_ext__253__Vfuncout;
        }
    }
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c = 0U;
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
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v = 0U;
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_y 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_val;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = ((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
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
        } else {
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_val 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized;
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_y 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_val;
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__shift_v = 0U;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = ((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
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
        }
    } else if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
        if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
                if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                        = ((m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                            << 0x00000010U) | (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                                               >> 0x10U));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                        = (1U & (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                                 >> 0x0fU));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                        = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y);
                } else {
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                        = (((- (IData)((1U & (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                                              >> 0x0fU)))) 
                            << 0x00000010U) | (0x0000ffffU 
                                               & m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                        = (1U & (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                                 >> 0x0fU));
                    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                        = (0U == (0x0000ffffU & m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b));
                }
            } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                    = ((0xffff0000U & m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b) 
                       | ((0x0000ff00U & ((- (IData)(
                                                     (1U 
                                                      & (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                                                         >> 7U)))) 
                                          << 8U)) | 
                          (0x000000ffU & m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b)));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                    = (1U & (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                             >> 7U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                    = (0U == (0x000000ffU & m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b));
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                    = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_neg;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                    = (0U == m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
            }
        } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y = 0U;
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z = 1U;
            } else {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                    = ((~ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized) 
                       & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask);
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                    = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                              & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                    = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y);
            }
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                = (0x00000001ffffffffULL & (- (QData)((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized))));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum) 
                   & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_c 
                = (0U != m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v 
                = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_neg) 
                   & (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                             & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask)));
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_neg;
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                = (0U == m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
        }
    } else if ((4U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                    = (0x00000080U | m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b);
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                    = (1U & (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b 
                             >> 7U));
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                    = (0U == (0x000000ffU & m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_b));
            } else {
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                    = (0x00000001ffffffffULL & ((QData)((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_sized)) 
                                                - (QData)((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized))));
                m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sub_res 
                    = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum) 
                       & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask);
                vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                    = ((6U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))
                        ? m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a
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
                    = (((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_neg) 
                        != (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_neg)) 
                       & ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__res_neg) 
                          != (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_neg)));
            }
        } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_sized 
                   ^ m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y);
        } else {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_sized 
                   | m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y);
        }
    } else if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
        if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = (m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_sized 
                   & m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
                = (0U != (vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                          & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__msb_mask));
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
                = (0U == vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y);
        } else {
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
                = (0x00000001ffffffffULL & ((QData)((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_sized)) 
                                            - (QData)((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized))));
            m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sub_res 
                = ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum) 
                   & vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__mask);
            vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
                = ((6U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))
                    ? m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_a
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
                = (((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_neg) 
                    != (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_neg)) 
                   & ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__res_neg) 
                      != (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_neg)));
        }
    } else if ((1U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_op_c))) {
        m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__sum 
            = (0x00000001ffffffffULL & ((QData)((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_sized)) 
                                        + (QData)((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized))));
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
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_v 
            = (((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_neg) 
                == (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_neg)) 
               & ((IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__res_neg) 
                  != (IData)(m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__a_neg)));
    } else {
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_y 
            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_n 
            = m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_neg;
        vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__alu_z 
            = (0U == m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__u_alu__DOT__b_sized);
    }
}

void Vm68k_top___024root___nba_sequent__TOP__2(Vm68k_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vm68k_top___024root___nba_sequent__TOP__2\n"); );
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
    // Body
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__is_bcc_uncond 
        = ((0x0cU == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_kind)) 
           & ((0U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_cc)) 
              | (1U == (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_cc))));
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__disp32 
        = ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_needs_disp_word)
            ? (((- (IData)((1U & ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_1) 
                                  >> 0x0000000fU)))) 
                << 0x00000010U) | (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_1))
            : vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_imm);
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__total_words_eff 
        = (7U & ((IData)(1U) + ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_dst_ext_words) 
                                + (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__dec_src_ext_words))));
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_v_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_c_c = 0U;
    m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__wb_main_data_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_n_c = 0U;
    vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__cc_z_c = 0U;
    if ((1U & (~ ((~ (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_valid)) 
                  | (IData)(vlSelfRef.halted))))) {
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
                if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                              >> 4U)))) {
                    if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                        if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                      >> 2U)))) {
                            if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
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
            if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                          >> 4U)))) {
                if ((8U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
                    if ((1U & (~ ((IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind) 
                                  >> 2U)))) {
                        if ((2U & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__ex_kind))) {
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
               | ((IData)(vlSelfRef.halted) | (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__redirect_valid)))) 
           & (IData)(vlSelfRef.m68k_top__DOT__g_core__BRA__0__KET____DOT__u_core__DOT__would_be_complete));
}
