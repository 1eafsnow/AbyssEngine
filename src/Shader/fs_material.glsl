#version 400

in vec3 Vertex;
in vec3 Normal;
in vec2 Texture;
in vec4 Shadow;

out vec4 FragColor;

struct Material
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	bool hasDiffuseTexture;
	bool hasSpecularTexture;
	sampler2D diffuseSampler;
	sampler2D specularSampler;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 color;
	float intensity;
};

struct PointLight
{
	vec3 location;
	vec3 color;
	float intensity;
};

uniform sampler2D shadowSampler;
uniform Material material;
uniform PointLight pointLight;
uniform DirectionalLight directionalLight;
uniform vec3 viewLocation;

float ShadowCalculation(vec4 fragPosLightSpace, float bias)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
	float currentDepth = projCoords.z;

    float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowSampler, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowSampler, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;

    return shadow;
}

float GetDiffuseFactor(vec3 direction, vec3 normal)
{
	return max(dot(normal, -direction), 0.0);	
}

float GetSpecularFactor(vec3 direction, vec3 normal, vec3 view)
{
	return max(dot(-view, reflect(direction, normal)), 0.0);
}

vec3 GetDirectionalLightDiffuse(vec3 direction, vec3 color, float intensity, vec3 normal)
{
	direction = normalize(direction);
	return GetDiffuseFactor(direction, normal) * color * intensity;
}

vec3 GetPointLightDiffuse(vec3 location, vec3 color, float intensity, vec3 normal)
{
	vec3 direction = normalize(Vertex - location);
	return GetDiffuseFactor(direction, normal) * color * intensity;
}

vec3 GetDirectionalLightSpecular(vec3 direction, vec3 color, float intensity, vec3 normal)
{
	direction = normalize(direction);
	vec3 viewDirection = normalize(Vertex - viewLocation);
	return GetSpecularFactor(direction, normal, viewDirection) * color * intensity;
}

vec3 GetPointLightSpecular(vec3 location, vec3 color, float intensity, vec3 normal)
{
	vec3 direction = normalize(Vertex - location);
	vec3 viewDirection = normalize(Vertex - viewLocation);
	return GetSpecularFactor(direction, normal, viewDirection) * color * intensity;
}

void main() 
{		
	float alpha;
	vec3 diffuseColor;	
	if (material.hasDiffuseTexture)
	{
		vec4 diffuseRGBA = texture(material.diffuseSampler, Texture);
		diffuseColor = diffuseRGBA.rgb;
		alpha = diffuseRGBA.a;
	}
	else
	{
		diffuseColor = material.diffuseColor;
		alpha = 1.0;
	}
	vec3 specularColor;
	if (material.hasSpecularTexture)
	{
		specularColor = texture(material.specularSampler, Texture).rgb;
	}
	else
	{
		specularColor = material.specularColor;
	}
	
	vec3 normal = normalize(Normal);	
	vec3 directionalLightDiffuse = GetDirectionalLightDiffuse(directionalLight.direction, directionalLight.color, directionalLight.intensity, normal);
	vec3 pointLightDiffuse = GetPointLightDiffuse(pointLight.location, pointLight.color, pointLight.intensity, normal);
	vec3 diffuseResult = (directionalLightDiffuse + pointLightDiffuse) * diffuseColor;

	vec3 directionalLightSpecular = GetDirectionalLightSpecular(directionalLight.direction, directionalLight.color, directionalLight.intensity, normal);
	vec3 pointLightSpecular = GetPointLightSpecular(pointLight.location, pointLight.color, pointLight.intensity, normal);
	vec3 specularResult = (directionalLightSpecular + pointLightSpecular) * specularColor;

	float bias = max(0.05 * (1.0 - dot(normal, directionalLight.direction)), 0.005);
	FragColor = vec4(material.ambientColor + (1.0 - ShadowCalculation(Shadow, 0.005)) * (diffuseResult + specularResult), alpha);
	
	//FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}