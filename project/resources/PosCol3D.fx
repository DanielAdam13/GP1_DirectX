// -------------------------
//   Input/Ouput Structs
// -------------------------
struct VS_INPUT
{
    float3 Position : POSITION;
    float3 InColor : COLOR;
    float2 UV : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float3 OutColor : COLOR;
    float2 UV : TEXCOORD;
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
//SamplerState samPoint
//{
//    Filter = MIN_MAG_MIP_POINT;
//    AddressU = Wrap; // Ohter options: Mirror, Clamp, Border
//    AddressV = Wrap;
//};
//SamplerState samLinear
//{
//    Filter = MIN_MAG_MIP_LINEAR;
//    AddressU = Wrap; // Ohter options: Mirror, Clamp, Border
//    AddressV = Wrap;
//};
//SamplerState samAnisotropic
//{
//    Filter = MIN_MAG_MIP_ANISOTROPIC;
//    MaxAnisotropy = 16;
//    AddressU = Wrap; // Ohter options: Mirror, Clamp, Border
//    AddressV = Wrap;
//};
SamplerState gSampler;

// -------------------------
//   Shader Functions
// -------------------------

// Vertex Shader
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Position = mul(float4(input.Position, 1.f), gWorldViewProj);
    output.OutColor = input.InColor;
    output.UV = input.UV;
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
    //pass P1
    //{
    //    SetVertexShader(CompileShader(vs_5_0, VS()));
    //    SetGeometryShader(NULL);
    //    SetPixelShader(CompileShader(ps_5_0, PS_Linear()));
    //}
    //pass P2
    //{
    //    SetVertexShader(CompileShader(vs_5_0, VS()));
    //    SetGeometryShader(NULL);
    //    SetPixelShader(CompileShader(ps_5_0, PS_Aniso()));
    //}
}