# PP10

### Did this task in 90 Minutes, but I had several problems here and there.

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

1. **What does `typedef struct { ... } Point;` achieve compared to `struct Point { ... };`?** *With typedef struct {...} we created a new type, you only have to wirte the name of the type, like Point = ... . That is not the same for struct Point{...}, there you have to write always struct before you declare a new var. .*
2. **How does the compiler lay out a `Point` in memory?** *The point are simply two type double numbers which sit in the memory like any other double value. In my case it's  -0x10(%rbp) and -0x8(%rbp).*

---

### Task 1: Linking the Math Library (`-lm`)

**Objective:** Compile and link a program against the math library.

1. In `solutions/`, compile `point_main.c` with:

   ```bash
   gcc -o solutions/point_main solutions/point_main.c -lm
   ```
2. Run `./solutions/point_main` and verify it prints `5.0`.

#### Reflection Questions

1. **Why is the `-lm` flag necessary to resolve `sqrt`?** *With -lm we are linking to the compiled parth of the math.h library which uses many useful functions like sqrt(), which is not part of the basic c language.*
2. **What happens if you omit `-lm` when calling math functions?** *In my case the programe wont get even compiled, thats because we use a function that is not defined. The compiler simply doesn' t know what to do.*

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

1. **What are the advantages and drawbacks of a header-only library?** *The main advantage of an header-only librarie is that you could use several small functions that you have writen in small programms and you only have to include them, you don't have to link any obeject datas.  The problem is, that you can't get these libraries to big, otherwise they get inefficient. And you have to use inline or static functions to avoid double definitoins.*
2. **How does `static inline` affect linkage and code size?** *I tried it and it saves like 4byte. Thats not as much as I thaught, but I think it's because we use such a small header file. I have also read, that if I would use several .c files in one programm, that we would see a huge differense.

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

1. **Why must you include `solutions/util.o` when linking instead of just the header?** *The header only declares that there will be a function called clamp using these vars. But it doesn't have the information about what this function does. These informations are saved inside the util.c file. Therefor we need to link the object file util.o, so that our main function is able to call clamp.*
2. **What symbol resolution occ
urs at compile vs. link time?** *At compile time the compiler sees that main uses a function, which is declared inside the header. That is the information, that is needed, so that the compiler knows that for our example "clamp" is defined inside any file. At link time the compiler searches the object file for the "right functions" in our case he finds them insde the libutil.h defined "clamp" function. He know links the function to the part of the mainfunction, so that it works like the function would be inside of main.*
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

1. **How does `ar` create an archive, and how does the linker find `-lutil`?** * ar does create an archive, that holds all the objectdatas that is needed to define all symbols.* 
2. **What is the purpose of `ldconfig`?** *In our case ldconfig is needed, so that the system is able to find the new librarie. Normally it's used for dynamic libraries, so that the compiler knows where to search these libraries.*

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

1. **What files does `libjansson-dev` install, and where?** */.
/usr
/usr/include
/usr/include/jansson.h
/usr/include/jansson_config.h
/usr/lib
/usr/lib/x86_64-linux-gnu
/usr/lib/x86_64-linux-gnu/libjansson.a
/usr/lib/x86_64-linux-gnu/pkgconfig
/usr/lib/x86_64-linux-gnu/pkgconfig/jansson.pc
/usr/share
/usr/share/doc
/usr/share/doc/libjansson-dev
/usr/share/doc/libjansson-dev/copyright
/usr/lib/x86_64-linux-gnu/libjansson.so
/usr/share/doc/libjansson-dev/changelog.Debian.gz*
2. **How does the linker know where to find `-ljansson`?** * The l infront of -ljansson does mean librarie, so the linker knows that he has to search for a librarie called jansson. The linker than searches inside a few standard directories for libraries after the .so file from jansson, where all functions are defined. Usually the linker would find the librarie that way. If the linker doesn't find it inside the standard directoris, you have to give him the correct path.

---

### Task 6: Building and Installing a GitHub Library (This task didn't work right on my system, it said that the repo does not exisit, I did the task as good as I could, by searching for answers for the questions.)

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

1. **What does `make install` do, and how does `PREFIX` affect installation paths?** *Make install does exequte the makefile, so that the library gets copied to s location that is executable from the hole system, so that it is usable from anylocation on the system. The prefix does determine the main path of the program.
2. **How can you inspect a library’s exported symbols to verify installation?** *With readelf or nm you are able to see the available symbols. You know that the library is complete if nm/readelf gives you a list with all symbols.

---

**Remember:** Stop after **90 minutes** and record where you stopped.
