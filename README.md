# Recursive Tag Replacement Engine

A lightweight C library for processing templated strings with tags (e.g. `{{name}}`) and replacing them with user-defined values.

## Table of Contents
1. Installation(#installation)
2. Usage(#usage)
3. API Reference(#api-reference)
   1. Constants(#constants)
   2. Types(#types)
   3. tagger_lib(#tagger_lib)
   4. util_lib(#util_lib)
4. Examples(#examples)
5. License(#license)

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

## Usage

I have found much use for the library in enabling automation when writing long test files with dense and repetitive natual language like syntax, especially where using these sorts of files is not optional and tedious.

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

#### AllocateBuffer
```c
char* AllocateBuffer(
    size_t BufferSize
);
```
- **Description**: Allocate and initialize a buffer of at least `BufferSize` bytes
- **Parameters**:
  - `BufferSize` (`size_t`)
- **Returns**: (`char*`) Pointer to zero-initialized buffer; caller must free

#### Matches
```c
int Matches(
    const char* Input,
    int Pos,
    const char* Pattern
);
```
- **Description**: Check if `Pattern` matches `Input` starting at `Pos`
- **Parameters**:
  - `Input` (`const char*`)
  - `Pos` (`int`)
  - `Pattern` (`const char*`)
- **Returns**: (`int`) Non-zero if matches, zero otherwise

#### ExtractTagName
```c
char* ExtractTagName(
    const char* Input,
    int* Pos,
    const char* TagStart,
    const char* TagEnd
);
```
- **Description**: Extract the tag name between `TagStart` and `TagEnd` from `Input`; updates `Pos`
- **Parameters**:
  - `Input` (`const char*`)
  - `Pos` (`int*`)
  - `TagStart` (`const char*`)
  - `TagEnd` (`const char*`)
- **Returns**: (`char*`) Extracted tag name; caller must free

#### ExtractPlainText
```c
char* ExtractPlainText(
    const char* Input,
    int* Pos,
    const char* TagStart
);
```
- **Description**: Extract text from `Input` until `TagStart` is encountered; updates `Pos`
- **Parameters**:
  - `Input` (`const char*`)
  - `Pos` (`int*`)
  - `TagStart` (`const char*`)
- **Returns**: (`char*`) Extracted text; caller must free

#### ContainsTag
```c
int ContainsTag(
    const char* Text,
    const char* TagStart
);
```
- **Description**: Determine if `Text` contains the substring `TagStart`
- **Parameters**:
  - `Text` (`const char*`)
  - `TagStart` (`const char*`)
- **Returns**: (`int`) Non-zero if found, zero otherwise

#### AppendToBuffer
```c
void AppendToBuffer(
    char** Buffer,
    const char* Text
);
```
- **Description**: Append `Text` to the end of `*Buffer`, reallocating if necessary
- **Parameters**:
  - `Buffer` (`char**`): Pointer to buffer pointer
  - `Text` (`const char#### Error
```c
void Error(
    const char* Message
);
```
- **Description**: Print or handle an error with the given `Message` and exit if necessary
- **Parameters**:
  - `Message` (`const char*`)

## Examples

Yet to be added, only current example that can be shared is written in main.c

## License

Not sure how these work yet.
