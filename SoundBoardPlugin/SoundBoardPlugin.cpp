#include "pch.h"
#include "SoundBoardPlugin.h"
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "Winmm.lib")
#include <mmsystem.h>
#include <string>
#include <Lmcons.h>
#include <filesystem>
#include <unordered_map>

BAKKESMOD_PLUGIN(SoundBoardPlugin, "A soundboard plugin who plays custom sounds when game events", "1.4.0", PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;
std::unordered_map<std::string, std::chrono::time_point<std::chrono::steady_clock>> lastSoundTimes;

// Load function
void SoundBoardPlugin::onLoad()
{
    _globalCvarManager = cvarManager;
    cvarManager->registerCvar("soundboard_volume", "1.0", "Volume for soundboard (0.0 - 1.0)", true, true, 0.0f, true, 1.0f);

    // Register CVars
    for (const auto& soundFile : soundFiles) {
        std::string enabledCvarName = "soundboard_enabled_" + soundFile;
        std::string cooldownCvarName = "soundboard_cooldown_" + soundFile;

        // Register enabled and cooldown CVar (default to 500 ms)
        cvarManager->registerCvar(enabledCvarName, "1", "Enable sound for event: " + soundFile);
        cvarManager->registerCvar(cooldownCvarName, "500", "Cooldown for sound event: " + soundFile);
    }

    this->LoadHooks();

    LOG("Soundboard plugin loaded!");
}

void SoundBoardPlugin::onUnload() {
    cvarManager->executeCommand("writeconfig", false);
}

void SoundBoardPlugin::PlaySoundIfEnabled(const std::string& cvarName, const std::string& soundFile)
{
    if (cvarManager->getCvar(cvarName).getBoolValue()) {
        this->PlayASound(soundFile);
    }
}

// Hooks listener
void SoundBoardPlugin::LoadHooks()
{
    struct SoundEvent {
        std::string eventName;
        std::string soundFile;
    };

    std::vector<SoundEvent> soundEvents = {
        {"Function TAGame.GoalCrossbarVolumeManager_TA.CalculateHitNormal", "crossbar.wav"},
        {"Function TAGame.Car_TA.OnHitBall", "car_hit_ball.wav"},
        {"Function CarComponent_Boost_TA.Active.BeginState", "boost.wav"},
        {"Function TAGame.Car_TA.OnJumpPressed", "jump.wav"},
        {"Function TAGame.Car_TA.BumpCar", "bump.wav"}
    };

    for (const auto& soundEvent : soundEvents) {
        gameWrapper->HookEvent(soundEvent.eventName, [this, soundEvent](std::string event) {
            auto now = std::chrono::steady_clock::now();
            auto lastTime = lastSoundTimes[soundEvent.soundFile];

            // Read CVar values
            bool isEnabled = cvarManager->getCvar("soundboard_enabled_" + soundEvent.soundFile).getBoolValue();
            int cooldownMs = cvarManager->getCvar("soundboard_cooldown_" + soundEvent.soundFile).getIntValue();
            std::chrono::milliseconds cooldown(cooldownMs);

            if (isEnabled && now - lastTime >= cooldown) {
                lastSoundTimes[soundEvent.soundFile] = now;
                PlayASound(soundEvent.soundFile);
            }
            });
    }

    gameWrapper->HookEventWithCallerPost<ServerWrapper>(
        "Function TAGame.GFxHUD_TA.HandleStatTickerMessage",
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
    StatEventWrapper statEvent = StatEventWrapper(pStruct->StatEvent);

    std::unordered_map<std::string, std::string> statSoundMap = {
        {"Goal", "goal.wav"},
        {"Save", "save.wav"},
        {"Demolish", "demolition.wav"},
        {"AerialGoal", "aerial_goal.wav"},
        {"EpicSave", "epic_save.wav"},
        {"MVP", "mvp.wav"}
    };

    auto it = statSoundMap.find(statEvent.GetEventName());
    if (it != statSoundMap.end()) {
        std::string soundFile = it->second;
        std::string enableCvar = "soundboard_enabled_" + soundFile;
        std::string cooldownCvar = "soundboard_cooldown_" + soundFile;

        auto now = std::chrono::steady_clock::now();
        auto lastTime = lastSoundTimes[soundFile];

        int cooldownMs = cvarManager->getCvar(cooldownCvar).getIntValue();
        std::chrono::milliseconds cooldown(cooldownMs);

        if (cvarManager->getCvar(enableCvar).getBoolValue() && now - lastTime >= cooldown) {
            lastSoundTimes[soundFile] = now;
            PlayASound(soundFile);
        }
    }
}

// SoundBoard feature
void SoundBoardPlugin::PlayASound(std::string name)
{
    std::wstring wName(name.begin(), name.end());

    wchar_t soundFilePath[MAX_PATH];
    swprintf(soundFilePath, MAX_PATH, L"%ls\\sounds\\%ls", gameWrapper->GetDataFolder().c_str(), wName.c_str());

    float volume = cvarManager->getCvar("soundboard_volume").getFloatValue();
    // Volume range is 0 to 0xFFFF (16-bit)
    DWORD volumeSetting = static_cast<DWORD>(volume * 0xFFFF);  
    waveOutSetVolume(0, MAKELONG(volumeSetting, volumeSetting));

    if (std::filesystem::exists(soundFilePath))
    {
        PlaySound(soundFilePath, NULL, SND_FILENAME | SND_ASYNC);
    }
}
