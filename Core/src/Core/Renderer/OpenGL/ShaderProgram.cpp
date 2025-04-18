#include "ShaderProgram.hpp"

#include <glad/glad.h>

#include "Core/Log.hpp"

namespace Engine
{
	bool create_shader(const char* source, const GLenum shader_type, GLuint& shader_id)
	{
		shader_id = glCreateShader(shader_type);

		glShaderSource(shader_id, 1, &source, nullptr);

		glCompileShader(shader_id);

		GLint success;

		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLchar info_log[1024];

			glGetShaderInfoLog(shader_id, 1024, nullptr, info_log);

			LOG_CRITICAL("");

			return false;
		}

		return true;
		
	}

	ShaderProgram::ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src)
	{
		GLuint vertex_shader_id = 0;

		if (!create_shader(vertex_shader_src, GL_VERTEX_SHADER, vertex_shader_id))
		{
			LOG_CRITICAL("");

			glDeleteShader(vertex_shader_id);
			return;
		}

		GLuint fragment_shader_id = 0;
		if (!create_shader(fragment_shader_src, GL_FRAGMENT_SHADER, fragment_shader_id))
		{
			LOG_CRITICAL("");

			glDeleteShader(fragment_shader_id);
			return;
		}
		m_id = glCreateProgram();
		glAttachShader(m_id, vertex_shader_id);
		glAttachShader(m_id, fragment_shader_id);
		glLinkProgram(m_id);

		GLint success;
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLchar info_log[1024];
			glGetShaderInfoLog(m_id, 1024, nullptr, info_log);

			LOG_CRITICAL("");

			glDeleteProgram(m_id);
			m_id = 0;

			glDeleteShader(vertex_shader_id);
			glDeleteShader(fragment_shader_id);

			return;
		}
		else
		{
			m_isCompiled = true;
		}	

		glDetachShader(m_id, vertex_shader_id);
		glDetachShader(m_id, fragment_shader_id);

		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);
	}

	ShaderProgram::~ShaderProgram()
	{
		if (m_id != 0)
		{
			glDeleteProgram(m_id);
		}
	}

	void ShaderProgram::bind() const
	{
		glUseProgram(m_id);
	}

	void ShaderProgram::unbind()
	{
		glUseProgram(0);
	}

	bool ShaderProgram::isCompiled() const
	{
		return m_isCompiled;
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram)
	{
		if (this != &shaderProgram)
		{
			if (m_id != 0)
			{
				glDeleteProgram(m_id);
			}

			m_id = shaderProgram.m_id;
			m_isCompiled = shaderProgram.m_isCompiled;

			shaderProgram.m_id = 0;
			shaderProgram.m_isCompiled = false;
		}
		return *this;
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram)
	{
		m_id = shaderProgram.m_id;
		m_isCompiled = shaderProgram.m_isCompiled;

		m_id = 0;
		m_isCompiled = false;
	}
}