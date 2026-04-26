#pragma once
#include "../../CPP Lib/BLIB.h"
#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include <d3d12.h>
#include <d3dcompiler.h>
#include <string>

const char* htmlVectorShader = R"(


cbuffer Matrices : register(b0)
{
    int renderWidth;
    int renderHeight;
    float cr;
    float cg;
    float cb;
    float ca;
    int width;
    int height;
    float bGr;
    float bGg;
    float bGb;
    float bGa;
    int brTL;
    int brTR;
    int brBL;
    int brBR;
    int bools; //bit 0 is for draw image;
    int justifyContent;
    int alignItems;
    float blr;
    float blg;
    float blb;
    float bla;
    float btr;
    float btg;
    float btb;
    float bta;
    float brr;
    float brg;
    float brb;
    float bra;
    float bbr;
    float bbg;
    float bbb;
    float bba;
    int bTl;
    int bTt;
    int bTr;
    int bTb;
    float x;
    float y;

    int font;
    int character;
    int fontsize;
    int percShiftX;
    int percShiftY;
	float scaleXt;
	float scaleYt;
}

Texture2D image : register(t0);
SamplerState sampler1 : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texcoords : TEXCOORD;
};

PSInput VSMain(float2 position : POSITION, float2 texcoords: TEXCOORD)
{
	PSInput result;
	bool drawText = ((bools & 0x2) >> 1) == 0x1;

    if(drawText)
	{
		
		float scaleX = fontsize / (float)renderWidth;
		float scaleY = fontsize / (float)renderHeight;

		position.x *= scaleX;
		position.x -= 1;
		position.x += scaleX;
		position.x += (x / renderWidth) * 2;

		position.y *= scaleY;
		position.y += 1;
		position.y -= scaleY;
		position.y -= (y / renderHeight) * 2;
    
		result.position = float4(position, 0.0, 1.0);
		result.texcoords = texcoords;
	}
	else
	{    
		float percW = (width / (float) renderWidth);
		float percH = (height / (float) renderHeight);
    
		float baseShiftX = (((float) renderWidth / 2) / (float) renderWidth) * -2; //moving the box to x == 0 or L
		float baseShiftY = (((float) renderHeight / 2) / (float) renderHeight) * 2; //moving the box to y == 0 or T
    
		float shiftPosX = x / (float) renderWidth;          //Use this to move to appropriate X
		float shiftPosY = (y / (float) renderHeight) * -1;  //Use this to move to appropriate Y
    
		float xScaledAndShifted = ((position.x * percW) + percW) + baseShiftX + (shiftPosX * 2);
		float yScaledAndShifted = ((position.y * percH) - percH) + baseShiftY + (shiftPosY * 2);

		result.position = float4(float3(float2( xScaledAndShifted, yScaledAndShifted), 0), 1.0f);
		result.texcoords = texcoords;
	}
	return result;
}



)";


const char* htmlPixelShader = R"(

cbuffer Matrices : register(b0)
{
    int renderWidth;
    int renderHeight;
    float cr;
    float cg;
    float cb;
    float ca;
    int width;
    int height;
    float bGr;
    float bGg;
    float bGb;
    float bGa;
    int brTL;
    int brTR;
    int brBL;
    int brBR;
    int bools; //bit 0 is for draw image;
    int justifyContent;
    int alignItems;
    float blr;
    float blg;
    float blb;
    float bla;
    float btr;
    float btg;
    float btb;
    float bta;
    float brr;
    float brg;
    float brb;
    float bra;
    float bbr;
    float bbg;
    float bbb;
    float bba;
    int bTl;
    int bTt;
    int bTr;
    int bTb;
    float x;
    float y;

    int font;
    int character;
    int fontsize;
    int percShiftX;
    int percShiftY;
	float scaleXt;
	float scaleYt;
}

Texture2D image : register(t0);
SamplerState sampler1 : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texcoords : TEXCOORD;
};


