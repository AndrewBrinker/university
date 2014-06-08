# Phase 3 To Do List

- [x] Add page table structure
- [x] Add page tables to PCB, and VM
- [x] Add 32 registers to the VM for LRU accounting
- [ ] Add logic for LRU accounting. Each time a page is accessed, save the clock tick count in the proper register in the VM.
- [x] Add 32 registers to the OS for FIFO accounting.
- [ ] Add logic for FIFO accounting. Each time a page is brought into memory, save the clock in the proper register in the OS.


Let's put things in order...

1. Each process needs to be loaded into memory
2. When it is, it needs to be partitioned into pages
3. Each page is 8 words long
4. Each line of a file is a word
5. So every eight lines becomes a page


Logic for TLB usage

    1. Logical address is run through TLB
    2. If hit...
        1. Return frame #
    3. If miss...
        1. Lookup in page table
        2. Check if TLB is full
            1. If TLB is full...
                1. Swap new frame # for old one by some method
            2. Else...
                1. Add to TLB
