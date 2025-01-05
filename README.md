
# NetMonitor

NetMonitor is a lightweight and efficient network monitoring tool built using C and the `ncurses` library. This project provides a terminal-based interface for tracking network connections, bandwidth usage, and traffic rates in real-time.

## Features

- **Real-Time Monitoring**:
  - View active TCP, UDP, and ICMP connections.
  - Monitor transmitted and received data for a selected network interface.
  - Track live traffic rates (RX/TX) and total bandwidth usage.

- **Interactive UI**:
  - Terminal-based UI using `ncurses`.
  - Dynamic resizing to adapt to your terminal size.
  - Intuitive navigation through active network connections.

- **Cross-Platform Compatibility**:
  - Designed to work on Linux systems using `/proc` and `/sys` directories.

---

## Getting Started

### Prerequisites

Ensure your system meets the following requirements:

- GCC (GNU Compiler Collection)
- `make`
- `ncurses` development libraries
- Linux-based OS with access to `/proc` and `/sys` directories

To install the necessary dependencies on Debian/Ubuntu, use:

```bash
sudo apt update
sudo apt install build-essential libncurses5-dev libncursesw5-dev
```

---

### Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/mariusel9911/SO2_PROIECT.git
   cd SO2_PROIECT
   ```

2. Build the project using `make`:

   ```bash
   make
   ```

3. Run the application:

   ```bash
   ./NetMonitor
   ```

---

## Usage

### Key Features

- **Interface Selection**:
  - Use arrow keys to navigate the list of network interfaces.
  - Press `Enter` to select an interface.
  - Press `Q` to quit the interface selection.

- **Connection Table**:
  - View detailed information about active connections, including:
    - Local Address and Port
    - Foreign Address and Port
    - Protocol (TCP, UDP, ICMP)
    - Bytes transferred
    - Data rate (in Kbps)

- **Footer Stats**:
  - Monitor total RX (received), TX (transmitted), and combined traffic.

### Controls

- `↑` / `↓` : Navigate through the connection table.
- `Q` : Exit the application.

---

## Project Structure

### Source Files

- `NetMonitor.c`: The main program, handling initialization, signal handling, and the application loop.
- `ui.c`: Functions for drawing and updating the UI, including tables and footers.
- `ipSearch_v1.c`: Functions to parse `/proc/net` for active connections and retrieve interface statistics from `/sys/class/net`.

### Headers

- `ui.h`: UI-related declarations.
- `ipSearch_v1.h`: Network-related declarations and data structures.

### Makefile

- Automates the build process.
- Supports `make` and `make clean`.

### Build Targets

- `all` (default): Compiles the project.
- `clean`: Removes compiled binaries.

---

## Example Output

Below is an example of the application's interface:

```
+---------------------------+----------------------+-------+----------+--------+
| Local Address            | Foreign Address      | Bytes | Protocol | Rate   |
+---------------------------+----------------------+-------+----------+--------+
| 127.0.0.1:631            | 0.0.0.0:0            | 0 B   | TCP      | 0.00 Kbps |
| 127.0.0.1:25             | 0.0.0.0:0            | 0 B   | TCP      | 0.00 Kbps |
| 10.0.2.15:59648          | 146.75.122.132:80    | 0 B   | TCP      | 0.00 Kbps |
| 10.0.2.15:59644          | 146.75.122.132:80    | 0 B   | TCP      | 0.00 Kbps |
| 10.0.2.15:43008          | 13.107.246.44:443    | 0 B   | TCP      | 0.00 Kbps |
| 10.0.2.15:68             | 10.0.2.2:67          | 0 B   | UDP      | 0.00 Kbps |
| 0.0.0.0:51771            | 0.0.0.0:0            | 0 B   | UDP      | 0.00 Kbps |
| 0.0.0.0:5353             | 0.0.0.0:0            | 0 B   | UDP      | 0.00 Kbps |
+---------------------------+----------------------+-------+----------+--------+

RX: 180.00 B                1.04 kbps                        Interface: enp0s3
TX: 198.00 B                1.15 kbps
Total: 378.00 B                                              PRESS Q TO EXIT...


```

---

## Contributing

We welcome contributions! If you have ideas for improvements or find a bug, feel free to:

1. Fork the repository.
2. Create a new branch for your feature or bugfix.
3. Submit a pull request with a clear description.



## Acknowledgments

- [NCurses Documentation](https://invisible-island.net/ncurses/)
- Linux `/proc` and `/sys` documentation
- Open-source contributors for their support and inspiration.

---

## Authors

- **mariusel9911**
- paulescufilip
- GitHub: [https://github.com/mariusel9911](https://github.com/mariusel9911)

Feel free to reach out for collaboration or any questions regarding the project!

---

Thank you for using NetMonitor! 🚀

