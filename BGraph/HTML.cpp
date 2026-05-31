#include "HTML.h"  


HTMLElement::HTMLElement(HTMLElementType type)
{
	this->type = type;
}

void HTMLElement::CopyFlatDetailsToElement(HTMLElement* element)
{
	for (int i = 0; i < attributes->count; i++)
	{
		KeyPointerPair* pair = (KeyPointerPair*)attributes->items[i];

		element->attributes->AddPointer(new KeyPointerPair(Strings::Clone((char*)pair->key), Strings::Clone((char*)pair->pointer)));
	}
	element->parent = parent;
	element->type = type;
	if (tag != NULL)
		element->tag = Strings::Clone(tag);

	element->bGr = bGr;
	element->bGg = bGg;
	element->bGb = bGb;
	element->bGa = bGa;

	element->cr = cr;
	element->cg = cg;
	element->cb = cb;
	element->ca = ca;

	element->blr = blr;
	element->blg = blg;
	element->blb = blb;
	element->bla = bla;

	element->btr = btr;
	element->btg = btg;
	element->btb = btb;
	element->bta = bta;

	element->brr = brr;
	element->brg = brg;
	element->brb = brb;
	element->bra = bra;

	element->bbr = bbr;
	element->bbg = bbg;
	element->bbb = bbb;
	element->bba = bba;

	element->radiusTopLeft = radiusTopLeft;
	element->radiusTopRight = radiusTopRight;
	element->radiusBottomLeft = radiusBottomLeft;
	element->radiusBottomRight = radiusBottomRight;

	element->display = display;
	element->justifyContent = justifyContent;
	element->alignItems = alignItems;
	element->flexRow = flexRow;
	element->flexWrap = flexWrap;
	element->flexGrow = flexGrow;
	element->flexShrink = flexShrink;
	element->flexBasis = flexBasis;
	element->flexBasisType = flexBasisType;
	element->boxSizing = boxSizing;
	element->gap = gap;

	element->minWidth = minWidth;
	element->minWidthScaling = minWidthScaling;
	element->maxWidth = maxWidth;
	element->maxWidthScaling = maxWidthScaling;
	element->width = width;
	element->widthScaling = widthScaling;

	element->minHeight = minHeight;
	element->minHeightScaling = minHeightScaling;
	element->maxHeight = maxHeight;
	element->maxHeightScaling = maxHeightScaling;
	element->height = height;
	element->heightScaling = heightScaling;

	element->bTl = bTl;
	element->bTt = bTt;
	element->bTr = bTr;
	element->bTb = bTb;

	element->pl = pl;
	element->pt = pt;
	element->pr = pr;
	element->pb = pb;

	element->ml = ml;
	element->mt = mt;
	element->mr = mr;
	element->mb = mb;

	element->zIndex = zIndex;
	element->position = position;
	element->leftSet = leftSet;
	element->left = left;
	element->topSet = topSet;
	element->top = top;
	element->rightSet = rightSet;
	element->right = right;
	element->bottomSet = bottomSet;
	element->bottom = bottom;
	element->x = x;
	element->y = y;
	element->actualWidth = actualWidth;
	element->actualHeight = actualHeight;

	if (imageLocation != NULL)
		element->imageLocation = Strings::Clone(imageLocation);

	element->textAlign = textAlign;
	element->font = font;
	element->fontsize = fontsize;

	if (text != NULL)
		element->text = Strings::Clone(text);
	element->textwrap = textwrap;

	element->inputType = inputType;

	element->overflowHandling = overflowHandling;
	/*element->overflowX = overflowX;
	element->overflowY = overflowY;
	element->scrollPosX = scrollPosX;
	element->scrollPosY = scrollPosY;
	element->scrollBarThickness = scrollBarThickness;
	element->scrollSpacing = scrollSpacing;
	element->scrollBarScaleX = 1;
	element->scrollBarScaleY = 1;
	element->scrollBarShiftScaleX = 1;
	element->scrollBarShiftScaleY = 1;
	element->scrollXSelected = false;
	element->scrollYSelected = false;
	element->scrollStartX = 0;
	element->scrollStartY = 0;*/

}

int HTMLElement::GetChildIndex(HTMLElement* child)
{
	for (int i = 0; i < this->children.size(); i++)
	{
		if (this->children.at(i) == child)
		{
			return i;
		}
	}
	return -1;
}

HTMLElement::~HTMLElement()
{

	if (this->imageLocation != NULL)
		free(this->imageLocation);

	for (HTMLElement* element : this->conditionals)
	{
		delete element;
	}

	for (HTMLElement* element : this->generators)
	{
		delete element;
	}

	for (int i = 0; i < attributes->count; i++)
	{
		KeyPointerPair* kpp = (KeyPointerPair*)attributes->items[i];
		free(kpp->key);
		free(kpp->pointer);
		delete kpp;
	}

	//attributes->FreeEverything(); //Frees pointerlist internals and free's kpp's
	delete attributes; //calls deconstructor and pointerlist structure pointer

	if (this->text != NULL)
	{
		free(this->text);
	}

	for (HTMLElement* element : this->children)
	{
		delete element;
	}

	if (this->tag != NULL)
	{
		free(this->tag);
	}
}

//Widths

float HTMLElement::GetElementMinContentWidth(HTMLElement* element)
{
	switch (element->minWidthScaling)
	{
	case(PERCENTAGE):
	{
		return  GetElementContentWidth(element->parent) * (element->minWidth / 100.0f);
	}
	case(PX):
	{
		return element->minWidth;
	}
	// Dont know how im gonna deal with fit-content
	}
}

float HTMLElement::GetElementMaxContentWidth(HTMLElement* element)
{
	switch (element->maxWidthScaling)
	{
	case(PERCENTAGE):
	{
		return  GetElementContentWidth(element->parent) * (element->maxWidth / 100.0f);
	}
	case(PX):
	{
		return element->maxWidth;
	}
	// Dont know how im gonna deal with fit-content
	}
}

float HTMLElement::GetElementExtraWidth(HTMLElement* element)
{
	return (element->pl + element->pr + element->bTl + element->bTr + element->ml + element->mr);
}

float HTMLElement::GetElementInternalExtraWidth(HTMLElement* element)
{
	return (element->pl + element->pr + element->bTl + element->bTr);
}

float HTMLElement::GetElementExternalExtraWidth(HTMLElement* element)
{
	return (element->ml + element->mr);
}

float HTMLElement::GetInternalBoxSizedWidth(float widthPX, HTMLElement* element)
{
	float ExtraInternal = GetElementInternalExtraWidth(element);

	if (element->boxSizing == CONTENT_BOX)
		return widthPX + ExtraInternal;
	else
		if (widthPX < ExtraInternal)
			return ExtraInternal;
		else
			return widthPX;
}

float HTMLElement::GetElementOverallWidth(HTMLElement* element)
{
	return element->actualWidth + (element->ml + element->mr);
}

float HTMLElement::GetElementContentWidth(HTMLElement* element)
{
	return element->actualWidth - (element->pl + element->pr + element->bTl + element->bTr);
}
//

//Heights

float HTMLElement::GetElementMinContentHeight(HTMLElement* element)
{
	switch (element->minHeightScaling)
	{
	case(PERCENTAGE):
	{
		return  GetElementContentHeight(element->parent) * (element->minHeight / 100.0f);
	}
	case(PX):
	{
		return element->minHeight;
	}
	// Dont know how im gonna deal with m
	}
}

float HTMLElement::GetElementMaxContentHeight(HTMLElement* element)
{
	switch (element->maxHeightScaling)
	{
	case(PERCENTAGE):
	{
		return  GetElementContentHeight(element->parent) * (element->maxHeight / 100.0f);
	}
	case(PX):
	{
		return element->maxHeight;
	}
	// Dont know how im gonna deal with fit-content
	}
}

float HTMLElement::GetElementExtraHeight(HTMLElement* element)
{
	return (element->pt + element->pb + element->bTt + element->bTb + element->mt + element->mb);
}

float HTMLElement::GetElementInternalExtraHeight(HTMLElement* element)
{
	return (element->pt + element->pb + element->bTt + element->bTb);
}

float HTMLElement::GetElementExternalExtraHeight(HTMLElement* element)
{
	return (element->mt + element->mb);
}

float HTMLElement::GetInternalBoxSizedHeight(float heightPX, HTMLElement* element)
{
	float ExtraInternal = GetElementInternalExtraHeight(element);

	if (element->boxSizing == CONTENT_BOX)
		return heightPX + ExtraInternal;
	else
		if (heightPX < ExtraInternal)
			return ExtraInternal;
		else
			return heightPX;
}

float HTMLElement::GetElementOverallHeight(HTMLElement* element)
{
	return element->actualHeight + (element->mt + element->mb);
}

float HTMLElement::GetElementContentHeight(HTMLElement* element)
{
	return element->actualHeight - (element->pt + element->pb + element->bTt + element->bTb);
}
//

//Flex Stretching
float HTMLElement::GetChildFlexContentWidth(HTMLElement* chld)
{
	float chldContentBox = 0;
	if (chld->flexBasisType != HTML_AUTO)
	{

		switch (chld->flexBasisType)
		{
		case(PERCENTAGE):
		{
			chldContentBox = GetInternalBoxSizedWidth(GetElementContentWidth(chld->parent) * (chld->flexBasis / 100.0f), chld) - GetElementInternalExtraWidth(chld);
			break;
		}
		case(PX):
		{
			chldContentBox = GetInternalBoxSizedWidth(chld->flexBasis, chld) - GetElementInternalExtraWidth(chld);
			break;
		}
		case(FIT_CONTENT):
		{
			chldContentBox = GetElementContentWidth(chld);
			break;
		}
		}
	}
	else
	{
		switch (chld->widthScaling)
		{
		case(PERCENTAGE):
		{
			chldContentBox = GetInternalBoxSizedWidth(GetElementContentWidth(chld->parent) * (chld->width / 100.0f), chld) - GetElementInternalExtraWidth(chld);
			break;
		}
		case(PX):
		{
			chldContentBox = GetInternalBoxSizedWidth(chld->width, chld) - GetElementInternalExtraWidth(chld);
			break;
		}
		case(FIT_CONTENT):
		{
			chldContentBox = GetElementContentWidth(chld);
			break;
		}
		}
	}
	return chldContentBox;
}

float HTMLElement::GetChildFlexContentHeight(HTMLElement* chld)
{
	float chldContentBox = 0;
	if (chld->flexBasisType != HTML_AUTO)
	{

		switch (chld->flexBasisType)
		{
		case(PERCENTAGE):
		{
			chldContentBox = GetInternalBoxSizedHeight(GetElementContentHeight(chld->parent) * (chld->flexBasis / 100.0f), chld) - GetElementInternalExtraHeight(chld);
			break;
		}
		case(PX):
		{
			chldContentBox = GetInternalBoxSizedHeight(chld->flexBasis, chld) - GetElementInternalExtraHeight(chld);
			break;
		}
		case(FIT_CONTENT):
		{
			chldContentBox = GetElementContentHeight(chld);
			break;
		}
		}
	}
	else
	{
		switch (chld->heightScaling)
		{
		case(PERCENTAGE):
		{
			chldContentBox = GetInternalBoxSizedHeight(GetElementContentHeight(chld->parent) * (chld->height / 100.0f), chld) - GetElementInternalExtraHeight(chld);
			break;
		}
		case(PX):
		{
			chldContentBox = GetInternalBoxSizedHeight(chld->height, chld) - GetElementInternalExtraHeight(chld);
			break;
		}
		case(FIT_CONTENT):
		{
			chldContentBox = GetElementContentHeight(chld);
			break;
		}
		}
	}
	return chldContentBox;
}

void HTMLElement::HTMLCalculateFlexStretchingWidth(HTMLElement* element)
{
	float totalWidth = 0;
	int shrinkers = 0;
	int growers = 0;
	bool childrenDirty = false;

	for (int i = 0; i < element->children.size(); i++)
	{
		HTMLElement* chld = element->children.at(i);

		if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
		{
			if (chld->flexGrow)
				growers++;
			if (chld->flexShrink)
				shrinkers++;

			if (chld->flexBasisType != HTML_AUTO)
			{
				switch (chld->flexBasisType)
				{
				case(PERCENTAGE):
				{
					float newWidth = GetInternalBoxSizedWidth(GetElementContentWidth(element) * (chld->flexBasis / 100.0f), chld);
					chld->actualWidth = newWidth;
					totalWidth += chld->actualWidth + GetElementExternalExtraWidth(chld);
					chld->dirtyWidth = false;
					break;
				}
				case(PX):
				{
					float newWidth = GetInternalBoxSizedWidth(chld->flexBasis, chld);
					chld->actualWidth = newWidth;
					totalWidth += chld->actualWidth + GetElementExternalExtraWidth(chld);
					chld->dirtyWidth = false;
					break;
				}
				case(FIT_CONTENT):
				{
					if (chld->dirtyWidth)
					{
						childrenDirty = true;
						break;
					}
					totalWidth += chld->actualWidth + GetElementExternalExtraWidth(chld);
					break;
				}
				}
			}
			else
			{
				switch (chld->widthScaling)
				{
				case(PERCENTAGE):
				{
					totalWidth += GetInternalBoxSizedWidth(GetElementContentWidth(element) * (chld->width / 100.0f), chld) + GetElementExternalExtraWidth(chld);
					break;
				}
				case(PX):
				{
					totalWidth += GetInternalBoxSizedWidth(chld->width, chld) + GetElementExternalExtraWidth(chld);
					break;
				}
				case(FIT_CONTENT):
				{
					if (chld->dirtyWidth)
					{
						childrenDirty = true;
						break;
					}
					totalWidth += (chld->actualWidth + GetElementExternalExtraWidth(chld));
					break;
				}
				}
			}
			if (i != 0)
				totalWidth += element->gap;
		}
	}

	if (!childrenDirty)
	{
		element->flexStretchDirty = false;
		float elementContentWidth = GetElementContentWidth(element);
		float toDisperse = elementContentWidth - totalWidth;
		float remaining = 0;
		float splitvalue = 0;

		if (toDisperse > 0 && growers > 0)
			splitvalue = toDisperse / growers;
		else if (toDisperse < 0 && shrinkers > 0)
			splitvalue = toDisperse / shrinkers;

		HTMLElement::HTMLInvalidateChildrenPositions(element);
		element->flexDirtyWidth = true;
		element->flexDirtyHeight = true;

		for (int i = 0; i < element->children.size(); i++)
		{
			HTMLElement* chld = element->children.at(i);

			if (sqrt(pow(toDisperse, 2)) > 0.001)
			{
				HTMLElement::HTMLInvalidateDependantChildrenWidth(chld);
			}

			if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
			{
				if (toDisperse > 0 && growers > 0)
				{

					if (chld->flexGrow)
					{
						float chldContentBox = GetChildFlexContentWidth(chld); //This should be content only unless its border-box
						float chldInternalExtra = GetElementInternalExtraWidth(chld);
						if (chld->boxSizing == HTMLBoxSizing::BORDER_BOX)
						{
							chldContentBox += chldInternalExtra;
						}
						float chldMaxContentWidth = GetElementMaxContentWidth(chld);
						if (chldContentBox + splitvalue > chldMaxContentWidth)
						{
							growers--;
							float diff = chldMaxContentWidth - chldContentBox;
							chld->actualWidth = GetInternalBoxSizedWidth(chldContentBox + diff, chld);
							remaining += splitvalue - diff;
						}
						else
						{
							chld->actualWidth = GetInternalBoxSizedWidth(chldContentBox + splitvalue, chld);
						}
					}
				}
				else if (toDisperse < 0 && shrinkers > 0)
				{
					if (chld->flexShrink)
					{

						float chldContentBox = GetChildFlexContentWidth(chld);
						float chldInternalExtra = GetElementInternalExtraWidth(chld);
						if (chld->boxSizing == HTMLBoxSizing::BORDER_BOX)
						{
							chldContentBox += chldInternalExtra;
						}
						float chldMinContentWidth = GetElementMinContentWidth(chld);
						if (chldContentBox + splitvalue < chldMinContentWidth)
						{
							shrinkers--;
							float diff = chldMinContentWidth - chldContentBox;
							chld->actualWidth = GetInternalBoxSizedWidth(chldContentBox + diff, chld);
							remaining += splitvalue - diff;
						}
						else
						{
							chld->actualWidth = GetInternalBoxSizedWidth(chldContentBox + splitvalue, chld);
						}
					}
				}
			}
		}

		while (remaining != 0 && sqrt(pow(remaining, 2)) > 0.001)
			HTMLDistributeRemainingFlexWidthSpacing(element, &remaining, &growers, &shrinkers);
	}
}

void HTMLElement::HTMLDistributeRemainingFlexWidthSpacing(HTMLElement* element, float* remaining, int* growers, int* shrinkers)
{
	float remainingSplit = 0;
	if (*remaining > 0 && *growers > 0)
		remainingSplit = *remaining / *growers;
	else if (*remaining < 0 && *shrinkers > 0)
		remainingSplit = *remaining / *shrinkers;

	for (int i = 0; i < element->children.size(); i++)
	{
		HTMLElement* chld = element->children.at(i);
		if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
		{
			if (*remaining > 0 && *growers > 0)
			{
				if (chld->flexGrow)
				{
					float chldContentBox = GetElementContentWidth(chld);
					float chldInternalExtra = GetElementInternalExtraWidth(chld);
					if (chld->boxSizing == HTMLBoxSizing::BORDER_BOX)
					{
						chldContentBox += chldInternalExtra;
					}
					float chldMaxContentWidth = GetElementMaxContentWidth(chld);

					if (chldContentBox == chldMaxContentWidth)
					{
						continue;
					}

					if (chldContentBox + remainingSplit > chldMaxContentWidth)
					{
						*growers--;
						float diff = chldMaxContentWidth - chldContentBox;
						chld->actualWidth = GetInternalBoxSizedWidth(chldContentBox + diff, chld);
						*remaining -= remainingSplit - diff;
					}
					else
					{
						chld->actualWidth = GetInternalBoxSizedWidth(chldContentBox + remainingSplit, chld);
						*remaining -= remainingSplit;
					}
				}
			}
			else if (*remaining < 0 && *shrinkers > 0)
			{
				if (chld->flexShrink)
				{
					float chldContentBox = GetElementContentWidth(chld);
					float chldInternalExtra = GetElementInternalExtraWidth(chld);
					if (chld->boxSizing == HTMLBoxSizing::BORDER_BOX)
					{
						chldContentBox += chldInternalExtra;
					}
					float chldMinContentWidth = GetElementMinContentWidth(chld);

					if (chldContentBox == chldMinContentWidth)
					{
						continue;
					}

					if (chldContentBox + remainingSplit < chldMinContentWidth)
					{
						*shrinkers--;
						float diff = chldMinContentWidth - chldContentBox;
						chld->actualWidth = GetInternalBoxSizedWidth(chldContentBox + diff, chld);
						*remaining -= remainingSplit - diff;
					}
					else
					{
						chld->actualWidth = GetInternalBoxSizedWidth(chldContentBox + remainingSplit, chld);
						*remaining -= remainingSplit;
					}
				}
			}
		}
	}
}

