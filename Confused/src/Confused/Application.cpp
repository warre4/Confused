#pragma warning( disable : 4251 ) // needs to have dll-interface to be used by clients of class

#include "General.h"

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
import Confused.WindowManager;
import Confused.Window;
import Confused.Utils;

void Confused::Application::InitializeCore()
{
	// Init only (doesn't require cleanup later)
	Logger::Initialize(m_Name);

	Window* pMainWindow = WindowManager::CreateWin(WindowProps("Main Window"));
	WindowManager::CreateWin(WindowProps(m_Name, 800, 600));

	//------------------------------------------
	// Initialize
	//------------------------------------------
	RENDERER.Initialize(pMainWindow);

	LOGT("Engine started");
}

void Confused::Application::CleanupCore()
{
	RENDERER.Cleanup();

	WindowManager::Cleanup();

	LOGT("Engine exitted");
	LOGT("Program stopped running, press enter to quit...");
	std::cin.get();
}

void Confused::Application::Run()
{
	// Initialize
	InitializeCore();
	Initialize();

	auto& time = TIME;
	auto& renderer = RENDERER;

	bool shouldContinue = true;
	auto lastTime = time.GetCurrent();
	auto currentTime = lastTime;
	float deltaTime = 0.f;

	//// Inputs
	//std::future<void> inputs = std::async(std::launch::async, []() {
	//	LOGT("Press enter...");
	//	std::cin.get();
	//	LOGT("Input received!");
	//	});

	float totalTime = 0;

	// Loop
	while (shouldContinue)
	{
		currentTime = time.GetCurrent();
		deltaTime = _CHRONO duration<float, _CHRONO seconds::period>(currentTime - lastTime).count();
		time.SetElapsed(deltaTime);

		totalTime += UTILS.DurationInMilliseconds(lastTime, currentTime);

		lastTime = currentTime;

		// Update
		Window* pClosedWindow = WindowManager::Update();
		if (pClosedWindow == renderer.GetWindow())
		{
			shouldContinue = false; // As long as the main window exists, keep going
			continue;
		}

		// Render
		renderer.Render();

		//LOGT("FPS: " + std::to_string(int(1.f / deltaTime)));
		LOGT("Total runtime: " + std::to_string((int)totalTime) + " ms");
	}

	// Cleanup
	Cleanup();
	CleanupCore();
}