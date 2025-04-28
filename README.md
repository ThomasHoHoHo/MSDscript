**MSDscript Interpreter**

A C++ Scripting Language Implementation with Qt GUI

Project Owner: Thomas Ho

**Overview**

MSDscript is an interpreted scripting language implementation featuring arithmetic operations, variables, functions, and advanced memory management. Developed as part of advanced software construction coursework, this project demonstrates core compiler/interpreter concepts through 12 iterative development phases.

**Key capabilities:**

Expression Evaluation: Arithmetic operations, variable binding, and conditional logic
Function Support: First-class functions with lexical scoping
Memory Safety: Smart pointers for automatic garbage collection
Testing Framework: Randomized property-based testing
Toolchain Integration: Makefile build system with Catch2 testing

**Tech Stack:**
- Core Language: C++ 17
- GUI Framework: Qt 6
- Testing: Catch2
- Build System: CMake
- Memory Management: Smart Pointers (std::shared_ptr)


**This educational project evolved through 12 development phases to implement:**
- Recursive-descent parser with AST generation
- Environment-based interpreter with closure support
- Smart pointer memory management system
- Property-based test framework
- Qt-based debugging interface

**Key Features:**
1. Advanced Grammar Parsing
bnf
<expr> = <number> 
       | <expr> + <expr>
       | <expr> * <expr>
       | <variable>
       | _let <var> = <expr> _in <expr>
       | _if <expr> _then <expr> _else <expr>
2. Mathematical Execution Engine
python
3 + 5 * 2        # Interps to 13
(2 + 3) * (4 - 1) # Interps to 15
1 + 2 == 3       # Evaluates to _true
3. Let Bindings & Functions
javascript
_let factorial = _fun (fact)
                _fun (n)
                  _if n == 1
                  _then 1
                  _else n * fact(fact)(n - 1)
_in factorial(factorial)(5)  # Returns 120
4. Boolean Logic & Error Handling
python
_if (2 + 2 == 5) _then 1 _else 0  # Returns 0
_let x = _true _in x + 5          # Throws "Cannot add boolean to number"
5. Smart Memory Management
cpp
// Automatic garbage collection
PTR(Expr) e = NEW(Add)(NEW(Num)(3), NEW(Num)(5));
