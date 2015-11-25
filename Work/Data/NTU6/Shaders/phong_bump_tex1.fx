float4x4 mWVP;
float4x4 mWorld;
float4 mainLightPosition;
float4 mainLightColor;
float4 ambLgt;
float4 amb;
float4 dif;
float4 spe;
float power;
float4 camPosition;

// bump map data   
texture bumpMap;
sampler2D bumpMapSampler = sampler_state
{
   Texture = <bumpMap>;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;
   AddressU = Wrap;
   AddressV = Wrap;
};

// color map data   
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


// vertex shader output for bump map
struct VS_OUTPUT
{
   float4 pos  : POSITION;
   float2 tex0 : TEXCOORD0;
   float3 lgt  : TEXCOORD1;
   float3 cam  : TEXCOORD2;
};

// bump map vertex shader
VS_OUTPUT PhongBumpVS(float4 inPos   : POSITION,
                      float3 inNorm  : NORMAL,
                      float2 inTex   : TEXCOORD0,
                      float3 inTangU : TANGENT)
{
   VS_OUTPUT out1 = (VS_OUTPUT) 0;     
   float4 a = mul(inPos, mWorld);
   
   // transform the vertex
   out1.pos = mul(inPos, mWVP);
   
   // compute the 3x3 transformation matrix for world space to the tangent space
   float3x3 wToTangent;
   wToTangent[0] = mul(inTangU, (float3x3) mWorld);
   float3 inTangV = cross(inTangU, inNorm);
   wToTangent[1] = mul(inTangV, (float3x3) mWorld);
   wToTangent[2] = mul(inNorm, (float3x3) mWorld);
   
   // transform the rest data to tangent space
   out1.lgt = normalize(mul(wToTangent, mainLightPosition.xyz - a.xyz));
   out1.cam = normalize(mul(wToTangent, camPosition.xyz - a.xyz));
   
   // pass the texture coordinate
   out1.tex0 = inTex;

   return out1;
}

// bump map pixel shader
void PhongBumpPS(in float2 vTex : TEXCOORD0,
                 in float3 vLgt  : TEXCOORD1,
                 in float3 vCam  : TEXCOORD2,
                 out float4 oCol  : COLOR0)
{
   float3 lgtDir = normalize(vLgt);
   float3 hDir = normalize(normalize(vCam) + lgtDir);
   float3 normDir = (2.0f*tex2D(bumpMapSampler, vTex) - 1.0f);
   
   float diff = saturate(dot(normDir, lgtDir));
   float spec = pow(saturate(dot(hDir, normDir)), power);
   
   float3 colorT = tex2D(colorMapSampler, vTex);
   oCol.rgb = colorT*(ambLgt*amb + diff*mainLightColor*dif + spec*mainLightColor*spe);   
   oCol.a = dif.a;
}

// bump map technique
technique PhongBump
{
   pass P0
   {
      VertexShader = compile vs_2_0 PhongBumpVS();
      PixelShader = compile ps_2_0 PhongBumpPS();
   }
}