#pragma once

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "bakkesmod/wrappers/GameObject/Stats/StatEventWrapper.h"
#include "version.h"
#include <chrono>

constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

class SoundBoardPlugin : public BakkesMod::Plugin::BakkesModPlugin, public SettingsWindowBase
{
	void onLoad() override;
	void onUnload() override;
	void LoadHooks();
	void PlayASound(std::string name);
	void OnStatTickerMessage(void* params);
	void PlaySoundIfEnabled(const std::string& cvarName, const std::string& soundFile);

	public:
		void RenderSettings() override;

		std::vector<std::string> soundFiles = {
		   "crossbar.wav","car_hit_ball.wav","boost.wav","jump.wav","bump.wav",
		   "goal.wav","save.wav","demolition.wav","aerial_goal.wav","epic_save.wav","mvp.wav"
		};
};