# Install & Build (Visual Studio 2017) – Project File Map

This section describes which files participate in the build of **spisocketswin32** under Visual Studio 2017 (v141 toolset), and how they’re organized between compiled sources, headers, resources, and non-build items.

---

## spisocketswin32.vcxproj

### Header Files (ClInclude)

These headers are compiled into the project and exposed to the compiler.

| File | Build Item |
| --- | --- |
| `Resource.h` | ClInclude |
| `spisocketswin32.h` | ClInclude |
| `stdafx.h` | ClInclude |
| `targetver.h` | ClInclude |


### Source Files (ClCompile)

These `.cpp` files produce object code linked into the final executable.

| File | Build Item |
| --- | --- |
| `spisocketswin32.cpp` | ClCompile |
| `stdafx.cpp` | ClCompile |


### Resource Script (ResourceCompile)

The Windows resource compiler embeds icons, dialogs, string tables, etc.

| File | Build Item |
| --- | --- |
| `spisocketswin32.rc` | ResourceCompile |


### Non-Build Items (None)

Auxiliary files included in the project tree but not compiled or linked.

| File | Build Item |
| --- | --- |
| `ReadMe.txt` | None |
| `small.ico` | None |
| `spi.licenseheader` | None |
| `spisocketswin32.ico` | None |


---

## spisocketswin32.vcxproj.filters

Visual Studio uses the `.filters` file to group files in the IDE. The following shows how files are assigned to filters.

### Filter Definitions

- **Source Files**: `*.cpp;*.c;*.bat;*.asm;…`
- **Header Files**: `*.h;*.hpp;*.inl;…`
- **Resource Files**: `*.rc;*.ico;*.png;*.wav;…`

### File-to-Filter Assignments

| Filter | File | Citation |
| --- | --- | --- |
| Source Files | `stdafx.cpp` |  |
| Source Files | `spisocketswin32.cpp` |  |
| Header Files | `stdafx.h` |  |
| Header Files | `targetver.h` |  |
| Header Files | `Resource.h` |  |
| Header Files | `spisocketswin32.h` |  |
| Resource Files | `spisocketswin32.rc` |  |
| Resource Files | `small.ico` |  |
| Resource Files | `spisocketswin32.ico` |  |
| (None) | `ReadMe.txt` | – |
| (None) | `spi.licenseheader` | – |


---

## Legacy / Sample Sources

The repository also contains standalone example files—**TCP.cpp**, **UDP.cpp**, and **Sockets.cpp**—but these are *not* referenced in any `<ClCompile>` or `<ClInclude>` ItemGroup in **spisocketswin32.vcxproj**, and so are *not* built into the spisocketswin32 application. They appear to be legacy or import samples.