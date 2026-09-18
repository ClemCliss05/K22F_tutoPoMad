# MK22FN512 tuto PoMad

A modern embedded C++ tutorial and firmware foundation for the **NXP MK22FN512**, adapted from the original [PoMad tutorial](https://www.pomad.fr/) for STM32 MCUs.

The project focuses on learning how to build **reliable, maintainable and secure embedded firmware** using modern C++, CMake, testing and CI/CD practices.

Go through the commits in order to understand the different development steps.

---

## Purpose

This project provides a structured foundation for developing TARGET firmware.

Main objectives:

* Separate application logic from hardware
* Keep hardware-independent code testable
* Reduce coupling between components
* Build reusable drivers and services
* Learn modern embedded C++
* Use CMake for reproducible builds
* Integrate automated testing and static analysis
* Apply security practices to embedded software

---

## Tools

Main development tools:

* C++17 / C11
* CMake
* ARM GCC (`arm-none-eabi-g++`)
* GoogleTest
* GDB
* J-Link
* Git / GitHub
* GitHub Actions

VSCode extensions:

* C/C++
* CMake Tools
* Cortex-Debug

---

## Project Structure

```text
firmware/
├── app/          # Application entry point
├── services/     # Application-level services
├── core/         # Hardware-independent components
├── drivers/      # MCU peripheral drivers
└── platform/     # TARGET-specific code

tests/             # Host unit tests
scripts/           # Build and analysis scripts
cmake/             # CMake toolchain configuration
docs/              # Project documentation
debug/             # SVD/XML peripheral descriptions
.github/           # CI/CD workflows
```

See [ARCHITECTURE.md](ARCHITECTURE.md) for more details.

---

## Build Firmware

```bash
./scripts/build.sh
```

The firmware is cross-compiled for the TARGET using ARM GCC.

Generated files include:

```text
firmware.elf
firmware.bin
firmware.hex
firmware.map
firmware.asm
```

---

## Run Tests

```bash
cmake -B build/tests \
    -DANALYSIS=ON \
    -DTESTS=ON

cmake --build build/tests
ctest --test-dir build/tests
```

Unit tests execute on the host using GoogleTest.

Host-side sanitizers such as **ASAN** and **UBSAN** can be used to detect memory and undefined-behavior issues in portable code.

---

## Flash Firmware

```bash
./scripts/build.sh flash
```

The firmware is flashed using **J-Link** and `scripts/flash_jlink.sh`.

---

## Static Analysis

### clang-format

Checks and formats the source code according to the project style.

```bash
./scripts/clang-format.sh
```

### clang-tidy

Performs C++ code-quality and correctness analysis.

```bash
./scripts/clang-tidy.sh
```

Current scope:

```text
firmware/core
```

### cppcheck

Performs additional bug, portability, performance and style analysis.

```bash
./scripts/cppcheck.sh
```

Analyzed source:

```text
firmware/core
firmware/services
firmware/drivers
firmware/app
tests
```

Vendor CMSIS files and generated/target-specific startup and linker files are excluded.

### Run Everything

```bash
./scripts/static_analysis.sh
```

This runs the project's static-analysis checks (clang-format, clang-tidy and cppcheck).

### CodeQL

CodeQL performs security-oriented static analysis and data-flow analysis.

```bash
./scripts/codeql.sh
```

The CodeQL database is created from the **real ARM firmware compilation**, allowing CodeQL to analyze the actual C/C++ firmware source and build context.

The project currently uses:

```text
cpp-security-extended
```

Main focus:

* Memory safety
* Buffer overflows
* Unsafe memory operations
* Uninitialized data
* Dangerous APIs
* Pointer and lifetime issues
* Security-sensitive data flows

CodeQL is complementary to cppcheck and clang-tidy rather than a replacement for them.

---

## CI/CD

GitHub Actions automatically performs the development checks.

### Code Quality

* clang-format
* clang-tidy
* cppcheck
* Host unit tests
* ASAN
* UBSAN

### Firmware

* ARM cross-compilation
* ELF generation
* BIN generation
* HEX generation
* Firmware size information

### Security

* CodeQL database creation
* CodeQL security analysis
* SARIF security results

The goal is to detect problems before firmware reaches the target hardware.

---

## Design Goals

The project follows these principles:

* Separation of concerns
* Minimal coupling
* Hardware-independent core logic
* Explicit hardware drivers
* Testability
* Maintainability
* Reproducible builds
* Automated verification
* Security by default

---

## License

MIT
