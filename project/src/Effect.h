// DirectX Headers
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>

#include <string>

class Effect final
{
public:
	Effect(ID3D11Device* pDevice, const std::wstring& assetPath);
	Effect(const Effect& other) = delete;
	Effect(Effect&& rffect) = delete;

	ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetPath);

	ID3DX11Effect* GetEffect() const;
	ID3D11Device* GetDevice() const;
private:
	ID3D11Device* m_pDevice;
	const std::wstring assetFile;

	ID3DX11Effect* m_pEffect;
};