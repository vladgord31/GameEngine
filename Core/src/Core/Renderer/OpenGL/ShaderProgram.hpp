#pragma once

namespace Engine
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src);
		~ShaderProgram();

		ShaderProgram(ShaderProgram&&);
		ShaderProgram& operator=(ShaderProgram&&);
		
		ShaderProgram() = delete;
		ShaderProgram operator=(const ShaderProgram&) = delete;

		void bind() const;
		static void unbind();
		bool isCompiled() const;

	private:
		bool m_isCompiled = false;
		unsigned int m_id;
	};
}