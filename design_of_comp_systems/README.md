# Simple 8‑bit CPU – BrainFuck Interpreter (VHDL)

This project implements a small 8‑bit CPU in VHDL capable of executing a subset of the BrainFuck language.  
The design uses synchronous RAM, input/output ports, and a finite‑state machine to decode and run instructions.

## Features
- 13‑bit **Program Counter (PC)**
- 13‑bit **Data Pointer (PTR)** with circular addressing
- 12‑bit **Loop Counter (CNT)** for nested loops
- Full BrainFuck instruction support:
  - `>` `<` pointer movement  
  - `+` `-` data increment/decrement  
  - `.` output  
  - `,` input  
  - `[` `]` while loops  
  - `(` `)` do‑while loops  
  - `0–9`, `A–F` hexadecimal immediate write  
  - `@` program halt
- Synchronous memory interface (read/write sequencing)
- Output blocking when LCD is busy
- Input waiting until data is valid

## Architecture Overview
The CPU is built from:
- **Three synchronous registers**: PC, PTR, CNT  
- **Two multiplexers** selecting memory address and write data  
- **A large FSM** controlling instruction fetch, decode, execution, and loop handling  
- **Status outputs**: `READY` (initialization done), `DONE` (program finished)

## Memory & I/O
- `DATA_ADDR`, `DATA_RDATA`, `DATA_WDATA`, `DATA_RDWR`, `DATA_EN`  
- `IN_DATA`, `IN_VLD`, `IN_REQ`  
- `OUT_DATA`, `OUT_WE`, `OUT_BUSY`, `OUT_INV`

The CPU waits for:
- `OUT_BUSY = 0` before printing  
- `IN_VLD = 1` before reading input  

## Loop Handling
Loops are implemented using the CNT register:
- `[` increments CNT, `]` decrements CNT  
- CPU skips forward/backward until CNT returns to zero  
- Supports nested loops and do‑while constructs


