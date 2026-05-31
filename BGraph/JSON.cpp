#include "JSON.h"  

//JSONElement
JSONElement::JSONElement()
{

}

JSONElement::~JSONElement()
{

	if (this->parent != NULL)
	{
		for (int i = 0; i < this->parent->children->count; i++)
		{
			JSONElement* child = (JSONElement*)this->parent->children->items[i];

			if (child == this)
			{
				this->parent->children->RemovePointer(i);
				break;
			}
		}
	}

	if (this->type != RENAMED_REFERENCE)
	{
		for (int i = 0; i < children->count; i++)
		{
			JSONElement* element = (JSONElement*)this->children->items[i];
			delete element;
		}
		delete children;
		children = NULL;
	}

	if (name != NULL)
	{
		free(name);
		name = NULL;
	}
	if (value != NULL)
	{
		free(value);
		value = NULL;
	}
}

JSONElement* JSONElement::GetElement(const char* varName, JSONElement* element, bool canCreate)
{
	JSONElement* result = NULL;
	char* start = NULL;
	char* following = NULL;


	if (varName != NULL)
	{
		/*if (element->name != NULL && Strings::Compare(varName, element->name))
			return element;*/

		int index = Strings::IndexOf(varName, ".", 1);
		int length = Strings::Length(varName);
		uint64_t val = -1;
		bool isNumber = false;

		if (index != -1)
		{
			start = Strings::Substring(varName, 0, index);
			following = Strings::Substring(varName, index + 1, length - index);
		}
		else
		{
			start = (char*)varName; //this is a bit naughty
		}

		isNumber = Strings::IsNumeric(start);

		if (isNumber)
			val = Strings::StringToUint(start);

		if (element->children->count == 0 && element->type == NONE)
		{
			if (isNumber)
				element->type = ARRAY;
			else
				element->type = OBJECT;
		}

		if (element->type == ARRAY || (element->type == RENAMED_REFERENCE && element->ref->type == ARRAY))///
		{
			if (element->children->count > val)
			{
				result = (JSONElement*)element->children->items[val];
			}
			else if (canCreate)
			{
				for (int i = element->children->count; i < val + 1; i++)
				{
					JSONElement* val = new JSONElement();
					val->name = Strings::UintToString(i);
					if (element->type == RENAMED_REFERENCE)
						val->parent = element->ref;//ref reference of renamed_reference type is the original object it is referencing
					else
						val->parent = element;
					val->type = NONE;
					element->children->AddPointer(val);
				}
				result = (JSONElement*)element->children->items[val];
			}
			if (following != NULL && result != NULL)
				result = JSONElement::GetElement(following, result, canCreate);
		}
		else
			for (int i = 0; i < element->children->count; i++)
			{
				JSONElement* chld = (JSONElement*)element->children->items[i];

				if (Strings::Compare(start, chld->name))
				{
					if (following != NULL)
						result = JSONElement::GetElement(following, chld, canCreate);
					else
						result = chld;
					break;
				}
				//else if(->)
			}

		if (canCreate && result == NULL && (element->type == OBJECT || (element->type == RENAMED_REFERENCE && element->ref != NULL)))
		{
			result = new JSONElement();
			element->children->AddPointer(result);
			if (element->type == RENAMED_REFERENCE)
				result->parent = element->ref;//ref reference of renamed_reference type is the original object it is referencing
			else
				result->parent = element;
			result->name = Strings::Clone(start);
			result->type = NONE;

			if (following != NULL)
			{
				result = JSONElement::GetElement(following, result, canCreate);
			}
		}

		if (index != -1)
		{
			free(start);
			if (following != NULL)
				free(following);
		}
	}

	return result;
}

JSONElement* JSONElement::CreateRenamedReference(const char* newName, JSONElement* element)
{
	JSONElement* fakeItem = new JSONElement();

	delete fakeItem->children;

	fakeItem->type = RENAMED_REFERENCE;
	fakeItem->name = Strings::Clone(newName);
	//fakeItem->parent = element; //This is for inside of generator loops
	fakeItem->ref = element;
	fakeItem->children = element->children;
	return fakeItem;
}

JSONElement* JSONElement::ConvertBlankToRenamedReference(JSONElement* convertee, const char* newName, JSONElement* element)
{
	if (convertee->children->count != 0 || convertee->type != NONE || convertee->name != NULL || convertee->value != NULL)
	{
		for (int i = 0; i < convertee->children->count; i++)
		{
			JSONElement* element = (JSONElement*)convertee->children->items[i];
			delete element;
		}

		if (convertee->name != NULL)
			free(convertee->name);
		if (convertee->value != NULL)
			free(convertee->value);
	}

	delete convertee->children;
	convertee->type = RENAMED_REFERENCE;
	convertee->name = Strings::Clone(newName);
	//convertee->parent = element; //This is for inside of generator loops
	convertee->ref = element;
	convertee->children = element->children;
	return convertee;
}

