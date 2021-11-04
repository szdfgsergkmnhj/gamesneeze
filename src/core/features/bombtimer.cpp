#include "features.hpp"
#include "../../includes.hpp"
#include <SDL2/SDL_video.h>
#include <cmath>
#include <cstdio>
#include <cstring>
// thanks uc
float scaleDamageArmor(float flDamage, int armor_value) {
	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;
	if (armor_value > 0) {
		float flNew = flDamage * flArmorRatio;
		float flArmor = (flDamage - flNew) * flArmorBonus;
 
		if (flArmor > static_cast<float>(armor_value)) {
			flArmor = static_cast<float>(armor_value) * (1.f / flArmorBonus);
			flNew = flDamage - flArmor;
		}
 
		flDamage = flNew;
	}
	return flDamage;
}

float bombbar = 0;
void Features::BombTimer::draw() {
    if (CONFIGBOOL("Visuals>World>Items>C4 Timer")){
    ImGui::Begin("fart", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
    ImGui::Text("Bomb Timer");
    ImGui::Separator();
        char btimelabel[32] = "";
        char dtimelabel[32] = "";
        char bsite[32] = "";
        char bombdamage[32] = "";
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

                                //janky but looks gud
                                if (isdefused == 0 && bombTime >= 0.f)
                                    bombbar = bombTime;
                                //also jank but atleast the timer doesnt keep going after someone stops defusing
                                if (deftime <= 0.f)
                                deftime = 0;
                                else if (defuser == -1)
                                deftime = 0;

                                if (bombTime >= 0.f) {
                                    if (isdefused == 0) { //going
                                    ImGui::Text("Bomb Planted");
                                    snprintf(btimelabel, 32, "%.3f", bombTime);
                                    snprintf(dtimelabel, 32, "%.3f", deftime);
                                    }
                                    else { //defused
                                    ImGui::Text("Bomb Defused");
                                    snprintf(btimelabel, 32, "%.3f", bombbar); 
                                    snprintf(dtimelabel, 32, "");
                                    }
                                }
                                else if (isdefused == 0){ //exploded
                                    ImGui::Text("Bomb Exploded");
                                    snprintf(btimelabel, 32, "");
                                    snprintf(dtimelabel, 32, "");
                                }
                                switch (bombsite){
                                    case 0:
                                    snprintf(bsite, 32, "Site: A"); break;
                                    case 1:
                                    snprintf(bsite, 32, "Site: B"); break;
                                    //never seen more than 3 bombsites in a map so wont add more
                                    case 2: 
                                    snprintf(bsite, 32, "Site: C"); break;
                                }
                                // what the fuck even
                                float damagePercentage = 1.0f;
                                
                                float flDamage = 500.f; // 500 - default, if radius is not written on the map https://i.imgur.com/mUSaTHj.png
                                float flBombRadius = flDamage * 3.5f;
                                auto flDistanceToLocalPlayer = (((PlantedC4*)ent)->origin() + /*((PlantedC4*)ent)->viewOffset()*/ Globals::localPlayer->viewOffset()) - (Globals::localPlayer->origin() + Globals::localPlayer->viewOffset());// ((c4bomb origin + viewoffset) - (localplayer origin + viewoffset))
                                float fSigma = flBombRadius / 3.0f; 
                                auto fGaussianFalloff = exp(-flDistanceToLocalPlayer.Length() * flDistanceToLocalPlayer.Length() / (2.0f * fSigma * fSigma));
                                float flAdjustedDamage = flDamage * fGaussianFalloff * damagePercentage;
                                
                                //flAdjustedDamage = scaleDamageArmor(flAdjustedDamage, Globals::localPlayer->armor());

                                snprintf(bombdamage, 32, "Bomb damage: %.f", flAdjustedDamage);
                                // drawing
                                ImGui::ProgressBar(bombbar/40, ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f), btimelabel);
                                ImGui::ProgressBar(deftime/40, ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f), dtimelabel);
                                ImGui::Text(bsite);
                                ImGui::Text(bombdamage);
                            }
                        }
                    }
                }
            }
        }
    ImGui::End();
    }
}
