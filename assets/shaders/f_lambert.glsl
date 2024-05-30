#version 330

uniform float ambientLight = 0;

out vec4 pixelColor; //Output variable of the fragment shader. (Almost) final pixel color.
//Varying variables
in vec4 fColor;
in vec4 fLight;//eye space
in vec4 fNormal; //eye space
in vec4 fViewer;//eye space

void main(void) {
	//Normalized, interpolated vectors
	vec4 ml = normalize(fLight);
	vec4 mn = normalize(fNormal);
	vec4 mv = normalize(fViewer);
	//Reflected vector
	vec4 mr = reflect(-ml, mn);

	//Surface parameters
	vec4 kd = fColor;
	vec4 ks = vec4(1,1,1,1);

	//Lighting model computation
	float nl = clamp(dot(mn, ml), 0, 1);
	float rv = pow(clamp(dot(mr, mv), 0, 1), 50);
	pixelColor = vec4(kd.rgb * min(nl + ambientLight, 0.9), kd.a) + vec4(ks.rgb * rv, 0);
	// pixelColor=fColor;
}