void HTMLElement::HTMLCalculateFlexStretchingHeight(HTMLElement* element)
{
	float totalHeight = 0;
	int shrinkers = 0;
	int growers = 0;
	bool childrenDirty = false;

	for (int i = 0; i < element->children.size(); i++)
	{
		HTMLElement* chld = element->children.at(i);
		if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
		{
			if (chld->flexGrow)
				growers++;
			if (chld->flexShrink)
				shrinkers++;

			if (chld->flexBasisType != HTML_AUTO)
			{

				switch (chld->flexBasisType)
				{
				case(PERCENTAGE):
				{
					float newHeight = GetInternalBoxSizedHeight(GetElementContentHeight(element) * (chld->flexBasis / 100.0f), chld);
					chld->actualHeight = newHeight;
					totalHeight += chld->actualHeight + GetElementExternalExtraHeight(chld);
					chld->dirtyHeight = false;
					break;
				}
				case(PX):
				{
					float newHeight = GetInternalBoxSizedHeight(chld->flexBasis, chld);
					chld->actualHeight = newHeight;
					totalHeight += chld->actualHeight + GetElementExternalExtraHeight(chld);
					chld->dirtyHeight = false;
					break;
				}
				case(FIT_CONTENT):
				{
					if (chld->dirtyHeight)
					{
						childrenDirty = true;
						break;
					}
					totalHeight += chld->actualHeight + GetElementExternalExtraHeight(chld);
					break;
				}
				}
			}
			else
			{
				switch (chld->heightScaling)
				{
				case(PERCENTAGE):
				{
					totalHeight += GetInternalBoxSizedHeight(GetElementContentHeight(element) * (chld->height / 100.0f), chld) + GetElementExternalExtraHeight(chld);
					break;
				}
				case(PX):
				{
					totalHeight += GetInternalBoxSizedHeight(chld->height, chld) + GetElementExternalExtraHeight(chld);
					break;
				}
				case(FIT_CONTENT):
				{
					if (chld->dirtyHeight)
					{
						childrenDirty = true;
						break;
					}
					totalHeight += (chld->actualHeight + GetElementExternalExtraHeight(chld));
					break;
				}
				}
			}
			if (i != 0)
				totalHeight += element->gap;
		}
	}

	if (!childrenDirty)
	{
		element->flexStretchDirty = false;
		float elementContentHeight = GetElementContentHeight(element);
		float toDisperse = elementContentHeight - totalHeight;
		float remaining = 0;
		float splitvalue = 0;

		if (toDisperse > 0 && growers > 0)
			splitvalue = toDisperse / growers;
		else if (toDisperse < 0 && shrinkers > 0)
			splitvalue = toDisperse / shrinkers;

		HTMLElement::HTMLInvalidateChildrenPositions(element);
		element->flexDirtyWidth = true;
		element->flexDirtyHeight = true;

		for (int i = 0; i < element->children.size(); i++)
		{
			HTMLElement* chld = element->children.at(i);

			if (sqrt(pow(toDisperse, 2)) > 0.001)
			{
				HTMLElement::HTMLInvalidateDependantChildrenHeight(chld);
			}

			if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
			{
				if (toDisperse > 0 && growers > 0)
				{
					if (chld->flexGrow)
					{
						float chldContentBox = GetChildFlexContentHeight(chld);
						float chldInternalExtra = GetElementInternalExtraHeight(chld);
						if (chld->boxSizing == HTMLBoxSizing::BORDER_BOX)
						{
							chldContentBox += chldInternalExtra;
						}
						float chldMaxContentHeight = GetElementMaxContentHeight(chld);
						if (chldContentBox + splitvalue > chldMaxContentHeight)
						{
							growers--;
							float diff = chldMaxContentHeight - chldContentBox;
							chld->actualHeight = GetInternalBoxSizedHeight(chldContentBox + diff, chld);
							remaining += splitvalue - diff;
						}
						else
						{
							chld->actualHeight = GetInternalBoxSizedHeight(chldContentBox + splitvalue, chld);
						}
					}
				}
				else if (toDisperse < 0 && shrinkers > 0)
				{
					if (chld->flexShrink)
					{

						float chldContentBox = GetChildFlexContentHeight(chld);
						float chldInternalExtra = GetElementInternalExtraHeight(chld);
						if (chld->boxSizing == HTMLBoxSizing::BORDER_BOX)
						{
							chldContentBox += chldInternalExtra;
						}
						float chldMinContentHeight = GetElementMinContentHeight(chld);
						if (chldContentBox + splitvalue < chldMinContentHeight)
						{
							shrinkers--;
							float diff = chldMinContentHeight - chldContentBox;
							chld->actualHeight = GetInternalBoxSizedHeight(chldContentBox + diff, chld);
							remaining += splitvalue - diff;
						}
						else
						{
							chld->actualHeight = GetInternalBoxSizedHeight(chldContentBox + splitvalue, chld);
						}
					}
				}
			}
		}

		while (remaining != 0 && sqrt(pow(remaining, 2)) > 0.001)
			HTMLDistributeRemainingFlexHeightSpacing(element, &remaining, &growers, &shrinkers);
	}
}

void HTMLElement::HTMLDistributeRemainingFlexHeightSpacing(HTMLElement* element, float* remaining, int* growers, int* shrinkers)
{
	float remainingSplit = 0;
	if (*remaining > 0 && *growers > 0)
		remainingSplit = *remaining / *growers;
	else if (*remaining < 0 && *shrinkers > 0)
		remainingSplit = *remaining / *shrinkers;

	for (int i = 0; i < element->children.size(); i++)
	{
		HTMLElement* chld = element->children.at(i);
		if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
		{
			if (*remaining > 0 && *growers > 0)
			{
				if (chld->flexGrow)
				{
					float chldContentBox = GetElementContentHeight(chld);
					float chldInternalExtra = GetElementInternalExtraHeight(chld);
					if (chld->boxSizing == HTMLBoxSizing::BORDER_BOX)
					{
						chldContentBox += chldInternalExtra;
					}
					float chldMaxContentHeight = GetElementMaxContentHeight(chld);

					if (chldContentBox == chldMaxContentHeight)
					{
						continue;
					}

					if (chldContentBox + remainingSplit > chldMaxContentHeight)
					{
						*growers--;
						float diff = chldMaxContentHeight - chldContentBox;
						chld->actualHeight = GetInternalBoxSizedHeight(chldContentBox + diff, chld);
						*remaining -= remainingSplit - diff;
					}
					else
					{
						chld->actualHeight = GetInternalBoxSizedHeight(chldContentBox + remainingSplit, chld);
						*remaining -= remainingSplit;
					}
				}
			}
			else if (*remaining < 0 && *shrinkers > 0)
			{
				if (chld->flexShrink)
				{
					float chldContentBox = GetElementContentHeight(chld);
					float chldInternalExtra = GetElementInternalExtraHeight(chld);
					if (chld->boxSizing == HTMLBoxSizing::BORDER_BOX)
					{
						chldContentBox += chldInternalExtra;
					}
					float chldMinContentHeight = GetElementMinContentHeight(chld);

					if (chldContentBox == chldMinContentHeight)
					{
						continue;
					}

					if (chldContentBox + remainingSplit < chldMinContentHeight)
					{
						*shrinkers--;
						float diff = chldMinContentHeight - chldContentBox;
						chld->actualHeight = GetInternalBoxSizedHeight(chldContentBox + diff, chld);
						*remaining -= remainingSplit - diff;
					}
					else
					{
						chld->actualHeight = GetInternalBoxSizedHeight(chldContentBox + remainingSplit, chld);
						*remaining -= remainingSplit;
					}
				}
			}
		}
	}
}
//

void HTMLElement::HTMLCalculateSizesAndPositionsTD(HTMLElement* element)
{
	//Handle body root element;
	if (element->parent == NULL && (element->dirtyWidth || element->dirtyHeight || element->dirtyPositionX || element->dirtyPositionY))
	{
		float ExtraWidthX = element->pl + element->pr + element->bTl + element->bTr;
		float ExtraHeightY = element->pt + element->pb + element->bTt + element->bTb;
		element->actualWidth = element->width + ExtraWidthX;
		element->actualHeight = element->height + ExtraHeightY;
		element->x = 0 + element->ml;
		element->y = 0 + element->mt;
		element->dirtyWidth = false;
		element->dirtyHeight = false;
		element->dirtyPositionX = false;
		element->dirtyPositionY = false;
	}

	//min-width and min-height has to do with flex grid, % sizing and fit-content
	//flex stuff
	if (element->display == FLEX && element->flexRow && !element->dirtyWidth && !element->widthScaling != FIT_CONTENT && element->flexStretchDirty)
	{
		HTMLElement::HTMLCalculateFlexStretchingWidth(element);
	}
	else if (element->display == FLEX && !element->flexRow && !element->dirtyHeight && !element->heightScaling != FIT_CONTENT && element->flexStretchDirty)
	{
		HTMLElement::HTMLCalculateFlexStretchingHeight(element);
	}

	//Calculate children sizing and positions
	for (int i = 0; i < element->children.size(); i++)
	{
		HTMLElement* current = element->children.at(i);
		float eX = current->pl + current->pr + current->bTl + current->bTr;
		float eY = current->pt + current->pb + current->bTt + current->bTb;

		//Calculate height of child
		if (current->dirtyHeight)
		{
			switch (current->heightScaling)
			{
			case(PERCENTAGE):
			{
				if (current->position != HTMLElementPosition::POS_ABSOLUTE)
					current->actualHeight = ((element->actualHeight - (element->pt + element->pb + element->bTt + element->bTb)) * (current->height / 100.0f));
				else
				{
					HTMLElement* rel = HTMLElement::GetPositionReferableParent(current);
					current->actualHeight = ((rel->actualHeight - (rel->bTt + rel->bTb)) * (current->height / 100.0f));
				}
				current->dirtyHeight = false;
				break;
			}
			case(PX):
			{
				current->actualHeight = current->height;
				current->dirtyHeight = false;
				break;
			}
			case(FIT_CONTENT):
			{
				break;
			}
			}

			if (current->actualHeight < current->minHeight)
				current->actualHeight = current->minHeight;
			else if (current->actualHeight > current->maxHeight)
				current->actualHeight = current->maxHeight;

			//Add padding and border thickness. top and bottom.
			if (!current->dirtyHeight)
			{
				if (current->boxSizing == CONTENT_BOX)
					current->actualHeight += eY;
				else
					if (current->actualHeight < eY)
						current->actualHeight = eY;
			}
		}

		//Calculate position y of current element
		if (current->dirtyPositionY && current->parent != NULL && !current->parent->dirtyPositionY)
		{
			// block code. flex child also acts as block when parent is block
			if ((element->display == BLOCK && current->display == BLOCK) || (element->display == BLOCK && current->display == FLEX))
			{
				bool unresolved = false;
				float y = element->y + element->pt + element->bTt;
				float shift = 0;
				if (current->position != HTMLElementPosition::POS_ABSOLUTE) // Added Absolute positioning calculations still have to do for flex below
				{
					for (int x = 0; x < i; x++)
					{
						HTMLElement* chld = element->children.at(x);

						if (!chld->dirtyHeight)
						{
							switch (chld->display)
							{
							case(BLOCK):
							{
								if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
									y += (chld->actualHeight) + chld->mt + chld->mb;
								break;
							}
							case(FLEX):
							{
								if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
									y += (chld->actualHeight) + chld->mt + chld->mb;
								break;
							}
							}
						}
						else
						{
							unresolved = true;
							break;
						}
					}
					shift = current->mt;
				}
				else
				{
					HTMLElement* rel = HTMLElement::GetPositionReferableParent(current);
					if (current->topSet)
					{
						y = rel->y + rel->bTt + current->top;
						shift = current->mt;
					}
					else if (current->bottomSet)
					{
						if (rel->dirtyHeight) //Could cause issue(inf loop)
							unresolved = true;
						else
						{
							y = rel->y + rel->actualHeight - (current->bottom + current->actualHeight + rel->bTb);
							shift = -current->mb;
						}
					}
				}

				if (!unresolved)
				{
					current->y = y + shift;
					current->dirtyPositionY = false;
				}
			}

			//flex row
			if ((element->display == FLEX && element->flexRow))
			{
				if (current->position != HTMLElementPosition::POS_ABSOLUTE) // Added Absolute positioning calculations still have to do for flex below
				{
					float y = element->y + element->pt + element->bTt;
					current->y = y + current->mt;
					current->dirtyPositionY = false;
				}
				else
				{
					HTMLElement* rel = HTMLElement::GetPositionReferableParent(current);
					bool unresolved = false;
					float y = 0;
					float shift = 0;
					if (current->topSet)
					{
						y = rel->y + rel->bTt + current->top;
						shift = current->mt;
					}
					else if (current->bottomSet)
					{
						if (rel->dirtyHeight) //Could cause issue(inf loop)
							unresolved = true;
						else
						{
							y = rel->y + rel->actualHeight - (current->bottom + current->actualHeight + rel->bTb);
							shift = -current->mb;
						}
					}

					if (!unresolved)
					{
						current->y = y + shift;
						current->dirtyPositionY = false;
					}
				}
			}
			//flex column
			else if ((element->display == FLEX && !element->flexRow))
			{
				bool unresolved = false;
				float y = element->y + element->pt + element->bTt;
				float shift = 0;
				if (current->position != HTMLElementPosition::POS_ABSOLUTE) // Added Absolute positioning calculations still have to do for flex below
				{
					for (int x = 0; x < i; x++)
					{
						HTMLElement* chld = element->children.at(x);

						if (!chld->dirtyHeight)
						{
							if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
							{
								y += (chld->actualHeight) + chld->mt + chld->mb;
								if (element->gap > 0)
									y += element->gap;
							}
						}
						else
						{
							unresolved = true;
							break;
						}
					}
					shift = current->mt;
				}
				else
				{
					HTMLElement* rel = HTMLElement::GetPositionReferableParent(current);
					//y = rel->y + rel->pt + rel->bTt + current->top;
					if (current->topSet)
					{
						y = rel->y + rel->bTt + current->top;
						shift = current->mt;
					}
					else if (current->bottomSet)
					{
						if (rel->dirtyHeight) //Could cause issue(inf loop)
							unresolved = true;
						else
						{
							y = rel->y + rel->actualHeight - (current->bottom + current->actualHeight + rel->bTb);
							shift = -current->mb;
						}
					}
				}

				if (!unresolved)
				{
					current->y = y + shift;
					current->dirtyPositionY = false;
				}
			}

			//if (current->position == POS_RELATIVE && !current->dirtyPositionY)
			//{
			//	/*float parentLeft = element->x + element->pl + element->bTl;
			//	float parentTop = element->y + element->pt + element->bTt;
			//	float parentRight = element->x + element->pl + element->bTl + element->actualWidth;
			//	float parentBottom = element->y + element->pt + element->bTt + element->actualHeight;*/

			//	if (current->topSet)
			//	{
			//		current->y += current->top;
			//		HTMLShiftChildrenPositions(element, 0, element->top, 0, 0);
			//	}

			//	if (!current->topSet && current->bottomSet)
			//	{
			//		current->y -= current->bottom;
			//		HTMLShiftChildrenPositions(element, 0, 0, 0, element->bottom);
			//	}
			//}
		}

		//Calculate width of child
		if (current->dirtyWidth)
		{
			switch (current->widthScaling)
			{
			case(PERCENTAGE):
			{
				if (current->position != HTMLElementPosition::POS_ABSOLUTE)
					current->actualWidth = ((element->actualWidth - (element->pl + element->pr + element->bTl + element->bTr)) * (current->width / 100.0f));
				else
				{
					HTMLElement* rel = HTMLElement::GetPositionReferableParent(current);
					current->actualWidth = ((rel->actualWidth - (rel->bTl + rel->bTr)) * (current->width / 100.0f));
				}
				current->dirtyWidth = false;
				break;
			}
			case(PX):
			{
				current->actualWidth = current->width;
				current->dirtyWidth = false;
				break;
			}
			case(FIT_CONTENT):
			{
				break;
			}
			}

			if (current->actualWidth < current->minWidth)
				current->actualWidth = current->minWidth;
			else if (current->actualWidth > current->maxWidth)
				current->actualWidth = current->maxWidth;

			//Add padding and border thickness. left and right.
			if (!current->dirtyWidth)
			{
				if (current->boxSizing == CONTENT_BOX)
					current->actualWidth += eX;
				else
					if (current->actualWidth < eX)
						current->actualWidth = eX;
			}
		}

		//Calculate position x of current element
		if (current->dirtyPositionX && current->parent != NULL && !current->parent->dirtyPositionX)
		{
			if (KeyPointerPair::GetKeyValuePointer(current->attributes, "break") != NULL)
			{
				int z = 0;
			}
			// block code. flex child also acts as block when parent is block
			if ((element->display == BLOCK && current->display == BLOCK) || (element->display == BLOCK && current->display == FLEX))
			{
				if (current->position != HTMLElementPosition::POS_ABSOLUTE) // need flex cases
				{
					float x = element->x + element->pl + element->bTl;
					current->x = x + current->ml;
					current->dirtyPositionX = false;
				}
				else
				{
					bool unresolved = false;
					HTMLElement* rel = HTMLElement::GetPositionReferableParent(current);
					float x = 0;
					float shift = 0;
					if (current->leftSet)
					{
						x = rel->x + rel->bTl + current->left;
						shift = current->ml;
					}
					else if (current->rightSet)
					{
						if (rel->dirtyWidth) //Could cause issue(inf loop)
							unresolved = true;
						else
						{
							x = rel->x + rel->actualWidth - (current->right + current->actualWidth + rel->bTr);
							shift = -current->mr;
						}
					}

					if (!unresolved)
					{
						current->x = x + shift;
						current->dirtyPositionX = false;
					}
				}
			}

			//flex row
			if ((element->display == FLEX && element->flexRow))
			{
				bool unresolved = false;
				float x = element->x + element->pl + element->bTl;
				float shift = 0;
				if (current->position != HTMLElementPosition::POS_ABSOLUTE)
				{
					for (int y = 0; y < i; y++)
					{
						HTMLElement* chld = element->children.at(y);

						if (!chld->dirtyWidth)
						{
							if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
							{
								x += (chld->actualWidth) + chld->ml + chld->mr;
								if (element->gap > 0)
									x += element->gap;
							}
						}
						else
						{
							unresolved = true;
							break;
						}
					}
					shift = current->ml;
				}
				else
				{
					HTMLElement* rel = HTMLElement::GetPositionReferableParent(current);
					//x = rel->x + rel->pl + rel->bTl + current->left;

					if (current->leftSet)
					{
						x = rel->x + rel->bTl + current->left;
						shift = current->ml;
					}
					else if (current->rightSet)
					{
						if (rel->dirtyWidth) //Could cause issue(inf loop)
							unresolved = true;
						else
						{
							x = rel->x + rel->actualWidth - (current->right + current->actualWidth + rel->bTr);
							shift = -current->mr;
						}
					}
				}

				if (!unresolved)
				{
					current->x = x + shift;
					current->dirtyPositionX = false;
				}
			}
			//flex column
			else if ((element->display == FLEX /*&& current->display == BLOCK*/ && !element->flexRow))
			{
				if (current->position != HTMLElementPosition::POS_ABSOLUTE) // need flex cases
				{
					float x = element->x + element->pl + element->bTl;
					current->x = x + current->ml;
					current->dirtyPositionX = false;
				}
				else
				{
					bool unresolved = false;
					HTMLElement* rel = HTMLElement::GetPositionReferableParent(current);
					float x = 0;
					float shift = 0;
					if (current->leftSet)
					{
						x = rel->x + rel->bTl + current->left;
						shift = current->ml;
					}
					else if (current->rightSet)
					{
						if (rel->dirtyWidth) //Could cause issue(inf loop)
							unresolved = true;
						else
						{
							x = rel->x + rel->actualWidth - (current->right + current->actualWidth + rel->bTr);
							shift = -current->mr;
						}
					}

					if (!unresolved)
					{
						current->x = x + shift;
						current->dirtyPositionX = false;
					}
				}
			}

			//if (current->position == POS_RELATIVE && !current->dirtyPositionX)
			//{
			//	/*float parentLeft = element->x + element->pl + element->bTl;
			//	float parentTop = element->y + element->pt + element->bTt;
			//	float parentRight = element->x + element->pl + element->bTl + element->actualWidth;
			//	float parentBottom = element->y + element->pt + element->bTt + element->actualHeight;*/
			//	if (current->leftSet)
			//	{
			//		current->x += current->left;
			//		HTMLShiftChildrenPositions(element, element->left, 0, 0, 0);
			//	}

			//	if (!current->leftSet && current->rightSet)
			//	{
			//		current->x -= current->right;
			//		HTMLShiftChildrenPositions(element, 0, 0, element->right, 0);
			//	}
			//}
		}

		HTMLElement::HTMLCalculateSizesAndPositionsTD(current);
	}
}

