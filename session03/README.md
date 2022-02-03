# Session03 (02/02/2022) - Boolean Expression Parser

In this session, we create a postfix boolean expression parser. The user enters expressions in postfix
notation, and the program evaluates and displays their result.

## Example Usage
```
t f > ~
>>> true

f f # f > t ^ ~
>>> false

t f t > > f f # + ~
>>> false

## Recompiling

To compile the project, clone the repository to your computer. Then, navigate to this folder and type `make`. This will produce an executable `bep` inside the bin directory. Run with `bin/bep` (this assumes a Linux setup).
