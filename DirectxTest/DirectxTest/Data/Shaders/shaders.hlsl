static const float MAX_DIRECTIONAL_LIGHTS = 5;
static const float MAX_POINT_LIGHTS = 5;

Texture2D diffuseTexture;

SamplerState TextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct DirectionalLightsProperties
{
	float4 direction;
	float4 color;
};

struct PointLightProperties
{
	float4 worldPosition;
	float4 color;
};

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
	float4 normal : TEXTCOORD0;
	float4 worldPos : TEXCOORD1;
	float2 uv : TEXCOORD2;
};

cbuffer ConstantBuffer
{
	float4x4 transformMatrix;
	float4x4 modelRotation;
    float4 eyePosition;
	float4 ambientColor;
	DirectionalLightsProperties directionalLightsProperties[MAX_DIRECTIONAL_LIGHTS];
	PointLightProperties pointLightsProperties[MAX_POINT_LIGHTS];
};

VOut VShader(float4 position : POSITION, float4 color : COLOR, float4 normal : NORMAL, float2 uv : TEXCOORD)
{
    VOut output;

    output.position = mul(transformMatrix, position);
	output.worldPos = output.position;
	output.color = color;
	output.normal = mul(modelRotation, normal);
	output.uv = uv;


    return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float4 normal : TEXTCOORD0, float4 worldPos : TEXCOORD1, float2 uv : TEXCOORD2) : SV_TARGET
{
	float4 finalColor = float4(0, 0, 0, 0);
	normal = normalize(normal);

	float4 sampledDiffuseColor = diffuseTexture.Sample(TextureSampler, uv);

	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
	{
		float4 lightDir = -directionalLightsProperties[i].direction;
		float4 lightColor = float4(directionalLightsProperties[i].color.xyz, 0);
		float specularExp = directionalLightsProperties[i].color.w;

		float4 specularColor = float4(0, 0, 0, 0);

		float4 reflectedLight = reflect(lightDir, normal);
		float4 eyeVertex = normalize(eyePosition - worldPos);
		float specularFactor = max(dot(eyeVertex, reflectedLight), 0);

		float4 normalRotated = normalize(mul(modelRotation, normal));
		float diffuseBrightness = saturate(dot(normalRotated, lightDir));

		if (diffuseBrightness > 0)
		{
			float specular = pow(specularFactor, specularExp);
			specularColor = saturate(lightColor * specular);
		}

		finalColor = specularColor + sampledDiffuseColor * (lightColor * diffuseBrightness + ambientColor);
	}

	for (int x = 0; x < MAX_POINT_LIGHTS; x++)
	{
		float4 lightPos = pointLightsProperties[x].worldPosition;
		float4 lightDist = float4((lightPos - worldPos).xyz, 0);

		float distance = length(lightDist);
		float4 lightDir = lightDist / distance;
		

		float4 lightColor = float4(pointLightsProperties[x].color.xyz, 0);
		float specularExp = pointLightsProperties[x].color.w;

		float4 specularColor = float4(0, 0, 0, 0);

		normal.w = 0;
		float4 reflectedLight = float4(reflect(lightDir.xyz, normal.xyz),0);

		float4 eyeVertex = float4(eyePosition - worldPos);
		float specularFactor = max(dot(eyeVertex / length(eyeVertex), reflectedLight), 0);

		float4 normalRotated = normalize(mul(modelRotation, normal));
		float diffuseBrightness = saturate(dot(normalRotated, lightDir));

		if (diffuseBrightness > 0)
		{
			float specularBrightness = pow(specularFactor, specularExp);
			specularColor = lightColor * specularBrightness;
		}

		float a0 = 1;
		float a1 = 0.0045f;
		float a2 = 0.00075f;

		float attenuationFactor = a0 + a1 * distance + a2 * distance * distance;

		finalColor += (specularColor + sampledDiffuseColor * (lightColor * diffuseBrightness + ambientColor)) / attenuationFactor;
	}

	return finalColor;
}