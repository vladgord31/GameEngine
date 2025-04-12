#pragma once
#include "VertexBuffer.hpp"

namespace Engine
{
	class IndexBuffer
	{
	public:
		IndexBuffer(const void* data, size_t count, const VertexBuffer::EUsage = VertexBuffer::EUsage::Static);
		~IndexBuffer();
		IndexBuffer(IndexBuffer&& index_buffer) noexcept;
		IndexBuffer& operator=(IndexBuffer&& index_buffer) noexcept;

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		void bind() const;
		static void unbind();
		size_t get_count() const;

	private:
		unsigned int m_id;
		size_t m_count;
	};
}