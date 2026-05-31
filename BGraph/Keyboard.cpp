#include "Keyboard.h"  


bool Keyboard::IsSet(int key)
{
	for (int i = 0; i < this->keyCount; i++)
	{
		if (this->keys[i].key == key)
		{
			return this->keys[i].set;
		}
	}
	return false;
}

DBuffer* Keyboard::GetSetKeys()
{
	DBuffer* keys = new DBuffer();

	for (int i = 0; i < this->keyCount; i++)
	{
		if (this->keys[i].set)
			keys->Add(this->keys[i].key);
	}

	return keys;
}

void Keyboard::SetKey(int key)
{
	for (int i = 0; i < this->keyCount; i++)
	{
		if (this->keys[i].key == key)
		{
			this->keys[i].set = true;
			return;
		}
	}

	for (int i = 0; i < this->keyCount; i++)
	{
		if (this->keys[i].key == -1)
		{
			this->keys[i].key = key;
			this->keys[i].set = true;
			return;
		}
	}
}

void Keyboard::ResetKey(int key)
{
	for (int i = 0; i < this->keyCount; i++)
	{
		if (this->keys[i].key == key)
		{
			this->keys[i].set = false;
			return;
		}
	}
}

unsigned char Keyboard::TranslateKeyboardToHTMLTextChar(unsigned char value)
{
	int offsetlowercase = 0x1A;
	int offsetnumbers = 0x1A * 2;
	int spbase = (0x1A * 2) + 0xA;
	int offsetsp0 = spbase;
	int offsetsp1 = spbase + 0x10;
	int offsetsp2 = spbase + 0x10 + 0x7;
	int offsetsp3 = spbase + 0x10 + 0x7 + 0x6;

	if (value >= 0x41 && value <= 0x5A)
	{
		return (value - 0x41);
	}

	if (value >= 0x61 && value <= 0x7A)
	{
		return (value - 0x61) + offsetlowercase;
	}

	if (value >= 0x30 && value <= 0x39)
	{
		return (value - 0x30) + offsetnumbers;
	}

	if (value >= 0x20 && value <= 0x2F)
	{
		return (value - 0x20) + offsetsp0;
	}

	if (value >= 0x3A && value <= 0x40)
	{
		return (value - 0x3A) + offsetsp1;
	}

	if (value >= 0x5B && value <= 0x60)
	{
		return (value - 0x5B) + offsetsp2;
	}

	if (value >= 0x7B && value <= 0x7F)
	{
		return (value - 0x7B) + offsetsp3;
	}
}

unsigned char Keyboard::TranslateHTMLTextCharToKeyboard(unsigned char value)
{
	int offsetlowercase = 0x1A;
	int offsetnumbers = 0x1A * 2;
	int spbase = (0x1A * 2) + 0xA;
	int offsetsp0 = spbase;
	int offsetsp1 = spbase + 0x10;
	int offsetsp2 = spbase + 0x10 + 0x7;
	int offsetsp3 = spbase + 0x10 + 0x7 + 0x6;

	if (value >= 0 && value < 0x1A)
	{
		return (value + 0x41);
	}

	if (value >= 0x1A && value < 0x34)
	{
		return (value + 0x61) - offsetlowercase;
	}

	if (value >= 0x34 && value < 0x3E)
	{
		return (value + 0x30) - offsetnumbers;
	}

	if (value >= 0x3E && value < 0x4E)
	{
		return (value + 0x20) - offsetsp0;
	}

	if (value >= 0x4E && value < 0x55)
	{
		return (value + 0x3A) - offsetsp1;
	}

	if (value >= 0x55 && value < 0x5B)
	{
		return (value + 0x5B) - offsetsp2;
	}

	if (value >= 0x5B && value <= 0x60)
	{
		return (value + 0x7B) - offsetsp3;
	}
}