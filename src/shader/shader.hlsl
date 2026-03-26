struct InputVSH{
    float x : TEXCOORD0;
    float y : TEXCOORD1;
    float z : TEXCOORD2;
};
struct OutputVSH{
    float4 Position : SV_Position;
};
OutputVSH mainVSH(InputVSH input) {
    OutputVSH output;
    output.Position = float4(input.x, input.y, input.z, 1.0f);
    return output;
}


struct OutputFSH{
    float4 Color : SV_Target0;
};

OutputFSH mainFSH(){
    OutputFSH output;
    output.Color = float4(1.0f, 0.0f, 0.0f, 1.0f); // Red
    return output;
}