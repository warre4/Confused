module;
#include "General.h"

#include <unordered_set>
#include <algorithm>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
export module Confused.WindowManager;

import Confused.Window;

namespace Confused
{
	export class CONFUSED_API WindowManager final
	{
	public:
		static Window* CreateWin(const WindowProps& props = WindowProps())
		{
			// Initialize
			if(GetNrActiveWindows() == 0)
				glfwInit();

			// Create Window
			Window* pWindow = new Window(props);
			m_pWindows.insert(pWindow);

			LOGT(std::to_string(GetNrActiveWindows()) + " windows open.");

			return pWindow;
		}
		static void Cleanup()
		{
			if (GetNrActiveWindows() > 0)
			{
				for (Window* pWindow : m_pWindows)
					delete pWindow;

				m_pWindows.clear();

				LOGT("All open windows closed.");
				LOGT(std::to_string(GetNrActiveWindows()) + " windows open.");
			}
		}

		// TEMPORARY RETURN VALUE: returns what Window was closed this frame (nullptr if none)
		static Window* Update()
		{
			Window* returnValue = nullptr;

			for (auto it = m_pWindows.begin(); it != m_pWindows.end(); )
			{
				Window* element = *it;

				// Update Windows
				element->Update();

				// Check if they should close
				if (element->GetShouldClose())
				{
					// TODO: event window closed (prevent missing references)
					// Temporary fix...
					if (returnValue == nullptr)
						returnValue = element;
					else
						EXCEPTION("Temp fix to pass windows detected multiple windows closing in the same frame");

					// Delete (and close) window
					delete element;
					it = m_pWindows.erase(it);
				}
				else
				{
					++it;
				}
			}

			if (returnValue)
			{
				uint32_t nrActiveWindows = GetNrActiveWindows();
				LOGT(std::to_string(nrActiveWindows) + " windows open.");

				if (nrActiveWindows == 0)
					glfwTerminate();
			}

			return returnValue;
		}

		[[nodiscard]] inline static uint32_t GetNrActiveWindows() noexcept { return uint32_t(m_pWindows.size()); }

	private:
		static std::unordered_set<Window*> m_pWindows;
	};

	std::unordered_set<Window*> WindowManager::m_pWindows = std::unordered_set<Window*>{};
}