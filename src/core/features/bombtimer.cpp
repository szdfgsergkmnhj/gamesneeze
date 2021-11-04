#include "features.hpp"
#include "../../includes.hpp"
#include <cstring>

void Features::BombTimer::draw() {
    if (CONFIGBOOL("Visuals>World>Items>C4 Timer")){
    ImGui::Begin("fart", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
    ImGui::Text("Bomb Timer");
        char btimelabel[32] = "";
        char dtimelabel[32] = "";
        if (Interfaces::engine->IsInGame()) {
            for (auto i : entityDistanceMap) {
                if (Globals::localPlayer) {
                    if (i.second != Interfaces::engine->GetLocalPlayer()) {
                        Entity* ent = (Entity*)Interfaces::entityList->GetClientEntity(i.second);
                        if (ent) {
                            ClientClass* clientClass = ent->clientClass();
                            if (clientClass->m_ClassID == EClassIds::CPlantedC4) {
                                float bombTime = ((PlantedC4*)ent)->time() - Interfaces::globals->curtime;
                                int isdefused = ((PlantedC4*)ent)->defused();
                                float deftime = ((PlantedC4*)ent)->defusetime() - Interfaces::globals->curtime;
                                int defuser = ((PlantedC4*)ent)->defuser();
                                int bombsite = ((PlantedC4*)ent)->bsite();
                                //janky but atleast the timer doesnt keep going after someone stops defusing
                                if (deftime <= 0.f)
                                deftime = 0;
                                else if (defuser == -1)
                                deftime = 0;
                                
                                if (bombTime >= 0.f) {
                                    if (isdefused == 0) {
                                    snprintf(btimelabel, 32, "%.3f", bombTime);
                                    snprintf(dtimelabel, 32, "%.3f", deftime);
                                    }
                                    else {
                                    snprintf(btimelabel, 32, "Defused!");
                                    snprintf(dtimelabel, 32, "");
                                    }
                                }
                                
                                ImGui::ProgressBar(bombTime/40, ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f), btimelabel);
                                ImGui::ProgressBar(deftime/40, ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f), dtimelabel);
                                /*
                                Globals::drawList->AddText(ImVec2(100, 16), ImColor(0, 0, 0, 255), label);
                                Globals::drawList->AddText(ImVec2(99, 15), ImColor(255, 255, 255, 255), label);
                                //bomb time
                                Globals::drawList->AddRectFilled(ImVec2(99, 15), ImVec2(99 + bombTime * 401/bombTime, 20), ImColor(255, 255, 255, 150));
                                Globals::drawList->AddRectFilled(ImVec2(99, 15), ImVec2(500, 20), ImColor(255, 255, 255, 150));
                                //defuse time
                                Globals::drawList->AddRectFilled(ImVec2(99, 32), ImVec2(99 + deftime * 401/deftime, 27), ImColor(255, 255, 255, 150));
                                Globals::drawList->AddRectFilled(ImVec2(99, 22), ImVec2(500, 27), ImColor(255, 255, 255, 150));
                                */
                                
                            }
                        }
                    }
                }
            }
        }
    ImGui::End();
    }
}
