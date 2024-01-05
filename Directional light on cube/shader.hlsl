cbuffer ConstantBuffer
{
    matrix worldViewProjection;
    Matrix world;
    float4 materialColour;
    float4 ambientLightColour;
    float4 directonalLightColour;
    float4 directionalLightVector;
};

struct VertexIn
{
    float3 InputPosition : POSITION;
    float3 Normal : NORMAL;
};

struct VertexOut
{
    float4 OutputPosition : SV_POSITION;
    float4 Colour : COLOR;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
	
	// Transform to homogeneous clip space.
    vout.OutputPosition = mul(worldViewProjection, float4(vin.InputPosition, 1.0f));

    // Multiply the world transformation matrix by the normal to get the adjusted normal
    float4 adjustedNormal = mul(world, float4(vin.Normal, 0.0f));

    // Take the dot product of the adjusted normal and the vector to the light source
    float diffuseLightAmount = saturate(dot(adjustedNormal.xyz, -directionalLightVector.xyz));

    // Multiply the diffuse light amount by the directional light colour and add ambient light
    vout.Colour = saturate(materialColour * (diffuseLightAmount * directonalLightColour + ambientLightColour));

    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return pin.Colour;
}
