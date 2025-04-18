#include "VertexBuffer.hpp"

#include "Core/Log.hpp"

#include <glad/glad.h>

namespace Engine
{
	constexpr unsigned int shader_data_type_to_components_count(const ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:
			case ShaderDataType::Int:
				return 1;

			case ShaderDataType::Float2:
			case ShaderDataType::Int2:
				return 2;

			case ShaderDataType::Float3:
			case ShaderDataType::Int3:
				return 3;

			case ShaderDataType::Float4:
			case ShaderDataType::Int4:
				return 4;
		}

		LOG_ERROR("shader_data_type_to_component_type: unknown ShaderDataType!");
		return 0;
	}

	constexpr size_t shader_data_type_size(const ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
				return sizeof(GL_FLOAT) * shader_data_type_to_components_count(type);

			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
				return sizeof(GL_INT) * shader_data_type_to_components_count(type);
		}

		LOG_ERROR("shader_data_type_size: unknown ShaderDataType!");
		return 0;
	}

	constexpr unsigned int shader_data_type_to_component_type(const ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
				return GL_FLOAT;

			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
				return GL_INT;
		}

		LOG_ERROR("shader_data_type_to_component_type: unknown ShaderDataType!");
		return 0;
	}

	constexpr GLenum usage_to_GLenum(const VertexBuffer::EUsage usage)
	{
		switch (usage)
		{
		case VertexBuffer::EUsage::Static: return GL_STATIC_DRAW;
		case VertexBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
		case VertexBuffer::EUsage::Stream: return GL_STREAM_DRAW;
		}

		LOG_CRITICAL("Unknown VertexBuffer usage!");
		return GL_STREAM_DRAW;
	}

	BufferElement::BufferElement(const ShaderDataType _type)
		: type(_type)
		, component_type(shader_data_type_to_component_type(_type))
		, components_count(shader_data_type_to_components_count(_type))
		, size(shader_data_type_size(_type))
		, offset(0)
	{
	}

	BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
		: m_elements(std::move(elements))
	{
		size_t offset = 0;
		m_stride = 0;

		for (auto& element : m_elements)
		{
			element.offset = offset;
			offset += element.size;
			m_stride += element.size;
		}
	}

	const std::vector<BufferElement>& BufferLayout::get_elements() const
	{
		return m_elements;
	}

	size_t BufferLayout::get_stride() const
	{
		return m_stride;
	}

	VertexBuffer::VertexBuffer(const void* data, const size_t size, BufferLayout buffer_layout, const EUsage usage)
		: m_buffer_layout(std::move(buffer_layout))
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, size, data, usage_to_GLenum(usage));
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& vertexBuffer) noexcept
	{
		m_id = vertexBuffer.m_id;
		vertexBuffer.m_id = 0;
		return *this;
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& vertexBuffer)noexcept
		: m_id(vertexBuffer.m_id),
		m_buffer_layout(vertexBuffer.m_buffer_layout)
	{
		vertexBuffer.m_id = 0;
	}

	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	const BufferLayout& VertexBuffer::get_layout() const
	{
		return m_buffer_layout;
	}
}