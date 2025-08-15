global float StringSpaceScaler = 1;

internal D_GlyphInfo D_GetGlyphFromCodepoint(wchar_t CurrentCodepoint,_D_Font_Info * FontInfo)
{
	int Hash = CurrentCodepoint % GLYPH_INFO_HASHMAP_LENGTH;
    
	D_GlyphInfoNode* PreviousNode = 0;
	D_GlyphInfoNode** CurrentNodePointer = &FontInfo->GlyphHashmap[Hash];
    
	bool GlyphNotExist = true;
    
	while ((*CurrentNodePointer))
	{
		PreviousNode = *CurrentNodePointer;
        
		if ((*CurrentNodePointer)->Info.value != CurrentCodepoint)
		{
			CurrentNodePointer = &(*CurrentNodePointer)->NextNode;
		}
		else
		{
			GlyphNotExist = false;
			break;
		}
	}
    
	if (GlyphNotExist)
	{
		AppData->UpdateAtlas = true;
		(*CurrentNodePointer) = FontInfo->AllGlyphInfo + FontInfo->AllGlyphInfoCount;
		FontInfo->AllGlyphInfoCount++;
        
		if (PreviousNode)
			PreviousNode->NextNode = *CurrentNodePointer;
        
		D_GlyphInfoNode* CurrentNode = *CurrentNodePointer;
		D_GlyphInfo* CurrentGlyphInfo = &CurrentNode->Info;
        
		CurrentGlyphInfo->value = CurrentCodepoint;
        
		float ScaleFactor = stbtt_ScaleForPixelHeight(&FontInfo->FontInfo, FontInfo->GlyphResolution);
        
		stbtt_GetCodepointHMetrics(&FontInfo->FontInfo, CurrentCodepoint, &CurrentGlyphInfo->advanceX, 0);
		CurrentGlyphInfo->advanceX *= ScaleFactor;
        
		int GlyphWidth = 0;
		int GlyphHeight = 0;
		int GlyphOffsetX = 0;
		int GlyphOffsetY = 0;
        
		unsigned char* GlyphBitmap = stbtt_GetCodepointSDF
		(
         &FontInfo->FontInfo,
         ScaleFactor,
         CurrentCodepoint,
         5,
         180,
         180 / 5.0,
         &GlyphWidth,
         &GlyphHeight,
         &GlyphOffsetX,
         &GlyphOffsetY
         );
        
		CurrentGlyphInfo->offsetX = GlyphOffsetX;
		CurrentGlyphInfo->offsetY = GlyphOffsetY;
        
		//what is this?
		//font base line?
		int Ascent = 0;
		stbtt_GetFontVMetrics(&FontInfo->FontInfo, &Ascent, 0, 0);
		CurrentGlyphInfo->offsetY += Ascent * ScaleFactor;
#if false
		stbtt_MakeCodepointBitmap
		(
         &FontContext->FontInfo,
         (unsigned char*)BitmapForCodepoint,
         GlyphWidth,
         GlyphHeight,
         GlyphWidth,
         ScaleFactor,
         ScaleFactor,
         CurrentCodepoint
         );
#endif
        
		stbrp_rect Rect = {};
        
		Rect.w = GlyphWidth + FontInfo->GlyphAltasPadding * 2;
		Rect.h = GlyphHeight + FontInfo->GlyphAltasPadding * 2;
        
		stbrp_pack_rects(&FontInfo->STB_RP_Context, &Rect, 1);
        
		CurrentGlyphInfo->GlyphRect.x = Rect.x + (float)FontInfo->GlyphAltasPadding;
		CurrentGlyphInfo->GlyphRect.y = Rect.y + (float)FontInfo->GlyphAltasPadding;
		CurrentGlyphInfo->GlyphRect.width = (float)GlyphWidth;
		CurrentGlyphInfo->GlyphRect.height = (float)GlyphHeight;
        
		if (Rect.was_packed)
		{
			for (int BitmapY = 0; BitmapY < GlyphHeight; BitmapY++)
			{
				for (int BitmapX = 0; BitmapX < GlyphWidth; BitmapX++)
				{
					int YRow = (Rect.y + BitmapY) * FontInfo->GlyphAtlasImage.width;
					int XRow = (Rect.x + BitmapX);
					unsigned char* Pixel = ((unsigned char*)FontInfo->GlyphAtlasImage.data) + YRow + XRow;
					*Pixel = ((unsigned char*)GlyphBitmap)[BitmapY * GlyphWidth + BitmapX];
				}
			}
		}
        
		stbtt_FreeSDF(GlyphBitmap, FontInfo->FontInfo.userdata);
	}
    
	return (*CurrentNodePointer)->Info;
}

