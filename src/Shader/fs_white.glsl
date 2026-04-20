#version 400

in vec3 Vertex;
in vec3 Normal;
in vec2 Texture;
in vec4 Shadow;

out vec4 FragColor;

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

uniform PointLight pointLight;
uniform DirectionalLight directionalLight;
uniform vec3 viewLocation;
uniform sampler2D shadowSampler;

float ShadowCalculation(vec4 fragPosLightSpace, float bias)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowSampler, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

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
	vec3 normal = normalize(Normal);
	vec3 directionalLightDiffuse = GetDirectionalLightDiffuse(directionalLight.direction, directionalLight.color, directionalLight.intensity, normal);
	vec3 pointLightDiffuse = GetPointLightDiffuse(pointLight.location, pointLight.color, pointLight.intensity, normal);
	vec3 diffuseResult = (directionalLightDiffuse + pointLightDiffuse) * vec3(0.5, 0.5, 0.5);

	vec3 directionalLightSpecular = GetDirectionalLightSpecular(directionalLight.direction, directionalLight.color, directionalLight.intensity, normal);
	vec3 pointLightSpecular = GetPointLightSpecular(pointLight.location, pointLight.color, pointLight.intensity, normal);
	vec3 specularResult = (directionalLightSpecular + pointLightSpecular) * vec3(0.5, 0.5, 0.5);

	float bias = max(0.05 * (1.0 - dot(normal, directionalLight.direction)), 0.005);
	FragColor = vec4(vec3(0.3, 0.3, 0.3) + (1.0 - ShadowCalculation(Shadow, 0.001)) * (diffuseResult + specularResult), 1);
	//FragColor = vec4(vec3(0.3, 0.3, 0.3) + (diffuseResult + specularResult), 1);
	//FragColor = vec4(1.0, 1.0, 1.0, 0.0);
}