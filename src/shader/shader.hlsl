struct InputVSH{
    float x : TEXCOORD0;
    float y : TEXCOORD1;
    float z : TEXCOORD2;
};
struct OutputVSH{
    float4 Position : SV_Position;
};
Output mainVSH(Input input) {
    Output output;
    output.Position = float4(input.x, input.y, input.z, 1.0f);
    return output;
}


struct OutputVSH{
    float4 Color : SV_Target0;
};
Output mainVSH(){
    Output output;
    output.Color = float4(1.0f, 0.0f, 0.0f, 1.0f); // Red
    return output;
}