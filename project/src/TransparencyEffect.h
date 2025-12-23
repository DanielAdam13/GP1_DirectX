#pragma once
#include "Effect.h"

class TransparencyEffect final : public Effect
{
public:
	explicit TransparencyEffect(ID3D11Device* pDevice);
	TransparencyEffect(TransparencyEffect& other) = delete;
	TransparencyEffect(const TransparencyEffect& other) = delete;
	TransparencyEffect(TransparencyEffect&& effect) = delete;
	virtual ~TransparencyEffect() noexcept = default;

private:

};