
# Welcome to the See Esoteric Programming Language

Run ASCII art of a room as code. This interpreter was written entirely using a sea of C, see?

To use this interpreter run `make see` in the root directory, then run `./see <path_to_file>`

The file extension used throughout the examples is `.see`, although there is no restriction on this

## Table of Contents

## Section 1 - Basic Vision Overview

In this section we will cover the basic execution loop of See.

### 1.1 - Pointers

See executes code based on what the pointer can see. Pointers have the following properties:

- A cell the pointer is currently at
- An angle the pointer is looking at
- A set of bindings currently held by the pointer
- Input and output functions
- A pointer ID

The initial position of the pointer is marked by the character `.`, with the pointer facing right.

### 1.2 - Lines of Sight

What decides what code to run is the line of sight. The line of sight starts from a pointer, which is always assumed to be perfectly in the centre of a cell. The line of sight does not see the cell the pointer is currently on.

Cells will be grouped into two main categories: transparents and opaques.

Transparents are cells that continue the line of sight. They cannot trigger pointer side-effects, and can only functionally alter the current state of the line of sight.

Opaques are cells that end the line of sight. They are the way pointer state is altered.

It is important to note that transparents do not mean they cannot affect the line of sight: transparents include reflectors and refractors.

For example, the pointer's line of sight at first in

``` See
#.abc\n
    nd 
    ne |k\n
    nf   jn
    n\ghi/n
    nnnnnnn
```

is:

`abc\def\ghi/j\k|k\j/ihg\fed\cba#`

### 1.3 - Pointer Parallelism

Any code which defines multiple pointers will treat each pointer as a thread, and so they will run in parallel. The board state, is locked as appropriate between multiple threads using the Multiple-Readers Single-Writer pattern of locking. This is done because most of the accesses to the board are pointers reading the line of sight.

## Section 2 - Line of Sight Functional Overview

Each line of sight is functional internally, with a terminator which may perform side effects. A line of sight has the following properties:

- A stack
- A set of current bindings
- Ray information (float position and angle)

Transparents on the line of sight can functionally alter the stack and bindings in the line of sight. They operate in a postfix manner, without any operator hierarchy. These operations are typically lazy, with evaluation deferred until later, allowing the language to construct lambdas from unevaluated expressions.

## Section 3
