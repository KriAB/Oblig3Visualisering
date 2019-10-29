#version 330 core
out vec4 fragColor;         //FragColor

in vec3 normalTransposed;   //Normal
in vec3 fragmentPosition;   //FragPos
//in vec2 UV;

uniform float ambientStrengt;   // hardcoded in tutorial
uniform vec3 lightPosition;     // lightPos
in vec3 cameraPosition;    // viewPos
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform float lightPower;
uniform float specularStrength;
uniform int specularExponent;
//uniform sampler2D textureSampler;

void main() {
    //ambient
    vec3 ambient = ambientStrengt * lightColor;

    //diffuse
    vec3 normalCorrected = normalize(normalTransposed);
    vec3 lightDirection = normalize(lightPosition - fragmentPosition);
    float diff = max(dot(normalCorrected, lightDirection), 0.0);
    vec3 diffuse = diff * objectColor * lightColor * lightPower;

    //specular
    vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
    float spec = 0.0;
    if (diff > 0.0)
    {
        vec3 reflectDirection = reflect(lightDirection, normalCorrected);
        spec = pow(max(dot(cameraPosition, reflectDirection), 0.0), specularExponent);
    }
    vec3 specular = spec * lightColor * specularStrength;

    vec3 result = ambient + diffuse; // + specular;
    fragColor = vec4(result, 1.0);
}

//Using calculations in world space,
//https://learnopengl.com/Lighting/Basic-Lighting
//but could just as easy be done in camera space
//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/
