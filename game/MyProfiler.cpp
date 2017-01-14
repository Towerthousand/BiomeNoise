#include "MyProfiler.hpp"

MyProfiler::MyProfiler() {
}

void MyProfiler::renderCustomInterface() const {
    ImGui::Begin("Controls", nullptr, ImVec2(0.47f*Window::getInstance()->getSize().x, 0.11f*Window::getInstance()->getSize().y), 0.9f, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::SetWindowPos(ImVec2(0.5f*Window::getInstance()->getSize().x, 0.05f*Window::getInstance()->getSize().y), ImGuiSetCondition_FirstUseEver);
    ImGui::Separator();
    ImGui::Text("Biome Generator");
    ImGui::End();
}
