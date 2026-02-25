## Internals (for advanced users) – Key helper functions

This section describes two low-level utility routines provided in **spisocketswin32.cpp** that you may extend or replace when customizing argument parsing or string encoding in the application.

### CommandLineToArgvA

*File: spisocketswin32.cpp*

Parses an ANSI command-line string into an argv-style array of C-strings, handling quoting and whitespace. This custom implementation mirrors Windows’ `CommandLineToArgvW`, but operates on narrow (ANSI) strings.

```cpp
PCHAR* CommandLineToArgvA(
    PCHAR CmdLine,
    int* _argc
);
```

Purpose

- Split a single `CmdLine` buffer into individual arguments.
- Support quoted substrings (text within `"`).
- Return both an array of pointers (`argv`) and the argument count (`*_argc`).

Behavior

1. Compute total length of `CmdLine`.
2. Allocate one block of memory via `GlobalAlloc`, sized to hold both:
3. An array of `PCHAR` pointers.
4. A contiguous character buffer for argument text.
5. Initialize parsing state flags:
6. `in_QM` – inside a double-quoted token
7. `in_TEXT` – currently accumulating non-whitespace characters
8. `in_SPACE` – the previous character was whitespace
9. Iterate each character `a` in `CmdLine`:
10. If in a quoted segment (`in_QM`), append all characters except closing `"` to the current token.
11. If not in a quote:
12. On encountering `"`, begin a quoted segment and, if starting a new token, record its pointer.
13. On whitespace (`' '`, `'\t'`, `'\n'`, `'\r'`), terminate the current token (if one was open).
14. Otherwise, start or continue a token, copying the character.
15. After the loop, terminate the final token, append a NULL sentinel in `argv`, and set `*_argc` to the number of arguments.

Memory Management

- The returned `PCHAR*` and its internal buffers must be freed by calling `LocalFree` on the pointer returned by `CommandLineToArgvA` .

---

### UTF-8 Encoding/Decoding Helpers

*File: spisocketswin32.cpp*

Two inline routines to convert between wide (UTF-16) strings and UTF-8 encoded `std::string`, using the Windows API functions `WideCharToMultiByte` and `MultiByteToWideChar`.

#### std::string utf8_encode(const std::wstring &wstr)

Converts a UTF-16 (`std::wstring`) string to a UTF-8 (`std::string`) representation.

```cpp
std::string utf8_encode(const std::wstring &wstr);
```

| Parameter | Description |
| --- | --- |
| wstr | Input wide‐character (UTF-16) string. |


| Returns | UTF-8 encoded string. |
| --- | --- |


Implementation Steps

1. Call `WideCharToMultiByte` with `CP_UTF8` and `NULL` output buffer to determine `size_needed` (in bytes).
2. Construct an output `std::string strTo` of length `size_needed`.
3. Call `WideCharToMultiByte` again to fill `strTo`’s buffer with the UTF-8 bytes.
4. Return `strTo` .

#### std::wstring utf8_decode(const std::string &str)

Converts a UTF-8 (`std::string`) string to a wide (UTF-16) `std::wstring`.

```cpp
std::wstring utf8_decode(const std::string &str);
```

| Parameter | Description |
| --- | --- |
| str | Input UTF-8 encoded string. |


| Returns | UTF-16 (wide) string. |
| --- | --- |


Implementation Steps

1. Call `MultiByteToWideChar` with `CP_UTF8` and `NULL` output buffer to determine `size_needed` (in wide characters).
2. Construct an output `std::wstring wstrTo` of length `size_needed`.
3. Call `MultiByteToWideChar` again to fill `wstrTo`’s buffer with wide characters.
4. Return `wstrTo` .

---

#### Customization Notes

- **CommandLineToArgvA** can be adapted to support alternate quoting rules or escape sequences by modifying the state machine in its parsing loop.
- **utf8_encode/utf8_decode** rely on the system code page for UTF-8; to target other code pages, pass a different code page constant to `WideCharToMultiByte`/`MultiByteToWideChar`.
- Both utilities are defined in the main module to avoid external dependencies; you may extract them into a shared utility library if reusing across multiple projects.