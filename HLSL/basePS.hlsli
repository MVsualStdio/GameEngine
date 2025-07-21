struct VertexIn
{
    float4 posH : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

Texture2D g_Tex : register(t0);
SamplerState g_SamLinear : register(s0);

float4 PS(VertexIn pIn) : SV_TARGET
{
    float4 texColor1 = g_Tex.Sample(g_SamLinear, pIn.tex);
    return texColor1;
}