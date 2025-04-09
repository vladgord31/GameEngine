#include "VertexArray.hpp"

#include "Core/Log.hpp"

#include <glad/glad.h>

namespace Engine
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_id);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_id);
	}

	VertexArray& VertexArray::operator=(VertexArray && vertexArray) noexcept
	{
		m_id = vertexArray.m_id;
		m_element_count = vertexArray.m_element_count;
		vertexArray.m_id = 0;
		vertexArray.m_element_count = 0;

		return *this;
	}

	VertexArray::VertexArray(VertexArray&& vertexArray)
		: m_id(vertexArray.m_id)
		, m_element_count(vertexArray.m_element_count)
	{
		vertexArray.m_id = 0;
		vertexArray.m_element_count = 0;
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind()
	{
		glBindVertexArray(0);
	}

	void VertexArray::add_buffer(const VertexBuffer& vertexBuffer)
	{
		bind();
		vertexBuffer.bind();

		glEnableVertexAttribArray(m_element_count);
		glVertexAttribPointer(m_element_count, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(0));
		++m_element_count;

		glEnableVertexAttribArray(m_element_count);
		glVertexAttribPointer(m_element_count, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(3 * sizeof(float)));
		++m_element_count;
	}
}