struct VertexIn
{
    float4 posH : SV_POSITION;
    float4 color : COLOR;
};

float4 PS(VertexIn pIn) : SV_TARGET
{
    return pIn.color;
}