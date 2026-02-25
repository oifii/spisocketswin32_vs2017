# Troubleshooting & FAQ – Runtime Issues

This section covers common runtime problems when running spisocketswin32 in UDP server or client mode, including missing messages, firewall or port conflicts, and invisible window behavior. Each entry explains how the code handles the scenario and suggests diagnostics or remedies.

## No UDP Messages Received

### 1. Server Fails to Bind to Port 50001

In StartGlobalProcess, the server socket is created and bound each timer tick. If `socket.bind(port)` does not return `sf::Socket::Done`, the loop breaks silently—no error is reported and no messages are received .

• **Diagnosis**

- Ensure no other process is listening on UDP port 50001.
- From a command prompt, run:

```bat
    netstat -a -n -p UDP | findstr 50001
```

- If another application holds the port, stop or reconfigure it.

• **Solution**

- Close conflicting application or change its port.
- To use a different port, rebuild with a new `port` constant (default: 50001) in `resource.h` .

### 2. Client Sends to Wrong Destination IP

When running in client mode, the application sends its message to `global_ipaddress` on port 50001. If the IP is incorrect, your server will not receive it .

• **Diagnosis**

- Verify the server’s IP address (e.g., `ipconfig` on Windows).
- Check that `global_ipaddress` was passed correctly as the second command-line argument.

• **Solution**

- Launch the app with:

```bat
    spisocketswin32 client 192.168.1.42 "Hello"
```

- For local testing, use `127.0.0.1` or the server’s LAN IP.

## Firewall and Network Configuration

### 1. Windows Firewall Blocking UDP

By default, Windows Firewall may block incoming UDP on port 50001.

• **Diagnosis**

- From an elevated PowerShell prompt, run:

```powershell
    Get-NetFirewallRule –DisplayName "*spisocketswin32*"  
```

• **Solution**

- Create an inbound rule to allow UDP on port 50001 for `spisocketswin32.exe`.
- Open **Windows Defender Firewall with Advanced Security**.
- Add **New Rule → Port → UDP 50001 → Allow** → select the spisocketswin32 executable.

### 2. Network Address Translation (NAT) or VPN

If server and client are on different subnets, ensure routers or VPN allow UDP 50001 traffic between them.

## Port-in-Use Bind Failures

When `socket.bind(port)` or `socket.receive(...)` fails, the code uses `break;` to exit the receive loop without logging.

• **Symptom**

- Server window launches but never restores or shows new text.

• **Workaround**

- Insert logging around bind/receive to capture errors.
- Alternatively, modify the code to retry bind on failure or display a `MessageBox` on error.

## Window Not Visible or Always Minimized

### 1. Layered Window Transparency

In `InitInstance`, the window is created as a layered (transparent) window with alpha set by `global_alpha` . If `global_alpha` is 0, the window is fully transparent.

• **Diagnosis**

- Check the alpha argument (9th command-line parameter).
- Default: 200 (semi-opaque).

• **Solution**

- Pass a higher alpha (0–255) as parameter 8:

```bat
    spisocketswin32 server "" "" 100 100 400 400 200
```

### 2. Title Bar and Window Style

If `global_titlebardisplay` is 0, the window is created with `WS_POPUP | WS_VISIBLE`—no border or caption .

• **Symptom**

- The window has no frame and may be hard to find on screen.

• **Solution**

- Pass `1` as the 10th argument to enable title bar:

```bat
    spisocketswin32 server "" "" 100 100 400 400 200 1
```

## Automatic Window Restoration on Message Receipt

When a message arrives, the code restores the window (if minimized or hidden) and resets a 5-second timer before hiding it again .

```cpp
// Inside StartGlobalProcess:
if (socket.receive(in, sizeof(in), received, sender, senderPort) != sf::Socket::Done)
    break;
ShowWindow(global_hwnd, SW_RESTORE);
KillTimer(global_hwnd, IDT_TIMER1);
SetTimer(global_hwnd, IDT_TIMER1, 5000, NULL);
```

• **Behavior**

- `ShowWindow(..., SW_RESTORE)` brings the window to the foreground.
- A one-shot 5 second timer (`IDT_TIMER1`) is set; when it elapses, the window is hidden again.

• **Tip**

- To keep the window visible longer, adjust the timer interval in `InitInstance` or `StartGlobalProcess`.

---

If you encounter issues beyond these scenarios, enable verbose logging around `socket.bind`, `socket.send`, and `socket.receive` calls to pinpoint network or API errors.