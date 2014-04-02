/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

class VirtualMachine {
 public:
  VirtualMachine();

 private:
    DISALLOW_COPY_AND_ASSIGN(VirtualMachine);

};

#endif  // VIRTUALMACHINE_H
