# Changelog

## V0.1A

First Production Patch, V0.01 Alpha

### Bug Fixes

* [BUG] Fixed bug in Lua-U lifter branch location being 0.
* [BUG] Fixed bug in branch saftey analysis transformer where cmp instruction registers will not get picked up.
* [BUG] Fixed bug in Lua-U parsing where jump IL refrences where nullptr.
* [BUG] Fixed bug in AST CFG Generation where labels where not in a new line.

### Optimizations

* [OPTIMIZED] Organized source compilation files.
* [OPTIMIZED] Removed IR compare kind in favor for AST bin kind.
* [OPTIMIZED] Improved AST transformers.
* [OPTIMIZED] Improved AST element kinds.
* [OPTIMIZED] Improved AST bin kinds.
* [OPTIMIZED] Improved IL commit errors. 
* [OPTIMIZED] Visual Studio build configuration.
* [OPTIMIZED] Improve AST node functions.
* [OPTIMIZED] Removed automatic naming.

### Added Features

* [ADDED] Set conditional operation to lifter.
* [ADDED] All debugs marco.
* [ADDED] IL Dump and native language struct disassembly.
* [ADDED] Return AST node element kind.
* [ADDED] GOTO support.