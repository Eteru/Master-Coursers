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
vec3 LightColor[numberOfLights];
vec3 LightDir[numberOfLights];

void main()
{
	// Center of aurora
	vec3 centerPosition = vec3(0.0, 0.0, 0.0);
	float opacity = 100.0;
	// compute distance on x & y
	float fragmentDist = sqrt(fragmentPosition.x * fragmentPosition.x + fragmentPosition.y * fragmentPosition.y);
	fragmentDist = (fragmentDist > 25) ? fragmentDist - 25 : 0;
	opacity -= fragmentDist;

	// Light emission properties
	LightColor[0] = vec3(0.32, 0.49, 0.36);
	LightColor[1] = vec3(1.0, 0.4, 0.33);
	LightColor[2] = vec3(0.32, 0.77, 0.56);
	LightColor[3] = vec3(1.0, 0.0, 0.0);
	LightColor[4] = vec3(0.0, 1.0, 1.0);
	LightColor[5] = vec3(1.0, 0.0, 0.1);
	LightColor[6] = vec3(1.0, 1.0, 0.0);
	LightColor[7] = vec3(0.0, 1.0, 0.0);
	LightColor[8] = vec3(1.0, 0.0, 0.0);
	LightColor[9] = vec3(0.0, 0.0, 1.0);


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
	vec3 MaterialDiffuseColor = fragmentColor;
	vec3 MaterialAmbientColor = vec3(0.9,0.9,0.9) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.01,0.01,0.01) * MaterialDiffuseColor;

	// Accumulators
	vec3 MaterialDiffuseAcc = vec3(0.0, 0.0, 0.0);
	vec3 MaterialSpecularAcc = vec3(0.0, 0.0, 0.0);

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal_cameraspace );
	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);

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

		// Direction in which the triangle reflects the light
		vec3 R = reflect(-l,n);

		// Cosine of the angle between the Eye vector and the Reflect vector,
		// clamped to 0
		//  - Looking into the reflection -> 1
		//  - Looking elsewhere -> < 1
		float cosAlpha = clamp( dot( E,R ), 0,1 );

		MaterialDiffuseAcc += (MaterialDiffuseColor * LightPower * LightColor[i] * cosTheta / (distance*distance));
		// MaterialSpecularAcc += (MaterialSpecularColor * LightPower * LightColor[i] * pow(cosAlpha,5) / (distance*distance));
	}

	color = vec4(
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		0.4 * clamp( MaterialDiffuseAcc, 0,1 ) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularAcc,
		opacity/100.f);
}