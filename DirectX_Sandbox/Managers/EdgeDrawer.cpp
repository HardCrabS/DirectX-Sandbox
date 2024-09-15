#include "EdgeDrawer.h"

void EdgeDrawer::Initialize()
{
	auto graphics = &Graphics::getInstance();

	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = (float)graphics->GetWidth();
	texDesc.Height = (float)graphics->GetHeight();
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	// Create the texture
	ID3D11Texture2D* pRenderTargetTexture = nullptr;
	HRESULT hr = graphics->GetDevice()->CreateTexture2D(&texDesc, nullptr, &pRenderTargetTexture);

	hr &= graphics->GetDevice()->CreateRenderTargetView(pRenderTargetTexture, nullptr, &pEdgeDrawerRenderTargetView);
	hr &= graphics->GetDevice()->CreateShaderResourceView(pRenderTargetTexture, nullptr, &pRenderedSceneTexture);
	assert(SUCCEEDED(hr));

	screenQuad = std::make_unique<CustomMesh>(MeshData(PrimitiveType::ScreenQuad));
	edgeDrawMaterial = std::make_shared<EdgeMaterial>(graphics->GetWidth(), graphics->GetHeight(), 2);
	solidMaterial = std::make_shared<SolidMaterial>(XMFLOAT4(1, 1, 0, 1));
	graphics->GetResourcesContainer().RegisterMaterial("EdgeDrawerMaterial", edgeDrawMaterial);
	graphics->GetResourcesContainer().RegisterMaterial("EdgeDrawerSolidMaterial", solidMaterial);
}

void EdgeDrawer::Render()
{
	auto graphics = &Graphics::getInstance();

	// Change render target
	float bgColor[4] = { 0.0f, 0.0f, 0.f, 1.f };
	graphics->GetDeviceContext()->ClearRenderTargetView(pEdgeDrawerRenderTargetView, bgColor);
	graphics->GetDeviceContext()->OMSetRenderTargets(1, &pEdgeDrawerRenderTargetView, nullptr);

	// render scene with simple solid material
	const auto& entities = ECSWorld::getInstance().GetEntities();

	for (const auto& it : entities) {
		auto& e = it.second;
		if (!e->HasComponents(MeshComponent::TypeID(), DrawEdgeFlag::TypeID()))
			continue;

		RenderManager::getInstance().RenderEntity(e.get(), solidMaterial);
	}

	// restore render target
	graphics->SetDefaultRenderTarget();

	edgeDrawMaterial->SetTexture(pRenderedSceneTexture);
	edgeDrawMaterial->UpdateConstantBuffer();
	RenderManager::getInstance().RenderMesh(*screenQuad, edgeDrawMaterial);
}

EdgeDrawer::~EdgeDrawer()
{
	pEdgeDrawerRenderTargetView->Release();
	pRenderedSceneTexture->Release();
}