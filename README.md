# Recursive Tag Replacement Engine

A lightweight C library for processing templated strings with tags (e.g. `{{name}}`) and replacing them with user-defined values.

## Table of Contents
1. [Installation](#installation)
2. [Usage](#usage)
3. [API Reference](#api-reference)
   - [Constants](#constants)
   - [Types](#types)
   - [tagger_lib](#tagger_lib)
   - [util_lib](#util_lib)
4. [Examples](#examples)
5. [License](#license)

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/qasimkamran/Recursive-Tag-Replacement-Engine.git
   cd Recursive-Tag-Replacement-Engine
   ```
2. Build the library and executables:
   ```bash
   make
   ```

   The build includes debug symbols by default (using `-g` in `CFLAGS`). To build without debug symbols, edit the `CFLAGS` variable in the `Makefile`.

## Usage

After building, the executables live in the `bin/` directory, and convenience symlinks are created at the repository root so you can run them directly:

```bash
./main
# Output: Hello, Qasim! Welcome to example.com.

./benchmark
# Displays a progress bar and writes timing data to benchmarks/BENCHMARK_AppendToBuffer.csv
```

## API Reference

### Constants
- `START_TAG` (`char*`): Start tag delimiter (default: `"{{"`).
- `END_TAG` (`char*`): End tag delimiter (default: `"}}"`).
- `MAX_RECURSION_DEPTH` (`int`): Maximum nested tag resolution depth (default: `3`).
- `BUFFER_SIZE` (`int`): Default buffer size for utility operations (default: `32`).

### Types

#### Tag
```c
typedef struct {
    char* Name;         // The name of the tag (without delimiters)
    char* Replacement;  // The replacement text for this tag
} Tag;
```

#### TagDictionary
```c
typedef struct {
    Tag* Tags;   // Array of Tag entries
    int  Count;  // Number of entries in the dictionary
} TagDictionary;
```

#### RecursionContext
```c
typedef struct {
    int    Depth;        // Current recursion depth
    char** ActiveTags;   // Array of active tag names
    int    ActiveCount;  // Number of active tags
} RecursionContext;
```

### tagger_lib

#### ProcessInput
```c
char* ProcessInput(
    const char* Input,
    RecursionContext* Ctx,
    TagDictionary* Dict
);
```
- **Description**: Process the input string, replacing tags using `Dict` and tracking nested resolution with `Ctx`
- **Parameters**:
  - `Input` (`const char*`): Input string containing tags
  - `Ctx` (`RecursionContext*`): Initialized recursion context
  - `Dict` (`TagDictionary*`): Dictionary of tags and their replacements
- **Returns**: (`char*`) Newly allocated result string; caller must free
- **Example**:
  ```c
  RecursionContext ctx = {0, NULL, 0};
  TagDictionary dict = {/* initialize tags */};
  char* result = ProcessInput("Hello, {{name}}!", &ctx, &dict);
  // ... use result ...
  free(result);
  ```

#### ResolveTag
```c
char* ResolveTag(
    const char* TagName,
    RecursionContext* Ctx,
    TagDictionary* Dict
);
```
- **Description**: Resolve a single tag by name, handling recursion and lookup
- **Parameters**:
  - `TagName` (`const char*`): Name of the tag without delimiters
  - `Ctx` (`RecursionContext*`): Current recursion context
  - `Dict` (`TagDictionary*`): Dictionary of tag mappings
- **Returns**: (`char*`) Allocated replacement string; caller must free
- **Example**:
  ```c
  char* val = ResolveTag("site", &ctx, &dict);
  // ...
  free(val);
  ```

#### IsTagActive
```c
int IsTagActive(
    RecursionContext* Ctx,
    const char* TagName
);
```
- **Description**: Check if `TagName` is currently active in `Ctx`
- **Parameters**:
  - `Ctx` (`RecursionContext*`): Recursion context
  - `TagName` (`const char*`): Tag name to check
- **Returns**: (`int`) Non-zero if active, zero otherwise

#### AddActiveTag
```c
void AddActiveTag(
    RecursionContext* Ctx,
    const char* TagName
);
```
- **Description**: Mark `TagName` as active in `Ctx`
- **Parameters**:
  - `Ctx` (`RecursionContext*`)
  - `TagName` (`const char*`)

#### RemoveActiveTag
```c
void RemoveActiveTag(
    RecursionContext* Ctx,
    const char* TagName
);
```
- **Description**: Remove `TagName` from the active list in `Ctx`
- **Parameters**:
  - `Ctx` (`RecursionContext*`)
  - `TagName` (`const char*`)

#### LookupTag
```c
char* LookupTag(
    TagDictionary* Dict,
    const char* TagName
);
```
- **Description**: Lookup `TagName` in `Dict` and return its replacement (newly allocated)
- **Parameters**:
  - `Dict` (`TagDictionary*`)
  - `TagName` (`const char*`)
- **Returns**: (`char*`) Replacement text or `NULL` if not found

### util_lib

#### Types
```c
typedef struct {
    char *Data;      // Pointer to buffer data
    size_t Length;   // Current length of data
    size_t Capacity; // Allocated capacity
} DynamicBuffer;

typedef enum {
    ALLOC_NONE,     // No initialization
    ALLOC_MEMSET,   // Zero-initialize via memset
    ALLOC_CALLOC    // Allocate and clear memory
} AllocPolicy;
```

#### Constants
- `BUFFER_SIZE` (`int`): Default buffer size for utility operations (default: `32`).
- `PROGRESS_BAR_WIDTH` (`int`): Width of the progress bar for `PrintProgressBar` (default: `50`).

#### Functions
- `void PrintProgressBar(double Fraction);`
  - Description: Render a progress bar to stdout for the given fraction (0.0–1.0).

- `void PrintError(const char* Message);`
  - Description: Print an error message using `perror`.

- `void StandardError(const char* Format, ...);`
  - Description: Print a formatted error message to `stderr`.

- `char* AllocateBuffer(size_t BufferSize, AllocPolicy Policy);`
  - Description: Allocate a buffer with the specified size and initialization policy.
  - Parameters:
    - `BufferSize` (`size_t`): Minimum size of the buffer in bytes.
    - `Policy` (`AllocPolicy`): Allocation policy (`ALLOC_NONE`, `ALLOC_MEMSET`, or `ALLOC_CALLOC`).
  - Returns: Pointer to the allocated buffer or `NULL` on failure (caller must free).

- `void DynamicBufferInit(DynamicBuffer* Buf);`
  - Description: Initialize an empty `DynamicBuffer`.

- `void DynamicBufferFree(DynamicBuffer* Buf);`
  - Description: Release resources held by `DynamicBuffer`.

- `int DynamicBufferReserve(DynamicBuffer* Buf, size_t Capacity);`
  - Description: Ensure `Buf` has at least `Capacity` bytes allocated; returns non-zero on success.

- `void AppendToDynamicBuffer(DynamicBuffer* Buf, const char* Text);`
  - Description: Append `Text` to `Buf`, growing capacity exponentially as needed.

- `int Matches(const char* Input, int Pos, const char* Pattern);`
  - Description: Check if `Pattern` matches `Input` starting at `Pos`.

- `char* ExtractTagName(const char* Input, int* Pos, const char* TagStart, const char* TagEnd);`
  - Description: Extract the tag name between `TagStart` and `TagEnd` from `Input`; updates `Pos`.

- `char* ExtractPlainText(const char* Input, int* Pos, const char* TagStart);`
  - Description: Extract text from `Input` until `TagStart` is encountered; updates `Pos`.

- `int ContainsTag(const char* Text, const char* TagStart);`
  - Description: Determine if `Text` contains the substring `TagStart`.

## Examples

### Basic Example

After building, run the main example program:

```bash
./main
# Output: Hello, Qasim! Welcome to example.com.
```

### Benchmark

Run the performance benchmark to measure buffer append times:

```bash
./benchmark
# Displays a progress bar and writes timing data to benchmarks/BENCHMARK_AppendToBuffer.csv
```

## License

Not sure how licenses work yet.
