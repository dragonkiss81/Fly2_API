// This shader implements Image-based Lighting for 
// Phong Reflection Model with Sub-surface scattering
// C.Wang 0626, 2015
//

float4x4 mWVP;             // local to screen projection matrix
float4x4 mWorld;           // local to world matrix
float4x4 mWV;              // local to view matrix

float4 matDif;             // diffuse color of the material
float4 matSpe;             // specular color of the material
float4 camPosition;        // camera position

float wax = 0.3f;                // wax parameter to lower the Phong's constrast
float sigma = 1.5f;              // distance facter for SSS effect
float grow = 1.5;                // enlarge factor to generate the distance map
float ringWeight = 0.1;          // larger will cause SSS is happening only near the silhouette
float backLightEnhanceS = 3.0f;  // enhancement factor for back light

float2 pixelSize;            // position calibration for rendering texture to texture (for DX9 only)
float fIteration = 1.0f;     // the interation count for range of Kawase blur

// for textures of the models
bool beColorMap = false;     
bool beDepthMap = false;
bool beBackLightMap = false;

// for light probes
bool beProbeDiffuse = false;
bool beProbeSpecular = false;
float probeEnhanceD = 1.0f;
float probeEnhanceS = 0.2f;
float4 probeCenter = float4(0.0f, 0.0f, 0.0f, 0.0);     // light probe center & 1/radius
float2 probeOffset = {0.0f, 0.0f};

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

// distance map data   
texture depthMap;
sampler2D depthMapSampler = sampler_state
{
   Texture = <depthMap>;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;
   AddressU = Wrap;
   AddressV = Wrap;
};

// the back light map
texture backLightMap;
sampler2D backLightMapSampler = sampler_state
{
   Texture = <backLightMap>;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = None;
   AddressU = Wrap;
   AddressV = Wrap;
};


// the diffuse environment light probe
texture probeDiffuse;
sampler2D probeDiffuseSampler = sampler_state
{
   Texture = <probeDiffuse>;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = None;
   AddressU = Wrap;
   AddressV = Wrap;
};

// the specular environment light probe
texture probeSpecular;
sampler2D probeSpecularSampler = sampler_state
{
   Texture = <probeSpecular>;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = None;
   AddressU = Wrap;
   AddressV = Wrap;
};

  
/*---------------------------------------------------------------
  vertex output for SSS material rendering (with AO on vertices)
 ----------------------------------------------------------------*/
struct VS_OUTPUT_AO
{
   float4 pos  : POSITION;
   float2 tex0 : TEXCOORD0;
   float3 norm : TEXCOORD1;
   float3 cam  : TEXCOORD2;
   float3 texD : TEXCOORD3;
   float4 ao   : TEXCOORD4;
};


/*-----------------------------------------
  vertex output for SSS material rendering
 ------------------------------------------*/
struct VS_OUTPUT
{
   float4 pos  : POSITION;
   float2 tex0 : TEXCOORD0;
   float3 norm : TEXCOORD1;
   float3 cam  : TEXCOORD2;
   float3 texD : TEXCOORD3;
};


/*----------------------------------------------
  vertex output for generating the distance map
 -----------------------------------------------*/
struct VS_OUTPUT_Depth
{
   float4 pos  : POSITION;
   float depth : TEXCOORD0;
};


/*-------------------------------------
  vertex output for blurring the image
 --------------------------------------*/
struct VS_OUTPUTBloom
{
   float4 pos         : POSITION;
   float2 topLeft     : TEXCOORD0;
   float2 topRight    : TEXCOORD1;
   float2 bottomRight : TEXCOORD2;
   float2 bottomLeft  : TEXCOORD3;	
};


/*-------------------------------------
  vertex shader for blurring the image
  using Kawase blur
 --------------------------------------*/
