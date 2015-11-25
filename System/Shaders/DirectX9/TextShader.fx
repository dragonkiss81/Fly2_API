// This shader implements Phong shading with one color texture
// C.Wang 0624, 2013
//

float4x4 mWVP;
float4 TextColor = { 1.0f, 1.0f, 1.0f, 1.0f};           // text color
bool beColormap = true;

texture colorMap;
sampler2D colorMapSampler = sampler_state
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
   float4 pos  : POSITION;
   float2 tex0 : TEXCOORD0;
};
   

// vertex shader
VS_OUTPUT TextVert(float4 inPos  : POSITION,
                   float2 inTex  : TEXCOORD0)
{
   VS_OUTPUT out1 = (VS_OUTPUT) 0;
      
   out1.pos = mul(inPos, mWVP);
   out1.tex0 = inTex;

   return out1;
}
   

// pixel shader
void TextFrag(in float2 vTex : TEXCOORD0,
              out float4 oCol  : COLOR0)
{
   // the ambient term  
   oCol = TextColor;

   if (beColormap) {
      float4 colorT = tex2D(colorMapSampler, vTex);
      oCol *= colorT; 
  }
}

technique TextShader
{
   pass P0
   {
      VertexShader = compile vs_3_0 TextVert();
      PixelShader = compile ps_3_0 TextFrag();
   }
}