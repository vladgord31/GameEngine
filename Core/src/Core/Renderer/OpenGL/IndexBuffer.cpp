#include "IndexBuffer.hpp"
#include "glad/glad.h"
#include "Core/Log.hpp"

namespace Engine
{
	constexpr GLenum usage_to_GLenum(const VertexBuffer::EUsage usage)
	{
		switch (usage)
		{
			case VertexBuffer::EUsage::Static: return GL_STATIC_DRAW;
			case VertexBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
			case VertexBuffer::EUsage::Stream: return GL_STREAM_DRAW;
			default: 
				LOG_ERROR("Unknown VertexBuffer usage");
				return GL_STREAM_DRAW;
		}
	}

	IndexBuffer::IndexBuffer(const void* data, size_t count, const VertexBuffer::EUsage usage)
		: m_count(count)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, usage_to_GLenum(usage));
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& index_buffer) noexcept
		: m_id(index_buffer.m_id)
		, m_count(index_buffer.m_count)
	{
		index_buffer.m_id = 0;
		index_buffer.m_count = 0;
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& index_buffer) noexcept
	{
		m_id = index_buffer.m_id;
		m_count = index_buffer.m_count;
		m_id = 0;
		m_count = 0;
		return *this;
	}

	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IndexBuffer::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	size_t IndexBuffer::get_count() const
	{
		return m_count;
	}
}