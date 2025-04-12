#pragma once
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace Engine
{
	class VertexArray
	{
	public:
		VertexArray();
		VertexArray(VertexArray&&);
		VertexArray& operator=(VertexArray&&) noexcept;
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		void add_vertex_buffer(const VertexBuffer& vertex_buffer);
		void set_index_buffer(const IndexBuffer& index_buffer);

		void bind() const;
		static void unbind();
		size_t get_indicies_count() const;

	private:
		unsigned int m_id = 0;
		unsigned int m_element_count = 0;
		size_t m_indices_count = 0;
	};
}