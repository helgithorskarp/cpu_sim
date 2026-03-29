#ifndef UI_H
#define UI_H
#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_internal.h"
#include "cpu.h"
#include "Algorithms.h"
#include "Algorithms/FCFS/FCFS.h"
#include "Algorithms/RR/RR.h"
#include "Algorithms/SJF/SJF.h"
#include "Algorithms/SRTF/SRTF.h"
#include <format>
#include <memory>
#include <set>
#include <map>
#include <iostream>
#include <cmath>

namespace CpuSimUI {
    enum {
        PADD_EXISTS = 1,
        PADD_EMPTY_NAME
    };
    enum {
        SIM_EMPTY_PROCESSES = 1,
        SIM_ALG_NULLPTR
    };
    struct algorithm_listing {
        std::shared_ptr<Algorithm> algorithm;
        std::string name;
        std::string ident;
    };
    struct process_time_seg {
        int start_time;
        int duration;
    };
    void SetDockspace();
    int CreateProcess(std::string name, int arrival_time, int burst_time);
    int StartSimulation(Algorithm& algorithm, int total_time);
    void RenderUI();
}

#endif
