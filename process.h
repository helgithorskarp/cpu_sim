#ifndef PROCESS_H
#define PROCESS_H
#include "common.h"

enum Priority {
    High,
    Medium,
    Low
};


enum ProcessState {
    BLOCKING,
    RUNNING,
    READY
};


class Process {
    public:
        Priority priority; /// Only used for algorithms that care about priotiy
        int arrival_time; /// in the timeline at what time slice does this process arrive
        int burst_time; /// Minimum amount of time process needs on the cpu before preemption
        
        // tracking progress
        int remaining_time;      /// how much time does it need on the cpu in total
        int total_wait_time = 0; /// total time process spends not on the cpu (has arrived and is not running)
        int turnaround_time = 0;  //// Time it takes from arrival -> Completion
        int response_time = 0;    /// Time it takes to first get a run on the cpu
        
        // scheduling metadata
        int pid;
        int completion_time = 0; /// when does this process finish, remaining time == 0
        int last_scheduled_time = 0;  
        std::string name;
        
        // state, possibly not needed for this, lets see...
        ProcessState state; 
};

#endif