void HTMLElement::HTMLCalculateFlexWrap(HTMLElement* element)
{
	if (element->display == FLEX)
	{
		bool childrenStillDirty = false;

		for (int i = 0; i < element->children.size(); i++)
		{
			HTMLElement* current = element->children.at(i);

			if (current->dirtyWidth || current->dirtyHeight)
			{
				childrenStillDirty = true;
			}
		}

		if (!childrenStillDirty)
			if (element->flexWrap && element->flexRow && element->widthScaling != FIT_CONTENT)
			{
				float greatestBottom = 0;
				float rTop = element->y + element->pt + element->bTt;
				float pRight = element->x + element->actualWidth - (element->pr + element->bTr);
				float pLeft = element->x + element->pl + element->bTl;
				for (int i = 0; i < element->children.size(); i++)
				{
					HTMLElement* chld = element->children.at(i);
					if (chld->position != POS_ABSOLUTE)
					{
						float xLeft = chld->x - chld->ml;
						float width = HTMLElement::GetElementOverallWidth(chld);
						float xRight = xLeft + width;

						float yBottom = (chld->y - chld->mt) + HTMLElement::GetElementOverallHeight(chld) + element->gap;

						if (yBottom > greatestBottom)
							greatestBottom = yBottom;


						if (xRight > pRight)
						{
							float shiftX = xLeft - pLeft;
							float shiftY = greatestBottom - rTop;
							rTop = greatestBottom;
							for (int j = i; j < element->children.size(); j++)
							{
								HTMLElement* shChld = element->children.at(j);
								shChld->x -= shiftX;
								shChld->y += shiftY;
								HTMLElement::HTMLInvalidateChildrenPositions(shChld);
							}
						}
					}
				}
			}
			else if (element->flexWrap && !element->flexRow && element->heightScaling != FIT_CONTENT)
			{

			}
	}

	for (int i = 0; i < element->children.size(); i++)
	{
		HTMLElement::HTMLCalculateFlexWrap(element->children.at(i));
	}
}

void HTMLElement::HTMLCalculateSizesAndPositionsBU(HTMLElement* element, JSONElement* variables)
{
	bool childrenStillDirty = false;
	//Calculate sizing and position of children first. This code goes to the deepest in the order of children and starts there

	//bool isBreak = false;

	for (int i = 0; i < element->children.size(); i++)
	{
		HTMLElement* current = element->children.at(i);
		HTMLElement::HTMLCalculateSizesAndPositionsBU(current, variables);

		/*if (!isBreak)
			isBreak = KeyPointerPair::GetKeyValuePointer(current->attributes, "break") != NULL;*/

		if (current->dirtyWidth || current->dirtyHeight || current->dirtyPositionX || current->dirtyPositionY)
		{
			childrenStillDirty = true;
		}
	}


	if (element->children.size() == 0 && (element->dirtyWidth || element->dirtyHeight))
	{
		int z = 0;
	}

	if (!childrenStillDirty)
	{
		//Handling Current Element

		//Calculating farthest right and bottom of current element
		float right = 0;
		float left = element->x;
		float top = element->y;
		float bottom = 0;
		float eX = (element->pl + element->pr) + (element->bTl + element->bTr);
		float eY = (element->pt + element->pb) + (element->bTt + element->bTb);

		for (int i = 0; i < element->children.size(); i++)
		{
			HTMLElement* chld = element->children.at(i);

			if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
			{
				float nx = chld->x;
				float ny = chld->y;

				float chldRight = nx + (chld->actualWidth) + chld->mr;
				float chldBottom = ny + (chld->actualHeight) + chld->mb;

				if (chldRight > right)
					right = chldRight;
				if (chldBottom > bottom)
					bottom = chldBottom;
			}

		}

		//Calculating sizing and positioning


		if ((element->text != NULL || element->type == HTMLElementType::INP) && element->textDirty && !element->dirtyPositionX && !element->dirtyPositionY /*&& ((element->parent->display == FLEX && !element->parent->flexDirtyWidth && !element->parent->flexDirtyHeight) || element->parent->display != FLEX)*/) ////Fuuuuu
		{
			HTMLHandleTextCharacters(element, variables);
			element->textDirty = false;
			element->dirtyWidth = true;
			element->dirtyHeight = true;

			if (element->parent->heightScaling == FIT_CONTENT)
				element->parent->dirtyHeight = true;
			if (element->parent->widthScaling == FIT_CONTENT)
				element->parent->dirtyWidth = true;
		}

		//Calculate height
		if (element->dirtyHeight)
		{
			switch (element->heightScaling)
			{
			case(FIT_CONTENT):
			{
				if (element->text != NULL)
				{
					if (!element->dirtyPositionY && !element->textDirty)// && element->characters.size() > 0)
					{
						int originalY = element->y + element->pt + element->bTt;
						int lasty = originalY;

						for (int i = 0; i < element->characters.size(); i++)
						{
							if (element->characters.at(i).y + element->fontsize > lasty)
								lasty = element->characters.at(i).y + element->fontsize;
						}
						element->actualHeight = (lasty - originalY);// element->characters.at(0).y); 
						element->dirtyHeight = false;
					}

					/*if (element->characters.size() == 0)
					{
						element->dirtyHeight = false;
					}*/
				}
				else
				{
					element->actualHeight = (bottom - top) - (element->bTt + element->pt); //minus bTt and pt so that the height is only content
					element->dirtyHeight = false;
				}
				break;
			}
			}

			//Add padding and border thickness. top and bottom.
			/*if (!element->dirtyHeight)
			{
				element->actualHeight += eY;
			}*/

			if (element->actualHeight < element->minHeight)
				element->actualHeight = element->minHeight;
			else if (element->actualHeight > element->maxHeight)
				element->actualHeight = element->maxHeight;

			//Add padding and border thickness. left and right.
			if (!element->dirtyHeight)
			{
				if (element->boxSizing == CONTENT_BOX)
					element->actualHeight += eY;
				else
					if (element->actualHeight < eY)
						element->actualHeight = eY;
			}
		}

		//Calculate position y of current element
		if (element->dirtyPositionY && element->parent != NULL && !element->parent->dirtyPositionY)
		{
			// block code. flex child also acts as block when parent is block
			if ((element->parent->display == BLOCK && element->display == BLOCK) || (element->parent->display == BLOCK && element->display == FLEX))
			{
				bool unresolved = false;
				float y = (element->parent->y + element->parent->pt + element->parent->bTt);
				int myIndex = element->parent->GetChildIndex(element);
				float shift = 0;
				if (element->position != HTMLElementPosition::POS_ABSOLUTE)
				{
					for (int x = 0; x < myIndex; x++)
					{
						HTMLElement* chld = element->parent->children.at(x);

						if (!chld->dirtyHeight)
						{
							if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
								y += (chld->actualHeight) + chld->mt + chld->mb;
						}
						else
						{
							unresolved = true;
							break;
						}
					}
					shift = element->mt;
				}
				else
				{
					HTMLElement* rel = HTMLElement::GetPositionReferableParent(element);
					//y = rel->y + rel->pt + rel->bTt + element->top;
					if (element->topSet)
					{
						y = rel->y + rel->bTt + element->top;
						shift = element->mt;
					}
					else if (element->bottomSet)
					{
						if (rel->dirtyHeight) //Could cause issue(inf loop)
							unresolved = true;
						else
						{
							shift = -element->mb;
							y = rel->y + rel->actualHeight - (element->bottom + element->actualHeight + rel->bTb);
						}
					}
				}

				if (!unresolved)
				{
					element->y = y + shift;
					element->dirtyPositionY = false;
				}
			}


			//if (element->position == POS_RELATIVE && !element->dirtyPositionX)
			//{
			//	/*float parentLeft = element->x + element->pl + element->bTl;
			//	float parentTop = element->y + element->pt + element->bTt;
			//	float parentRight = element->x + element->pl + element->bTl + element->actualWidth;
			//	float parentBottom = element->y + element->pt + element->bTt + element->actualHeight;*/

			//	if (element->topSet)
			//	{
			//		element->y += element->top;
			//		HTMLShiftChildrenPositions(element, 0, 0, element->top, 0);
			//	}

			//	if (!element->topSet && element->bottomSet)
			//	{
			//		element->y -= element->bottom;
			//		HTMLShiftChildrenPositions(element, 0, 0, 0, element->bottom);
			//	}
			//}
		}

		//Calculate width
		if (element->dirtyWidth)
		{
			switch (element->widthScaling)
			{
			case(FIT_CONTENT):
			{
				if (element->text != NULL)
				{
					if (!element->dirtyPositionX && !element->textDirty)
					{
						int lastx = element->x + element->pl + element->bTl;

						if (element->textwrap)
						{
							HTMLElement* ancestor = NULL;
							HTMLElement* curanc = element;

							float accumulatedPadding = 0;
							float accumulatedBorder = 0;

							if (curanc->widthScaling != FIT_CONTENT)
							{
								while (ancestor == NULL)
								{
									if (curanc->parent == NULL)
									{
										ancestor = curanc;
										break;
									}

									if (curanc->widthScaling == PERCENTAGE || curanc->widthScaling == PX)
										ancestor = curanc;
									else
									{
										accumulatedPadding += (curanc->pl + curanc->pr);
										accumulatedBorder += (curanc->bTl + curanc->bTr);
										curanc = curanc->parent;
									}
								}
								element->actualWidth = ancestor->actualWidth - (ancestor->pl + ancestor->pr + ancestor->bTl + ancestor->bTr + accumulatedPadding + accumulatedBorder);
							}
							else
							{
								float start = element->x + element->pl + element->bTl;
								float end = 0;

								for (int i = 0; i < element->characters.size(); i++)
								{
									HTMLTextCharacter character = element->characters.at(i);
									float charend = character.x + element->fontsize;
									if (charend > end)
										end = charend;
								}

								element->actualWidth = (end - start);// +GetElementInternalExtraWidth(element);
							}
						}
						else
						{
							for (int i = 0; i < element->characters.size(); i++)
							{
								if (element->characters.at(i).x > lastx)
									lastx = element->characters.at(i).x + element->fontsize;
							}
							element->actualWidth = (lastx - element->characters.at(0).x);
						}
						element->dirtyWidth = false;
					}
				}
				else
				{
					element->actualWidth = (right - left) - (element->bTl + element->pl);
					element->dirtyWidth = false;
				}
				break;
			}
			}

			//Add padding and border thickness. left and right.
			/*if (!element->dirtyWidth)
			{
				element->actualWidth += eX;
			}*/

			if (element->actualWidth < element->minWidth)
				element->actualWidth = element->minWidth;
			else if (element->actualWidth > element->maxWidth)
				element->actualWidth = element->maxWidth;

			//Add padding and border thickness. left and right.
			if (!element->dirtyWidth)
			{
				if (element->boxSizing == CONTENT_BOX)
					element->actualWidth += eX;
				else
					if (element->actualWidth < eX)
						element->actualWidth = eX;
			}
		}

		//Calculate position x of current element
		if (element->dirtyPositionX && element->parent != NULL && !element->parent->dirtyPositionX)
		{
			// block code. flex child also acts as block when parent is block
			if ((element->parent->display == BLOCK && element->display == BLOCK) || (element->parent->display == BLOCK && element->display == FLEX))
			{
				element->dirtyPositionX = false;
			}

			//if (element->position == POS_RELATIVE && !element->dirtyPositionX)
			//{
			//	/*float parentLeft = element->x + element->pl + element->bTl;
			//	float parentTop = element->y + element->pt + element->bTt;
			//	float parentRight = element->x + element->pl + element->bTl + element->actualWidth;
			//	float parentBottom = element->y + element->pt + element->bTt + element->actualHeight;*/
			//	if (element->leftSet)
			//	{
			//		element->x += element->left;
			//		HTMLShiftChildrenPositions(element, element->left, 0, 0, 0);
			//	}

			//	if (!element->leftSet && element->rightSet)
			//	{
			//		element->x -= element->right;
			//		HTMLShiftChildrenPositions(element, 0, element->right, 0, 0);
			//	}
			//}
		}
	}
}

