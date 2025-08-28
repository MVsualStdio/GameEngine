#include "CBuffer.h"
#include "IDrawer.h"
	
CBufferData::CBufferData(const std::string& name, uint32_t startSlot, uint32_t byteWidth, BYTE* initData) :
	cbufferData(byteWidth), cbufferName(name), startSlot(startSlot) {
	if (initData)
		memcpy_s(cbufferData.data(), byteWidth, initData, byteWidth);
}
	
HRESULT CBufferData::CreateBuffer(ID3D11Device* device)
{
    if (cBuffer != nullptr)
        return S_OK;
    D3D11_BUFFER_DESC cbd;
    ZeroMemory(&cbd, sizeof(cbd));
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.ByteWidth = (uint32_t)cbufferData.size();
    return device->CreateBuffer(&cbd, nullptr, cBuffer.GetAddressOf());
}

void CBufferData::UpdateBuffer(ID3D11DeviceContext* deviceContext)
{
    if (isDirty)
    {
        isDirty = false;
        D3D11_MAPPED_SUBRESOURCE mappedData;
        deviceContext->Map(cBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
        memcpy_s(mappedData.pData, cbufferData.size(), cbufferData.data(), cbufferData.size());
        deviceContext->Unmap(cBuffer.Get(), 0);
    }
}

ComPtr<ID3D11Buffer> CBufferData::getBuffer() {
    return cBuffer;
}

ConstantBufferVariable::ConstantBufferVariable(std::string name_, uint32_t offset, uint32_t size, CBufferData* pData)
    : name(name_), startByteOffset(offset), byteWidth(size), pCBufferData(pData) {

}

void ConstantBufferVariable::SetValue(const void* noPadData, uint32_t size) {
    BYTE* pData = pCBufferData->cbufferData.data() + startByteOffset;

    if (memcmp(pData, noPadData, size)) {
        memcpy_s(pData, size, noPadData, size);
        pCBufferData->isDirty = true;
    }

}

void ConstantBufferVariable::SetFloatMatrix(uint32_t rows, uint32_t cols, const float* noPadData) {
    SetMatrixInBytes(rows, cols, reinterpret_cast<const BYTE*>(noPadData));
}

void ConstantBufferVariable::SetMatrixInBytes(uint32_t rows, uint32_t cols, const BYTE* noPadData) {
    // 仅允许1x1到4x4
    if (rows == 0 || rows > 4 || cols == 0 || cols > 4)
        return;
    uint32_t remainBytes = byteWidth < 64 ? byteWidth : 64;
    BYTE* pData = pCBufferData->cbufferData.data() + startByteOffset;
    while (remainBytes > 0 && rows > 0)
    {
        uint32_t rowPitch = sizeof(uint32_t) * cols < remainBytes ? sizeof(uint32_t) * cols : remainBytes;
        // 仅当值不同时更新
        if (memcmp(pData, noPadData, rowPitch))
        {
            memcpy_s(pData, rowPitch, noPadData, rowPitch);
            pCBufferData->isDirty = true;
        }
        noPadData += cols * sizeof(uint32_t);
        pData += 16;
        remainBytes = remainBytes < 16 ? 0 : remainBytes - 16;
    }
}

void ConstantBufferVariable::Set(const Property& prop) {
    PropertyFunctor fun(*this);
    std::visit(fun, prop);
}