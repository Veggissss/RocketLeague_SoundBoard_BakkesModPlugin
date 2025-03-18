#include "pch.h"
#include "SoundBoardPlugin.h"
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "Winmm.lib")
#include <mmsystem.h>
#include <string>
#include <Lmcons.h>
#include <filesystem>

BAKKESMOD_PLUGIN(SoundBoardPlugin, "A soundboard plugin who plays custom sounds when game events", "1.3.2", PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;
std::chrono::time_point<std::chrono::steady_clock> lastSoundTime;
std::chrono::milliseconds minInterval;

// Load function
void SoundBoardPlugin::onLoad()
{
    _globalCvarManager = cvarManager;
    lastSoundTime = std::chrono::steady_clock::now();
    minInterval = std::chrono::milliseconds(500);
    this->LoadHooks();
}

// Hooks listener
void SoundBoardPlugin::LoadHooks()
{
    // CrossBar collision detection
    gameWrapper->HookEvent(
        "Function TAGame.GoalCrossbarVolumeManager_TA.CalculateHitNormal", // *.TriggerHit for all bar hits.
        [this](std::string eventName) {
            this->PlayASound("crossbar.wav");
        }
    );

    // Car hits ball collision with cooldown
    gameWrapper->HookEvent(
        "Function TAGame.Car_TA.OnHitBall",
        [this](std::string eventName) {
            auto now = std::chrono::steady_clock::now();
            if (now - lastSoundTime >= minInterval) {
                lastSoundTime = now;
                this->PlayASound("car_hit_ball.wav");
            }
        }
    );

    // Bump without demo
    gameWrapper->HookEvent(
        "Function TAGame.Car_TA.BumpCar",
        [this](std::string eventName) {
            auto now = std::chrono::steady_clock::now();
            if (now - lastSoundTime >= minInterval) {
                lastSoundTime = now;
                this->PlayASound("bump.wav");
            }
        }
    );

    gameWrapper->HookEventWithCallerPost<ServerWrapper>("Function TAGame.GFxHUD_TA.HandleStatTickerMessage",
        [this](ServerWrapper caller, void* params, std::string eventName) {
            OnStatTickerMessage(params);
        }
    );
}


// Ticker message detection
void SoundBoardPlugin::OnStatTickerMessage(void* params)
{
    struct StatTickerParams {
        uintptr_t Receiver;
        uintptr_t Victim;
        uintptr_t StatEvent;
    };

    StatTickerParams* pStruct = (StatTickerParams*)params;
    PriWrapper receiver = PriWrapper(pStruct->Receiver);
    PriWrapper victim = PriWrapper(pStruct->Victim);
    StatEventWrapper statEvent = StatEventWrapper(pStruct->StatEvent);

    if (statEvent.GetEventName() == "Goal") {
        this->PlayASound("goal.wav");
    }
    else if (statEvent.GetEventName() == "Save") {
        this->PlayASound("save.wav");
    }
    else if (statEvent.GetEventName() == "Demolish") {
        this->PlayASound("demolition.wav");
    }
    else if (statEvent.GetEventName() == "MVP") {
        this->PlayASound("mvp.wav");
    }
    else if (statEvent.GetEventName() == "AerialGoal") {
        this->PlayASound("aerial_goal.wav");
    }
    else if (statEvent.GetEventName() == "EpicSave") {
        this->PlayASound("epic_save.wav");
    }
}

// SoundBoard feature
void SoundBoardPlugin::PlayASound(std::string name)
{
    std::wstring wName(name.begin(), name.end());

    wchar_t soundFilePath[MAX_PATH];
    swprintf(soundFilePath, MAX_PATH, L"%ls\\sounds\\%ls", gameWrapper->GetDataFolder().c_str(), wName.c_str());

    if (std::filesystem::exists(soundFilePath))
    {
        PlaySound(soundFilePath, NULL, SND_FILENAME | SND_ASYNC);
    }
}
