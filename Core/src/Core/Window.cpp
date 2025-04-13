#include "Core/Window.hpp"
#include "Core/Log.hpp"
#include "Core/Renderer/OpenGL/ShaderProgram.hpp"
#include "Core/Renderer/OpenGL/VertexBuffer.hpp"
#include "Core/Renderer/OpenGL/VertexArray.hpp"
#include "Core/Renderer/OpenGL/IndexBuffer.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace Engine
{
    static bool s_GLFW_initialized = false;

    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f
    };

    GLuint indices[] = {
         0, 1, 2, 3, 2, 1
    };

    const char* vertex_shader =
        "#version 460\n"
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_color;"
        "out vec3 color;"
        "void main() {"
        "   color = vertex_color;"
        "   gl_Position = vec4(vertex_position, 1.0f);"
        "}";

    const char* fragment_shader =
        "#version 460\n"
        "in vec3 color;"
        "out vec4 fragment_color;"
        "void main() {"
        "   fragment_color = vec4(color, 1.0f);"
        "}";

    std::unique_ptr<ShaderProgram> p_shader_program;
    std::unique_ptr<VertexBuffer> p_vertices;
    std::unique_ptr<IndexBuffer> p_index_buffer;
    std::unique_ptr<VertexArray> p_vao;

	Window::Window(unsigned int width, unsigned int height, std::string title)
		: m_data({std::move(title), width, height})
	{
		int returnCode = init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init("#version 460");
        ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
	}

	Window::~Window()
	{
		shutdown();
	}

	int Window::init()
	{   
        LOG_INFO("[WINDOW] Creating window {0} width size {1}x{2}", m_data.title, m_data.width, m_data.height);
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
                LOG_INFO("[WINDOW] New size {0}x{1}", width, height);

                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                data.width = width;
                data.height = height;

                EventWindowResize event(width, height);
                data.eventCallbackFn(event);
            }
        );

        glfwSetWindowCloseCallback(m_pWindow,
            [](GLFWwindow* pWindow)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

                EventWindowClose event;
                data.eventCallbackFn(event);
            }
        );

        glfwSetCursorPosCallback(m_pWindow,
            [](GLFWwindow* pWindow, double x, double y)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

                EventMouseMoved event(x, y);
                data.eventCallbackFn(event);

            }
        );

        glfwSetFramebufferSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height)
            {
                glViewport(0, 0, width, height);
            }
        );

        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if (!p_shader_program->isCompiled())
        {
            return false;
        }

        BufferLayout buffer_layout
        {
            ShaderDataType::Float3,
            ShaderDataType::Float3,
        };

        p_vao = std::make_unique<VertexArray>();
        p_vertices = std::make_unique<VertexBuffer>(vertices, sizeof(vertices), buffer_layout);
        p_index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

        p_vao->add_vertex_buffer(*p_vertices);
        p_vao->set_index_buffer(*p_index_buffer);

        return 0;
	}

	void Window::on_update()
	{
        if (glfwWindowShouldClose(m_pWindow))
        {
            shutdown();
            return;
        }

        glClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(get_width());
        io.DisplaySize.y = static_cast<float>(get_height());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Begin("Change background color");
        ImGui::ColorEdit4("Background color", m_background_color);

        p_shader_program->bind();
        p_vao->bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(p_vao->get_indicies_count()), GL_UNSIGNED_INT, nullptr);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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