void HTMLElement::HTMLHandleFlexPositioning(HTMLElement* element)
{
	for (int i = 0; i < element->children.size(); i++)
	{
		HTMLElement* child = element->children.at(i);
		if (child->position != HTMLElementPosition::POS_ABSOLUTE)
			if (child->dirtyHeight || child->dirtyWidth || child->dirtyPositionX || child->dirtyPositionY)
				return;
	}

	if (element->display == FLEX && element->children.size() > 0 && (element->flexDirtyWidth || element->flexDirtyHeight))
	{
		bool largerContentWidth = false;
		bool largerContentHeight = false;
		float currentChildHeight = 0;
		float currentChildWidth = 0;

		for (int i = 0; i < element->children.size(); i++)
		{
			if (element->flexRow)
			{
				currentChildWidth += HTMLElement::GetElementOverallWidth(element->children.at(i));//not confirmed to work yet
				if (i != 0)
					currentChildWidth += element->gap;
				float height = HTMLElement::GetElementOverallHeight(element->children.at(i));
				if (height > currentChildHeight)
					currentChildHeight = height;
			}
			else
			{
				currentChildHeight += HTMLElement::GetElementOverallHeight(element->children.at(i));//not confirmed to work yet
				if (i != 0)
					currentChildHeight += element->gap;
				float width = HTMLElement::GetElementOverallWidth(element->children.at(i));
				if (width > currentChildWidth)
					currentChildWidth = width;
			}
		}

		if (HTMLElement::GetElementContentWidth(element) > currentChildWidth)
			largerContentWidth = true;
		if (HTMLElement::GetElementContentHeight(element) > currentChildHeight)
			largerContentHeight = true;

		if (element->flexRow)
		{
			if (!element->dirtyHeight && element->flexDirtyHeight)
			{
				switch (element->alignItems)
				{
				case(FLEX_AUTO):
				{
					element->flexDirtyHeight = false;
					break;
				}
				case(START):
				{
					element->flexDirtyHeight = false;
					break;
				}
				case(CENTER):
				{
					if (element->heightScaling == FIT_CONTENT && !largerContentHeight)
					{
						float largestYCenter = 0;
						for (int i = 0; i < element->children.size(); i++)
						{
							HTMLElement* chld = element->children.at(i);
							if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
							{
								float center = chld->y + (chld->actualHeight / 2.0f);

								if (center > largestYCenter)
									largestYCenter = center;
							}
						}

						for (int i = 0; i < element->children.size(); i++)
						{
							HTMLElement* chld = element->children.at(i);

							if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
							{
								float center = chld->y + (chld->actualHeight / 2.0f);
								float diff = largestYCenter - center;

								chld->y += diff;
							}
						}
						element->flexDirtyHeight = false;
					}
					else
					{
						float elementCenterPos = element->y + (element->actualHeight / 2);
						if (!element->flexWrap)
						{
							for (int i = 0; i < element->children.size(); i++)
							{
								HTMLElement* chld = element->children.at(i);
								if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
								{
									float center = chld->y + (chld->actualHeight / 2.0f);
									float diff = elementCenterPos - center;

									chld->y += diff;
								}
							}
						}
						else
						{
							//***********************************************Stuff being done here might have to be align-content instead************************************************************************
							float childStartY = element->y + element->pt + element->bTt;
							float childEndY = childStartY;
							for (int i = element->children.size() - 1; i >= 0; i--)
							{
								HTMLElement* chld = element->children.at(i);
								if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
								{
									childEndY = (chld->y - chld->mt) + HTMLElement::GetElementOverallHeight(chld);
									break;
								}
							}

							float childTotalHeight = childEndY - childStartY;
							float currentCenter = childStartY + (childTotalHeight / 2.0f);
							float diff = elementCenterPos - currentCenter;

							for (int i = 0; i < element->children.size(); i++)
							{
								HTMLElement* chld = element->children.at(i);
								if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
								{
									chld->y += diff;
								}
							}

						}
						element->flexDirtyHeight = false;
					}
					break;
				}
				case(END):
				{
					if (element->heightScaling == FIT_CONTENT && !largerContentHeight)
					{
						float largestYEnd = 0;
						for (int i = 0; i < element->children.size(); i++)
						{
							HTMLElement* chld = element->children.at(i);
							if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
							{
								float end = chld->y + (chld->actualHeight) + chld->mb;

								if (end > largestYEnd)
									largestYEnd = end;
							}
						}

						for (int i = 0; i < element->children.size(); i++)
						{
							HTMLElement* chld = element->children.at(i);

							if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
							{
								float end = chld->y + (chld->actualHeight) + chld->mb;
								float diff = largestYEnd - end;

								chld->y += diff;
							}
						}
						element->flexDirtyHeight = false;
					}
					else
					{
						if (!element->flexWrap)
						{
							for (int i = 0; i < element->children.size(); i++)
							{
								HTMLElement* chld = element->children.at(i);
								if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
								{
									float ref = element->y + (element->actualHeight - (element->pb + element->bTb));
									float diff = ref - (chld->y + chld->actualHeight + chld->mb);

									chld->y += diff;
								}
							}
						}
						else
						{
							//***********************************************Stuff being done here might have to be align-content instead************************************************************************
							float contentEnd = element->y + (element->actualHeight - (element->pb + element->bTb));
							float childEndY = 0;
							for (int i = element->children.size() - 1; i >= 0; i--)
							{
								HTMLElement* chld = element->children.at(i);
								if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
								{
									childEndY = (chld->y - chld->mt) + HTMLElement::GetElementOverallHeight(chld);
									break;
								}
							}

							float diff = contentEnd - childEndY;

							for (int i = 0; i < element->children.size(); i++)
							{
								HTMLElement* chld = element->children.at(i);
								if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
								{
									chld->y += diff;
								}
							}

						}
						element->flexDirtyHeight = false;
					}
					break;
				}
				}
			}

			if (!element->dirtyWidth && element->flexDirtyWidth)
			{
				switch (element->justifyContent)
				{
				case(FLEX_AUTO):
				{
					element->flexDirtyWidth = false;
					break;
				}
				case(START):
				{
					element->flexDirtyWidth = false;
					break;
				}
				case(CENTER):
				{
					if (element->widthScaling == FIT_CONTENT && !largerContentWidth)
					{
						// cant be done?
						element->flexDirtyWidth = false;
					}
					else
					{

						if (element->flexWrap && element->children.size() > 1)
						{
							float elementCenter = element->x + (element->actualWidth / 2);
							float lastY = 0;
							int startIndex = 0;
							int childrenCount = element->children.size();
							bool lastRow = false;
							for (int j = 0; j < childrenCount; j++)
							{
								HTMLElement* chldS = element->children.at(j);
								float currentY = chldS->y - chldS->mt;
								bool differentRow = currentY != lastY;
								if (differentRow || j == childrenCount - 1)
								{
									if (lastRow || (currentY == lastY && j == childrenCount - 1))
										j++;

									lastY = currentY;

									float totalWidthChildren = 0;
									for (int i = startIndex; i < j; i++)
									{
										HTMLElement* chld = element->children.at(i);
										if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
										{
											totalWidthChildren += chld->actualWidth + chld->ml + chld->mr;
											if (i != startIndex)
												totalWidthChildren += element->gap;
										}
									}

									HTMLElement* chStart = element->children.at(startIndex);
									float childrenStart = chStart->x - chStart->ml;

									float diff = (elementCenter - (childrenStart + (totalWidthChildren / 2)));

									for (int i = startIndex; i < j; i++)
									{
										HTMLElement* chld = element->children.at(i);
										if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
										{
											chld->x += diff;
										}
									}

									startIndex = j;
									if (differentRow && j == childrenCount - 1 && !lastRow)
									{
										lastRow = true;
										j--;
									}
								}
							}
						}
						else
						{
							float elementCenter = element->x + (element->actualWidth / 2);
							float totalWidthChildren = 0;
							for (int i = 0; i < element->children.size(); i++)
							{
								HTMLElement* chld = element->children.at(i);
								if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
								{
									totalWidthChildren += chld->actualWidth + chld->ml + chld->mr;
									if (i != 0)
										totalWidthChildren += element->gap;
								}
							}

							HTMLElement* chStart = element->children.at(0);
							float childrenStart = chStart->x - chStart->ml;

							float diff = (elementCenter - (childrenStart + (totalWidthChildren / 2)));

							for (int i = 0; i < element->children.size(); i++)
							{
								HTMLElement* chld = element->children.at(i);
								if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
								{
									chld->x += diff;
								}
							}
						}

						element->flexDirtyWidth = false;
					}
					break;
				}
				case(END):
				{
					if (element->widthScaling == FIT_CONTENT && !largerContentWidth)
					{
						// cant be done?
						element->flexDirtyWidth = false;
					}
					else
					{
						if (element->flexWrap && element->children.size() > 1)
						{
							float elementContentEnd = element->x + (element->actualWidth - (element->pr + element->bTr));
							float lastY = 0;
							int startIndex = 0;
							int childrenCount = element->children.size();
							bool lastRow = false;
							for (int j = 0; j < childrenCount; j++)
							{
								HTMLElement* chldS = element->children.at(j);
								float currentY = chldS->y - chldS->mt;
								bool differentRow = currentY != lastY;
								if (differentRow || j == childrenCount - 1)
								{
									float totalEndChildren = element->x + element->pl + element->bTl;

									if (lastRow || (currentY == lastY && j == childrenCount - 1))
										j++;

									lastY = currentY;

									for (int i = startIndex; i < j; i++)
									{
										HTMLElement* chld = element->children.at(i);
										if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
										{
											totalEndChildren += chld->actualWidth + chld->ml + chld->mr;
											if (i != startIndex)
												totalEndChildren += element->gap;
										}
									}

									float diff = (elementContentEnd - totalEndChildren);

									for (int i = startIndex; i < j; i++)
									{
										HTMLElement* chld = element->children.at(i);
										if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
										{
											chld->x += diff;
										}
									}

									startIndex = j;
									if (differentRow && j == childrenCount - 1 && !lastRow)
									{
										lastRow = true;
										j--;
									}
								}
							}
						}
						else
						{
							float elementContentEnd = element->x + (element->actualWidth - (element->pr + element->bTr));
							float totalEndChildren = element->x + element->pl + element->bTl;
							for (int i = 0; i < element->children.size(); i++)
							{
								HTMLElement* chld = element->children.at(i);
								if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
								{
									totalEndChildren += chld->actualWidth + chld->ml + chld->mr;
									if (i != 0)
										totalEndChildren += element->gap;
								}
							}

							float diff = (elementContentEnd - totalEndChildren);

							for (int i = 0; i < element->children.size(); i++)
							{
								HTMLElement* chld = element->children.at(i);
								if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
								{
									chld->x += diff;
								}
							}
						}
						element->flexDirtyWidth = false;
					}
					break;
				}
				case(SPACE_BETWEEN):
				{
					float elementContentWidth = element->actualWidth - (element->pl + element->pr + element->bTl + element->bTr);
					float totalWidthChildren = 0;
					for (int i = 0; i < element->children.size(); i++)
					{
						HTMLElement* chld = element->children.at(i);
						if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
						{
							totalWidthChildren += chld->actualWidth + chld->ml + chld->mr;
							if (i != 0)
								totalWidthChildren += element->gap;
						}
					}

					float diff = (elementContentWidth - totalWidthChildren);

					if (diff > 0)
					{
						float seperation = diff / (element->children.size() - 1);

						for (int i = 1; i < element->children.size(); i++)
						{
							HTMLElement* chld1 = HTMLElement::GetPreviousSpaceTakingSibling(i, element);//element->children.at(i-1);
							HTMLElement* chld2 = element->children.at(i);

							//almost forgot chld2->ml
							if (chld1->position != HTMLElementPosition::POS_ABSOLUTE && chld2->position != HTMLElementPosition::POS_ABSOLUTE)
							{
								chld2->x = chld2->ml + chld1->x + chld1->actualWidth + chld1->mr + seperation;
							}
						}
					}
					element->flexDirtyWidth = false;
					break;
				}
				//case(SPACE_AROUND): // not done yet
				//{
				//	float elementContentWidth = element->actualWidth - (element->pl + element->pr + element->bTl + element->bTr);
				//	float totalWidthChildren = 0;
				//	for (int i = 0; i < element->children.size(); i++)
				//	{
				//		HTMLElement* chld = element->children.at(i);

				//		totalWidthChildren += chld->actualWidth + chld->ml + chld->mr;
				//	}

				//	float diff = (elementContentWidth - totalWidthChildren);

				//	//if (diff > 0)
				//	//{
				//	//	float seperation = diff / (element->children.size() + 1);

				//	//	for (int i = 1; i < element->children.size(); i++)
				//	//	{
				//	//		HTMLElement* chld1 = element->children.at(i - 1);
				//	//		HTMLElement* chld2 = element->children.at(i);

				//	//		//almost forgot chld2->ml
				//	//		chld2->x = chld2->ml + chld1->x + chld1->actualWidth + chld1->mr + seperation;
				//	//	}
				//	//}
				//	break;
				//}
				}
			}
		}
		else // Column 
		{
			if (!element->dirtyHeight && element->flexDirtyHeight)
			{
				switch (element->justifyContent)
				{
				case(FLEX_AUTO):
				{
					element->flexDirtyHeight = false;
					break;
				}
				case(START):
				{
					element->flexDirtyHeight = false;
					break;
				}
				case(CENTER):
				{
					if (element->heightScaling == FIT_CONTENT && !largerContentHeight)
					{
						//cant be done?
						element->flexDirtyHeight = false;
					}
					else
					{
						float elementCenter = element->y + (element->actualHeight / 2);
						float totalHeightChildren = 0;
						for (int i = 0; i < element->children.size(); i++)
						{
							HTMLElement* chld = element->children.at(i);

							if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
							{
								totalHeightChildren += chld->actualHeight + chld->mt + chld->mb;
								if (i != 0)
									totalHeightChildren += element->gap;
							}
						}

						HTMLElement* chStart = element->children.at(0);
						float childrenStart = chStart->y - chStart->mt;

						float diff = (elementCenter - (childrenStart + (totalHeightChildren / 2)));

						for (int i = 0; i < element->children.size(); i++)
						{
							HTMLElement* chld = element->children.at(i);

							if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
							{
								chld->y += diff;
							}
						}
						element->flexDirtyHeight = false;
					}
					break;
				}
				case(END):
				{
					if (element->heightScaling == FIT_CONTENT && !largerContentHeight)
					{
						// cant be done?
						element->flexDirtyHeight = false;
					}
					else
					{
						float elementContentEnd = element->y + (element->actualHeight - (element->pb + element->bTb));
						float totalEndChildren = element->y + element->pt + element->bTt;
						for (int i = 0; i < element->children.size(); i++)
						{
							HTMLElement* chld = element->children.at(i);

							if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
							{
								totalEndChildren += chld->actualHeight + chld->mt + chld->mb;
								if (i != 0)
									totalEndChildren += element->gap;
							}
						}

						float diff = (elementContentEnd - totalEndChildren) / 2;

						for (int i = 0; i < element->children.size(); i++)
						{
							HTMLElement* chld = element->children.at(i);

							if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
							{
								chld->y += diff;
							}
						}
						element->flexDirtyHeight = false;
					}
					break;
				}
				case(SPACE_BETWEEN):
				{
					float elementContentHeight = element->actualHeight - (element->pt + element->pb + element->bTt + element->bTb);
					float totalHeightChildren = 0;
					for (int i = 0; i < element->children.size(); i++)
					{
						HTMLElement* chld = element->children.at(i);

						if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
						{
							totalHeightChildren += chld->actualHeight + chld->mt + chld->mb;
							if (i != 0)
								totalHeightChildren += element->gap;
						}
					}

					float diff = (elementContentHeight - totalHeightChildren);

					if (diff > 0)
					{
						float seperation = diff / (element->children.size() - 1);

						for (int i = 1; i < element->children.size(); i++)
						{
							HTMLElement* chld1 = HTMLElement::GetPreviousSpaceTakingSibling(i, element);//element->children.at(i - 1);
							HTMLElement* chld2 = element->children.at(i);

							//almost forgot chld2->ml
							if (chld1->position != HTMLElementPosition::POS_ABSOLUTE && chld2->position != HTMLElementPosition::POS_ABSOLUTE)
							{
								chld2->y = chld2->mt + chld1->y + chld1->actualHeight + chld1->mb + seperation;
							}
						}
					}
					element->flexDirtyHeight = false;
					break;
				}
				//case(SPACE_AROUND): // not done yet
				//{
				//}
				}
			}

			if (!element->dirtyWidth && element->flexDirtyWidth)
			{
				switch (element->alignItems)
				{
				case(FLEX_AUTO):
				{
					element->flexDirtyWidth = false;
					break;
				}
				case(START):
				{
					element->flexDirtyWidth = false;
					break;
				}
				case(CENTER):
				{
					if (element->widthScaling == FIT_CONTENT && !largerContentWidth)
					{
						float largestXCenter = 0;
						for (int i = 0; i < element->children.size(); i++)
						{
							HTMLElement* chld = element->children.at(i);
							if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
							{
								float center = chld->x + (chld->actualWidth / 2.0f);

								if (center > largestXCenter)
									largestXCenter = center;
							}
						}

						for (int i = 0; i < element->children.size(); i++)
						{
							HTMLElement* chld = element->children.at(i);
							if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
							{
								float center = chld->x + (chld->actualWidth / 2.0f);
								float diff = largestXCenter - center;

								chld->x += diff;
							}
						}
						element->flexDirtyWidth = false;
					}
					else
					{
						float elementCenterPos = element->x + (element->actualWidth / 2);
						for (int i = 0; i < element->children.size(); i++)
						{
							HTMLElement* chld = element->children.at(i);
							if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
							{
								float center = chld->x + (chld->actualWidth / 2.0f);
								float diff = elementCenterPos - center;

								chld->x += diff;
							}
						}
						element->flexDirtyWidth = false;
					}
					break;
				}
				case(END):
				{
					if (element->widthScaling == FIT_CONTENT && !largerContentWidth)
					{
						float largestXEnd = 0;
						for (int i = 0; i < element->children.size(); i++)
						{
							HTMLElement* chld = element->children.at(i);
							if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
							{
								float end = chld->x + (chld->actualWidth) + chld->mr;

								if (end > largestXEnd)
									largestXEnd = end;
							}
						}

						for (int i = 0; i < element->children.size(); i++)
						{
							HTMLElement* chld = element->children.at(i);
							if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
							{
								float end = chld->x + (chld->actualWidth) + chld->mr;
								float diff = largestXEnd - end;

								chld->x += diff;
							}
						}
						element->flexDirtyWidth = false;
					}
					else
					{
						for (int i = 0; i < element->children.size(); i++)
						{
							HTMLElement* chld = element->children.at(i);
							if (chld->position != HTMLElementPosition::POS_ABSOLUTE)
							{
								float ref = element->x + (element->actualWidth - (element->pr + element->bTr));
								float diff = ref - (chld->x + chld->actualWidth + chld->mb);

								chld->x += diff;
							}
						}
						element->flexDirtyWidth = false;
					}
					break;
				}
				}
			}
		}

		for (int i = 0; i < element->children.size(); i++)
		{
			HTMLElement* chld = element->children.at(i);
			if (chld->text != NULL)
				chld->textDirty = true;
			HTMLInvalidateChildrenPositions(chld);
		}
		return;
	}
	else if (element->children.size() == 0)
	{
		element->flexDirtyWidth = false;
		element->flexDirtyHeight = false;
	}


	for (int i = 0; i < element->children.size(); i++)
		HTMLHandleFlexPositioning(element->children.at(i));
}

