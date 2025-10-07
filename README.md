# Recursive Tag Interpolation

A small C library that expands `{{tag}}` placeholders inside strings, following nested references until every tag is resolved or a recursion safeguard kicks in. It ships with a sample executable, a micro-benchmark, and several repro programs that exercise tricky edge cases.

## Features
- Expands tags recursively while tracking active tags to break cycles and respect `MAX_RECURSION_DEPTH` (default 3).
- Uses a growable buffer so callers do not need to guess the output size; `ProcessInput` hands back an owned `char*`.
- Ships as a static library (`bin/libtagger.a`) plus convenience executables (`main`, `benchmark`).
- Includes utilities (`util_lib.*`) for safe buffer management, formatted error reporting, and a terminal progress bar.
- Provides self-contained repro programs under `tests/` that document current limitations and bugs.

## Build
Requires a POSIX-ish toolchain with `gcc` (or another C11 compiler), `make`, and `ar`.

```bash
make            # builds bin/libtagger.a and drops ./main + ./benchmark symlinks
make clean      # removes build artefacts
```

Debug symbols are enabled by default (`-g`); edit `CFLAGS` in the top-level `Makefile` to change build flags.

## Running The Samples
```bash
./main
# → Hello, Qasim! Welcome to example.com.

./benchmark
# → streams a progress bar and writes benchmarks/BENCHMARK_AppendToBuffer.csv
```

The benchmark sweeps buffer sizes and append counts to measure the cost of repeated `AppendToDynamicBuffer` calls.

## Using The Library In Your Code
1. Include the public headers:
   ```c
   #include "tagger_lib.h"
   #include "util_lib.h"
   ```
2. Populate a `TagDictionary` with `Tag { Name, Replacement }` pairs. Replacements may contain further tags.
3. Initialise a `RecursionContext` (zeroed depth and active-tag tracking).
4. Call `ProcessInput`, free the returned string when done.

```c
Tag tags[] = {
    { "name", "Ada" },
    { "greeting", "Hello, {{name}}!" },
    { "site", "example.com" }
};

TagDictionary dict = { .Tags = tags, .Count = 3 };
RecursionContext ctx = {0};

char *output = ProcessInput("{{greeting}} Welcome to {{site}}.", &ctx, &dict);
printf("%s\n", output);
free(output);
```

Link against the static library produced in `bin/`:
```bash
gcc demo.c bin/libtagger.a -I. -lm -o demo
```

The tag delimiters default to `{{` / `}}`; override them by redefining `START_TAG` / `END_TAG` before including `tagger_lib.h` or by editing the header directly. Raise or lower `MAX_RECURSION_DEPTH` the same way.

## API Reference

### Constants
- `START_TAG` (`char*`): Leading delimiter for tags (defaults to `"{{"`).
- `END_TAG` (`char*`): Trailing delimiter for tags (defaults to `"}}"`).
- `MAX_RECURSION_DEPTH` (`int`): Maximum nested tag resolution depth (defaults to `3`).
- `BUFFER_SIZE` (`int`): Initial buffer size used throughout `util_lib` (defaults to `32`).
- `PROGRESS_BAR_WIDTH` (`int`): Characters drawn by `PrintProgressBar` (defaults to `50`).

### Types

#### Tag
```c
typedef struct {
    char *Name;        // tag name without delimiters
    char *Replacement; // replacement text; may itself contain tags
} Tag;
```

#### TagDictionary
```c
typedef struct {
    Tag *Tags; // pointer to array of Tag entries
    int  Count; // number of valid entries in Tags
} TagDictionary;
```

#### RecursionContext
```c
typedef struct {
    int    Depth;       // current recursion depth
    char **ActiveTags;  // dynamically-sized array of active tag names
    int    ActiveCount; // number of active entries
} RecursionContext;
```

#### DynamicBuffer
```c
typedef struct {
    char   *Data;     // heap buffer (NULL until reserved)
    size_t  Length;   // number of bytes currently used
    size_t  Capacity; // number of bytes allocated
} DynamicBuffer;
```

#### AllocPolicy
```c
typedef enum {
    ALLOC_NONE,
    ALLOC_MEMSET,
    ALLOC_CALLOC
} AllocPolicy;
```

### tagger_lib

#### `char *ProcessInput(const char *Input, RecursionContext *Ctx, TagDictionary *Dict);`
- Walks `Input`, expanding tags using `Dict` while tracking recursion in `Ctx`.
- Returns a heap buffer owned by the caller (free with `free`).
- Returns `NULL` if allocation fails or arguments are missing.

