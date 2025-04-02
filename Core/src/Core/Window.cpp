#include "Core/Window.hpp"
#include "Core/Log.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Engine
{
    static bool s_GLFW_initialized = false;

	Window::Window(unsigned int width, unsigned int height, std::string title)
		: m_data({std::move(title), height, width})
	{
		int returnCode = init();
	}

	Window::~Window()
	{
		shutdown();
	}

	int Window::init()
	{   
        LOG_INFO("Creating window {0} width size {1}x{2}", m_data.title, m_data.width, m_data.height);
        if (!s_GLFW_initialized)
        {
            if (!glfwInit())
            {
                LOG_CRITICAL("Can't initalize GLFW!");
                return -1;
            }
            s_GLFW_initialized = true;
        }
        
        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        if (!m_pWindow)
        {
            LOG_CRITICAL("Can't create window {0} width size {1}x{2}", m_data.title, m_data.width, m_data.height);
            shutdown();
            return -2;
        }

        glfwMakeContextCurrent(m_pWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_CRITICAL("Can't initialize GLAD!");
            shutdown();
            return -3;
        }

        glfwSetWindowUserPointer(m_pWindow, &m_data);

        glfwSetWindowSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height)
            {
                LOG_INFO("New size {0}x{1}", width, height);

                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                data.width = width;
                data.height = height;

                Event event;
                event.width = width;
                event.height = height;
                data.eventCallbackFn(event);
            }
        );

        return 0;
	}

	void Window::on_update()
	{
        if (glfwWindowShouldClose(m_pWindow))
        {
            shutdown();
            return;
        }

        glClearColor(1, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
	}

    void Window::set_event_callback(const EventCallbackFn& callback)
    {
        m_data.eventCallbackFn = callback;
    }

    void Window::shutdown()
    {
        if (m_pWindow)
        {
            glfwDestroyWindow(m_pWindow);
            m_pWindow = nullptr;
        }
        if (s_GLFW_initialized)
        {
            glfwTerminate();
            s_GLFW_initialized = false;
        }
    }

	unsigned int Window::get_height() const
	{
		return m_data.height;
	}

	unsigned int Window::get_width() const
	{
		return m_data.width;
	}
}