float4 PSMain(PSInput input) : SV_TARGET
{
    bool drawImage = (bools & 0x1) == 0x1;
	bool drawText = ((bools & 0x2) >> 1) == 0x1;
    
    if(!drawImage && !drawText)
    {
        float ypos = y; //- (actualHeight / 2);
        float xpos = x; // - (actualWidth / 2);
    
        float4 bordercolor = float4(blr, blg, blb, bla);
    
        float4 bordercolorleft = float4(blr, blg, blb, bla);
        float4 bordercolortop = float4(btr, btg, btb, bta);
        float4 bordercolorright = float4(brr, brg, brb, bra);
        float4 bordercolorbottom = float4(bbr, bbg, bbb, bba);
        float4 backgroundcolor = float4(bGr, bGg, bGb, bGa);
    
    
        if (brTL > 0)
        {
            float pivotX = xpos + brTL;
            float pivotY = ypos + brTL;
            
            if (input.position.x > xpos && input.position.x < pivotX && input.position.y > ypos && input.position.y < pivotY)
            {
                float distance = sqrt(pow(pivotX - input.position.x, 2) + pow(pivotY - input.position.y, 2));
                if (distance > brTL)
                    discard;
            }
        }
    
    
        if (brTL > 0 && (brTL > bTl || brTL > bTt) && input.position.x < xpos + (width / 2) && input.position.y < ypos + (height / 2))
        {
            float nxpos = xpos + bTl;
            float nypos = ypos + bTt;
        
            float a = (nypos - ypos) / (nxpos - xpos);
        
            float ly = ((input.position.x - xpos) * a);
        
            float rx = max(0, brTL - bTl);
            float ry = max(0, brTL - bTt);

            float dx = input.position.x - (nxpos + rx);
            float dy = input.position.y - (nypos + ry);
        
            if (rx > 0 && ry > 0)
            {
                float ellipseVal = (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry);
                if (ellipseVal > 1.0 && input.position.x < (nxpos + rx) && input.position.y < (nypos + ry))
                    if(ly < input.position.y - ypos)
                        return bordercolorleft; //discard; // inside inner ellipse
                    else
                        return bordercolortop;
            }
        }
    
        if (brTR > 0)
        {
            float pivotX = xpos + width - brTR;
            float pivotY = ypos + brTR;
            
            if (input.position.x > pivotX && input.position.x < xpos + width && input.position.y > ypos && input.position.y < pivotY)
            {
                float distance = sqrt(pow(pivotX - input.position.x, 2) + pow(pivotY - input.position.y, 2));
                if (distance > brTR)
                    discard;
            }
        }
    
        if (brTR > 0 && (brTR > bTr || brTR > bTt) && input.position.x > xpos + (width / 2) && input.position.y < ypos + (height / 2))
        {
            float nxpos = xpos + width - bTr;
            float nypos = ypos + bTt;
        
            float a = (nypos - ypos) / (nxpos - (xpos + width));
            float ly = ((input.position.x - (xpos + width)) * a);
        
            float rx = max(0, brTR - bTr);
            float ry = max(0, brTR - bTt);

            float dx = input.position.x - (nxpos - rx);
            float dy = input.position.y - (nypos + ry);
        
            if (rx > 0 && ry > 0)
            {
                float ellipseVal = (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry);
                if (ellipseVal > 1.0 && input.position.x > (nxpos - rx) && input.position.y < (nypos + ry))
                    if (ly < input.position.y - ypos)
                        return bordercolorright; //discard; // inside inner ellipse
                    else
                        return bordercolortop; //discard; // inside inner ellipse
            }
        }
    
        if (brBL > 0)
        {
            float pivotX = xpos + brBL;
            float pivotY = ypos + height - brBL;
            
            if (input.position.x > xpos && input.position.x < pivotX && input.position.y < ypos + height && input.position.y > pivotY)
            {
                float distance = sqrt(pow(pivotX - input.position.x, 2) + pow(pivotY - input.position.y, 2));
                if (distance > brBL)
                    discard;
            }
        }
    
        if (brBL > 0 && (brBL > bTl || brBL > bTb) && input.position.x < xpos + (width / 2) && input.position.y > ypos + (height / 2))
        {
            float nxpos = xpos + bTl;
            float nypos = ypos + height - bTb;
        
            float a = (nypos - (ypos + height)) / (nxpos - (xpos));
            float ly = ((input.position.x - (xpos)) * a);
        
            float rx = max(0, brBL - bTl);
            float ry = max(0, brBL - bTb);

            float dx = input.position.x - (nxpos + rx);
            float dy = input.position.y - (nypos - ry);
        
            if (rx > 0 && ry > 0)
            {
                float ellipseVal = (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry);
                if (ellipseVal > 1.0 && input.position.x < (nxpos + rx) && input.position.y > (nypos - ry))
                    if (ly < input.position.y - (ypos + height))
                        return bordercolorbottom; //discard; // inside inner ellipse
                    else
                        return bordercolorleft; //discard; // inside inner ellipse
            }
        }
    
        if (brBR > 0)
        {
            float pivotX = xpos + width - brBR;
            float pivotY = ypos + height - brBR;
            
            if (input.position.x > pivotX && input.position.x < xpos + width && input.position.y < ypos + height && input.position.y > pivotY)
            {
                float distance = sqrt(pow(pivotX - input.position.x, 2) + pow(pivotY - input.position.y, 2));
                if (distance > brBR)
                    discard;
            }
        }
)";

const char* htmlPixelShaderPT2 = R"(
    
        if (brBR > 0 && (brBR > bTr || brBL > bTb) && input.position.x > xpos + (width / 2) && input.position.y > ypos + (height / 2))
        {
            float nxpos = xpos + width - bTr;
            float nypos = ypos + height - bTb;
        
            float a = (nypos - (ypos + height)) / (nxpos - (xpos + width));
            float ly = ((input.position.x - (xpos + width)) * a);
        
            float rx = max(0, brBL - bTr);
            float ry = max(0, brBL - bTb);

            float dx = input.position.x - (nxpos - rx);
            float dy = input.position.y - (nypos - ry);
        
            if (rx > 0 && ry > 0)
            {
                float ellipseVal = (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry);
                if (ellipseVal > 1.0 && input.position.x > (nxpos - rx) && input.position.y > (nypos - ry))
                    if (ly < input.position.y - (ypos + height))
                        return bordercolorbottom; //discard; // inside inner ellipse
                    else
                        return bordercolorright; //discard; // inside inner ellipse
            }
        }
    
    
    
        if (bTl > 0)
        {
        
            if ((input.position.x > xpos && input.position.x < xpos + bTl && input.position.y > ypos + bTt && input.position.y < ypos + height - bTb))
            {
                return bordercolorleft;
            }
        
            if ((input.position.x > xpos && input.position.x < xpos + bTl && input.position.y < ypos + bTt))
            {
                float x0 = xpos;
                float y0 = ypos;
            
                float x1 = xpos;
                float y1 = y0 + bTt;
            
                float x2 = x0 + bTl;
                float y2 = y0 + bTt;
            
                float edgeAB = (input.position.x - x0) * (y1 - y0) - (input.position.y - y0) * (x1 - x0);
                float edgeBC = (input.position.x - x1) * (y2 - y1) - (input.position.y - y1) * (x2 - x1);
                float edgeCA = (input.position.x - x2) * (y0 - y2) - (input.position.y - y2) * (x0 - x2);

            
                if ((edgeAB >= 0) && (edgeBC >= 0) && (edgeCA >= 0))
                    return bordercolorleft;
            }
        
            if ((input.position.x > xpos && input.position.x < xpos + bTl && input.position.y > ypos + height - bTb))
            {            
                float x0 = xpos;
                float y0 = ypos + height;
            
                float x1 = xpos;
                float y1 = y0 - bTb;
            
                float x2 = x0 + bTl;
                float y2 = y0 - bTb;
            
                float edgeAB = (input.position.x - x0) * (y1 - y0) - (input.position.y - y0) * (x1 - x0);
                float edgeBC = (input.position.x - x1) * (y2 - y1) - (input.position.y - y1) * (x2 - x1);
                float edgeCA = (input.position.x - x2) * (y0 - y2) - (input.position.y - y2) * (x0 - x2);

            
                if ((edgeAB <= 0) && (edgeBC <= 0) && (edgeCA <= 0))
                    return bordercolorleft;
            }
        }
    
        if (bTt > 0)
        {
        
            if ((input.position.y > ypos && input.position.y < ypos + bTt && input.position.x > xpos + bTl && input.position.x < xpos + width - bTr))
            {
                return bordercolortop;
            }
        
            if ((input.position.y > ypos && input.position.y < ypos + bTt && input.position.x < xpos + bTl))
            {
                float x0 = xpos + bTl;
                float y0 = ypos + bTt;
            
                float x1 = xpos + bTl;
                float y1 = ypos;
            
                float x2 = xpos;
                float y2 = ypos;
            
                float edgeAB = (input.position.x - x0) * (y1 - y0) - (input.position.y - y0) * (x1 - x0);
                float edgeBC = (input.position.x - x1) * (y2 - y1) - (input.position.y - y1) * (x2 - x1);
                float edgeCA = (input.position.x - x2) * (y0 - y2) - (input.position.y - y2) * (x0 - x2);

            
                if ((edgeAB >= 0) && (edgeBC >= 0) && (edgeCA >= 0))
                    return bordercolortop;
            }
        
            if ((input.position.y > ypos && input.position.y < ypos + bTt && input.position.x > xpos + width - bTr))
            {
                float x0 = xpos + width;
                float y0 = ypos;
            
                float x1 = x0 - bTr;
                float y1 = ypos;
            
                float x2 = x1;
                float y2 = ypos + bTt;
            
                float edgeAB = (input.position.x - x0) * (y1 - y0) - (input.position.y - y0) * (x1 - x0);
                float edgeBC = (input.position.x - x1) * (y2 - y1) - (input.position.y - y1) * (x2 - x1);
                float edgeCA = (input.position.x - x2) * (y0 - y2) - (input.position.y - y2) * (x0 - x2);

            
                if ((edgeAB >= 0) && (edgeBC >= 0) && (edgeCA >= 0))
                    return bordercolortop;
            }
        }
    
        if (bTr > 0)
        {
        
            if ((input.position.x < xpos + width && input.position.x > xpos + width - bTr && input.position.y > ypos + bTt && input.position.y < ypos + height - bTb))
            {
                return bordercolorright;
            }
        
            if ((input.position.x < xpos + width && input.position.x > xpos + width - bTr && input.position.y < ypos + bTt))
            {
                float x0 = xpos + width;
                float y0 = ypos;
            
                float x1 = x0;
                float y1 = ypos + bTt;
            
                float x2 = x0 - bTr;
                float y2 = y1;
            
                float edgeAB = (input.position.x - x0) * (y1 - y0) - (input.position.y - y0) * (x1 - x0);
                float edgeBC = (input.position.x - x1) * (y2 - y1) - (input.position.y - y1) * (x2 - x1);
                float edgeCA = (input.position.x - x2) * (y0 - y2) - (input.position.y - y2) * (x0 - x2);

            
                if ((edgeAB <= 0) && (edgeBC <= 0) && (edgeCA <= 0))
                    return bordercolorright;
            }
        
            if ((input.position.x < xpos + width && input.position.x > xpos + width - bTr && input.position.y > ypos + height - bTb))
            {
                float x0 = xpos + width;
                float y0 = ypos + height - bTb;
            
                float x1 = x0;
                float y1 = y0 + bTb;
            
                float x2 = x0 - bTr;
                float y2 = y0;
            
                float edgeAB = (input.position.x - x0) * (y1 - y0) - (input.position.y - y0) * (x1 - x0);
                float edgeBC = (input.position.x - x1) * (y2 - y1) - (input.position.y - y1) * (x2 - x1);
                float edgeCA = (input.position.x - x2) * (y0 - y2) - (input.position.y - y2) * (x0 - x2);

            
                if ((edgeAB <= 0) && (edgeBC <= 0) && (edgeCA <= 0))
                    return bordercolorright;
            }
        }
    
    
        if (bTb > 0)
        {
            if ((input.position.y < ypos + height && input.position.y > ypos + height - bTb && input.position.x > xpos + bTl && input.position.x < xpos + width - bTr))
            {
                return bordercolorbottom;
            }
        
            if ((input.position.y < ypos + height && input.position.y > ypos + height - bTb && input.position.x < xpos + bTl))
            {
                float x0 = xpos;
                float y0 = ypos + height;
            
                float x1 = x0 + bTl;
                float y1 = y0 - bTb;
            
                float x2 = x1;
                float y2 = y0;
            
                float edgeAB = (input.position.x - x0) * (y1 - y0) - (input.position.y - y0) * (x1 - x0);
                float edgeBC = (input.position.x - x1) * (y2 - y1) - (input.position.y - y1) * (x2 - x1);
                float edgeCA = (input.position.x - x2) * (y0 - y2) - (input.position.y - y2) * (x0 - x2);

            
                if ((edgeAB <= 0) && (edgeBC <= 0) && (edgeCA <= 0))
                    return bordercolorbottom;
            }
        
            if ((input.position.y < ypos + height && input.position.y > ypos + height - bTb && input.position.x > xpos + width - bTr))
            {
                float x0 = xpos + width - bTr;
                float y0 = ypos + height - bTb;
            
                float x1 = x0 + bTr;
                float y1 = y0 + bTb;
            
                float x2 = x0;
                float y2 = y1;
            
                float edgeAB = (input.position.x - x0) * (y1 - y0) - (input.position.y - y0) * (x1 - x0);
                float edgeBC = (input.position.x - x1) * (y2 - y1) - (input.position.y - y1) * (x2 - x1);
                float edgeCA = (input.position.x - x2) * (y0 - y2) - (input.position.y - y2) * (x0 - x2);

            
                if ((edgeAB <= 0) && (edgeBC <= 0) && (edgeCA <= 0))
                    return bordercolorbottom;
            }
        }
    
        return float4(bGr, bGg, bGb, bGa);
    }
    else if(drawText)
    {
		input.texcoords *= float2(scaleXt, scaleYt);
		input.texcoords.x += scaleXt * character;
		input.texcoords.y += scaleYt * font;
		float4 result = image.Sample(sampler1, input.texcoords);
		return result;
    }
	else
	{
        float4 textureValue = image.Sample(sampler1, input.texcoords);
		textureValue.a = bGa;
        return textureValue;
	}
    return float4(0, 0, 0, 1);
}


)";

