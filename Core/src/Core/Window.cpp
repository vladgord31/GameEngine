#include "Core/Window.hpp"
#include "Core/Log.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Engine
{
    static bool s_GLFW_initialized = false;

	Window::Window(unsigned int width, unsigned int height, std::string title)
		: m_title(std::move(title))
		, m_width(width)
		, m_height(height)
	{
		int returnCode = init();
	}

	Window::~Window()
	{
		shutdown();
	}

	int Window::init()
	{   
        LOG_INFO("Creating window {0} width size {1}x{2}", m_width, m_width, m_height);
        if (!s_GLFW_initialized)
        {
            if (!glfwInit())
            {
                LOG_CRITICAL("Can't initalized GLFW!");
                return -1;
            }
            s_GLFW_initialized = true;
        }
        
        m_pWindow = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
        if (!m_pWindow)
        {
            LOG_CRITICAL("Can't create window {0} width size {1}x{2}", m_width, m_width, m_height);
            shutdown();
            return -2;
        }

        glfwMakeContextCurrent(m_pWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_CRITICAL("Can't initialized GLAD!");
            return -1;
        }

        return 0;
	}

	void Window::on_update()
	{
        glClearColor(1, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
	}

    void Window::shutdown()
    {
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

	unsigned int Window::get_height() const
	{
		return m_height;
	}

	unsigned int Window::get_width() const
	{
		return m_width;
	}
}