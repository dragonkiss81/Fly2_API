float4x4 mWVP;                  // local to screen projection matrix
float4x4 mWorld;                // local to world matrix
float4x4 mWV;                   // local to view matrix
float4x4 mWorldInv;			    // world to local matrix
float4 mainLightPosition;		// light position
float4 mainLightColor;          // light color
float4 mainLightDirection;     // light direction for parallel light source
float4 ambLgt;					//Light Ambient
float4 amb;						//
float4 dif;						//Material Diffuse
float4 spe;                    //Material Specular
float power;                   //Material Shineness


//----shadow data------------------------
float4 camPosition;            //cam position
float4x4 mLightVP;				//projectCamera: view to screen
float4x4 mLightV;				//projectCamera: view
float fFar, fNear;				//projectCamera: Far/fNear

// hardcoded for half K shadow map
//const float2 texelSize = {1.0f/2048.0f, 1.0f/2048.0f};
const float2 texelSize = {1.0f/4096.0f, 1.0f/4096.0f};

texture shadowMap;
sampler shadowMapSamp = sampler_state
{
   Texture = <shadowMap>;
   MinFilter = LINEAR;
   MagFilter = LINEAR;

   AddressU = Clamp;
   AddressV = Clamp;
};
//------------------------------------------

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

   float4 diffuse : COLOR0;
   float4 ambient : COLOR1;
   float4 shadowMapUV : TEXCOORD3;
   float depth : TEXCOORD4;
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


   //---shadow data-----------------------
   // diffuse per-vertex light and ambient
   out1.diffuse = dif*saturate((dot(inNorm, normalize(mainLightPosition.xyz - a.xyz))));
   out1.ambient = dif*0.3f;
	
   //  transform projected textures
   //  mLightViewProj * texture adjustment matrix
   //  mScale * mTransform * mRotate * mView * mProj * mScaleBias
   float4 tex = mul(inPos, mWorld);
   out1.shadowMapUV = mul(tex, mLightVP);
   out1.shadowMapUV.x = out1.shadowMapUV.x*0.5f + out1.shadowMapUV.w*(0.5f + 0.5f*texelSize.x);
   out1.shadowMapUV.y = -out1.shadowMapUV.y*0.5f + out1.shadowMapUV.w*(0.5f + 0.5f*texelSize.y);
   out1.shadowMapUV.z = 0.0f;
	
  //  measure distance between light and point
  //  transform light source
  //  mScale * mTransform * mRotate * mView * mProj
   float4 p = mul(inPos, mWorld);
   p = mul(p, mLightV);
   out1.depth = (-p.z - fNear)/(fFar - fNear)-0.00001f;
   return out1;
}

// bump map pixel shader
void PhongBumpPS(in float2 vTex : TEXCOORD0,
                 in float3 vLgt  : TEXCOORD1,
                 in float3 vCam  : TEXCOORD2,

				 in float4 diffuse : COLOR0,
                 in float4 ambient : COLOR1,
                 in float4 shadowMapUV : TEXCOORD3,
                 in float depth : TEXCOORD4,
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

   //----shadow data----------
    //float3 ProjCoords = camPosition.xyz / camPosition.w;
    //float2 UVCoords;
    //UVCoords.x = 0.5 * ProjCoords.x + 0.5;
    //UVCoords.y = 0.5 * ProjCoords.y + 0.5;
    //float z = 0.5 * ProjCoords.z + 0.5;

    //float xOffset = 1.0/shadowMapUV.x;
    //float yOffset = 1.0/shadowMapUV.y;

 //   float Factor = 0.0;

 //   for (int y = -1 ; y <= 1 ; y++) {
 //       for (int x = -1 ; x <= 1 ; x++) {

 //           float2 Offsets = float2(x * texelSize.x, y * texelSize.y);
 //           float3 UVC = float3(UVCoords.x + Offsets.x,UVCoords.y + Offsets.y, z + 0.00001);
 //           Factor += tex2Dproj(shadowMapSamp, UVC);
 //       }
 //   }
	//if(depth > Factor){
 //       oCol *= c1 ;
 //  }
 float4 kk;
 kk.r = tex2D(shadowMapSamp,shadowMapUV.xy);
 float4 y1;
 y1.x = shadowMapUV.x +texelSize.x;
 y1.y = shadowMapUV.y +texelSize.y;
  float4 y2;
 y2.x = shadowMapUV.x +texelSize.x;
 y2.y = shadowMapUV.y -texelSize.y;
  float4 y3;
 y3.x = shadowMapUV.x -texelSize.x;
 y3.y = shadowMapUV.y +texelSize.y;
  float4 y4;
 y4.x = shadowMapUV.x -texelSize.x;
 y4.y = shadowMapUV.y -texelSize.y;

 kk.r = tex2D(shadowMapSamp,(y1.xy));
 kk.g = tex2D(shadowMapSamp,(y2.xy));
 kk.b = tex2D(shadowMapSamp,(y3.xy));
 kk.a = tex2D(shadowMapSamp,(y4.xy));

   float4 df = {depth,depth,depth,depth};
   float4 dddf = (df>kk);
   oCol *= (1.0f-dot(dddf,float4(0.1f,0.1f,0.1f,0.1f)));




   //float4 c = {0.0f,0.0f,0.0f,1.0f};
   //float4 c1 = {0.4f,0.4f,0.4f,1.0f};
   //float a;
   //float sMap = tex2Dproj(shadowMapSamp, shadowMapUV).x;
   //// value of shadow_map bigger or equal than depth value: return diffuse
   //if(depth > sMap){
   //    // oCol *= c1 ;
   //}
}

// bump map technique
technique PhongBump
{
   pass P0
   {
      VertexShader = compile vs_3_0 PhongBumpVS();
      PixelShader = compile ps_3_0 PhongBumpPS();
   }
}