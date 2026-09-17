# Architecture

## Overview

The firmware is organized into layers that separate application behavior, reusable logic and hardware-specific code.

```text
                         ┌─────────────┐
                         │     app     │
                         └──────┬──────┘
                                │
                                ▼
                         ┌─────────────┐
                         │  services   │
                         └──────┬──────┘
                                │
                    ┌───────────┴───────────┐
                    ▼                       ▼
             ┌─────────────┐        ┌─────────────┐
             │    core     │        │   drivers   │
             └─────────────┘        └──────┬──────┘
                                           │
                                           ▼
                                    ┌─────────────┐
                                    │  platform   │
                                    └─────────────┘
```

The main principle is:

> **Application behavior should not depend directly on MCU registers.**

---

## Layers

### App

Application entry point and system composition.

Responsibilities:

* System initialization
* Instantiate components
* Connect services, drivers and core modules
* Run the main application flow

Location:

```text
firmware/app/
```

`app` is where the different components are assembled.

---

### Services

Application-level functionality built using reusable components and hardware drivers.

Responsibilities:

* Application behavior
* Feature implementation
* High-level workflows
* Coordination between core components and drivers

Examples:

```text
services/
├── sensor/
├── delay/
└── logger/
```

A service may depend on:

```text
core
drivers
```

For example:

```text
UartLoggerBackend
        ↓
     Uart driver
```

Services should not access MCU registers directly.

---

### Core

Hardware-independent and reusable components.

Responsibilities:

* Generic algorithms
* Data structures
* Infrastructure
* Hardware-independent logic

Examples:

```text
core/
├── logger/
└── ringbuffer/
```

Rules:

* No MCU register access
* No CMSIS dependency
* No hardware driver dependency
* Prefer host-testable code

This layer is the main target for fast unit testing.

---

### Drivers

Low-level hardware drivers providing a clean interface to MCU peripherals.

Responsibilities:

* Peripheral configuration
* Register access
* Hardware communication
* Minimal hardware-related processing

Examples:

```text
drivers/
├── gpio/
├── uart/
├── adc/
└── pit/
```

Rules:

* Hardware-specific logic belongs here
* No application/business logic
* No dependency on services
* Keep interfaces simple and explicit

Drivers depend on the target platform.

---

### Platform

Target-specific MCU support.

Responsibilities:

* Startup code
* Interrupt handling
* Clock configuration
* Linker script
* CMSIS
* Device headers
* MCU-specific definitions

Location:

```text
firmware/platform/mk22fn512/
```

Structure:

```text
platform/mk22fn512/
├── startup/
├── linker/
├── cmsis/
└── bsp/
```

The platform layer contains code that is tightly coupled to the MK22FN512.

---

## Dependency Rules

### Allowed

```text
app       → services

services  → core
services  → drivers

drivers   → platform
```

### Forbidden

```text
core      → drivers
core      → platform

services  → platform
services  → registers

app       → registers
```

The important rule is that **higher-level code uses interfaces provided by lower-level components instead of accessing hardware directly**.

---

## Build Architecture

### Firmware Build

The firmware is cross-compiled using ARM GCC.

```text
app
 ↓
services
 ↓
core + drivers
 ↓
platform
 ↓
ARM GCC
 ↓
firmware.elf
```

The build also generates:

```text
firmware.bin
firmware.hex
firmware.map
firmware.asm
```

---

### Host Analysis / Test Build

Portable components can be compiled for the host.

```text
core
 ↓
services
 ↓
GoogleTest
 ↓
Host executable
```

This allows fast execution of unit tests without hardware.

Hardware-dependent drivers are validated primarily through:

* ARM compilation
* static analysis
* target hardware
* integration tests

---

## Testing Strategy

The project uses several complementary verification methods.

### Unit Tests

Target:

```text
core
portable services
```

Environment:

```text
Host + GoogleTest
```

Advantages:

* Fast
* Deterministic
* Easy to run in CI
* No hardware required

---

### Sanitizers

Host builds can use:

```text
ASAN
UBSAN
```

They help detect:

* Memory errors
* Buffer issues
* Use-after-free
* Undefined behavior

They complement static analysis but do not replace it.

---

## Static Analysis Strategy

Different tools are used for different purposes.

| Tool         | Main purpose                             |
| ------------ | ---------------------------------------- |
| clang-format | Consistent formatting                    |
| clang-tidy   | C++ correctness and code quality         |
| cppcheck     | Bugs, portability, performance and style |
| CodeQL       | Security and data-flow analysis          |

### cppcheck

cppcheck analyzes the project source files under:

```text
firmware/
tests/
```

while excluding vendor and target-support files such as:

```text
cmsis/
startup/
linker/
```

This provides broad coverage of the project's own source code.

---

### clang-tidy

clang-tidy currently focuses on:

```text
firmware/core
firmware/services
```

These components are suitable for host-based C++ analysis using the project's compilation database.

Hardware-specific analysis can be extended later using the ARM compilation database.

---

### CodeQL

CodeQL is used as the project's security-oriented static analyzer.

The database is created from an actual ARM firmware build:

```text
CMake
  ↓
ARM GCC
  ↓
compiled firmware
  ↓
CodeQL database
```

The project currently uses:

```text
codeql/cpp-queries:codeql-suites/cpp-security-extended.qls
```

The security analysis focuses on patterns such as:

* Buffer overflows
* Unsafe memory access
* Uninitialized variables
* Dangerous functions
* Pointer/lifetime errors
* Security-sensitive data flows

CodeQL is especially useful when the project starts processing external data such as Bluetooth packets.

Temporary vulnerable test cases may be used to verify that CodeQL is correctly configured. These tests are **validation tests only** and must not become part of production firmware.

---

## Security Analysis Philosophy

No single static-analysis tool can detect every class of firmware vulnerability.

The project therefore uses several complementary layers:

```text
             Source Code
                  │
       ┌──────────┼──────────┐
       ▼          ▼          ▼
   cppcheck   clang-tidy   CodeQL
       │          │          │
       ▼          ▼          ▼
     Bugs      C++ quality  Security
                  │
                  ▼
             Unit Tests
                  │
                  ▼
             ASAN / UBSAN
                  │
                  ▼
             Target Hardware
```

This layered approach is particularly important for embedded software where memory safety, undefined behavior and external input handling can directly affect system reliability and security.

---

## CMake Organization

The project is split into independent CMake targets.

```text
platform_mk22fn512
        ↑
     drivers
        ↑
      core
        ↑
    services
        ↑
       app
```

Each layer exposes only the dependencies required by the next layer.

This keeps the build modular and makes individual components easier to test and maintain.

---

## Long-Term Architecture

The current architecture is intentionally simple.

Future components can be added without changing the basic structure:

```text
firmware/
├── app/
├── core/
│   ├── logger/
│   └── ringbuffer/
├── services/
│   ├── sensor/
│   ├── communication/
│   └── security/
├── drivers/
│   ├── gpio/
│   ├── uart/
│   ├── adc/
│   └── bluetooth/
└── platform/
    └── mk22fn512/
```

For the future Bluetooth part, a typical flow will become:

```text
Bluetooth Driver
       ↓
Communication Service
       ↓
Packet Parser
       ↓
Core data structures
       ↓
Application
```

Security checks should be performed as close as possible to the point where external data enters the system.

---

## Goal

The architecture aims to provide firmware that is:

* Portable
* Testable
* Maintainable
* Scalable
* Secure
* CI/CD friendly
* Suitable for professional embedded development
