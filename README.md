# PP10

## Goal

In this exercise you will:

* Explore custom `struct` types and `typedef` in C.
* Link against existing system libraries (e.g., `-lm`).
* Create and evolve a custom C library from header-only to a precompiled static archive and install it system-wide.
* Install and use a third-party JSON library (`jansson`) via your package manager.
* Download, build, and install a GitHub-hosted library with a Makefile into standard include/lib paths.

**Important:** Start a stopwatch when you begin and work uninterruptedly for **90 minutes**. When time is up, stop immediately and record where you paused.

---

## Workflow

1. **Fork** this repository on GitHub.
2. **Clone** your fork locally.
3. Create a `solutions/` directory in the project root:

   ```bash
   mkdir solutions
   ```
4. For each task, add or modify source files under `solutions/`.
5. **Commit** and **push** your solutions.
6. **Submit** your GitHub repo link for review.

---

## Prerequisites

* GNU C compiler (`gcc`) and linker (`ld`).
* Make utility (`make`).
* `apt` or your distro’s package manager.

---

## Tasks

### Task 0: Exploring `typedef` and `struct`

**Objective:** Define and use a custom struct type with `typedef`.

1. Create `solutions/point.h` with:

   ```c
   typedef struct {
       double x;
       double y;
   } Point;
   ```
2. Create `solutions/point_main.c` that includes `point.h`, declares a `Point p = {3.0, 4.0}`, and prints its distance from origin using `sqrt(p.x*p.x + p.y*p.y)` (linking `-lm`).

#### Reflection Questions

1. **What does `typedef struct { ... } Point;` achieve compared to `struct Point { ... };`?** *With typedef struct {...} we created a new type, you only have to wirte the name of the type, like Point = ... . That is not the same for struct Point{...}, there you have to write always struct before you declare a new var. .
2. **How does the compiler lay out a `Point` in memory?** *The point are simply two type double numbers which sit in the memory like any other double value. In my case it's  -0x10(%rbp) and -0x8(%rbp).

---

### Task 1: Linking the Math Library (`-lm`)

**Objective:** Compile and link a program against the math library.

1. In `solutions/`, compile `point_main.c` with:

   ```bash
   gcc -o solutions/point_main solutions/point_main.c -lm
   ```
2. Run `./solutions/point_main` and verify it prints `5.0`.

#### Reflection Questions

1. **Why is the `-lm` flag necessary to resolve `sqrt`?**
2. **What happens if you omit `-lm` when calling math functions?**

---

### Task 2: Header-Only Library

**Objective:** Create a simple header-only utility library.

1. Create `solutions/libutil.h` with an inline function:

   ```c
   #ifndef LIBUTIL_H
   #define LIBUTIL_H
   static inline int clamp(int v, int lo, int hi) {
       if (v < lo) return lo;
       if (v > hi) return hi;
       return v;
   }
   #endif
   ```
2. Create `solutions/util_main.c` that includes `libutil.h`, calls `clamp(15, 0, 10)` and prints `10`.
3. Compile and run:

   ```bash
   gcc -o solutions/util_main solutions/util_main.c
   ./solutions/util_main
   ```

#### Reflection Questions

1. **What are the advantages and drawbacks of a header-only library?**
2. **How does `static inline` affect linkage and code size?**

---

### Task 3: Precompiled Static Library

**Objective:** Convert the header-only utility into a compiled static library and link it manually.

1. Split `clamp` into `solutions/util.c` & `solutions/util.h` (remove `inline` and `static`).
2. Compile:

   ```bash
   gcc -c solutions/util.c -o solutions/util.o
   ```
3. Create the executable linking manually:

   ```bash
   gcc -o solutions/util_main_pc solutions/util.o solutions/util_main.c
   ```
4. Run `./solutions/util_main_pc` to verify output.

#### Reflection Questions

1. **Why must you include `solutions/util.o` when linking instead of just the header?**
2. **What symbol resolution occurs at compile vs. link time?**

---

### Task 4: Packaging into `.a` and System Installation

**Objective:** Archive the static library and install it to system paths.

1. Create `libutil.a`:

   ```bash
   ar rcs libutil.a solutions/util.o
   ```
2. Move headers and archive:

   ```bash
   sudo cp solutions/util.h /usr/local/include/libutil.h
   sudo cp libutil.a /usr/local/lib/libutil.a
   sudo ldconfig
   ```
3. Compile a test program using system-installed lib:

   ```bash
   gcc -o solutions/util_sys solutions/util_main.c -lutil
   ```

   (assumes `#include <libutil.h>`)

#### Reflection Questions

1. **How does `ar` create an archive, and how does the linker find `-lutil`?**
2. **What is the purpose of `ldconfig`?**

---

### Task 5: Installing and Using `jansson`

**Objective:** Install a third-party JSON library and link against it.

1. Install via `apt`:

   ```bash
   sudo apt update && sudo apt install libjansson-dev
   ```
2. Create `solutions/json_main.c`:

   ```c
   #include <jansson.h>
   #include <stdio.h>
   int main(void) {
       json_t *root = json_pack("{s:i, s:s}", "id", 1, "name", "Alice");
       char *dump = json_dumps(root, 0);
       printf("%s\n", dump);
       free(dump);
       json_decref(root);
       return 0;
   }
   ```
3. Compile and run:

   ```bash
   gcc -o solutions/json_main solutions/json_main.c -ljansson
   ./solutions/json_main
   ```

#### Reflection Questions

1. **What files does `libjansson-dev` install, and where?**
2. **How does the linker know where to find `-ljansson`?**

---

### Task 6: Building and Installing a GitHub Library

**Objective:** Download, build, and install a library from GitHub using its Makefile.

1. Choose a small C library on GitHub (e.g., `sesh/strbuf`).
2. Clone and build:

   ```bash
   git clone https://github.com/sesh/strbuf.git
   cd strbuf
   make
   ```
3. Install to system paths:

   ```bash
   sudo make install PREFIX=/usr/local
   sudo ldconfig
   ```
4. Write `solutions/strbuf_main.c` that includes `strbuf.h`, uses its API, and prints a test string.
5. Compile and link:

   ```bash
   gcc -o solutions/strbuf_main solutions/strbuf_main.c -lstrbuf
   ./solutions/strbuf_main
   ```

#### Reflection Questions

1. **What does `make install` do, and how does `PREFIX` affect installation paths?**
2. **How can you inspect a library’s exported symbols to verify installation?**

---

**Remember:** Stop after **90 minutes** and record where you stopped.
