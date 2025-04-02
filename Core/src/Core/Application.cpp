#include <iostream>
#include "Core/Application.hpp"
#include "Core/Window.hpp"
#include "Core/Log.hpp"

namespace Engine
{
	Application::Application()
	{
        LOG_INFO("Starting application!");
	}

	Application::~Application()
	{
		LOG_INFO("Closing application!");
	}

	int Application::start(unsigned int window_width, unsigned int window_height, const char* window_title)
	{
		m_pWindow = std::make_unique<Window>(window_width, window_height, window_title);

		m_pWindow->set_event_callback(
			[](Event& event)
			{
				LOG_INFO("[EVENT] Changed size to {0}x{1}", event.width, event.height);
			}
		);

		while (true)
		{
			m_pWindow->on_update();
			on_update();
		}
		
		return 0;
	}
}