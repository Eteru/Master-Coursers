#version 330 core

// Interpolated values from the vertex shaders
in vec3 fragmentColor;
in vec3 fragmentPosition;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace0;
in vec3 LightDirection_cameraspace1;
in vec3 LightDirection_cameraspace2;
in vec3 LightDirection_cameraspace3;
in vec3 LightDirection_cameraspace4;
in vec3 LightDirection_cameraspace5;
in vec3 LightDirection_cameraspace6;
in vec3 LightDirection_cameraspace7;
in vec3 LightDirection_cameraspace8;
in vec3 LightDirection_cameraspace9;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform mat4 V;
// uniform vec3 LightPosition_worldspace1;
// uniform vec3 LightPosition_worldspace2;

const int numberOfLights = 10;
vec4 LightColor[numberOfLights];
vec3 LightDir[numberOfLights];

void main()
{
	// Center of aurora
	vec3 centerPosition = vec3(0.0, 0.0, 0.0);
	float opacity = 100.0;
	float cutoff = cos(14);
	float outerCutoff = cos(17.5);
	// compute distance on x & y
	/*float fragmentDist = sqrt(fragmentPosition.x * fragmentPosition.x + fragmentPosition.y * fragmentPosition.y);
	fragmentDist = (fragmentDist > 25) ? fragmentDist - 25 : 0;
	opacity -= fragmentDist;
	opacity /= 100.f;*/

	// Light emission properties
	LightColor[0] = vec4(0.32, 0.49, 0.36, 1);
	LightColor[1] = vec4(1.0, 0.4, 0.33, 1);
	LightColor[2] = vec4(0.32, 0.77, 0.56, 1);
	LightColor[3] = vec4(1.0, 0.0, 0.0, 1);
	LightColor[4] = vec4(0.0, 1.0, 1.0, 1);
	LightColor[5] = vec4(1.0, 0.0, 0.1, 1);
	LightColor[6] = vec4(1.0, 1.0, 0.0, 1);
	LightColor[7] = vec4(0.0, 1.0, 0.0, 1);
	LightColor[8] = vec4(1.0, 0.0, 0.0, 1);
	LightColor[9] = vec4(0.0, 0.0, 1.0, 1);


	LightDir[0] = LightDirection_cameraspace0;
	LightDir[1] = LightDirection_cameraspace1;
	LightDir[2] = LightDirection_cameraspace2;
	LightDir[3] = LightDirection_cameraspace3;
	LightDir[4] = LightDirection_cameraspace4;
	LightDir[5] = LightDirection_cameraspace5;
	LightDir[6] = LightDirection_cameraspace6;
	LightDir[7] = LightDirection_cameraspace7;
	LightDir[8] = LightDirection_cameraspace8;
	LightDir[9] = LightDirection_cameraspace9;

	float LightPower = 5.0f;
	
	// Material properties
	vec4 MaterialDiffuseColor = vec4(fragmentColor, 0);

	// Accumulators
	vec4 MaterialDiffuseAcc = vec4(0.0, 0.0, 0.0, 0);

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal_cameraspace );
	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);

	int noLightOnFragment = 0;

	for (int i = 0; i < numberOfLights; ++i) {
		vec3 LightDirection_cameraspace = LightDir[i];

		// Distance to the light
		float distance = length( fragmentPosition - Position_worldspace );

		// Direction of the light (from the fragment to the light)
		vec3 l = normalize( LightDirection_cameraspace );

		// Cosine of the angle between the normal and the light direction, 
		// clamped above 0
		//  - light is at the vertical of the triangle -> 1
		//  - light is perpendicular to the triangle -> 0
		//  - light is behind the triangle -> 0
		float cosTheta = clamp( dot( n,l ), 0,1 );
		float theta = dot(n, -l);
		float epsilon = cutoff - outerCutoff;
		float intensity = clamp((theta - outerCutoff) / epsilon, 0, 1);

		MaterialDiffuseAcc += (MaterialDiffuseColor * LightPower * LightColor[i] * cosTheta / (distance*distance)) * intensity;

		if (cosTheta >= 0)
			noLightOnFragment += 1;
	}

	if (noLightOnFragment > 0) {
		MaterialDiffuseAcc = vec4(MaterialDiffuseAcc.x / noLightOnFragment,
			MaterialDiffuseAcc.y / noLightOnFragment,
			MaterialDiffuseAcc.z / noLightOnFragment,
			opacity);
	}

	color = MaterialDiffuseAcc;
}