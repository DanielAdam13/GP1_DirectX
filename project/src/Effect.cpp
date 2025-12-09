#include "Effect.h"
#include <iostream>
#include <sstream> // string stream for wstringstream

Effect::Effect(ID3D11Device* pDevice, const std::wstring& assetPath)
	:m_pDevice{ pDevice },
	assetFile{ assetPath },
	m_pEffect{}
{
	m_pEffect = LoadEffect(m_pDevice, assetFile);
}

ID3DX11Effect* Effect::LoadEffect(ID3D11Device* pDevice, const std::wstring& assetPath)
{
	HRESULT result;
	ID3D10Blob* pErrorBlob{ nullptr };
	ID3DX11Effect* pEffect;

	DWORD shaderFlags{ 0 };

#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	result = D3DX11CompileEffectFromFile(assetPath.c_str(),
		nullptr,
		nullptr,
		shaderFlags,
		0,
		pDevice,
		&pEffect,
		&pErrorBlob);

	if (FAILED(result))
	{
		if (pErrorBlob != nullptr)
		{
			const char* pErrors{ static_cast<char*>(pErrorBlob->GetBufferPointer()) };
			std::wstringstream ss{};

			for (int i{ 0 }; i < pErrorBlob->GetBufferSize(); ++i)
			{
				ss << pErrors[i];
			}

			OutputDebugStringW(ss.str().c_str());
			pErrorBlob->Release();
			pErrorBlob = nullptr;

			std::wcout << ss.str() << L"\n";
		}
		else
		{
			std::wstringstream ss{};

			ss << "EffectLoader: Failed to Create Effect From File! \nPath: " << assetPath;
			std::wcout << ss.str() << L"\n";
			return nullptr;
		}
	}

	return pEffect;
}

ID3DX11Effect* Effect::GetEffect() const
{
	return m_pEffect;
}

ID3D11Device* Effect::GetDevice() const
{
	return m_pDevice;
}
