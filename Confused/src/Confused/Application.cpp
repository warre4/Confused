#include "Macros.h"

#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include <future>

import Confused.Application;
import Confused.Singleton;
import Confused.Time;
import Confused.Renderer;

void Confused::Application::InitializeCore()
{
	Logger::Initialize();
	Renderer::GetInstance().Initialize();

	CORE_LOGT("Engine started");
}

void Confused::Application::CleanupCore()
{
	Renderer::GetInstance().Cleanup();

	CORE_LOGT("Engine exitted");
	CORE_LOGT("Program stopped running, press enter to quit...");
	std::cin.get();
}

void Confused::Application::Run()
{
	// Initialize
	InitializeCore();
	Initialize();

	auto& time = Time::GetInstance();
	auto& renderer = Renderer::GetInstance();

	bool shouldContinue = true;
	auto lastTime = time.GetCurrent();
	auto currentTime = lastTime;
	float deltaTime = 0.f;

	// Inputs
	std::future<void> inputs = std::async(std::launch::async, []() {
		CORE_LOGT("Inputs initialized, press enter to stop the engine...");
		std::cin.get();
		CORE_LOGT("Input received!");
		});

	// Loop
	while (shouldContinue)
	{
		currentTime = time.GetCurrent();
		deltaTime = std::chrono::duration<float, std::micro>(currentTime - lastTime).count() / std::micro::den;
		lastTime = currentTime;

		time.SetElapsed(deltaTime);

		// Update
		if (inputs._Is_ready())
			shouldContinue = false;

		// Render
		renderer.Render();

		//CORE_LOGT("FPS: " + std::to_string(int(1.f / deltaTime)));
	}

	// Cleanup
	Cleanup();
	CleanupCore();
}