# Answers

## Grammars

1. Defined in `files/grammars/def1.grammar`.
2. Defined in `files/grammars/def2.grammar`.
3. Impossible to define in a context-free grammar. Proof in `files/proof.pdf`.

## Canonical LR(0) Sets

1. Found in `files/results/results1.txt`.
2. Found in `files/results/results2.txt`.
3. None given.

## Are they SLR?

1. No. When input to Flex/Bison, multiple shift/reduce and reduce/reduce errors
   are found. As seen in `code/y.output`, one such conflict is present in state
   3, where the presence of a 1 makes possible two distinct reductions. There
   are other such conflicts (which can be seen in the same file), but one
   conflict is sufficient.
2. Yes. When input to Flex/Bison, no shift/reduce or reduce/reduce errors are
   found.
3. No (not a CFG, therefore not SLR).
