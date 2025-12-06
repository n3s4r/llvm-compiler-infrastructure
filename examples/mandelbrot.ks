# Mandelbrot Set Renderer for Kaleidoscope
# From LLVM Tutorial Chapter 6
# This demonstrates user-defined operators

extern putchard(char);

# Define logical OR operator with precedence 5
def binary| 5 (LHS RHS)
  if LHS then
    1
  else if RHS then
    1
  else
    0;

# Define logical AND operator with precedence 6
def binary& 6 (LHS RHS)
  if !LHS then
    0
  else
    RHS;

# Define sequence operator (:) with precedence 1
def binary: 1 (x y) y;

# Define comparison less-than or equal
def binary= 9 (LHS RHS)
  !(LHS < RHS | LHS > RHS);

# Define greater-than-or-equal
def binary>= 9 (LHS RHS)
  !(LHS < RHS);

# Define less-than-or-equal  
def binary<= 9 (LHS RHS)
  !(LHS > RHS);

# Define unary negation
def unary!(v)
  if v then
    0
  else
    1;

# Density function for Mandelbrot visualization
def printdensity(d)
  if d > 8 then
    putchard(32)  # ' '
  else if d > 4 then
    putchard(46)  # '.'
  else if d > 2 then
    putchard(43)  # '+'
  else
    putchard(42); # '*'

# Mandelbrot iteration function
# Determines if point (real, imag) is in the Mandelbrot set
def mandelconverger(real imag iters creal cimag)
  if iters > 255 | (real*real + imag*imag > 4) then
    iters
  else
    mandelconverger(real*real - imag*imag + creal,
                    2*real*imag + cimag,
                    iters+1, creal, cimag);

# Helper to start iteration
def mandelconverge(real imag)
  mandelconverger(real, imag, 0, real, imag);

# Plot one row of the Mandelbrot set
def mandelhelp(xmin xmax xstep y)
  for x = xmin, x < xmax, xstep in
    printdensity(mandelconverge(x, y));

# Main Mandelbrot plotting function
def mandel(xmin xmax xstep ymin ymax ystep)
  for y = ymin, y < ymax, ystep in
    (mandelhelp(xmin, xmax, xstep, y) : putchard(10));

# Render a basic Mandelbrot set
# Run: mandel(-2.3, 1.3, 0.05, -1.3, 1.3, 0.07);

