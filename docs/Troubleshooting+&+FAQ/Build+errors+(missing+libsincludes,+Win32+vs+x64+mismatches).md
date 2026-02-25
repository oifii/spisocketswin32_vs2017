# Troubleshooting & FAQ – Build Errors (missing libs/includes, Win32 vs x64 mismatches)

## Overview

When building spisocketswin32 in Visual Studio 2017 (v141), common failures arise from incorrect include/library paths or selecting the wrong platform configuration. This guide helps you diagnose and resolve:

- “Cannot open include file” errors for SFML, FreeImage, PortAudio or spiwavsetlib.
- Linker errors (LNK1104) for missing `.lib` files.
- “machine type” mismatches when mixing Win32 and x64 binaries.
- Misaligned relative directory structure expected by the `.vcxproj`.

Follow these steps to verify your project settings, correct relative paths, and align platform configurations.

---

## 1. Missing Headers / Include Files

### Symptoms

- error C1083: Cannot open include file: ‘SFML/Network.hpp’: No such file or directory
- error C1083: Cannot open include file: ‘FreeImage.h’: No such file or directory

### Causes

- The Additional Include Directories in your project configuration don’t point to the library headers.
- You’re building Debug|x64 but your paths reference `lib-src\sfml-2.5.1_vs2017`, which is x86 only.

### Verification & Fix

1. Open **Project → Properties → C/C++ → General → Additional Include Directories**.
2. Ensure the configured include paths exactly match those in **spisocketswin32.vcxproj** under each configuration:

|  | Configuration | Include Directories |
| --- | --- | --- |
| Debug | Win32 | `..\lib-src\portaudio\include;..\lib-src\freeimage_vs2017\Source\;..\spiwavsetlib;..\lib-src\sfml-2.5.1_vs2017\include` |
| Debug | x64 | `..\lib-src\portaudio(x64)\include;..\lib-src\freeimage_vs2017(x64)\Source\;..\spiwavsetlib;..\lib-src\sfml-2.5.1(x64)_vs2017\include` |
| Release | Win32 | `..\lib-src\portaudio\include;..\lib-src\freeimage_vs2017\Source\;..\spiwavsetlib;..\lib-src\sfml-2.5.1_vs2017\include` |
| Release | x64 | `..\lib-src\portaudio(x64)\include;..\lib-src\freeimage_vs2017(x64)\Source\;..\spiwavsetlib;..\lib-src\sfml-2.5.1(x64)_vs2017\include` |


1. Adjust paths if your local folder layout differs. Paths are relative to the project directory.

```card
{
    "title": "Tip",
    "content": "Use VS's \u201cEdit\u2026\u200b\u201d button next to Additional Include Directories to browse and verify each path."
}
```

---

## 2. Missing Library Dependencies

### Symptoms

- error LNK1104: cannot open file ‘sfml-network.lib’
- LNK2019: unresolved external symbol _WavSetLib_Initialize referenced in …

### Causes

- Additional Dependencies list does not include the correct `.lib` files for the current platform/configuration.
- Linking against x86 libs when building x64 (or vice versa).

### Verification & Fix

1. Open **Project → Properties → Linker → Input → Additional Dependencies**.
2. Confirm entries match **spisocketswin32.vcxproj**:

