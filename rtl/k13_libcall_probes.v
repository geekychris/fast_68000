// =====================================================================
// k13_libcall_probes.v — reusable K1.3 library-call tracer probes
//
// Drop-in instrumentation for spotting which AmigaDOS / exec library
// functions a workload hits, with arguments + (for return-side probes)
// the values their RTS hands back.
//
// These probes are instantiated by `include "k13_libcall_probes.v"`
// from inside the m68k_core.v `is_settled` PC-classification block, so
// every probe has u_rf.regs[i] in scope (D0..D7 = 0..7, A0..A7 = 8..15)
// plus retired / ex_pc.
//
// All probes are gated by a `+define+LIBCALL_<NAME>_TRACE` per LVO so
// you can enable just the ones you care about without paying for the
// rest.  A meta-define `+define+LIBCALL_ALL` turns them all on.
//
// Architecture
// ------------
// K1.3 ExecBase lives at slow $C00276 (verified via chip[$4] = ExecBase).
// Library functions are reached by `JSR LVO(A6)` where A6 = LibraryBase
// and LVO is the negative offset.  Each library has a small jump
// table in front of its base — entry at base + LVO is a 6-byte
// `JMP abs.L` to the implementation.
//
// For exec.library, the JT lives at fixed slow-RAM addresses we can
// probe directly:
//
//     exec function     LVO offset   K1.3 JT entry address
//     ----------------  ----------   ---------------------
//     OpenLibrary        -552         $C00276 - 552 = $C0004E
//     Forbid             -132         $C001F2
//     Permit             -138         $C001EC
//     FindTask           -294         $C00150
//     AllocMem           -198         $C001B0
//     FreeMem            -210         $C001A4
//     Wait               -318         $C00138
//     Signal             -324         $C00132
//     PutMsg             -366         $C00108  (exec.library/PutMsg)
//
// dos.library function addresses depend on DOSBase, which is set at
// runtime when OpenLibrary("dos.library") returns.  The OpenLibrary
// probe below captures the lib name pointer; a follow-up probe can
// be added for specific dos LVOs once DOSBase is known empirically
// from a prior run.  Common dos LVOs are listed in the
// LIBCALL_DOS_*_TRACE comments below.
//
// How probes report
// -----------------
// Each probe prints a single line of the form
//     [LIBCALL] r=<retired> <function>(<arg0>=<val>, <arg1>=<val>, …)
// Adding a Python decoder is straightforward — group by function name,
// count occurrences, correlate args.  See tools/decode_libcall_trace.py.
//
// Adding a new LVO
// ----------------
// 1. Compute the JT entry address: ExecBase ($C00276) + LVO.
// 2. Look up the function signature (Amiga ROM Kernel Reference Manual
//    or the .fd files at github.com/cnvogelg/amitools / amiga-libfd-tools).
// 3. Add a new `ifdef` block below.  Use u_rf.regs[N] (N = 0..7 for
//    D0..D7, 8..15 for A0..A7) to access registers.
// 4. Document the LVO in the table above so future maintainers can find
//    it without re-reading the source.
//
// Adding a non-exec library
// -------------------------
// dos.library / intuition.library / graphics.library bases are set at
// runtime.  Two patterns work:
//
//   (a) Capture the base from OpenLibrary's return value in a prior run,
//       then hardcode the JT addresses in a new probe block.  See
//       LIBCALL_DOS_TRACE_HARDCODED for an example template.
//
//   (b) For non-exec libraries used only once, dynamically detect the
//       base via the OpenLibrary probe — it captures D0 = LibBase at
//       return, and you can grep for that in run.log to find the base
//       for your specific boot.
// =====================================================================

