# kitty-tabs

A fast fuzzy tab switcher for Kitty terminal built in pure C using `fzf`.

It lets you quickly search and switch between Kitty tabs using a lightweight native binary instead of slow scripting layers.

---

## ⚡ Why this exists

Switching tabs in terminal workflows should feel instant.

Most implementations (Node/Python scripts) add unnecessary overhead:
- runtime startup cost
- slow process spawning
- extra abstraction layers

This project removes all of that and directly uses:
- C for execution speed
- POSIX APIs for process control
- `fzf` for fuzzy selection

Result: near-instant tab switching experience

---

## Features

- Fast native C implementation
- Fuzzy tab search using `fzf`
- Kitty overlay support
- Lightweight execution path

---

## Setup

### 1. Configure
```bash
cmake --preset=default
```

---

### 2. Build
```bash
cmake --build build
```

---

### 3. Keep at ~/.local/bin (macOS)
```bash
cp ./kitty_tabs ~/.local/bin
```

---

### 4. Bind with a keyboard shortcut in kitty.conf
```bash
map option+space launch --type=overlay ~/.local/bin/kitty_tabs
```
