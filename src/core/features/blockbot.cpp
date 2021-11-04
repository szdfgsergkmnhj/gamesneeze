#include "features.h"
#include "../../includes.hpp"
//p100 paste from aimtux sponsored by fagi.xyz

void Features::Blockbot::createMove(CUserCmd* cmd) {
    if (Globals::localPlayer && CONFIGBOOL("Misc>Misc>Movement>Blockbot") && Menu::CustomWidgets::isKeyDown(CONFIGINT("Misc>Misc>Movement>Blockbot Key"))) {
        //C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer()); Globals::localPlayer exists
        float bestdist = 250.f;
        int index = -1;

	    for (int i = 1; i < Interfaces::globals->maxClients; i++) {
            Player* p = (Player*)Interfaces::entityList->GetClientEntity(i);
            if (p && p != Globals::localPlayer) {
                if (p->health() > 0 && !p->dormant() && !p->isEnemy() && p->visible()) {
		            float dist = Globals::localPlayer->origin().DistTo(p->origin());

		            if (dist < bestdist) {
		    	    bestdist = dist;
		            index = i;
		            }
	            }
            }
        }

        if (index != -1){
            Player* target = (Player*)Interfaces::entityList->GetClientEntity(index);
            if (target){
                QAngle angles = calcAngle(Globals::localPlayer->origin(), target->origin());
                angles.y -= Globals::localPlayer->eyeAngles().y;
                normalizeAngles(angles);

                if (angles.y < 0.0f)
                cmd->sidemove = 450.f;
                else if (angles.y > 0.0f)
                cmd->sidemove = -450.f;
            }
        }
    }
}