void SetupTextureBuffers(ID3D12Device10* device, ID3D12GraphicsCommandList* commandList, const char* file, ID3D12Resource** upload, ID3D12Resource** gpuBuffer, ID3D12DescriptorHeap* cbvsrvuavHeap, int cbvsrvuavIndex, int* pwidth, int* pheight)
{
	bool recalculated = false;
	int width;
	int height;
	int channels;
	unsigned char* data;
	data = stbi_load(file, &width, &height, &channels, 4);

	if (data != NULL && channels == 3)
	{
		channels = 4;
	}

	if (data != NULL && channels == 4)
	{
		D3D12_HEAP_PROPERTIES hprop = {};
		D3D12_RESOURCE_DESC rdesc = {};
		if (*upload == NULL)
		{
			hprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			hprop.CreationNodeMask = 0;
			hprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			hprop.Type = D3D12_HEAP_TYPE_UPLOAD;
			hprop.VisibleNodeMask = 0;

			rdesc.Height = 1;
			rdesc.Width = width * height * channels;
			rdesc.Alignment = 0;
			rdesc.Format = DXGI_FORMAT_UNKNOWN;
			rdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			rdesc.SampleDesc.Count = 1;
			rdesc.SampleDesc.Quality = 0;
			rdesc.MipLevels = 1;
			rdesc.DepthOrArraySize = 1;
			rdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			rdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

			device->CreateCommittedResource(&hprop, D3D12_HEAP_FLAG_NONE, &rdesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, __uuidof(ID3D12Resource), (void**)upload) == S_OK ? "" : throw "Not OK!";
		}

		hprop = {};
		hprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		hprop.CreationNodeMask = 0;
		hprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		hprop.Type = D3D12_HEAP_TYPE_DEFAULT;
		hprop.VisibleNodeMask = 0;

		rdesc = {};
		rdesc.Alignment = 0;
		rdesc.DepthOrArraySize = 1;
		rdesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		rdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		rdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rdesc.Height = height;
		rdesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		rdesc.MipLevels = 1;
		rdesc.SampleDesc.Count = 1;
		rdesc.Width = width;

		device->CreateCommittedResource(&hprop, D3D12_HEAP_FLAG_NONE, &rdesc, D3D12_RESOURCE_STATE_COPY_DEST, NULL, __uuidof(ID3D12Resource), (void**)gpuBuffer) == S_OK ? "" : throw "Not OK!";

		char* dma;
		(*upload)->Map(0, NULL, (void**)&dma);
		memcpy(dma, data, width * height * channels);
		(*upload)->Unmap(0, NULL);

		D3D12_PLACED_SUBRESOURCE_FOOTPRINT foot = {};
		device->GetCopyableFootprints(&rdesc, 0, 1, 0, &foot, NULL, NULL, NULL);

		D3D12_TEXTURE_COPY_LOCATION scpy = {};
		scpy.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		scpy.pResource = (*upload);
		scpy.PlacedFootprint = { 0 ,{DXGI_FORMAT_R8G8B8A8_UNORM, (unsigned int)width, (unsigned int)height, 1, (unsigned int)(width * channels)} };

		D3D12_TEXTURE_COPY_LOCATION dcpy = {};
		dcpy.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		dcpy.pResource = (*gpuBuffer);
		dcpy.SubresourceIndex = 0;

		commandList->CopyTextureRegion(&dcpy, 0, 0, 0, &scpy, NULL);

		D3D12_RESOURCE_TRANSITION_BARRIER trbar = {};
		trbar.pResource = (*gpuBuffer);
		trbar.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		trbar.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		trbar.Subresource = 0xFFFFFFFF;

		D3D12_RESOURCE_BARRIER tbar = {};
		tbar.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		tbar.Transition = trbar;
		tbar.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

		commandList->ResourceBarrier(1, &tbar);

		D3D12_TEX2D_SRV notsure;
		notsure.MipLevels = 1;
		notsure.MostDetailedMip = 0;
		notsure.PlaneSlice = 0;
		notsure.ResourceMinLODClamp = 0;

		D3D12_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvd.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvd.Texture2D = notsure;

		UINT norTexDescSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		D3D12_CPU_DESCRIPTOR_HANDLE texHnd = cbvsrvuavHeap->GetCPUDescriptorHandleForHeapStart();
		texHnd.ptr += (cbvsrvuavIndex * norTexDescSize);
		device->CreateShaderResourceView((*gpuBuffer), &srvd, texHnd);
	}
	stbi_image_free(data);
	*pwidth = width;
	*pheight = height;
}

class DrawSettings
{
public:
	int renderWidth;
	int renderHeight;
	float color[4];
	int width;
	int height;
	float backgroundColor[4];
	int borderRadius[4];
	int bools; //bit 0 is display flex, bit 1 is flex row/column
	int justifyContent;
	int alignItems;
	float borderColorLeft[4];
	float borderColorTop[4];
	float borderColorRight[4];
	float borderColorBottom[4];
	int borderThickness[4];
	float x;
	float y;
	//text
	int font;
	int character;
	int fontsize;
	int percShiftX;
	int percShiftY;
	float scaleX;
	float scaleY;
};

class TextSettings
{
public:
    int renderWidth;
    int renderHeight;
    int font;
    int character;
    int fontsize;
    int percShiftX;
    int percShiftY;
	float scaleX;
	float scaleY;
    int x;
    int y;
};

class ImageStorage
{
public:
	int index = -1;
	ID3D12Resource* resource;
	ID3D12Resource* upload;
	int width;
	int height;
};

class GraphicsModule2D
{
public:
	int iaPosition = -1;
	int iaColors = -1;
	int iaNormals = -1;
	int iaTexCoords = -1;
	int iaTangents = -1;
	int iaJoints = -1;
	int iaWeights = -1;
	bool indexed = false;
	ID3D12PipelineState* pipeline = NULL;
	ID3D12RootSignature* rootSignature = NULL;
	D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	GraphicsModule2D(GraphicsModule2D&) = delete;
	GraphicsModule2D(ID3D12PipelineState* pipeline, ID3D12RootSignature* rootSignature)
	{
		this->pipeline = pipeline;
		this->rootSignature = rootSignature;
	}

	void SetTopology(D3D_PRIMITIVE_TOPOLOGY topo)
	{
		this->topology = topo;
	}
	~GraphicsModule2D()
	{
		pipeline->Release();
		rootSignature->Release();
	}
};

class GraphicsModel
{
private:
	GraphicsModel(GraphicsModel&) = delete;
public:
	GraphicsModule2D* graphicsModule; // Dont free

	ID3D12Resource* posU = NULL;
	ID3D12Resource* posR = NULL;
	ID3D12Resource* texU = NULL;
	ID3D12Resource* texR = NULL;

	D3D12_VERTEX_BUFFER_VIEW Positions;
	D3D12_VERTEX_BUFFER_VIEW TexCoords;

	ID3D12Resource* indR = NULL;
	ID3D12Resource* indU = NULL;
	D3D12_INDEX_BUFFER_VIEW Indices;
	uint64_t indexCount;

	/*char* vertexdata;
	char* indexdata;*/

	GraphicsModel(ID3D12Device10* device, ID3D12GraphicsCommandList* commandList, GraphicsModule2D* graphmod)
	{
		this->graphicsModule = graphmod;

		int vertexSize = sizeof(float) * 2;
		UINT verticeByteLength = vertexSize * 4;
		float* vertexdata = (float*)malloc(verticeByteLength);

		UINT indiceByteLength = (sizeof(uint32_t) * 6);
		uint32_t* indices = (uint32_t*)malloc(indiceByteLength);

		int texcoordSize = sizeof(float) * 2;
		UINT texcoordByteLength = texcoordSize * 4;
		float* texdata = (float*)malloc(texcoordByteLength);

		vertexdata[0] = -1; //top left
		vertexdata[1] = 1;
		vertexdata[2] = 1;  //top right
		vertexdata[3] = 1;
		vertexdata[4] = -1; //bottom left
		vertexdata[5] = -1;
		vertexdata[6] = 1; //bottom right
		vertexdata[7] = -1;

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;

		indices[3] = 2;
		indices[4] = 1;
		indices[5] = 3;

		texdata[0] = 0; //top left
		texdata[1] = 0;
		texdata[2] = 1;  //top right
		texdata[3] = 0;
		texdata[4] = 0; //bottom left
		texdata[5] = 1;
		texdata[6] = 1; //bottom right
		texdata[7] = 1;

		this->indexCount = 6;

		SetupVertexBuffer(device, commandList, (char*)vertexdata, verticeByteLength, &posU, &posR, &Positions, 2 * sizeof(float));
		SetupVertexBuffer(device, commandList, (char*)texdata, texcoordByteLength, &texU, &texR, &TexCoords, 2 * sizeof(float));

		free(vertexdata);
		free(texdata);

		if (this->graphicsModule->indexed)
		{
			SetupIndexBuffer(device, commandList, (char*)indices, indiceByteLength, &indU, &indR, &Indices, sizeof(uint32_t));
		}
		free(indices);
	}

	void SetupVertexBuffer(ID3D12Device10* device, ID3D12GraphicsCommandList* commandList, char* refdata, UINT byteLength, ID3D12Resource** upload, ID3D12Resource** gpuBuffer, D3D12_VERTEX_BUFFER_VIEW* view, uint64_t dataInstanceSize)
	{
		uint64_t ChunkLength = 0;
		uint64_t perfectSections = byteLength / 256;
		if (byteLength % 256 != 0)
			perfectSections++;
		ChunkLength = perfectSections * 256;

		D3D12_HEAP_PROPERTIES uprop = {};
		uprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		uprop.CreationNodeMask = 0;
		uprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		uprop.Type = D3D12_HEAP_TYPE_UPLOAD;
		uprop.VisibleNodeMask = 0;

		D3D12_RESOURCE_DESC uDesc = {};
		uDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		uDesc.Format = DXGI_FORMAT_UNKNOWN;
		uDesc.MipLevels = 1;
		uDesc.Alignment = 0;
		uDesc.DepthOrArraySize = 1;
		uDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		uDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		uDesc.SampleDesc.Count = 1;
		uDesc.Width = ChunkLength;
		uDesc.Height = 1;

		device->CreateCommittedResource(&uprop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &uDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, __uuidof(ID3D12Resource), (void**)upload);

		D3D12_HEAP_PROPERTIES hProp = {};
		hProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		hProp.CreationNodeMask = 0;
		hProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		hProp.Type = D3D12_HEAP_TYPE_DEFAULT;
		hProp.VisibleNodeMask = 0;

		D3D12_RESOURCE_DESC rDesc = {};
		rDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		rDesc.Format = DXGI_FORMAT_UNKNOWN;
		rDesc.MipLevels = 1;
		rDesc.Alignment = 0;
		rDesc.DepthOrArraySize = 1;
		rDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		rDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		rDesc.SampleDesc.Count = 1;
		rDesc.Width = ChunkLength;
		rDesc.Height = 1;

		device->CreateCommittedResource(&hProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_COPY_DEST, NULL, __uuidof(ID3D12Resource), (void**)gpuBuffer) == S_OK ? "" : throw "Failed To Create Vertex Buffer";

		ID3D12Resource* uploadRes = *upload;
		ID3D12Resource* gpuRes = *gpuBuffer;

		void* data;
		uploadRes->Map(0, NULL, &data);
		memmove(data, refdata, byteLength);
		uploadRes->Unmap(0, NULL);

		D3D12_RESOURCE_TRANSITION_BARRIER rtBar;
		rtBar.pResource = *gpuBuffer;
		rtBar.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		rtBar.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
		rtBar.Subresource = 0xFFFFFFFF;

		D3D12_RESOURCE_BARRIER rBar;
		rBar.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		rBar.Transition = rtBar;
		rBar.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

		commandList->CopyResource(gpuRes, uploadRes);
		commandList->ResourceBarrier(1, &rBar);

		view->BufferLocation = gpuRes->GetGPUVirtualAddress();
		view->SizeInBytes = byteLength;
		view->StrideInBytes = dataInstanceSize;
	}

