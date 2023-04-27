precision mediump float;

varying vec2 v_uv;

uniform vec4 u_PartColor;
uniform sampler2D u_texture; 

void main()
{
	vec4 texel = texture2D(u_texture, v_uv); 
	gl_FragColor = u_PartColor* texel;
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
