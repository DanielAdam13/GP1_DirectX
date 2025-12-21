// -------------------------
//   Input/Ouput Structs
// -------------------------
struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Color : COLOR;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float3 Color : COLOR;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

// -------------------------
//   WorldViewProjection Matrix
// -------------------------
float4x4 gWorldViewProj : WorldViewProjection;

// -------------------------
//   Diffuse Texture Variable
// -------------------------
Texture2D gDiffuseMap : DiffuseMap;

// -------------------------
//   Sampler States (How to Sample Texture)
// -------------------------
SamplerState gSampler;

// -------------------------
//   Shader Functions
// -------------------------

// Vertex Shader
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Position = mul(float4(input.Position, 1.f), gWorldViewProj);
    output.Color = input.Color;
    output.UV = input.UV;
    output.Normal = input.Normal;
    output.Tangent = input.Tangent;
    return output;
}

// Pixel Shader
float4 PS(VS_OUTPUT input) : SV_Target
{
    return gDiffuseMap.Sample(gSampler, input.UV);
}

// -------------------------
//   Technique
// -------------------------
technique11 DefaultTechnique
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}