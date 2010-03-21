
  ***********************************************************************
  *                                                                     *
  *   MaxCalc - powerful yet simple to use scientific calculator        *
  *                                                                     *
  *                                                     MaxCalc 2.0.0   *
  *                                     (c) Michael Maximov 2005-2010   *
  *                                 http://code.google.com/p/maxcalc/   *
  *                                                                     *
  ***********************************************************************


  MaxCalc is a powerful and simple to use scientific calculator which
  evaluates expressions typed from the keyboard.
  It is free and open source software and is distributed under GPL license.
  MaxCalc has console and graphical (based on Qt4 library) versions and
  available for Windows and Linux.


Features
========

  * Calculates mathematical expressions typed from the keyboard.
  * High precision of calculations (up to 50 digits).
  * Wide collection of built-in functions.
  * Complex numbers support.
  * Unit conversions support.
  * Graphical and command line versions for Windows and Linux are available.


Usage
=====

  Just type a mathematical expression like that:
    
      -2+3*4-5/6

  and press <Enter>. MaxCalc will calculate it and print the result.
  
  Five arithmetic operators (plus +, minus -, multiply *, divide /, power ^)
  and brackets are supported:
  
      10.2 + ((3^5) * 2/3) - (2.1^-5.8)

  You can use exponential notation:
  
      1.2E+3 + 54.1E2 + 1E-5
  
  There is a wide collection of built-in functions, for example:
  
      abs(sin(-pi/4)) + cos(pi/4) + pow(8; 2)
      
  Type
  
      #funcs
  
  to see all of them.
  In graphical version you can also see them in the list at the right.

  Unit conversions usage:
  
      15 [in->cm]

  Type
  
      #convs
  
  To display them all.
  In graphical version you can select them from the menu.

  You can define and use variables. Try this:
  
      x = 2.1
      y = 3.2
      x + 2*y
      x = result ^ 2
      variable = 10
      #vars
      x + y + result
      #vars
      #del x
      #vars
      #del
      #vars
      result

  Variable "result" (or "res") is built in and contains result of previous
  expression.
  Apart from variables there are two built in constants: "pi" and "e".
  
  Complex numbers are supported:
  
      i + sqrt(-1) + 2 / i

  MaxCalc also support several commands starting with "#".
  Type "help" to see them.


Functions
=========

  Example: abs(sin(-pi/4)) + cos(pi/4) + pow(8; 2)
  
  All functions except "pow" have one argument. "pow" has two arguments.

  Common:
    abs                                 Absolute value
    sqr                                 Square
    sqrt                                Square root
    pow                                 Power
    fact / factorial                    Factorial

  Trigonometric:
    sin                                 Sine
    cos                                 Cosine
    tan / tg                            Tangent
    cot / ctg                           Cotangent
    
    asin / arcsin                       Arc sine
    acos / arccos                       Arc cosine
    atan / arctan / atg / arctg         Arc tangent
    acot / arccot / actg / arcctg       Arc cotangent
    
    sinh                                Hyperbolic sine
    cosh                                Hyperbolic cosine
    tanh / tgh                          Hyperbolic tangent
    coth / ctgh                         Hyperbolic cotangent

    asinh / arcsinh                     Hyperbolic arc sine
    acosh / arccosh                     Hyperbolic arc cosine
    atanh / arctanh / atgh / arctgh     Hyperbolic arc tangent
    acoth / arccoth / actgh / arcctgh   Hyperbolic arc cotangent

  Logarithmic
    ln                                  Natural logarithm
    log2                                Base-2 logarithm
    log10                               Base-10 logarithm
    exp                                 Exponent


Unit conversions
================

  Example: 15 [in->cm]

  Length:
    mil         Mil
    in          Inch
    ft          Foot
    yd          Yard
    mi          Mile
    micron      Micron
    mm          Millimeter
    cm          Centimeter
    m           Meter
    km          Kilometer
  
  Weight:
    lb          Pound
    oz          Ounce
    g           Gramm
    kg          Kilogram
  
  Time:
    micros      Microsecond
    ms          Millisecond
    s           Second
    min         Minute
    h           Hour
    d           Day
  
  Speed:
    mi/h        Mile per Hour
    m/s         Meter per Second
    ft/h        Foot per Hour
    km/h        Kilometer per Hour
    knot        Knot
    
  Temperature:
    k           Kelvin
    c           Celsius
    f           Fahrenheit


Commands
========

  #funcs                                Display list of built-in functions.
  #convs                                Display list of unit conversions.
  #consts                               Display list of built-in constants.
  #vars                                 Display list of variables.
  #del                                  Delete all variables
  #del [<var>]                          Delete variable <var>
  #angles                               Display angle unit
  #angles radians / degrees / grads     Set angle unit.
  #output                               Display output settings
  #output , / . / i / j / <precision>   Set output settings.
  #ver                                  Display version information.
  help                                  Display help about these commands.
  exit                                  Close the program.

