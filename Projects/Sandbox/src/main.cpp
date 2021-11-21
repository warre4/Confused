#include "Confused.h"

class Sandbox final : public Confused::Application
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}
};

Confused::Application* Confused::CreateApplication()
{
	return new Sandbox();
}