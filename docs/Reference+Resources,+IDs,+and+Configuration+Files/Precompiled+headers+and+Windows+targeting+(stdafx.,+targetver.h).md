# Reference: Resources, IDs, and Configuration Files – Precompiled Headers and Windows Targeting

## Overview

The spisocketswin32 project uses a classic Visual C++ precompiled header (PCH) setup to accelerate build times and centralize common includes. The two key files are **stdafx.h/stdafx.cpp**, which define and generate the PCH, and **targetver.h**, which controls the Windows SDK version the application targets. Developers updating toolchains or SDKs must understand how these files interact and where to adjust macros.

## stdafx.h

*Path: `stdafx.h`*

`stdafx.h` is the header designated for precompilation. It pulls in seldom-changed system and CRT headers so downstream `.cpp` files compile faster.

Contents summary:

- Includes `targetver.h` to establish the Windows platform version
- Defines `WIN32_LEAN_AND_MEAN` to trim rarely used Win32 APIs
- Includes `<windows.h>` and standard C headers

Key excerpt:

```cpp
#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN            // Exclude rarely-used stuff from Windows headers
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here
```

## stdafx.cpp

*Path: `stdafx.cpp`*

`stdafx.cpp` is the compilation unit that produces the PCH (`spisocketswin32.pch`) and the associated object file (`stdafx.obj`). It should include only `stdafx.h`.

Key excerpt:

```cpp
// stdafx.cpp : source file that includes just the standard includes
// spiwavwin32.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
```

### PCH Settings in the `.vcxproj`

The project file configures `stdafx.cpp` to **Create** the PCH in every configuration, and all other `.cpp` files to **Use** it:

- `<PrecompiledHeader>Create</PrecompiledHeader>` on `stdafx.cpp`
- `<PrecompiledHeader>Use</PrecompiledHeader>` on other compilation units

## targetver.h

*Path: `targetver.h`*

This header controls which Windows platform features are available by default. It includes **SDKDDKVer.h**, which sets the `_WIN32_WINNT` macro to the highest installed SDK value.

Key excerpt:

```cpp
#pragma once

// Including SDKDDKVer.h defines the highest available Windows platform.
// To target an earlier platform, include <WinSDKVer.h> and
// set the _WIN32_WINNT macro before including SDKDDKVer.h.

#include <SDKDDKVer.h>
```

## Updating Windows Target Version

To build against an earlier Windows version (e.g. Windows 7), modify **targetver.h**:

1. Add before `#include <SDKDDKVer.h>`:

```cpp
   #include <WinSDKVer.h>
   #define _WIN32_WINNT 0x0601   // Windows 7
```

1. Then include SDKDDKVer:

```cpp
   #include <SDKDDKVer.h>
```

This ensures the project only uses APIs available on the specified platform.

## Key Files Reference

| File | Responsibility |
| --- | --- |
| stdafx.h | Core PCH header: common system and CRT includes |
| stdafx.cpp | Generates PCH (`.pch`) and precompiled type information (`.obj`) |
| targetver.h | Defines Windows SDK targeting via SDKDDKVer.h (`_WIN32_WINNT`) |