internal float GetCodepointWidth(wchar_t CodepointPointer, _D_Font_Info* FontInfo, float Scale, float SpaceSizeOffset, float SpaceScaler)
{
	D_GlyphInfo CurrentGlyphInfo = D_GetGlyphFromCodepoint(CodepointPointer, FontInfo);;
	D_Rectangle GlyphRect = CurrentGlyphInfo.GlyphRect;
    
	if (CurrentGlyphInfo.value != CodepointPointer)
		ASSERT;
    
	float FontAdvanceX = ((float)CurrentGlyphInfo.advanceX * Scale) / (float)FontInfo->GlyphResolution;
	float FontOffsetX = ((float)CurrentGlyphInfo.offsetX * Scale) / (float)FontInfo->GlyphResolution;
    
	return SpaceSizeOffset + (FontAdvanceX)*SpaceScaler;
}

internal float GetGlyphWidth(D_GlyphInfo GlyphInfo , _D_Font_Info *FontInfo , float SpaceSizeOffset , float SpaceScaler , float Scale)
{
	float FontAdvanceX = ((float)GlyphInfo.advanceX * Scale) / (float)FontInfo->GlyphResolution;
	float FontOffsetX = ((float)GlyphInfo.offsetX * Scale) / (float)FontInfo->GlyphResolution;
    
	//why it need to multiply 2?
	return (SpaceSizeOffset + (FontAdvanceX + FontOffsetX)*SpaceScaler) * 2;
}

internal void GetStringWidth(int Count,wchar_t * CodepointPointer,_D_Font_Info *FontInfo, float SpaceSizeOffset,float SpaceScaler,float Scale,int * ResultCount,float * ResultWidth)
{
	float TotalSpace = 0;
	int CurrentCount = 0;
    
	while (1)
	{
		wchar_t CurrentCodepoint = *CodepointPointer;
        
		if (!CurrentCodepoint)
			break;
        
		if (CurrentCount >= Count && Count != -1)
			break;
#if true
		D_GlyphInfo CurrentGlyphInfo = D_GetGlyphFromCodepoint(CurrentCodepoint, FontInfo);;
		D_Rectangle GlyphRect = CurrentGlyphInfo.GlyphRect;
        
		if (CurrentGlyphInfo.value != CurrentCodepoint)
			ASSERT;        
        
		TotalSpace += GetGlyphWidth(CurrentGlyphInfo ,FontInfo , SpaceSizeOffset ,SpaceScaler , Scale);
        
		CodepointPointer++;
		CurrentCount++;
        
#endif 
	}
    
	if(ResultWidth)
		*ResultWidth = TotalSpace;
    
	if(ResultCount)
		*ResultCount = CurrentCount;
}

internal float GetTotalStringWidth(const wchar_t* Text,float Scale)
{
	int StubCount = 0;
	float Width = 0;
	GetStringWidth(-1, (wchar_t*)Text, &AppData->DefaultFont, 0, StringSpaceScaler, Scale,&StubCount,&Width);
    
	return Width;
}

internal float GetTotalStringWidth(char * Text,float Scale)
{
    int TextLength = strlen(Text);
    wchar_t * TempText = (wchar_t *)AllocateFrame(wchar_t , TextLength + 2);
    
    for(int CharIndex = 0 ; CharIndex < TextLength ; CharIndex++)
    {
        TempText[CharIndex] = Text[CharIndex];
    }
    
    TempText[TextLength] = '\0';
    
	return GetTotalStringWidth((const wchar_t *)TempText, Scale);
}

