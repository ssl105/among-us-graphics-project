#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 posOutput;
in vec3 normalOutput;
in vec2 TexCoord;

// point light
struct PointLight {
    vec3 pos;
    vec3 col;
    vec3 atten;
};

// directional lighting
struct DirLight{
    vec3 dir;
    vec3 col;
};

// materials
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform sampler2D texture1;
uniform bool toon;
uniform bool texShade;
uniform vec3 viewPos;
uniform Material material;
uniform PointLight pLight;
uniform DirLight dirLight;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 norm, vec3 pos, vec3 viewDir);

void main()
{
    
    vec3 norm = normalize(normalOutput);
    vec3 viewDir = normalize (viewPos - posOutput);
    
    vec3 result = CalcDirLight(dirLight, norm, viewDir);  
    fragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.dir);
    
    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    //toon shading
    if (toon) {
        float edge = max(dot(norm, viewDir), 0.0);
        if (edge < 0.5) {
            return vec3(0);
        }

        float intensity = diff*spec;
        vec3 color = light.col * material.ambient;

        if (intensity > 0.0005) {
            color = 1.0 * color;
        } else {
            color = 0.5 * color;
        }

        return color;
    }

    // texture shade
    if (texShade) {
        vec3 ambient = light.col * vec3(texture(texture1, TexCoord));
        vec3 diffuse = light.col * diff * vec3(texture(texture1, TexCoord));
        vec3 specular = light.col * spec * material.specular;

        return (ambient + diffuse +specular);
    }


    // combine results
    vec3 ambient = light.col * material.ambient;
    vec3 diffuse = light.col * (diff * material.diffuse);
    vec3 specular = light.col * (spec * material.specular);

    return (ambient + diffuse + specular);
}

// calculates the color when using point light
vec3 CalcPointLight(PointLight light, vec3 norm, vec3 pos, vec3 viewDir) {
    // ambient shading
    vec3 ambient = light.col * material.ambient;
    
    //diffuse shading
    vec3 lightDir = normalize(light.pos - pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.col * (diff * material.diffuse);
    
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.col * (spec * material.specular);
    
    // Attenuation
    float dist = length(light.pos - pos);
    float attenuation = 1.0 / (light.atten.x + light.atten.y * dist + light.atten.z * (dist * dist));
    
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse+ specular);
}
