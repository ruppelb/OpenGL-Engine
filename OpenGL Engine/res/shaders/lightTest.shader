#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

out vec3 norm;
out vec3 fragment_position;

uniform mat4 u_MVP;
uniform mat4 u_normalMat;
uniform mat4 u_model;

void main()
{
	gl_Position = u_MVP * position;

	fragment_position = vec3(u_model * position);
	norm = normalize(vec3(u_normalMat * normal));
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

struct Light {
	vec4 vector; //(either position or direction depending on w coordinate 1.0 or 0.0
	//float linear;
	//float quadratic;
	float falloff;
	float radius;
	vec3 color;
	vec3 ambient;
};

#define NR_LIGHTS 5
uniform Light lights[NR_LIGHTS];

// in's from vertex shader
in vec3 fragment_position;  
in vec3 norm;

//camera
uniform vec3 view_pos;

void main()
{
	
	vec3 view_dir = normalize(view_pos - fragment_position);

	vec3 result;
	
	for(int i = 0; i < NR_LIGHTS; i++)
	{
		vec3 light_dir;
		float attenuation = 1.0;
		if(lights[i].vector.w < 0.001)
		{
			//directional light
			light_dir =  normalize(vec3(-lights[i].vector));
		}else{
			//point light
			light_dir = normalize( vec3(lights[i].vector) - fragment_position);

			float dist = distance(fragment_position, vec3(lights[i].vector));
			//attenuation = (1.0/(1.0 + dist * lights[i].linear + pow(dist, 2) * lights[i].quadratic)); //attenuation according to OpenGL lighting model

			//attenuation function according to https://lisyarus.github.io/blog/graphics/2022/07/30/point-light-attenuation.html
			float s = dist / lights[i].radius;
			if (s >= 1.0) {
				attenuation = 0.0;
			}
			else {
				attenuation = pow((1 - pow(s, 2)), 2) / (1.0 + lights[i].falloff * s);
			}
		}
		vec3 diff = max(dot(norm, light_dir), 0.0f) * attenuation * vec3(0.0,0.0,1.0);
		
		//specular
		vec3 halfway_dir = normalize(light_dir + view_dir);

		float specular_value;
		vec3 spec = pow(max(dot(norm, halfway_dir), 0.0), 16) * attenuation * vec3(1.0,0.0,0.0);

		//vec3 ambient = vec3(0.0,0.0,0.2);
		result +=  (diff + spec);//ambient);
	}

	color = vec4(result,1.0f);
};