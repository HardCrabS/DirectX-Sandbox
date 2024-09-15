#include <d3d11.h>
#include <assert.h>
#include "Graphics.h"
#include "RenderManager.h"
#include "Materials/SolidMaterial.h"
#include "Materials/EdgeMaterial.h"

class EdgeDrawer
{
	ID3D11RenderTargetView* pEdgeDrawerRenderTargetView = nullptr;
	ID3D11ShaderResourceView* pRenderedSceneTexture = nullptr;
	std::unique_ptr<CustomMesh> screenQuad;
	std::shared_ptr<EdgeMaterial> edgeDrawMaterial;
	std::shared_ptr<SolidMaterial> solidMaterial;

public:
	EdgeDrawer() {}
	~EdgeDrawer();
	void Initialize();
	void Render();
};