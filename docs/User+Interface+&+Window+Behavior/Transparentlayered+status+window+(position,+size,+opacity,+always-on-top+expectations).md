# User Interface & Window Behavior – Transparent/Layered Status Window

## Overview

The status window presents incoming UDP messages and automation-triggered notifications in a minimal, always-visible overlay. It uses Win32 layered window support to render text directly over the desktop or other applications, with configurable position, size, and opacity. Users can tailor the window geometry and transparency via command-line parameters, and the static text control renders messages with a transparent background and customizable text color.

## Window Creation and Layering

During initialization (`InitInstance`), the application creates the main window either as a standard framed window or a borderless popup, based on the `global_titlebardisplay` flag. Immediately after window creation, it enables layered rendering and applies the global opacity value:

```cpp
// Create either WS_OVERLAPPEDWINDOW or borderless WS_POPUP
if(global_titlebardisplay) {
    hWnd = CreateWindow(szWindowClass, szTitle,
        WS_OVERLAPPEDWINDOW,
        global_x, global_y, global_xwidth, global_yheight,
        NULL, NULL, hInstance, NULL);
} else {
    hWnd = CreateWindow(szWindowClass, szTitle,
        WS_POPUP | WS_VISIBLE,
        global_x, global_y, global_xwidth, global_yheight,
        NULL, NULL, hInstance, NULL);
}
global_hwnd = hWnd;

// Enable layered window and set opacity
SetWindowLong(hWnd, GWL_EXSTYLE,
    GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
SetLayeredWindowAttributes(hWnd, 0, global_alpha, LWA_ALPHA);
```

(InitInstance)

- WS_EX_LAYERED allows per-window alpha blending.
- `global_alpha` (0–255) controls overall window opacity.

## Command-Line Configuration of Geometry and Opacity

The window’s position (`global_x`, `global_y`), size (`global_xwidth`, `global_yheight`), and opacity (`global_alpha`) default to:

```cpp
int global_x = 100;
int global_y = 200;
int global_xwidth = 400;
int global_yheight = 400;
BYTE global_alpha = 200;
```

(Global defaults)

These values can be overridden by supplying additional command-line arguments when launching the application:

```cpp
if(nArgs > 4) global_x       = atoi(szArgList[4]);
if(nArgs > 5) global_y       = atoi(szArgList[5]);
if(nArgs > 6) global_xwidth  = atoi(szArgList[6]);
if(nArgs > 7) global_yheight = atoi(szArgList[7]);
if(nArgs > 8) global_alpha   = atoi(szArgList[8]);
```

()

## Static Control and Text Rendering

A child STATIC control displays status text. It is created with `WS_EX_TRANSPARENT` so its background does not obscure the layered window:

```cpp
HWND hStatic = CreateWindowEx(
    WS_EX_TRANSPARENT,
    L"STATIC", L"",
    WS_CHILD | WS_VISIBLE | global_staticalignment,
    0, 100, 100, 100,
    hWnd, (HMENU)IDC_MAIN_STATIC,
    GetModuleHandle(NULL), NULL);
SendMessage(hStatic, WM_SETFONT,
    (WPARAM)global_hFont,
    MAKELPARAM(FALSE, 0));
```

(WM_CREATE)

In the `WM_CTLCOLORSTATIC` handler, the background mode is set to transparent and the text color is drawn using the `global_statictextcolor` RGB value:

```cpp
case WM_CTLCOLORSTATIC: {
    SetBkMode((HDC)wParam, TRANSPARENT);
    SetTextColor((HDC)wParam, global_statictextcolor);
    return (INT_PTR)::GetStockObject(NULL_PEN);
}
```

(WM_CTLCOLORSTATIC)

The text color components themselves can be adjusted via command-line parameters `global_statictextcolor_red`, `_green`, and `_blue`, combined into `global_statictextcolor` before window creation  .

## Dynamic Resizing and Layout

When the window is resized (including on creation), the `WM_SIZE` handler recalculates the client area dimensions, updates the child control’s size, and reinitializes any dependent libraries:

```cpp
case WM_SIZE: {
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);
    global_staticwidth  = rcClient.right;
    global_staticheight = rcClient.bottom;
    SetWindowPos(
        GetDlgItem(hWnd, IDC_MAIN_STATIC),
        NULL, 0, 0,
        global_staticwidth, global_staticheight,
        SWP_NOZORDER);
    // Reinitialize layered or drawing subsystem as needed
    WavSetLib_Initialize(
        global_hwnd, IDC_MAIN_STATIC,
        global_staticwidth, global_staticheight,
        global_fontwidth, global_fontheight,
        global_staticalignment);
} break;
```

(WM_SIZE)

## Expected Visual Behavior

- The window appears at the specified (`global_x`, `global_y`) screen coordinates with the given width and height.
- Its overall transparency matches `global_alpha`.
- Status messages render as standalone text—no opaque background—allowing the desktop or underlying windows to show through.
- Text alignment (left, center, right) follows `global_staticalignment`.
- Font height is set via `global_fontheight`, and average character width is computed at runtime based on the selected font.

This minimal overlay design ensures the status window remains visually lightweight while clearly presenting incoming messages and automation notifications without disrupting the user’s workspace.