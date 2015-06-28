struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

cbuffer ConstantBuffer
{
	float4x4 transformMatrix;
	float4x4 modelRotation;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR, float4 normal : NORMAL)
{
    VOut output;

    output.position = mul(transformMatrix, position);

	

    //float4 lightVector = float4(0,0,1,0);

	float4 lightPos = float4(0, 0, 200, 0);
    float4 lightVector = normalize(output.position - lightPos);

    float4 lightColor = float4(0,1,0,1);

	float4 normalRotated = normalize(mul(modelRotation, normal));
	float diffuseBrightness = saturate(dot(normalRotated, lightVector));

	output.color = lightColor * diffuseBrightness;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}