
cbuffer ConstantBuffer : register(b0)
{
    matrix g_World; 
    matrix g_View;  
    matrix g_Proj; 
}

struct VertexIn
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    float4 color : COLOR;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

Texture2D g_Tex : register(t0);
SamplerState g_SamLinear : register(s0);
Texture2D g_Tex2 : register(t1);


VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.color = vIn.color;
    vOut.posH = mul(float4(vIn.pos,1.0), g_World);
    vOut.posH = mul(vOut.posH, g_View);
    vOut.posH = mul(vOut.posH, g_Proj);
    vOut.tex = vIn.tex;
    return vOut;
}

float4 PS(VertexIn pIn) : SV_TARGET
{
    float4 texColor1 = g_Tex.Sample(g_SamLinear, pIn.tex);
    float4 texColor2 = g_Tex2.Sample(g_SamLinear, pIn.tex);
    return texColor1 * 0.8 + texColor2 * 0.2;
}