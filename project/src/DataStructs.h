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
		float position[3];
		float color[3];
		float UBCooridnate[2];
	};

	struct VertexOut
	{
		float position[4];
		float color[3];
		float UBCooridnate[2];
	};
}