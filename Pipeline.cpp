#include "Pipeline.h"

Pipeline::Pipeline(D3D11Context* context, Material* material, IDrawer* drawer)
	: m_context(context)
	, m_material(material)
	, m_drawer(drawer) {

	const D3D11_INPUT_ELEMENT_DESC inputLayout[4] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		
	};

	vertex.data.resize(3);
	vertex.data[0].pos = Eigen::Vector3<float>(0.0f, 0.5f, 0.5f);
	vertex.data[1].pos = Eigen::Vector3<float>(0.5f, -0.5f, 0.5f);
	vertex.data[2].pos = Eigen::Vector3<float>(-0.5f, -0.5f, 0.5f);

	vertex.data[0].color = Eigen::Vector4<float>(0.0f, 1.0f, 0.0f, 1.0f);
	vertex.data[1].color = Eigen::Vector4<float>(0.0f, 0.0f, 1.0f, 1.0f);
	vertex.data[2].color = Eigen::Vector4<float>(1.0f, 0.0f, 0.0f, 1.0f);

	vertex.data[0].uv = Eigen::Vector2<float>(0.0f, 0.0f);
	vertex.data[1].uv = Eigen::Vector2<float>(0.0f, 0.0f);
	vertex.data[2].uv = Eigen::Vector2<float>(0.0f, 0.0f);
		
	vertex.data[0].normal = Eigen::Vector3<float>(0.0f, 0.0f, 0.0f);
	vertex.data[1].normal = Eigen::Vector3<float>(0.0f, 0.0f, 0.0f);
	vertex.data[2].normal = Eigen::Vector3<float>(0.0f, 0.0f, 0.0f);


	vertex.index = { 0,1,2 };
	
	m_context->m_Device->CreateInputLayout(inputLayout, ARRAYSIZE(inputLayout),
		material->getVSShader()->getBlob()->GetBufferPointer(), 
		material->getVSShader()->getBlob()->GetBufferSize(), &pInputLayout);

	CD3D11_BUFFER_DESC bufferDesc(0,
		D3D11_BIND_VERTEX_BUFFER,
		D3D11_USAGE_DEFAULT,
		0);

	D3D11_SUBRESOURCE_DATA initData{ nullptr, 0, 0 };
	bufferDesc.ByteWidth = vertex.dataSize();
	initData.pSysMem = vertex.ptrData();
	m_context->m_Device->CreateBuffer(&bufferDesc, &initData, pVertexBuffers.GetAddressOf());

	bufferDesc.ByteWidth = vertex.indexSize();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	initData.pSysMem = vertex.ptrIndex();
	m_context->m_Device->CreateBuffer(&bufferDesc, &initData, pIndexBuffer.GetAddressOf());
}

void Pipeline::IA() {
	UINT size = vertex.itemSize();
	UINT offset = 0;
	m_context->m_DeviceContext->IASetVertexBuffers(0, 1, pVertexBuffers.GetAddressOf(), &size, &offset);
	m_context->m_DeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_context->m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_context->m_DeviceContext->IASetInputLayout(pInputLayout);
}

void Pipeline::VS() {
	m_context->m_DeviceContext->VSSetShader(m_material->getVSShader()->getVS().Get(), 0, 0);
	Material::CBuffers buffers = m_material->getVSShader()->getUniform();
	for (auto buffer : buffers) {
		CBufferData bufferData = buffer.second;
		bufferData.UpdateBuffer(m_context->m_DeviceContext.Get());
		m_context->m_DeviceContext->VSSetConstantBuffers(bufferData.getSlot(), 1, bufferData.getBuffer().GetAddressOf());
	}
}

void Pipeline::Rasterizer() {

}

void Pipeline::PS() {
	m_context->m_DeviceContext->PSSetShader(m_material->getPSShader()->getPS().Get(), 0, 0);
	Material::CBuffers buffers = m_material->getPSShader()->getUniform();
	for (auto buffer : buffers) {
		CBufferData bufferData = buffer.second;
		bufferData.UpdateBuffer(m_context->m_DeviceContext.Get());
		m_context->m_DeviceContext->PSSetConstantBuffers(bufferData.getSlot(), 1, bufferData.getBuffer().GetAddressOf());
	}
}

void Pipeline::OM() {
	m_context->m_DeviceContext->OMSetRenderTargets(1, m_drawer->getRenderTarget().GetAddressOf(), nullptr);
}

void Pipeline::DrawIndex() {
	m_context->m_DeviceContext->DrawIndexed(3, 0, 0);
}