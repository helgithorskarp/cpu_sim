#CPU Scheduler Simulator

A C++23 CPU scheduling visualizer using ImGui and GLFW.

## Prerequisites

Cloning the repository:
```bash
git clone https://github.com/helgithorskarp/cpu_sim.git && cd cpu_sim
```

The following are required to build the project regardless of platform:
* **C++ compiler** with C++23 support
* **CMake 3.20+**
* **Ninja**

### Windows
* **MSVC 17.4+** *(Visual Studio 2022)* — install the "Desktop development with C++" workload
* **CMake** — [direct download](https://cmake.org/download/) or via package manager
* **Ninja** — via any of the following:
```bash
winget install Ninja-build.Ninja
scoop install ninja
choco install ninja
```

### macOS
* **Apple Clang 15+** *(Xcode 15 or later)*
* **CMake** and **Ninja** via Homebrew:
```bash
brew install cmake ninja
```

### Linux

**Ubuntu / Debian**
```bash
sudo apt install cmake ninja-build build-essential \
  libx11-dev libxrandr-dev libxinerama-dev \
  libxcursor-dev libxi-dev libgl1-mesa-dev
```

**Arch / Manjaro**
```bash
sudo pacman -S --needed cmake ninja base-devel \
  libx11 libxrandr libxinerama libxcursor libxi mesa
```

**Fedora**
```bash
sudo dnf install cmake ninja-build gcc-c++ \
  libX11-devel libXrandr-devel libXinerama-devel \
  libXcursor-devel libXi-devel mesa-libGL-devel
```

---

## Build and Run

Run these commands from the project root:

### 1. Configure
```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
```

### 2. Compile
```bash
cmake --build build
```

### 3. Execution
* **Windows:** `.\build\cpu_sim.exe`
* **macOS/Linux:** `./build/cpu_sim`

---

## Project Structure

* **main.cpp**: Window and OpenGL context setup.
* **UI.cpp**: ImGui interface logic.
* **cpu.cpp**: Simulation engine.
* **Algorithms/**: Logic for FCFS, SJF, SRTF, RR, and Priority.
