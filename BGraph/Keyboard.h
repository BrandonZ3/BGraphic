#pragma once 

class KeyboardKey
{
public:
	int key = -1;
	bool set = false;
};

class Keyboard
{
	UINT keyCount = 200;
	KeyboardKey keys[200];

public:
	bool IsSet(int key);

	DBuffer* GetSetKeys();

	void SetKey(int key);

	void ResetKey(int key);

	static unsigned char TranslateKeyboardToHTMLTextChar(unsigned char value);
	static unsigned char TranslateHTMLTextCharToKeyboard(unsigned char value);
};
