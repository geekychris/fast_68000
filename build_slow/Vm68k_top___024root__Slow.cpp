// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vm68k_top.h for the primary calling header

#include "Vm68k_top__pch.h"

void Vm68k_top___024root___ctor_var_reset(Vm68k_top___024root* vlSelf);

Vm68k_top___024root::Vm68k_top___024root(Vm68k_top__Syms* symsp, const char* namep)
 {
    vlSymsp = symsp;
    vlNamep = strdup(namep);
    // Reset structure values
    Vm68k_top___024root___ctor_var_reset(this);
}

void Vm68k_top___024root::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

Vm68k_top___024root::~Vm68k_top___024root() {
    VL_DO_DANGLING(std::free(const_cast<char*>(vlNamep)), vlNamep);
}
