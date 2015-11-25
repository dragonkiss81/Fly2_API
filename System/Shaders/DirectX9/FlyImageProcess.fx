// Fly2 Image Processing Shaders v 1.0
// C.Wang 0409, 2015

float4x4 WorldViewProj;
float4 dif;
float3 grayColor = float3(0.299f, 0.587f, 0.114f);
float2 texSize = { 0.0, 0.0 };
float weights[5] = { 0.0545f, 0.244f, 0.403f, 0.244f, 0.0545f };    // my Gaussian Weights for 5
float threshold = 0.3;


// the textures
texture colorMap;
sampler2D colorMapSampler = sampler_state
{
   Texture = <colorMap>;
   MinFilter = Linear;
   MagFilter = Linear;
   AddressU = Wrap;
   AddressV = Wrap;
};


texture gaussianColorMap;
sampler2D gaussianColorMapSampler = sampler_state
{
   Texture = <gaussianColorMap>;
   MinFilter = Linear;
   MagFilter = Linear;
   AddressU = Wrap;
   AddressV = Wrap;
};


texture colorMapA;
sampler2D colorMapASampler = sampler_state
{
   Texture = <colorMapA>;
   MinFilter = Linear;
   MagFilter = Linear;
   AddressU = Wrap;
   AddressV = Wrap;
};


texture colorMapB;
sampler2D colorMapBSampler = sampler_state
{
   Texture = <colorMapB>;
   MinFilter = Linear;
   MagFilter = Linear;
   AddressU = Wrap;
   AddressV = Wrap;
};

   
struct VS_OUTPUT
{
   float4 pos : POSITION;
   float2 tex : TEXCOORD0;
};


float4 EncodeRGBE8(in float3 rgb)
{
   float4 vEncoded;

   // apply absolute value
   float3 aRGB = abs(rgb);

   // determine the largest color component
   float maxComp = max(max(aRGB.r, aRGB.g), aRGB.b);

   // round to the nearest integer exponent
   float fExp = ceil(log2(maxComp));

   // divide the components by the shared exponent
   float3 vRGB = rgb/exp2(fExp);
   vEncoded.rgb = vRGB*0.5 + 0.5;
	
   // store the shared exponent in the alpha channel
   vEncoded.a = (fExp + 128)/255;

   return vEncoded;
}


float3 DecodeRGBE8(in float4 rgbe)
{
   float3 vDecoded;

   // retrieve the shared exponent
   float fExp = rgbe.a*255 - 128;

   // multiply through the each color
   float3 vRGB = rgbe.rgb*2.0 - 1.0;
   vDecoded = vRGB*exp2(fExp);

   return vDecoded;
}


VS_OUTPUT PlateVS(float4 inPos : POSITION,
                  float2 inTex : TEXCOORD0)
{
   VS_OUTPUT out1 = (VS_OUTPUT) 0;

   // transform the vertex to screen
   out1.pos = mul(inPos, WorldViewProj);
   out1.pos.xy = out1.pos.xy + float2(-texSize.x, texSize.y);
            
   // texture coordinate
   out1.tex = inTex;

   return out1;
}


float4 ScalePS(in float2 vTex0 : TEXCOORD0) : COLOR
{
   float4 r = tex2D(colorMapSampler, vTex0);
   return r;
}


float4 GrayColorPS(in float2 vTex0 : TEXCOORD0) : COLOR
{
   float4 texColor = tex2D(colorMapSampler, vTex0);
   float luminance = dot(texColor.rgb, grayColor);
   float4 result = luminance.rrrr;
   result.a = dif.a;
   return result;
}


