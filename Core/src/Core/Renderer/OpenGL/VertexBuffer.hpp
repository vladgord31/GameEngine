#pragma once

#include <vector>

namespace Engine
{
	enum class ShaderDataType
	{
		Float,
		Float2,
		Float3,
		Float4,

		Int,
		Int2,
		Int3,
		Int4,
	};

	struct BufferElement
	{
		ShaderDataType type;
		uint32_t component_type;
		size_t components_count;
		size_t size;
		size_t offset;

		BufferElement(const ShaderDataType type);
	};

	class BufferLayout
	{
	public:
		BufferLayout(std::initializer_list<BufferElement> elements);
		const std::vector<BufferElement>& get_elements() const;
		size_t get_stride() const;

	private:
		std::vector<BufferElement> m_elements;
		size_t m_stride = 0;
	};

	class VertexBuffer
	{
	public:
		enum class EUsage
		{
			Static,
			Dynamic,
			Stream,
		};

		VertexBuffer(const void* data, const size_t size, BufferLayout buffer_layout, EUsage usage = VertexBuffer::EUsage::Static);
		VertexBuffer& operator=(VertexBuffer&&) noexcept;
		VertexBuffer(VertexBuffer&&) noexcept;
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		void bind() const;
		static void unbind();
		const BufferLayout& get_layout() const;

	private:
		unsigned int m_id = 0;
		BufferLayout m_buffer_layout;
	};
}