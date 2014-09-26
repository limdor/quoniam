#version 150

#extension GL_EXT_gpu_shader4 : enable

uniform sampler2D   diffuseTexture;

uniform bool           faceCulling;
uniform bool   applyDiffuseTexture;
uniform bool     applyIllumination;
uniform vec3        cameraPosition;

struct material
{
    vec3         ka;
    vec3         kd;
    vec3         ks;
    float shininess;
};

uniform material geometryMaterial;

struct light
{
    bool    enabled;
    vec3     lookAt;
    vec3 irradiance;
};

uniform light              light1;
uniform light              light2;
uniform float  ambientLightAmount;

in vec3    vo_NormalWS; // Vector normal en espai de món
in vec3  vo_PositionWS; // Posició en espai de món
in vec2   vo_TextCoord; // Coordenades de textura
in vec4       vo_Color; // Color per vertex

vec4 ShadeFragment()
{
    vec4 returnColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    if( gl_FrontFacing || !faceCulling )
    {
        vec3 ka = geometryMaterial.ka;
        vec3 kd = geometryMaterial.kd;
        vec3 ks = geometryMaterial.ks;
        float shininess = geometryMaterial.shininess;

        if(ka == vec3(0.0f, 0.0f, 0.0f))
        {
            ka = kd;
        }
        vec3 diffuseColor = vec3(1.0f, 1.0f, 1.0f);
        //vec3 diffuseColor = vec3(0.74f, 0.74f, 0.74f);

        if( applyDiffuseTexture )
        {
            diffuseColor = texture(diffuseTexture, vo_TextCoord).rgb;
        }

        ks *= diffuseColor;
        kd *= diffuseColor;
        ka *= diffuseColor;

        vec3 intensityAmbientLight = vec3(1.0f, 1.0f, 1.0f);
        vec3 lightsContribution = vec3(0.0f, 0.0f, 0.0f);
        if( applyIllumination )
        {
            //Bibliography: Real Time Rendering (Third Edition) (p.114)
            vec3 v = normalize( cameraPosition - vo_PositionWS);
            vec3 n = normalize( vo_NormalWS );

            if( light1.enabled )
            {
                vec3 EL1 = light1.irradiance;
                vec3 l1 = normalize( light1.lookAt );
                vec3 h1 = normalize( v + l1 );
                float cosTh1 = max( 0.0f, dot( n, h1 ) );
                float cosTi1 = max( 0.0f, dot( n, l1 ) );

                if(cosTh1 != 0.0f)
                {
                    lightsContribution += ( kd + ks*pow(cosTh1,shininess) ) * EL1 * cosTi1;
                }
                else
                {
                    lightsContribution += kd * EL1 * cosTi1;
                }
            }

            if( light2.enabled )
            {
                vec3 EL2 = light2.irradiance;
                vec3 l2 = normalize( light2.lookAt );
                vec3 h2 = normalize( v + l2 );
                float cosTh2 = max( 0.0f, dot( n, h2 ) );
                float cosTi2 = max( 0.0f, dot( n, l2 ) );

                if(cosTh2 != 0.0f)
                {
                    lightsContribution += ( kd + ks*pow(cosTh2,shininess) ) * EL2 * cosTi2;
                }
                else
                {
                    lightsContribution += kd * EL2 * cosTi2;
                }
            }
            returnColor = vec4( ambientLightAmount*ka*intensityAmbientLight + lightsContribution, 1.0f );
        }
        else
        {
            returnColor = vec4( kd, 1.0f );
        }
    }
    else
    {
        if(faceCulling)
        {
            discard;
        }
    }
    return returnColor;
}
