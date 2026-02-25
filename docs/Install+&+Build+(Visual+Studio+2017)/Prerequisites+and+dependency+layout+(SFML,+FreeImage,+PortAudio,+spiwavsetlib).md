# Install & Build (Visual Studio 2017)

This section describes how to prepare your development environment and lay out third-party dependencies so that **spisocketswin32** (Visual Studio 2017 / v141) will compile and link correctly for both Win32 and x64 targets in Debug and Release configurations.

---

## Prerequisites

- **Visual Studio 2017** with the “Desktop development with C++” workload installed.
- **Windows SDK** (installed via VS installer).
- C++17 (or later) toolset (v141) selected in project properties.
- **git** (to clone the repository) or ZIP download of `oifii/spisocketswin32_vs2017`.
- Windows system library: **winmm.lib** (part of Windows SDK).

---

## Directory Layout

The repository expects the following relative structure for third-party libraries:

```plaintext
<solution root>
│   spisocketswin32.vcxproj
│
├── lib-src
│   ├── sfml-2.5.1_vs2017
│   │   ├── include
│   │   └── lib
│   ├── sfml-2.5.1(x64)_vs2017
│   │   ├── include
│   │   └── lib
│   ├── freeimage_vs2017
│   │   └── Source
│   ├── freeimage3180_vs2017
│   │   └── Dist
│   │       ├── x32
│   │       └── x64
│   └── portaudio
│       └── include
│       └── build
│           └── msvc
│               ├── Win32
│               │   ├── Debug
│               │   └── Release
│               └── x64
│                   ├── Debug
│                   └── Release
│
└── spiwavsetlib_vs2017u
    ├── debug
    │   └── spiwavsetlib_vs2017.lib
    └── release
        └── spiwavsetlib_vs2017.lib
```

- **sfml-2.5.1_vs2017** and **sfml-2.5.1(x64)_vs2017**: SFML 2.5.1 built for VS2017, Win32 and x64.
- **freeimage_vs2017**: FreeImage 3.18.0 source tree; Dist directories hold the compiled `.lib`.
- **portaudio**: PortAudio headers plus prebuilt `.lib` for Win32/x64, Debug/Release under `build\msvc\…`.
- **spiwavsetlib_vs2017u**: Prebuilt audio-playback library used by the project.
- **winmm.lib**: System-supplied multimedia library (no additional folder needed).

---

## Include Directories

Based on the `<AdditionalIncludeDirectories>` entries in **spisocketswin32.vcxproj**, the compiler will look in:

|  | Configuration | Include Paths (relative to project) |
| --- | --- | --- |
| Debug | Win32 | `..\lib-src\portaudio\include`<br/>`..\lib-src\freeimage_vs2017\Source`<br/>`..\spiwavsetlib`<br/>`..\lib-src\sfml-2.5.1_vs2017\include` |
| Debug | x64 | `..\lib-src\portaudio(x64)\include`<br/>`..\lib-src\freeimage_vs2017(x64)\Source`<br/>`..\spiwavsetlib`<br/>`..\lib-src\sfml-2.5.1(x64)_vs2017\include` |
| Release | Win32 | `..\lib-src\portaudio\include`<br/>`..\lib-src\freeimage_vs2017\Source`<br/>`..\spiwavsetlib`<br/>`..\lib-src\sfml-2.5.1_vs2017\include` |
| Release | x64 | `..\lib-src\portaudio(x64)\include`<br/>`..\lib-src\freeimage_vs2017(x64)\Source`<br/>`..\spiwavsetlib`<br/>`..\lib-src\sfml-2.5.1(x64)_vs2017\include` |


---

## Linker Dependencies

The `<AdditionalDependencies>` property lists all `.lib` files that must be available at link time:

### Debug | Win32

- `..\lib-src\sfml-2.5.1_vs2017\lib\sfml-network.lib`
- `..\lib-src\sfml-2.5.1_vs2017\lib\sfml-system.lib`
- `..\lib-src\freeimage_vs2017\Dist\x32\FreeImage.lib`
- `winmm.lib`
- `..\spiwavsetlib_vs2017u\debug\spiwavsetlib_vs2017.lib`
- `..\lib-src\portaudio\build\msvc\Win32\Debug\portaudio_x86.lib`

### Debug | x64

- `..\lib-src\sfml-2.5.1(x64)_vs2017\lib\sfml-network.lib`
- `..\lib-src\sfml-2.5.1(x64)_vs2017\lib\sfml-system.lib`
- `..\lib-src\freeimage3180_vs2017\Dist\x64\FreeImage.lib`
- `winmm.lib`
- `..\spiwavsetlib_vs2017u\x64\debug\spiwavsetlib_vs2017.lib`
- `..\lib-src\portaudio(x64)\build\msvc\x64\Debug\portaudio_x64.lib`

### Release | Win32

- `..\lib-src\sfml-2.5.1_vs2017\lib\sfml-network.lib`
- `..\lib-src\sfml-2.5.1_vs2017\lib\sfml-system.lib`
- `..\lib-src\freeimage_vs2017\Dist\x32\FreeImage.lib`
- `winmm.lib`
- `..\spiwavsetlib_vs2017u\release\spiwavsetlib_vs2017.lib`
- `..\lib-src\portaudio\build\msvc\Win32\Release\portaudio_x86.lib`

### Release | x64

- `..\lib-src\sfml-2.5.1(x64)_vs2017\lib\sfml-network.lib`
- `..\lib-src\sfml-2.5.1(x64)_vs2017\lib\sfml-system.lib`
- `..\lib-src\freeimage3180_vs2017\Dist\x64\FreeImage.lib`
- `winmm.lib`
- `..\spiwavsetlib_vs2017u\x64\release\spiwavsetlib_vs2017.lib`
- `..\lib-src\portaudio(x64)\build\msvc\x64\Release\portaudio_x64.lib`

---

## Third-Party Library Versions

- **SFML 2.5.1**
- Include: `lib-src/sfml-2.5.1_vs2017/include` (Win32) and `lib-src/sfml-2.5.1(x64)_vs2017/include` (x64)
- Lib: `.../lib/sfml-network.lib`, `.../lib/sfml-system.lib`

- **FreeImage 3.18.0**
- Source: `lib-src/freeimage_vs2017/Source` (Win32) and `lib-src/freeimage_vs2017(x64)/Source` (x64)
- Dist: `.lib` under `Dist\x32` or `Dist\x64`

- **PortAudio**
- Headers: `lib-src/portaudio/include` (Win32) and `lib-src/portaudio(x64)/include` (x64)
- Libs: prebuilt under `build\msvc\Win32\{Debug,Release}` and `build\msvc\x64\{Debug,Release}`

- **spiwavsetlib_vs2017u**
- Debug lib: `spiwavsetlib_vs2017u\debug\spiwavsetlib_vs2017.lib`
- Release lib: `spiwavsetlib_vs2017u\release\spiwavsetlib_vs2017.lib`
- x64 variants under `spiwavsetlib_vs2017u\x64\{debug,release}`

- **winmm.lib**
- System multimedia library; no additional installation required beyond Windows SDK.

---

## Summary

1. Clone or extract the repository so that `spisocketswin32.vcxproj` sits at the solution root.
2. Ensure the `lib-src` and `spiwavsetlib_vs2017u` folders mirror the layout above.
3. Install Visual Studio 2017 with C++ workloads and the Windows SDK.
4. Open **spisocketswin32.vcxproj**, select the desired Configuration (Debug/Release) and Platform (Win32/x64), then Build.