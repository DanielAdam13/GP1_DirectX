#include "ShadingEffect.h"
#include <iostream>
#include <sstream> // string stream for wstringstream
#include "Texture.h"

ShadingEffect::ShadingEffect(ID3D11Device* pDevice, const std::wstring& assetPath)
	: Effect::Effect(pDevice, assetPath),
	m_pTechnique{},
	m_pWorldViewProjMatrixVariable{},
	m_pDiffuseMapVairable{},
	m_pWorldMatrixVariable{},
	m_pCameraPosVariable{}
{
	m_pTechnique = m_pEffect->GetTechniqueByName("DefaultTechnique");
	if (!m_pTechnique->IsValid())
	{
		std::wcout << L"Technique not valid!\n";
		m_pTechnique = nullptr;
	}

	m_pWorldMatrixVariable = m_pEffect->GetVariableByName("gWorldMatrix")->AsMatrix();
	if (!m_pWorldMatrixVariable->IsValid())
	{
		std::wcout << L"m_pWorldMatrixVariable is not valid!\n";
		m_pWorldMatrixVariable = nullptr;
	}

	m_pCameraPosVariable = m_pEffect->GetVariableByName("gCameraPos")->AsVector();
	if (!m_pCameraPosVariable->IsValid())
	{
		std::wcout << L"m_pCameraPosVariable is not valid!\n";
		m_pCameraPosVariable = nullptr;
	}

	m_pWorldViewProjMatrixVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	if (!m_pWorldViewProjMatrixVariable->IsValid())
	{
		std::wcout << L"m_pWorldViewProjMatrixVariable is not valid!\n";
		m_pWorldViewProjMatrixVariable = nullptr;
	}

	m_pDiffuseMapVairable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
	if (!m_pDiffuseMapVairable->IsValid())
	{
		std::wcout << L"m_pDiffuseMapVariable not valid!\n";
		m_pDiffuseMapVairable = nullptr;
	}
	
	m_pNormalMapVairable = m_pEffect->GetVariableByName("gNormalMap")->AsShaderResource();
	if (!m_pNormalMapVairable->IsValid())
	{
		std::wcout << L"m_pNormalMapVariable not valid!\n";
		m_pNormalMapVairable = nullptr;
	}

	m_pSpecularMapVairable = m_pEffect->GetVariableByName("gSpecularMap")->AsShaderResource();
	if (!m_pSpecularMapVairable->IsValid())
	{
		std::wcout << L"m_pSpecularMapVariable not valid!\n";
		m_pSpecularMapVairable = nullptr;
	}

	m_pGlossMapVairable = m_pEffect->GetVariableByName("gGlossinessMap")->AsShaderResource();
	if (!m_pGlossMapVairable->IsValid())
	{
		std::wcout << L"m_pGlossMapVariable not valid!\n";
		m_pGlossMapVairable = nullptr;
	}
}

ShadingEffect::~ShadingEffect()
{
	Effect::~Effect();

	if (m_pTechnique)
		m_pTechnique = nullptr; // Technique is owned by Effect, thus no need to Release

	if (m_pWorldViewProjMatrixVariable)
		m_pWorldViewProjMatrixVariable = nullptr; // Matrix is owned by Effect --||--

	if (m_pWorldMatrixVariable)
		m_pWorldMatrixVariable = nullptr;

	if (m_pCameraPosVariable)
		m_pCameraPosVariable = nullptr;

	if (m_pDiffuseMapVairable)
		m_pDiffuseMapVairable = nullptr;

	if (m_pNormalMapVairable)
		m_pNormalMapVairable = nullptr;

	if (m_pSpecularMapVairable)
		m_pSpecularMapVairable = nullptr;

	if (m_pGlossMapVairable)
		m_pGlossMapVairable = nullptr;
}

ID3DX11Effect* ShadingEffect::GetEffect() const
{
	return m_pEffect;
}

ID3DX11EffectTechnique* ShadingEffect::GetTechnique() const
{
	return (m_pTechnique && m_pTechnique->IsValid()) ? m_pTechnique : nullptr;
}

ID3DX11EffectMatrixVariable* ShadingEffect::GetWorldMatrix() const
{
	return m_pWorldMatrixVariable;
}

ID3DX11EffectVectorVariable* ShadingEffect::GetCameraPos() const
{
	return m_pCameraPosVariable;
}

ID3DX11EffectMatrixVariable* ShadingEffect::GetWorldViewProjMatrix() const
{
	return m_pWorldViewProjMatrixVariable;
}

void ShadingEffect::SetDiffuseMap(Texture* pDiffuseTexture) // Bind texture's SRV to Effect's SRV
{
	if (m_pDiffuseMapVairable)
	{
		m_pDiffuseMapVairable->SetResource(pDiffuseTexture->GetSRV());
	}
}

void ShadingEffect::SetNormalMap(Texture* pNormalTexture)
{
	if (m_pNormalMapVairable && pNormalTexture)
	{
		m_pNormalMapVairable->SetResource(pNormalTexture->GetSRV());
	}
}

void ShadingEffect::SetSpecularMap(Texture* pSpecularTexture)
{
	if (m_pSpecularMapVairable && pSpecularTexture)
	{
		m_pSpecularMapVairable->SetResource(pSpecularTexture->GetSRV());
	}
}

void ShadingEffect::SetGlossMap(Texture* pGlossTexture)
{
	if (m_pGlossMapVairable && pGlossTexture)
	{
		m_pGlossMapVairable->SetResource(pGlossTexture->GetSRV());
	}
}