void JSONElement::CreateRootPathFromReference(JSONElement* element, char** ref)
{
	if (element->type == RENAMED_REFERENCE && element->ref != NULL)
	{
		if (*ref == NULL)
		{
			*ref = (char*)malloc(1);
		}
		*ref[0] = 0;

		JSONElement* cel = element;

		while (cel->parent != NULL && cel->parent->name != NULL || (cel->ref != NULL && cel->ref->name != NULL))
		{
			if (Strings::Length(*ref) != 0)
				Strings::FreeAndAssign(ref, Strings::Concat(".", *ref));
			if (cel->type == RENAMED_REFERENCE)
			{
				Strings::FreeAndAssign(ref, Strings::Concat(cel->ref->name, *ref));
				cel = cel->ref;
			}
			else
			{
				Strings::FreeAndAssign(ref, Strings::Concat(cel->parent->name, *ref));
				cel = cel->parent;
			}
		}
	}
}


//JSON
PointerList* JSON::GetKeys(const char* json)
{
	int64_t length = Strings::Length(json);
	PointerList* keys = new PointerList();

	int objectDepth = 0;
	int arrayDepth = 0;
	bool instring = false;
	bool key = false;

	int64_t keystart = -1;
	int64_t keyend = -1;

	for (int64_t i = 0; i < length; i++)
	{
		switch (json[i])
		{
		case '{':
		{
			objectDepth += 1;
			key = true;
			break;
		}
		case '}':
		{
			objectDepth -= 1;
			break;
		}
		case '[':
		{
			arrayDepth += 1;
			break;
		}
		case ']':
		{
			arrayDepth -= 1;
			break;
		}
		case ',':
		{
			if (objectDepth == 1 && arrayDepth == 0)
				key = true;
			break;
		}
		case ':':
		{
			key = false;
			break;
		}
		case '\'':
		case '"':
		{
			if (i > 0 && json[i - 1] != '\\')
				instring = !instring;
			break;
		}

		}

		if (objectDepth == 0)
			break;

		if (objectDepth > 1 || arrayDepth > 0)
		{
			continue;
		}

		if (instring && key && keystart == -1)
		{
			keystart = i + 1;
			continue;
		}

		if (!instring && key && keystart != -1)
		{
			keyend = i;

			int64_t newlength = keyend - keystart;

			char* newkey = (char*)malloc(newlength + 1);
			newkey[newlength] = 0;

			for (int x = 0; x < newlength; x++)
				newkey[x] = json[keystart + x];

			keys->AddPointer(newkey);

			keystart = -1;
			keyend = -1;
		}

	}

	return keys;
}

char* JSON::GetValue(const char* property, const char* json)
{
	char* output = NULL;
	int64_t length = Strings::Length(json);

	int objectDepth = 0;
	int arrayDepth = 0;
	bool instring = false;
	bool key = false;

	int64_t start = -1;
	int64_t end = -1;

	bool getnextvalue = false;

	for (int64_t i = 0; i < length; i++)
	{
		switch (json[i])
		{
		case '{':
		{
			objectDepth += 1;
			key = true;
			break;
		}
		case '}':
		{
			objectDepth -= 1;
			break;
		}
		case '[':
		{
			arrayDepth += 1;
			break;
		}
		case ']':
		{
			arrayDepth -= 1;
			break;
		}
		case ',':
		{
			if (objectDepth == 1 && arrayDepth == 0)
				key = true;
			break;
		}
		case ':':
		{
			key = false;
			break;
		}
		case '\'':
		case '"':
		{
			if (i > 0 && json[i - 1] != '\\')
				instring = !instring;
			break;
		}

		}

		if (objectDepth == 0)
			break;

		if (getnextvalue && (!key || objectDepth > 1) && !(json[i] == ' ' || json[i] == '\r' || json[i] == '\n' || json[i] == '\t' || json[i] == ':') && start == -1)
		{
			start = i;
			//continue;
		}

		//objectDepth == 1
		if ((json[i + 1] == ',' || json[i + 1] == '}') && getnextvalue && objectDepth <= 1 && arrayDepth == 0 && !(json[i] == ' ' || json[i] == '\r' || json[i] == '\n' || json[i] == '\t' || json[i] == ':') && !instring && start != -1)
		{
			end = i + 1;

			int64_t newlength = end - start;

			char* newvalue = (char*)malloc(newlength + 1);
			newvalue[newlength] = 0;

			for (int x = 0; x < newlength; x++)
				newvalue[x] = json[start + x];

			return newvalue;
		}

		if (objectDepth > 1 || arrayDepth > 0)
		{
			continue;
		}

		if (instring && key && start == -1 && !getnextvalue)
		{
			start = i + 1;
			continue;
		}

		if (!instring && key && start != -1 && !getnextvalue)
		{
			end = i;

			int64_t newlength = end - start;

			char* newkey = (char*)malloc(newlength + 1);
			newkey[newlength] = 0;

			for (int x = 0; x < newlength; x++)
				newkey[x] = json[start + x];

			getnextvalue = Strings::Compare(property, newkey);

			free(newkey);

			start = -1;
			end = -1;
		}

	}

	return output;
}

