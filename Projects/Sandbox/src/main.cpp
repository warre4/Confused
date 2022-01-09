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
	}

	void Cleanup() override
	{
		LOGT(GetName() + " cleaning up");
	}

//------------------------
APPLICATION_END(Sandbox)

// Helper functions
