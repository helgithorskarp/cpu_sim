#include "UI.h"

namespace CpuSimUI {
    void RenderUI() {
        static result_simulation last_run_results;
        static bool data_exists = false;

        ImGui::Begin("CPU Simulator");

        if (ImGui::Button("Start SRTF")) {
            SRTF alg;
            std::vector<Process> processes = {
                Process(0, 8, 1, "P1"),  // long job, heavily starved
                Process(1, 4, 2, "P2"),  // preempts P1
                Process(2, 2, 3, "P3"),  // preempts P2
                Process(3, 1, 4, "P4"),  // ties with P3 remaining
                Process(4, 3, 5, "P5"),  // gets starved for a while
                Process(6, 1, 6, "P6"),  // preempts P2 mid-run
                Process(8, 5, 7, "P7"),  // long wait
                Process(9, 2, 8, "P8"),  // short, jumps queue
                Process(12, 1, 9, "P9"), // preempts P5
            };
            Cpu cpu(25, processes);

            last_run_results = cpu.run_scheduling_sim(alg);
            data_exists = true;
        }

        ImGui::Separator();

        if (data_exists) {
            ImGui::Text("Simulation finished.");
            for (Process& p : last_run_results.timeline) {
                ImGui::Text("Process %s arrived at %d and ended at %d", p.name.c_str(), p.arrival_time, p.completion_time);
            }
        }
        else {
            ImGui::Text("No simulation data yet.");
        }
        ImGui::End();
    }

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
}
