Texture2D g_Tex : register(t0);
Texture2D g_TexShadow : register(t1);
SamplerState g_SamLinear : register(s0);

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
    float2 tex : TEXCOORD0;

    float4 lightPosH : TEXCOORD1;
};

cbuffer ConstantBuffer : register(b0)
{
    matrix g_World; 
    matrix g_View;  
    matrix g_Proj; 
    matrix g_WorldInvTranspose;

    matrix g_Light;
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

    vOut.lightPosH = mul(float4(vIn.pos, 1.0f), g_World);
    vOut.lightPosH = mul(vOut.lightPosH, g_Light);


    return vOut;
}


float ShadowCalculation(float4 lightPosH)
{
    float4 shadowPos = lightPosH / lightPosH.w;
    shadowPos.x = (shadowPos.x + 1) * 0.5;
    shadowPos.y = (-shadowPos.y + 1) * 0.5;
    float depth = g_TexShadow.Sample(g_SamLinear, shadowPos.xy).r;
    float bias = 0.003;

    uint textureWidth, textureHeight;
    g_TexShadow.GetDimensions(textureWidth, textureHeight);
    float2 texSize = float2(1.0 / textureWidth,1.0/textureHeight);

    float res = 0.0;
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = g_TexShadow.Sample(g_SamLinear, shadowPos.xy + float2(x,y) * texSize).r;
            if(pcfDepth + bias < shadowPos.z){
                res += 1.0;     
            }
            
        }
    }

    if(shadowPos.z > 1.0){
        res = 0.0;
    }
        
    return res / 9.0;
}

float4 PS(VertexOut pIn) : SV_TARGET
{
    float4 abedo = g_Tex.Sample(g_SamLinear, pIn.tex);
   // float4 abedo = texColor1;

    float ambientStrength = 0.3f;
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

    //float3 color = (ambient + diffuse + specular) * abedo.xyz;

    float shadow = ShadowCalculation(pIn.lightPosH);

    float3 color = (ambient + (1.0 - shadow) * (diffuse + specular)) * abedo.xyz;

    return float4(color,abedo.w);
} 