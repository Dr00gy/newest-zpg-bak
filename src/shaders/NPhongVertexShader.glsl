#version 450
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tg; 

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 fragmentWorldPosition;

out vec3 vnc;
out vec2 uvc;
out mat3 tbn;

void main () {
	
	 gl_Position = modelMatrix * vec4(vp, 1.0);
	 vec4 position = modelMatrix * vec4(vp,1.0f);
	 fragmentWorldPosition = position.xyz/position.w;
	 mat3 normalMat = transpose(inverse(mat3(modelMatrix)));

	//Gram–Schmidt orthonormalization
	vec3 _normal = normalize(vn);
    vec3 _tangent = normalize(tg);
    _tangent = normalize(_tangent - dot(_tangent, _normal) * _normal);
    vec3 _bitangent = cross(_normal, _tangent);

	 //TBN Matrix
	 vec3 T = normalize(vec3(normalMat * _tangent));
	 vec3 B = normalize(vec3(normalMat * _bitangent));
	 vec3 N = normalize(vec3(normalMat * _normal));
	 tbn = mat3(T, B, N);

	 uvc = uv;
     vnc = normalMat * vn;;
}
