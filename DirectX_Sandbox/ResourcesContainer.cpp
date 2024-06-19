#include "ResourcesContainer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Utils.h"

void ResourcesContainer::Initialize(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	this->device = device;
	this->devcon = devcon;
}

ID3D11ShaderResourceView* ResourcesContainer::GetTexture(const std::string& path, const aiTexture* aiEmbeddedTexture)
{
	if (m_loadedTextures.find(path) != m_loadedTextures.end())
		return m_loadedTextures.at(path).Get();
	if (aiEmbeddedTexture != nullptr)
		return LoadTexture(aiEmbeddedTexture);

	return LoadTexture(path);
}

ID3D11ShaderResourceView* ResourcesContainer::LoadTexture(const std::string& path)
{
	ShaderResource textureResource;

	DirectX::TexMetadata metadata;
	DirectX::ScratchImage image;
	HRESULT hr = LoadFromWICFile(Utils::StringToWString(path).c_str(), DirectX::WIC_FLAGS_NONE, &metadata, image);
	assert(SUCCEEDED(hr));
	hr = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), 
		metadata, textureResource.GetAddressOf());
	assert(SUCCEEDED(hr));

	ID3D11ShaderResourceView* resourceView = textureResource.Get();
	m_loadedTextures[path] = std::move(textureResource);

	return resourceView;
}

ID3D11ShaderResourceView* ResourcesContainer::LoadTexture(const aiTexture* texture)
{
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = texture->mWidth;
	desc.Height = texture->mHeight > 0 ? texture->mHeight : 1;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	// TODO: handle uncompressed texture case. Check for texture->mHeight > 0
	int width, height, channels;
	unsigned char* imageData = stbi_load_from_memory(
		reinterpret_cast<unsigned char*>(texture->pcData),
		texture->mWidth,
		&width, &height, &channels, STBI_rgb_alpha);

	desc.Width = width;
	desc.Height = height;

	D3D11_SUBRESOURCE_DATA initData = {};
	//initData.pSysMem = texture->pcData;
	initData.pSysMem = imageData;
	initData.SysMemPitch = width * 4;

	ID3D11Texture2D* dxTexture = nullptr;
	HRESULT hr = device->CreateTexture2D(&desc, &initData, &dxTexture);
	assert(SUCCEEDED(hr));

	ShaderResource textureResource;
	device->CreateShaderResourceView(dxTexture, nullptr, textureResource.GetAddressOf());
	ID3D11ShaderResourceView* resourceView = textureResource.Get();
	m_loadedTextures[texture->mFilename.C_Str()] = std::move(textureResource);

	stbi_image_free(imageData);  // clear texture data from CPU (for compressed)

	return resourceView;
}