	void SetupIndexBuffer(ID3D12Device10* device, ID3D12GraphicsCommandList* commandList, char* refdata, UINT byteLength, ID3D12Resource** upload, ID3D12Resource** gpuBuffer, D3D12_INDEX_BUFFER_VIEW* view, uint64_t dataInstanceSize)
	{
		uint64_t ChunkLength = 0;
		uint64_t perfectSections = byteLength / 256;
		if (byteLength % 256 != 0)
			perfectSections++;
		ChunkLength = perfectSections * 256;

		D3D12_HEAP_PROPERTIES hProp = {};
		hProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		hProp.CreationNodeMask = 0;
		hProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		hProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		hProp.VisibleNodeMask = 0;

		D3D12_RESOURCE_DESC rDesc = {};
		rDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		rDesc.Format = DXGI_FORMAT_UNKNOWN;
		rDesc.MipLevels = 1;
		rDesc.Alignment = 0;
		rDesc.DepthOrArraySize = 1;
		rDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		rDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		rDesc.SampleDesc.Count = 1;
		rDesc.Width = ChunkLength;
		rDesc.Height = 1;

		device->CreateCommittedResource(&hProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, __uuidof(ID3D12Resource), (void**)upload) == S_OK ? "" : throw "Failed To Create Upload Buffer";

		hProp = {};
		hProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		hProp.CreationNodeMask = 0;
		hProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		hProp.Type = D3D12_HEAP_TYPE_DEFAULT;
		hProp.VisibleNodeMask = 0;

		rDesc = {};
		rDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		rDesc.Format = DXGI_FORMAT_UNKNOWN;
		rDesc.MipLevels = 1;
		rDesc.Alignment = 0;
		rDesc.DepthOrArraySize = 1;
		rDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		rDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		rDesc.SampleDesc.Count = 1;
		rDesc.Width = ChunkLength;
		rDesc.Height = 1;

		device->CreateCommittedResource(&hProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_COPY_DEST, NULL, __uuidof(ID3D12Resource), (void**)gpuBuffer) == S_OK ? "" : throw "Failed To Create Vertex Buffer";

		ID3D12Resource* uploadRes = *upload;
		ID3D12Resource* gpuRes = *gpuBuffer;

		void* data;
		uploadRes->Map(0, NULL, &data);
		memmove(data, refdata, byteLength);
		uploadRes->Unmap(0, NULL);

		D3D12_RESOURCE_TRANSITION_BARRIER rtBar;
		rtBar.pResource = *gpuBuffer;
		rtBar.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		rtBar.StateAfter = D3D12_RESOURCE_STATE_INDEX_BUFFER;
		rtBar.Subresource = 0xFFFFFFFF;

		D3D12_RESOURCE_BARRIER rBar;
		rBar.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		rBar.Transition = rtBar;
		rBar.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

		commandList->CopyResource(gpuRes, uploadRes);
		commandList->ResourceBarrier(1, &rBar);

		view->BufferLocation = gpuRes->GetGPUVirtualAddress();
		view->SizeInBytes = byteLength;
		view->Format = DXGI_FORMAT_R32_UINT;
	}
	
	void Draw(ID3D12Device10* device, ID3D12GraphicsCommandList* cL, UINT settingSize, void* settings, D3D12_GPU_DESCRIPTOR_HANDLE* texViewHandle)
	{
		if (this->graphicsModule->indexed)
			cL->IASetIndexBuffer(&this->Indices);
		if (this->graphicsModule->iaPosition != -1)
			cL->IASetVertexBuffers(this->graphicsModule->iaPosition, 1, &this->Positions);
		if (this->graphicsModule->iaTexCoords != -1)
			cL->IASetVertexBuffers(this->graphicsModule->iaTexCoords, 1, &this->TexCoords);

		if (texViewHandle != NULL)
			cL->SetGraphicsRootDescriptorTable(1, *texViewHandle);

		cL->SetGraphicsRoot32BitConstants(0, settingSize, settings, 0);
		cL->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
	}

	void ReleaseTemporaryBuffers()
	{
		if (indU != NULL)
			indU->Release();
		if (posU != NULL)
			posU->Release();
		if (texU != NULL)
			texU->Release();
	}

	~GraphicsModel()
	{
		if (posR != NULL)
			posR->Release();
		if (texR != NULL)
			texR->Release();
		if (indR != NULL)
			indR->Release();
		ReleaseTemporaryBuffers();
	}
};

class DrawingData
{
public:
	GraphicsModel* htmlModel;
	BLIB::PointerList* modelTextures = new BLIB::PointerList();
	ID3D12DescriptorHeap* textureHeap;
	int textureHeapCount = 0;
	DrawSettings tempSettings;
};

class BGraph
{
	const char* textTextureLocation = "text.png";
	int textureHeapIncrementSize = 0;

	int drawSettingSize = (sizeof(DrawSettings) / 4);
	int textSettingSize = (sizeof(TextSettings) / 4);
	std::vector<char*> files;	//name and fn*
	BLIB::PointerList* functions = new BLIB::PointerList();	//name and fn*
	BLIB::PointerList* pages = new BLIB::PointerList();		//name and json
	BLIB::JSONElement* variables = new BLIB::JSONElement();
	BLIB::PointerList* components = new BLIB::PointerList();

	BLIB::PointerList* root = new BLIB::PointerList();
	BLIB::HTMLElement* current = NULL;
	BLIB::HTMLElement* active = NULL;
	BLIB::HTMLElement* lastTouched = NULL;

	BGraph(BGraph&) = delete;
	DrawingData* dData = new DrawingData();

	ID3D12Device10* device = NULL;
	ID3D12GraphicsCommandList* commandList = NULL;
	GraphicsModule2D* htmlGModule = NULL;

	int height;
	int width;

	BLIB::DBuffer* activeKeys = new BLIB::DBuffer();

	void bGNavigate(const char* pageName)
	{
		BLIB::KeyPointerPair* value = BLIB::KeyPointerPair::GetKeyValuePointer(root, pageName);
		if (value != NULL)
		{
			current = (BLIB::HTMLElement*)value->pointer;
			Refresh();
		}
	}

	void bGStyle(BLIB::HTMLElement* element, char* style)
	{
		BLIB::HTMLParser::ParseStyling(style, element);
	}

	void bGSet(BLIB::HTMLElement* element, const char* varname, const char* value)
	{
		BLIB::JSONElement* var = BLIB::JSONElement::GetElement(varname, variables, true);
		int valueLength = BLIB::Strings::Length(value);
		if (value[0] == '*' && value[valueLength - 1] == '*')
		{
			char* refName = BLIB::Strings::Replace(value, "*", "");
			BLIB::JSONElement* ref = BLIB::JSONElement::GetElement(refName, variables, true);

			if (var->type != BLIB::JSONElementType::NONE)
			{
				delete var;
				var = BLIB::JSONElement::GetElement(varname, variables, true);
				BLIB::JSONElement::ConvertBlankToRenamedReference(var, varname, ref);
			}
			else
				BLIB::JSONElement::ConvertBlankToRenamedReference(var, varname, ref);
			free(refName);
		}
		else
		{

			if (var->type == BLIB::JSONElementType::NONE)
			{
				var->type = BLIB::JSONElementType::KVP;
			}

			if (var != NULL)
			{
				free(var->value);
				var->value = BLIB::Strings::Clone(value);
			}
			else
			{
				BLIB::JSONElement* newVar = new BLIB::JSONElement();
				newVar->name = BLIB::Strings::Clone(varname);
				newVar->value = BLIB::Strings::Clone(value);
				variables->children->AddPointer(newVar);
			}
		}

		Refresh();
	}

	bool HandleHTMLHover(BLIB::HTMLElement* element, int x, int y)
	{
		for (int i = element->children.size() - 1; i >= 0; i--)
		{
			if (HandleHTMLHover(element->children.at(i), x, y))
				return true;
		}

		BLIB::KeyPointerPair* value = BLIB::KeyPointerPair::GetKeyValuePointer(element->attributes, "bGHover");
		if (value != NULL)
		{
			if (TestCoordinates(element, x, y))
			{
				element->hover = true;

				BLIB::PointerList* clickargs = BLIB::PointerList::SplitString((char*)value->pointer, ";");

				if (clickargs->count == 2 && BLIB::Strings::Compare((char*)clickargs->items[0], "bGStyle"))
				{
					bGStyle(element, (char*)clickargs->items[1]);
				}

				clickargs->FreeEverything();
			}
			else
			{
				element->hover = false;
				BLIB::KeyPointerPair* style = BLIB::KeyPointerPair::GetKeyValuePointer(element->attributes, "style");

				if (style != NULL)
				{
					bGStyle(element, (char*)style->pointer);
				}
				
			}
		}

		return element->hover;
	}

