Texture2D g_Tex : register(t0);
SamplerState g_SamLinear : register(s0);
Texture2D g_Tex2 : register(t1);

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
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

cbuffer ConstantBuffer : register(b0)
{
    matrix g_World; 
    matrix g_View;  
    matrix g_Proj; 
    matrix g_WorldInvTranspose;
};

cbuffer PointLightBuffer : register(b1)
{
    float3 lightPosition;
    float3 lightColor;
    float3 eyePosW;
 
    float3 pad;
};


VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    matrix viewProj = mul(g_View, g_Proj);
    float4 posW = mul(float4(vIn.pos,1.0), g_World);
    float3x3 world = g_World;

    vOut.color = vIn.color;
    vOut.posH = mul(posW, viewProj);
    vOut.posW = posW.xyz;
    vOut.tex = vIn.tex;
    vOut.normalW = mul(vIn.normal, (float3x3)g_WorldInvTranspose);
    return vOut;
}

float4 PS(VertexOut pIn) : SV_TARGET
{
    float4 texColor1 = g_Tex.Sample(g_SamLinear, pIn.tex);
    float4 texColor2 = g_Tex2.Sample(g_SamLinear, pIn.tex);
    float4 abedo = texColor1 * 0.8 + texColor2 * 0.2;

   // float4 abedo = texColor1;

    float ambientStrength = 0.1f;
    float3 ambient = ambientStrength * lightColor;

    float3 norm = normalize(pIn.normalW);
    float3 lightDir = normalize(lightPosition - pIn.posW);
    float diff = max(dot(norm, lightDir), 0.0);
    float3 diffuse = diff * lightColor;

    float specularStrength = 0.5f;
    float3 viewDir = normalize(eyePosW - pIn.posW);
    float3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float3 specular = specularStrength * spec * lightColor;  

    float3 color = (ambient + diffuse + specular) * abedo.xyz;
    return float4(color,abedo.w);
} 