//the sdf bitmap allocate its own memory and free everytime
//stop it?
internal void DrawTextWithFont(_D_Font_Info* FontInfo, _Rect Rect , Color TextColor, wchar_t* String, float Scale, float SpaceSizeOffset, float SpaceScaler, bool Centre , Vector3 Up , Vector3 Right)
{
	if (!String)
		return;
    
	int CodepointsCount = 0;
    
	wchar_t* CodepointPointer = (wchar_t*)String;
    
	float TotalSpace = 0;
	GetStringWidth(-1, CodepointPointer, FontInfo, SpaceSizeOffset, SpaceScaler, Scale , 0 , &TotalSpace);
    
	float CentreSpace = TotalSpace * 0.5f;
	TotalSpace = 0;
    
	CodepointPointer = (wchar_t*)String;
    
	while (1)
	{
		wchar_t CurrentCodepoint = *CodepointPointer;
		CodepointPointer++;
        
		if (!CurrentCodepoint)
			break;
        
		int Hash = CurrentCodepoint % GLYPH_INFO_HASHMAP_LENGTH;
		D_GlyphInfoNode* PreviousNode = 0;
		D_GlyphInfoNode** CurrentNodePointer = &FontInfo->GlyphHashmap[Hash];
        
		while ((*CurrentNodePointer))
		{
			PreviousNode = *CurrentNodePointer;
            
			if ((*CurrentNodePointer)->Info.value != CurrentCodepoint)
			{
				CurrentNodePointer = &(*CurrentNodePointer)->NextNode;
			}
			else
			{
				break;
			}
		}
        
#if true
		D_GlyphInfo* CurrentGlyphInfo = &(*CurrentNodePointer)->Info;
		D_Rectangle GlyphRect = CurrentGlyphInfo->GlyphRect;
        
		if (CurrentGlyphInfo->value != CurrentCodepoint)
			ASSERT;
        
		//There is over draw the left and bottom
		//although it is transparent 
		//i wonder what it would be
        
		//it was right here
		//is the padding was wrong?
        
		D_Rectangle ScrCoord =
		{
			GlyphRect.x - FontInfo->GlyphAltasPadding,
			GlyphRect.y - FontInfo->GlyphAltasPadding,
			GlyphRect.width + 2.0*FontInfo->GlyphAltasPadding,
			GlyphRect.height + 2.0*FontInfo->GlyphAltasPadding
		};
        
		float TexCoordLeft = ScrCoord.x / FontInfo->GlyphAtlasTexture.width;
		float TexCoordBottom = ScrCoord.y / FontInfo->GlyphAtlasTexture.height;
		float TexCoordRight = (ScrCoord.x + ScrCoord.width) / FontInfo->GlyphAtlasTexture.width;
		float TexCoordTop = (ScrCoord.y + ScrCoord.height) / FontInfo->GlyphAtlasTexture.height;
        
		int TexCoordIndex = 0;
		Vector2 TexCoord[4] = {};
        
		TexCoord[TexCoordIndex++] = { TexCoordRight, TexCoordBottom };        
		TexCoord[TexCoordIndex++] = { TexCoordLeft, TexCoordBottom };
		
		TexCoord[TexCoordIndex++] = { TexCoordRight, TexCoordTop };
		TexCoord[TexCoordIndex++] = { TexCoordLeft, TexCoordTop };
        
		Vector2 TextSize = {};
        
		TextSize.x = ((float)(GlyphRect.width - FontInfo->GlyphAltasPadding) * Scale)/ (float)FontInfo->GlyphResolution;
		TextSize.y = ((float)(GlyphRect.height - FontInfo->GlyphAltasPadding) * Scale)/ (float)FontInfo->GlyphResolution;
        
		Vector3 TextPosition = Vector3Scale(Right , TotalSpace);
        
		if (Centre)
		{
			TextPosition = Vector3Subtract(TextPosition , Vector3Scale(Right , CentreSpace));
		}
		
		float FontOffsetX = ((float)CurrentGlyphInfo->offsetX * Scale) / (float)FontInfo->GlyphResolution;
        
		TextPosition = Vector3Add(TextPosition , Vector3Scale( Right , FontOffsetX));
		TextPosition = Vector3Add(TextPosition , Vector3Scale( Right , TextSize.x));
        
		float FontOffsetY = ((float)CurrentGlyphInfo->offsetY * Scale) / (float)FontInfo->GlyphResolution;
        
		TextPosition = Vector3Subtract(TextPosition , Vector3Scale( Up , FontOffsetY));
		TextPosition = Vector3Subtract(TextPosition , Vector3Scale( Up , TextSize.y * 0.5f));
		TextPosition = Vector3Add(TextPosition , Vector3Scale( Up , Scale *0.5f));
        
		_Rect TextRect = Rect;
        
		TextPosition = Vector3RotateByQuaternion(TextPosition, TextRect.Rotation);
		TextRect.Position = Vector3Add(TextRect.Position ,TextPosition);
        
		TextRect.Size = TextSize;
        
		Vector4 QuadColor[4] = {};
        
		//what was i thinking?
		for (int VertexIndex = 0; VertexIndex < Vertex_Count; VertexIndex++)
		{
			QuadColor[VertexIndex].x = ((float)TextColor.r /255.0);
			QuadColor[VertexIndex].y = ((float)TextColor.g /255.0);
			QuadColor[VertexIndex].z = ((float)TextColor.b /255.0);
			QuadColor[VertexIndex].w = ((float)TextColor.a /255.0);
		}
        
		_DrawRectText(TextRect ,QuadColor, TexCoord , Up , Right);
		//DrawRectLine(TextQuad,Fade(WHITE,0.4));
        
		//the offset is broken now
		TotalSpace += GetGlyphWidth(*CurrentGlyphInfo , FontInfo , SpaceSizeOffset , SpaceScaler , Scale);
        
#endif 
	}
}

