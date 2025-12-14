#pragma once
// DirectX Headers
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>

#include "DataStructs.h"
#include <vector>
#include "Matrix.h"

class Effect;
#include "Texture.h"
#include <string>

using namespace dae;

enum class PrimitiveTopology
{
	TriangleList,
	TriangleStrip
};

class Mesh final
{
public:
	Mesh(const std::vector<VertexIn>& _vertices, const std::vector<uint32_t>& _indices, PrimitiveTopology _primitive,
		const std::string& diffuseTexturePath, ID3D11Device* pDevice);
	~Mesh();

	void Render(ID3D11DeviceContext* pDeviceContext, const Matrix& viewProjMatrix);

	void Translate(const Vector3& offset);
	void RotateY(float yaw);
	void Scale(const Vector3& scale);

	std::vector<VertexIn> m_Vertices;
	std::vector<uint32_t> m_Indices;

private:
	// Direct X Resources
	Effect* m_pEffect;

	ID3D11InputLayout* m_pInputLayout{};
	ID3D11Buffer* m_pVertexBuffer{};
	ID3D11Buffer* m_pIndexBuffer{};
	uint32_t m_NumIndices{};

	// Mesh Members
	

	PrimitiveTopology m_CurrentTopology;

	Vector3 m_Position;
	float m_RotY;
	Vector3 m_Scale;

	Matrix m_WorldMatrix;
	Matrix m_TranslationMatrix;
	Matrix m_RotationMatrix;
	Matrix m_ScaleMatrix;

	Texture* m_pDiffuseTetxure;

	void CreateLayouts(ID3D11Device* pDevice);

	
};