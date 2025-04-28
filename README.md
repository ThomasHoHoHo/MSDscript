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
