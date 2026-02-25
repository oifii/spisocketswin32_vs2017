# Getting Started: Running spisocketswin32 (Server & Client)

This section describes how to launch **spisocketswin32** in either **server** or **client** mode, detailing every command-line argument and its default value as defined in the application’s source (`spisocketswin32.cpp`).

## Usage

```text
spisocketswin32.exe
  [mode]
  [ipaddress]
  [message]
  [x]
  [y]
  [width]
  [height]
  [alpha]
  [titleBar]
  [menuBar]
  [accelerator]
  [fontHeight]
  [textRed]
  [textGreen]
  [textBlue]
  [windowClass]
  [windowTitle]
  [beginScript]
  [endScript]
```

- All arguments are positional.
- Omit trailing arguments to use defaults.
- Port is fixed at **50001** (UDP).

## Argument Reference

The table below maps each positional argument to its internal variable, default value, and purpose. Arguments are parsed in `_tWinMain` (see argument-parsing block) ; defaults come from the global variables section .

| Position | Argument | Variable | Default | Description |
| --- | --- | --- | --- | --- |
| 1 | mode | `global_servername` | `"server"` | `"server"` to listen/display; `"client"` to send a single UDP message. |
| 2 | ipaddress | `global_ipaddress` | `""` | Destination IPv4 (client only). |
| 3 | message | `global_message` | `"done"` | Text to send (client); in theory, response text for server (unused). |
| 4 | x | `global_x` | `100` | Initial X coordinate of the window. |
| 5 | y | `global_y` | `200` | Initial Y coordinate of the window. |
| 6 | width | `global_xwidth` | `400` | Window width in pixels. |
| 7 | height | `global_yheight` | `400` | Window height in pixels. |
| 8 | alpha | `global_alpha` | `200` | Window opacity (0 = transparent … 255 = opaque). |
| 9 | titleBar | `global_titlebardisplay` | `1` | Show title bar (1) or hide (0). |
| 10 | menuBar | `global_menubardisplay` | `0` | Show menu bar (1) or hide (0). |
| 11 | accelerator | `global_acceleratoractive` | `0` | Enable accelerator keys (1) or disable (0). |
| 12 | fontHeight | `global_fontheight` | `24` | Static text font height, in logical units. |
| 13 | textRed | `global_statictextcolor_red` | `255` | Red component of static text color (0–255). |
| 14 | textGreen | `global_statictextcolor_green` | `0` | Green component of static text color (0–255). |
| 15 | textBlue | `global_statictextcolor_blue` | `0` | Blue component of static text color (0–255). |
| 16 | windowClass | `szWindowClass` | `L"spisocketswin32class"` | Window class name (wide string). |
| 17 | windowTitle | `szTitle` | `L"spisocketswin32title"` | Title displayed in the window’s title bar. |
| 18 | beginScript | `global_begin` | `"begin.ahk"` | Path to script/batch to run at startup (empty = none). |
| 19 | endScript | `global_end` | `"end.ahk"` | Path to script/batch to run on exit (empty = none). |


## Examples

1. **Run as server with defaults**

```plaintext
   spisocketswin32.exe
```

Listens on port 50001 and displays incoming messages in a layered window at (100,200), 400×400, 80% opacity.

1. **Run as client, send “Hello” to 192.168.1.10**

```plaintext
   spisocketswin32.exe client 192.168.1.10 Hello
```

Sends “Hello” to the server and exits.

1. **Run server at custom position/size and no title bar**

```plaintext
   spisocketswin32.exe server "" "" 50 50 600 300 255 0
```

Starts a server window at (50,50), 600×300, fully opaque, title bar hidden.

1. **Custom scripts on start/end**

```plaintext
   spisocketswin32.exe server "" "" 100 100 400 400 200 1 0 0 24 255 0 0 spisocketClass "My Title" startup.bat shutdown.bat
```

Make sure to quote any arguments containing spaces.