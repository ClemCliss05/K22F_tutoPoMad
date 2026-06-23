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
- SARIF Viewer

---

## Architecture

Layered architecture:

```text
app
 ↓
services
 ↓
core
 ↓
drivers
 ↓
platform (MK22FN512)
```

See `ARCHITECTURE.md` for details.

---

## Project Structure

```text
firmware/
├── app/                  # Application entry point
├── core/                 # Hardware-independent modules
│   ├── logger/
│   └── ringbuffer/
├── services/             # Application services
│   └── sensor/
├── drivers/              # Peripheral drivers
│   ├── gpio/
│   └── uart/
└── platform/
    └── mk22fn512/
        ├── startup/
        ├── linker/
        ├── cmsis/
        ├── clock.cpp
        └── interrupt.cpp

tests/                    # Host unit tests
scripts/                  # Build and analysis tools
cmake/                    # Toolchain configuration
```

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

## Static Analysis

### clang-format

```bash
./scripts/clang-format.sh
```

### clang-tidy

```bash
./scripts/clang-tidy.sh
```

### cppcheck

```bash
./scripts/cppcheck.sh
```

### CodeQL

```bash
./scripts/codeql.sh
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
- SARIF reporting

---

## Design Principles

- Separation of concerns
- Hardware abstraction
- Testability first
- Dependency minimization
- Reproducible builds
- CI-driven quality control

---

## License

MIT
