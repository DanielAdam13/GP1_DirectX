#pragma once
// DirectX Headers
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>

#include "DataStructs.h"
#include <vector>

class Effect;

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
		ID3D11Device* pDevice);
	~Mesh();

	void Render(ID3D11DeviceContext* pDeviceContext) const;

private:
	// Mesh Members
	std::vector<VertexIn> m_Vertices;
	std::vector<uint32_t> m_Indices;

	PrimitiveTopology m_CurrentTopology;

	// Direct X Resources
	Effect* m_pEffect;

	ID3D11InputLayout* m_pInputLayout{};
	ID3D11Buffer* m_pVertexBuffer{};
	ID3D11Buffer* m_pIndexBuffer{};
	uint32_t m_NumIndices{};

	void CreateLayouts(ID3D11Device* pDevice);
};