#version 450 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 Color;
in vec3 FragPos;

out vec4 fColor;

struct Light{
	vec3 position;
	vec3 color;
	vec3 attenuation;
	float ambient;
};

struct Material{
	sampler2D diffuseMap;

	float shine;
	float alpha;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;
uniform Material material;
uniform vec3 cameraPos;

void main(){
	// Calculating the stuff that never actually changes
	vec3 matColor = texture(material.diffuseMap, TexCoords).rgb; // For ambient/diffuse calculations
	vec3 norm = normalize(Normal); // For diffuse/specular calculations
	vec3 viewDir = normalize(cameraPos - FragPos); // For specular calculations
	// Initialize all these variables outside of the loop to save a bit of computation time
	vec3 ambient, diffuse, specular, lightDir, reflectDir;
	float diff, spec, atten, dist;
	// Calculating Ambient
	ambient = light.ambient * material.ambient * matColor * light.color;
	// Calculating Diffuse
	lightDir = normalize(light.position - FragPos);
	diff = max(dot(norm, lightDir), 0.0);
	diffuse = diff * material.diffuse * matColor * light.color;
	// Calculating Specular
	reflectDir = reflect(-lightDir, norm);
	spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
	specular = material.specular * spec * light.color;
	// Add the current light's effects to the end result
	vec3 result = ambient + specular + diffuse;
	fColor = vec4(result, material.alpha);
}