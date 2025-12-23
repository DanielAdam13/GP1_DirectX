#pragma once
// DirectX Headers
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>

#include "DataStructs.h"
#include <vector>
#include "Matrix.h"

class TransparencyEffect;
class ShadingEffect;
#include "Effect.h"

#include "Texture.h"
#include <string>
#include <memory>

using namespace dae;

enum class PrimitiveTopology
{
	TriangleList,
	TriangleStrip
};

class Mesh final
{
public:
	Mesh(ID3D11Device* pDevice, const std::string& mainBodyMeshOBJ, Effect::EffectType effectType, PrimitiveTopology _primitive,
		const std::string& diffuseTexturePath, const std::string& normalTexturePath = "", const std::string& specularTexturePath = "", const std::string& glossTexturePath = "");
	~Mesh();

	enum class SamplerType
	{
		Point = 0,
		Linear = 1,
		Anisotropic = 2
	};

	void Render(ID3D11DeviceContext* pDeviceContext, const Matrix& viewProjMatrix, SamplerType samplerType, Vector3& cameraPos);

	void Translate(const Vector3& offset);
	void RotateY(float yaw);
	void Scale(const Vector3& scale);

private:
	// Mesh Members
	std::unique_ptr<Effect> m_pEffect;
	std::vector<VertexIn> m_Vertices;
	std::vector<uint32_t> m_Indices;

	PrimitiveTopology m_CurrentTopology;

	Vector3 m_Position;
	float m_RotY;
	Vector3 m_Scale;

	Matrix m_WorldMatrix;
	Matrix m_TranslationMatrix;
	Matrix m_RotationMatrix;
	Matrix m_ScaleMatrix;

	std::unique_ptr<Texture> m_pDiffuseTetxure;
	std::unique_ptr<Texture> m_pNormalTexture;
	std::unique_ptr<Texture> m_pSpecularTexture;
	std::unique_ptr<Texture> m_pGlossTexture;

	void CreateLayouts(ID3D11Device* pDevice);
	void CreateSamplerStates(ID3D11Device* pDevice);
	std::unique_ptr<Effect> CreateEffect(Effect::EffectType effecttype, ID3D11Device* pDevice);

	// Direct X Resources
	ID3D11InputLayout* m_pInputLayout{};
	ID3D11Buffer* m_pVertexBuffer{};
	ID3D11Buffer* m_pIndexBuffer{};
	uint32_t m_NumIndices{};

	ID3D11SamplerState* m_pPointSampler{};
	ID3D11SamplerState* m_pLinearSampler{};
	ID3D11SamplerState* m_pAnisotropicSampler{};
	ID3D11SamplerState* m_CurrentSampler{};

};