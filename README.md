# MK22FN512 tuto PoMad (https://www.pomad.fr/)

A modern tuto for MK22FN512 microcontrollers focused on learning how to use an MCU.
The tuto PoMad is adapted for STM32 MCU but this git is designed for MK22FN512 NXP MCU.
Go through the diferent commit in order to understand each steps of the tuto.

---

## Purpose

This project provides a solid foundation for developing reliable and maintainable MK22FN512 firmware.

Main objectives:

- Separate application logic from hardware
- Improve testability
- Reduce coupling
- Support long-term scalability
- Integrate modern development workflows

---

## Tools

VSCode main extensions:

- C/C++
- CMake Tools
- Cortex-Debug
- GitHub Actions

---

## Project Structure

```text
firmware/
├── app/        # Application entry point
├── core/       # Hardware-independent modules
├── services/   # Business logic
├── drivers/    # Application services
└── platform/   # Target-specific code

tests/                    # Host unit tests
scripts/                  # Build and analysis tools
cmake/                    # Toolchain configuration
docs/                     # Project documentation
debug/                    # svd/xml files for CPU peripherals description
.github/                  # CI/CD workflows
```

See [ARCHITECTURE.md](ARCHITECTURE.md) for details.

---

## Build Firmware

```bash
./scripts/build.sh
```

Generated files:

```text
firmware.elf
firmware.bin
firmware.hex
firmware.map
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

Tests execute on the host using GoogleTest.

---

## Flash Firmware

```bash
./scripts/build.sh flash
```

Flash firmware.elf by using JLinkExe and scripts/flash_jlink.sh

---

## Static Analysis

### clang-format

```bash
./scripts/clang-format.sh
```

### clang-tidy

```bash
./scripts/clang-tidy.sh
```

Scope:

```text
core
services
```

### cppcheck

```bash
./scripts/cppcheck.sh
```

Scope:

```text
core
services
tests
```

### CodeQL

```bash
./scripts/codeql.sh
```

Focus:

```text
Security
Memory safety
Unsafe patterns
```

### Run Everything

```bash
./scripts/static_analysis.sh
```

---

## CI/CD

GitHub Actions automatically performs:

### Analysis

- clang-format validation
- clang-tidy
- cppcheck
- unit tests
- ASAN
- UBSAN

### Firmware

- ARM cross compilation
- ELF generation
- BIN generation
- HEX generation

### Security

- CodeQL analysis

---

## Design Goals

- Separation of concerns
- Hardware abstraction
- Testability
- Maintainability
- Reproducible builds
- Security by default

---

## License

MIT