//TODO : i want size to be fix
internal void D_DrawTextScreen(Vector3 Position ,char * Text , float Size, Color TextColor , bool Centre)
{
	_Rect Rect = GetRect();
	Rect.Position = Position;
    
	Rect.Size.y = Size;
    
	int StringLength = strlen(Text);
	wchar_t * ConvertedString = AllocateFrame(wchar_t , StringLength + 1);
	for (int CharIndex = 0; CharIndex < StringLength; CharIndex++) ConvertedString[CharIndex] = Text [CharIndex];
    
	Vector3 Right = {ViewMatrix.m0 , ViewMatrix.m4 , ViewMatrix.m8 };
	Vector3 Up = { ViewMatrix.m1 , ViewMatrix.m5 , ViewMatrix.m9 };
    
	//this isn't correct at all
	//Up = Vector3Scale(Up , RemovePrespective(Up));
	//Right = Vector3Scale(Right, RemovePrespective(Right));
	
	DrawTextWithFont(&AppData->DefaultFont, Rect , TextColor, (wchar_t*)ConvertedString , Size, 0 , StringSpaceScaler , Centre , Up , Right);
}

internal void D_DrawText(_Rect Rect , const wchar_t* String, Color TextColor, float Scale, float SpaceSizeOffset, float SpaceScaler, bool Centre)
{
	DrawTextWithFont(&AppData->DefaultFont, Rect , TextColor, (wchar_t*)String, Scale, SpaceSizeOffset, SpaceScaler, Centre , {0,1,0} , {1,0,0});
}

internal void D_DrawText(_Rect Rect, const wchar_t* String, Color TextColor, bool Centre)
{
	D_DrawText(Rect,String,  TextColor, Rect.Size.y , 0 , StringSpaceScaler, Centre);
}

internal void D_DrawText(Vector2 Position,const wchar_t* String, Color TextColor, float Size, bool Centre)
{
    
	_Rect Rect= GetRect();
	Rect.Position.x = Position.x;
	Rect.Position.y = Position.y;
    
	Rect.Size.y = Size;
    
	D_DrawText(Rect, (wchar_t*)String, TextColor, Centre);
    
}

internal void D_DrawText(Vector3 Position, const wchar_t* String, Color TextColor, float Size, bool Centre)
{
	_Rect Rect = GetRect();
	Rect.Position = Position;
    
	Rect.Size.y = Size;
    
	D_DrawText(Rect, (wchar_t*)String, TextColor, Centre);
}

internal void D_DrawText(Vector3 Position , float Size , char * String , Color TextColor , bool Centre)
{
	int StringLength = strlen(String);
	wchar_t * ConvertString = AllocateFrame(wchar_t , StringLength + 1);
	for (int CharIndex = 0; CharIndex < StringLength; CharIndex++) ConvertString[CharIndex] = String[CharIndex];
    
	D_DrawText(Position , ConvertString, TextColor , Size , Centre);
}

internal void D_DrawText(Vector2 Position , float Size , char * String , Color TextColor , bool Centre)
{
	int StringLength = strlen(String);
	wchar_t * ConvertString = AllocateFrame(wchar_t , StringLength + 1);
	for (int CharIndex = 0; CharIndex < StringLength; CharIndex++) ConvertString[CharIndex] = String[CharIndex];
    
	D_DrawText(Position , ConvertString, TextColor , Size , Centre);
}

