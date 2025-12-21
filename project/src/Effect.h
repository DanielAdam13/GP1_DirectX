// DirectX Headers
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>

#include <string>
class Texture;

class Effect final
{
public:
	explicit Effect(ID3D11Device* pDevice, const std::wstring& assetPath);
	Effect(Effect& other) = delete;
	Effect(const Effect& other) = delete;
	Effect(Effect&& rffect) = delete;
	~Effect();

	ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetPath);

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
	ID3DX11Effect* m_pEffect;
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