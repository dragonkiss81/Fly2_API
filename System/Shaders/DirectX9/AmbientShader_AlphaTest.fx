// This shader implements Phong shading with one color texture
// C.Wang 0624, 2013
//

float4x4 ModelviewProjMatrix;
float4x4 WorldMatrix;
float4 lgtPos;
float4 lgtCol;
float3 camPos;
float4 Kd;
float4 Ka;
float4 skyColor = { 1.0f, 1.0f, 1.0f, 1.0f};           // sky color for ambient
float4 groundColor = { 0.4f, 0.4f, 0.4f, 1.0f};        // ground color for ambient

   
texture Texture0;
sampler2D colorMapSampler = sampler_state
{
   Texture = <Texture0>;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;
   AddressU = Wrap;
   AddressV = Wrap;
};
   
struct VS_OUTPUT
{
   float4 pos  : POSITION;
   float2 tex0 : TEXCOORD0;
   float3 norm : TEXCOORD1;
   float3 lgt  : TEXCOORD2;
   float3 cam  : TEXCOORD3;
};
   

// vertex shader
VS_OUTPUT PhongLight1(float4 inPos  : POSITION,
                      float3 inNorm : NORMAL,
                      float2 inTex  : TEXCOORD0)
{
   float4 wPos;
   VS_OUTPUT out1 = (VS_OUTPUT) 0;
      
   wPos = mul(inPos, WorldMatrix);
   out1.pos = mul(inPos, ModelviewProjMatrix);
   out1.norm = mul(inNorm, (float3x3) WorldMatrix);

   // setup lighting direction
   out1.lgt = normalize(lgtPos.xyz - wPos.xyz);
   out1.cam = normalize(camPos.xyz - wPos.xyz);
   out1.tex0 = inTex;

   return out1;
}
   

// pixel shader
void PhongTex1Light1(in float2 vTex : TEXCOORD0,
                     in float3 vNorm : TEXCOORD1,
                     in float3 vLgt  : TEXCOORD2,
                     in float3 vCam  : TEXCOORD3,
                    out float4 oCol  : COLOR0)
{
   float3 normDir = normalize(vNorm);
   float3 lgtDir = normalize(vLgt);
   float3 skyDir = float3(0.0f, 0.0f, 1.0f);
      
   float diff = dot(normDir, lgtDir)*0.5f + 0.5f;   // half Lambert
   float3 ambient = lerp(groundColor, skyColor, dot(normDir, skyDir)*0.5 + 0.5);
      
   oCol.rgb = ambient*Ka.rgb + diff*lgtCol*Kd.rgb;
   oCol.a = Kd.a;

   float4 colorT = tex2D(colorMapSampler, vTex);
   oCol = oCol*colorT;
}

technique AmbientShader_AlphaTest
{
   pass P0
   {
      AlphaTestEnable = TRUE;	
      AlphaFunc = GREATER;
      AlphaRef = 6;		
      AlphaBlendEnable = TRUE;

      VertexShader = compile vs_3_0 PhongLight1();
      PixelShader = compile ps_3_0 PhongTex1Light1();
   }
}