#include "cpu.h"
#include "Algorithms/SJF/SJF.h"
#include "Algorithms/RR/RR.h"

int main(void) {
SJF alg;
std::vector<Process> processes = {
    Process(0, 8, 1, "P1"),  
    Process(1, 3, 2, "P2"),
    Process(2, 3, 3, "P3"),  
    Process(5, 1, 4, "P4"),
    Process(8, 5, 5, "P5"), 
    Process(9, 2, 6, "P6"),
};
Cpu cpu(30, processes);
    result_simulation result = cpu.run_scheduling_sim(alg);

    std::cout << "Avg Response Time:   " << result.avg_response_time << '\n';
    std::cout << "Avg Turnaround Time: " << result.avg_turnaround_time << '\n';
    std::cout << "Avg Waiting Time:    " << result.avg_waiting_time << '\n';
    std::cout << "Throughput:          " << result.throughput << '\n';
}