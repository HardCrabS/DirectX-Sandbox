#include "EdgeMaterial.h"

void EdgeMaterial::UpdateConstantBuffer()
{
	devcon->UpdateSubresource(edgeCbBuffer, 0, nullptr, &edgeCB, 0, 0);
	devcon->PSSetConstantBuffers(0, 1, &edgeCbBuffer);
}

void EdgeMaterial::CreateBuffers()
{
	D3D11_BUFFER_DESC edgeBuffer;
	ZeroMemory(&edgeBuffer, sizeof(D3D11_BUFFER_DESC));
	edgeBuffer.Usage = D3D11_USAGE_DEFAULT;
	edgeBuffer.ByteWidth = sizeof(EdgeDrawBuffer);
	edgeBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	edgeBuffer.CPUAccessFlags = 0;
	edgeBuffer.MiscFlags = 0;
	HRESULT hr = device->CreateBuffer(&edgeBuffer, NULL, &edgeCbBuffer);
	assert(SUCCEEDED(hr));
}

void EdgeMaterial::CleanUp()
{
	edgeCbBuffer->Release();
}
