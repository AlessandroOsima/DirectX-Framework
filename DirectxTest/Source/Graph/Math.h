#pragma once 

namespace Math
{
	struct Vector2
	{
		float x;
		float y;
	};

	struct Vector3
	{
		float x;
		float y;
		float z;
	};

	struct Vector4
	{
		float x;
		float y;
		float z;
		float w;
	};

	struct Color
	{
		float r;
		float g;
		float b;
		float a;
	};

	struct Vertex
	{
		Vector3 position;
		Color color;
	};

}