#pragma once

#include <string>

struct GLFWwindow;

namespace Engine
{
	class Window
	{
	public:
		Window(unsigned int width, unsigned int height, std::string title);
		~Window();

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		void on_update();
		unsigned int get_width() const;
		unsigned int get_height() const;

	private:
		int init();
		void shutdown();

		GLFWwindow* m_pWindow;
		std::string m_title;
		unsigned int m_width;
		unsigned int m_height;
	};
}
