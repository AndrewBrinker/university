  read    0
  read    1
  call    6       ! multiply function
  load    0   24  ! return value
  write   0
  halt
  loadi   2   8   ! Multiply function
  loadi   3   0
  shr     1
  store   2   23  ! counter
  getstat 2
  andi    2   1
  compri  2   1
  jumpe   15
  jump    16
  add     3   0
  shl     0
  load    2   23  ! counter
  subi    2   1
  compri  2   0
  jumpg   8
  store   3   24 ! return value
  return
  noop            ! counter
  noop            ! return value
