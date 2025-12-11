#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "ColorRGB.h"
#include "MathHelpers.h"

#include <iostream>

#include <vector>
#include <array>

namespace dae
{
	struct VertexIn
	{
		Vector3 position{};
		ColorRGB color{};
		Vector2 UVCoordinate{};
	};

	struct VertexOut
	{
		Vector4 position{};
		ColorRGB color{};
		Vector2 UVCoordinate{};
	};
}