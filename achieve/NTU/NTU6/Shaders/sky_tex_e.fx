float4x4 WorldViewProj;

// the texture
texture colorMap;
samplerCUBE colorMapSampler = sampler_state
{
   Texture = <colorMap>;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;
   AddressU = Wrap;
   AddressV = Wrap;
};
   
struct VS_OUTPUT
{
   float4 pos : POSITION;
   float4 dif : COLOR0;
   float3 tex : TEXCOORD0;
};
   
VS_OUTPUT Sky_Tex0VS(float4 inPos : POSITION,
                     float2 inTex : TEXCOORD0)
{
   VS_OUTPUT out1 = (VS_OUTPUT) 0;

   // transform the vertex to screen
   out1.pos = mul(inPos, WorldViewProj);
      
   // perform the lighting
   out1.dif = 1.0f;
      
   // texture coordinate
   out1.tex = inPos.xyz;
   return out1;
}
   
struct PS_INPUT
{
   float4 Color : COLOR;
   float3 texUV : TEXCOORD0;
};
  
float4 Sky_Tex0PS(PS_INPUT in1) : COLOR
{
   //float4 texColor = texCUBE(colorMapSampler, in1.texUV);
   float4 texColor = texCUBE(colorMapSampler, in1.texUV);
   texColor.a = 1.0f;
   return in1.Color*texColor;
}
   
technique sky_tex
{
   pass P0
   {
      VertexShader = compile vs_2_0 Sky_Tex0VS();
      PixelShader = compile ps_2_0 Sky_Tex0PS();
   }
}   
