#ifndef CPU_H
#define CPU_H
#include "process.h"
#include "common.h"
#include "Algorithms/Algorithms.h"


struct result_simulation {
    double avg_waiting_time; /// avg waiting time of all FINISHED PROCCESES
    double avg_turnaround_time; /// avg turnaround time of all FINISHED procceses
    double avg_response_time; /// avg response time of all proccesses that actually got a run on cpu
    double throughput;  // if all procceses finished -> Finish time / # procceses, if all procceses did not finish -> Time_line_length / # procceses finished
    std::vector<Process> timeline;
    std::vector<Process> processes;  // final state of all processes for per-process breakdown
};

class Cpu {
    public:
        std::vector<Process> time_line; /// Keeps track of what Process in the cpu at time time_line_length.
        int time_line_length;
        Process *running_process = nullptr;
        std::vector<Process> processes;

        Cpu(int _time_line_length, std::vector<Process> _processes) : time_line_length(_time_line_length), processes(_processes) {}



        result_simulation run_scheduling_sim(Algorithm& scheduling_algorithm);
};




#endif