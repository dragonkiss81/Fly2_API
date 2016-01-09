float4x4 mWVP;
float4x4 mWorld;
float4 mainLightPosition;
float4 mainLightColor;
float4 ambLgt;
float4 dif;
float4 amb;
float4 camPosition;

texture sinTanMap;
sampler sinTanMapSampler = sampler_state
{
   Texture = <sinTanMap>;
   MinFilter = LINEAR;
   MagFilter = LINEAR;
   MipFilter = NONE;

   AddressU = Clamp;
   AddressV = Clamp;   
};

struct VS_OUTPUT
{
   float4 pos  : POSITION;
   float3 norm : TEXCOORD0;
   float3 lgt  : TEXCOORD1;
   float3 cam  : TEXCOORD2;
};
   
VS_OUTPUT OrenNayar_VS(float4 inPos  : POSITION,
                       float3 inNorm : NORMAL)
{
   float4 a;
   VS_OUTPUT out1 = (VS_OUTPUT) 0;
      
   a = mul(inPos, mWorld);
   out1.pos = mul(inPos, mWVP);
   out1.norm = mul(inNorm, (float3x3) mWorld);
   out1.lgt = normalize(mainLightPosition.xyz - a.xyz);
   out1.cam = normalize(camPosition.xyz - a.xyz);

   return out1;
}
   
void OrenNayar_PS(in float3 vNorm : TEXCOORD0,
                  in float3 vLgt  : TEXCOORD1,
                  in float3 vCam  : TEXCOORD2,
                  out float4 oCol  : COLOR0)
{
   const float weight = 1.5f;
   const float s = 0.7f;
   const float s2 = s * s;
   const float A = 1 - 0.5 * s2 / (s2 + 0.33);
   const float B = 0.45 * s2 / (s2 + 0.09);

   // calculate all vectors
   float3 N = normalize(vNorm);
   float3 L = normalize(vLgt);
   float3 V = normalize(vCam);
   float NL = dot(N, L);
   float VN = dot(V, N);
   
   // fetch texture with sin(alpha) + tan(alpha)
   float sinTan = tex2D(sinTanMapSampler, float2(saturate(NL), saturate(VN))).x;	
   
   float3 LProjected = normalize(L - NL*N);
   float3 VProjected = normalize(V - VN*N);
   float C = saturate(dot(LProjected, VProjected));
   
   oCol.rgb = ambLgt*amb + weight*dif*saturate(NL * (A + B*C*sinTan))*mainLightColor;	
   oCol.a = dif.a;
}

technique OrenNayar
{
   pass P0
   {
      VertexShader = compile vs_2_0 OrenNayar_VS();
      PixelShader = compile ps_2_0 OrenNayar_PS();
   }
}