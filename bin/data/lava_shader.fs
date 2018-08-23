// Lava shader - Created by laserdog in 2017-11-20 : https://www.shadertoy.com/view/llsBR4

uniform vec3 iResolution;	// tile size
uniform float iTime;	//	elapsed time
uniform sampler2D tex;

// random2 function by Patricio Gonzalez
vec2 random2(vec2 st){
    st = vec2( dot(st,vec2(127.1,311.7)),
              dot(st,vec2(269.5,183.3)) );
    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}

// Value Noise by Inigo Quilez - iq/2013
// https://www.shadertoy.com/view/lsf3WH
float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f*f*(3.0-2.0*f);

    return mix( mix( dot( random2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ), 
                     dot( random2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),
                mix( dot( random2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ), 
                     dot( random2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);
}

void main()
{
    vec3 orange = vec3(1., .45, 0.);
    vec3 yellow = vec3(1., 1., 0.);
    
	vec2 uv = (2. * gl_FragCoord.xy - iResolution.xy) / iResolution.y;

    uv.y += cos(iTime / 10.) * .1 + iTime / 10.;
    uv.x *= sin(iTime * 1. + uv.y * 4.) * .1 + .8;
    uv += noise(uv * 30.25 + iTime / 5.);

    float col = smoothstep(.01,.2, noise(uv * 3.))
        + smoothstep(.01,.2, noise(uv * 6. + .5))
        + smoothstep(.01,.3, noise(uv * 7. + .2));

    orange.rg += .3 * sin(uv.y * 4. + iTime / 1.) * sin(uv.x * 5. + iTime / 1.);

    gl_FragColor.rgb = mix(yellow, orange, vec3(smoothstep(0., 1., col)));
    gl_FragColor.a = 1.0;
}