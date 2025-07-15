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
    vOut.posH = float4(vIn.pos,1.0);
    return vOut;
}