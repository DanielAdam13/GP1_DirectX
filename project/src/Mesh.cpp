#include "Mesh.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "ColorRGB.h"
#include "ShadingEffect.h"

#define SAFE_RELEASE(p) \
if (p) {p->Release(); p = nullptr; }

Mesh::Mesh(ID3D11Device* pDevice, const std::vector<VertexIn>&_vertices, const std::vector<uint32_t>&_indices, PrimitiveTopology _primitive,
	const std::string & diffuseTexturePath, const std::string& normalTexturePath, const std::string & specularTexturePath, const std::string & glossTexturePath)
	: m_Vertices{ _vertices },
	m_Indices{ _indices },
	m_CurrentTopology{ _primitive },
	m_pEffect{},
	m_Position{ 0.f, 0.f, 0.f },
	m_RotY{ 0.f },
	m_Scale{ 1.f, 1.f, 1.f },
	m_WorldMatrix{},
	m_TranslationMatrix{ Matrix::CreateTranslation(m_Position) },
	m_RotationMatrix{ Matrix::CreateRotationY(m_RotY) },
	m_ScaleMatrix{ Matrix::CreateScale(m_Scale) },
	m_pDiffuseTetxure{},
	m_pNormalTexture{},
	m_pSpecularTexture{},
	m_pGlossTexture{}
{
	m_WorldMatrix = m_ScaleMatrix * m_RotationMatrix * m_TranslationMatrix;
	m_pEffect = new ShadingEffect(pDevice, L"resources/PosCol3D.fx"); // Mesh owns Effect FOR NOW
	CreateLayouts(pDevice);
	CreateSamplerStates(pDevice);
	m_CurrentSampler = m_pPointSampler;
	m_pDiffuseTetxure = std::unique_ptr<Texture>(Texture::LoadFromFile(pDevice, diffuseTexturePath));
	m_pNormalTexture = std::unique_ptr<Texture>(Texture::LoadFromFile(pDevice, normalTexturePath));;
	m_pSpecularTexture = std::unique_ptr<Texture>(Texture::LoadFromFile(pDevice, specularTexturePath));
	m_pGlossTexture = std::unique_ptr<Texture>(Texture::LoadFromFile(pDevice, glossTexturePath));
}

Mesh::~Mesh()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pInputLayout);

	SAFE_RELEASE(m_pPointSampler);
	SAFE_RELEASE(m_pLinearSampler);
	SAFE_RELEASE(m_pAnisotropicSampler);

	delete m_pEffect;
	m_pEffect = nullptr;

	/*delete m_pDiffuseTetxure;
	m_pDiffuseTetxure = nullptr;*/
}