char* JSON::GetArrayItem(const char* json, uint64_t index)
{
	int64_t length = Strings::Length(json);

	int objectDepth = 0;
	int arrayDepth = 0;
	int arrayitem = 0;

	int64_t start = -1;
	int64_t end = -1;

	for (int64_t i = 0; i < length; i++)
	{
		switch (json[i])
		{
		case '{':
		{
			objectDepth += 1;
			break;
		}
		case '}':
		{
			objectDepth -= 1;
			break;
		}
		case '[':
		{
			arrayDepth += 1;
			break;
		}
		case ']':
		{
			arrayDepth -= 1;
			break;
		}
		case ',':
		{
			if (objectDepth == 0 && arrayDepth == 1)
				arrayitem += 1;
			break;
		}
		}

		if ((arrayDepth == 1 && json[i] == '['))
		{
			continue;
		}

		if (objectDepth <= 1 && arrayDepth == 1 && arrayitem == index && !(json[i] == ' ' || json[i] == '\r' || json[i] == '\n' || json[i] == '\t' || json[i] == ':' || json[i] == ',') && start == -1)
		{
			start = i;
			continue;
		}

		if (objectDepth == 0 && start != -1 && ((arrayDepth == 1 && (json[i] == ',' && arrayitem == index + 1)) || (arrayDepth == 0 || (json[i] == ']' && arrayitem == index))))
		{
			end = i;

			int64_t newlength = end - start;

			char* newvalue = (char*)malloc(newlength + 1);
			newvalue[newlength] = 0;

			for (int x = 0; x < newlength; x++)
				newvalue[x] = json[start + x];

			return newvalue;

		}
	}

	return NULL;
}

uint64_t JSON::GetArrayLength(const char* json)
{
	int64_t length = Strings::Length(json);

	int objectDepth = 0;
	int arrayDepth = 0;
	int arrayitem = -1;

	int64_t start = -1;
	int64_t end = -1;

	bool atleast1 = false;

	for (int64_t i = 0; i < length; i++)
	{
		switch (json[i])
		{
		case '{':
		{
			objectDepth += 1;
			break;
		}
		case '}':
		{
			objectDepth -= 1;
			break;
		}
		case '[':
		{
			arrayDepth += 1;
			break;
		}
		case ']':
		{
			arrayDepth -= 1;
			break;
		}
		case ',':
		{
			if (objectDepth == 0 && arrayDepth == 1)
				arrayitem += 1;
			break;
		}
		}

		if (arrayDepth == 1 && objectDepth == 0 && arrayitem == -1 && !(json[i] == ' ' || json[i] == '\r' || json[i] == '\n' || json[i] == '\t' || json[i] == '[' || json[i] == ']'))
		{
			arrayitem++;
		}
	}

	return ++arrayitem;
}

void JSON::DynamicArrayParser(uint64_t* outputCount, void** outputArray, char* json, uint64_t datatypesize, void (*DynamicConstructor)(void* mylocation, char* json))
{
	*outputCount = 0;
	*outputArray = NULL;
	if (json != NULL)
	{
		*outputCount = JSON::GetArrayLength(json);
		*outputArray = malloc(*outputCount * datatypesize);

		if (*outputArray != NULL)
		{
			for (int i = 0; i < *outputCount; i++)
			{
				char* item = JSON::GetArrayItem(json, i);
				if (item != NULL)
				{
					void* location = ((unsigned char*)*outputArray) + (i * datatypesize);
					DynamicConstructor(location, item);
					free(item);
				}
			}
		}
	}
}

void JSON::Uint64_tFromJSON(void* mylocation, char* value)
{
	uint64_t* me = (uint64_t*)mylocation;

	*me = Strings::StringToUint(value);
}

void JSON::FloatFromJSON(void* mylocation, char* value)
{
	float* me = (float*)mylocation;
	sscanf_s(value, "%f", me);
}

char* JSON::ResolveValue(const char* json, const char* variableName) // Not Very Efficient ... needs a rework
{
	char* result = NULL;
	int index = Strings::IndexOf(variableName, ".", 1);
	if (index != -1)
	{
		char* name = Strings::Substring(variableName, 0, index);
		char* value = JSON::GetValue(name, json);
		char* following = Strings::Substring(variableName, index + 1, Strings::Length(variableName) - index);
		if (value != NULL)
			result = ResolveValue(value, following);
		free(following);
		free(value);
		free(name);
	}
	else
	{
		result = JSON::GetValue(variableName, json);
	}

	if (result != NULL)
	{
		int length = Strings::Length(result);

		if (result[0] == '\"' && result[length - 1] == '\"')
		{
			Strings::FreeAndAssign(&result, Strings::Substring(result, 1, length - 2));
		}
	}

	return result;
}

JSONElement* JSON::CreateJSONElementTree(const char* json)
{
	//Maby Later
	return NULL;
}