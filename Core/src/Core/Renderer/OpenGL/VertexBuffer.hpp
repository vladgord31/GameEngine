#pragma once

namespace Engine
{
	class VertexBuffer
	{
	public:
		enum class EUsage
		{
			Static,
			Dynamic,
			Stream,
		};

		VertexBuffer(const void* data, const size_t size, EUsage usage = VertexBuffer::EUsage::Static);
		VertexBuffer& operator=(VertexBuffer&&) noexcept;
		VertexBuffer(VertexBuffer&&) noexcept;
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		void bind() const;
		static void unbind();

	private:
		unsigned int m_id = 0;
	};
}