#include "ResourcesContainer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Utils.h"

void ResourcesContainer::Initialize(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	this->device = device;
	this->devcon = devcon;
}

ID3D11ShaderResourceView* ResourcesContainer::GetTexture(const std::string& path, const aiTexture* aiEmbeddedTexture, 
	TextureFormat textureFormat)
{
	if (m_loadedTextures.find(path) != m_loadedTextures.end())
		return m_loadedTextures.at(path).Get();
	if (aiEmbeddedTexture != nullptr)
		return LoadEmbeddedTexture(aiEmbeddedTexture);

	return LoadTexture(path, textureFormat);
}

ID3D11ShaderResourceView* ResourcesContainer::LoadTexture(const std::string& path, TextureFormat textureFormat)
{
	ShaderResource textureResource;

	DirectX::TexMetadata metadata;
	DirectX::ScratchImage image;
	HRESULT hr;
	if (textureFormat == TextureFormat::WIC)
	{
		hr = LoadFromWICFile(Utils::StringToWString(path).c_str(), DirectX::WIC_FLAGS_NONE, &metadata, image);
	}
	else if (textureFormat == TextureFormat::DDS)
	{
		hr = LoadFromDDSFile(Utils::StringToWString(path).c_str(), DirectX::DDS_FLAGS_NONE, &metadata, image);
	}
	else
	{
		logError("[ResourcesContainer] Unknown TextureFormat!");
	}
	assert(SUCCEEDED(hr));
	hr = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), 
		metadata, textureResource.GetAddressOf());
	assert(SUCCEEDED(hr));

	ID3D11ShaderResourceView* resourceView = textureResource.Get();
	m_loadedTextures[path] = std::move(textureResource);

	return resourceView;
}

ID3D11ShaderResourceView* ResourcesContainer::LoadEmbeddedTexture(const aiTexture* texture)
{
	D3D11_TEXTURE2D_DESC desc = {};
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	ID3D11Texture2D* dxTexture = nullptr;

	if (texture->mHeight == 0)
	{
		// The texture is compressed
		int width, height, channels;
		unsigned char* imageData = stbi_load_from_memory(
			reinterpret_cast<unsigned char*>(texture->pcData),
			texture->mWidth,
			&width, &height, &channels, STBI_rgb_alpha);

		desc.Width = width;
		desc.Height = height;

		initData.pSysMem = imageData;
		initData.SysMemPitch = width * 4;

		HRESULT hr = device->CreateTexture2D(&desc, &initData, &dxTexture);
		assert(SUCCEEDED(hr));

		stbi_image_free(imageData);  // clear texture data from CPU
	}
	else
	{
		desc.Width = texture->mWidth;
		desc.Height = texture->mHeight;

		initData.pSysMem = texture->pcData;
		initData.SysMemPitch = texture->mWidth * 4;

		HRESULT hr = device->CreateTexture2D(&desc, &initData, &dxTexture);
		assert(SUCCEEDED(hr));
	}

	ShaderResource textureResource;
	device->CreateShaderResourceView(dxTexture, nullptr, textureResource.GetAddressOf());
	ID3D11ShaderResourceView* resourceView = textureResource.Get();
	m_loadedTextures[texture->mFilename.C_Str()] = std::move(textureResource);

	return resourceView;
}

void ResourcesContainer::RegisterMaterial(const std::string& key, const std::shared_ptr<Material>& material)
{
	logInfo("[ResourcesContainer] Registered material: " + key);
	material->SetDeviceAndDevcon(device, devcon);
	material->Initialize();
	m_materials[key] = std::weak_ptr(material);
}

std::shared_ptr<Material> ResourcesContainer::GetMaterial(const std::string& key)
{
	if (m_materials.find(key) != m_materials.end()) {
		logInfo("[ResourcesContainer] Retrieved material: " + key);
		return m_materials.at(key).lock();
	}
	return nullptr;
}