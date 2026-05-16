// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VM68K_TOP__SYMS_H_
#define VERILATED_VM68K_TOP__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vm68k_top.h"

// INCLUDE MODULE CLASSES
#include "Vm68k_top___024root.h"

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES) Vm68k_top__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vm68k_top* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vm68k_top___024root            TOP;

    // CONSTRUCTORS
    Vm68k_top__Syms(VerilatedContext* contextp, const char* namep, Vm68k_top* modelp);
    ~Vm68k_top__Syms();

    // METHODS
    const char* name() const { return TOP.vlNamep; }
};

#endif  // guard
