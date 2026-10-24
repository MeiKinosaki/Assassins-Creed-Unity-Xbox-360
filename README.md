# Assassin's Creed Unity — Xbox 360 research port

This repository is an **experimental, independent porting/reimplementation scaffold** targeting Xbox 360. It is not currently a playable conversion of the Xbox One/PC game, and it does not contain or claim to reconstruct Ubisoft's proprietary source code or assets.

## Current portable gameplay core

- Inventory stacks with capacity and quantity validation, including slot availability and slot reuse.
- Capacity-limited FIFO gameplay event queue with explicit overflow rejection and bounded per-frame processing to limit frame spikes.
- Single-objective progress tracker.
- Equipment loadout slots: main hand, tool, outfit.
- Platform-service interfaces for clock, input, storage and logging, ready for platform-specific implementations.
- Portable runtime lifecycle with timing, pause/resume edge detection, configurable event queue capacity, bounded per-frame event processing, and stop state.
- Versioned little-endian save snapshot codec with a checksum.
- Adapter that captures/restores inventory, objective progress and equipment.
- Storage bridge that encodes/decodes save snapshots through an injected storage service, rejects oversized input at the storage boundary, and preserves output state when reads fail.
- Bounded desktop/reference file-storage backend with round-trip and failure-safety tests; this is not an Xbox 360 save-data backend.
- CMake build and GitHub Actions tests using standard C++11.

These are generic foundation modules. They are not yet connected to the original game's engine, animation, combat, AI, missions, renderer, audio, or world data.

## Build and run tests

Requirements: CMake 3.10+ and a C++11 compiler.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 2
ctest --test-dir build --output-on-failure
```

The CI workflow is at [Gameplay Core CI](.github/workflows/gameplay-core.yml).

## Xbox 360 status

No Xbox 360-targeted build, `default.xex`, console boot, frame rate, or compatibility result is currently verified by this repository's portable CI. A separate compatible Xbox 360 toolchain and platform layer are still required. A successful desktop test run does not prove Xbox 360 compatibility.

## Next technical milestones

1. Keep portable tests green and expand malformed-save/boundary coverage.
2. Implement and test a concrete platform storage backend behind the storage interface.
3. Verify which Xbox 360 homebrew SDK/toolchain can compile the minimal platform target.
4. Produce and test a minimal target executable before attempting larger game systems.
5. Implement game-specific systems only with source code and assets the project is authorized to use.

## Legal and technical note

A binary for Xbox One/PC cannot simply be recompiled into an Xbox 360 executable. The CPU architecture, APIs, memory model, graphics pipeline and performance constraints differ substantially. A functional port requires extensive engineering and lawful access to the code/assets or an independently implemented compatible replacement.
