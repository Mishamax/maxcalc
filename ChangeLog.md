### v2.0.1 (28-Mar-2010) ###

  * Added: Save window geometry to ini file (GUI).
  * Added: Support for complex arguments of trigonometric functions in degrees/grads mode.
  * Improved: Output of complex numbers.
  * Changed: Default precision is 25 digits instead of 50.
  * Fixed: Bug in sin() function for complex arguments.
  * Fixed: Bugs in inverse trigonometric functions for complex arguments.
  * Fixed: Rounding errors and other small issues in unit conversions.
  * Internal: decNumber library updated to v3.68.

### v2.0.0 (21-Mar-2010) ###

  * Added: Radians / degrees / grads support.
  * Added: Comma decimal separator support.
  * Added: Support for 'j' as imaginary one.
  * Added: Minimize / close to system tray and single instance mode (GUI).
  * Added: Links to Web site and bug tracker (GUI).
  * Added: Execution of commands starting with '#' (GUI).
  * Added: Command line arguments support on Linux (Console).
  * Added: Save settings to .ini file.
  * Added: Number output format settings.
  * Added: Descriptions to unit conversions.
  * Added: Readme.
  * Improved: Menu structure (GUI).
  * Improved: Syntax of commands (Console).
  * Improved: Internal exception system; added more error messages.
  * Fixed: History highlighting (GUI).
  * Fixed: EOF handling (Console).
  * Internal: Improved code portability; added CMake build system support.
  * Internal: Code is greatly cleaned up.

### v1.9.3 Milestone-4 (19-Sep-2009) ###

  * Added: Meaningful error messages instead of 'Error!'.
  * Fixed: Command line params handling in console version.

### v1.9.2 Milestone-3 (2-May-2009) ###

  * Added: Unit conversions.
  * Added: C-like assignment operators (`+=`, `-=`, `*=`, `/=`, `^=`).
  * Changed: Functions and variables lists in GUI improved.

### v1.9.1 Milestone-2 (19-Feb-2009) ###

  * Added: Unlimited variables support.
  * Added: Factorial function.
  * Changed: General number format is now used instead of scientific.
  * Changed: Complex numbers like 'i1' are no longer supported; use '1i' instead.
  * Fixed: Number scaling bugs.
  * Internal: Overhauled exception handling in engine.

### v1.9.0 Milestone-1 (31-Dec-2008) ###

First public release.


Includes graphical (Qt 4) and console versions which can be build on Windows and Linux
without changes (binaries are currently available only for Windows).


**Main features:**


  * Expressions are inputed as strings from keyboard.
  * 50 digits precision of calculations.
  * Complex numbers support.
  * Collection of built-in functions (abs, sqr, sqrt, pow, exp, trigonometric, inverse trigonometric and hyperbolic functions, logarithms).
  * Built-in E and PI constants, and RES variable which saves last calculated result.
  * Internal parser and console version are written on pure C++ and use STL only.
  * Graphical version is based on Qt 4 library.