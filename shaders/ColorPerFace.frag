#version 150

out float outputColor;

uniform bool ignoreNormals;
uniform int offset;

void main()
{
    if(gl_FrontFacing || ignoreNormals)
    {
        float valor = float(gl_PrimitiveID + offset + 1);
        outputColor = valor;
    }
    else
    {
        outputColor = 0.0f;
    }
}
