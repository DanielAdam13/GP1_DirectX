// DirectX Headers
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>

#include <string>

class Effect abstract
{
public:
	Effect(ID3D11Device* pDevice, const std::wstring& assetPath);
	Effect(Effect& other) = delete;
	Effect(const Effect& other) = delete;
	Effect(Effect&& rffect) = delete;
	~Effect() noexcept;

protected:
	ID3DX11Effect* m_pEffect;
	ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetPath);

private:
	
};