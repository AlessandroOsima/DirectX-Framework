static const float MAX_DIRECTIONAL_LIGHTS = 5;
static const float MAX_POINT_LIGHTS = 5;

struct DirectionalLightsProperties
{
	float4 direction;
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
	float4 normal : TEXTCOORD0;
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

VOut VShader(float4 position : POSITION, float4 color : COLOR, float4 normal : NORMAL)
{
    VOut output;

    output.position = mul(transformMatrix, position);
	output.color = color;
	output.normal = normal;

    //float4 lightVector = float4(0,0,1,0);

	/*
	float4 lightPos = directionalLightsProperties[i].worldPosition;
	float4 lightColor = directionalLightsProperties[i].color;
	float specularExp = 10;

	float4 specularColor = float4(0, 0, 0, 0);

	float4 lightVector = float4(normalize(lightPos - output.position).xyz,0);

	float4 reflectedLight = normalize(reflect(normalize(lightVector), normal));
	float4 eyeVertex = normalize(output.position - eyePosition);
	float specularFactor = dot(eyeVertex, reflectedLight);

	if (specularFactor > 0)
	{
	specularFactor = pow(specularFactor, specularExp);
	specularColor = lightColor * specularFactor;
	}

	float d = length(lightVector);

	float4 normalRotated = normalize(mul(modelRotation, normal));
	float diffuseBrightness = saturate(dot(normalRotated, lightVector));

	float a0 = 0;
	float a1 = 0;
	float a2 = 0;

	float attenuationFactor = a0 + a1 * d + a2 * d * d;

	output.color = output.color + specularColor + lightColor * diffuseBrightness;


	///////
	float4 lightPos = float4(0, 0, 300, 0);
    float4 lightColor = float4(1,1,1,1);
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
	*/
    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float4 normal : TEXTCOORD0) : SV_TARGET
{
	float4 finalColor = ambientColor;
	normal = normalize(normal);

	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
	{
		float4 lightDir = -directionalLightsProperties[i].direction;
		float4 lightColor = float4(directionalLightsProperties[i].color.xyz, 0);
		float specularExp = directionalLightsProperties[i].color.w;

		float4 specularColor = float4(0, 0, 0, 0);

		float4 reflectedLight = reflect(lightDir, normal);
		float4 eyeVertex = normalize(eyePosition - position);
		float specularFactor = max(dot(eyeVertex, reflectedLight), 0);

		float4 normalRotated = normalize(mul(modelRotation, normal));
		float diffuseBrightness = saturate(dot(normalRotated, lightDir));

		if (diffuseBrightness > 0)
		{
			float specular = pow(specularFactor, specularExp);
			specularColor = saturate(lightColor * specular);
		}

		finalColor += specularColor + lightColor * diffuseBrightness;
	}

	return finalColor;
}