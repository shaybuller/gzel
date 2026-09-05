# NTR Rendering Library

**NTR** is a fast, lightweight C graphics library for Linux. It runs directly on **OpenGL** and **XCB** without any extra bloat

---

## How It Works

* **Single Header File:** Just drop `<ntr/ntr.h>` into your C source code.
* **Shared Library (`.so`):** Clean dynamic library linkage with zero heavy dependencies.
* **Easy Installation:** Pre-packaged as a `.deb` package for quick setup on Debian and Ubuntu.

---

## 💡 Educational Guide: Shared Libraries & Linking

### 1. Where Does `libntr.so` Live?

For `gcc` and the Linux runtime linker (`ld.so`) to automatically discover your library without needing complex local paths, the shared library file must reside in a standard system directory:

* **Header Path:** `/usr/local/include/ntr/ntr.h` *(Where `gcc` searches when you write `#include <ntr/ntr.h>`)*
* **Shared Object Path:** `/usr/lib/libntr.so`   *(Where `gcc` and Linux search when you pass `-lntr`)*

> **Note:** Installing via the provided `.deb` package handles this placement automatically. If you are building manually or placing files yourself, ensure `libntr.so` is placed in `/usr/lib/` (or `/usr/local/lib/`).

---

### 2. Full Compilation Command & Flag Breakdown

To compile your application, run the full `gcc` command:

```bash
gcc example.c -lntr -lxcb -lxcb-xkb -lEGL -lGL -lm -o example
