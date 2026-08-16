// struct InputVSH{
//     float x : TEXCOORD0;
//     float y : TEXCOORD1;
//     float z : TEXCOORD2;
// };
// struct OutputVSH{
//     float4 Position : SV_Position;
// };
// OutputVSH mainVSH(InputVSH input) {
//     OutputVSH output;
//     output.Position = float4(input.x, input.y, input.z, 1.0f);
//     return output;
// }


// struct OutputFSH{
//     float4 Color : SV_Target0;
// };

// OutputFSH mainFSH(){
//     OutputFSH output;
//     output.Color = float4(1.0f, 0.0f, 0.0f, 1.0f); // Red
//     return output;
// }


// Vertex output (also our pixel‑shader input)
struct PixelData{
    float4 position : SV_POSITION;
    float3 uvw       : TEXCOORD0;
};


//************************************************************vertex
// Vertex inputs
struct VSInput {
    float3 a_position : TEXCOORD0;
    float2 size : TEXCOORD1;
};

PixelData mainVSH(VSInput input, uint vertexID:SV_VertexID){
    PixelData output;

    float2 pos2D = input.a_position.xy;
    float height = input.a_position.z;
    float2 vertCord;
    float2 uvCoord;

    //0,1,2: top left triangle, 3,4,5: bottom right triangle
    switch(vertexID){
        case 0://top left corner
            vertCord = pos2D;
            uvCoord = float2(0,0);
            break;
        case 1: case 5://top right corner
            vertCord = pos2D + float2(input.size.x, 0);
            uvCoord = float2(1,0);
            break;
        case 2: case 3:// bottom left corner
            vertCord = pos2D + float2(0, input.size.y);
            uvCoord = float2(0,1);
            break;
        case 4://bottom right corner
            vertCord = pos2D + input.size;
            uvCoord = float2(1,1);
            break;
    }

    output.position = float4(vertCord,0,1);
    output.uvw = float3(uvCoord, 0);

    return output;
}


//************************************************************fragment

Texture2DArray<float4> gTextureArray : register(t0, space2);
SamplerState gSampler : register(s0, space2);

float4 mainFSH(PixelData input) : SV_TARGET{
    float4 color = gTextureArray.Sample(gSampler, input.uvw);

    return color;
    // return float4(1, 0, 0, 1);
}