# Basic Kaleidoscope Examples
# Introduction to the language features

# Simple arithmetic
2 + 3;
4 * (2 + 3);
10 / 2;
7 - 3;

# Define a simple function
def add(x y) x + y;
add(3, 4);

# Define a more complex function
def average(a b) (a + b) / 2;
average(10, 20);

# Nested function calls
add(average(1, 3), average(5, 7));

# Using external functions
extern sin(x);
extern cos(x);

sin(1.0);
cos(0.0);

# Define a function using extern
def circle_x(radius angle) radius * cos(angle);
def circle_y(radius angle) radius * sin(angle);

circle_x(1.0, 0.5);
circle_y(1.0, 0.5);

# Comparison operators
5 < 10;   # Returns 1.0 (true)
10 < 5;   # Returns 0.0 (false)

# If/then/else expression
def abs(x) if x < 0 then 0-x else x;
abs(-5);
abs(5);

# Max and min functions
def max(a b) if a > b then a else b;
def min(a b) if a < b then a else b;

max(10, 20);
min(10, 20);

# For loop - compute sum of 1 to n
extern printd(x);

def sum(n)
  var total = 0 in
  (for i = 1, i < n + 1 in
     total = total + i) :
  total;

sum(10);  # 1+2+3+4+5+6+7+8+9+10 = 55

# Factorial function (recursive)
def factorial(n)
  if n < 2 then
    1
  else
    n * factorial(n - 1);

factorial(5);  # 120

# Power function (recursive)
def pow(base exp)
  if exp < 1 then
    1
  else
    base * pow(base, exp - 1);

pow(2, 10);  # 1024