VS_OUTPUTBloom VSBloom(float4 pos : POSITION)
{
   VS_OUTPUTBloom out1 = (VS_OUTPUTBloom) 0;        
   out1.pos.xy = pos.xy + pixelSize.xy;
   out1.pos.z = 0.5f;
   out1.pos.w = 1.0f;

   float2 tex = float2(0.5f, -0.5f)*pos.xy + 0.5f.xx;	
   
   float2 halfPixelSize = pixelSize/2.0f;
   float2 dUV = pixelSize.xy*fIteration + halfPixelSize;
	
   // sample top left
   out1.topLeft = float2(tex.x - dUV.x, tex.y + dUV.y); 
	
   // sample top right
   out1.topRight = float2(tex.x + dUV.x, tex.y + dUV.y);
	
   // sample bottom right
   out1.bottomRight = float2(tex.x + dUV.x, tex.y - dUV.y);
	
   // sample bottom left
   out1.bottomLeft = float2(tex.x - dUV.x, tex.y - dUV.y);

   return out1;
}


/*------------------------------------
  pixel shader for blurring the image
  using Kawase blur
 -------------------------------------*/
float4 PSBloom(float2 topLeft     : TEXCOORD0,
               float2 topRight    : TEXCOORD1,
               float2 bottomRight : TEXCOORD2,
               float2 bottomLeft  : TEXCOORD3) : COLOR0
{
   float4 addedBuffer = 0.0f;

   // sample top left
   addedBuffer = tex2D(colorMapSampler, topLeft);

   // sample top right
   addedBuffer += tex2D(colorMapSampler, topRight);
	
   // sample bottom right
   addedBuffer += tex2D(colorMapSampler, bottomRight);
	
   // sample bottom left
   addedBuffer += tex2D(colorMapSampler, bottomLeft);
	
   // average
   return addedBuffer *= 0.25f;
}


/*---------------------------------------------------------------------------
  get the texture coordinate when using Lat-Log map for image-based lighting
  C.Wang 0623, 2015
 ----------------------------------------------------------------------------*/
float2 LatLongIM(float3 v)
{
   float3 vv = normalize(v);
   float theta = acos(vv.z);      // we use +z up
   float phi = atan2(vv.x, vv.y) + 3.1415962;
   
   return float2(phi, theta)*float2(0.15916, 0.31831);
}

   
/*--------------------------------------------------------------------------------------
  the vertex shader for Phong shading using image-based lighting with ambient occlusion
  and approximated sub-surface scattering
 ---------------------------------------------------------------------------------------*/   
VS_OUTPUT_AO Phong_AO_IM_VS(float4 inPos  : POSITION,
                            float3 inNorm : NORMAL,
                            float2 inTex  : TEXCOORD0,
                            float4 inAO   : TEXCOORD1)
{
   VS_OUTPUT_AO out1 = (VS_OUTPUT_AO) 0;
      
   float4 a = mul(inPos, mWorld);
   float4 p = mul(inPos, mWVP);
   float4 vv = mul(inPos, mWV);

   out1.pos = p;
   out1.norm = mul(inNorm, (float3x3) mWorld);
   out1.cam = normalize(camPosition.xyz - a.xyz);
   out1.tex0 = inTex;
   out1.ao = inAO;

   // generate the texture coordinates for distance map (screen space)
   out1.texD.xy = float2(0.5, -0.5)*p.xy/p.w + 0.5.xx;

   // calculate the distance to camera
   out1.texD.z = length(vv);

   return out1;

}
   

/*-------------------------------------------------------------------------------------
  the pixel shader for Phong shading using image-based lighting with ambient occlusion
  and approximated sub-surface scattering
 --------------------------------------------------------------------------------------*/   
