#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXTex.h>
#include "Material.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <assimp/texture.h>

enum TextureFormat
{
	WIC,
	DDS,
};

using ShaderResource = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>;

class ResourcesContainer
{
	std::unordered_map<std::string, ShaderResource> m_loadedTextures;
	std::unordered_map<std::string, std::weak_ptr<Material>> m_materials;

	ID3D11Device* device;
	ID3D11DeviceContext* devcon;

public:
	ResourcesContainer() : m_loadedTextures() {}
	ResourcesContainer(const ResourcesContainer&) = delete;
	void operator=(const ResourcesContainer&) = delete;

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* devcon);

	ID3D11ShaderResourceView* GetTexture(const std::string& path, const aiTexture* aiEmbeddedTexture = nullptr, 
		TextureFormat textureFormat=TextureFormat::WIC);

	void RegisterMaterial(const std::string& key, const std::shared_ptr<Material>& material);
	std::shared_ptr<Material> GetMaterial(const std::string& key);

private:
	ID3D11ShaderResourceView* LoadTexture(const std::string& path, TextureFormat textureFormat);
	ID3D11ShaderResourceView* LoadEmbeddedTexture(const aiTexture* texture);
};