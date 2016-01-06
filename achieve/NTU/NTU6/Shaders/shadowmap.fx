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

// hardcoded for half K shadow map
const float2 texelSize = {1.0f/512.0f, 1.0f/512.0f};

texture depthMap;
sampler depthMapSamp = sampler_state
{
   Texture = <depthMap>;
   MinFilter = LINEAR;
   MagFilter = LINEAR;
   MipFilter = NONE;

   AddressU = Clamp;
   AddressV = Clamp;
};

struct VS_OUTPUTCREATESHADOWMAP
{
   float4 pos : POSITION;
   float dep : TEXCOORD0;
};

VS_OUTPUTCREATESHADOWMAP VSCreateShadowMap(float4 pos : POSITION,
                                           float3 norm : NORMAL)
{
   VS_OUTPUTCREATESHADOWMAP vsOut = (VS_OUTPUTCREATESHADOWMAP) 0; 

   // transform light source
   // mScale * mTransform * mRotate * mView * mProj
   float4 p = mul(pos, mWorld);	
	
   // output position
   vsOut.pos = mul(p, mLightVP);
    
   // depth value
   vsOut.dep = ((vsOut.pos.z + fNear)/(fFar - fNear));
   vsOut.pos.z = vsOut.dep*vsOut.pos.w;

   return vsOut;
}

float4 PSCreateShadowMap(float dep : TEXCOORD0) : COLOR
{   
   return dep;
}


struct VS_OUTPUT
{
   //float4 pos : POSITION;
   //float4 diffuse : COLOR0;
   //float4 ambient : COLOR1;
   //float4 shadowMapUV : TEXCOORD0;
   //float depth : TEXCOORD1;
   float4 pos : POSITION;
   float2 tex : TEXCOORD0;	
};


VS_OUTPUT VSScene(float4 pos : POSITION,
                  float3 norm : NORMAL)
{
   //VS_OUTPUT vsOut = (VS_OUTPUT) 0;        
	
   // transform object
   // mScale * mTransform * mRotate * m_matWorld * m_matView * m_matProj
   //vsOut.pos = mul(pos, mWVP);
    
   // diffuse per-vertex light and ambient
   //vsOut.diffuse = matColor*saturate(dot(vLightDir, norm));
   //vsOut.ambient = matColor*0.3f;                     
	
   // transform projected textures
   // == mLightViewProj * texture adjustment matrix
   // mScale * mTransform * mRotate * mView * mProj * mScaleBias
   //float4 tex = mul(pos, mWorld);
   //vsOut.shadowMapUV = mul(tex, mLightVP);
   //vsOut.shadowMapUV.x = vsOut.shadowMapUV.x*0.5 + vsOut.shadowMapUV.w*(0.5 + 0.5*texelSize.x);
   //vsOut.shadowMapUV.y = -vsOut.shadowMapUV.y*0.5 + vsOut.shadowMapUV.w*(0.5 + 0.5*texelSize.y);
   //vsOut.shadowMapUV.z = 0.0f;
	
   // measure distance between light and point
   // transform light source
   // mScale * mTransform * mRotate * mView * mProj
   //float4 p = mul(pos, mWorld);
   //p = mul(p, mLightVP);
   //vsOut.depth = (((p.z + fNear)/(fFar - fNear))) - 0.0008f;
		
   //return vsOut;
   
   VS_OUTPUT out1 = (VS_OUTPUT) 0;
   out1.pos.xy = pos.xy;
   out1.pos.z = 0.5f;
   out1.pos.w = 1.0f;

   out1.tex = float2(0.5f, -0.5f)*pos.xy + 0.5f.xx;	

   return out1;
}

//float OffsetLookup(sampler2D map, float4 loc, float2 offset)
//{
//   float2 texcoordProj = (loc.xy/loc.w) + texelSize;
//   return tex2D(map, float2(texcoordProj + offset*texelSize));
//}

// -------------------------------------------------------------
// pixel shader that creates scene
// -------------------------------------------------------------
float4 PSScene(VS_OUTPUT inData) : COLOR
{   
   //float4 c = 0.0f;
   //float sMap = OffsetLookup(depthMapSamp, inData.shadowMapUV, float2(0.0f, 0.0f));
    
   // value of shadow_map bigger or equal than depth value: return diffuse
   //c += (inData.depth - sMap > 0.0f ) ? 0.0f : 1.0f;
    
   //return c;
   return tex2D(depthMapSamp, inData.tex);
}  

// -------------------------------------------------------------
// Compiler directives
// -------------------------------------------------------------
technique shadowMap
{
   pass P0
   {
      VertexShader = compile vs_1_1 VSCreateShadowMap();
      PixelShader = compile ps_2_0 PSCreateShadowMap();
   }
}

technique renderScene    
{
   pass P0
   {
      VertexShader = compile vs_1_1 VSScene();
      PixelShader  = compile ps_2_0 PSScene();
   }
}