void Phong_AO_IM_PS(in float2 uv0   : TEXCOORD0,
                    in float3 vNorm : TEXCOORD1,
                    in float3 vCam  : TEXCOORD2,
                    in float3 uvD   : TEXCOORD3,
                    in float4 vAO   : TEXCOORD4,
                   out float4 oCol  : COLOR0)
{   
   // calculate/normalize all vectors
   float3 N = normalize(vNorm);
   float3 V = normalize(vCam);
   
   float4 diffLightColor;
   float4 specLightColor;

   // get diffuse lighting data
   if (beProbeDiffuse) {
      float2 uv = LatLongIM(N);
      diffLightColor = tex2D(probeDiffuseSampler, uv)*probeEnhanceD;
   }
   else {
      diffLightColor = 0.0f;
   }
   float4 diffWax = (diffLightColor + wax)/(1.0 + wax);

   // get specular lighting data
   if (beProbeSpecular) {
      float3 refl = reflect(-V, N);
      float2 uv = LatLongIM(refl);
      specLightColor = tex2D(probeSpecularSampler, uv)*probeEnhanceS;
   }
   else {
      specLightColor = 0.0f;
   }

   // get color texture and ao value (from vertex)   
   float aoValue = 1.0;
   float4 baseColor = 0.0;
   if (beColorMap) {
      aoValue = vAO.r;
      float4 texColor = tex2D(colorMapSampler, uv0);
      baseColor = matDif*texColor;
   }
   else {
      aoValue = uv0.r;
      baseColor = matDif;
   }

   // calculate the back light if we have sub-surface scattering effect ON
   float4 backLight = 0.0;
   if (beDepthMap) {
      // find the distance of light in model
      float si = tex2D(depthMapSampler, uvD.xy) - uvD.z;
      
      if (si > 0.0) {
         // get back light intensity
         float bColor;
         if (beBackLightMap) {
            float3 V2 = -V;
            float2 uvV2 = LatLongIM(V2);
            float4 backLiteColor = tex2D(backLightMapSampler, uvV2);
            bColor = dot(backLiteColor.rgb, float3(0.3, 0.6, 0.1));
            bColor *= bColor;
         }
         else {
            bColor = 0.0f;
         }

         // calculate the ring weight
         float dd = dot(N, V);
         float weight = (1.0 - dd)*ringWeight + (1.0 - ringWeight);

         // calculate the back light effect
         backLight = exp(-si/sigma)*bColor*backLightEnhanceS*weight;
      }
   }

   // final result
   oCol = (baseColor*(diffWax + backLight) + matSpe*specLightColor)*aoValue;
   oCol.a = matDif.a;
}


/*---------------------------------------------------------------
  the vertex shader for Phong shading using image-based lighting
  with approximated sub-surface scattering
 ----------------------------------------------------------------*/   
VS_OUTPUT Phong_IM_VS(float4 inPos  : POSITION,
                      float3 inNorm : NORMAL,
                      float2 inTex  : TEXCOORD0)
{
   VS_OUTPUT out1 = (VS_OUTPUT) 0;
      
   float4 a = mul(inPos, mWorld);
   float4 p = mul(inPos, mWVP);
   float4 vv = mul(inPos, mWV);

   out1.pos = p;
   out1.norm = mul(inNorm, (float3x3) mWorld);
   out1.cam = normalize(camPosition.xyz - a.xyz);
   out1.tex0 = inTex;

   // generate the texture coordinates for distance map (screen space)
   out1.texD.xy = float2(0.5, -0.5)*p.xy/p.w + 0.5.xx;

   // calculate the distance to camera
   out1.texD.z = length(vv);

   return out1;
}
   

/*--------------------------------------------------------------
  the pixel shader for Phong shading using image-based lighting
  with approximated sub-surface scattering
 ---------------------------------------------------------------*/
