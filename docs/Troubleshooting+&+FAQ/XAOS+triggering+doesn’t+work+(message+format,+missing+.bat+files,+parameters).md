# Troubleshooting & FAQ – XAOS Triggering Doesn’t Work

When running in server mode, spisocketswin32 listens for UDP messages and attempts to dispatch XAOS automation by launching batch scripts. If you see incoming messages displayed but no XAOS action is taken, use the guidance below to diagnose and resolve the issue.

## Common Symptoms

- The transparent window shows “Message received…” but no batch file runs.
- Status window displays “WARNING: invalid xaos command received” followed by the raw message.
- No parameters are passed to your `.bat` scripts, or they error out.

## 1. Message Format Requirements

XAOS dispatch logic only parses messages meeting a minimum length and format:

- Total message length must be at least 20 characters.
- Characters at positions 15–19 (zero-based substring indices) are extracted and converted to an integer command code.
- All characters from position 20 to the end form the parameters string.

Implementation snippet:

```cpp
unsigned int xaos_command = 0;
std::string xaos_parameters = "";

if (global_line.length() >= 20)
{
    // Extract 5‐character command field starting at index 15
    xaos_command = atoi((global_line.substr(15, 5)).c_str());
    // Remainder is passed as parameters
    xaos_parameters = global_line.substr(20, global_line.length() - 20);
}
```

Ensure your UDP-sent message pads or formats the command code exactly in that field. For example, to send command 10 with parameters `"zoom 2.0"`, your message might look like:

```plaintext
ABCDEFGHIJKLMNO0010zoom 2.0
12345678901234567890123456
          ^^^^^   ^^^^^^^^^
          cmd=0010 params=zoom 2.0
```

## 2. Supported XAOS Command Values

Only four discrete command codes are recognized. If the parsed integer does not match one of these, a warning is issued and no script runs:

- 1
- 10
- 100
- 1000

```cpp
if (xaos_command == 1)
    ShellExecuteA(NULL, "open", "xaos_cmd_1.bat", xaos_parameters.c_str(), NULL, nShowCmd);
else if (xaos_command == 10)
    ShellExecuteA(NULL, "open", "xaos_cmd_10.bat", xaos_parameters.c_str(), NULL, nShowCmd);
else if (xaos_command == 100)
    ShellExecuteA(NULL, "open", "xaos_cmd_100.bat", xaos_parameters.c_str(), NULL, nShowCmd);
else if (xaos_command == 1000)
    ShellExecuteA(NULL, "open", "xaos_cmd_1000.bat", xaos_parameters.c_str(), NULL, nShowCmd);
else
{
    StatusAddText("WARNING: invalid xaos command received\n");
    StatusAddText(global_line.c_str());
    StatusAddText("\n\n");
}
```

## 3. Required `.bat` Files

The following batch scripts must exist in the application’s working directory or be reachable via the system PATH. ShellExecuteA invokes them by name:

- xaos_cmd_1.bat
- xaos_cmd_10.bat
- xaos_cmd_100.bat
- xaos_cmd_1000.bat

If any of these files are missing, ShellExecuteA will fail silently. Verify:

1. Filenames are exact (including underscores and extension).
2. The current working directory is where the `.bat` files reside, or they are installed in a folder listed in %PATH%.

## 4. Parameter Passing

Parameters are passed exactly as the fourth argument to ShellExecuteA. If your batch script expects quoted parameters or escaped characters, ensure your UDP message encodes them correctly. For example, to send two parameters:

```plaintext
…1000firstArg secondArg
```

Here `xaos_parameters` becomes `"firstArg secondArg"`.

If parameters contain spaces or special characters, wrap them inside quotes in the script or adjust your batch file to parse `%*`.

## Diagnosing XAOS Dispatch Failures

1. **Check the Status Window** – Warnings or raw message dumps reveal parse failures.
2. **Log **`**global_line**`** Length and Content** – Insert temporary `StatusAddText` calls before parsing to confirm message integrity.
3. **Verify Batch File Existence** – From a command prompt in the working directory, run `dir xaos_cmd_*.bat`.
4. **Manual Invocation** – Copy the parameters logged and run:

```bat
   xaos_cmd_10.bat zoom 2.0
```

to confirm the script itself works.

1. **Process Monitor** – Use Sysinternals ProcMon to see if ShellExecuteA attempts to launch the expected `.bat` file and where it looks for it.

## FAQ

**Q: I get “WARNING: invalid xaos command received” but my message looks correct.**

A: Confirm your command code occupies exactly characters 15–19 in the message and is zero-padded if necessary. The total length must be ≥ 20 characters before parsing.

**Q: Parameters never reach my **`**.bat**`** script.**

A: Ensure your message has no hidden nulls or non-printable characters before index 20. Use a hex editor or debug `global_line` to inspect the substring.

**Q: Batch files run but do nothing.**

A: Check that the working directory context inside the `.bat` script is correct, or use absolute paths within the script.

**Q: I need additional XAOS commands beyond 1, 10, 100, 1000.**

A: The application only supports those four codes. To add more, you must modify and recompile the source to include new `else if` branches and supply corresponding `.bat` files.

---

By following the format and values enforced in the parsing logic, and ensuring all required `.bat` files are in place, XAOS automation should trigger reliably upon incoming UDP messages.