void Mesh::Render(ID3D11DeviceContext* pDeviceContext, const Matrix& viewProjMatrix, SamplerType samplerType, Vector3& cameraPos)
{
	m_WorldMatrix = m_ScaleMatrix * m_RotationMatrix * m_TranslationMatrix;
	Matrix worldViewProjectionMatrix{ m_WorldMatrix * viewProjMatrix };
	m_pEffect->GetWorldViewProjMatrix()->SetMatrix(reinterpret_cast<float*>(&worldViewProjectionMatrix));

	// Bind Texture's SRV to GPU's resource view
	m_pEffect->SetDiffuseMap(m_pDiffuseTetxure.get()); 
	m_pEffect->SetNormalMap(m_pNormalTexture.get());
	m_pEffect->SetSpecularMap(m_pSpecularTexture.get());
	m_pEffect->SetGlossMap(m_pGlossTexture.get());

	// Set Primitive Topology
	if (m_CurrentTopology == PrimitiveTopology::TriangleList)
	{
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	else
	{
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}
	
	// Set Input Layout
	pDeviceContext->IASetInputLayout(m_pInputLayout);

	// Set Vertex Buffer
	constexpr UINT stride{ sizeof(VertexIn) };
	constexpr UINT offset{ 0 };
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Set Index Buffer
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	switch (samplerType)
	{
	case Mesh::SamplerType::Point:        
		m_CurrentSampler = m_pPointSampler; 
		break;
	case Mesh::SamplerType::Linear:       
		m_CurrentSampler = m_pLinearSampler; 
		break;
	case Mesh::SamplerType::Anisotropic:  
		m_CurrentSampler = m_pAnisotropicSampler; 
		break;
	}

	// ----- Apply Technique Pass -----
	D3DX11_TECHNIQUE_DESC techDesc{};
	m_pEffect->GetTechnique()->GetDesc(&techDesc);

	ID3DX11EffectPass* pass = m_pEffect->GetTechnique()->GetPassByIndex(0); // Technique has only one pass
	pass->Apply(0, pDeviceContext);

	// ----- Bind Variables AFTER Technique pass ------
	pDeviceContext->PSSetSamplers(0, 1, &m_CurrentSampler);

	const auto effectWorldMatrix{ m_pEffect->GetWorldMatrix() };
	const auto effectCameraPosVector{ m_pEffect->GetCameraPos() }; 

	if (effectWorldMatrix) 
		effectWorldMatrix->SetMatrix(reinterpret_cast<float*>(&m_WorldMatrix)); 

	if (effectCameraPosVector) 
		effectCameraPosVector->SetFloatVector(reinterpret_cast<float*>(&cameraPos));


	// ----- DRAW -----
	pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
}

void Mesh::CreateLayouts(ID3D11Device* pDevice)
{
	// Vertex Layout
	static constexpr uint32_t numElements{ 4 };
	D3D11_INPUT_ELEMENT_DESC vertexDesc[numElements]{};

	vertexDesc[0].SemanticName = "POSITION";
	vertexDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[0].AlignedByteOffset = offsetof(VertexIn, position); // Starts from offset position OR byte 0 if using float[3]
	vertexDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertexDesc[1].SemanticName = "TEXCOORD";
	vertexDesc[1].SemanticIndex = 0;
	vertexDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	vertexDesc[1].AlignedByteOffset = offsetof(VertexIn, UVCoordinate); // Starts from offset UVCoordinate OR +3 +3 floats = 24 bytes if using float[2]
	vertexDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertexDesc[2].SemanticName = "NORMAL";
	vertexDesc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[2].AlignedByteOffset = offsetof(VertexIn, normal);
	vertexDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertexDesc[3].SemanticName = "TANGENT";
	vertexDesc[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[3].AlignedByteOffset = offsetof(VertexIn, tangent);
	vertexDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	// Input Layout
	D3DX11_PASS_DESC passDesc{};
	m_pEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);

	HRESULT result{ pDevice->CreateInputLayout(
		vertexDesc,
		numElements,
		passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize,
		&m_pInputLayout) };

	if (FAILED(result))
		return;

	// Vertex buffer
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(VertexIn) * static_cast<uint32_t>(m_Vertices.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = m_Vertices.data();

	result = pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	if (FAILED(result))
		return;

	// Index Buffer
	m_NumIndices = static_cast<uint32_t>(m_Indices.size());

	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(uint32_t) * m_NumIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = m_Indices.data();
	result = pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);

	if (FAILED(result))
		return;
}

void Mesh::CreateSamplerStates(ID3D11Device* pDevice)
{
	D3D11_SAMPLER_DESC desc{};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	// Point
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	m_pPointSampler = nullptr;
	pDevice->CreateSamplerState(&desc, &m_pPointSampler);

	// Linear
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	m_pLinearSampler = nullptr;
	pDevice->CreateSamplerState(&desc, &m_pLinearSampler);

	// Anisotropic
	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.MaxAnisotropy = 16;
	m_pAnisotropicSampler = nullptr;
	pDevice->CreateSamplerState(&desc, &m_pAnisotropicSampler);
}

void Mesh::Translate(const Vector3& offset)
{
	m_Position += offset;
	m_TranslationMatrix = Matrix::CreateTranslation(m_Position);
}

void Mesh::RotateY(float yaw)
{
	m_RotY += yaw;
	m_RotationMatrix = Matrix::CreateRotationY(m_RotY);
}

void Mesh::Scale(const Vector3& scale)
{
	m_ScaleMatrix = Matrix::CreateScale(scale);
}
