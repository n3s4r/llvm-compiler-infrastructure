# Fibonacci Sequence Examples for Kaleidoscope
# From LLVM Tutorial Chapter 5

# Recursive Fibonacci function
def fib(x)
  if x < 3 then
    1
  else
    fib(x-1) + fib(x-2);

# Test the recursive version
fib(10);

# Iterative Fibonacci using mutable variables (Chapter 7)
# More efficient than recursive version
def fibi(x)
  var a = 1, b = 1, c in
  (for i = 3, i < x in
     c = a + b :
     a = b :
     b = c) :
  b;

# Test the iterative version
fibi(10);

# Print first 10 Fibonacci numbers
extern printd(x);

def printfib(n)
  for i = 1, i < n + 1 in
    printd(fib(i));

printfib(10);

