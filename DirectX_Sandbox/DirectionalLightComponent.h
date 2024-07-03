#pragma once
#include <DirectXMath.h>
#include "Component.h"
#include "Material.h"

class DirectionalLightComponent : public Component<DirectionalLightComponent>
{
	DirectX::XMVECTOR direction;
	DirectX::XMFLOAT3 color;
	float intensity = 1;

public:
	DirectionalLightComponent(DirectX::XMVECTOR dir, DirectX::XMFLOAT3 col, float intensity)
	: direction(dir), color(col), intensity(intensity) {}

	const DirectX::XMVECTOR& GetDirection() const { return direction; }
	float GetIntensity() const { return intensity; }

	DirectionalLight ConvertToConstantBufferStruct() const
	{
		DirectionalLight directional_light;
		directional_light.direction = this->direction;
		directional_light.color = this->color;
		directional_light.intensity = this->intensity;
		return directional_light;
	}
};