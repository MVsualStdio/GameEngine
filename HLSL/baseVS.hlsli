
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
};

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.color = vIn.color;
    vOut.posH = mul(float4(vIn.pos,1.0), g_World);
    vOut.posH = mul(vOut.posH, g_View);
    vOut.posH = mul(vOut.posH, g_Proj);
    return vOut;
}