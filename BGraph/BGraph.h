#pragma once
//#define STB_IMAGE_IMPLEMENTATION	//These 2 lines should be included in the project that uses BGraph.
//#include "stb_image.h"
#include <d3d12.h>
#include <d3dcompiler.h>
#include <string>
#include <vector>

#include "../../Library/Containers/PointerList.h"
#include "../../Library/Parsing/JSON.h"
#include "../../Library/Parsing/HTML.h"
#include "../../Library/Containers/DBuffer.h"
#include "../../Library/Containers/KeyPointerPair.h"
#include "../../Library/Base/Strings.h"
#include "../../Library/Window/Keyboard.h"
#include "../../Library/IO/Files.h"


class DrawSettings
{
public:
	int renderWidth;
	int renderHeight;
	float color[4];
	float width;
	float height;
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

	GraphicsModule2D(ID3D12PipelineState* pipeline, ID3D12RootSignature* rootSignature);
	void SetTopology(D3D_PRIMITIVE_TOPOLOGY topo);
	~GraphicsModule2D();
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

	GraphicsModel(ID3D12Device10* device, ID3D12GraphicsCommandList* commandList, GraphicsModule2D* graphmod);
	void SetupVertexBuffer(ID3D12Device10* device, ID3D12GraphicsCommandList* commandList, char* refdata, UINT byteLength, ID3D12Resource** upload, ID3D12Resource** gpuBuffer, D3D12_VERTEX_BUFFER_VIEW* view, uint64_t dataInstanceSize);
	void SetupIndexBuffer(ID3D12Device10* device, ID3D12GraphicsCommandList* commandList, char* refdata, UINT byteLength, ID3D12Resource** upload, ID3D12Resource** gpuBuffer, D3D12_INDEX_BUFFER_VIEW* view, uint64_t dataInstanceSize);
	void Draw(ID3D12Device10* device, ID3D12GraphicsCommandList* cL, UINT settingSize, void* settings, D3D12_GPU_DESCRIPTOR_HANDLE* texViewHandle);
	void ReleaseTemporaryBuffers();
	~GraphicsModel();
};

class DrawingData
{
public:
	GraphicsModel* htmlModel;
	PointerList* modelTextures = new PointerList();
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
	PointerList* functions = new PointerList();	//name and fn*
	PointerList* pages = new PointerList();		//name and json
	JSONElement* variables = new JSONElement();
	PointerList* components = new PointerList();
	PointerList* changedVariables = new PointerList();

	PointerList* root = new PointerList();
	HTMLElement* current = NULL;
	HTMLElement* active = NULL;
	HTMLElement* lastTouched = NULL;

	BGraph(BGraph&) = delete;
	DrawingData* dData = new DrawingData();

	ID3D12Device10* device = NULL;
	ID3D12GraphicsCommandList* commandList = NULL;
	GraphicsModule2D* htmlGModule = NULL;

	int height;
	int width;

	DBuffer* activeKeys = new DBuffer();
	\
	void bGNavigate(const char* pageName);
	void bGStyle(HTMLElement* element, char* style);
	void bGSet(HTMLElement* element, const char* varname, const char* value, bool allowRefresh = true);
	bool HandleHTMLScroll(HTMLElement* element, int x, int y, int scroll, bool shift, bool control);
	bool HandleHTMLHover(HTMLElement* element, int x, int y, bool allowPositive);
	bool HandleHTMLMouseUp(HTMLElement* element, int x, int y);
	bool HandleHTMLMouseDown(HTMLElement* element, int x, int y, bool allowPositive);
	bool HandleHTMLClick(HTMLElement* element, int x, int y, bool allowPositive);
	bool TestCoordinates(HTMLElement* element, int x, int y);

public:
	BGraph(ID3D12Device10* device, ID3D12GraphicsCommandList* commandList);
	void UpdateActiveKeys(DBuffer* keys);
	bool HandleScroll(int x, int y, int scroll, bool shift, bool control);
	void HandleClick(int x, int y);
	void HandleMouseDown(int x, int y);
	void HandleMouseUp(int x, int y);
	void HandleHover(int x, int y);
	PointerList* GetChangedVariables();
	void ClearChangedVariables();
	void DeleteVariable(char* variableName);
	void ScrollChangeAdapterX(HTMLElement* element, float* change);
	void ScrollChangeAdapterY(HTMLElement* element, float* change);
	void FindLinkedPages(HTMLElement* element);
	void CreateHTMLPipeline();
	void DrawPage(D3D12_VIEWPORT* vp, D3D12_RECT* sc);
	void DrawHTML(HTMLElement* element, ID3D12Device10* device, ID3D12GraphicsCommandList* cL, DrawingData* dData, D3D12_RECT* sc);
	void Initialize(char* mainpage, int width, int height);
	void AddTexture(ID3D12Device10* device, ID3D12GraphicsCommandList* cL, const char* location);
	void AddPage(char* path);
	void AddPage(char* name, HTMLElement* element);
	void GetComponents(HTMLElement* element);
	void CreateVariable(const char* name, JSONElementType type);
	void SetVariable(const char* name, const char* json, bool allowRefresh = true);
	JSONElement* GetVariable(const char* name);
	const char* GetVariableValueReference(const char* name);
	void UpdatePageResolution(int width, int height);
	void DrawVerticleScrollbar(HTMLElement* element, ID3D12Device10* device, ID3D12GraphicsCommandList* cL, DrawingData* dData);
	void DrawHorizontalScrollbar(HTMLElement* element, ID3D12Device10* device, ID3D12GraphicsCommandList* cL, DrawingData* dData);
	bool OnVerticalScrollbar(HTMLElement* element, int x, int y);
	bool OnHorizontalScrollbar(HTMLElement* element, int x, int y);
	void Refresh();
	~BGraph();
};