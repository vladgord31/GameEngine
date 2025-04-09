#pragma once
#include "VertexBuffer.hpp"

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

		void add_buffer(const VertexBuffer& vertexBuffer);

		void bind() const;
		void unbind();

	private:
		unsigned int m_id = 0;
		unsigned int m_element_count = 0;
	};
}