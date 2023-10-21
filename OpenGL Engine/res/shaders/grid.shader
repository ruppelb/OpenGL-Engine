#shader vertex
#version 330 core

uniform mat4 u_VP;
uniform mat4 u_inverseVP;
uniform float u_nearZ;
uniform float u_farZ;
layout(location = 0) in vec4 position;

out vec3 nearPoint;
out vec3 farPoint;
out mat4 fragVP;
out float near;
out float far;

/*
// Grid position are in xy clipped space
vec3 gridPlane[6] = vec3[](
	vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
	);*/

vec3 UnprojectPoint(float x, float y, float z) {
	vec4 unprojectedPoint = u_inverseVP * vec4(x, y, z, 1.0);
	return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main()
{
    vec3 p = position.xyz;//gridPlane[gl_VertexID].xyz;
	nearPoint = UnprojectPoint(p.x, p.y, -1.0).xyz; // unprojecting on the near plane
	farPoint = UnprojectPoint(p.x, p.y, 1.0).xyz; // unprojecting on the far plane
    fragVP = u_VP;
    near = u_nearZ;
    far = u_farZ;
	gl_Position = vec4(p, 1.0);
};

#shader fragment
#version 330 core

 in vec3 nearPoint; // nearPoint calculated in vertex shader
 in vec3 farPoint; // farPoint calculated in vertex shader
 in mat4 fragVP;
 in float near;
 in float far;
 layout(location = 0) out vec4 color;

 vec4 grid(vec3 fragPos3D, float scale, bool drawAxis) {
     vec2 coord = fragPos3D.xz * scale; // use the scale variable to set the distance between the lines
     vec2 derivative = fwidth(coord);
     vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
     float line = min(grid.x, grid.y);
     float minimumz = min(derivative.y, 1);
     float minimumx = min(derivative.x, 1);
     vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
     // z axis
     if (fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
         color.z = 1.0;
     // x axis
     if (fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
         color.x = 1.0;
     return color;
 }

 float computeDepth(vec3 pos) {
     vec4 clip_space_pos = fragVP * vec4(pos.xyz, 1.0);
     float ndc_depth = (clip_space_pos.z / clip_space_pos.w);
     //clamp depth from -1 to 1 NDC range in 0 to 1 range for depth buffer
     return (1.0 - 0.0) * 0.5 * ndc_depth + (1.0 + 0.0) * 0.5;
 }

 float computeLinearDepth(vec3 pos) {
     vec4 clip_space_pos = fragVP * vec4(pos.xyz, 1.0);
     float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0; // put back between -1 and 1
     float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near)); // get linear value between 0.01 and 100
     return linearDepth / far; // normalize
 }

void main()
{
	float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);
    gl_FragDepth = computeDepth(fragPos3D);

    float linearDepth = computeLinearDepth(fragPos3D);
    float fading = max(0, (0.5 - linearDepth));

    color = grid(fragPos3D, 1, true) * float(t > 0);
    color.a *= fading;
};