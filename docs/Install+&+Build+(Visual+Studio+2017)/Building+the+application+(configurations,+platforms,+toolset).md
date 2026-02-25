# Install & Build (Visual Studio 2017)

This section describes how to open, configure, and compile the spisocketswin32 application in Visual Studio 2017, targeting the v141 toolset and Windows 10 SDK 10.0.19041.0. You’ll learn how to select the appropriate build configurations (Debug/Release) and platforms (Win32/x64), review key project settings (toolset, SDK version, subsystem), and understand where the compiler and linker search for headers and libraries.

---

## Prerequisites

- **Visual Studio 2017** with C++ workload installed
- **Windows 10 SDK** version **10.0.19041.0**
- Built dependencies available on disk:
- SFML 2.5.1 (Network, System)
- FreeImage
- PortAudio
- winmm (Windows Multimedia library)
- spiwavsetlib

---

## Opening the Project

1. Launch **Visual Studio 2017**.
2. Choose **File → Open → Project/Solution…**.
3. Navigate to the repository root `oifii/spisocketswin32_vs2017` and open **spisocketswin32.vcxproj**.
4. The IDE will load four project configurations: Debug|Win32, Debug|x64, Release|Win32, and Release|x64 .

---

## Configurations & Platforms

In **Solution Explorer**, right-click the project and select **Properties**. Under **Configuration Manager**, you’ll see:

| Configuration | Platform | Purpose |
| --- | --- | --- |
| --------------: | :--------: | :-------------------------------------- |
| Debug | Win32 | 32-bit build with debug symbols |
| Debug | x64 | 64-bit build with debug symbols |
| Release | Win32 | 32-bit optimized build (no debug) |
| Release | x64 | 64-bit optimized build (no debug) |


These appear in the `<ItemGroup Label="ProjectConfigurations">` section of **spisocketswin32.vcxproj** .

---

## Platform Toolset & SDK Version

Within each Configuration’s **General** settings:

- **Platform Toolset**: `v141` (Visual Studio 2017 compiler)
- **Windows SDK Version**: `10.0.19041.0`

These are defined in the `<PropertyGroup>` blocks:

```xml
<PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'" Label="Configuration">
  <ConfigurationType>Application</ConfigurationType>
  <UseDebugLibraries>true</UseDebugLibraries>
  <CharacterSet>Unicode</CharacterSet>
  <PlatformToolset>v141</PlatformToolset>
</PropertyGroup>
```

---

## Include Directories

The project specifies where to find headers for its dependencies under **C/C++ → Additional Include Directories**:

- **Win32 (Debug & Release)**

`..\lib-src\portaudio\include;..\lib-src\freeimage_vs2017\Source\;..\spiwavsetlib;..\lib-src\sfml-2.5.1_vs2017\include`

- **x64 (Debug & Release)**

`..\lib-src\portaudio(x64)\include;..\lib-src\freeimage_vs2017(x64)\Source\;..\spiwavsetlib;..\lib-src\sfml-2.5.1(x64)_vs2017\include`

---

## Linker Settings

Under **Linker → System** and **Linker → Input**, verify:

- **SubSystem**: `Windows`
- **Generate Debug Information**: `true` for all configurations
- **Additional Dependencies**:

```plaintext
  sfml-network.lib; sfml-system.lib;
  FreeImage.lib;
  winmm.lib;
  spiwavsetlib_vs2017.lib;
  portaudio_x86.lib (Win32) or portaudio_x64.lib (x64);
  %(AdditionalDependencies)
```

An example for **Debug|Win32**:

```xml
<AdditionalDependencies>
  ..\lib-src\sfml-2.5.1_vs2017\lib\sfml-network.lib;
  ..\lib-src\sfml-2.5.1_vs2017\lib\sfml-system.lib;
  ..\lib-src\freeimage_vs2017\Dist\x32\FreeImage.lib;
  winmm.lib;
  ..\spiwavsetlib_vs2017u\debug\spiwavsetlib_vs2017.lib;
  ..\lib-src\portaudio\build\msvc\Win32\Debug\portaudio_x86.lib;
  %(AdditionalDependencies)
</AdditionalDependencies>
```

---

## Build Steps

1. In the toolbar, select the desired **Solution Configuration** (Debug or Release) and **Solution Platform** (Win32 or x64).
2. Choose **Build → Build Solution** (or press **F7**).
3. The compiler will emit `.obj` files in `$(ProjectDir)\<Configuration>\<Platform>\` and link them into:

- `Debug\Win32\spisocketswin32.exe`
- `Debug\x64\spisocketswin32.exe`
- `Release\Win32\spisocketswin32.exe`
- `Release\x64\spisocketswin32.exe`

1. Debug builds include `.pdb` symbol files. Release builds enable whole-program optimizations (`<WholeProgramOptimization>true</WholeProgramOptimization>`) and disable debug libraries (`<UseDebugLibraries>false</UseDebugLibraries>`) .

---

With these settings in place, you can successfully compile and run **spisocketswin32** under any supported configuration.