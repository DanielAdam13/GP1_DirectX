#include "Effect.h"

class Texture;

class ShadingEffect final : Effect
{
public:
	explicit ShadingEffect(ID3D11Device* pDevice, const std::wstring& assetPath);
	ShadingEffect(ShadingEffect& other) = delete;
	ShadingEffect(const ShadingEffect& other) = delete;
	ShadingEffect(ShadingEffect&& effect) = delete;
	~ShadingEffect() noexcept;

	ID3DX11Effect* GetEffect() const;
	ID3DX11EffectTechnique* GetTechnique() const;
	ID3DX11EffectMatrixVariable* GetWorldMatrix() const;
	ID3DX11EffectVectorVariable* GetCameraPos() const;

	ID3DX11EffectMatrixVariable* GetWorldViewProjMatrix() const;

	void SetDiffuseMap(Texture* pDiffuseTexture);
	void SetNormalMap(Texture* pNormalTexture);
	void SetSpecularMap(Texture* pSpecularTexture);
	void SetGlossMap(Texture* pGlossTexture);
	
private:
	ID3DX11EffectTechnique* m_pTechnique;
	
	ID3DX11EffectMatrixVariable* m_pWorldViewProjMatrixVariable;

	ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVairable;
	ID3DX11EffectShaderResourceVariable* m_pNormalMapVairable;
	ID3DX11EffectShaderResourceVariable* m_pSpecularMapVairable;
	ID3DX11EffectShaderResourceVariable* m_pGlossMapVairable;

	// Shading Variables
	ID3DX11EffectMatrixVariable* m_pWorldMatrixVariable;
	ID3DX11EffectVectorVariable* m_pCameraPosVariable;
};