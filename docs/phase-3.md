# Phase 3

## Task

- Implementing demand-paging

## Pages

- pages are 8 words
- 32 frames in 256 word memory
- the page table will have 32 entries, each 8 bits long, as follows:
  - frame number - 5 bits
  - modify bit - 1 bit
  - valid/invalid bit - 1 bit
  - empty bit - 1 bit

## Algorithms

- Implement two replacement algorithms
  - FIFO
  - LRU

## Page Faults

- When a page fault occurs…
  - The process is placed in the wait queue with the trap completion set to 35 clock ticks
  - After 35 clock ticks, the process is moved to the ready queue and the new page is loaded into memory

## PCB

- Each process (PCB) should have
  - # of page faults
  - hit ratio

## TLB

- Need to add a translation look-aside buffer to the Virtual Machine
- Memory references…
  - Check the TLB
  - Take 4 clock ticks if successful
  - Otherwise, trap to the OS
- Whenever a new process takes over the Virtual Machine, it copies its page table onto the TLB
- So, the page table should be added to the PCB, and included in the load/unload operations

## Program Counter

- OS should not check the validity of the program counter regularly
- base+limit should always point to the currently executing page
- The hardware (Virtual Machine) makes sure the value of pc is bound by them (base < pc < (base+limit))
- If the pc goes out of bounds and the new page is not in the TLB, an OS trap is generated
- If the page is already in the TLB, the values of base and limit registers are updated to point to the new page and the process will continue without a page fault

## LRU

- Add 32 registers to the VM (one for each frame)
- When a frame is accessed, the hardware saves the current time in the corresponding register
- OS access frame registers to perform the LRU page replacement algorithm

## FIFO

- FIFO doesn’t use these registers
- The OS maintains its own vector of frame registers, and FIFO is entirely done by the OS without the VM
- When a page is brought into memory, the OS records the current time in its frame registers.
- When it picks a victim, it uses its own registers, not the VM’s
