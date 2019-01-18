#version 330 core
in vec2 texCoord;
// in vec3 normalV;
// in vec3 fragPos;

out vec4 FragColor;

// struct Material
// {
// 	sampler2D diffuse;
// 	sampler2D specular;
// 	float shininess;
// };

// struct Light {
// 	vec3 ambient;
// 	vec3 diffuse;
// 	vec3 specular;
// 	vec3 position;
// 	vec3 direction;

// 	float constant;
// 	float linear;
// 	float quadratic;

// 	float cutOff;
// 	float outerCutOff;
// };

// uniform vec3 viewPos;
// uniform Material material;
// uniform Light sunLight;
// uniform Light pointLight;
// uniform Light spotLight;

uniform sampler2D diffuse_map1;
// uniform sampler2D diffuse_map2;
// uniform sampler2D diffuse_map3;
// uniform sampler2D diffuse_map4;

// uniform sampler2D specular_map1;
// uniform sampler2D specular_map2;
// uniform sampler2D specular_map3;
// uniform sampler2D specular_map4;

uniform sampler2D height_map1;
// uniform sampler2D height_map2;
// uniform sampler2D height_map3;
// uniform sampler2D height_map4;

// uniform sampler2D normal_map1;
// uniform sampler2D normal_map2;
// uniform sampler2D normal_map3;
// uniform sampler2D normal_map4;

// vec4 calcLight(int lightType, Light light, vec3 normal, vec3 viewDir)
// {
// 	vec3 norm = normalize(normal);
// 	vec3 ambientColor = light.ambient * vec3(texture(diffuse_map1, texCoord));
	
// 	if(lightType == 1)
// 	{
// 		// directional
// 		vec3 lightDir = normalize(-light.direction);

// 		float diff = max(dot(norm, lightDir), 0.0);
// 		vec3 diffuseColor = (diff * vec3(texture(diffuse_map1, texCoord))) * light.diffuse;

// 		vec3 rDir = reflect(-lightDir, norm);
// 		float spec = pow(max(dot(viewDir, rDir), 0.0), material.shininess);
// 		vec3 specularColor = light.specular * spec * vec3(texture(specular_map1, texCoord));

// 		return vec4((ambientColor + diffuseColor + specularColor), 1.0);
// 	}

// 	vec3 lightDir = normalize(light.position - fragPos);
// 	float diff = max(dot(norm, lightDir), 0.0);
// 	vec3 diffuseColor = (diff * vec3(texture(diffuse_map1, texCoord))) * light.diffuse;

// 	vec3 rDir = reflect(-lightDir, norm);
// 	float spec = pow(max(dot(viewDir, rDir), 0.0), material.shininess);
// 	vec3 specularColor = light.specular * spec * vec3(texture(specular_map1, texCoord));

// 	if(lightType == 2)
// 	{
// 		// spot
// 		float theta = dot(lightDir, normalize(-light.direction));
// 		float epsilon = light.cutOff - light.outerCutOff;
// 		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

// 		diffuseColor *= intensity;
// 		specularColor *= intensity;

// 		return vec4((ambientColor + diffuseColor + specularColor), 1.0);
// 	}

// 	if(lightType == 3)
// 	{
// 		// point
// 		float dist = length(light.position - fragPos);
// 		float attenuation =  1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

// 		return vec4((ambientColor + diffuseColor + specularColor) * attenuation, 1.0);
// 	}
// }

void main()
{
	// vec3 ambientColor = spotLight.ambient * vec3(texture(diffuse_map1, texCoord));

	// vec3 diffuseColor = vec3(texture(diffuse_map1, texCoord)) * spotLight.diffuse;
	
	FragColor = texture(diffuse_map1, texCoord);
}