#include "Confused.h"

CREATE_APPLICATION(Sandbox, "Sandbox");

void Sandbox::Initialize()
{
	LOGT(GetName() + " initializing");

	// testing enum
	Confused::Layer hudLayer = _CF LAYER_HUD_0;
	uint8_t debugLayerAsInt = _CF LAYER_DEBUG_1;
	LOGI("Layer: " + UTILS.ToString(hudLayer));
	LOGI("Layer: " + UTILS.ToString(Confused::Layer(debugLayerAsInt)));

	std::vector<std::string> vec{ "", "Hello", "There!", "", "General", "Kenobi." };
	UTILS.PrintStrings(vec, "Vector of strings", LOGGER, _LOGLEVEL debug);
}


void Sandbox::Cleanup()
{
	LOGT(GetName() + " cleaning up");
}

// Helper functions
