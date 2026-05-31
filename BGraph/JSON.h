#pragma once 

enum JSONElementType
{
	NONE,
	RENAMED_REFERENCE,
	OBJECT,
	ARRAY,
	FLAT,
	KVP
};

class JSONElement
{
private:
	JSONElement(JSONElement&) = delete;
public:
	JSONElement();
	JSONElementType type;
	char* name;
	char* value;
	JSONElement* parent;
	JSONElement* ref;
	PointerList* children = new PointerList();
	~JSONElement();

	static JSONElement* GetElement(const char* varName, JSONElement* element, bool canCreate = true);
	static JSONElement* CreateRenamedReference(const char* newName, JSONElement* element);
	static JSONElement* ConvertBlankToRenamedReference(JSONElement* convertee, const char* newName, JSONElement* element);
	static void CreateRootPathFromReference(JSONElement* element, char** ref);
};

class JSON
{
public:
	static PointerList* GetKeys(const char* json);

	static char* GetValue(const char* property, const char* json);

	static char* GetArrayItem(const char* json, uint64_t index);

	static uint64_t GetArrayLength(const char* json);

	static void DynamicArrayParser(uint64_t* outputCount, void** outputArray, char* json, uint64_t datatypesize, void (*DynamicConstructor)(void* mylocation, char* json));

	static void Uint64_tFromJSON(void* mylocation, char* value);

	static void FloatFromJSON(void* mylocation, char* value);

	static char* ResolveValue(const char* json, const char* variableName);

	static JSONElement* CreateJSONElementTree(const char* json);
};