`ifdef LIBCALL_ALL
`define LIBCALL_OPENLIB_TRACE
`define LIBCALL_ALLOCMEM_TRACE
`define LIBCALL_FREEMEM_TRACE
`define LIBCALL_WAIT_TRACE
`define LIBCALL_SIGNAL_TRACE
`define LIBCALL_FINDTASK_TRACE
`define LIBCALL_FORBID_TRACE
`define LIBCALL_PERMIT_TRACE
`define LIBCALL_PUTMSG_TRACE
`endif

`ifdef LIBCALL_OPENLIB_TRACE
    // exec.library/OpenLibrary  LVO -552
    //   IN:  A1 = library-name string ptr, D0 = minimum version
    //   OUT: D0 = LibraryBase (or 0 on failure)
    //
    // Useful for: finding when dos.library / intuition.library are
    // opened, and CAPTURING THEIR BASE ADDRESS so follow-up dos/intuition
    // probes can be wired up.  Watch the next-cycle PC after this
    // probe — that's where the RTS lands and D0 = LibraryBase.
    if (is_settled && ex_pc == 32'h00c0_004e)
        $display("[LIBCALL] r=%0d OpenLibrary(name_ptr=%h, ver=%h)  A6=%h",
            retired, u_rf.regs[9], u_rf.regs[0], u_rf.regs[14]);
`endif

`ifdef LIBCALL_ALLOCMEM_TRACE
    // exec.library/AllocMem  LVO -198
    //   IN:  D0 = byte size, D1 = MEMF_* flags
    //   OUT: D0 = ptr (or 0 if exhausted)
    //
    // MEMF flags:
    //   $00000001 = MEMF_PUBLIC   (= ANY)
    //   $00000002 = MEMF_CHIP
    //   $00000004 = MEMF_FAST
    //   $00000100 = MEMF_LOCAL    (kept across reset)
    //   $00001000 = MEMF_CLEAR    (zero-fill)
    //   $00020000 = MEMF_REVERSE  (allocate from top)
    if (is_settled && ex_pc == 32'h00c0_01b0)
        $display("[LIBCALL] r=%0d AllocMem(size=%0d, flags=%h)  A6=%h",
            retired, u_rf.regs[0], u_rf.regs[1], u_rf.regs[14]);
`endif

`ifdef LIBCALL_FREEMEM_TRACE
    // exec.library/FreeMem  LVO -210
    //   IN:  A1 = ptr, D0 = byte size
    //
    // Pair with AllocMem trace to see leak / failed-load cleanup.
    // E.g. LoadSeg failure: AllocMem(7000, CHIP) → ptr=$5xxxx; then
    // moments later FreeMem($5xxxx, 7000) means LoadSeg aborted.
    if (is_settled && ex_pc == 32'h00c0_01a4)
        $display("[LIBCALL] r=%0d FreeMem(ptr=%h, size=%0d)  A6=%h",
            retired, u_rf.regs[9], u_rf.regs[0], u_rf.regs[14]);
`endif

`ifdef LIBCALL_WAIT_TRACE
    // exec.library/Wait  LVO -318
    //   IN:  D0 = signal mask
    //   OUT: D0 = signals that fired
    //
    // Hot in idle systems.  Use the signal mask to deduce which port
    // / device the task is blocked on:
    //   $00000010 = DOSSIG_PKT (DOS packet reply)
    //   $00000100 = handler default-signal (CON/RAM/etc.)
    //   $20000000 = SIGBREAKF_CTRL_C
    //   <other>   = AllocSignal'd by the task (e.g. UserPort)
    if (is_settled && ex_pc == 32'h00c0_0138)
        $display("[LIBCALL] r=%0d Wait(mask=%h)  task=%h",
            retired, u_rf.regs[0], u_rf.regs[14]);
`endif

`ifdef LIBCALL_SIGNAL_TRACE
    // exec.library/Signal  LVO -324
    //   IN:  A1 = task ptr, D0 = signal mask
    //
    // Pair with Wait: tracking who's signalling whom finds dropped /
    // never-fires-from-our-sim signals.
    if (is_settled && ex_pc == 32'h00c0_0132)
        $display("[LIBCALL] r=%0d Signal(task=%h, mask=%h)",
            retired, u_rf.regs[9], u_rf.regs[0]);
`endif

`ifdef LIBCALL_FINDTASK_TRACE
    // exec.library/FindTask  LVO -294
    //   IN:  A1 = task name (or 0 for ThisTask)
    //   OUT: D0 = task ptr
    if (is_settled && ex_pc == 32'h00c0_0150)
        $display("[LIBCALL] r=%0d FindTask(name=%h)",
            retired, u_rf.regs[9]);
`endif

`ifdef LIBCALL_FORBID_TRACE
    // exec.library/Forbid  LVO -132 — disables task scheduling.
    if (is_settled && ex_pc == 32'h00c0_01f2)
        $display("[LIBCALL] r=%0d Forbid()", retired);
`endif

`ifdef LIBCALL_PERMIT_TRACE
    // exec.library/Permit  LVO -138 — re-enables task scheduling.
    if (is_settled && ex_pc == 32'h00c0_01ec)
        $display("[LIBCALL] r=%0d Permit()", retired);
`endif

`ifdef LIBCALL_PUTMSG_TRACE
    // exec.library/PutMsg  LVO -366 — drop a Message into a MsgPort.
    //   IN:  A0 = MsgPort ptr, A1 = Message
    //
    // ALL packet-based DOS calls (LoadSeg, Lock, Open, Read, ...) flow
    // through PutMsg internally.  Probing here is the highest-yield
    // observation point for "what is the CLI sending to the file
    // system right now?".
    if (is_settled && ex_pc == 32'h00c0_0108)
        $display("[LIBCALL] r=%0d PutMsg(port=%h, msg=%h)",
            retired, u_rf.regs[8], u_rf.regs[9]);
`endif

// =====================================================================
// dos.library template (not enabled by default).
//
// Once you've captured DOSBase from an OpenLibrary probe run, define
// `+define+DOSBASE_VALUE=32'h00cN_NNNN` (and `+define+LIBCALL_DOS_*`
// for the LVOs you want).  Replace 32'h00cN_NNNN below with the
// captured base.  The math: dos.library JT entry = DOSBase + LVO.
//
// Common dos.library LVOs:
//   -30:  Open(name, mode)            -> BPTR file
//   -36:  Close(file)
//   -42:  Read(file, buf, len)        -> bytes read
//   -48:  Write(file, buf, len)
//   -84:  Lock(name, mode)            -> BPTR lock
//   -90:  UnLock(lock)
//   -150: LoadSeg(name)               -> BPTR seglist
//   -156: UnLoadSeg(seglist)
//
// Example for LoadSeg if DOSBase = $00c08000:
// =====================================================================
`ifdef LIBCALL_DOS_LOADSEG_TRACE
    // dos.library/LoadSeg  LVO -150
    //   IN:  D1 = BPTR name (BCPL string ptr)
    //   OUT: D0 = BPTR seglist (or 0 on failure)
    //
    // Define DOSBASE_VALUE first (as a 32-bit Verilog literal).
    if (is_settled && ex_pc == (`DOSBASE_VALUE - 32'd150))
        $display("[LIBCALL] r=%0d LoadSeg(name_bptr=%h)  A6=%h",
            retired, u_rf.regs[1], u_rf.regs[14]);
`endif

`ifdef LIBCALL_DOS_OPEN_TRACE
    // dos.library/Open  LVO -30 — open a file.
    if (is_settled && ex_pc == (`DOSBASE_VALUE - 32'd30))
        $display("[LIBCALL] r=%0d Open(name_bptr=%h, mode=%h)",
            retired, u_rf.regs[1], u_rf.regs[2]);
`endif

`ifdef LIBCALL_DOS_LOCK_TRACE
    // dos.library/Lock  LVO -84.
    if (is_settled && ex_pc == (`DOSBASE_VALUE - 32'd84))
        $display("[LIBCALL] r=%0d Lock(name_bptr=%h, mode=%h)",
            retired, u_rf.regs[1], u_rf.regs[2]);
`endif

// =====================================================================
// Exec Signal-call-site probes — fires at each `JSR _LVOSignal(A6)` site
// in K1.3 ROM (opcode `4EAE FEBC`), showing the calling PC + A1 (target
// task) + D0 (mask).  Use this to find which ROM function is the
// dominant Signal()er when summary mode says "Signal fires 50K times
// and we don't know who's calling".
//
// Sites are the 11 distinct `JSR _LVOSignal(A6)` instructions in
// kick_13.bin — see static-scan in WB13_DEBUG_JOURNAL / boing notes.
// =====================================================================
`ifdef LIBCALL_SIGNAL_CALLER_TRACE
    if (is_settled && (
            ex_pc == 32'h00fc_1bd6 || ex_pc == 32'h00fc_2e78 ||
            ex_pc == 32'h00fc_60e2 || ex_pc == 32'h00fc_6116 ||
            ex_pc == 32'h00fc_6da6 || ex_pc == 32'h00fd_3cf4 ||
            ex_pc == 32'h00fe_0266 || ex_pc == 32'h00fe_4896 ||
            ex_pc == 32'h00fe_5d4e || ex_pc == 32'h00fe_9e62 ||
            ex_pc == 32'h00ff_46b6))
        $display("[LIBCALL] r=%0d SignalCaller(at=%h, A1=%h, D0=%h)",
            retired, ex_pc, u_rf.regs[9], u_rf.regs[0]);
`endif

// =====================================================================
// Exec Wait-call-site probes — same idea for `JSR _LVOWait(A6)` (opcode
// `4EAE FEC2`).  Each Wait site shows where in the OS a task blocks.
// 16 sites in K1.3 ROM.
// =====================================================================
`ifdef LIBCALL_WAIT_CALLER_TRACE
    if (is_settled && (
            ex_pc == 32'h00fc_075a || ex_pc == 32'h00fc_1c4a ||
            ex_pc == 32'h00fc_2e2a || ex_pc == 32'h00fc_2f3a ||
            ex_pc == 32'h00fd_3cdc || ex_pc == 32'h00fe_024e ||
            ex_pc == 32'h00fe_4854 || ex_pc == 32'h00fe_5e6a ||
            ex_pc == 32'h00fe_5f30 || ex_pc == 32'h00fe_5f38 ||
            ex_pc == 32'h00fe_a396 || ex_pc == 32'h00fe_aa7e ||
            ex_pc == 32'h00fe_aaf2 || ex_pc == 32'h00ff_36da ||
            ex_pc == 32'h00ff_396c || ex_pc == 32'h00ff_4698))
        $display("[LIBCALL] r=%0d WaitCaller(at=%h, D0=%h)",
            retired, ex_pc, u_rf.regs[0]);
`endif
