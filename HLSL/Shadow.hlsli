cbuffer ConstantBuffer : register(b0)
{
    matrix g_World; 
    matrix g_View;  
    matrix g_Proj; 
}

struct VertexIn
{
    float3 pos : POSITION;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
};

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.posW = mul(float4(vIn.pos,1.0), g_World);
    vOut.posH = mul(vOut.posW, g_View);
    vOut.posH = mul(vOut.posH, g_Proj);
    return vOut;
}


float4 PS(VertexOut pIn) : SV_TARGET
{
    return float4(pIn.posH.z,pIn.posH.z,pIn.posH.z,pIn.posH.z);
}