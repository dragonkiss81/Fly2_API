float4x4 mWVP;    // local to screen projection matrix
float4x4 mWV;     // local to view matrix
float fFar, fNear;//projectCamera: Far/fNear

struct VS_OUTPUT{
   float4 pos : POSITION;
   float dep : TEXCOORD0;
};

VS_OUTPUT VSCreateDepthMap(float4 pos : POSITION, float3 norm : NORMAL){

   VS_OUTPUT vsOut = (VS_OUTPUT) 0; 
   
   // output position
   vsOut.pos = mul(pos, mWVP);
   
   float4 p;
   p = mul(pos, mWV);
   
   // depth value - TheFly's z is negative in view space when in front of the camera
   vsOut.dep = (-p.z - fNear)/(fFar - fNear);
   //vsOut.dep = saturate((-p.z - fNear)/(fFar - fNear));
   return vsOut;
}

void PSCreateDepthMap( in float dep    : TEXCOORD0,
                      out float4 oCol  : COLOR0    ){
	//float4 c = {1.0f,0.0f,0.0f,1.0f};
	float4 c = {dep,0.0f,0.0f,1.0f};
	oCol = c;				   
}

// -------------------------------------------------------------
// Compiler directives
// -------------------------------------------------------------
technique shadowMap
{
   pass P0
   {
      VertexShader = compile vs_2_0 VSCreateDepthMap();
      PixelShader  = compile ps_2_0 PSCreateDepthMap();
   }
}