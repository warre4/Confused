#include "Confused.h"

APPLICATION_BEGIN(Sandbox)
//------------------------

public:
	Sandbox()
		: Confused::Application("Sandbox")
	{

	}
	~Sandbox()
	{

	}

private:
	void Initialize() override
	{
		LOGT(GetName() + " initializing");

		// testing enum
		Confused::Layer hudLayer = _CONFUSED LAYER_HUD_0;
		uint8_t debugLayerAsInt = _CONFUSED LAYER_DEBUG_1;
		LOGI("Layer: " + UTILS.ToString(hudLayer));
		LOGI("Layer: " + UTILS.ToString(Confused::Layer(debugLayerAsInt)));

		//std::vector<std::string> vec{"", "Hello", "There!", "", "General", "Kenobi."};
		//UTILS.PrintStrings(vec, "Vector of strings", LOGGER, _LOGLEVEL debug);
	}

	void Cleanup() override
	{
		LOGT(GetName() + " cleaning up");
	}

//------------------------
APPLICATION_END(Sandbox)

// Helper functions
