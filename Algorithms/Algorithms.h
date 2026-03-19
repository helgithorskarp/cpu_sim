#ifndef ALGORITHM_H
#define ALGORITHM_H
#include "../common.h"
#include "../process.h"



/// This is simply a interface saying that all algorithms must implement this step function
/// Each algorithm individually handles creating their own step funciton, they get a time number n, and the current process on the cpu
/// they run their code, decide which process should be now on the cpu, either return the same process or a new one, 
/// and update data of the process them selves like increasing the total waited time of all processes not running, etc

class Algorithm {
public:
    virtual std::optional<Process> step(int current_time, Process* currently_running) = 0;
    virtual ~Algorithm() = default;
};
#endif