#### `char *ResolveTag(const char *TagName, RecursionContext *Ctx, TagDictionary *Dict);`
- Resolves a single tag by name, recursively expanding nested tags.
- Increments/decrements `Ctx->Depth` and updates the active-tag stack while expanding.
- Returns a newly allocated string on success; caller frees it.

#### `int IsTagActive(RecursionContext *Ctx, const char *TagName);`
- Checks if `TagName` is currently in the active-tag stack.
- Returns `1` when active, `0` when inactive, and `-1` on invalid arguments.

#### `void AddActiveTag(RecursionContext *Ctx, const char *TagName);`
- Pushes `TagName` into the active-tag stack, reallocating as needed.
- On allocation failure, logs an error and leaves the context unchanged.

#### `void RemoveActiveTag(RecursionContext *Ctx, const char *TagName);`
- Removes `TagName` from the active stack, shifting the remaining entries down.
- Frees the stored tag copy and shrinks the array; safe to call even if the tag is absent.

#### `char *LookupTag(TagDictionary *Dict, const char *TagName);`
- Scans the dictionary linearly and returns the stored replacement pointer.
- Returns `NULL` when the tag does not exist; the caller must not free the result.

### util_lib

#### Buffer helpers
- `void DynamicBufferInit(DynamicBuffer *Buf);` — zeroes a buffer struct so it can be reserved later.
- `int DynamicBufferReserve(DynamicBuffer *Buf, size_t Capacity);` — ensures `Buf` has at least `Capacity` bytes; resets `Length` to `0` on success.
- `void AppendToDynamicBuffer(DynamicBuffer *Buf, const char *Text);` — appends `Text`, doubling capacity until it fits.
- `void DynamicBufferFree(DynamicBuffer *Buf);` — releases the heap buffer and resets bookkeeping.

#### Allocation helpers
- `char *AllocateBuffer(size_t BufferSize, AllocPolicy Policy);` — wraps `malloc`/`calloc` with optional clearing; caller frees the result.

#### Parsing helpers
- `int Matches(const char *Input, int Pos, const char *Pattern);` — returns `1` when `Pattern` matches `Input` starting at `Pos`, `0` when it does not, `-1` on invalid parameters.
- `char *ExtractTagName(const char *Input, int *Pos, const char *TagStart, const char *TagEnd);` — copies the tag name between delimiters and advances `*Pos` past the closing delimiter.
- `char *ExtractPlainText(const char *Input, int *Pos, const char *TagStart);` — slices text until the next tag (or end of string) and advances `*Pos`.
- `int ContainsTag(const char *Text, const char *TagStart);` — declared but currently unimplemented; see `tests/missing_contains_tag_implementation.c`.

#### Diagnostics
- `void PrintProgressBar(double Fraction);` — renders a progress bar to stdout; expects `Fraction` between `0.0` and `1.0`.
- `void PrintError(const char *Message);` — convenience wrapper around `perror`.
- `void StandardError(const char *Format, ...);` — `stderr` printf-style helper using `va_list`.

## Repository Layout
- `main.c` – minimal end-to-end example using the library
- `benchmark.c` – synthetic workload for `AppendToDynamicBuffer`
- `tagger_lib.*` – core tag expansion logic and recursion guards
- `util_lib.*` – buffer utilities, error helpers, progress bar
- `benchmarks/` – CSV output from the benchmark runner
- `tests/` – focused repro programs (`make -C tests`, `make -C tests test-all`)

## Known Gaps & Repro Programs
The binaries in `tests/` capture issues that still need attention:
- `missing_contains_tag_implementation` highlights the stubbed-out `ContainsTag` helper.
- `active_tag_removal_shift_bug` shows that removing an active tag leaks/reshuffles the array incorrectly.
- `lost_pointer_when_realloc_fails` forces `realloc` failure to demonstrate the missing rollback path in `AddActiveTag`.
- `progress_bar_scaling_error` documents the progress bar’s incorrect percentage math when fed large fractions.

Run them from the repo root after building:
```bash
make -C tests
./tests/active_tag_removal_shift_bug
```

## Contributing
Issues and PRs that improve memory safety, finish unimplemented helpers, or add tests are welcome. Please keep the public API in `tagger_lib.h` backward compatible whenever possible.

## License
No license has been chosen yet. Until one is added, treat the code as "all rights reserved" and ask before reusing it.
