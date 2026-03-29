#include "UI.h"

namespace CpuSimUI {
    static std::vector<Process> processes;
    static int n_pid = 1;
    static std::string add_process_status_msg = "";
    static std::vector<algorithm_listing> algorithms;
    static result_simulation simulation_results;
    static std::set<int> unique_pids;
    static std::map<int, Process> pid_to_process;

    void SetDockspace() {
        static bool opt_fullscreen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        if (opt_fullscreen) {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        ImGui::Begin("Dockspace Parent", nullptr, window_flags);
        if (opt_fullscreen) ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("Dockspace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        else {
            ImGui::Text("ERROR: Docking is not enabled in io.ConfigFlags");
        }
        ImGui::End();
    }

    void InitAlgorithms() {
        algorithms.push_back({std::make_shared<FCFS>(), "First Come, First Serve", "FCFS"});
        algorithms.push_back({std::make_shared<RR>(1), "Round Robin", "RR"});
        algorithms.push_back({std::make_shared<SJF>(), "Shortest Job First", "SJF"});
        algorithms.push_back({std::make_shared<SRTF>(), "Shortest Remaining Time First", "SRTF"});
    }

    int CreateProcess(std::string name, int arrival_time, int burst_time) {
        if (name.empty())
            return PADD_EMPTY_NAME;
        for (const auto& p : processes) {
            if (p.name == name)
                return PADD_EXISTS;
        }
        Process p;
        p.name = name;
        p.arrival_time = arrival_time;
        p.burst_time = burst_time;
        p.pid = n_pid++;
        processes.push_back(p);
        return 0;
    }

    int StartSimulation(std::shared_ptr<Algorithm> algorithm) {
        if (processes.empty())
            return SIM_EMPTY_PROCESSES;
        if (algorithm == nullptr)
            return SIM_ALG_NULLPTR;
        int total_time = 0;
        for (const auto& p : processes) {
            total_time += p.burst_time;
        }
        Cpu cpu(total_time, processes);
        simulation_results = cpu.run_scheduling_sim(*algorithm);
        for (const auto& p : simulation_results.timeline) {
            unique_pids.insert(p.pid);
            pid_to_process[p.pid] = p;
            std::cout << p.pid << ": " << p.name << std::endl;
        }
        return 0;
    }

    void RenderUI() {
        /*
         * BEGIN - Create process
         */
        static std::string n_process_name;
        static int n_arrival_time = 0;
        static int n_burst_time = 1;
        if (algorithms.empty())
            InitAlgorithms();

        ImGui::Begin("Create Process", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::InputText("Process Name", &n_process_name);
        ImGui::SliderInt("Arrival Time", &n_arrival_time, 0, 100);
        ImGui::SliderInt("Burst Time", &n_burst_time, 1, 100);
        ImGui::Spacing();

        if (ImGui::Button("Add Process")) {
            int padd_status = CreateProcess(n_process_name, n_arrival_time, n_burst_time);
            if (padd_status == PADD_EMPTY_NAME)
                add_process_status_msg = "Process name cannot be empty";
            else if (padd_status == PADD_EXISTS)
                add_process_status_msg = std::format("Process \"{}\" already exists", n_process_name);
            else
                add_process_status_msg = std::format("Process \"{}\" (PID: {}) added", n_process_name, n_pid - 1);
        }

        ImGui::SameLine(0, -1);
        ImGui::Text("%s", (add_process_status_msg.length() == 0) ? "" : add_process_status_msg.c_str());
        ImGui::End();
        /*
         * END - Create process
         */

        /*
         * BEGIN - Processes list
         */
        ImGui::Begin("Processes", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        static ImGuiTableFlags processes_table_flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg;

        if (ImGui::BeginTable("Processes", 4, processes_table_flags)) {
            ImGui::TableSetupColumn("PID");
            ImGui::TableSetupColumn("Process Name");
            ImGui::TableSetupColumn("Arrival Time");
            ImGui::TableSetupColumn("Burst Time");
            ImGui::TableHeadersRow();

            for (int i = 0; i < processes.size(); ++i) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d", processes[i].pid);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", processes[i].name.c_str());
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%d", processes[i].arrival_time);
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%d", processes[i].burst_time);
            }
            ImGui::EndTable();
        }
        ImGui::End();
        /*
         * END - Processes list
         */

        /*
         * BEGIN - Run simulation
         */
        ImGui::Begin("Simulation", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        static std::string simulation_status_msg = "";
        static int selected_algorithm_idx = 0;
        static std::shared_ptr<Algorithm> selected_algorithm;
        static const char* selected_algorithm_preview = "Select an algorithm";

        if (ImGui::BeginCombo("Algorithms", selected_algorithm_preview)) {
            for (int i = 0; i < algorithms.size(); ++i) {
                const bool is_selected = (selected_algorithm_idx == i);
                if (ImGui::Selectable(algorithms[i].name.c_str(), is_selected)) {
                    selected_algorithm_idx = i;
                    selected_algorithm_preview = algorithms[i].name.c_str();
                    selected_algorithm = algorithms[i].algorithm;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (ImGui::Button("Run Simulation")) {
            int simulation_status = StartSimulation(selected_algorithm);
            if (simulation_status == SIM_ALG_NULLPTR)
                simulation_status_msg = "Please select an algorithm";
            else if (simulation_status == SIM_EMPTY_PROCESSES)
                simulation_status_msg = "Please add at least one process";
            else {
                simulation_status_msg = std::format("Successfully ran {}", algorithms[selected_algorithm_idx].ident);
            }
        };
        ImGui::Text("%s", simulation_status_msg.c_str());
        ImGui::End();
        /*
         * END - Run simulation
         */

        /*
         * BEGIN - Show simulation results
         */
        ImGui::Begin("Results", nullptr);

        static ImGuiTableFlags results_table_flags = ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_Resizable;
        static float slot_w = 15.0f;
        static float row_h = 25.0f;

        if (ImGui::BeginTable("Gantt Chart", 2, results_table_flags)) {
            ImGui::TableSetupColumn("Process", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableSetupColumn("Timeline", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();

            std::vector<Process> timeline = simulation_results.timeline;

            if (!timeline.empty()) {
                for (int pid : unique_pids) {
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, row_h);

                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("%s", pid_to_process[pid].name.c_str());
                    ImGui::TextDisabled("PID: %d", pid);

                    ImGui::TableSetColumnIndex(1);
                    ImVec2 start_pos = ImGui::GetCursorScreenPos();
                    ImDrawList* dl = ImGui::GetWindowDrawList();

                    for (int i = 0; i < timeline.size(); ++i) {
                        if (timeline[i].pid == pid) {
                            ImVec2 b_min = ImVec2(start_pos.x + (i * slot_w), start_pos.y);
                            ImVec2 b_max = ImVec2(b_min.x + slot_w, start_pos.y + row_h);

                            ImU32 color = ImColor::HSV(pid * 0.16f, 0.7f, 0.8f);

                            dl->AddRectFilled(b_min, b_max, color);
                            dl->AddLine(ImVec2(b_max.x, b_min.y), ImVec2(b_max.x, b_max.y), IM_COL32(255,255,255,30));
                        }
                    }
                }
                ImGui::Dummy(ImVec2(timeline.size() * slot_w, row_h));
            }
            ImGui::EndTable();
        }
        ImGui::End();
        /*
         * END - Show simulation results
         */
    }
}
