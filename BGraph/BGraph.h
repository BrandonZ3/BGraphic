#pragma once
#include "../../CPP Lib/BLIB.h"
#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include <d3d12.h>
#include <d3dcompiler.h>
#include <string>



void SetupTextureBuffers(ID3D12Device10* device, ID3D12GraphicsCommandList* commandList, char* file, ID3D12Resource** upload, ID3D12Resource** gpuBuffer, ID3D12DescriptorHeap* cbvsrvuavHeap, int cbvsrvuavIndex)
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
};

class FlatImageStorage
{
public:
	int index = -1;
	ID3D12Resource* resource;
	ID3D12Resource* upload;
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
};

class GraphicsModel
{
private:
	GraphicsModel(GraphicsModel&) = delete;
public:
	GraphicsModule2D* graphicsModule;

	ID3D12Resource* posU = NULL;
	ID3D12Resource* posR = NULL;
	ID3D12Resource* texU = NULL;
	ID3D12Resource* texR = NULL;

	D3D12_VERTEX_BUFFER_VIEW Positions;
	D3D12_VERTEX_BUFFER_VIEW TexCoords;

	ID3D12Resource* indR;
	ID3D12Resource* indU;
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

	void ReleaseTemporaryBuffers()
	{
		if (this->graphicsModule->indexed)
			indU->Release();
		if (posU != NULL)
			posU->Release();
		if (texU != NULL)
			texU->Release();
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
	
	void Draw(ID3D12Device10* device, ID3D12GraphicsCommandList* cL, DrawSettings* settings, D3D12_GPU_DESCRIPTOR_HANDLE* texViewHandle)
	{
		if (this->graphicsModule->indexed)
			cL->IASetIndexBuffer(&this->Indices);
		if (this->graphicsModule->iaPosition != -1)
			cL->IASetVertexBuffers(this->graphicsModule->iaPosition, 1, &this->Positions);
		if (this->graphicsModule->iaTexCoords != -1)
			cL->IASetVertexBuffers(this->graphicsModule->iaTexCoords, 1, &this->TexCoords);

		if (texViewHandle != NULL)
			cL->SetGraphicsRootDescriptorTable(1, *texViewHandle);

		cL->SetGraphicsRoot32BitConstants(0, (sizeof(DrawSettings) / 4), settings, 0);
		cL->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
	}
};

class DrawingData
{
public:
	GraphicsModel* flat;
	std::map<char*, FlatImageStorage> flatModelTextures;
	ID3D12DescriptorHeap* flatTextureHeap;
	int flatTextureHeapCount = 0;
	DrawSettings tempSettings;
};

class BGraph
{
	std::vector<char*> files;	//name and fn*
	BLIB::PointerList* functions = new BLIB::PointerList();	//name and fn*
	BLIB::PointerList* pages = new BLIB::PointerList();		//name and json
	BLIB::PointerList* variables = new BLIB::PointerList();	//name and json

	BLIB::PointerList* root = new BLIB::PointerList();
	BLIB::HTMLElement* current = NULL;

	BGraph(BGraph&) = delete;
	DrawingData* dData = new DrawingData();

	ID3D12Device10* device = NULL;
	ID3D12GraphicsCommandList* commandList = NULL;
	GraphicsModule2D* gModule = NULL;

	int height;
	int width;

	void HandleClick(BLIB::HTMLElement* element, char* page)
	{

	}

	void bgNavigate(const char* pageName)
	{
		BLIB::KeyPointerPair* value = BLIB::KeyPointerPair::GetKeyValuePointer(root, pageName);
		if (value != NULL)
		{
			current = (BLIB::HTMLElement*)value->pointer;
		}
	}

	bool HandleHTMLClick(BLIB::HTMLElement* element, int x, int y)
	{
		for (int i = element->children.size() - 1; i >= 0; i--)
		{
			if (HandleHTMLClick(element->children.at(i), x, y))
				return true;
		}

		float left = element->x;
		float top = element->y;
		float right = element->x + element->actualWidth;
		float bottom = element->y + element->actualHeight;

		if (x > left && x < right && y > top && y < bottom)
		{
			BLIB::KeyPointerPair* value = BLIB::KeyPointerPair::GetKeyValuePointer(element->attributes, "bGClick");
			if (value != NULL)
			{
				BLIB::PointerList* clickargs = BLIB::PointerList::SplitString((char*)value->pointer, ";");

				if (clickargs->count == 2 && BLIB::Strings::Compare((char*)clickargs->items[0], "bGNavigate"))
				{
					///////////////// Need to make sure that bGname is taken out and the name of the file is the pagename.
					bgNavigate((char*)clickargs->items[1]);
				}

				clickargs->FreeEverything();
			}

			return true;
		}
		return false;
	}

public:
	BGraph(ID3D12Device10* device, ID3D12GraphicsCommandList* commandList)
	{
		this->device = device;
		this->commandList = commandList;
		Create2DPipeline();

		D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};
		dhDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		dhDesc.NodeMask = 0;
		dhDesc.NumDescriptors = 1000;
		dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		device->CreateDescriptorHeap(&dhDesc, __uuidof(ID3D12DescriptorHeap), (void**)&(dData->flatTextureHeap)) == S_OK ? "" : throw "Broken";

		dData->flat = new GraphicsModel(device, commandList, gModule);
	}

