# CPU Scheduler Simulator

A C++23 CPU scheduling visualizer using ImGui and GLFW.

## Prerequisites

* Clone the repository
```bash
git clone https://github.com/helgithorskarp/cpu_sim.git
cd cpu_sim
```

### Windows
* __MSVC 17.4+__ *(Visual Studio 2022)*
* [__CMake (direct link)__](https://cmake.org/download/)
* __Ninja__ *(via package manager)*
```bash
winget install ninja
scoop install ninja
choco install ninja
```


### macOS
* __Apple Clang 15+__
* __CMake__ *(via package manager)*
* __Ninja__ *(via package manager)*
```bash
brew install cmake
brew install ninja
```

### Linux
* __GCC 13+ or Clang 16+__
* __CMake__ *(via package manager)*
* __Ninja__ *(via package manager)*
* __Graphics Development Libraries__ *(via package manager)*:
```bash
sudo apt install cmake ninja-build build-essential libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev
sudo pacman -S cmake ninja-build build-essential libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev
yay -S install cmake ninja-build build-essential libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev
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
