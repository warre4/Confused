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

		std::vector<std::string> vec{"", "Hello", "There!", "", "General", "Kenobi."};
		UTILS.PrintStrings(vec, "Vector of strings", LOGGER);
	}

	void Cleanup() override
	{
		LOGT(GetName() + " cleaning up");
	}

//------------------------
APPLICATION_END(Sandbox)

// Helper functions