void HTMLElement::HTMLHandleTextCharacters(HTMLElement* element, JSONElement* variables)
{
	char* value = NULL;
	bool freeValue = false;

	if (element->text != NULL && Strings::Contains(element->text, "{{") && Strings::Contains(element->text, "}}"))
	{
		value = HTMLStringInterpolate(element->text, variables);
		if (value != NULL)
			freeValue = true;

	}
	else
	{
		value = element->text;
	}

	if (value != NULL || element->type == HTMLElementType::INP)
	{
		if (element->type != HTMLElementType::INP)
		{
			int length = Strings::Length(value);

			if (element->characters.size() == 0)
			{
				for (int i = 0; i < length; i++)
				{
					HTMLTextCharacter c;

					c.character = Keyboard::TranslateKeyboardToHTMLTextChar(value[i]);

					element->characters.push_back(c);
				}
			}
		}

		//bool allparentsresolved;

		int charactercount = element->characters.size();

		int lastx = element->x + element->pl + element->bTl;
		int lasty = element->y + element->pt + element->bTt;
		int left = lastx;

		HTMLElement* ancestor = NULL;
		HTMLElement* curanc = element;
		int maxwidth = 0;
		float accumulatedPadding = 0;
		float accumulatedBorder = 0;
		float accumulatedLeft = 0;

		if (element->textwrap && element->widthScaling != FIT_CONTENT) //I think i may have broken something else by putting !FITCONTENT, and below forloop havent confirmed yet
		{
			while (ancestor == NULL)
			{
				if (curanc->parent == NULL)
				{
					ancestor = curanc;
					break;
				}

				if (curanc->widthScaling == PERCENTAGE || curanc->widthScaling == PX)
					ancestor = curanc;
				else
				{
					accumulatedPadding += (curanc->pl + curanc->pr);
					accumulatedBorder += (curanc->bTl + curanc->bTr);
					accumulatedLeft += (curanc->pl + curanc->bTl);
					curanc = curanc->parent;
				}
			}
			lastx = ancestor->x + ancestor->pl + ancestor->bTl + accumulatedLeft;
			//lasty = ancestor->y + ancestor->pt + ancestor->bTt;
			left = lastx;
			maxwidth = ancestor->actualWidth - (ancestor->pl + ancestor->pr + ancestor->bTl + ancestor->bTr + accumulatedPadding + accumulatedBorder);
		}

		for (int i = 0; i < charactercount; i++)
		{
			if (element->textwrap && element->widthScaling != FIT_CONTENT)
			{
				element->characters.at(i).x = lastx;
				lastx += element->fontsize;

				if (lastx - left > maxwidth)
				{
					lastx = left;
					element->characters.at(i).x = lastx;
					lastx += element->fontsize;

					lasty += element->fontsize;
					element->characters.at(i).y = lasty;
				}
				else
				{
					element->characters.at(i).y = lasty;
				}
			}
			else
			{
				element->characters.at(i).x = lastx;
				lastx += element->fontsize;
				element->characters.at(i).y = lasty;
			}
		}

		if (element->textAlign != TEXT_START && charactercount != 0)
			switch (element->textAlign)
			{
			case(TEXT_CENTER):
			{
				int firstofline = 0;
				float yStart = element->characters.at(0).y;
				float width = 0;
				float centerx = (maxwidth / 2) + left;
				for (int i = 0; i < charactercount; i++)
				{
					bool islast = i == charactercount - 1;
					if (width == 0)
						firstofline = i;
					width += element->fontsize;
					if (element->characters.at(i).y > yStart || islast)
					{
						if (islast)
						{
							float offsetcenter = (width / 2) + left;
							float diff = centerx - offsetcenter;
							for (int x = firstofline; x < i + 1; x++)
							{
								element->characters.at(x).x += diff;
							}
						}
						else
						{
							width -= element->fontsize;
							float offsetcenter = (width / 2) + left;
							float diff = centerx - offsetcenter;
							for (int x = firstofline; x < i; x++)
							{
								element->characters.at(x).x += diff;
							}
							yStart = element->characters.at(i).y;
							i--;
						}
						width = 0;
						continue;
					}
				}
				break;
			}
			case(TEXT_END):
			{
				int firstofline = 0;
				float yStart = element->characters.at(0).y;
				float width = 0;
				for (int i = 0; i < charactercount; i++)
				{
					bool islast = i == charactercount - 1;
					if (width == 0)
						firstofline = i;
					width += element->fontsize;
					if (element->characters.at(i).y > yStart || islast)
					{
						if (islast)
						{

							float diff = maxwidth - width;
							for (int x = firstofline; x < i + 1; x++)
							{
								element->characters.at(x).x += diff;
							}
						}
						else
						{
							width -= element->fontsize;
							float diff = maxwidth - width;
							for (int x = firstofline; x < i; x++)
							{
								element->characters.at(x).x += diff;
							}
							yStart = element->characters.at(i).y;
							i--;
						}
						width = 0;
						continue;
					}
				}
				break;
			}
			}

		if (freeValue)
		{
			free(value);
		}
	}
}

char* HTMLElement::HTMLStringInterpolate(const char* text, JSONElement* variables)
{
	char* value = NULL;
	char* refVar = Strings::Replace(text, "{{", "");
	Strings::FreeAndAssign(&refVar, Strings::Replace(refVar, "}}", ""));
	Strings::FreeAndAssign(&refVar, Strings::Trim(refVar));

	JSONElement* var = JSONElement::GetElement(refVar, variables, false);
	if (var != NULL && var->value != NULL)
		value = Strings::Clone(var->value);

	free(refVar);
	return value;
}

void HTMLElement::HTMLInvalidateChildrenPositions(HTMLElement* element)
{
	for (int i = 0; i < element->children.size(); i++)
	{
		HTMLElement* child = element->children.at(i);
		child->dirtyPositionX = true;
		child->dirtyPositionY = true;
		child->textDirty = true;
		HTMLInvalidateChildrenPositions(child);
	}
}

void HTMLElement::HTMLInvalidateDependantChildrenWidth(HTMLElement* element)
{
	if (element->widthScaling != FIT_CONTENT)
		for (int i = 0; i < element->children.size(); i++)
		{
			HTMLElement* chld = element->children.at(i);

			chld->dirtyWidth = true;
			chld->dirtyPositionX = true;
			chld->dirtyPositionY = true;
			chld->flexDirtyWidth = true;
			chld->flexDirtyHeight = true;
			chld->flexStretchDirty = true;
			HTMLInvalidateDependantChildrenWidth(chld);
		}
}

void HTMLElement::HTMLInvalidateDependantChildrenHeight(HTMLElement* element)
{
	if (element->heightScaling != FIT_CONTENT)
		for (int i = 0; i < element->children.size(); i++)
		{
			HTMLElement* chld = element->children.at(i);

			chld->dirtyHeight = true;
			chld->dirtyPositionX = true;
			chld->dirtyPositionY = true;
			chld->flexDirtyWidth = true;
			chld->flexDirtyHeight = true;
			chld->flexStretchDirty = true;
			HTMLInvalidateDependantChildrenHeight(chld);
		}
}

void HTMLElement::HTMLInvalidateAll(HTMLElement* element)
{
	element->dirtyHeight = true;
	element->dirtyWidth = true;
	element->dirtyPositionX = true;
	element->dirtyPositionY = true;
	element->flexDirtyHeight = true;
	element->flexDirtyWidth = true;
	element->flexStretchDirty = true;
	element->textDirty = true;
	element->scrollBarScaleXDirty = true;
	element->scrollBarScaleYDirty = true;

	for (int i = 0; i < element->children.size(); i++)
		HTMLInvalidateAll(element->children.at(i));
}

void HTMLElement::HTMLShiftChildrenPositions(HTMLElement* element, int shiftLeft, int shiftTop, int shiftRight, int shiftBottom)
{
	for (int i = 0; i < element->children.size(); i++)
	{
		HTMLElement* child = element->children.at(i);
		child->x += shiftLeft;
		child->x -= shiftRight;
		child->y += shiftTop;
		child->y -= shiftBottom;
		/*if (child->text != NULL)
		{
			for(int i = 0; i < child->characters.size(); i++)
			{
				child->characters.at(i).x += shiftLeft;
				child->characters.at(i).x -= shiftRight;
				child->characters.at(i).y += shiftTop;
				child->characters.at(i).y -= shiftBottom;
			}
		}
		HTMLShiftChildrenPositions(child, shiftLeft, shiftTop, shiftRight, shiftBottom);*/
	}
}

bool HTMLElement::HTMLAnyDirty(HTMLElement* element)
{
	if ((element->textDirty && (element->text != NULL || element->type == INPUT)) || element->dirtyWidth || element->dirtyHeight || element->dirtyPositionX || element->dirtyPositionY || (element->display == FLEX && (element->flexDirtyWidth || element->flexDirtyHeight)))
		return true;
	bool found = false;
	for (int i = 0; i < element->children.size(); i++)
		if (HTMLAnyDirty(element->children.at(i)))
			return true;
	return false;
}

void HTMLElement::HTMLPrintTree(HTMLElement* element, int depth, char** string)
{
	char* tab = Strings::CreateIndent(depth);
	Strings::FreeAndAssign(string, Strings::Concat(*string, tab));
	Strings::FreeAndAssign(string, Strings::Concat(*string, "{\r\n"));


	Strings::FreeAndAssign(string, Strings::Concat(*string, tab));
	Strings::FreeAndAssign(string, Strings::Concat(*string, "\"type\": "));

	switch (element->type)
	{
	case(BODY):
	{
		Strings::FreeAndAssign(string, Strings::Concat(*string, "\"BODY\""));
		break;
	}
	case(DIV):
	{
		Strings::FreeAndAssign(string, Strings::Concat(*string, "\"DIV\""));
		break;
	}
	case(IMG):
	{
		Strings::FreeAndAssign(string, Strings::Concat(*string, "\"IMG\""));
		break;
	}
	case(INPUT):
	{
		Strings::FreeAndAssign(string, Strings::Concat(*string, "\"INPUT\""));
		break;
	}
	case(P):
	{
		Strings::FreeAndAssign(string, Strings::Concat(*string, "\"P\""));
		break;
	}
	}

	Strings::FreeAndAssign(string, Strings::Concat(*string, ",\r\n"));


	Strings::FreeAndAssign(string, Strings::Concat(*string, tab));
	Strings::FreeAndAssign(string, Strings::Concat(*string, "\"display\": "));

	switch (element->display)
	{
	case(BLOCK):
	{
		Strings::FreeAndAssign(string, Strings::Concat(*string, "\"BLOCK\""));
		break;
	}
	case(FLEX):
	{
		Strings::FreeAndAssign(string, Strings::Concat(*string, "\"FLEX\""));
		break;
	}
	}

	Strings::FreeAndAssign(string, Strings::Concat(*string, ",\r\n"));

	if (element->display == FLEX)
	{
		char* flexDirtyWidth = Strings::UintToString((uint64_t)element->flexDirtyWidth);
		Strings::FreeAndAssign(string, Strings::Concat(*string, tab));
		Strings::FreeAndAssign(string, Strings::Concat(*string, "\"dirtyflexwidth\": "));
		Strings::FreeAndAssign(string, Strings::Concat(*string, flexDirtyWidth));
		Strings::FreeAndAssign(string, Strings::Concat(*string, ",\r\n"));
		free(flexDirtyWidth);

		char* flexDirtyHeight = Strings::UintToString((uint64_t)element->flexDirtyHeight);
		Strings::FreeAndAssign(string, Strings::Concat(*string, tab));
		Strings::FreeAndAssign(string, Strings::Concat(*string, "\"dirtyflexheight\": "));
		Strings::FreeAndAssign(string, Strings::Concat(*string, flexDirtyHeight));
		Strings::FreeAndAssign(string, Strings::Concat(*string, ",\r\n"));
		free(flexDirtyHeight);
	}

	char* x = Strings::UintToString((uint64_t)element->x);
	Strings::FreeAndAssign(string, Strings::Concat(*string, tab));
	Strings::FreeAndAssign(string, Strings::Concat(*string, "\"x\": "));
	Strings::FreeAndAssign(string, Strings::Concat(*string, x));
	Strings::FreeAndAssign(string, Strings::Concat(*string, ",\r\n"));
	free(x);

	char* y = Strings::UintToString((uint64_t)element->x);
	Strings::FreeAndAssign(string, Strings::Concat(*string, tab));
	Strings::FreeAndAssign(string, Strings::Concat(*string, "\"y\": "));
	Strings::FreeAndAssign(string, Strings::Concat(*string, y));
	Strings::FreeAndAssign(string, Strings::Concat(*string, ",\r\n"));
	free(y);

	char* width = Strings::UintToString((uint64_t)element->actualWidth);
	Strings::FreeAndAssign(string, Strings::Concat(*string, tab));
	Strings::FreeAndAssign(string, Strings::Concat(*string, "\"width\": "));
	Strings::FreeAndAssign(string, Strings::Concat(*string, width));
	Strings::FreeAndAssign(string, Strings::Concat(*string, ",\r\n"));
	free(width);

	char* height = Strings::UintToString((uint64_t)element->actualHeight);
	Strings::FreeAndAssign(string, Strings::Concat(*string, tab));
	Strings::FreeAndAssign(string, Strings::Concat(*string, "\"height\": "));
	Strings::FreeAndAssign(string, Strings::Concat(*string, height));
	Strings::FreeAndAssign(string, Strings::Concat(*string, ",\r\n"));
	free(height);

	char* dirtyPositionX = Strings::UintToString((uint64_t)element->dirtyPositionX);
	Strings::FreeAndAssign(string, Strings::Concat(*string, tab));
	Strings::FreeAndAssign(string, Strings::Concat(*string, "\"dirtyposx\": "));
	Strings::FreeAndAssign(string, Strings::Concat(*string, dirtyPositionX));
	Strings::FreeAndAssign(string, Strings::Concat(*string, ",\r\n"));
	free(dirtyPositionX);

	char* dirtyPositionY = Strings::UintToString((uint64_t)element->dirtyPositionY);
	Strings::FreeAndAssign(string, Strings::Concat(*string, tab));
	Strings::FreeAndAssign(string, Strings::Concat(*string, "\"dirtyposy\": "));
	Strings::FreeAndAssign(string, Strings::Concat(*string, dirtyPositionY));
	Strings::FreeAndAssign(string, Strings::Concat(*string, ",\r\n"));
	free(dirtyPositionY);

	char* dirtyWidth = Strings::UintToString((uint64_t)element->dirtyWidth);
	Strings::FreeAndAssign(string, Strings::Concat(*string, tab));
	Strings::FreeAndAssign(string, Strings::Concat(*string, "\"dirtywidth\": "));
	Strings::FreeAndAssign(string, Strings::Concat(*string, dirtyWidth));
	Strings::FreeAndAssign(string, Strings::Concat(*string, ",\r\n"));
	free(dirtyWidth);

	char* dirtyHeight = Strings::UintToString((uint64_t)element->dirtyHeight);
	Strings::FreeAndAssign(string, Strings::Concat(*string, tab));
	Strings::FreeAndAssign(string, Strings::Concat(*string, "\"dirtyheight\": "));
	Strings::FreeAndAssign(string, Strings::Concat(*string, dirtyHeight));
	Strings::FreeAndAssign(string, Strings::Concat(*string, ",\r\n"));
	free(dirtyHeight);

	Strings::FreeAndAssign(string, Strings::Concat(*string, tab));
	Strings::FreeAndAssign(string, Strings::Concat(*string, "\"children\": [ \r\n"));

	for (int i = 0; i < element->children.size(); i++)
	{
		HTMLPrintTree(element->children.at(i), depth + 1, string);
		if (i != element->children.size() - 1)
			Strings::FreeAndAssign(string, Strings::Concat(*string, ",\r\n"));
	}

	Strings::FreeAndAssign(string, Strings::Concat(*string, "\r\n"));
	Strings::FreeAndAssign(string, Strings::Concat(*string, tab));
	Strings::FreeAndAssign(string, Strings::Concat(*string, "]\r\n"));
	Strings::FreeAndAssign(string, Strings::Concat(*string, tab));
	Strings::FreeAndAssign(string, Strings::Concat(*string, "}"));
	free(tab);

}

void HTMLElement::ApplyScrolling(float x, float y, HTMLElement* element, HTMLElement* scrollOriginator)
{
	bool canScrollAbsolute = false;

	if (element->position == HTMLElementPosition::POS_ABSOLUTE)
	{
		HTMLElement* ref = HTMLElement::GetPositionReferableParent(element);
		canScrollAbsolute = ref == scrollOriginator || HTMLElement::IsDescendantOf(scrollOriginator, ref);
	}

	if (element->position != HTMLElementPosition::POS_ABSOLUTE || canScrollAbsolute)
	{
		element->x -= x;
		element->y -= y;

		if (element->characters.size() > 0)
		{
			for (int i = 0; i < element->characters.size(); i++)
			{
				element->characters.at(i).x -= x;
				element->characters.at(i).y -= y;
			}
		}

		for (int i = 0; i < element->children.size(); i++)
		{
			ApplyScrolling(x, y, element->children.at(i), scrollOriginator);
		}
	}
}

void HTMLElement::RecursiveApplyScrolling(HTMLElement* element, HTMLElement* scrollOriginator)
{
	if (element->scrollPosX != 0 || element->scrollPosY != 0)
	{
		for (int i = 0; i < element->children.size(); i++)
		{
			ApplyScrolling(element->scrollPosX * element->scrollBarShiftScaleX, element->scrollPosY * element->scrollBarShiftScaleY, element->children.at(i), scrollOriginator);
		}
	}

	for (int i = 0; i < element->children.size(); i++)
	{
		RecursiveApplyScrolling(element->children.at(i), scrollOriginator);
	}
}

void HTMLElement::CalculateScrollbars(HTMLElement* element)
{
	float finalChildX = 0;
	float finalChildY = 0;

	float elementScLeft = (element->x + element->bTl);
	float elementScRight = (element->x + element->actualWidth) - element->bTr;
	float elementScTop = (element->y + element->bTt);
	float elementScBottom = (element->y + element->actualHeight) - element->bTb;

	for (int i = 0; i < element->children.size(); i++)
	{
		HTMLElement* chld = element->children.at(i);

		CalculateScrollbars(chld);

		if (element->overflowHandling == HTMLOverflow::AUTO && chld->position != HTMLElementPosition::POS_ABSOLUTE)
		{
			float overallX = HTMLElement::GetElementOverallWidth(chld) + (chld->x - chld->ml);
			float overallY = HTMLElement::GetElementOverallHeight(chld) + (chld->y - chld->mt);
			if (finalChildX < overallX)
				finalChildX = overallX;
			if (finalChildY < overallY)
				finalChildY = overallY;
		}
	}

	if ((element->widthScaling != HTMLScaling::FIT_CONTENT || (element->flexBasisType != HTMLScaling::FIT_CONTENT && element->flexBasisType != HTMLScaling::HTML_AUTO)) && element->overflowHandling == HTMLOverflow::AUTO)
	{
		float widthChildren = finalChildX - (element->x + element->pl + element->bTl);
		if (widthChildren > 0)
		{
			if (element->scrollBarScaleXDirty)
			{
				float elementWidth = HTMLElement::GetElementContentWidth(element);
				element->scrollBarScaleX = elementWidth / widthChildren;
				element->scrollBarScaleXDirty = !element->scrollBarScaleXDirty;
				element->scrollBarShiftScaleX = ((widthChildren - elementWidth)) / (element->actualWidth - (((element->actualWidth - element->scrollSpacing)) * element->scrollBarScaleX));
			}
		}
	}

	if ((element->heightScaling != HTMLScaling::FIT_CONTENT || (element->flexBasisType != HTMLScaling::FIT_CONTENT && element->flexBasisType != HTMLScaling::HTML_AUTO)) && element->overflowHandling == HTMLOverflow::AUTO)
	{
		float heightChildren = finalChildY - (element->y + element->pt + element->bTt);
		if (heightChildren > 0)
		{
			if (element->scrollBarScaleYDirty)
			{
				float elementHeight = HTMLElement::GetElementContentHeight(element);
				element->scrollBarScaleY = elementHeight / heightChildren;
				element->scrollBarScaleYDirty = !element->scrollBarScaleYDirty;
				element->scrollBarShiftScaleY = ((heightChildren - elementHeight)) / (element->actualHeight - (((element->actualHeight - element->scrollSpacing)) * element->scrollBarScaleY));
			}
		}
	}
}

HTMLElement* HTMLElement::GetPositionReferableParent(HTMLElement* chld)
{
	HTMLElement* current = chld->parent;
	while (current->position != HTMLElementPosition::POS_RELATIVE && current->position != HTMLElementPosition::POS_ABSOLUTE && current->parent != NULL)
	{
		current = current->parent;
	}
	return current;
}

bool HTMLElement::IsDescendantOf(HTMLElement* ancestor, HTMLElement* chld)
{
	HTMLElement* current = chld;
	while (current->parent != NULL)
	{
		if (current->parent == ancestor)
			return true;
		else
			current = current->parent;
	}
	return false;
}

