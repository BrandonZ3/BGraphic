cbuffer Positioning : register(b0)
{
    int renderWidth;
    int renderHeight;
    int font;
    int character;
    int fontsize;
    int percShiftX;
    int percShiftY;
    int x;
    int y;
}

Texture2D image : register(t0);
SamplerState sampler1 : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float2 color : TEXCOORD;
};

PSInput VSMain(float2 position : POSITION, float2 texcoord : TEXCOORD)
{
    PSInput ps;
    
    position -= float2(0.5f, -0.5f);
    position += float2(percShiftX, -percShiftY);
    
    ps.position = float4(position, 0.0, 1.0);
    ps.color = texcoord;

    return ps;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float4 result = image.Sample(sampler1, input.color);
    return result;
}

