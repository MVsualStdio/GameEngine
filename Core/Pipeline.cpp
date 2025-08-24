#include "Pipeline.h"
#include "MeshFilter.h"

Pipeline::Pipeline(D3D11Context* context, Material* material, IDrawer* drawer, AnyVertexBuffer* vertex)
	: m_context(context)
	, m_material(material)
	, m_drawer(drawer)
	, m_vertex(vertex) { 

	m_context->m_Device->CreateInputLayout(vertex->layout(), vertex->layoutCount(),
		m_material->getVSShader()->getBlob()->GetBufferPointer(),
		m_material->getVSShader()->getBlob()->GetBufferSize(), &pInputLayout);

	CD3D11_BUFFER_DESC bufferDesc(0,
		D3D11_BIND_VERTEX_BUFFER,
		D3D11_USAGE_DEFAULT,
		0);

	D3D11_SUBRESOURCE_DATA initData{ nullptr, 0, 0 };
	bufferDesc.ByteWidth = vertex->vertexCount();
	initData.pSysMem = vertex->vertexData();
	m_context->m_Device->CreateBuffer(&bufferDesc, &initData, pVertexBuffers.GetAddressOf());

	bufferDesc.ByteWidth = vertex->indexCount();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	initData.pSysMem = vertex->indexData();
	m_context->m_Device->CreateBuffer(&bufferDesc, &initData, pIndexBuffer.GetAddressOf());
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	m_context->m_Device->CreateSamplerState(&sampDesc, pSampleState.GetAddressOf());
}


void Pipeline::IA() {
	UINT size = m_vertex->vertexSize();
	UINT offset = 0;
	m_context->m_DeviceContext->IASetVertexBuffers(0, 1, pVertexBuffers.GetAddressOf(), &size, &offset);
	m_context->m_DeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_context->m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_context->m_DeviceContext->IASetInputLayout(pInputLayout);
}

void Pipeline::VS() {
	m_context->m_DeviceContext->VSSetShader(m_material->getVSShader()->getVS().Get(), 0, 0);
	IShader::CBuffers buffers = m_material->getVSShader()->getUniform();
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
	IShader::CBuffers buffers = m_material->getPSShader()->getUniform();
	for (auto buffer : buffers) {
		CBufferData bufferData = buffer.second;
		bufferData.UpdateBuffer(m_context->m_DeviceContext.Get());
		m_context->m_DeviceContext->PSSetConstantBuffers(bufferData.getSlot(), 1, bufferData.getBuffer().GetAddressOf());
	}
	IShader::TextureBuffer textures = m_material->getPSShader()->getTexture();
	for (auto texture : textures) {
		m_context->m_DeviceContext->PSSetShaderResources(texture.first, 1, texture.second.pSRV.GetAddressOf());
	}
	IShader::SampleBuffer samples = m_material->getPSShader()->getSample();
	for (auto sample : samples) {
		m_context->m_DeviceContext->PSSetSamplers(sample.first, 1, pSampleState.GetAddressOf());
	}

}

void Pipeline::OM() {
	m_context->m_DeviceContext->OMSetRenderTargets(1, 
		m_drawer->getRenderTarget()->GetRenderTarget().GetAddressOf(), 
		m_drawer->getDepthStencil()->GetDepthStencil().Get());
}

void Pipeline::DrawIndex() {
	m_context->m_DeviceContext->DrawIndexed(m_vertex->indexSize(), 0, 0);
}