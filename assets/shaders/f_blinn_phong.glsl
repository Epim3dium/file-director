#version 330

struct Light {
    //if w = 0 then it is directional light
    vec4 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
};
uniform Light light; 

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;


uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;
uniform sampler2D height0;

in vec2 fTex;
in vec3 fNormal;
in vec3 fPos;

in vec3 fLightDir;
in vec3 fViewDir;
in mat4 TBN;

out vec4 pixelColor;
vec2 parallaxTexCoords(vec4 v, vec2 t, float h, float s) {
	vec2 ti = -h*v.xy / s;
	float hi = -1 / s;

	vec2 tc = t;
	float hc = 1;
    //height
	float ht = texture(height0, tc).r;

	// if (v.z <= 0) discard;

	while (hc > ht) {
		tc = tc + ti;
		if (tc.x < 0 || tc.x>1 || tc.y < 0 || tc.y>1) discard;
		hc = hc + hi;
        //height
		ht = texture(height0, tc).r;
	}

	vec2 tco = tc - ti;
	float hco = hc - hi;
    //height
	float hto = texture(height0, tco).r;

	float x = (hco - hto) / (hco - hto - (hc - ht));

	return (1-x)*tco+x*tc;
}

void main(void) {
    vec3 viewDir =  normalize(fViewDir);

    if(dot(viewDir, fNormal) < 0.f)
        discard;

    vec3 lightDir = normalize(fLightDir);
    if(light.position.w == 0) { //treat as directional
        lightDir = normalize(vec3(-light.position));
    }

    vec3 halfwayDir = normalize(lightDir + viewDir);

    // vec2 tex = parallaxTexCoords(vec4(viewDir, 0), fTex, 0.1, 10);
    vec2 normTex = fTex - floor(fTex);
    vec2 tex = parallaxTexCoords(vec4(viewDir, 0), normTex, 0.001, 10);
    vec3 norm = texture(normal0, tex).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    norm = vec3(normalize(TBN * vec4(norm, 0)));
    // vec3 norm = normalize(fNormal);

    float diff = max(dot(norm, lightDir), 0.0);
    float shininess = 50.0;
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);

    vec4 color = texture(diffuse0, tex);
    
    vec4 specularColor = texture(specular0, tex);
    vec4 specular = vec4(light.specular, 1) * spec * specularColor;
    vec4 diffuse = diff * vec4(light.diffuse, 1) * color;
    vec4 ambient = vec4(light.ambient, 1) * color;
    
    vec4 result = diffuse + ambient + specular;
    pixelColor = result;
}
