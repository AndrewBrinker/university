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

## Algorithms

- Implement two replacement algorithms
  - FIFO
  - LRU

## Page Faults

- When a page fault occurs…
  - The process is placed in the wait queue with the trap completion set to 35 clock ticks
  - After 35 clock ticks, the process is moved to the ready queue and the new page is loaded into memory

1. Page fault is signalled from the VM
2. If there are free frames in the TLB, the proper page is loaded into a frame
3. If there are no free frames...
  1. If LRU, the OS accesses the VM's frame registers and selects the victim page according to LRU
  2. If FIFO, the OS accesses its own frame registers and selects the victim page according to FIFO
4. The victim page is swapped out of the TLB and replaced with the new page
5. Wait until 35 clock ticks have happened
6. Once 35 clock ticks have occurred, the process is moved to the ready queue

## PCB

- Each process (PCB) should have
  - # of page faults
  - hit ratio

## Multiprogramming

- A degree of multiprogramming equal to 5 is maintained at all times, meaning that only five programs should be in the ready queue, wait queue, or running at any given time.
- All programs should be in the jobs queue. When a program finishes, the jobs queue should be checked. If there is a job that is not yet in either the ready or wait queue, add it to the ready queue.

## Context Switches

- Page Fault must be added as a reason to context switch. Bit 10 of sr is used along with bits 5-7. When 5-7 are all zeros, if 10 is 0 than it's a time slice. If bit 10 is 1 then it's a page fault.

## Programs

- There should be ten total programs:
  - addVector.s
  - fact1.s
  - fact2.s
  - io.s
  - simple1.s
  - simple2.s
  - sub.s
  - subVector.s
  - sum1.s
  - sum2.s
- Test with only two programs at first and add more later
- The first two programs each generate one page fault w/ 28 frames still free, so no replacement is needed for them.

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

- The OS maintains its own vector of frame registers, and FIFO is entirely done by the OS without the VM
- When a page is brought into memory, the OS records the current time in its frame registers.
- When it picks a victim, it uses its own registers, not the VM’s
