#include <iostream>
#include "Core/Application.hpp"
#include "Core/Log.hpp"
#include "Core/Window.hpp"
#include "Core/Event.hpp"

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
		
		m_event_dispatcher.add_event_listener<EventMouseMoved>(
			[](EventMouseMoved& event)
			{	
				LOG_INFO("[MOUSE] Mouse moved to {0}x{1}.", event.m_x, event.m_y);
			}
		);

		m_event_dispatcher.add_event_listener<EventWindowResize>(
			[](EventWindowResize& event)
			{
				LOG_INFO("[WINDOW] Changed size to {0}x{1}.", event.m_width, event.m_height);
			}
		);

		m_event_dispatcher.add_event_listener<EventWindowClose>(
			[this](EventWindowClose& event)
			{
				LOG_INFO("[WINDOW] Close.");
				m_bCloseWindow = true;
			}
		);

		m_pWindow->set_event_callback(
			[this](BaseEvent& event)
			{
				m_event_dispatcher.dispatch(event);
			}
		);

		while (!m_bCloseWindow)
		{
			m_pWindow->on_update();
			on_update();
		}
		
		return 0;
	}
}