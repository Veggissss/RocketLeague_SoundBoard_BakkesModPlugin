#include "pch.h"
#include "SoundBoardPlugin.h"

void SoundBoardPlugin::RenderSettings() {
	ImGui::Text("Soundboard Settings");
	ImGui::NewLine();

	CVarWrapper volumeCvar = cvarManager->getCvar("soundboard_volume");
	float volume = volumeCvar.getFloatValue();
	if (ImGui::SliderFloat("Sound volume (Application)", &volume, 0.0f, 1.0f)) {
		volumeCvar.setValue(volume);
	}
	ImGui::NewLine();

	for (const auto& soundFile : soundFiles) {
		std::string enableCvarName = "soundboard_enabled_" + soundFile;
		std::string cooldownCvarName = "soundboard_cooldown_" + soundFile;

		CVarWrapper soundEnabledCvar = cvarManager->getCvar(enableCvarName);
		bool isEnabled = soundEnabledCvar.getBoolValue();
		if (ImGui::Checkbox(enableCvarName.c_str(), &isEnabled)) {
			soundEnabledCvar.setValue(isEnabled);
		}

		int cooldown = cvarManager->getCvar(cooldownCvarName).getIntValue();
		if (ImGui::SliderInt(("Cooldown (ms): " + soundFile).c_str(), &cooldown, 0, 10000)) {
			cvarManager->getCvar(cooldownCvarName).setValue(cooldown);
		}
	}
}