/*---------------
  Shadow Mapping
 ---------------*/
float4x4 mWVP;
float4x4 mWorld;
float4x4 mLightVP;
float4x4 mLightV;
float4 matColor;	
float4 vLightDir;
float fFar, fNear;

// hardcoded for 1k shadow map
const float2 texelSize = {1.0f/1024.0f, 1.0f/1024.0f};

texture shadowMap;
sampler shadowMapSamp = sampler_state
{
   Texture = <shadowMap>;
   MinFilter = LINEAR;
   MagFilter = LINEAR;

   AddressU = Clamp;
   AddressV = Clamp;
};


struct VS_OUTPUT
{
   float4 pos : POSITION;
   float4 diffuse : COLOR0;
   float4 ambient : COLOR1;
   float4 shadowMapUV : TEXCOORD0;
   float depth : TEXCOORD1;
};


VS_OUTPUT VSScene(float4 pos : POSITION,
                  float3 norm : NORMAL)
{
   VS_OUTPUT vsOut = (VS_OUTPUT) 0;        
   float4 a = mul(pos, mWorld);
   
   // transform object
   vsOut.pos = mul(pos, mWVP);
    
   // diffuse per-vertex light and ambient
   vsOut.diffuse = matColor*saturate((dot(norm, normalize(vLightDir - a.xyz))));
   vsOut.ambient = matColor*0.3f;                     
	
   // transform projected textures
   // == mLightViewProj * texture adjustment matrix
   // mScale * mTransform * mRotate * mView * mProj * mScaleBias
   float4 tex = mul(pos, mWorld);
   vsOut.shadowMapUV = mul(tex, mLightVP);
   vsOut.shadowMapUV.x = vsOut.shadowMapUV.x*0.5 + vsOut.shadowMapUV.w*(0.5 + 0.5*texelSize.x);
   vsOut.shadowMapUV.y = -vsOut.shadowMapUV.y*0.5 + vsOut.shadowMapUV.w*(0.5 + 0.5*texelSize.y);
   vsOut.shadowMapUV.z = 0.0f;
	
   // measure distance between light and point
   // transform light source
   // mScale * mTransform * mRotate * mView * mProj
   float4 p = mul(pos, mWorld);
   p = mul(p, mLightV);
   vsOut.depth = (((-p.z - fNear)/(fFar - fNear))) - 0.0001f;
		
   return vsOut;
}


// -------------------------------------------------------------
// pixel shader that creates scene
// -------------------------------------------------------------
float4 PSScene(VS_OUTPUT inData) : COLOR
{   
   float4 c = 0.0f;
   float a;
   float sMap = tex2Dproj(shadowMapSamp, inData.shadowMapUV).x;
    
   // value of shadow_map bigger or equal than depth value: return diffuse
   a = (inData.depth > sMap) ? 0.3f : 1.0f;
   c += (inData.diffuse + inData.ambient)*a;
   return c;
}  
// -------------------------------------------------------------
// Compiler directives
// -------------------------------------------------------------

technique renderScene    
{
   pass P0
   {
      VertexShader = compile vs_2_0 VSScene();
      PixelShader  = compile ps_2_0 PSScene();
   }
}
