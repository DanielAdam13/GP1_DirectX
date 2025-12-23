#include "Effect.h"

class ShadingEffect final : public Effect
{
public:
	explicit ShadingEffect(ID3D11Device* pDevice);
	ShadingEffect(ShadingEffect& other) = delete;
	ShadingEffect(const ShadingEffect& other) = delete;
	ShadingEffect(ShadingEffect&& effect) = delete;
	virtual ~ShadingEffect() noexcept;
	
	virtual ID3DX11EffectMatrixVariable* GetWorldMatrix() const;
	virtual ID3DX11EffectVectorVariable* GetCameraPos() const;

	virtual void SetNormalMap(Texture* pNormalTexture);
	virtual void SetSpecularMap(Texture* pSpecularTexture);
	virtual void SetGlossMap(Texture* pGlossTexture);
	
private:

	ID3DX11EffectShaderResourceVariable* m_pNormalMapVairable{};
	ID3DX11EffectShaderResourceVariable* m_pSpecularMapVairable{};
	ID3DX11EffectShaderResourceVariable* m_pGlossMapVairable{};

	// Shading Variables
	ID3DX11EffectMatrixVariable* m_pWorldMatrixVariable{};
	ID3DX11EffectVectorVariable* m_pCameraPosVariable{};
};