HTMLElement* HTMLElement::GetScissorParent(HTMLElement* currentDrawElement)
{
	HTMLElement* current = currentDrawElement->parent;
	while (current->overflowHandling != HTMLOverflow::AUTO && current->overflowHandling != HTMLOverflow::HIDDEN && current->parent != NULL)
	{
		current = current->parent;
	}
	return current;
}

HTMLElement* HTMLElement::GetPreviousSpaceTakingSibling(int current, HTMLElement* parent)
{
	HTMLElement* previous = NULL;
	int size = parent->children.size();
	if (current - 1 >= 0 && current < size)
	{
		for (int i = current - 1; i >= 0; i--)
		{
			HTMLElement* element = parent->children.at(i);
			if (element->position != HTMLElementPosition::POS_ABSOLUTE)
			{
				return element;
			}
		}
	}

	return previous;
}

//HTMLParser
HTMLElement* HTMLParser::Parse(const char* document)
{
	int length = Strings::Length(document);


	bool inTag = false;
	bool inClosingTag = false;
	bool inString = false;

	bool onTagName = false;
	bool onAttributeName = false;

	char* currentTag = NULL;
	int tagNameLength = 0;
	char* currentAttribute = NULL;
	int attributeNameLength = 0;
	char* currentAttributeValue = NULL;
	int attributeValueLength = 0;
	bool inP = false;
	int textLength = 0;


	HTMLElement* body = new HTMLElement(BODY);
	HTMLElement* current = body;
	for (int i = 0; i < length; i++)
	{
		bool outsideall = !inString && !inTag && !inP;

		if (outsideall && document[i] == '<' && document[i + 1] != '/')
		{
			inTag = true;
			continue;
		}

		if (inTag && currentTag == NULL && !onTagName && document[i] != ' ')
		{
			onTagName = true;
			tagNameLength++;
			continue;
		}

		if (inTag && onTagName && document[i] != ' ' && document[i] != '>')
		{
			tagNameLength++;
		}

		if (inTag && onTagName && (document[i] == ' ' || document[i] == '>'))
		{
			currentTag = (char*)malloc(tagNameLength + 1);
			currentTag[tagNameLength] = 0;
			memmove(currentTag, &document[i - tagNameLength], tagNameLength);
			tagNameLength = 0;
			onTagName = false;

			HTMLElement* newH = NULL;

			if (Strings::CompareCaseInsensitive(currentTag, "div"))
			{
				newH = new HTMLElement(DIV);
			}
			else if (Strings::CompareCaseInsensitive(currentTag, "input"))
			{
				newH = new HTMLElement(INPUT);
				newH->width = 200;
				newH->height = 20;
				newH->heightScaling = PX;
				newH->widthScaling = PX;
				newH->bGr = 255;
				newH->bGg = 255;
				newH->bGb = 255;
				newH->bGa = 255;
				newH->bTl = 1;
				newH->bTt = 1;
				newH->bTr = 1;
				newH->bTb = 1;
				newH->blr = 0;
				newH->blg = 0;
				newH->blb = 0;
				newH->bla = 255;
				newH->btr = 0;
				newH->btg = 0;
				newH->btb = 0;
				newH->bta = 255;
				newH->brr = 0;
				newH->brg = 0;
				newH->brb = 0;
				newH->bra = 255;
				newH->bbr = 0;
				newH->bbg = 0;
				newH->bbb = 0;
				newH->bba = 255;
				newH->textwrap = false;
				newH->overflowHandling = HIDDEN;
				newH->boxSizing = HTMLBoxSizing::BORDER_BOX;
			}
			else if (Strings::CompareCaseInsensitive(currentTag, "img"))
			{
				newH = new HTMLElement(IMG);
			}
			else if (Strings::CompareCaseInsensitive(currentTag, "p"))
			{
				newH = new HTMLElement(P);
			}
			else
			{
				newH = new HTMLElement(CUSTOM);
				newH->tag = Strings::Clone(currentTag);
			}

			if (newH != NULL)
			{
				newH->parent = current;
				current->children.push_back(newH);
				current = current->children.at(current->children.size() - 1);
			}

			if (document[i] == ' ')
				continue;
		}

		if (inTag && !onTagName && !onAttributeName && currentAttribute == NULL && document[i] != ' ' && document[i] != '>' && currentTag != NULL)
		{
			onAttributeName = true;
			attributeNameLength++;
			continue;
		}

		if (onAttributeName && document[i] != '=')
		{
			attributeNameLength++;
		}

		if (onAttributeName && document[i] == '=')
		{
			currentAttribute = (char*)malloc(attributeNameLength + 1);
			currentAttribute[attributeNameLength] = 0;
			memmove(currentAttribute, &document[i - attributeNameLength], attributeNameLength);
			attributeNameLength = 0;
			onAttributeName = false;
			continue;
		}

		if (!onAttributeName && currentAttribute != NULL && document[i - 1] == '=' && document[i] == '\"')
		{
			inString = true;
			continue;
		}

		if (inString && document[i] != '\"')
		{
			attributeValueLength++;
		}

		if (inString && document[i] == '\"' && document[i - 1] != '\\')
		{
			char* currentAttributeValue = (char*)malloc(attributeValueLength + 1);
			currentAttributeValue[attributeValueLength] = 0;
			memmove(currentAttributeValue, &document[i - attributeValueLength], attributeValueLength);
			attributeValueLength = 0;
			inString = false;


			current->attributes->AddPointer(new KeyPointerPair(currentAttribute, currentAttributeValue));

			if (Strings::CompareCaseInsensitive(currentAttribute, "style"))
			{
				ParseStyling(currentAttributeValue, current);
			}

			if (Strings::CompareCaseInsensitive(currentAttribute, "src") && current->type == IMG)
			{
				PointerList* src = PointerList::SplitString(currentAttributeValue, ";");

				if (src->count > 0)
				{
					if (!Strings::CompareCaseInsensitive((char*)src->items[0], ""))
					{
						current->imageLocation = Strings::Trim((const char*)src->items[0]);
					}
				}

				src->FreeEverything();
				delete src;
			}

			if (Strings::CompareCaseInsensitive(currentAttribute, "type") && current->type == INPUT)
			{
				if (Strings::CompareCaseInsensitive(currentAttributeValue, "text"))
					current->inputType = TEXT;
				if (Strings::CompareCaseInsensitive(currentAttributeValue, "textarea"))
				{
					current->inputType = TEXTAREA;
					current->textwrap = true;
					current->overflowHandling = AUTO;
				}
				else if (Strings::CompareCaseInsensitive(currentAttributeValue, "checkbox"))
				{
					current->inputType = CHECKBOX;
					current->width = 10;
					current->height = 10;
					current->pl = 1;
					current->pt = 1;
					current->pr = 1;
					current->pb = 1;
				}
				else if (Strings::CompareCaseInsensitive(currentAttributeValue, "range"))
					current->inputType = RANGE;
			}

			currentAttribute = NULL;
			currentAttributeValue = NULL;
			continue;
		}

		if (inTag && !onAttributeName && !inString && document[i] == '>')
		{
			inTag = false;

			if (Strings::CompareCaseInsensitive(currentTag, "input") || Strings::CompareCaseInsensitive(currentTag, "img"))
			{
				if (current->parent != NULL)
				{
					current = current->parent;
				}
			}

			if (Strings::CompareCaseInsensitive(currentTag, "p"))
			{
				inP = true;
			}

			free(currentTag);
			currentTag = NULL;
			continue;
		}

		bool endofp = document[i] == '<' && document[i + 1] == '/' && document[i + 2] == 'p';

		if (inP && !endofp)
		{
			textLength++;
		}

		if (inP && document[i] == '<' && document[i + 1] == '/' && document[i + 2] == 'p')
		{
			char* text = (char*)malloc(textLength + 1);
			text[textLength] = 0;
			memmove(text, &document[i - textLength], textLength);
			textLength = 0;
			inP = false;
			current->text = text;
			inClosingTag = true;
			continue;
		}

		if (outsideall && document[i] == '<' && document[i + 1] == '/')
		{
			inClosingTag = true;
			continue;
		}

		if (inClosingTag && document[i] == '>')
		{
			inClosingTag = false;

			if (current->parent != NULL)
			{
				current = current->parent;
			}
			continue;
		}
	}
	if (currentTag != NULL)
		free(currentTag);

	return body;
}

