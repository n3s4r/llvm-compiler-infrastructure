# User-Defined Operators Examples for Kaleidoscope
# From LLVM Tutorial Chapter 6

# Define a unary NOT operator
def unary!(v)
  if v then
    0
  else
    1;

# Test unary NOT
!0;  # Should return 1
!1;  # Should return 0

# Define binary OR operator with precedence 5
def binary| 5 (LHS RHS)
  if LHS then
    1
  else if RHS then
    1
  else
    0;

# Test OR operator
0|0;  # Should return 0
0|1;  # Should return 1
1|0;  # Should return 1
1|1;  # Should return 1

# Define binary AND operator with precedence 6 (higher than OR)
def binary& 6 (LHS RHS)
  if !LHS then
    0
  else
    RHS;

# Test AND operator
0&0;  # Should return 0
0&1;  # Should return 0
1&0;  # Should return 0
1&1;  # Should return 1

# Define sequence operator (like comma in C)
# Very low precedence (1)
def binary: 1 (x y) y;

# Use sequence operator for side effects
extern printd(x);
printd(1) : printd(2) : printd(3);

# Define equality operator
def binary= 9 (LHS RHS)
  !(LHS < RHS | LHS > RHS);

# Test equality
5 = 5;  # Should return 1
5 = 4;  # Should return 0

# Define less-than-or-equal
def binary<= 9 (LHS RHS)
  !(LHS > RHS);

# Define greater-than-or-equal  
def binary>= 9 (LHS RHS)
  !(LHS < RHS);

# Test comparison operators
3 <= 5;  # Should return 1
5 >= 3;  # Should return 1

# Combine operators in complex expressions
def test(a b c)
  (a > 0) & (b > 0) | (c > 0);

test(1, 1, 0);  # Should return 1
test(0, 0, 1);  # Should return 1
test(0, 0, 0);  # Should return 0

