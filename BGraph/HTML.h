#pragma once 

enum HTMLElementType
{
	ROOT,
	BODY,
	DIV,
	INP,
	IMG,
	P,
	CUSTOM
};

enum HTMLScaling
{
	HTML_AUTO,
	PX,
	PERCENTAGE,
	FIT_CONTENT
};

enum HTMLDisplayType
{
	BLOCK,
	FLEX
};

enum HTMLBoxSizing
{
	CONTENT_BOX,
	BORDER_BOX
};

enum HTMLFlexPosition
{
	FLEX_AUTO,
	START,
	CENTER,
	END,
	SPACE_BETWEEN,
	SPACE_AROUND
};

enum HTMLElementPosition
{
	POS_STATIC,
	POS_RELATIVE,
	POS_ABSOLUTE,
	POS_FIXED
};

enum HTMLTextPosition
{
	TEXT_START,
	TEXT_CENTER,
	TEXT_END
};

enum HTMLOverflow
{
	AUTO,
	HIDDEN,
	VISIBLE
};

enum HTMLInputType
{
	TEXT,
	TEXTAREA,
	CHECKBOX,
	RANGE
};

class HTMLTextCharacter
{
public:
	int character;
	float x;
	float y;
};

class HTMLElement
{
	HTMLElement(HTMLElement&) = delete;

public:

	PointerList* attributes = new PointerList();
	std::vector<HTMLElement*> children;
	HTMLElement* parent;
	HTMLElementType type;
	char* tag = NULL;

	//Dirty?
	bool dirtyWidth = true;
	bool dirtyHeight = true;
	bool dirtyPositionX = true;
	bool dirtyPositionY = true;
	bool flexDirtyHeight = true;
	bool flexDirtyWidth = true;
	bool textDirty = true;
	bool scrollBarScaleXDirty = true;
	bool scrollBarScaleYDirty = true;
	bool flexStretchDirty = true;

	//Color Variables
	//background
	int bGr;
	int bGg;
	int bGb;
	int bGa;
	//color (text)
	int cr;
	int cg;
	int cb;
	int ca;
	//border
	int blr;
	int blg;
	int blb;
	int bla;

	int btr;
	int btg;
	int btb;
	int bta;

	int brr;
	int brg;
	int brb;
	int bra;

	int bbr;
	int bbg;
	int bbb;
	int bba;

	//border radius
	int radiusTopLeft;
	int radiusTopRight;
	int radiusBottomLeft;
	int radiusBottomRight;

	HTMLDisplayType display = BLOCK;
	HTMLFlexPosition justifyContent = FLEX_AUTO;
	HTMLFlexPosition alignItems = FLEX_AUTO;
	bool flexRow = true;
	bool flexWrap = false;
	bool flexGrow = false;
	bool flexShrink = false;
	float flexBasis = 0;
	HTMLScaling flexBasisType = HTML_AUTO; // Normally auto
	HTMLBoxSizing boxSizing = CONTENT_BOX;
	float gap = 0;

	//sizing
	int minWidth = 0;
	HTMLScaling minWidthScaling = PX;
	int maxWidth = 9999;
	HTMLScaling maxWidthScaling = PX;
	int width = 100;
	HTMLScaling widthScaling = HTMLScaling::PERCENTAGE;//this is not standard, i think standard is auto, which is basically the same thing //HTMLScaling::FIT_CONTENT;

	int minHeight = 0;
	HTMLScaling minHeightScaling = PX;
	int maxHeight = 9999;
	HTMLScaling maxHeightScaling = PX;
	int height;
	HTMLScaling heightScaling = HTMLScaling::FIT_CONTENT;

	int bTl;
	int bTt;
	int bTr;
	int bTb;
	//padding
	int pl;
	int pt;
	int pr;
	int pb;
	//margin
	int ml;
	int mt;
	int mr;
	int mb;

	//position/drawing
	int zIndex = 0;
	HTMLElementPosition position = POS_STATIC;
	bool leftSet = false;
	int left = 0;
	bool topSet = false;
	int top = 0;
	bool rightSet = false;
	int right = 0;
	bool bottomSet = false;
	int bottom = 0;
	float x;
	float y;
	float actualWidth;
	float actualHeight;

	//image
	char* imageLocation = NULL;

	//dynamic rendering
	std::vector<HTMLElement*> conditionals;
	std::vector<HTMLElement*> generators;

	//content
	HTMLTextPosition textAlign = TEXT_START;
	int font = 1;
	int fontsize = 12;
	char* text = NULL;
	bool textwrap = true;
	std::vector<HTMLTextCharacter> characters;


	//control
	bool hover = false;
	bool active = false;
	HTMLInputType inputType = TEXT;
	bool checked = false;


