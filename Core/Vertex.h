#pragma once
#include "D3D11Context.h"
#include "Eigen/Core"

template<class Vertex>
class VertexBuffer {
public:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	const Vertex* vertexData() const { return vertices.data(); }
	const size_t vertexSize() const { return sizeof(Vertex); }
	const size_t vertexCount() const { return vertices.size() * sizeof(Vertex); }
	
	const uint32_t* indexData() const { return indices.data(); }
	const size_t indexSize() const { return indices.size(); }
	const size_t indexCount() const { return indices.size() * sizeof(uint32_t); }
	
	virtual const D3D11_INPUT_ELEMENT_DESC* layout() const { return nullptr; }
	virtual const unsigned int layoutCount() const { return 0; }
};

class AnyVertexBuffer {
public:
	template<class VertexType>
	AnyVertexBuffer(std::shared_ptr<VertexBuffer<VertexType>> buffer)
		: m_self(std::make_shared<Model<VertexType>>(buffer)) { }

	const void* vertexData()  const { return m_self->vertexData(); }
	const size_t vertexSize() const  { return m_self->vertexSize(); }
	const size_t vertexCount() const  { return m_self->vertexCount(); }

	const uint32_t* indexData()  const { return m_self->indexData(); }
	const size_t indexSize() const { return m_self->indexSize(); }
	const size_t indexCount() const  { return m_self->indexCount(); }

	const D3D11_INPUT_ELEMENT_DESC* layout() const  { return m_self->layout(); }
	const unsigned int layoutCount() const  { return m_self->layoutCount(); }

	template<class VertexType>
	std::shared_ptr<VertexBuffer<VertexType>> vertex() {
		auto model = std::dynamic_pointer_cast<Model<VertexType>>(m_self);
		if (model) {
			return model->m_buffer;
		}
		return nullptr;
	}

private:
	struct Concept {
		virtual ~Concept() = default;

		virtual const void* vertexData() const = 0;
		virtual const size_t vertexSize() const = 0;
		virtual const size_t vertexCount() const = 0;

		virtual const uint32_t* indexData() const = 0;
		virtual const size_t indexSize() const = 0;
		virtual const size_t indexCount() const = 0;

		virtual const D3D11_INPUT_ELEMENT_DESC* layout() const = 0;
		virtual const unsigned int layoutCount() const = 0;

	};

	template<typename VertexType>
	struct Model : Concept {
		std::shared_ptr<VertexBuffer<VertexType>> m_buffer;

		Model(std::shared_ptr<VertexBuffer<VertexType>> buf) : m_buffer(buf) {}

		const void* vertexData() const override { return m_buffer->vertexData(); }
		const size_t vertexSize() const override { return m_buffer->vertexSize(); }
		const size_t vertexCount() const override { return m_buffer->vertexCount(); }

		const uint32_t* indexData() const override { return m_buffer->indexData(); }
		const size_t indexSize() const override { return m_buffer->indexSize(); }
		const size_t indexCount() const override { return m_buffer->indexCount(); }

		const D3D11_INPUT_ELEMENT_DESC* layout() const override { return m_buffer->layout(); }
		const unsigned int layoutCount() const override { return m_buffer->layoutCount(); }


	};


	std::shared_ptr<Concept> m_self;
};