float4 SharpenPS(in float2 vTex0 : TEXCOORD0) : COLOR
{
   float2 uv;
   float4 c4 = tex2D(colorMapSampler, vTex0);

   uv = vTex0 + float2(-texSize.x, -texSize.y);
   float4 c0 = tex2D(colorMapSampler, uv);

   uv = vTex0 + float2(0.0f, -texSize.y);
   float4 c1 = tex2D(colorMapSampler, uv);

   uv = vTex0 + float2(texSize.x, -texSize.y);
   float4 c2 = tex2D(colorMapSampler, uv);

   uv = vTex0 + float2(-texSize.x, 0.0);
   float4 c3 = tex2D(colorMapSampler, uv);

   uv = vTex0 + float2(texSize.x, 0.0);
   float4 c5 = tex2D(colorMapSampler, uv);

   uv = vTex0 + float2(-texSize.x, texSize.y);
   float4 c6 = tex2D(colorMapSampler, uv);

   uv = vTex0 + float2(0.0f, texSize.y);
   float4 c7 = tex2D(colorMapSampler, uv);

   uv = vTex0 + float2(texSize.x, texSize.y);
   float4 c8 = tex2D(colorMapSampler, uv);

   float4 result = (c4*17.0 - c0 - c1 - c2 - c3 - c5 - c6 - c7 - c8)/9.0f;
   result.a = dif.a;
   return result;
}


float4 SubtractPS(in float2 vTex0 : TEXCOORD0) : COLOR
{
   float4 colorA = tex2D(colorMapASampler, vTex0);
   float4 colorB = tex2D(colorMapBSampler, vTex0);

   float3 result = colorA.rgb - colorB.rgb;
   return EncodeRGBE8(result);
}


float4 AddPS(in float2 vTex0 : TEXCOORD0) : COLOR 
{
   float4 colorA = tex2D(colorMapASampler, vTex0);   // 0~255
   float4 colorB = tex2D(colorMapBSampler, vTex0);   // fixed-point
   float3 cB = DecodeRGBE8(colorB);

   float3 result = colorA.rrr + cB.rrr;
   return float4(result, 1.0);
}


float4 FilterMultiplyPS(in float2 vTex0 : TEXCOORD0) : COLOR 
{
   float4 colorA = tex2D(colorMapASampler, vTex0);
   float3 cA = DecodeRGBE8(colorA);
   float4 colorB = tex2D(colorMapBSampler, vTex0);
   float3 cB = DecodeRGBE8(colorB);

   float3 weight = sign(cA - threshold.rrr);
   weight = weight*0.5 + 0.5;

   float3 result = cB*weight;
   return EncodeRGBE8(result);
}


float4 LaplacianPS(in float2 vTex0 : TEXCOORD0) : COLOR 
{
   float2 uv;
   float4 c4 = tex2D(colorMapSampler, vTex0);

   uv = vTex0 + float2(-texSize.x, -texSize.y);
   float4 c0 = tex2D(colorMapSampler, uv);

   uv = vTex0 + float2(0.0f, -texSize.y);
   float4 c1 = tex2D(colorMapSampler, uv);

   uv = vTex0 + float2(texSize.x, -texSize.y);
   float4 c2 = tex2D(colorMapSampler, uv);

   uv = vTex0 + float2(-texSize.x, 0.0);
   float4 c3 = tex2D(colorMapSampler, uv);

   uv = vTex0 + float2(texSize.x, 0.0);
   float4 c5 = tex2D(colorMapSampler, uv);

   uv = vTex0 + float2(-texSize.x, texSize.y);
   float4 c6 = tex2D(colorMapSampler, uv);

   uv = vTex0 + float2(0.0f, texSize.y);
   float4 c7 = tex2D(colorMapSampler, uv);

   uv = vTex0 + float2(texSize.x, texSize.y);
   float4 c8 = tex2D(colorMapSampler, uv);

   float3 result = abs(c4.rgb*8.0 - c0.rgb - c1.rgb - c2.rgb - c3.rgb - c5.rgb - c6.rgb - c7.rgb - c8.rgb);
   return EncodeRGBE8(result);
}


struct VS_OUTPUT_GAUSS
{
   float4 pos : POSITION;
   float2 tap0 : TEXCOORD0;
   float2 tap1 : TEXCOORD1;    
   float2 tap2 : TEXCOORD2;
   float2 tap1Neg : TEXCOORD3;
   float2 tap2Neg : TEXCOORD4;
};


