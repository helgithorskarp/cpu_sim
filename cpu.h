#ifndef CPU_H
#define CPU_H
#include "process.h"
#include "common.h"
#include "Algorithms/Algorithms.h"


struct result_simulation {
    int avg_waiting_time;
    int avg_turnaround_time;
    int avg_response_time;
    float cpu_utilization;
    int total_idle_time;
    int throughput;          // processes completed per unit time
    std::vector<Process> timeline;
    std::vector<Process> processes;  // final state of all processes for per-process breakdown
};

class Cpu {
    public:
        std::vector<Process> time_line; /// Keeps track of what Process in the cpu at time time_line_length.
        int time_line_length;
        Process running_process;



        result_simulation run_scheduling_sim(Algorithm& scheduling_algorithm);
};




#endif