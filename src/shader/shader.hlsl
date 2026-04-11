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



// Vertex inputs
struct VSInput
{
    float3 a_position : POSITION0;
    float4 a_color    : COLOR0;
};

// Vertex output (also our pixel‑shader input)
struct VSOutput
{
    float4 position : SV_POSITION;
    float4 v_color  : COLOR0;
};

VSOutput mainVSH(VSInput input)
{
    VSOutput output;

    output.position = float4(input.a_position, 1.0f);
    output.v_color  = input.a_color;

    return output;
}

struct PSInput
{
    float4 v_color : COLOR0;
};

float4 mainFSH(PSInput input) : SV_TARGET
{
    return input.v_color;
}