cbuffer ConstantBuffer
{
    Matrix WorldViewProjection;
    Matrix World;
    float4 MaterialColour;
    float4 AmbientLightColour;
    float4 DirectionalLightColour;
    float4 DirectionalLightVector;
};

Texture2D Texture;
SamplerState ss;

struct VertexIn
{
    float3 InputPosition : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

struct VertexOut
{
    float4 OutputPosition : SV_POSITION;
    float4 Colour : COLOR;
    float2 TexCoord : TEXCOORD;
};



VertexOut VS(VertexIn vin)
{
    VertexOut vout;
	
	// Transform to homogeneous clip space.
    vout.OutputPosition = mul(WorldViewProjection, float4(vin.InputPosition, 1.0f));
    vout.Colour = AmbientLightColour;
    vout.TexCoord = vin.TexCoord;
    return vout;
	
	/*VertexOut vout;
	
	vout.OutputPosition = mul(WorldViewProjection, float4(vin.InputPosition, 1.0f));
    //vout.outNormal = vin.Normal;
    float4 norm = (mul(World, float4(vin.Normal, 0.0f)));
	


	// Dot product of adjusted normal and vector back to the light source
    float diffuseLight = saturate(normalize(dot(norm,  - DirectionalLightVector)));
   
	// Calculate the amount of diffuse light hitting the vertex
	// Normalize it and ensure it's between 0 and 1
    float4 lighting = (DirectionalLightColour * diffuseLight );

	// Add ambient light and ensure each component is between 0 and 1
    lighting += AmbientLightColour;
	lighting = saturate(lighting);


	 
    vout.Colour = lighting * MaterialColour ;


    return vout; */

}


float4 PS(VertexOut pin) : SV_Target
{
    
    return pin.Colour * Texture.Sample(ss, pin.TexCoord);

}