	//overflow
	HTMLOverflow overflowHandling = VISIBLE;
	bool overflowX = false;
	bool overflowY = false;
	double scrollPosX = 0;
	double scrollPosY = 0;
	float scrollBarThickness = 10.0f;
	float scrollSpacing = 4.0f;
	double scrollBarScaleX = 1;
	double scrollBarScaleY = 1;
	double scrollBarShiftScaleX = 1;
	double scrollBarShiftScaleY = 1;
	bool scrollXSelected = false;
	bool scrollYSelected = false;
	double scrollStartX = 0;
	double scrollStartY = 0;




	int GetChildIndex(HTMLElement* child);
	HTMLElement(HTMLElementType type);
	void CopyFlatDetailsToElement(HTMLElement* element);
	~HTMLElement();

	static float GetElementMinContentWidth(HTMLElement* element);
	static float GetElementMaxContentWidth(HTMLElement* element);
	static float GetElementExtraWidth(HTMLElement* element);
	static float GetElementInternalExtraWidth(HTMLElement* element);
	static float GetElementExternalExtraWidth(HTMLElement* element);
	static float GetInternalBoxSizedWidth(float widthPX, HTMLElement* element);
	static float GetElementOverallWidth(HTMLElement* element);
	static float GetElementContentWidth(HTMLElement* element);

	static float GetElementMinContentHeight(HTMLElement* element);
	static float GetElementMaxContentHeight(HTMLElement* element);
	static float GetElementExtraHeight(HTMLElement* element);
	static float GetElementInternalExtraHeight(HTMLElement* element);
	static float GetElementExternalExtraHeight(HTMLElement* element);
	static float GetInternalBoxSizedHeight(float heightPX, HTMLElement* element);
	static float GetElementOverallHeight(HTMLElement* element);
	static float GetElementContentHeight(HTMLElement* element);

	static float GetChildFlexContentWidth(HTMLElement* chld);
	static float GetChildFlexContentHeight(HTMLElement* chld);

	static void HTMLCalculateFlexStretchingWidth(HTMLElement* element);
	static void HTMLDistributeRemainingFlexWidthSpacing(HTMLElement* element, float* remaining, int* growers, int* shrinkers);
	static void HTMLCalculateFlexStretchingHeight(HTMLElement* element);
	static void HTMLDistributeRemainingFlexHeightSpacing(HTMLElement* element, float* remaining, int* growers, int* shrinkers);

	static void HTMLCalculateSizesAndPositionsTD(HTMLElement* element);
	static void HTMLCalculateFlexWrap(HTMLElement* element);
	static void HTMLCalculateSizesAndPositionsBU(HTMLElement* element, JSONElement* variables);
	static void HTMLHandleFlexPositioning(HTMLElement* element);
	static void HTMLHandleTextCharacters(HTMLElement* element, JSONElement* variables);
	static char* HTMLStringInterpolate(const char* text, JSONElement* variables);
	static void HTMLInvalidateChildrenPositions(HTMLElement* element);
	static void HTMLInvalidateAll(HTMLElement* element);
	static void HTMLInvalidateDependantChildrenWidth(HTMLElement* element);
	static void HTMLInvalidateDependantChildrenHeight(HTMLElement* element);

	static void HTMLShiftChildrenPositions(HTMLElement* element, int shiftLeft, int shiftTop, int shiftRight, int shiftBottom);
	static bool HTMLAnyDirty(HTMLElement* element);
	static void HTMLPrintTree(HTMLElement* element, int depth, char** string);
	static HTMLElement* GetPositionReferableParent(HTMLElement* chld);
	static bool IsDescendantOf(HTMLElement* ancestor, HTMLElement* chld);
	static HTMLElement* GetScissorParent(HTMLElement* chld);
	static HTMLElement* GetPreviousSpaceTakingSibling(int current, HTMLElement* parent);
	static void ApplyScrolling(float x, float y, HTMLElement* element, HTMLElement* scrollOriginator);
	static void RecursiveApplyScrolling(HTMLElement* element, HTMLElement* scrollOriginator);
	static void CalculateScrollbars(HTMLElement* element);
};

class HTMLParser
{
public:
	static HTMLElement* Parse(const char* document);
	static void ParseStyling(const char* style, HTMLElement* element);
	static void ResolveHTML(HTMLElement* element, JSONElement* variables);
	static void ResolveDynamicHTML(HTMLElement* element, JSONElement* variables, PointerList* components);//variables should be KeyPointerPair
	static HTMLElement* RecursiveGenerate(HTMLElement* ref, HTMLElement* parent, JSONElement* tempVariables, JSONElement* variables);
	static HTMLElement* RecreateElement(HTMLElement* ref, HTMLElement* parent);
	//Components Function
	static void FindAndReplaceComponentsVariables(HTMLElement* container, HTMLElement* element);
	static void UpdateElementValues(HTMLElement* element, JSONElement* variables);
};
