# CLAUDE.md — Code-Analyzer

This file documents the codebase structure, development workflows, and conventions for AI assistants working on this repository.

---

## Project Overview

**Code-Analyzer** is a C++ static analysis tool that tokenizes C++ source files, builds an Abstract Syntax Tree (AST), and computes code metrics (function complexity, line counts, nesting depth). It is a Windows-only Visual Studio 2015 project.

- **Author:** Rohit Sharma (SUID: 242093353); original library code by Jim Fawcett (Syracuse University)
- **Course:** CSE 687 — Object Oriented Design, Project #2
- **Language:** C++ (Visual C++ 2015 / MSVC)
- **Platform:** Windows 10, x64
- **Build System:** Visual Studio Solution (`.sln` + `.vcxproj`)

---

## Repository Structure

```
Code-Analyzer/
├── MetricExecutive/        # Main entry point (main() lives here)
├── Parser/                 # Core parsing engine — Rule-Action pattern
├── Tokenizer/              # Character-level tokenizer — State pattern
├── SemiExp/                # Semi-expression collector and ITokCollection interface
├── ScopeStack/             # Template-based scope tracking stack
├── AST/                    # Abstract Syntax Tree (ASTNode, SyntaxTree)
├── MetricsAnalysis/        # Formats and outputs analysis results
├── FileMgr/                # File/directory enumeration and path utilities
├── DataStore/              # Simple vector-based file path store
├── Utilities/              # StringHelper and Converter<T> templates
├── Inputs/                 # Test input files (.cpp, .h samples)
├── x64/                    # Compiled build output (Debug/Release)
├── Project2_CodeAnalyzer.sln  # Visual Studio solution file
├── compile.bat             # Build script (calls devenv /rebuild)
└── run.bat                 # Execution script
```

---

## Building the Project

> **Windows only.** Requires Visual Studio 2015 (or compatible devenv in PATH).

### Full rebuild (debug)
```bat
compile.bat
```
Internally runs:
```bat
devenv Project2_CodeAnalyzer.sln /rebuild debug
```

### Run the analyzer
```bat
run.bat
```
Internally runs:
```bat
x64\Debug\MetricExecutive.exe ./ *.cpp,*.h
```

### Command-line usage
```
MetricExecutive.exe <directory> <pattern>
```
- `<directory>` — Root directory to search for source files
- `<pattern>` — Comma-separated file extension patterns (e.g., `*.cpp,*.h`)

Example:
```bat
x64\Debug\MetricExecutive.exe ./Inputs *.cpp,*.h
```

---

## Package Descriptions

### MetricExecutive
- **Files:** `MetricExecutive.h`, `MetricExecutive.cpp`
- **Role:** Application entry point. Parses command-line args, drives file enumeration via `FileMgr`, invokes the parser pipeline per file, and collects/displays metrics.
- **`main()` is here.**

### Parser
- **Files:** `Parser.h`, `Parser.cpp`, `ConfigureParser.h`, `ActionsAndRules.h`
- **Role:** Core parsing engine. Uses a **Rule-Action pattern** — `IRule` objects inspect token streams and fire associated `IAction` callbacks.
- `ConfigParseToConsole` (implements `IBuilder`) assembles the parser with all rules/actions wired together.
- `Repository` is a shared-state object passed to all actions.

### Tokenizer
- **Files:** `Tokenizer.h`, `Tokenizer.cpp`
- **Namespace:** `Scanner`
- **Role:** Character-level tokenizer. Implements the **State design pattern** via `ConsumeState` subclasses to handle alphanumeric tokens, string/character literals, punctuation, and whitespace.

### SemiExp
- **Files:** `SemiExp.h`, `SemiExp.cpp`, `itokcollection.h`
- **Namespace:** `Scanner`
- **Role:** Groups tokens into "semi-expressions" — logical units terminated by `{`, `}`, `;`, or newline (for `#` preprocessor lines). Implements the `ITokCollection` interface.

### ScopeStack
- **Files:** `ScopeStack.h`, `ScopeStack.cpp`
- **Role:** Generic template stack (`ScopeStack<element>`) used to track open scopes during parsing. Header contains full template implementation.

### AST
- **Files:** `SyntaxTree.h`, `SyntaxTree.cpp`
- **Role:** Builds and traverses the Abstract Syntax Tree. `ASTNode` holds the type, name, line numbers, and child nodes. `SyntaxTree` wraps the root and provides tree-walk utilities.

### MetricsAnalysis
- **Files:** `MetricsAnalysis.h`, `MetricsAnalysis.cpp`
- **Role:** Receives the completed AST and outputs formatted metrics (function name, line count, complexity/nesting depth) to console.

### FileMgr
- **Files:** `FileMgr.h`, `FileMgr.cpp`, `FileSystem.h`, `FileSystem.cpp`
- **Role:** Directory-recursive file enumeration with pattern matching. `FileSystem` provides low-level path/file utilities wrapping Win32 APIs.

