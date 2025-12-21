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
//  Textures
// -------------------------
Texture2D gDiffuseMap : DiffuseMap;
Texture2D gNormalMap : NormalMap;
Texture2D gSpecularMap : SpecularMap;
Texture2D gGlossinessMap : GlossinessMap;

// -------------------------
//   Sampler States (How to Sample Texture)
// -------------------------
SamplerState gSampler;

// -------------------------
//   Shading Variables
// -------------------------
float4x4 gWorldMatrix : WORLD;
float3 gCameraPos : CAMERA;

struct LIGHT
{
    float3 LightDirection : LightDir;
    float LightIntensity : Intensity;
};

static const LIGHT gLight1 = { normalize(float3(0.577f, -0.577f, 0.577f)), 7.f };

const float PI = 3.1415f;

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
    output.Normal = mul(normalize(input.Normal), (float3x3) gWorldMatrix); // Transformed Normal to World
    output.Tangent = mul(normalize(input.Tangent), (float3x3) gWorldMatrix); // Transformed Tangent to World
    return output;
}

// Pixel Shader Logic
float3 SampleNormal(float2 uv)
{
    float3 normal = gNormalMap.Sample(gSampler, uv).xyz;
    return normalize(normal * 2.f - 1.f);
}
float3 GetLambertColor(VS_OUTPUT input)
{
    const float3 N = normalize(input.Normal);
    const float3 T = normalize(input.Tangent);
    const float3 B = normalize(cross(N, T)); // Left Handed
    
    float3x3 TBN = float3x3(T, B, N);
    
    float3 tangentSpaceNormal = SampleNormal(input.UV);
    float3 worldNormal = normalize(mul(tangentSpaceNormal, TBN));
    
    float3 lightDir = normalize(-gLight1.LightDirection);
    float cosTheta = saturate(dot(worldNormal, lightDir));
    float3 albedo = gDiffuseMap.Sample(gSampler, input.UV).rgb;
    const float diffuseReflectance = 1.f;
    
    float3 LambertColor = (albedo * diffuseReflectance / PI) * cosTheta * gLight1.LightIntensity;
    
    return LambertColor;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return float4(GetLambertColor(input), 1.f);
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