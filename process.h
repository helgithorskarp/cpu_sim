#ifndef PROCESS_H
#define PROCESS_H
#include "common.h"





class Process {
    public:
    /// arrival time, burst time, pid and name need to be always passed in 
        Process(int _arrival_time, int _burst_time, int _pid, std::string _name, int _priority=-1) :
            arrival_time(_arrival_time), burst_time(_burst_time), pid(_pid), name(_name), priority(_priority), remaining_time(_burst_time) {}

        Process() {}
        int priority; /// Only used for algorithms that care about priotiy
        int arrival_time; /// in the timeline at what time slice does this process arrive
        int burst_time; /// totel time proccess needs on the cpu
        
        // tracking progress
        int remaining_time;      /// how much time does it need on the cpu in total
        int response_time = -1;    /// Time it takes to first get a run on the cpu, defaulted to -1, if it stays there proccess never got a run on cpu

        /// total waiting time can be derived from burst time, arrival time and remaining time 
        /// turnaround time can also be calculated from compleation time, and arrival time (Time it takes from arrival -> Completion)
        /// response time can be calculated from, arrival time, response_time.
        
        // scheduling metadata
        int pid;
        int completion_time = -1; /// when does this process finish, remaining time == 0
        std::string name;
};

#endif