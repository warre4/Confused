#include "Confused.h"

class Sandbox : public Confused::Application
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