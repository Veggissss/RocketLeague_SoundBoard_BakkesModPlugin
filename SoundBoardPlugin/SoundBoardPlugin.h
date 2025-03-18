#pragma once

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "bakkesmod/wrappers/GameObject/Stats/StatEventWrapper.h"
#include "version.h"
#include <chrono>

constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

class SoundBoardPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
	void onLoad() override;
	void LoadHooks();
	void PlayASound(std::string name);
	void OnStatTickerMessage(void* params);
};