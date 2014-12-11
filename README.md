# Cmpl 0.0.1

A research C/C++ build system

## Warning

There's likely going to be some pretty crappy code so hopefully it works. I want
to keep on improving it. I'm always open to suggestions or comments. Hopefully
it's useful to someone other than myself.

# Bootstrapping

Version `0.0.1` contains `src/cmpl-bootstrap/main.cpp`, find a way to compile
this file and run it in the directory that contains `project.cmpl` or any
subdirectory reachable from that directory.

## Building

Ensure you have `clang` 3.5 and the latest version of `cmpl` then just run
`cmpl`.

## Guidelines

- The last released version should always be able to compile a new release
- Never `#include <iostream>`
- Stick to a sane C++14 style
- Leverage Clang's analysis libraries

## TODO

- Use timestamps (and inodes for identifiers?) to skip unnecessary compiles
- Allow dependencies between targets and dependency resolution, enforcing no
  circular dependencies
- Enable libraries as targets, automatically specifying the correct compiler
  flags like `-fPIC`
- Describe the `project.cmpl` file and it's options, this should include a way
  to specify additional compiler flags
- Allow a way to get the version from the latest `git` tag or fallback to get
  it from this `README` and update `VERSION` in `version.cpp` only if it
  differs (otherwise right now you either never rebuild it and miss changes or
  always rebuild it and may waste a lot of time relinking)

## Why?

This project is intended as a research project. My current intention is just to
make my life easier. A lot of optimizations to reduce compile times and ease
program understanding need to know what object files link to and all the flags
they're compiled with. The build system should also know what flags change
**what** code is compiled (example: different targets) and **how** code is
compiled (example: different optimization levels). If you're reading the code,
you don't care how it's compiled. My goal is to implement these features and
leverage them in code analysis and editors.

## Related Projects

* [Make][] - Far more general and not as useful to me

[Make]: http://www.gnu.org/software/make/
