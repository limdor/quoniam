#version 150

vec3 ThermalScale(in float pPercent)
{
    vec3 returnColor;

    if ( pPercent < 0.25f )
    {
        pPercent = pPercent * 4;// / 0.25;
        returnColor = vec3( 0.0, pPercent, 1.0 );
    }
    else if ( pPercent < 0.5f )
    {
        pPercent = (pPercent - 0.25f) * 4;// / 0.25;
        returnColor = vec3( 0.0 , 1.0 , 1.0 - pPercent );
    }
    else if ( pPercent < 0.75f )
    {
        pPercent = (pPercent - 0.5f) * 4;// / 0.25;
        returnColor = vec3( pPercent , 1.0 , 0.0 );
    }
    else
    {
        pPercent = (pPercent - 0.75f) * 4;// / 0.25;
        returnColor = vec3( 1.0 , 1.0 - pPercent , 0.0 );
    }

    return returnColor;
}
