## Networking Details & Protocol Notes

This section describes how spisocketswin32 handles UDP communication: the fixed listening port, the limits on incoming payloads, and how sender information is surfaced to the user in the layered window.

### UDP Port

- spisocketswin32 always uses UDP port 50001 for both server (listener) and client (sender) roles.
- The port is defined as a global constant in the main module:

```cpp
  const unsigned short port = 50001; // Choose an arbitrary port for opening sockets
```

- Regardless of whether the application is started in “server” or “client” mode (as determined by the first command-line argument), all SFML UdpSocket bind/send calls reference this same `port` constant.

### Payload Size

- Incoming UDP messages are read into a fixed-size buffer of 128 bytes. In the timed listener callback (`StartGlobalProcess`), this is declared as:

```cpp
  const int maxsize = 128;
  char in[maxsize];
  memset(in, 0, maxsize);
  std::size_t received;
```

- The socket receive call uses the full size of this buffer:

```cpp
  if (socket.receive(in, sizeof(in), received, sender, senderPort) != sf::Socket::Done)
      break;
```

- Practical constraints:
- Any datagram larger than 128 bytes will be silently truncated to fit this buffer.
- `received` indicates the actual number of bytes written; applications relying on larger payloads must implement fragmentation at the caller side or increase `maxsize` in the source.

### Sender Identification

spisocketswin32 surfaces both the sender’s IP address and port to the user:

1. **IP Address Display**

After a successful receive, the code captures the remote endpoint:

```cpp
   sf::IpAddress sender;
   unsigned short senderPort;
   socket.receive(in, sizeof(in), received, sender, senderPort);
```

1. **UI Logging**

The transparent layered window appends lines showing:

- A “listening” notification:

```cpp
     global_line = global_servername;
     global_line += " is listening to port, waiting for a message...\n";
     StatusAddText(global_line.c_str());
```

- The sender’s IP (via `sender.toString()`) on its own line:

```cpp
     global_line = "Message received from client ";
     StatusAddText(global_line.c_str());
     global_line = sender.toString();
     StatusAddText(global_line.c_str());
     StatusAddText("\n");
```

- The message payload immediately afterward:

```cpp
     global_line = in;
     global_line += "\0"; // ensure null termination
     StatusAddText(global_line.c_str());
     StatusAddText("\n");
```

Users will see:

- A prompt in the transparent window indicating the server is awaiting a message.
- Upon receive, the sender’s IP string, followed by the raw text of the payload, each on separate lines.

These network details ensure predictable behavior for automation scripts (`xaos_cmd_*.bat`) that parse incoming messages: all commands arrive on UDP 50001, must fit within 128 bytes, and are clearly annotated with the sender address in the UI log.