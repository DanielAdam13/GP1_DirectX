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

class Mesh final
{
public:
	Mesh(ID3D11Device* pDevice, const std::vector<VertexIn> vertices, const std::vector<uint32_t> indices);
	~Mesh();

private:
	std::vector<VertexIn> m_Vertices;
	std::vector<uint32_t> m_Indices;

	Effect* m_pEffect;

	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	void CreateLayouts(ID3D11Device* pDevice);
};