### DataStore
- **Files:** `DataStore.h` (header-only)
- **Role:** Thin wrapper around `std::vector<std::string>` for storing enumerated file paths.

### Utilities
- **Files:** `Utilities.h`, `Utilities.cpp`
- **Role:** `StringHelper` (split, trim, print) and `Converter<T>` (template to/from string conversion).

---

## Code Conventions

### Naming
| Construct | Convention | Example |
|-----------|-----------|---------|
| Classes | PascalCase | `SyntaxTree`, `MetricsAnalyzer` |
| Methods | camelCase | `doAction()`, `getFiles()` |
| Private members | camelCase with `p_` prefix for pointers | `p_Toker`, `pAST` |
| Namespaces | PascalCase | `Scanner` |
| Constants/macros | UPPER_SNAKE | `TOKEN_VERBOSITY` |

### Header Guards
All headers use the classic include guard pattern (no `#pragma once`):
```cpp
#ifndef FILENAME_H
#define FILENAME_H
// ...
#endif
```

### File Headers
Every `.h` and `.cpp` file begins with a structured comment block:
```cpp
/////////////////////////////////////////////////////////////////////
// FileName.h - Brief description                                  //
// ver X.Y                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Code Metrics Analysis                              //
// Author:      Name, Institution                                  //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * Describe what this package does.
 *
 * Public Interface:
 * -----------------
 * List key classes/functions.
 *
 * Build Process:
 * --------------
 * List dependencies.
 *
 * Maintenance History:
 * --------------------
 * ver X.Y : DD Mon YYYY - changes
 */
```
**Maintain this convention when adding new files.**

### Design Patterns in Use
- **Rule-Action Pattern** (`Parser/`) — decouple grammar rules from their side effects
- **State Pattern** (`Tokenizer/`) — `ConsumeState` subclasses handle token character classes
- **Builder Pattern** (`Parser/ConfigureParser.h`) — `IBuilder` / `ConfigParseToConsole` assembles the parser
- **Repository Pattern** (`Parser/`) — `Repository` singleton shares state across all actions
- **Template Classes** (`ScopeStack/`, `Utilities/`) — generic implementations in headers

### Namespaces
- Tokenizer and SemiExp code lives in the `Scanner` namespace.
- All other packages use global namespace (no explicit namespace declaration).

---

## Testing

There is no automated test framework. Testing is manual:

1. **Small test set:** Run against `Inputs/` directory:
   ```bat
   x64\Debug\MetricExecutive.exe ./Inputs *.cpp,*.h
   ```

2. **Full project test:** Run against entire source tree (default `run.bat`):
   ```bat
   x64\Debug\MetricExecutive.exe ./ *.cpp,*.h
   ```

3. **Interactive pauses:** The program calls `_getch()` after displaying each file's metrics. Press any key to advance. This is intentional for interactive review — do not remove these pauses.

**Test input files:**
- `Inputs/Test.h` — Sample header with class and function definitions
- `Inputs/Test.cpp` — Sample C++ with lambdas, loops, and conditionals
- `Inputs/Parser/Parser.h`, `Parser.cpp` — Parser-specific test inputs

---

## Key Data Flow

```
Command-line args
       │
       ▼
  MetricExecutive
       │
       ├─► FileMgr ──► DataStore (list of file paths)
       │
       └─► For each file:
              │
              ▼
          Tokenizer  (character stream → tokens)
              │
              ▼
           SemiExp   (tokens → semi-expressions)
              │
              ▼
            Parser   (semi-expressions → AST nodes via Rules/Actions)
              │      uses ScopeStack to track open scopes
              ▼
              AST    (complete syntax tree)
              │
              ▼
       MetricsAnalysis  (tree → console output)
```

---

## Important Constraints

- **Windows-only:** `FileSystem.cpp` uses Win32 APIs (`FindFirstFile`, `FindNextFile`). Do not attempt cross-platform builds.
- **Visual Studio 2015:** Project files target MSVC v140 toolset. Newer VS versions can open the solution but may upgrade the toolset.
- **Interactive console:** `_getch()` pauses require an interactive terminal. Piping stdout will still work but the pause requires a keypress.
- **No external dependencies:** The project is entirely self-contained with no NuGet packages or third-party libraries.

---

## Git Workflow

- Default development branch pattern: `claude/<session-id>`
- `master` holds the baseline (single "First Commit")
- No CI/CD pipeline exists — all builds are manual

---

## Glossary

| Term | Meaning |
|------|---------|
| Semi-expression | A sequence of tokens ending in `{`, `}`, `;`, or `\n` (for preprocessor lines) |
| ASTNode | A node in the syntax tree with fields: `type`, `name`, `startLine`, `endLine`, `children` |
| Complexity | Nesting depth metric — counts `{` depth within a function body |
| IRule | Interface: `test(ITokCollection&)` returns true when rule fires |
| IAction | Interface: `doAction(ITokCollection&)` — side effect when a rule fires |
| Repository | Shared object holding `ScopeStack`, `AST pointer`, and `Toker pointer` for use by all actions |
