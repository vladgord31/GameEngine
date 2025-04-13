#pragma once

#include <string>
#include <functional>

#include "Core/Event.hpp"

struct GLFWwindow;

namespace Engine
{
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(BaseEvent&)>;

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
		float m_background_color[4] = { 1.f, 0.f, 0.f, 1.f };
	};
}
