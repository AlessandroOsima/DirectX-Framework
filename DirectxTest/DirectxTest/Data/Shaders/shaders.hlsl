static const float MAX_DIRECTIONAL_LIGHTS = 5;
static const float MAX_POINT_LIGHTS = 5;

struct DirectionalLightsProperties
{
	float4 worldPosition;
	float4 color;
};

struct PointLightProperties
{
	float4 worldPosition;
	float4 color;
	float specularExponent;
	float radius;
};

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

cbuffer ConstantBuffer
{
	float4x4 transformMatrix;
	float4x4 modelRotation;
    float4 eyePosition;

	DirectionalLightsProperties directionalLightsProperties;
	//DirectionalLightsProperties[MAX_DIRECTIONAL_LIGHTS] directionalLightsProperties;
	//PointLightProperties[MAX_POINT_LIGHTS] pointLightsProperties;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR, float4 normal : NORMAL)
{
    VOut output;

    output.position = mul(transformMatrix, position);

    //float4 lightVector = float4(0,0,1,0);

	float4 lightPos = float4(0, 0, 300, 0);
    float4 lightColor = float4(0.5,0.5,0.5,1);
    float4 ambientLight = float4(0.1,0.1,0.1,1);
    float specularExp = 10;

    float4 specularColor = float4(0,0,0,0);

    float4 lightVector = output.position - lightPos;
    float4 reflectedLight = normalize(reflect(normalize(lightVector),normal));
    float4 eyeVertex = normalize(output.position - eyePosition);
    float specularFactor = dot(eyeVertex, reflectedLight);

    if(specularFactor > 0)
    {
        specularFactor = pow(specularFactor, specularExp);
        specularColor = lightColor * specularFactor;
    }


    float d = length(lightVector);
    

	float4 normalRotated = normalize(mul(modelRotation, normal));
    float diffuseBrightness = saturate(dot(normalRotated, normalize(lightVector)));
    

    float a0 = 0;
    float a1 = 0;
    float a2 = 0;

    float attenuationFactor = a0 + a1 * d + a2 * d * d ;

    output.color = specularColor + lightColor * diffuseBrightness ;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}