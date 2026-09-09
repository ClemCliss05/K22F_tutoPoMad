# Architecture

## Overview

The architecture isolates hardware-specific code from portable application logic.

```text
app
 ↓
services
 ↓
core
 ↓
drivers
 ↓
platform
```

---

## Layers

### App

Application entry point.

Responsibilities:

- System initialization
- Component wiring
- Main execution flow

Files:

```text
firmware/app
```

---

### Services

Application-specific behavior.

Responsibilities:

- Application behavior
- Feature implementation
- High-level workflows

Rules:

- Can use Core
- Must not access hardware directly

Files:

```text
firmware/services
```

---

### Core

Hardware-independent utilities.

Responsibilities:

- Reusable modules
- Algorithms
- Generic infrastructure

Examples:

```text
logger/
ringbuffer/
```

Rules:

- No hardware dependencies
- Fully testable on host

Files:

```text
firmware/core
```

---

### Drivers

Hardware abstraction layer.

Responsibilities:

- Peripheral access
- Hardware interfaces

Examples:

```text
gpio/
uart/
```

Rules:

- No business logic
- Minimal processing

Files:

```text
firmware/drivers
```

---

### Platform

Target-specific implementation.

Responsibilities:

- Startup code
- Interrupt handling
- Clock configuration
- Linker script
- Vendor CMSIS files

Files:

```text
firmware/platform/mcu_name/
├── startup/
├── linker/
├── cmsis/
├── bsp/
```

Contains:

- startup_mcu_name file
- linkerscript.ld
- CMSIS Core
- Target device headers

---

## Dependency Rules

Allowed:

```text
app      → services
services → core
drivers  → platform
```

Forbidden:

```text
core     → drivers
core     → platform

services → platform
services → registers

app      → registers
```

---

## Build Modes

### Firmware Build

```text
ANALYSIS=OFF
```

Builds:

- app
- services
- core
- drivers
- platform

---

### Analysis Build

```text
ANALYSIS=ON
```

Builds:

- core
- services
- tests (optional)

Used for:

- Unit tests
- clang-tidy
- cppcheck
- CodeQL

---

## Testing Strategy

Scope:

```text
core
services
```

Execution:

```text
Host machine
```

Benefits:

- Fast
- Deterministic
- Hardware-independent
- CI-friendly

---

## Static Analysis Strategy

| Tool | Purpose |
| -------- | ---------- |
| clang-format | formatting |
| clang-tidy | code quality |
| cppcheck | bug detection |
| CodeQL | security analysis |

CI automatically executes all checks.

---

## Goal

Provide a firmware foundation that is:

- Portable
- Testable
- Maintainable
- Scalable
- CI/CD friendly
