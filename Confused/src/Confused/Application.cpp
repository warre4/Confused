#pragma warning( disable : 4251 ) // needs to have dll-interface to be used by clients of class

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
import Confused.WindowManager;
import Confused.Window;
import Confused.Utils;

void Confused::Application::InitializeCore()
{
	// Init only (doesn't require cleanup later)
	Logger::Initialize();
	UTILS.Initialize();

	Window* pMainWindow = WindowManager::CreateWin(WindowProps("Main Window"));
	WindowManager::CreateWin(WindowProps(m_Name, 800, 600));

	//------------------------------------------
	// Initialize
	//------------------------------------------
	RENDERER.Initialize(pMainWindow);

	CORE_LOGT("Engine started");
}

void Confused::Application::CleanupCore()
{
	RENDERER.Cleanup();

	WindowManager::Cleanup();

	CORE_LOGT("Engine exitted");
	CORE_LOGT("Program stopped running, press enter to quit...");
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
	//	CORE_LOGT("Press enter...");
	//	std::cin.get();
	//	CORE_LOGT("Input received!");
	//	});

	// Loop
	while (shouldContinue)
	{
		currentTime = time.GetCurrent();
		deltaTime = std::chrono::duration<float, std::micro>(currentTime - lastTime).count() / std::micro::den;
		lastTime = currentTime;

		time.SetElapsed(deltaTime);

		// Update
		Window* pClosedWindow = WindowManager::Update();
		if (pClosedWindow == renderer.GetWindow())
		{
			shouldContinue = false; // As long as the main window exists, keep going
			continue;
		}

		// Render
		renderer.Render();

		//CORE_LOGT("FPS: " + std::to_string(int(1.f / deltaTime)));
	}

	// Cleanup
	Cleanup();
	CleanupCore();
}