#if 0
//TODO: make it work with char
internal void D_DrawText(_Rect Quad, const char * String,Color TextColor, float Scale, float SpaceSizeOffset, float SpaceScaler, bool Centre)
{
	wchar_t WCharTextString[128] = {};
	for (int CharIndex = 0; String[CharIndex]; CharIndex++)
		WCharTextString[CharIndex] = (int)String[CharIndex];
    
	D_DrawText(Quad , TextColor, WCharTextString, 0.6 * AppData->WindowTextSize, 0, StringSpaceScaler, false);
}
#endif

//TODO : it still terrible
global wchar_t * ModifyingString = 0;
global float * ModifyingFloat = 0;
global int * ModifyingInt = 0;

global float FloatResult = 0;
global bool FloatValueGet = false;

internal void SimpleFloatInput(wchar_t * TargetString , float *TargetFloat)
{
    ModifyingString = TargetString;
    ModifyingFloat = TargetFloat;
    swprintf(ModifyingString , L"%.4f",*ModifyingFloat);
}

internal void GetFloatInput(float *TargetFloat)
{
	if (FloatValueGet)
	{
        
		if (ModifyingFloat == TargetFloat)
		{
			(*TargetFloat) = FloatResult;
			FloatValueGet = false;
            
			ModifyingFloat = 0;
			ModifyingString = 0;
            
		}
        
	}
}

//don't like the string ):
#define DrawMenuFloatInput(MenuPointer , FormatString , TargetFloatPointer) \
{ \
localPersist wchar_t _DummString[64] = {}; \
if (ModifyingString != _DummString)\
{ \
swprintf(_DummString , FormatString , *(TargetFloatPointer)); \
}\
\
if (DrawMenuButton(MenuPointer , _DummString))\
{ \
SimpleFloatInput(_DummString, TargetFloatPointer); \
}\
GetFloatInput(TargetFloatPointer); \
}

internal void SimpleIntInput(wchar_t * TargetString , int * TargetInt)
{
    ModifyingString = TargetString;
    ModifyingInt = TargetInt;
    swprintf(ModifyingString , L"%d",*ModifyingInt);
}

//i should put a bracket around it
#define DrawMenuIntInput(MenuPointer ,   FormatString , TargetIntPointer) \
{\
localPersist wchar_t _DummyString[64] = {}; \
if (ModifyingString != _DummyString)\
{ \
swprintf(_DummyString , FormatString , *(TargetIntPointer)); \
}\
\
if (DrawMenuButton(MenuPointer , _DummyString))\
{ \
SimpleIntInput(_DummyString, TargetIntPointer); \
} \
}

internal void UpdateFloatInput()
{
    bool FinishTyping = false;
    
    if(ModifyingString)
    {
        int StringEnd = 0;
        
        for(StringEnd = 0 ; (ModifyingString[StringEnd]) && (ModifyingString[StringEnd] != L'<') ; StringEnd++);
        
        if(KeyPressed(KEY_BACKSPACE))
        {
            ModifyingString[StringEnd] = 0;
            StringEnd--;
        }
        
        ModifyingString[StringEnd] = L'<';
        ModifyingString[StringEnd+1] = 0;
        
        bool BreakByUser = false;
        
        if(MouseButtonPressed(MOUSE_BUTTON_LEFT)) BreakByUser = true;
        if(MouseButtonPressed(MOUSE_BUTTON_RIGHT)) BreakByUser = true;
        if(KeyPressed(KEY_ESCAPE)) BreakByUser = true;
        if(KeyPressed(KEY_ENTER)) BreakByUser = true;
        
        if(BreakByUser)
        {
            FinishTyping = true;
            ModifyingString[StringEnd] = 0;
        }
        
        for(int NewChar = 0;NewChar = CharPressed() ; )
        {
            ModifyingString[StringEnd++] = NewChar;
        }
    }
    
    if(FinishTyping)
    {
        if(ModifyingFloat)
        {
            swscanf(ModifyingString,L"%f",ModifyingFloat);
            
			FloatValueGet = true;
			FloatResult = (*ModifyingFloat);
        }
        else if(ModifyingInt)
        {
            swscanf(ModifyingString,L"%d",ModifyingInt);
            
            ModifyingInt =0;
            ModifyingString = 0;
        }
        else
        {
            ASSERT;
        }
    }
}