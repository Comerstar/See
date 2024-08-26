
# See Documentation

This file lists every cell in See, and the associated behaviours of seeing that cell. Cells have two primary types, transparent and opaque. Transparent cells (including reflectors and refractors) continue the line of sight. Transparent cells cannot have side effects, and can only alter a local stack and set of bindings. Opaque cells stop the line of sight, and execute some side effect on the pointer state, such as changing the pointer position and angle, or printing out a result to IO.

# Section 1 - Mirrors

See has several mirror characters, which are all transparents. Their mirror forms should be relatively self explanatory. All mirror characters add void onto the stack.

# |
# /
# -
# \
# X
# +
# =
# _
# O

# Section 2 - Characters

All lower case English Latin characters are string/identifier constructors in See. They are all transparent cells.

# Section 3 - Digits

All Arabic digits are number constructors in See. They are all transparent cells.

# Section 4 - Operators

Operators allow for stack items to be manipulated.

# A

Pops the top two elements off the stack, and puts their sum lazily on the stack.

# S

Pops the top two elements off the stack, and pushes the second element popped minus the first element lazily on the stack.

# M

Pops the top two elements off the stack, and puts their product lazily on the stack.

# D

Pops the top two elements off the stack, and pushes the second element popped divided by the first element lazily on the stack.

# %

Pops the top two elements off the stack, and pushes the second element popped modulo the first element lazily on the stack.

# Section 5 - Advanced Operators

# ?

Conditional reflector, if the value on the stack is truthy, then it acts as /, otherwise it acts as \\. If the stack is empty it passes straight through.

# Y

The lambda operator, creates a lambda using the last two elements on the stack.

# $

The eval operator, evaluates the last element on the stack, and pushes the result back on.

# P

The pop operator, pops the last element on the stack.

# C

The copy operator, copies the last element on the stack.

# @

The apply operator, takes the last two elements on the stack and applies one to the other.

# R

The relu operator, takes the last element and relus it.

# I

The input operator, allows users to input either a string (all lower case Latin characters), an integer, or a float. Will keep asking the user for input until a valid input is given.

# Section 5 - Pointer Position Opaques

# ^

Makes the pointer face up.

# >

Makes the pointer face right.

# V

Makes the pointer face down.

# <

Makes the pointer face left.

# :

Jumps the pointer to it.

# Section 6 - Pointer Execution Opaques

# \#

The print opaque.

# !

The halt opaque. Halts the execution of the pointer.

# ~

The bind opaque, gets the lsat two elements on the stack, and creates a global binding for it.

# W

The write opaque, writes "the top element" of the stack to the cell the cell the pointer is currently at.