void HTMLParser::ParseStyling(const char* style, HTMLElement* current)
{
	PointerList* styling = PointerList::SplitString(style, ";");

	for (int i = 0; i < styling->count; i++)
	{
		PointerList* kvp = PointerList::SplitString((char*)styling->items[i], ":");

		PointerList* valueSplit = PointerList::SplitString((char*)kvp->items[1], " ");
		PointerList* commaSeperatedSplit = PointerList::SplitString((char*)kvp->items[1], ",");

		if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "width"))
		{
			if (Strings::Contains((char*)kvp->items[1], "px"))
			{
				char* wValue = Strings::Replace((char*)kvp->items[1], "px", "");
				Strings::FreeAndAssign(&wValue, Strings::Trim(wValue));

				current->width = Strings::StringToUint(wValue);
				current->widthScaling = HTMLScaling::PX;
				free(wValue);
			}
			else if (Strings::Contains((char*)kvp->items[1], "%"))
			{
				char* wValue = Strings::Replace((char*)kvp->items[1], "%", "");
				Strings::FreeAndAssign(&wValue, Strings::Trim(wValue));

				current->width = Strings::StringToUint(wValue);
				current->widthScaling = HTMLScaling::PERCENTAGE;
				free(wValue);
			}
			else if (Strings::Contains((char*)kvp->items[1], "fit-content"))
			{
				current->widthScaling = FIT_CONTENT;
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "height"))
		{
			if (Strings::Contains((char*)kvp->items[1], "px"))
			{
				char* hValue = Strings::Replace((char*)kvp->items[1], "px", "");
				Strings::FreeAndAssign(&hValue, Strings::Trim(hValue));

				current->height = Strings::StringToUint(hValue);
				current->heightScaling = HTMLScaling::PX;
				free(hValue);
			}
			else if (Strings::Contains((char*)kvp->items[1], "%"))
			{
				char* hValue = Strings::Replace((char*)kvp->items[1], "%", "");
				Strings::FreeAndAssign(&hValue, Strings::Trim(hValue));

				current->height = Strings::StringToUint(hValue);
				current->heightScaling = HTMLScaling::PERCENTAGE;
				free(hValue);
			}
			else if (Strings::Contains((char*)kvp->items[1], "fit-content"))
			{
				current->heightScaling = FIT_CONTENT;
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "background-color"))
		{
			if (Strings::Contains((char*)kvp->items[1], "rgba") && commaSeperatedSplit->count == 4)
			{
				char* r = Strings::Replace((char*)commaSeperatedSplit->items[0], "rgba(", "");
				Strings::FreeAndAssign(&r, Strings::Trim(r));
				char* g = Strings::Replace((char*)commaSeperatedSplit->items[1], " ", "");
				Strings::FreeAndAssign(&g, Strings::Trim(g));
				char* b = Strings::Replace((char*)commaSeperatedSplit->items[2], " ", "");
				Strings::FreeAndAssign(&b, Strings::Trim(b));
				char* a = Strings::Replace((char*)commaSeperatedSplit->items[3], ")", "");
				Strings::FreeAndAssign(&a, Strings::Trim(a));

				current->bGr = Strings::StringToUint(r);
				current->bGg = Strings::StringToUint(g);
				current->bGb = Strings::StringToUint(b);
				current->bGa = Strings::StringToUint(a);

				free(r);
				free(g);
				free(b);
				free(a);
			}
			else
			{
				current->bGr = Strings::StringToUint((char*)valueSplit->items[0]);
				current->bGg = Strings::StringToUint((char*)valueSplit->items[1]);
				current->bGb = Strings::StringToUint((char*)valueSplit->items[2]);
				current->bGa = Strings::StringToUint((char*)valueSplit->items[3]);
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "color"))
		{
			if (Strings::Contains((char*)kvp->items[1], "rgba") && commaSeperatedSplit->count == 4)
			{
				char* r = Strings::Replace((char*)commaSeperatedSplit->items[0], "rgba(", "");
				Strings::FreeAndAssign(&r, Strings::Trim(r));
				char* g = Strings::Replace((char*)commaSeperatedSplit->items[1], " ", "");
				Strings::FreeAndAssign(&g, Strings::Trim(g));
				char* b = Strings::Replace((char*)commaSeperatedSplit->items[2], " ", "");
				Strings::FreeAndAssign(&b, Strings::Trim(b));
				char* a = Strings::Replace((char*)commaSeperatedSplit->items[3], ")", "");
				Strings::FreeAndAssign(&a, Strings::Trim(a));

				current->cr = Strings::StringToUint(r);
				current->cg = Strings::StringToUint(g);
				current->cb = Strings::StringToUint(b);
				current->ca = Strings::StringToUint(a);

				free(r);
				free(g);
				free(b);
				free(a);
			}
			else
			{
				current->cr = Strings::StringToUint((char*)valueSplit->items[0]);
				current->cg = Strings::StringToUint((char*)valueSplit->items[1]);
				current->cb = Strings::StringToUint((char*)valueSplit->items[2]);
				current->ca = Strings::StringToUint((char*)valueSplit->items[3]);
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "border-color"))
		{
			if (Strings::Contains((char*)kvp->items[1], "rgba") && commaSeperatedSplit->count == 4)
			{
				char* r = Strings::Replace((char*)commaSeperatedSplit->items[0], "rgba(", "");
				Strings::FreeAndAssign(&r, Strings::Trim(r));
				char* g = Strings::Replace((char*)commaSeperatedSplit->items[1], " ", "");
				Strings::FreeAndAssign(&g, Strings::Trim(g));
				char* b = Strings::Replace((char*)commaSeperatedSplit->items[2], " ", "");
				Strings::FreeAndAssign(&b, Strings::Trim(b));
				char* a = Strings::Replace((char*)commaSeperatedSplit->items[3], ")", "");
				Strings::FreeAndAssign(&a, Strings::Trim(a));

				current->blr = Strings::StringToUint(r);
				current->blg = Strings::StringToUint(g);
				current->blb = Strings::StringToUint(b);
				current->bla = Strings::StringToUint(a);

				free(r);
				free(g);
				free(b);
				free(a);
			}
			else
			{
				current->blr = Strings::StringToUint((char*)valueSplit->items[0]);
				current->blg = Strings::StringToUint((char*)valueSplit->items[1]);
				current->blb = Strings::StringToUint((char*)valueSplit->items[2]);
				current->bla = Strings::StringToUint((char*)valueSplit->items[3]);
			}

			current->btr = current->blr;
			current->btg = current->blg;
			current->btb = current->blb;
			current->bta = current->bla;

			current->brr = current->blr;
			current->brg = current->blg;
			current->brb = current->blb;
			current->bra = current->bla;

			current->bbr = current->blr;
			current->bbg = current->blg;
			current->bbb = current->blb;
			current->bba = current->bla;
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "border-color") && valueSplit->count > 4)
		{
			current->blr = Strings::StringToUint((char*)valueSplit->items[0]);
			current->blg = Strings::StringToUint((char*)valueSplit->items[1]);
			current->blb = Strings::StringToUint((char*)valueSplit->items[2]);
			current->bla = Strings::StringToUint((char*)valueSplit->items[3]);
			current->btr = Strings::StringToUint((char*)valueSplit->items[4]);
			current->btg = Strings::StringToUint((char*)valueSplit->items[5]);
			current->btb = Strings::StringToUint((char*)valueSplit->items[6]);
			current->bta = Strings::StringToUint((char*)valueSplit->items[7]);
			current->brr = Strings::StringToUint((char*)valueSplit->items[8]);
			current->brg = Strings::StringToUint((char*)valueSplit->items[9]);
			current->brb = Strings::StringToUint((char*)valueSplit->items[10]);
			current->bra = Strings::StringToUint((char*)valueSplit->items[11]);
			current->bbr = Strings::StringToUint((char*)valueSplit->items[12]);
			current->bbg = Strings::StringToUint((char*)valueSplit->items[13]);
			current->bbb = Strings::StringToUint((char*)valueSplit->items[14]);
			current->bba = Strings::StringToUint((char*)valueSplit->items[15]);
		}
		else if ((Strings::CompareCaseInsensitive((char*)kvp->items[0], "border") && valueSplit->count == 3))
		{

			char* temp = Strings::Replace((char*)valueSplit->items[0], "px", "");
			Strings::FreeAndAssign(&temp, Strings::Trim(temp));
			float bT = Strings::StringToUint(temp);
			free(temp);

			current->bTl = bT;
			current->bTt = bT;
			current->bTr = bT;
			current->bTb = bT;

			int red;
			int green;
			int blue;
			int alpha = 255;

			if (Strings::CompareCaseInsensitive((char*)valueSplit->items[2], "white"))
			{
				red = 255;
				green = 255;
				blue = 255;
			}
			else if (Strings::CompareCaseInsensitive((char*)valueSplit->items[2], "black"))
			{
				red = 0;
				green = 0;
				blue = 0;
			}
			else if (Strings::CompareCaseInsensitive((char*)valueSplit->items[2], "red"))
			{
				red = 255;
				green = 0;
				blue = 0;
			}
			else if (Strings::CompareCaseInsensitive((char*)valueSplit->items[2], "green"))
			{
				red = 0;
				green = 255;
				blue = 0;
			}
			else if (Strings::CompareCaseInsensitive((char*)valueSplit->items[2], "blue"))
			{
				red = 0;
				green = 0;
				blue = 255;
			}
			else if (Strings::CompareCaseInsensitive((char*)valueSplit->items[2], "yellow"))
			{
				red = 255;
				green = 255;
				blue = 0;
			}
			else if (Strings::CompareCaseInsensitive((char*)valueSplit->items[2], "purple"))
			{
				red = 255;
				green = 0;
				blue = 255;
			}
			else if (Strings::CompareCaseInsensitive((char*)valueSplit->items[2], "brown"))
			{
				red = 0;
				green = 255;
				blue = 255;
			}

			current->blr = red;
			current->blg = green;
			current->blb = blue;
			current->bla = alpha;
			current->btr = red;
			current->btg = green;
			current->btb = blue;
			current->bta = alpha;
			current->brr = red;
			current->brg = green;
			current->brb = blue;
			current->bra = alpha;
			current->bbr = red;
			current->bbg = green;
			current->bbb = blue;
			current->bba = alpha;

		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "border-color-left"))
		{
			if (Strings::Contains((char*)kvp->items[1], "rgba") && commaSeperatedSplit->count == 4)
			{
				char* r = Strings::Replace((char*)commaSeperatedSplit->items[0], "rgba(", "");
				Strings::FreeAndAssign(&r, Strings::Trim(r));
				char* g = Strings::Replace((char*)commaSeperatedSplit->items[1], " ", "");
				Strings::FreeAndAssign(&g, Strings::Trim(g));
				char* b = Strings::Replace((char*)commaSeperatedSplit->items[2], " ", "");
				Strings::FreeAndAssign(&b, Strings::Trim(b));
				char* a = Strings::Replace((char*)commaSeperatedSplit->items[3], ")", "");
				Strings::FreeAndAssign(&a, Strings::Trim(a));

				current->blr = Strings::StringToUint(r);
				current->blg = Strings::StringToUint(g);
				current->blb = Strings::StringToUint(b);
				current->bla = Strings::StringToUint(a);

				free(r);
				free(g);
				free(b);
				free(a);
			}
			else
			{
				current->blr = Strings::StringToUint((char*)valueSplit->items[0]);
				current->blg = Strings::StringToUint((char*)valueSplit->items[1]);
				current->blb = Strings::StringToUint((char*)valueSplit->items[2]);
				current->bla = Strings::StringToUint((char*)valueSplit->items[3]);
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "border-color-top"))
		{
			if (Strings::Contains((char*)kvp->items[1], "rgba") && commaSeperatedSplit->count == 4)
			{
				char* r = Strings::Replace((char*)commaSeperatedSplit->items[0], "rgba(", "");
				Strings::FreeAndAssign(&r, Strings::Trim(r));
				char* g = Strings::Replace((char*)commaSeperatedSplit->items[1], " ", "");
				Strings::FreeAndAssign(&g, Strings::Trim(g));
				char* b = Strings::Replace((char*)commaSeperatedSplit->items[2], " ", "");
				Strings::FreeAndAssign(&b, Strings::Trim(b));
				char* a = Strings::Replace((char*)commaSeperatedSplit->items[3], ")", "");
				Strings::FreeAndAssign(&a, Strings::Trim(a));

				current->btr = Strings::StringToUint(r);
				current->btg = Strings::StringToUint(g);
				current->btb = Strings::StringToUint(b);
				current->bta = Strings::StringToUint(a);

				free(r);
				free(g);
				free(b);
				free(a);
			}
			else
			{
				current->btr = Strings::StringToUint((char*)valueSplit->items[0]);
				current->btg = Strings::StringToUint((char*)valueSplit->items[1]);
				current->btb = Strings::StringToUint((char*)valueSplit->items[2]);
				current->bta = Strings::StringToUint((char*)valueSplit->items[3]);
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "border-color-right"))
		{
			if (Strings::Contains((char*)kvp->items[1], "rgba") && commaSeperatedSplit->count == 4)
			{
				char* r = Strings::Replace((char*)commaSeperatedSplit->items[0], "rgba(", "");
				Strings::FreeAndAssign(&r, Strings::Trim(r));
				char* g = Strings::Replace((char*)commaSeperatedSplit->items[1], " ", "");
				Strings::FreeAndAssign(&g, Strings::Trim(g));
				char* b = Strings::Replace((char*)commaSeperatedSplit->items[2], " ", "");
				Strings::FreeAndAssign(&b, Strings::Trim(b));
				char* a = Strings::Replace((char*)commaSeperatedSplit->items[3], ")", "");
				Strings::FreeAndAssign(&a, Strings::Trim(a));

				current->brr = Strings::StringToUint(r);
				current->brg = Strings::StringToUint(g);
				current->brb = Strings::StringToUint(b);
				current->bra = Strings::StringToUint(a);

				free(r);
				free(g);
				free(b);
				free(a);
			}
			else
			{
				current->brr = Strings::StringToUint((char*)valueSplit->items[0]);
				current->brg = Strings::StringToUint((char*)valueSplit->items[1]);
				current->brb = Strings::StringToUint((char*)valueSplit->items[2]);
				current->bra = Strings::StringToUint((char*)valueSplit->items[3]);
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "border-color-bottom"))
		{
			if (Strings::Contains((char*)kvp->items[1], "rgba") && commaSeperatedSplit->count == 4)
			{
				char* r = Strings::Replace((char*)commaSeperatedSplit->items[0], "rgba(", "");
				Strings::FreeAndAssign(&r, Strings::Trim(r));
				char* g = Strings::Replace((char*)commaSeperatedSplit->items[1], " ", "");
				Strings::FreeAndAssign(&g, Strings::Trim(g));
				char* b = Strings::Replace((char*)commaSeperatedSplit->items[2], " ", "");
				Strings::FreeAndAssign(&b, Strings::Trim(b));
				char* a = Strings::Replace((char*)commaSeperatedSplit->items[3], ")", "");
				Strings::FreeAndAssign(&a, Strings::Trim(a));

				current->bbr = Strings::StringToUint(r);
				current->bbg = Strings::StringToUint(g);
				current->bbb = Strings::StringToUint(b);
				current->bba = Strings::StringToUint(a);

				free(r);
				free(g);
				free(b);
				free(a);
			}
			else
			{
				current->bbr = Strings::StringToUint((char*)valueSplit->items[0]);
				current->bbg = Strings::StringToUint((char*)valueSplit->items[1]);
				current->bbb = Strings::StringToUint((char*)valueSplit->items[2]);
				current->bba = Strings::StringToUint((char*)valueSplit->items[3]);
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "border-thickness"))
		{
			char* temp = Strings::Replace((char*)valueSplit->items[0], "px", "");
			Strings::FreeAndAssign(&temp, Strings::Trim(temp));
			current->bTl = Strings::StringToUint(temp);
			free(temp);

			temp = Strings::Replace((char*)valueSplit->items[1], "px", "");
			Strings::FreeAndAssign(&temp, Strings::Trim(temp));
			current->bTt = Strings::StringToUint(temp);
			free(temp);

			temp = Strings::Replace((char*)valueSplit->items[2], "px", "");
			Strings::FreeAndAssign(&temp, Strings::Trim(temp));
			current->bTr = Strings::StringToUint(temp);
			free(temp);

			temp = Strings::Replace((char*)valueSplit->items[3], "px", "");
			Strings::FreeAndAssign(&temp, Strings::Trim(temp));
			current->bTb = Strings::StringToUint(temp);
			free(temp);
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "border-radius"))
		{
			switch (valueSplit->count)
			{
			case(4):
			{
				char* temp = Strings::Replace((char*)valueSplit->items[0], "px", "");
				Strings::FreeAndAssign(&temp, Strings::Trim(temp));
				current->radiusTopLeft = Strings::StringToUint(temp);
				free(temp);

				temp = Strings::Replace((char*)valueSplit->items[1], "px", "");
				Strings::FreeAndAssign(&temp, Strings::Trim(temp));
				current->radiusTopRight = Strings::StringToUint(temp);
				free(temp);

				temp = Strings::Replace((char*)valueSplit->items[2], "px", "");
				Strings::FreeAndAssign(&temp, Strings::Trim(temp));
				current->radiusBottomLeft = Strings::StringToUint(temp);
				free(temp);

				temp = Strings::Replace((char*)valueSplit->items[3], "px", "");
				Strings::FreeAndAssign(&temp, Strings::Trim(temp));
				current->radiusBottomRight = Strings::StringToUint(temp);
				free(temp);
				break;
			}
			case(1):
			{
				char* temp = Strings::Replace((char*)valueSplit->items[0], "px", "");
				Strings::FreeAndAssign(&temp, Strings::Trim(temp));
				current->radiusTopLeft = Strings::StringToUint(temp);
				current->radiusTopRight = current->radiusTopLeft;
				current->radiusBottomLeft = current->radiusTopLeft;
				current->radiusBottomRight = current->radiusTopLeft;
				free(temp);
				break;
			}
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "display"))
		{
			if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "block"))
			{
				current->display = BLOCK;
			}
			/*else if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "inline"))
			{
				current->display = INLINE;
			}*/
			else if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "flex"))
			{
				current->display = FLEX;
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "box-sizing"))
		{
			if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "content-box"))
			{
				current->boxSizing = CONTENT_BOX;
			}
			else if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "border-box"))
			{
				current->boxSizing = BORDER_BOX;
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "flex-direction"))
		{
			bool direction = Strings::CompareCaseInsensitive((char*)kvp->items[1], "row");
			current->flexRow = direction;
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "flex-wrap"))
		{
			bool direction = Strings::CompareCaseInsensitive((char*)kvp->items[1], "wrap");
			current->flexWrap = direction;
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "flex-grow"))
		{
			bool grow = Strings::CompareCaseInsensitive((char*)kvp->items[1], "1");
			current->flexGrow = grow;
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "flex-shrink"))
		{
			bool shrink = Strings::CompareCaseInsensitive((char*)kvp->items[1], "1");
			current->flexShrink = shrink;
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "flex-basis"))
		{
			if (Strings::Contains((char*)kvp->items[1], "px"))
			{
				char* wValue = Strings::Replace((char*)kvp->items[1], "px", "");
				Strings::FreeAndAssign(&wValue, Strings::Trim(wValue));

				current->flexBasis = Strings::StringToUint(wValue);
				current->flexBasisType = HTMLScaling::PX;
				free(wValue);
			}
			else if (Strings::Contains((char*)kvp->items[1], "%"))
			{
				char* wValue = Strings::Replace((char*)kvp->items[1], "%", "");
				Strings::FreeAndAssign(&wValue, Strings::Trim(wValue));

				current->flexBasis = Strings::StringToUint(wValue);
				current->flexBasisType = HTMLScaling::PERCENTAGE;
				free(wValue);
			}
			else if (Strings::Contains((char*)kvp->items[1], "fit-content"))
			{
				current->flexBasis = FIT_CONTENT;
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "flex"))
		{
			if (valueSplit->count == 3)
			{
				bool shrink = Strings::CompareCaseInsensitive((char*)valueSplit->items[1], "1");
				current->flexShrink = shrink;
				bool grow = Strings::CompareCaseInsensitive((char*)valueSplit->items[0], "1");
				current->flexGrow = grow;

				if (Strings::Contains((char*)valueSplit->items[2], "px"))
				{
					char* wValue = Strings::Replace((char*)valueSplit->items[2], "px", "");
					Strings::FreeAndAssign(&wValue, Strings::Trim(wValue));

					current->flexBasis = Strings::StringToUint(wValue);
					current->flexBasisType = HTMLScaling::PX;
					free(wValue);
				}
				else if (Strings::Contains((char*)valueSplit->items[2], "%"))
				{
					char* wValue = Strings::Replace((char*)valueSplit->items[2], "%", "");
					Strings::FreeAndAssign(&wValue, Strings::Trim(wValue));

					current->flexBasis = Strings::StringToUint(wValue);
					current->flexBasisType = HTMLScaling::PERCENTAGE;
					free(wValue);
				}
				else if (Strings::Contains((char*)valueSplit->items[2], "fit-content"))
				{
					current->flexBasis = FIT_CONTENT;
				}
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "justify-content"))
		{
			if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "start"))
				current->justifyContent = HTMLFlexPosition::START;
			else if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "center"))
				current->justifyContent = HTMLFlexPosition::CENTER;
			else if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "end"))
				current->justifyContent = HTMLFlexPosition::END;
			else if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "space-between"))
				current->justifyContent = HTMLFlexPosition::SPACE_BETWEEN;
			else if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "space-around"))
				current->justifyContent = HTMLFlexPosition::SPACE_AROUND;
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "align-items"))
		{
			if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "start"))
				current->alignItems = HTMLFlexPosition::START;
			else if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "center"))
				current->alignItems = HTMLFlexPosition::CENTER;
			else if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "end"))
				current->alignItems = HTMLFlexPosition::END;
			/*else if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "split"))
				current->alignItems = HTMLFlexPosition::SPACE_BETWEEN;
			else if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "space-around"))
				current->alignItems = HTMLFlexPosition::SPACE_AROUND;*/
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "padding"))
		{

			switch (valueSplit->count)
			{
			case(4):
			{
				char* temp = Strings::Replace((char*)valueSplit->items[0], "px", "");
				Strings::FreeAndAssign(&temp, Strings::Trim(temp));
				current->pl = Strings::StringToUint(temp);
				free(temp);

				temp = Strings::Replace((char*)valueSplit->items[1], "px", "");
				Strings::FreeAndAssign(&temp, Strings::Trim(temp));
				current->pt = Strings::StringToUint(temp);
				free(temp);

				temp = Strings::Replace((char*)valueSplit->items[2], "px", "");
				Strings::FreeAndAssign(&temp, Strings::Trim(temp));
				current->pr = Strings::StringToUint(temp);
				free(temp);

				temp = Strings::Replace((char*)valueSplit->items[3], "px", "");
				Strings::FreeAndAssign(&temp, Strings::Trim(temp));
				current->pb = Strings::StringToUint(temp);
				free(temp);
				break;
			}
			case(1):
			{
				char* temp = Strings::Replace((char*)valueSplit->items[0], "px", "");
				Strings::FreeAndAssign(&temp, Strings::Trim(temp));
				current->pl = Strings::StringToUint(temp);
				current->pt = current->pl;
				current->pr = current->pl;
				current->pb = current->pl;
				free(temp);
				break;
			}
			}

			/*current->pl *= 2;
			current->pt *= 2;
			current->pr *= 2;
			current->pb *= 2;*/
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "margin"))
		{

			switch (valueSplit->count)
			{
			case(4):
			{
				char* temp = Strings::Replace((char*)valueSplit->items[0], "px", "");
				Strings::FreeAndAssign(&temp, Strings::Trim(temp));
				current->ml = Strings::StringToUint(temp);
				free(temp);

				temp = Strings::Replace((char*)valueSplit->items[1], "px", "");
				Strings::FreeAndAssign(&temp, Strings::Trim(temp));
				current->mt = Strings::StringToUint(temp);
				free(temp);

				temp = Strings::Replace((char*)valueSplit->items[2], "px", "");
				Strings::FreeAndAssign(&temp, Strings::Trim(temp));
				current->mr = Strings::StringToUint(temp);
				free(temp);

				temp = Strings::Replace((char*)valueSplit->items[3], "px", "");
				Strings::FreeAndAssign(&temp, Strings::Trim(temp));
				current->mb = Strings::StringToUint(temp);
				free(temp);
				break;
			}
			case(1):
			{
				char* temp = Strings::Replace((char*)valueSplit->items[0], "px", "");
				Strings::FreeAndAssign(&temp, Strings::Trim(temp));
				current->ml = Strings::StringToUint(temp);
				current->mt = current->ml;
				current->mr = current->ml;
				current->mb = current->ml;
				free(temp);
				break;
			}
			}

			/*current->ml *= 2;
			current->mt *= 2;
			current->mr *= 2;
			current->mb *= 2;*/
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "font-family"))
		{
			char* temp = Strings::Trim((char*)valueSplit->items[0]);
			current->font = Strings::StringToUint(temp);
			free(temp);
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "font-size"))
		{
			char* temp = Strings::Replace((char*)valueSplit->items[0], "px", "");
			Strings::FreeAndAssign(&temp, Strings::Trim(temp));
			current->fontsize = Strings::StringToUint(temp);
			free(temp);
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "text-wrap"))
		{
			char* temp = Strings::Trim((char*)valueSplit->items[0]);

			if (Strings::CompareCaseInsensitive(temp, "wrap"))
				current->textwrap = true;
			else
				current->textwrap = false;
			free(temp);
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "text-align"))
		{
			if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "start"))
				current->textAlign = TEXT_START;
			else if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "center"))
				current->textAlign = TEXT_CENTER;
			else if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "end"))
				current->textAlign = TEXT_END;
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "position"))
		{
			if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "relative"))
				current->position = POS_RELATIVE;
			else if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "absolute"))
				current->position = POS_ABSOLUTE;
			else if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "fixed"))
				current->position = POS_FIXED;
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "left"))
		{
			if (Strings::Contains((char*)kvp->items[1], "px"))
			{
				char* hValue = Strings::Replace((char*)kvp->items[1], "px", "");
				Strings::FreeAndAssign(&hValue, Strings::Trim(hValue));

				current->left = Strings::StringToUint(hValue);
				current->leftSet = true;
				free(hValue);
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "top"))
		{
			if (Strings::Contains((char*)kvp->items[1], "px"))
			{
				char* hValue = Strings::Replace((char*)kvp->items[1], "px", "");
				Strings::FreeAndAssign(&hValue, Strings::Trim(hValue));

				current->top = Strings::StringToUint(hValue);
				current->topSet = true;
				free(hValue);
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "right"))
		{
			if (Strings::Contains((char*)kvp->items[1], "px"))
			{
				char* hValue = Strings::Replace((char*)kvp->items[1], "px", "");
				Strings::FreeAndAssign(&hValue, Strings::Trim(hValue));

				current->right = Strings::StringToUint(hValue);
				current->rightSet = true;
				free(hValue);
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "bottom"))
		{
			if (Strings::Contains((char*)kvp->items[1], "px"))
			{
				char* hValue = Strings::Replace((char*)kvp->items[1], "px", "");
				Strings::FreeAndAssign(&hValue, Strings::Trim(hValue));

				current->bottom = Strings::StringToUint(hValue);
				current->bottomSet = true;
				free(hValue);
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "min-width"))
		{
			if (Strings::Contains((char*)kvp->items[1], "px"))
			{
				char* wValue = Strings::Replace((char*)kvp->items[1], "px", "");
				Strings::FreeAndAssign(&wValue, Strings::Trim(wValue));

				current->minWidth = Strings::StringToUint(wValue);
				current->minWidthScaling = HTMLScaling::PX;
				free(wValue);
			}
			else if (Strings::Contains((char*)kvp->items[1], "%"))
			{
				char* wValue = Strings::Replace((char*)kvp->items[1], "%", "");
				Strings::FreeAndAssign(&wValue, Strings::Trim(wValue));

				current->minWidth = Strings::StringToUint(wValue);
				current->minWidthScaling = HTMLScaling::PERCENTAGE;
				free(wValue);
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "max-width"))
		{
			if (Strings::Contains((char*)kvp->items[1], "px"))
			{
				char* wValue = Strings::Replace((char*)kvp->items[1], "px", "");
				Strings::FreeAndAssign(&wValue, Strings::Trim(wValue));

				current->maxWidth = Strings::StringToUint(wValue);
				current->maxWidthScaling = HTMLScaling::PX;
				free(wValue);
			}
			else if (Strings::Contains((char*)kvp->items[1], "%"))
			{
				char* wValue = Strings::Replace((char*)kvp->items[1], "%", "");
				Strings::FreeAndAssign(&wValue, Strings::Trim(wValue));

				current->maxWidth = Strings::StringToUint(wValue);
				current->maxWidthScaling = HTMLScaling::PERCENTAGE;
				free(wValue);
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "min-height"))
		{
			if (Strings::Contains((char*)kvp->items[1], "px"))
			{
				char* wValue = Strings::Replace((char*)kvp->items[1], "px", "");
				Strings::FreeAndAssign(&wValue, Strings::Trim(wValue));

				current->minHeight = Strings::StringToUint(wValue);
				current->minHeightScaling = HTMLScaling::PX;
				free(wValue);
			}
			else if (Strings::Contains((char*)kvp->items[1], "%"))
			{
				char* wValue = Strings::Replace((char*)kvp->items[1], "%", "");
				Strings::FreeAndAssign(&wValue, Strings::Trim(wValue));

				current->minHeight = Strings::StringToUint(wValue);
				current->minHeightScaling = HTMLScaling::PERCENTAGE;
				free(wValue);
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "max-height"))
		{
			if (Strings::Contains((char*)kvp->items[1], "px"))
			{
				char* wValue = Strings::Replace((char*)kvp->items[1], "px", "");
				Strings::FreeAndAssign(&wValue, Strings::Trim(wValue));

				current->maxHeight = Strings::StringToUint(wValue);
				current->maxHeightScaling = HTMLScaling::PX;
				free(wValue);
			}
			else if (Strings::Contains((char*)kvp->items[1], "%"))
			{
				char* wValue = Strings::Replace((char*)kvp->items[1], "%", "");
				Strings::FreeAndAssign(&wValue, Strings::Trim(wValue));

				current->maxHeight = Strings::StringToUint(wValue);
				current->maxHeightScaling = HTMLScaling::PERCENTAGE;
				free(wValue);
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "overflow"))
		{
			if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "visible"))
			{
				current->overflowHandling = VISIBLE;
			}
			else if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "hidden"))
			{
				current->overflowHandling = HIDDEN;
			}
			else if (Strings::CompareCaseInsensitive((char*)kvp->items[1], "auto"))
			{
				current->overflowHandling = AUTO;
			}
		}
		else if (Strings::CompareCaseInsensitive((char*)kvp->items[0], "gap"))
		{
			if (Strings::Contains((char*)kvp->items[1], "px"))
			{
				char* wValue = Strings::Replace((char*)kvp->items[1], "px", "");
				Strings::FreeAndAssign(&wValue, Strings::Trim(wValue));

				current->gap = Strings::StringToUint(wValue);
				free(wValue);
			}
		}

		commaSeperatedSplit->FreeEverything();
		delete commaSeperatedSplit;

		valueSplit->FreeEverything();
		delete valueSplit;

		kvp->FreeEverything();
		delete kvp;
	}

	styling->FreeEverything();
	delete styling;
}


