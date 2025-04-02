#pragma once

#include <string>
#include "Core/Event.hpp"
#include <functional>

struct GLFWwindow;

namespace Engine
{
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(unsigned int width, unsigned int height, std::string title);
		~Window();

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		void on_update();
		unsigned int get_width() const;
		unsigned int get_height() const;

		void set_event_callback(const EventCallbackFn& callback);

	private:
		struct WindowData
		{
			std::string title;
			unsigned int width;
			unsigned int height;
			EventCallbackFn eventCallbackFn;
		};

		int init();
		void shutdown();

		GLFWwindow* m_pWindow = nullptr;
		WindowData m_data;
	};
}
