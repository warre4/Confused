#include "Logger.h"
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include <future>

import Confused.Application;
import Confused.Singleton;
import Confused.Time;

void Confused::Application::InitializeCore()
{
	Confused::Logger::Initialize();
	CF_CORE_INFO("Initializing Confused Engine");


}

void Confused::Application::CleanupCore()
{
	CF_CORE_INFO("Cleaning up Confused");



	CF_CORE_TRACE("Program stopped running, press any button to quit...");
	std::cin.get();
}

void Confused::Application::Run()
{
	// Initialize
	InitializeCore();
	Initialize();

	auto& time = Time::GetInstance();

	// Loop
	bool shouldContinue = true;
	auto lastTime = time.GetCurrent();
	auto currentTime = lastTime;
	float deltaTime = 0.f;

	// Inputs
	std::future<void> inputs = std::async(std::launch::async, []() {
		CF_CORE_INFO("Inputs initialized, press anything to stop the engine...");
		std::cin.get();
		CF_CORE_INFO("Input received! Confused Engine exitting...");
		});

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


		CF_CORE_TRACE("FPS: " + std::to_string(int(1.f / deltaTime)));
	}

	// Cleanup
	Cleanup();
	CleanupCore();
}