//#define BGRAPH_DEBUG 1

void HTMLParser::ResolveHTML(HTMLElement* element, JSONElement* variables)
{
	bool dirty = true;
	int loopcount = 0;
	while (dirty)
	{
#ifdef BGRAPH_DEBUG
		char* string = (char*)malloc(1);
		if (string != NULL)
		{
			string[0] = 0;

			HTMLElement::HTMLPrintTree(element, 0, &string);

			DBuffer* buf = new DBuffer();
			buf->Add((unsigned char*)string, Strings::Length(string));
			Files::WriteFile((char*)"TempTreeJSON.json", buf);
			delete buf;
			free(string);
#endif
			HTMLElement::HTMLCalculateSizesAndPositionsTD(element);
			HTMLElement::HTMLCalculateFlexWrap(element);
			HTMLElement::HTMLCalculateSizesAndPositionsBU(element, variables);
			HTMLElement::HTMLHandleFlexPositioning(element);
			dirty = HTMLElement::HTMLAnyDirty(element);
			loopcount++;
#ifdef BGRAPH_DEBUG
		}
#endif
	}
	HTMLElement::CalculateScrollbars(element);
	HTMLElement::RecursiveApplyScrolling(element, element);
	int z = 0;
}

void HTMLParser::ResolveDynamicHTML(HTMLElement* element, JSONElement* variables, PointerList* components)
{
	KeyPointerPair* bGIf = KeyPointerPair::GetKeyValuePointer(element->attributes, "bGIf");
	KeyPointerPair* bGGenerator = KeyPointerPair::GetKeyValuePointer(element->attributes, "bGGenerator");

	if (bGIf != NULL)
	{
		char* condition = (char*)bGIf->pointer;
		bool result = false;

		if (Strings::Contains(condition, "=="))
		{

		}
		else if (Strings::Contains(condition, "<="))
		{

		}
		else if (Strings::Contains(condition, ">="))
		{

		}
		else if (Strings::Contains(condition, "<"))
		{

		}
		else if (Strings::Contains(condition, ">"))
		{

		}
		else
		{
			JSONElement* variable = JSONElement::GetElement(condition, variables, false);

			if (variable != NULL)
			{
				if (variable->type == RENAMED_REFERENCE)
				{
					result = true;
				}
				else
				{
					char* trm = Strings::Trim((char*)variable->value);
					if (Strings::CompareCaseInsensitive(trm, "false"))
					{
						result = false;
					}
					else if (Strings::Length(trm) > 0)
					{
						result = true;
					}

					free(trm);
				}
			}
		}

		if (!result)
		{
			if (element->children.size() > 0)
			{
				HTMLElement* child = element->children.at(0);
				element->children.pop_back();
				element->conditionals.push_back(child);
			}
		}
		else
		{
			if (element->conditionals.size() > 0)
			{
				HTMLElement* child = element->conditionals.at(0);
				element->conditionals.pop_back();
				element->children.push_back(child);
			}
		}
	}
	else if (bGGenerator != NULL)
	{
		if (element->generators.size() == 0)
		{
			if (element->children.size() > 0)
			{
				HTMLElement* child = element->children.at(0);
				element->children.pop_back();
				element->generators.push_back(child);
			}
		}

		char* value = (char*)bGGenerator->pointer;

		PointerList* generatorArgs = PointerList::SplitString(value, ";");

		if (generatorArgs->count > 0 && Strings::Length((char*)generatorArgs->items[1]) > 0)
		{
			char* intName = Strings::Trim((char*)generatorArgs->items[0]);
			char* varName = Strings::Trim((char*)generatorArgs->items[1]);
			char* tracking = NULL;

			if (generatorArgs->count > 2)
			{
				tracking = Strings::Trim((char*)generatorArgs->items[2]);
			}

			JSONElement* variableKP = JSONElement::GetElement(varName, variables);

			if (variableKP != NULL)
			{
				//remove any children that index and id/tracking item dont match.
				if (tracking == NULL)
				{
					for (int i = 0; i < element->children.size(); i++)
					{
						delete element->children.at(i);
					}
					element->children.clear();
				}

				for (int i = 0; i < variableKP->children->count; i++)
				{
					if (tracking != NULL)
					{
						char* genIndex = Strings::UintToString(i); // Must Free if not used

						JSONElement* tempVars = new JSONElement(); // Must Delete
						JSONElement* item = (JSONElement*)variableKP->children->items[i];
						JSONElement* fakeItem = JSONElement::CreateRenamedReference(intName, item); // Must Delete
						tempVars->children->AddPointer(fakeItem);
						fakeItem->parent = tempVars;

						JSONElement* trackel = JSONElement::GetElement(tracking, fakeItem, false);
						char* trackValue = NULL;

						if (trackel == NULL || trackel->value == NULL)
							trackValue = Strings::Clone(""); //Must Free if not used
						else
							trackValue = Strings::Clone(trackel->value); //Must Free if not used

						if (i < element->children.size())
						{
							KeyPointerPair* GenIndRef = KeyPointerPair::GetKeyValuePointer(element->children.at(i)->attributes, "bGGeneratorIndex");
							KeyPointerPair* GenTrackRef = KeyPointerPair::GetKeyValuePointer(element->children.at(i)->attributes, "bGGeneratorTracking");

							if (Strings::Compare((char*)GenIndRef->pointer, genIndex) && Strings::Compare((char*)GenTrackRef->pointer, trackValue))
							{
								free(trackValue);
								free(genIndex);
							}
							else
							{
								HTMLElement* child = HTMLParser::RecursiveGenerate(element->generators.at(0), NULL, tempVars, variables);

								KeyPointerPair* genIKPP = new KeyPointerPair(Strings::Clone("bGGeneratorIndex"), genIndex);
								KeyPointerPair* trackKPP = new KeyPointerPair(Strings::Clone("bGGeneratorTracking"), trackValue);

								child->attributes->AddPointer(genIKPP);
								child->attributes->AddPointer(trackKPP);

								HTMLElement* oldElement = element->children.at(i);
								delete oldElement;

								std::vector<HTMLElement*>::iterator it = element->children.begin();
								std::advance(it, i);
								element->children.erase(it);

								element->children.insert(element->children.begin() + i, child);
							}
						}
						else
						{
							HTMLElement* child = HTMLParser::RecursiveGenerate(element->generators.at(0), NULL, tempVars, variables);

							KeyPointerPair* genIKPP = new KeyPointerPair(Strings::Clone("bGGeneratorIndex"), genIndex);
							KeyPointerPair* trackKPP = new KeyPointerPair(Strings::Clone("bGGeneratorTracking"), trackValue);

							child->attributes->AddPointer(genIKPP);
							child->attributes->AddPointer(trackKPP);

							element->children.push_back(child);
						}
						delete tempVars;
					}
					else
					{
						JSONElement* tempVars = new JSONElement();
						JSONElement* item = (JSONElement*)variableKP->children->items[i]; //Doing some weird renaming and shallow copying
						JSONElement* fakeItem = JSONElement::CreateRenamedReference(intName, item);

						tempVars->children->AddPointer(fakeItem);
						fakeItem->parent = tempVars;

						HTMLElement* child = HTMLParser::RecursiveGenerate(element->generators.at(0), NULL, tempVars, variables);

						element->children.push_back(child);

						delete tempVars;
					}
				}
			}

			free(intName);
			free(varName);
		}

		generatorArgs->FreeEverything();
		delete generatorArgs;

	}

	if (element->tag != NULL)
	{
		KeyPointerPair* found = KeyPointerPair::GetKeyValuePointer(components, element->tag);

		if (found && element->children.size() == 0)
		{
			HTMLElement* newItem = HTMLParser::RecreateElement((HTMLElement*)found->pointer, NULL);
			HTMLParser::FindAndReplaceComponentsVariables(element, newItem);
			newItem->parent = element;
			element->children.push_back(newItem);
		}
	}

	for (int i = 0; i < element->children.size(); i++)
	{
		ResolveDynamicHTML(element->children.at(i), variables, components);
	}
}

HTMLElement* HTMLParser::RecursiveGenerate(HTMLElement* ref, HTMLElement* parent, JSONElement* tempVariables, JSONElement* variables)
{

	HTMLElement* result = new HTMLElement(ref->type);
	ref->CopyFlatDetailsToElement(result);

	if (tempVariables != NULL)
	{
		for (int i = 0; i < result->attributes->count; i++)
		{
			KeyPointerPair* pair = (KeyPointerPair*)result->attributes->items[i];
			if (Strings::Contains((char*)pair->pointer, "{{") && !(result->type == INPUT && Strings::Compare(pair->key, "value")))
			{
				char* interpolated = HTMLElement::HTMLStringInterpolate((char*)pair->pointer, tempVariables);
				if (interpolated != NULL)
				{
					free(pair->pointer);
					pair->pointer = interpolated;
				}
			}

			for (int x = 0; x < tempVariables->children->count; x++)
			{
				JSONElement* el = (JSONElement*)tempVariables->children->items[x];
				int length = Strings::Length(el->name);
				int lengthAtt = Strings::Length(pair->key);

				if ((!Strings::Compare(pair->key, "style") && !Strings::Compare(pair->key, "src") /*&& Strings::CountOf((char*)pair->pointer, ";") != 0*/) || (result->type == INPUT && Strings::Compare(pair->key, "value")))
				{
					PointerList* splitAttributeValue = PointerList::SplitString((char*)pair->pointer, ";");

					char* final = (char*)malloc(1);
					final[0] = 0;

					for (int y = 0; y < splitAttributeValue->count; y++)
					{
						char* section = (char*)splitAttributeValue->items[y];
						int dotdex = Strings::IndexOf(section, ".", 1);
						int location = Strings::IndexOf(section, el->name, 1);
						if (location != -1 && (dotdex == -1 || dotdex > location) && (dotdex - location == length || dotdex == -1))
						{
							char* ref = NULL;
							JSONElement::CreateRootPathFromReference(el, &ref);

							char* replacement = Strings::Replace(section, el->name, ref);

							free(splitAttributeValue->items[y]);
							splitAttributeValue->items[y] = replacement;

							free(ref);
						}

						Strings::FreeAndAssign(&final, Strings::Concat(final, (char*)splitAttributeValue->items[y]));
						if (y < splitAttributeValue->count - 1)
							Strings::FreeAndAssign(&final, Strings::Concat(final, ";"));
					}

					free(pair->pointer);
					pair->pointer = final;

					splitAttributeValue->FreeEverything();
					delete splitAttributeValue;

					if (result->type == INPUT)
					{
						HTMLParser::UpdateElementValues(result, variables);
					}
				}
			}
		}
	}

	for (int i = 0; i < result->attributes->count; i++)
	{
		KeyPointerPair* pair = (KeyPointerPair*)result->attributes->items[i];
		if (Strings::Contains((char*)pair->pointer, "{{") && !(result->type == INPUT && Strings::Compare(pair->key, "value")))
		{
			char* interpolated = HTMLElement::HTMLStringInterpolate((char*)pair->pointer, variables);
			if (interpolated != NULL)
			{
				free(pair->pointer);
				pair->pointer = interpolated;
			}
		}
	}

	if (tempVariables != NULL)
	{
		if (result->text != NULL && Strings::Contains(result->text, "{{"))
		{
			char* interpolated = HTMLElement::HTMLStringInterpolate((char*)result->text, tempVariables);
			if (interpolated != NULL)
			{
				free(result->text);
				result->text = interpolated;
			}
		}
	}

	if (result->text != NULL && Strings::Contains(result->text, "{{"))
	{
		char* interpolated = HTMLElement::HTMLStringInterpolate((char*)result->text, variables);
		if (interpolated != NULL)
		{
			free(result->text);
			result->text = interpolated;
		}
	}

	if (parent == NULL)
	{

		for (int i = 0; i < ref->children.size(); i++)
		{
			RecursiveGenerate(ref->children.at(i), result, tempVariables, variables);
		}

		return result;
	}
	else
	{
		result->parent = parent;
		parent->children.push_back(result);

		for (int i = 0; i < ref->children.size(); i++)
		{
			RecursiveGenerate(ref->children.at(i), result, tempVariables, variables);
		}
	}
	return NULL;
}

HTMLElement* HTMLParser::RecreateElement(HTMLElement* ref, HTMLElement* parent)
{

	HTMLElement* result = new HTMLElement(ref->type);
	ref->CopyFlatDetailsToElement(result);

	if (parent == NULL)
	{

		for (int i = 0; i < ref->children.size(); i++)
		{
			RecreateElement(ref->children.at(i), result);
		}

		return result;
	}
	else
	{
		result->parent = parent;
		parent->children.push_back(result);

		for (int i = 0; i < ref->children.size(); i++)
		{
			RecreateElement(ref->children.at(i), result);
		}
	}
	return NULL;
}

void HTMLParser::FindAndReplaceComponentsVariables(HTMLElement* container, HTMLElement* element)
{
	bool textDone = false;
	bool doText = element->text != NULL && Strings::Contains(element->text, "{{") && Strings::Contains(element->text, "}}");
	for (int i = 0; i < element->attributes->count; i++)
	{
		KeyPointerPair* pair = (KeyPointerPair*)element->attributes->items[i];
		for (int x = 0; x < container->attributes->count; x++)
		{
			KeyPointerPair* replaceRef = (KeyPointerPair*)container->attributes->items[x];

			if (!Strings::Compare(replaceRef->key, "style"))
			{
				if (!textDone && doText && Strings::Contains((char*)pair->pointer, replaceRef->key))
				{
					Strings::FreeAndAssign((char**)&element->text, Strings::Replace(element->text, replaceRef->key, (char*)replaceRef->pointer));
					textDone = true;
				}

				if (Strings::Contains((char*)pair->pointer, replaceRef->key))
				{
					Strings::FreeAndAssign((char**)&pair->pointer, Strings::Replace((char*)pair->pointer, replaceRef->key, (char*)replaceRef->pointer));
				}
			}
		}
	}


	for (int i = 0; i < element->children.size(); i++)
		FindAndReplaceComponentsVariables(container, element->children.at(i));
}

void HTMLParser::UpdateElementValues(HTMLElement* element, JSONElement* variables)
{
	if (element->type == HTMLElementType::INP)
	{
		for (int i = 0; i < element->attributes->count; i++)
		{
			KeyPointerPair* pair = (KeyPointerPair*)element->attributes->items[i];
			if (Strings::CompareCaseInsensitive(pair->key, "value"))
			{
				switch (element->inputType)
				{
				case(HTMLInputType::CHECKBOX):
				{
					char* val = HTMLElement::HTMLStringInterpolate((char*)pair->pointer, variables);
					if (val != NULL)
					{
						if (Strings::CompareCaseInsensitive(val, "true") && !element->checked)
						{
							element->checked = !element->checked;
							element->bGr = 255 - element->bGr;
							element->bGg = 255 - element->bGg;
							element->bGb = 255 - element->bGb;
						}

						free(val);
					}
					break;
				}
				case(HTMLInputType::TEXTAREA):
				case(HTMLInputType::TEXT):
				{
					char* val = HTMLElement::HTMLStringInterpolate((char*)pair->pointer, variables);
					if (val != NULL)
					{
						int valLength = Strings::Length(val);
						element->characters.clear();

						for (int x = 0; x < valLength; x++)
						{
							HTMLTextCharacter c;
							c.character = Keyboard::TranslateKeyboardToHTMLTextChar(val[x]);
							element->characters.push_back(c);
						}
						element->textDirty = true;
						free(val);
					}
					break;
				}
				}
			}
		}
	}

	for (int i = 0; i < element->children.size(); i++)
	{
		UpdateElementValues(element->children.at(i), variables);
	}
}