	void HandleClick(int x, int y)
	{
		HandleHTMLClick(current, x, y);
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

	void Create2DPipeline()
	{
		ID3D12PipelineState* pipeline;
		ID3D12RootSignature* root;

		D3D12_ROOT_CONSTANTS matrices = {};
		matrices.Num32BitValues = (sizeof(DrawSettings) / 4) + 1;
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
		ssDesc.BorderColor == D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;

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

		ID3DBlob* vsBlob = NULL;
		ID3DBlob* vserror = NULL;
		ID3DBlob* psBlob = NULL;
		ID3DBlob* pserror = NULL;

		D3DCompileFromFile(L"C:\\Users\\brand\\source\\repos\\Graphics Again\\Graphics Again\\2dshader.hlsl", NULL, NULL, "VSMain", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, NULL, &vsBlob, &vserror) == S_OK ? "" : throw "Failed To Compile Vertex Shader!";

		if (vserror != NULL)
		{
			char* vscerr = (char*)vserror->GetBufferPointer();
			OutputDebugStringA(vscerr);
			OutputDebugStringA("\r\nPS:");
		}

		D3DCompileFromFile(L"C:\\Users\\brand\\source\\repos\\Graphics Again\\Graphics Again\\2dshader.hlsl", NULL, NULL, "PSMain", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, NULL, &psBlob, &pserror) == S_OK ? "" : throw "Failed To Compile Vertex Shader!";

		if (pserror != NULL)
		{
			char* pscerr = (char*)pserror->GetBufferPointer();
			OutputDebugStringA(pscerr);
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

		gModule = new GraphicsModule2D(pipeline, root);
		gModule->indexed = true;
		gModule->iaPosition = 0;
		gModule->iaTexCoords = 1;
		gModule->topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}

	void DrawPage()
	{
		commandList->SetPipelineState(this->dData->flat->graphicsModule->pipeline);
		commandList->SetGraphicsRootSignature(this->dData->flat->graphicsModule->rootSignature);
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dData->tempSettings.renderWidth = this->width;
		dData->tempSettings.renderHeight = this->height;
		DrawHTML(current, device, commandList, dData);
	}

	void DrawHTML(BLIB::HTMLElement* element, ID3D12Device10* device, ID3D12GraphicsCommandList* cL, DrawingData* dData)
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
			FlatImageStorage imgS = dData->flatModelTextures[location];

			int size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			if (imgS.index != -1)
			{
				D3D12_GPU_DESCRIPTOR_HANDLE hnd = dData->flatTextureHeap->GetGPUDescriptorHandleForHeapStart();
				hnd.ptr += size * imgS.index;
				dData->flat->Draw(device, cL, &dData->tempSettings, &hnd);
			}
			else
			{
				ID3D12Resource* tempHndl = NULL;
				FlatImageStorage f = { dData->flatTextureHeapCount, };
				D3D12_GPU_DESCRIPTOR_HANDLE hnd = dData->flatTextureHeap->GetGPUDescriptorHandleForHeapStart();
				hnd.ptr += size * f.index;

				SetupTextureBuffers(device, cL, location, &f.upload, &f.resource, dData->flatTextureHeap, f.index);

				dData->flatTextureHeapCount++;
				dData->flatModelTextures[location] = f;

				dData->flat->Draw(device, cL, &dData->tempSettings, &hnd);
			}
		}
		else
			dData->flat->Draw(device, cL, &dData->tempSettings, NULL);

		for (int i = 0; i < element->children.size(); i++)
		{
			DrawHTML(element->children.at(i), device, cL, dData);
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

		ResolvePage(element);

		BLIB::KeyPointerPair* kpp = new BLIB::KeyPointerPair(name, element);
		this->root->AddPointer(kpp);
	}

	void ResolvePage(BLIB::HTMLElement* element)
	{
		bool dirty = true;
		while (dirty)
		{
#ifdef BGRAPH_DEBUG
			char* string = (char*)malloc(1);
			if (string != NULL)
			{
				string[0] = 0;

				BLIB::HTMLElement::HTMLPrintTree(element, 0, &string);

				BLIB::DBuffer* buf = new BLIB::DBuffer();
				buf->Add((unsigned char*)string, BLIB::Strings::Length(string));
				BLIB::Files::WriteFile((char*)"TempTreeJSON.json", buf);
				delete buf;
				free(string);
#endif
				BLIB::HTMLElement::HTMLCalculateSizesAndPositionsTD(element);
				BLIB::HTMLElement::HTMLCalculateSizesAndPositionsBU(element);
				BLIB::HTMLElement::HTMLHandleFlexPositioning(element);
				dirty = BLIB::HTMLElement::HTMLAnyDirty(element);
#ifdef BGRAPH_DEBUG
			}
#endif
		}
	}

	~BGraph()
	{
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
		}

		for (int i = 0; i < functions->count; i++)
		{
			BLIB::KeyPointerPair* kpp = (BLIB::KeyPointerPair*)functions->items[i];
			free(kpp->key);
		}
		for (int i = 0; i < pages->count; i++)
		{
			BLIB::KeyPointerPair* kpp = (BLIB::KeyPointerPair*)functions->items[i];
			free(kpp->key);
			free(kpp->pointer);
		}
		for (int i = 0; i < variables->count; i++)
		{
			BLIB::KeyPointerPair* kpp = (BLIB::KeyPointerPair*)functions->items[i];
			free(kpp->key);
			free(kpp->pointer);
		}

		functions->FreeEverything();
		pages->FreeEverything();
		variables->FreeEverything();
		delete functions;
		delete pages;
		delete variables;
	}
};