#version 330 core
  
// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightPosition_worldspace0;
uniform vec3 LightPosition_worldspace1;
uniform vec3 LightPosition_worldspace2;
uniform vec3 LightPosition_worldspace3;
uniform vec3 LightPosition_worldspace4;
uniform vec3 LightPosition_worldspace5;
uniform vec3 LightPosition_worldspace6;
uniform vec3 LightPosition_worldspace7;
uniform vec3 LightPosition_worldspace8;
uniform vec3 LightPosition_worldspace9;

layout(location = 0) in vec3 coords;
layout(location = 1) in vec3 colors;
layout(location = 2) in vec3 normals;

// Output data ; will be interpolated for each fragment.
out vec3 fragmentColor;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace0;
out vec3 LightDirection_cameraspace1;
out vec3 LightDirection_cameraspace2;
out vec3 LightDirection_cameraspace3;
out vec3 LightDirection_cameraspace4;
out vec3 LightDirection_cameraspace5;
out vec3 LightDirection_cameraspace6;
out vec3 LightDirection_cameraspace7;
out vec3 LightDirection_cameraspace8;
out vec3 LightDirection_cameraspace9;
out vec3 fragmentPosition;

void main()
{
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(coords,1);
	
	// Position of the vertex, in worldspace : M * position
	Position_worldspace = (M * vec4(coords,1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( V * M * vec4(coords,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space.

	LightDirection_cameraspace0 = ( V * vec4(LightPosition_worldspace0,1)).xyz;// + EyeDirection_cameraspace;
	LightDirection_cameraspace1 = ( V * vec4(LightPosition_worldspace1,1)).xyz;// + EyeDirection_cameraspace;
	LightDirection_cameraspace2 = ( V * vec4(LightPosition_worldspace2,1)).xyz;// + EyeDirection_cameraspace;
	LightDirection_cameraspace3 = ( V * vec4(LightPosition_worldspace3,1)).xyz;// + EyeDirection_cameraspace;
	LightDirection_cameraspace4 = ( V * vec4(LightPosition_worldspace4,1)).xyz;// + EyeDirection_cameraspace;
	LightDirection_cameraspace5 = ( V * vec4(LightPosition_worldspace5,1)).xyz;// + EyeDirection_cameraspace;
	LightDirection_cameraspace6 = ( V * vec4(LightPosition_worldspace6,1)).xyz;// + EyeDirection_cameraspace;
	LightDirection_cameraspace7 = ( V * vec4(LightPosition_worldspace7,1)).xyz;// + EyeDirection_cameraspace;
	LightDirection_cameraspace8 = ( V * vec4(LightPosition_worldspace8,1)).xyz;// + EyeDirection_cameraspace;
	LightDirection_cameraspace9 = ( V * vec4(LightPosition_worldspace9,1)).xyz;// + EyeDirection_cameraspace;

	
	// Normal of the the vertex, in camera space
	Normal_cameraspace = ( V * M * vec4(coords,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.

	fragmentColor = colors;
	fragmentPosition = coords;
}