VS_OUTPUT_GAUSS GaussianX5VS(float4 inPos : POSITION,
                             float2 inTex : TEXCOORD0)
{
   VS_OUTPUT_GAUSS out1 = (VS_OUTPUT_GAUSS) 0;

   // transform the vertex to screen
   out1.pos = mul(inPos, WorldViewProj);
   out1.pos.xy = out1.pos.xy + float2(-texSize.x, texSize.y);
            
   // texture coordinate
   out1.tap0 = inTex;
   out1.tap1 = inTex + float2(texSize.x, 0.0);	
   out1.tap2 = inTex + float2(texSize.x*2.0, 0.0);	
   out1.tap1Neg = inTex - float2(texSize.x, 0.0);	
   out1.tap2Neg = inTex - float2(texSize.x*2.0, 0.0);	

   return out1;
}


float4 Gaussian5PS(float2 tap0 : TEXCOORD0,
                   float2 tap1 : TEXCOORD1,    
                   float2 tap2 : TEXCOORD2,
                   float2 tap1Neg : TEXCOORD3,
                   float2 tap2Neg : TEXCOORD4) : COLOR
{
   float4 color[5];
   color[0] = tex2D(gaussianColorMapSampler, tap0)*weights[2]; 
   color[1] = tex2D(gaussianColorMapSampler, tap1)*weights[3];
   color[2] = tex2D(gaussianColorMapSampler, tap2)*weights[4];
   color[3] = tex2D(gaussianColorMapSampler, tap1Neg)*weights[1];
   color[4] = tex2D(gaussianColorMapSampler, tap2Neg)*weights[0];

   float4 result = color[0] + color[1] + color[2] + color[3] + color[4];
   return result;
}


VS_OUTPUT_GAUSS GaussianY5VS(float4 inPos : POSITION,
                             float2 inTex : TEXCOORD0)
{
   VS_OUTPUT_GAUSS out1 = (VS_OUTPUT_GAUSS) 0;

   // transform the vertex to screen
   out1.pos = mul(inPos, WorldViewProj);
   out1.pos.xy = out1.pos.xy + float2(-texSize.x, texSize.y);
            
   // texture coordinate
   out1.tap0 = inTex;
   out1.tap1 = inTex + float2(0.0, texSize.y);	
   out1.tap2 = inTex + float2(0.0, texSize.y*2.0);	
   out1.tap1Neg = inTex - float2(0.0, texSize.y);	
   out1.tap2Neg = inTex - float2(0.0, texSize.y*2.0);	

   return out1;
}


technique ScaleImage
{
   pass P0
   {
      VertexShader = compile vs_3_0 PlateVS();
      PixelShader = compile ps_3_0 ScalePS();
   }
}   


technique GrayColor
{
   pass P0
   {
      VertexShader = compile vs_3_0 PlateVS();
      PixelShader = compile ps_3_0 GrayColorPS();
   }
}   


technique SharpenShader
{
   pass P0
   {
      VertexShader = compile vs_3_0 PlateVS();
      PixelShader = compile ps_3_0 SharpenPS();
   }
}   


technique GaussX5
{
   pass P0
   {
      VertexShader = compile vs_3_0 GaussianX5VS();
      PixelShader = compile ps_3_0 Gaussian5PS();
   }
}   


technique GaussY5
{
   pass P0
   {
      VertexShader = compile vs_3_0 GaussianY5VS();
      PixelShader = compile ps_3_0 Gaussian5PS();
   }
}   


technique ImageSubtract
{
   pass P0
   {
      VertexShader = compile vs_3_0 PlateVS();
      PixelShader = compile ps_3_0 SubtractPS();
   }
}   


technique ImageAdd
{
   pass P0
   {
      VertexShader = compile vs_3_0 PlateVS();
      PixelShader = compile ps_3_0 AddPS();
   }
}   


technique LaplacianFilter
{
   pass P0
   {
      VertexShader = compile vs_3_0 PlateVS();
      PixelShader = compile ps_3_0 LaplacianPS();
   }
}   


technique ImageFilterMultiply
{
   pass P0
   {
      VertexShader = compile vs_3_0 PlateVS();
      PixelShader = compile ps_3_0 FilterMultiplyPS();
   }
}   
