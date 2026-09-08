# GZEL Graphics Library

* **GZEL** a lightweight C library for Linux. It runs directly on **OpenGL** and **XCB** without any extra bloat.
* v26.9.0 is now stable.

---

## How It Works

* **Single Header File:** Just drop `<gzel/gzel.h>` into your C source code.
* **Shared Library (`.so`):** Clean dynamic library linkage with zero heavy dependencies.
* **Easy Installation:** Pre-packaged as a `.deb` package for quick setup on Debian and Ubuntu.

---

## 💡 Educational Guide:

### 1. Where Does `libgzel.so` & `gzel.h` Live?

For `gcc` and the Linux runtime linker (`ld.so`) to automatically discover your library without needing complex local paths, the shared library file must reside in a standard system directory:

* **Header Path:** `/usr/local/include/gzel/gzel.h` *(Where `gcc` searches when you write `#include <gzel/gzel.h>`)*
* **Shared Object Path:** `/usr/lib/libgzel.so`   *(Where `gcc` and Linux search when you pass `-lgzel`)*

> **Note:** Installing via the provided `.deb` package handles this placement automatically. If you are building manually or placing files yourself, ensure `libgzel.so` is placed in `/usr/lib/` (or `/usr/local/lib/`).

---

### 2. Hello Triangle
```bash
#include <gzel/gzel.h>

u16 screen_width  = 500;
u16 screen_height = 500;

int main()
{   
    gzel_set_window(screen_width, screen_height, "Hello Triangle");
    gzel_link_gl_platfrom();

    while (!gzel_window_should_close())
    {   
        // Begin Draw
        gzel_start_cycle();

        // Hello Triangle
        gzel_clear_backgorund(CYAN);
        gzel_tri((vec2f32){0,500}, (vec2f32){250,0}, (vec2f32){500,500}, RED);
        
        // End Draw
        gzel_end_cycle();
    }
    return 0;
}
```

---

### 3. Full Compilation Command For Linux Desktop

To compile your application, run the full `gcc` command:

```bash
gcc example.c -lgzel -lxcb -lxcb-xkb -lEGL -lGL -lm -o example
```
---

### 4. get VS Code to properly color recognize (syntax highlight)

* Extension: Install the official Microsoft C/C++ extension for AST-based syntax highlighting and semantic coloring.
* Include Path: Press Ctrl+Shift+P -> C/C++: Edit Configurations (UI) -> Add header's directory to Include path `"/usr/local/include/gzel/**"`

  You should end up with something similar:
  
  ```bash
  {
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**",
                "/usr/local/include/gzel/**"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/clang",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "linux-clang-x64"
        }
    ],
    "version": 4
  }