|  | Configuration | Library Dependencies |
| --- | --- | --- |
| Debug | Win32 | `..\lib-src\sfml-2.5.1_vs2017\lib\sfml-network.lib;..\lib-src\sfml-2.5.1_vs2017\lib\sfml-system.lib;..\lib-src\freeimage_vs2017\Dist\x32\FreeImage.lib;winmm.lib;..\spiwavsetlib_vs2017u\debug\spiwavsetlib_vs2017.lib;..\lib-src\portaudio\build\msvc\Win32\Debug\portaudio_x86.lib` |
| Debug | x64 | `..\lib-src\sfml-2.5.1(x64)_vs2017\lib\sfml-network.lib;..\lib-src\sfml-2.5.1(x64)_vs2017\lib\sfml-system.lib;..\lib-src\freeimage3180_vs2017\Dist\x64\FreeImage.lib;winmm.lib;..\spiwavsetlib_vs2017u\x64\debug\spiwavsetlib_vs2017.lib;..\lib-src\portaudio(x64)\build\msvc\x64\Debug\portaudio_x64.lib` |
| Release | Win32 | `..\lib-src\sfml-2.5.1_vs2017\lib\sfml-network.lib;..\lib-src\sfml-2.5.1_vs2017\lib\sfml-system.lib;..\lib-src\freeimage_vs2017\Dist\x32\FreeImage.lib;winmm.lib;..\spiwavsetlib_vs2017u\release\spiwavsetlib_vs2017.lib;..\lib-src\portaudio\build\msvc\Win32\Release\portaudio_x86.lib` |
| Release | x64 | `..\lib-src\sfml-2.5.1(x64)_vs2017\lib\sfml-network.lib;..\lib-src\sfml-2.5.1(x64)_vs2017\lib\sfml-system.lib;..\lib-src\freeimage3180_vs2017\Dist\x64\FreeImage.lib;winmm.lib;..\spiwavsetlib_vs2017u\x64\release\spiwavsetlib_vs2017.lib;..\lib-src\portaudio(x64)\build\msvc\x64\Release\portaudio_x64.lib` |


1. If a `.lib` file is missing on disk, ensure you have built or downloaded:
2. SFML (network, system) libraries
3. FreeImage for x86/x64
4. PortAudio x86/x64
5. spiwavsetlib (debug/release, Win32/x64)

---

## 3. Win32 vs x64 Platform Mismatch

### Symptoms

- LNK1112: module machine type ‘X86’ conflicts with target machine type ‘x64’
- Unresolved externals referencing functions in libraries built for the opposite architecture.

### Cause

- Building under the wrong **Platform** (e.g. **x64**) while your `lib-src` folder contains only Win32 binaries, or vice versa.

### Resolution

1. In Visual Studio toolbar, verify **Solution Platforms** matches the architecture of your dependencies (Win32 or x64).
2. If needed, switch to **Win32** or **x64** and rebuild.
3. Confirm that the `.vcxproj` `<ItemDefinitionGroup>` for your chosen platform points to the correct subfolders under `lib-src`.

---

## 4. Expected Directory Structure

Ensure your checkout has the following relative layout under the project root:

```plaintext
spisocketswin32_vs2017
├─ lib-src
│  ├─ sfml-2.5.1_vs2017
│  │  ├─ include
│  │  └─ lib
│  ├─ sfml-2.5.1(x64)_vs2017
│  │  ├─ include
│  │  └─ lib
│  ├─ freeimage_vs2017
│  │  ├─ Source
│  │  └─ Dist\x32
│  ├─ freeimage3180_vs2017
│  │  └─ Dist\x64
│  ├─ portaudio
│  │  └─ build\msvc\Win32\Debug|Release
│  ├─ portaudio(x64)
│  │  └─ build\msvc\x64\Debug|Release
│  └─ spiwavsetlib_vs2017u
│     ├─ debug
│     ├─ release
│     ├─ x64\debug
│     └─ x64\release
└─ spisocketswin32.vcxproj
```

Mismatch in folder names or missing subfolders will cause build failures.

---

## FAQ

**Q:** “Why does FreeImage.h still fail after I added the include path?”

**A:** Double-check for trailing semicolons or stray spaces in Additional Include Directories. The path must exactly match `..\lib-src\freeimage_vs2017\Source\` (for Win32) or `..\lib-src\freeimage_vs2017(x64)\Source\` (for x64) .

**Q:** “I have SFML built for x64 but VS still links x86 .lib files.”

**A:** Confirm the **Platform** in the VS toolbar is set to **x64**, and rebuild the solution cleanly.

**Q:** “PortAudio symbols unresolved.”

**A:** Ensure you’ve built PortAudio with the same runtime library setting (Debug vs Release) and architecture, and that `..\lib-src\portaudio\build\msvc\…\portaudio_x86.lib` (or x64) is present and referenced .

---

## Key Points

- Always match **Configuration** + **Platform** in VS with the corresponding include/lib paths.
- Maintain the directory tree under **lib-src** exactly as expected.
- When in doubt, clean the solution, delete `Debug`/`Release` outputs, and rebuild after verifying paths.

With these checks, most build errors related to missing libraries, includes, or platform mismatches can be quickly resolved.