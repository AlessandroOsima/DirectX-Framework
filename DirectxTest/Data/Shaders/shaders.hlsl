struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

cbuffer ConstantBuffer
{
	float3 positionOffset;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;

    output.position = position;
	output.position.x += positionOffset.x;
	output.position.y += positionOffset.y;

    output.color = color;


    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}