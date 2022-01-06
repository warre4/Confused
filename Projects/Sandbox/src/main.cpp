#include "Confused.h"

APPLICATION_BEGIN(Sandbox)
//------------------------

public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}

private:
	void Initialize() override
	{
		LOGT("Sandbox initializing");
	}

	void Cleanup() override
	{
		LOGT("Sandbox cleaning up");
	}

//------------------------
APPLICATION_END(Sandbox)

// Helper functions
