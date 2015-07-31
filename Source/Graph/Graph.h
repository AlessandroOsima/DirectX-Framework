#pragma once

namespace Graph
{
    enum class PrimitiveTopology
    {
        PointList,
        LineList,
        TriangleList,
    };

	enum ConstantBufferBindTarget
	{
		BIND_VS = 0x1,
		BIND_PS = 0x2,
	};
}