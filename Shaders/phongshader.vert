#version 330 core
layout(location = 0) in vec3 vertexPosition;    //renamed from earlier shaders
layout(location = 1) in vec3 vertexNormal;      //renamed from earlier shaders
layout(location = 2) in vec2 vertexUV;          //not used

out vec3 fragmentPosition;
out vec3 normalTransposed;
out vec3 cameraPosition;
//out vec2 UV;

uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

void main() {
   fragmentPosition = vec3(mMatrix * vec4(vertexPosition, 1.0));
   normalTransposed = mat3(transpose(inverse(mMatrix))) * vertexNormal;

   cameraPosition = vec3(vMatrix[3][0], vMatrix[3][1], vMatrix[3][2]);
//   UV = vertexUV;
   gl_Position = pMatrix * vMatrix * mMatrix * vec4(vertexPosition, 1.0);
}

//Using calculations in world space,
//https://learnopengl.com/Lighting/Basic-Lighting
//but could just as easy be done in camera space
//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/