void Phong_IM_PS(in float2 uv0   : TEXCOORD0,
                 in float3 vNorm : TEXCOORD1,
                 in float3 vCam  : TEXCOORD2,
                 in float3 uvD   : TEXCOORD3,
                out float4 oCol  : COLOR0)
{   
   // calculate/normalize all vectors
   float3 N = normalize(vNorm);
   float3 V = normalize(vCam);
   
   float4 diffLightColor;
   float4 specLightColor;

   // get diffuse lighting data
   if (beProbeDiffuse) {
      float2 uv = LatLongIM(N);
      diffLightColor = tex2D(probeDiffuseSampler, uv)*probeEnhanceD;
   }
   else {
      diffLightColor = 0.0f;
   }
   float4 diffWax = (diffLightColor + wax)/(1.0 + wax);

   // get specular lighting data
   if (beProbeSpecular) {
      float3 refl = reflect(-V, N);
      float2 uv = LatLongIM(refl);
      specLightColor = tex2D(probeSpecularSampler, uv)*probeEnhanceS;
   }
   else {
      specLightColor = 0.0f;
   }
   
   // get color texture 
   float4 baseColor = 0.0;
   if (beColorMap) {
      float4 texColor = tex2D(colorMapSampler, uv0);
      baseColor = matDif*texColor;
   }
   else {
      baseColor = matDif;
   }

   // calculate the back light if we have sub-surface scattering effect ON
   float4 backLight = 0.0;
   if (beDepthMap) {
      // find the distance of light in model
      float si = tex2D(depthMapSampler, uvD.xy) - uvD.z;
      
      if (si > 0.0) {
         // get back light intensity
         float bColor;
         if (beBackLightMap) {
            float3 V2 = -V;
            float2 uvV2 = LatLongIM(V2);
            float4 backLiteColor = tex2D(backLightMapSampler, uvV2);
            bColor = dot(backLiteColor.rgb, float3(0.3, 0.6, 0.1));
            bColor *= bColor;
         }
         else {
            bColor = 0.0f;
         }

         // calculate the ring weight
         float dd = dot(N, V);
         float weight = (1.0 - dd)*ringWeight + (1.0 - ringWeight);

         // calculate the back light effect
         backLight = exp(-si/sigma)*bColor*backLightEnhanceS*weight;
      }
   }

   // final result
   oCol = baseColor*(diffWax + backLight) + matSpe*specLightColor;
   oCol.a = matDif.a;
}


/*-----------------------------------------------------------------------
  the vertex shader for calculating the distance of the object to camera
  for backfacing triangles
 ------------------------------------------------------------------------*/
VS_OUTPUT_Depth GenDepth_BF_VS(float4 inPos  : POSITION,
                               float3 inNorm : NORMAL)
{
   float4 a;
   VS_OUTPUT_Depth out1 = (VS_OUTPUT_Depth) 0;
   
   // grow the model a little bit
   float4 p = inPos;
   p.xyz += inNorm*grow;
   out1.pos = mul(p, mWVP);

   // calculate the depth
   a = mul(inPos, mWV);
   out1.depth = length(a);

   return out1;
}


/*----------------------------------------------------------------------
  the pixel shader for calculating the distance of the object to camera
  for backfacing triangles
 -----------------------------------------------------------------------*/ 
void GenDepth_BF_PS(in float dist   : TEXCOORD0,
                   out float4 oCol  : COLOR0)
{
   oCol = dist.rrrr;
}


technique Phong_IM_SSS_AO
{
   pass P0
   {
      VertexShader = compile vs_3_0 Phong_AO_IM_VS();
      PixelShader = compile ps_3_0 Phong_AO_IM_PS();
   }
}


technique Phong_IM_SSS
{
   pass P0
   {
      VertexShader = compile vs_3_0 Phong_IM_VS();
      PixelShader = compile ps_3_0 Phong_IM_PS();
   }
}


technique CalculateDistance
{
   pass P0
   {
      cullMode = CW;
      VertexShader = compile vs_3_0 GenDepth_BF_VS();
      PixelShader = compile ps_3_0 GenDepth_BF_PS();
   }
}


technique BlurImage
{
   pass P0
   {
      VertexShader = compile vs_3_0 VSBloom();
      PixelShader = compile ps_3_0 PSBloom();
   }
}
