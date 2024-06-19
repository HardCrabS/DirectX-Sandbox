#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXTex.h>
#include <unordered_map>
#include <string>
#include <assimp/texture.h>

using ShaderResource = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>;

class ResourcesContainer
{
	std::unordered_map<std::string, ShaderResource> m_loadedTextures;

	ID3D11Device* device;
	ID3D11DeviceContext* devcon;

public:
	ResourcesContainer() : m_loadedTextures() {}
	ResourcesContainer(const ResourcesContainer&) = delete;
	void operator=(const ResourcesContainer&) = delete;

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* devcon);

	ID3D11ShaderResourceView* GetTexture(const std::string& path, const aiTexture* aiEmbeddedTexture = nullptr);
	ID3D11ShaderResourceView* LoadTexture(const std::string& path);
	ID3D11ShaderResourceView* LoadTexture(const aiTexture* texture);
};