	bool HandleHTMLMouseUp(BLIB::HTMLElement* element, int x, int y)
	{
		//Have to check if its scroll bar first.
		/*if (OnHorizontalScrollbar(element, x, y))
		{
			active = element;
			element->scrollYSelected = false;
			element->scrollXSelected = true;
			element->scrollStartX = x;
			element->scrollStartY = y;
			return true;
		}

		if (OnVerticalScrollbar(element, x, y))
		{
			active = element;
			element->scrollYSelected = true;
			element->scrollXSelected = false;
			element->scrollStartX = x;
			element->scrollStartY = y;
			return true;
		}

		for (int i = element->children.size() - 1; i >= 0; i--)
		{
			if (HandleHTMLClick(element->children.at(i), x, y))
				return true;
		}*/

		if (active != NULL)
		{
			active->scrollXSelected = false;
			active->scrollYSelected = false;
		}
		return true;
	}

	bool HandleHTMLMouseDown(BLIB::HTMLElement* element, int x, int y)
	{
		bool testClicked = TestCoordinates(element, x, y);
		//Have to check if its scroll bar first.
		if (element->overflowHandling == BLIB::HTMLOverflow::AUTO && element->scrollBarScaleX < 1.0f && OnHorizontalScrollbar(element, x, y))
		{
			active = element;
			lastTouched = element;
			if (!active->scrollXSelected && !active->scrollYSelected)
			{
				element->scrollYSelected = false;
				element->scrollXSelected = true;
				element->scrollStartX = x;
				element->scrollStartY = y;
			}
			return true;
		}

		if (element->overflowHandling == BLIB::HTMLOverflow::AUTO && element->scrollBarScaleY < 1.0f && OnVerticalScrollbar(element, x, y))
		{
			active = element;
			lastTouched = element;
			if (!active->scrollXSelected && !active->scrollYSelected)
			{
				element->scrollYSelected = true;
				element->scrollXSelected = false;
				element->scrollStartX = x;
				element->scrollStartY = y;
			}
			return true;
		}

		if (element->type == BLIB::HTMLElementType::INPUT && testClicked)
		{
			active = element;
			lastTouched = element;
			return true;
		}

		if(testClicked)
			for (int i = element->children.size() - 1; i >= 0; i--)
			{
				if (HandleHTMLMouseDown(element->children.at(i), x, y))
					return true;
			}

		bool bGClick = BLIB::KeyPointerPair::GetKeyValuePointer(element->attributes, "bGClick") != NULL;
		if (testClicked && bGClick)
		{
			lastTouched = element;
			return true;
		}

		return false;
	}

	bool HandleHTMLClick(BLIB::HTMLElement* element, int x, int y)
	{

		for (int i = element->children.size() - 1; i >= 0; i--)
		{
			if (HandleHTMLClick(element->children.at(i), x, y))
				return true;
		}

		if (TestCoordinates(element, x, y) && lastTouched == element)
		{
			if (element == active && active->inputType == BLIB::HTMLInputType::CHECKBOX)
			{
				active->checked = !active->checked;
				active->bGr = 255 - active->bGr;
				active->bGg = 255 - active->bGg;
				active->bGb = 255 - active->bGb;
				return true;
			}

			BLIB::KeyPointerPair* value = BLIB::KeyPointerPair::GetKeyValuePointer(element->attributes, "bGClick");
			if (value != NULL)
			{
				BLIB::PointerList* clickargs = BLIB::PointerList::SplitString((char*)value->pointer, ";");

				if (clickargs->count == 2 && BLIB::Strings::Compare((char*)clickargs->items[0], "bGNavigate"))
				{
					///////////////// Need to make sure that bGname is taken out and the name of the file is the pagename.
					bGNavigate((char*)clickargs->items[1]);
				}

				if (clickargs->count == 2 && BLIB::Strings::Compare((char*)clickargs->items[0], "bGStyle"))
				{
					bGStyle(element, (char*)clickargs->items[1]);
				}

				if (clickargs->count == 3 && BLIB::Strings::Compare((char*)clickargs->items[0], "bGSet"))
				{
					bGSet(element, (char*)clickargs->items[1], (char*)clickargs->items[2]);
				}

				clickargs->FreeEverything();
				return true;
			}

		}
		return false;
	}

	bool TestCoordinates(BLIB::HTMLElement* element, int x, int y)
	{

		float left = element->x;
		float top = element->y;
		float right = element->x + element->actualWidth;
		float bottom = element->y + element->actualHeight;

		return x > left && x < right && y > top && y < bottom;
	}

public:
	BGraph(ID3D12Device10* device, ID3D12GraphicsCommandList* commandList)
	{
		this->device = device;
		this->commandList = commandList;
		CreateHTMLPipeline();

		D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};
		dhDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		dhDesc.NodeMask = 0;
		dhDesc.NumDescriptors = 1000;
		dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		device->CreateDescriptorHeap(&dhDesc, __uuidof(ID3D12DescriptorHeap), (void**)&(dData->textureHeap)) == S_OK ? "" : throw "Broken";

		dData->htmlModel = new GraphicsModel(device, commandList, htmlGModule);

		textureHeapIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		AddTexture(device, commandList, textTextureLocation);
	}

	void UpdateActiveKeys(BLIB::DBuffer* keys)
	{
		if (active != NULL && keys != NULL && active->type == BLIB::HTMLElementType::INPUT && active->inputType != BLIB::HTMLInputType::CHECKBOX && active->inputType != BLIB::HTMLInputType::RANGE)
		{
			unsigned char* keysPointer = keys->DataPointer(0);
			for (int i = 0; i < keys->count; i++)
			{
				unsigned char character = BLIB::Keyboard::TranslateKeyboardToHTMLTextChar(keysPointer[i]);
				BLIB::HTMLTextCharacter c;
				c.character = character; 
				if (character != 0x8)
					active->characters.push_back(c);
				else if(active->characters.size() > 0)
					active->characters.pop_back();
				active->textDirty = true;
			}

			/*if (active->textDirty)
				BLIB::HTMLParser::ResolveHTML(active->parent, variables);*/

			if (active->textwrap == false)
			{
				float totalWidth = active->characters.size() * active->fontsize;
				float contentWidth = BLIB::HTMLElement::GetElementContentWidth(active);
				float overflow = totalWidth - contentWidth;
				
				if (overflow > 0 && !active->textwrap && active->textDirty)
				{
					for (int i = 0; i < active->characters.size(); i++)
						active->characters.at(i).x -= overflow;
				}
			}

			if (keys->count > 0 && active->textDirty)
			{
				BLIB::KeyPointerPair* val = BLIB::KeyPointerPair::GetKeyValuePointer(active->attributes, "value");

				if (val != NULL && BLIB::Strings::Contains((char*)val->pointer, "{{"))
				{
					char* varName = BLIB::Strings::Replace((char*)val->pointer, "{{", "");
					BLIB::Strings::FreeAndAssign(&varName, BLIB::Strings::Replace(varName, "}}", ""));
					BLIB::Strings::FreeAndAssign(&varName, BLIB::Strings::Trim(varName));

					int size = active->characters.size();
					char* newString = (char*)malloc(size + 1);
					newString[size] = 0;
					for (int i = 0; i < size; i++)
					{
						newString[i] = BLIB::Keyboard::TranslateHTMLTextCharToKeyboard(active->characters.at(i).character);
					}

					SetVariable(varName, newString, false);//This is not gonna work for nested json

					free(varName);
				}
				Refresh();
			}
		}
	}

	void HandleClick(int x, int y)
	{
		HandleHTMLClick(current, x, y);
	}

	void HandleMouseDown(int x, int y)
	{
		lastTouched = NULL;
		HandleHTMLMouseDown(current, x, y);
	}

	void HandleMouseUp(int x, int y)
	{
		HandleHTMLMouseUp(current, x, y);
	}

	void HandleHover(int x, int y)
	{
		if (active != NULL && (active->scrollXSelected || active->scrollYSelected))
		{
			float diffX = 0;
			float diffY = 0;
			if (active->scrollXSelected)
			{
				diffX = x - active->scrollStartX;
				active->scrollStartX = x;
				if (active->scrollPosX + diffX <= active->actualWidth - (((active->actualWidth - active->scrollSpacing)) * active->scrollBarScaleX))
				{
					if(active->scrollPosX + diffX >= 0)
						active->scrollPosX += diffX;
					else
					{
						diffX = 0 - active->scrollPosX;
						if (diffX > -0.001)
							diffX = 0;
						active->scrollPosX += diffX;
					}
				}
				else
				{
					diffX = 0;
					double val = (active->actualWidth - (((active->actualWidth - active->scrollSpacing)) * active->scrollBarScaleX)) - active->scrollPosX;
					diffX = val;

					if (diffX < 0.001)
						diffX = 0;

					active->scrollPosX += diffX;
				}
			}

			if (active->scrollYSelected)
			{
				diffY = y - active->scrollStartY;
				active->scrollStartY = y;
				if (active->scrollPosY + diffY <= active->actualHeight - (((active->actualHeight - active->scrollSpacing)) * active->scrollBarScaleY))
				{
					if(active->scrollPosY + diffY >= 0)
						active->scrollPosY += diffY;
					else
					{
						diffY = 0 - active->scrollPosY;
						if (diffY > -0.001)
							diffY = 0;
						active->scrollPosY += diffY;
					}
				}
				else
				{
					diffY = 0;
					double val = (active->actualHeight - (((active->actualHeight - active->scrollSpacing)) * active->scrollBarScaleY)) - active->scrollPosY;
					diffY = val;

					if (diffY < 0.001)
						diffY = 0;

					active->scrollPosY += diffY;
				}
			}

			for (int i = 0; i < active->children.size(); i++)
				BLIB::HTMLElement::ApplyScrolling(diffX * active->scrollBarShiftScaleX, diffY * active->scrollBarShiftScaleY, active->children.at(i));

			return;
		}

		HandleHTMLHover(current, x, y);
	}

	void FindLinkedPages(BLIB::HTMLElement* element)
	{

		BLIB::KeyPointerPair* value = BLIB::KeyPointerPair::GetKeyValuePointer(element->attributes, "bGClick");
		if (value != NULL)
		{
			BLIB::PointerList* clickargs = BLIB::PointerList::SplitString((char*)value->pointer, ";");

			if (clickargs->count == 2 && BLIB::Strings::Compare((char*)clickargs->items[0], "bGNavigate"))
			{
				char* fileName = BLIB::Strings::Concat((char*) clickargs->items[1], ".html");
				bool found = false;

				for (int i = 0; i < files.size(); i++)
				{
					if (BLIB::Strings::Compare(fileName, files.at(i)))
					{
						found = true;
						break;
					}
				}

				if (!found)
				{
					files.push_back(fileName);
					AddPage(fileName);
				}
				else
					free(fileName);
			}

			clickargs->FreeEverything();
		}

		for (int i = 0; i < element->children.size(); i++)
		{
			FindLinkedPages(element->children.at(i));
		}
	}

	void CreateHTMLPipeline()
	{
		ID3D12PipelineState* pipeline = NULL;
		ID3D12RootSignature* root = NULL;

		D3D12_ROOT_CONSTANTS matrices = {};
		matrices.Num32BitValues = drawSettingSize;
		matrices.RegisterSpace = 0;
		matrices.ShaderRegister = 0;

		D3D12_ROOT_PARAMETER params[2] = {};
		params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		params[0].Constants = matrices;

		D3D12_DESCRIPTOR_RANGE range = {};
		range.BaseShaderRegister = 0;
		range.NumDescriptors = 1;
		range.OffsetInDescriptorsFromTableStart = 0;
		range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range.RegisterSpace = 0;

		D3D12_ROOT_DESCRIPTOR_TABLE tbl = {};
		tbl.NumDescriptorRanges = 1;
		tbl.pDescriptorRanges = &range;

		params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[1].DescriptorTable = tbl;
		params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;


		D3D12_STATIC_SAMPLER_DESC ssDesc = {};
		ssDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		ssDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		ssDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		ssDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		ssDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		ssDesc.MaxAnisotropy = 16;
		ssDesc.MaxLOD = D3D12_FLOAT32_MAX;
		ssDesc.MinLOD = 0;
		ssDesc.MipLODBias = 0;
		ssDesc.BorderColor = D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;

		D3D12_ROOT_SIGNATURE_DESC charRDesc = {};
		charRDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		charRDesc.NumParameters = 2;
		charRDesc.pParameters = params;
		charRDesc.NumStaticSamplers = 1;
		charRDesc.pStaticSamplers = &ssDesc;

		ID3DBlob* blob = NULL;
		ID3DBlob* error = NULL;

		D3D12SerializeRootSignature(&charRDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &blob, &error) == S_OK ? "" : throw "";
		this->device->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&root) == S_OK ? "" : throw "Failed To Create RootSig";

		blob->Release();
		if(error != NULL)
			error->Release();

		ID3DBlob* vsBlob = NULL;
		ID3DBlob* vserror = NULL;
		ID3DBlob* psBlob = NULL;
		ID3DBlob* pserror = NULL;

		D3DCompile(htmlVectorShader, BLIB::Strings::Length(htmlVectorShader), NULL, NULL, NULL, "VSMain", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, NULL, &vsBlob, &vserror);

		if (vserror != NULL)
		{
			char* vscerr = (char*)vserror->GetBufferPointer();
			OutputDebugStringA(vscerr);
			OutputDebugStringA("\r\nPS:");
			vserror->Release();
		}

		char* fullShader = BLIB::Strings::Concat(htmlPixelShader, htmlPixelShaderPT2);

		D3DCompile(fullShader, BLIB::Strings::Length(fullShader), NULL, NULL, NULL, "PSMain", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, NULL, &psBlob, &pserror);

		free(fullShader);

		if (pserror != NULL)
		{
			char* pscerr = (char*)pserror->GetBufferPointer();
			OutputDebugStringA(pscerr);
			pserror->Release();
		}

		D3D12_INPUT_ELEMENT_DESC iel[2];
		iel[0].AlignedByteOffset = 0;
		iel[0].Format = DXGI_FORMAT_R32G32_FLOAT;
		iel[0].InputSlot = 0;
		iel[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		iel[0].InstanceDataStepRate = 0;
		iel[0].SemanticIndex = 0;
		iel[0].SemanticName = "POSITION";


		iel[1].AlignedByteOffset = 0;
		iel[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		iel[1].InputSlot = 1;
		iel[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		iel[1].InstanceDataStepRate = 0;
		iel[1].SemanticIndex = 0;
		iel[1].SemanticName = "TEXCOORD";

		D3D12_INPUT_LAYOUT_DESC ild;
		ild.NumElements = 2;
		ild.pInputElementDescs = iel;

		D3D12_SHADER_BYTECODE vsC = {};
		vsC.BytecodeLength = vsBlob->GetBufferSize();
		vsC.pShaderBytecode = vsBlob->GetBufferPointer();

		D3D12_SHADER_BYTECODE psC = {};
		psC.BytecodeLength = psBlob->GetBufferSize();
		psC.pShaderBytecode = psBlob->GetBufferPointer();

		D3D12_RENDER_TARGET_BLEND_DESC rtBlend = {};
		rtBlend.BlendEnable = true;
		rtBlend.LogicOpEnable = false;
		rtBlend.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		rtBlend.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		rtBlend.BlendOp = D3D12_BLEND_OP_ADD;
		rtBlend.SrcBlendAlpha = D3D12_BLEND_ONE;
		rtBlend.DestBlendAlpha = D3D12_BLEND_ZERO;
		rtBlend.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		rtBlend.LogicOp = D3D12_LOGIC_OP_NOOP;
		rtBlend.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		D3D12_BLEND_DESC blDesc = {};
		blDesc.AlphaToCoverageEnable = false;
		blDesc.IndependentBlendEnable = false;
		blDesc.RenderTarget[0] = rtBlend;

		D3D12_DEPTH_STENCILOP_DESC bfdsoDesc = {};
		bfdsoDesc.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		bfdsoDesc.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		bfdsoDesc.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		bfdsoDesc.StencilPassOp = D3D12_STENCIL_OP_KEEP;

		D3D12_DEPTH_STENCILOP_DESC ffdsoDesc = {};
		ffdsoDesc.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		ffdsoDesc.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		ffdsoDesc.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		ffdsoDesc.StencilPassOp = D3D12_STENCIL_OP_KEEP;

		D3D12_DEPTH_STENCIL_DESC stDesc;
		stDesc.DepthEnable = false;
		stDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		stDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		stDesc.StencilEnable = false;
		stDesc.StencilReadMask = 0xFF;
		stDesc.StencilWriteMask = 0xFF;
		stDesc.FrontFace = ffdsoDesc;
		stDesc.BackFace = bfdsoDesc;

		D3D12_RASTERIZER_DESC rasDesc = {};
		rasDesc.FillMode = D3D12_FILL_MODE_SOLID;
		rasDesc.CullMode = D3D12_CULL_MODE_BACK;
		rasDesc.FrontCounterClockwise = false;
		rasDesc.DepthBias = 0;
		rasDesc.DepthBiasClamp = 0.0f;
		rasDesc.SlopeScaledDepthBias = 0.0f;
		rasDesc.DepthClipEnable = false;
		rasDesc.MultisampleEnable = false;
		rasDesc.AntialiasedLineEnable = false;
		rasDesc.ForcedSampleCount = 0;
		rasDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		D3D12_GRAPHICS_PIPELINE_STATE_DESC cgp = {};
		cgp.NodeMask = 0;
		cgp.InputLayout = ild;
		cgp.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		cgp.pRootSignature = root;
		cgp.PS = psC;
		cgp.VS = vsC;
		cgp.BlendState = blDesc;
		cgp.DepthStencilState = stDesc;
		cgp.RasterizerState = rasDesc;
		cgp.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		cgp.NumRenderTargets = 1;
		cgp.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		cgp.SampleDesc.Count = 1;
		cgp.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		cgp.SampleMask = UINT_MAX;

		this->device->CreateGraphicsPipelineState(&cgp, __uuidof(ID3D12PipelineState), (void**)&pipeline) == S_OK ? "" : throw "Failed To Create Pipeline";

		vsBlob->Release();
		psBlob->Release();

		htmlGModule = new GraphicsModule2D(pipeline, root);
		htmlGModule->indexed = true;
		htmlGModule->iaPosition = 0;
		htmlGModule->iaTexCoords = 1;
		htmlGModule->topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}

	void DrawPage(D3D12_VIEWPORT* vp, D3D12_RECT* sc)
	{
		commandList->SetPipelineState(this->dData->htmlModel->graphicsModule->pipeline);
		commandList->SetGraphicsRootSignature(this->dData->htmlModel->graphicsModule->rootSignature);
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dData->tempSettings.renderWidth = this->width;
		dData->tempSettings.renderHeight = this->height;
		DrawHTML(current, device, commandList, dData, sc);
	}

	void DrawHTML(BLIB::HTMLElement* element, ID3D12Device10* device, ID3D12GraphicsCommandList* cL, DrawingData* dData, D3D12_RECT* sc)
	{
		dData->tempSettings.bools = 0;
		dData->tempSettings.backgroundColor[0] = element->bGr / 255.0f;
		dData->tempSettings.backgroundColor[1] = element->bGg / 255.0f;
		dData->tempSettings.backgroundColor[2] = element->bGb / 255.0f;
		dData->tempSettings.backgroundColor[3] = element->bGa / 255.0f;
		dData->tempSettings.width = element->actualWidth;
		dData->tempSettings.height = element->actualHeight;
		dData->tempSettings.x = element->x;
		dData->tempSettings.y = element->y;
		dData->tempSettings.borderThickness[0] = element->bTl;
		dData->tempSettings.borderThickness[1] = element->bTt;
		dData->tempSettings.borderThickness[2] = element->bTr;
		dData->tempSettings.borderThickness[3] = element->bTb;
		dData->tempSettings.borderColorLeft[0] = element->blr;
		dData->tempSettings.borderColorLeft[1] = element->blg;
		dData->tempSettings.borderColorLeft[2] = element->blb;
		dData->tempSettings.borderColorLeft[3] = element->bla;
		dData->tempSettings.borderColorTop[0] = element->btr;
		dData->tempSettings.borderColorTop[1] = element->btg;
		dData->tempSettings.borderColorTop[2] = element->btb;
		dData->tempSettings.borderColorTop[3] = element->bta;
		dData->tempSettings.borderColorRight[0] = element->brr;
		dData->tempSettings.borderColorRight[1] = element->brg;
		dData->tempSettings.borderColorRight[2] = element->brb;
		dData->tempSettings.borderColorRight[3] = element->bra;
		dData->tempSettings.borderColorBottom[0] = element->bbr;
		dData->tempSettings.borderColorBottom[1] = element->bbg;
		dData->tempSettings.borderColorBottom[2] = element->bbb;
		dData->tempSettings.borderColorBottom[3] = element->bba;
		dData->tempSettings.borderRadius[0] = element->radiusTopLeft;
		dData->tempSettings.borderRadius[1] = element->radiusTopRight;
		dData->tempSettings.borderRadius[2] = element->radiusBottomLeft;
		dData->tempSettings.borderRadius[3] = element->radiusBottomRight;

		float test = min(element->actualWidth / 2, element->actualHeight / 2);

		if (dData->tempSettings.borderRadius[0] > test)
			dData->tempSettings.borderRadius[0] = test;

		if (dData->tempSettings.borderRadius[1] > test)
			dData->tempSettings.borderRadius[1] = test;

		if (dData->tempSettings.borderRadius[2] > test)
			dData->tempSettings.borderRadius[2] = test;

		if (dData->tempSettings.borderRadius[3] > test)
			dData->tempSettings.borderRadius[3] = test;

		if (element->imageLocation != NULL)
		{
			dData->tempSettings.bools = 1;
			char* location = element->imageLocation;
			BLIB::KeyPointerPair* img = BLIB::KeyPointerPair::GetKeyValuePointer(dData->modelTextures, location);

			if (img == NULL)
			{
				AddTexture(device, cL, location);
				img = BLIB::KeyPointerPair::GetKeyValuePointer(dData->modelTextures, location);
			}

			ImageStorage* imgS = (ImageStorage*)img->pointer;
			D3D12_GPU_DESCRIPTOR_HANDLE hnd = dData->textureHeap->GetGPUDescriptorHandleForHeapStart();
			hnd.ptr += textureHeapIncrementSize * imgS->index;
			dData->htmlModel->Draw(device, cL, drawSettingSize, &dData->tempSettings, &hnd);
		}
		else
			dData->htmlModel->Draw(device, cL, drawSettingSize, &dData->tempSettings, NULL);

		int charsize = element->characters.size();
		if (charsize > 0)
		{
			BLIB::KeyPointerPair* kpp = BLIB::KeyPointerPair::GetKeyValuePointer(dData->modelTextures, textTextureLocation);
			ImageStorage* imS = (ImageStorage*)kpp->pointer;
			D3D12_GPU_DESCRIPTOR_HANDLE hnd = dData->textureHeap->GetGPUDescriptorHandleForHeapStart();
			hnd.ptr += textureHeapIncrementSize * imS->index;

			dData->tempSettings.bools = 2;
			dData->tempSettings.font = element->font;
			dData->tempSettings.fontsize = element->fontsize;
			dData->tempSettings.scaleX = (imS->width / 100.0f) / imS->width; //this should be fixed, the 100 on this line and 2 on next should be dynamic
			dData->tempSettings.scaleY = (imS->height / 2.0f) / imS->height;

			for (int i = 0; i < charsize; i++)
			{
				dData->tempSettings.character = element->characters.at(i).character;
				dData->tempSettings.x = element->characters.at(i).x;
				dData->tempSettings.y = element->characters.at(i).y;
				dData->htmlModel->Draw(device, cL, drawSettingSize, &dData->tempSettings, &hnd);
			}
		}

		float elementScLeft = (element->x + element->bTl);
		float elementScRight = (element->x + element->actualWidth) - element->bTr;
		float elementScTop = (element->y + element->bTt);
		float elementScBottom = (element->y + element->actualHeight) - element->bTb;

		for (int i = 0; i < element->children.size(); i++)
		{
			if (element->overflowHandling == BLIB::HTMLOverflow::AUTO || element->overflowHandling == BLIB::HTMLOverflow::HIDDEN)
			{
				sc->left = elementScLeft;
				sc->right = elementScRight;
				sc->top = elementScTop;
				sc->bottom = elementScBottom;
				cL->RSSetScissorRects(1, sc);
			}

			BLIB::HTMLElement* chld = element->children.at(i);
			DrawHTML(chld, device, cL, dData, sc);
		}

		if (element->scrollBarScaleX < 1.0f)
			DrawHorizontalScrollbar(element, device, cL, dData);
		if (element->scrollBarScaleY < 1.0f)
			DrawVerticleScrollbar(element, device, cL, dData);

		if ((element->overflowHandling == BLIB::HTMLOverflow::AUTO || element->overflowHandling == BLIB::HTMLOverflow::HIDDEN) && element->children.size() > 0)
		{
			sc->left = elementScLeft;
			sc->right = elementScRight;
			sc->top = elementScTop;
			sc->bottom = elementScBottom;
			cL->RSSetScissorRects(1, sc);
		}
	}

	void Initialize(char* mainpage, int width, int height)
	{
		this->width = width;
		this->height = height;
		AddPage(mainpage);
		BLIB::KeyPointerPair* kpp = (BLIB::KeyPointerPair*)root->items[0];
		this->current = (BLIB::HTMLElement*)kpp->pointer;
	}

	void AddTexture(ID3D12Device10* device, ID3D12GraphicsCommandList* cL, const char* location)
	{
		ID3D12Resource* tempHndl = NULL;
		ImageStorage* f = new ImageStorage();
		f->index = dData->textureHeapCount;

		int width;
		int height;

		SetupTextureBuffers(device, cL, location, &f->upload, &f->resource, dData->textureHeap, f->index, &width, &height);

		f->width = width;
		f->height = height;

		dData->textureHeapCount++;

		BLIB::KeyPointerPair* nKpp = new BLIB::KeyPointerPair((char*)location, f);
		dData->modelTextures->AddPointer(nKpp);
	}

	void AddPage(char* path)
	{
		BLIB::DBuffer* file = BLIB::Files::ReadFile(path);
		file->Add((uint8_t)0);
		BLIB::HTMLElement* element = BLIB::HTMLParser::Parse((const char*)file->DataPointer(0));


		//////////////FIXXX MEEEEEEEEE path is assumed to just be file name ///////ALSO what about bGname ?????
		AddPage(BLIB::Strings::Replace(path, ".html", ""), element);

		if (element->children.size() > 0)
		{
			BLIB::KeyPointerPair* name = BLIB::KeyPointerPair::GetKeyValuePointer(element->children.at(0)->attributes, "bGname");
			bool destroy = false;
			if (name != NULL)
			{
				if (BLIB::KeyPointerPair::GetKeyValuePointer(pages, name->key) == NULL)
				{
					char* fileData = (char*)file->BufferOut(0, file->count);
					BLIB::KeyPointerPair* kpp = new BLIB::KeyPointerPair(BLIB::Strings::Clone((char*)name->pointer), fileData);
					pages->AddPointer(kpp);
				}
				else
				{
					destroy = true;
				}
			}
			else
			{
				destroy = true;
			}
			if(destroy)
			{
				BLIB::KeyPointerPair* page = (BLIB::KeyPointerPair*)root->items[root->count - 1];
				BLIB::HTMLElement* element = (BLIB::HTMLElement*)page->pointer;
				delete element;
				free(page->key);
				delete page;
				root->RemovePointer(root->count - 1);
			}
		}

		delete file;

		FindLinkedPages(element);
	}

	void AddPage(char* name, BLIB::HTMLElement* element)
	{
		element->width = width;
		element->height = height;

		GetComponents(element);
		BLIB::HTMLParser::ResolveDynamicHTML(element, variables, components);
		BLIB::HTMLParser::UpdateElementValues(element, variables);
		BLIB::HTMLParser::ResolveHTML(element, variables);

		BLIB::KeyPointerPair* kpp = new BLIB::KeyPointerPair(name, element);
		this->root->AddPointer(kpp);
	}

	void GetComponents(BLIB::HTMLElement* element)
	{
		if (element->tag != NULL)
		{
			char* fileName = BLIB::Strings::Concat(element->tag, ".html");

			BLIB::KeyPointerPair* found = BLIB::KeyPointerPair::GetKeyValuePointer(components, element->tag);
			if (found == NULL)
			{
				if (BLIB::Files::FileExists(fileName))
				{
					BLIB::DBuffer* file = BLIB::Files::ReadFile(fileName);
					file->Add((uint8_t)0);
					BLIB::HTMLElement* newelement = BLIB::HTMLParser::Parse((char*)file->DataPointer(0));
					BLIB::HTMLElement* child = newelement->children.at(newelement->children.size()-1);
					newelement->children.pop_back();
					components->AddPointer(new BLIB::KeyPointerPair(BLIB::Strings::Clone(element->tag), child));
					delete newelement;
					free(file);
				}
			}

			free(fileName);
		}

		for (int i = 0; i < element->children.size(); i++)
			GetComponents(element->children.at(i));
	}

	void SetVariable(const char* name, const char* json, bool allowRefresh = true)
	{
		BLIB::JSONElement* var = BLIB::JSONElement::GetElement(name, variables, true);

		if (var->type == BLIB::JSONElementType::NONE)
		{
			var->type = BLIB::JSONElementType::KVP;
		}

		if (var != NULL)
		{
			free(var->value);
			var->value = BLIB::Strings::Clone(json);
		}
		else
		{
			BLIB::JSONElement* newVar = new BLIB::JSONElement();
			newVar->name = BLIB::Strings::Clone(name);
			newVar->value = BLIB::Strings::Clone(json);
			variables->children->AddPointer(newVar);
		}

		if(allowRefresh)
			Refresh();
	}

	char* GetVariableValueReference(const char* name)
	{
		BLIB::JSONElement* var = BLIB::JSONElement::GetElement(name, variables);

		if (var != NULL)
			return (char*)var->value;
		return NULL;
	}

	void UpdatePageResolution(int width, int height)
	{
		current->width = width;
		current->height = height;
	}

	void DrawVerticleScrollbar(BLIB::HTMLElement* element, ID3D12Device10* device, ID3D12GraphicsCommandList* cL, DrawingData* dData)
	{
		/*dData->tempSettings.bools = 0;
		dData->tempSettings.backgroundColor[0] = 0 / 255.0f;
		dData->tempSettings.backgroundColor[1] = 0 / 255.0f;
		dData->tempSettings.backgroundColor[2] = 0 / 255.0f;
		dData->tempSettings.backgroundColor[3] = 128 / 255.0f;
		dData->tempSettings.width = element->scrollBarThickness;
		dData->tempSettings.height = element->actualHeight;
		dData->tempSettings.x = (element->x + element->actualWidth) - element->scrollBarThickness;
		dData->tempSettings.y = element->y;
		dData->tempSettings.borderRadius[0] = 0;
		dData->tempSettings.borderRadius[1] = 0;
		dData->tempSettings.borderRadius[2] = 0;
		dData->tempSettings.borderRadius[3] = 0;
		dData->htmlModel->Draw(device, cL, drawSettingSize, &dData->tempSettings, NULL);*/

		float halfies = element->scrollBarThickness / 2.0f;

		dData->tempSettings.bools = 0;
		dData->tempSettings.backgroundColor[0] = 64 / 255.0f;
		dData->tempSettings.backgroundColor[1] = 64 / 255.0f;
		dData->tempSettings.backgroundColor[2] = 64 / 255.0f;
		dData->tempSettings.backgroundColor[3] = 128 / 255.0f;
		dData->tempSettings.width = element->scrollBarThickness - element->scrollSpacing;
		dData->tempSettings.height = ((element->actualHeight - element->scrollSpacing)) * element->scrollBarScaleY;
		dData->tempSettings.x = (element->x + element->actualWidth) - element->scrollBarThickness + (element->scrollSpacing / 2);
		dData->tempSettings.y = element->y + (element->scrollSpacing / 2) + element->scrollPosY;
		dData->tempSettings.borderRadius[0] = halfies;
		dData->tempSettings.borderRadius[1] = halfies;
		dData->tempSettings.borderRadius[2] = halfies;
		dData->tempSettings.borderRadius[3] = halfies;
		dData->htmlModel->Draw(device, cL, drawSettingSize, &dData->tempSettings, NULL);
	}

	void DrawHorizontalScrollbar(BLIB::HTMLElement* element, ID3D12Device10* device, ID3D12GraphicsCommandList* cL, DrawingData* dData)
	{
		/*dData->tempSettings.bools = 0;
		dData->tempSettings.backgroundColor[0] = 0 / 255.0f;
		dData->tempSettings.backgroundColor[1] = 0 / 255.0f;
		dData->tempSettings.backgroundColor[2] = 0 / 255.0f;
		dData->tempSettings.backgroundColor[3] = 128 / 255.0f;
		dData->tempSettings.width = element->actualWidth;
		dData->tempSettings.height = element->scrollBarThickness;
		dData->tempSettings.x = element->x;
		dData->tempSettings.y = (element->y + element->actualHeight) - element->scrollBarThickness;
		dData->tempSettings.borderRadius[0] = 0;
		dData->tempSettings.borderRadius[1] = 0;
		dData->tempSettings.borderRadius[2] = 0;
		dData->tempSettings.borderRadius[3] = 0;
		dData->htmlModel->Draw(device, cL, drawSettingSize, &dData->tempSettings, NULL);*/

		float halfies = element->scrollBarThickness / 2.0f;

		dData->tempSettings.bools = 0;
		dData->tempSettings.backgroundColor[0] = 64 / 255.0f;
		dData->tempSettings.backgroundColor[1] = 64 / 255.0f;
		dData->tempSettings.backgroundColor[2] = 64 / 255.0f;
		dData->tempSettings.backgroundColor[3] = 128 / 255.0f;
		dData->tempSettings.width = ((element->actualWidth - element->scrollSpacing)) * element->scrollBarScaleX;
		dData->tempSettings.height = element->scrollBarThickness - element->scrollSpacing;
		dData->tempSettings.x = element->x + (element->scrollSpacing / 2) + element->scrollPosX;
		dData->tempSettings.y = (element->y + element->actualHeight) - element->scrollBarThickness + (element->scrollSpacing / 2);
		dData->tempSettings.borderRadius[0] = halfies;
		dData->tempSettings.borderRadius[1] = halfies;
		dData->tempSettings.borderRadius[2] = halfies;
		dData->tempSettings.borderRadius[3] = halfies;
		dData->htmlModel->Draw(device, cL, drawSettingSize, &dData->tempSettings, NULL);
	}

	bool OnVerticalScrollbar(BLIB::HTMLElement* element, int x, int y)
	{
		float width = element->scrollBarThickness - element->scrollSpacing;
		float height = ((element->actualHeight - element->scrollSpacing)) * element->scrollBarScaleY;
		float sx = (element->x + element->actualWidth) - element->scrollBarThickness + (element->scrollSpacing / 2);
		float sy = element->y + (element->scrollSpacing / 2) + element->scrollPosY;

		return x >= sx && x <= sx + width && y >= sy && y <= sy + height;
	}

	bool OnHorizontalScrollbar(BLIB::HTMLElement* element, int x, int y)
	{
		float width = ((element->actualWidth - element->scrollSpacing)) * element->scrollBarScaleX;
		float height = element->scrollBarThickness - element->scrollSpacing;
		float sx = element->x + (element->scrollSpacing / 2) + element->scrollPosX;
		float sy = (element->y + element->actualHeight) - element->scrollBarThickness + (element->scrollSpacing / 2);
		
		return x >= sx && x <= sx + width && y >= sy && y <= sy + height;
	}

	void Refresh()
	{
		BLIB::HTMLElement::HTMLInvalidateAll(current);
		BLIB::HTMLParser::ResolveDynamicHTML(current, variables, components);
		BLIB::HTMLParser::UpdateElementValues(current, variables);
		BLIB::HTMLParser::ResolveHTML(current, variables);
	}

	~BGraph()
	{
		delete htmlGModule;
		dData->textureHeap->Release();
		delete dData->htmlModel;

		for (int i = 0; i < dData->modelTextures->count; i++)
		{
			BLIB::KeyPointerPair* kpp = (BLIB::KeyPointerPair*)dData->modelTextures->items[i];
			//Not Deleting the key because HTMLElement does that
			ImageStorage* img = (ImageStorage*)kpp->pointer;
			img->resource->Release();
			img->upload->Release();
			delete img;
			delete kpp;
		}
		dData->modelTextures->FreeEverything();
		delete dData->modelTextures;

		delete dData;

		for (int i = 0; i < files.size(); i++)
		{
			delete files.at(i);
		}

		for (int i = 0; i < root->count; i++)
		{
			BLIB::KeyPointerPair* page = (BLIB::KeyPointerPair*)root->items[i];
			BLIB::HTMLElement* element = (BLIB::HTMLElement*)page->pointer;
			delete element;
			free(page->key);
			delete page;
		}

		for (int i = 0; i < functions->count; i++)
		{
			BLIB::KeyPointerPair* kpp = (BLIB::KeyPointerPair*)functions->items[i];
			free(kpp->key);
			// Not Freeing the function pointers.
			delete kpp;
		}
		for (int i = 0; i < pages->count; i++)
		{
			BLIB::KeyPointerPair* kpp = (BLIB::KeyPointerPair*)pages->items[i];
			free(kpp->key);
			free(kpp->pointer); 
			delete kpp;
		}
		for (int i = 0; i < components->count; i++)
		{
			BLIB::KeyPointerPair* kpp = (BLIB::KeyPointerPair*)components->items[i];
			free(kpp->key);
			BLIB::HTMLElement* element = (BLIB::HTMLElement*)kpp->pointer;
			delete element;
			delete kpp;
		}

		delete root;

		//functions->FreeEverything();
		//pages->FreeEverything();
		delete functions;
		delete pages;
		delete variables;
		delete components;
	}
};