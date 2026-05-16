// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vm68k_top__pch.h"

//============================================================
// Constructors

Vm68k_top::Vm68k_top(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vm68k_top__Syms(contextp(), _vcname__, this)}
    , clk{vlSymsp->TOP.clk}
    , rst_n{vlSymsp->TOP.rst_n}
    , halted{vlSymsp->TOP.halted}
    , halt_code_flat{vlSymsp->TOP.halt_code_flat}
    , retired_flat{vlSymsp->TOP.retired_flat}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vm68k_top::Vm68k_top(const char* _vcname__)
    : Vm68k_top(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vm68k_top::~Vm68k_top() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vm68k_top___024root___eval_debug_assertions(Vm68k_top___024root* vlSelf);
#endif  // VL_DEBUG
void Vm68k_top___024root___eval_static(Vm68k_top___024root* vlSelf);
void Vm68k_top___024root___eval_initial(Vm68k_top___024root* vlSelf);
void Vm68k_top___024root___eval_settle(Vm68k_top___024root* vlSelf);
void Vm68k_top___024root___eval(Vm68k_top___024root* vlSelf);

void Vm68k_top::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vm68k_top::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vm68k_top___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vm68k_top___024root___eval_static(&(vlSymsp->TOP));
        Vm68k_top___024root___eval_initial(&(vlSymsp->TOP));
        Vm68k_top___024root___eval_settle(&(vlSymsp->TOP));
        vlSymsp->__Vm_didInit = true;
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vm68k_top___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vm68k_top::eventsPending() { return false; }

uint64_t Vm68k_top::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vm68k_top::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vm68k_top___024root___eval_final(Vm68k_top___024root* vlSelf);

VL_ATTR_COLD void Vm68k_top::final() {
    contextp()->executingFinal(true);
    Vm68k_top___024root___eval_final(&(vlSymsp->TOP));
    contextp()->executingFinal(false);
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vm68k_top::hierName() const { return vlSymsp->name(); }
const char* Vm68k_top::modelName() const { return "Vm68k_top"; }
unsigned Vm68k_top::threads() const { return 1; }
void Vm68k_top::prepareClone() const { contextp()->prepareClone(); }
void Vm68k_top::atClone() const {
    contextp()->threadPoolpOnClone();
}
