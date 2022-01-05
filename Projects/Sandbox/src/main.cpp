#include "Confused.h"

#include <iostream>

APPLICATION_BEGIN(Sandbox)
//------------------------

public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}

	void Initialize() override
	{
		CF_INFO("Initializing Sandbox");
	}

	void Cleanup() override
	{
		CF_INFO("Cleaning up Sandbox");
	}

private:


//------------------------
APPLICATION_END(Sandbox)