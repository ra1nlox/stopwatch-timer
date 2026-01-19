# Stopwatch / Timer

Very simple stopwatch/timer made using C, [https://github.com/raysan5/raylib](Raylib) and [https://github.com/raysan5/raygui](Raygui)

# Build

## Dependencies

- gcc
- mingw-w64-gcc (for Windows build)
- raylib (included)
- raygui (included)

## Building

```bash
  # to make a binary for linux
  make linux

  # to make an executable for win64
  make win
```

These two commands create a `dist` directory in which will lie the said executables

# Usage

## Controls

- Space ― start/pause
- R ― reset the count
- S ― switch the count mode (stopwatch by default, timer if pressed)
- ↑ or ↓ ― increase/decrease seconds/minutes
- ← or → ― select minutes or seconds to increment/decrement
