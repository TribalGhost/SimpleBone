
static int hash[] = { 208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
    185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
    9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
    70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
    203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
    164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
    228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
    232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
    193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
    101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
    135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
    114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219 };


#define PNoise2(x, y,SEED) hash[(hash[(y + SEED) % 256] + x) % 256];

#define PLinInter(x,y,s) x + s * (y - x)

#define SmoothInter(x, y, s) PLinInter(x, y, s * s * (3 - 2 * s))

internal _Rect QuadPositionLeft(_Rect Quad, float PositionOffsetX, float SizeX, float SizeOffsetY)
{
	Quad.Position.x = ((PositionOffsetX + SizeX / 2) / AppData->WindowSize.x)* AppData->Right *2 - AppData->Right;
	Quad.Position.y = 0;
    
	Quad.Size.x = (SizeX / AppData->WindowSize.x) * AppData->Right*2;
	Quad.Size.y = AppData->Top * 2 - ((SizeOffsetY / AppData->WindowSize.y) * AppData->Top *2);
    
	return Quad;
}

internal _Rect QuadPositionLeft(float PositionOffsetX, float SizeX, float SizeOffsetY)
{
	return QuadPositionLeft(GetRect(), PositionOffsetX, SizeX, SizeOffsetY);
}

internal _Rect OffsetFromBottomLeft(float PixelOffsetX, float PixelOffsetY, float PixelSizeX,float PixelSizeY)
{
	_Rect Quad = GetRect();
    
	Quad.Position.x= ((PixelOffsetX + PixelSizeX / 2) / AppData->WindowSize.x) * AppData->Right * 2 - AppData->Right;
	Quad.Position.y= ((PixelOffsetY + PixelSizeY / 2) / AppData->WindowSize.y) * AppData->Top * 2 - AppData->Top ;
    
	Quad.Size.x = (PixelSizeX / AppData->WindowSize.x) * AppData->Right * 2;
	Quad.Size.y = (PixelSizeY / AppData->WindowSize.y) * AppData->Top * 2;
    
	return Quad;
}

internal _Rect OffsetFromQuadTopLeft(_Rect ParentQuad,_Rect ChildQuad, float PixelOffsetX,float PixelOffsetY)
{
	ChildQuad.Position = ParentQuad.Position;
	ChildQuad.Position.x -= ParentQuad.Size.x/2;
	ChildQuad.Position.y += ParentQuad.Size.y/2;
    
	ChildQuad.Position.x += ChildQuad.Size.x / 2;
	ChildQuad.Position.y -= ChildQuad.Size.y / 2;
    
	Vector2 ChildQuadOffset = PixelToSize(PixelOffsetX, PixelOffsetY);
    
	ChildQuad.Position.x += ChildQuadOffset.x;
	ChildQuad.Position.y -= ChildQuadOffset.y;
    
	return ChildQuad;
}

internal _Rect OffsetFromTopLeft(_Rect Quad , float PixelOffsetX,float PixelOffsetY)
{
	Vector2 WorldPosition = PixelToWorld({ 0, 0 });
	Quad.Position.x = WorldPosition.x + Quad.Size.x * 0.5f + PixelToWidth(PixelOffsetX);
	Quad.Position.y = WorldPosition.y - Quad.Size.y * 0.5f + PixelToHeight(PixelOffsetY);
    
	return Quad;
}

internal _Rect QuadToTopLeft(_Rect Quad)
{
	Quad.Position.x -= Quad.Size.x * 0.5f;
	Quad.Position.y += Quad.Size.y * 0.5f;
    
	return Quad;
}

internal _Rect QuadToBottomLeft(_Rect Quad)
{
	Quad.Position.x -= Quad.Size.x * 0.5f;
	Quad.Position.y -= Quad.Size.y * 0.5f;
    
	return Quad;
}

internal _Rect QuadMoveDownHalf(_Rect Quad)
{
	Quad.Position.y -= Quad.Size.y * 0.5f;
	return Quad;
}

internal Vector2 GetRectTopRightCorner(_Rect Quad)
{
	return { Quad.Position.x + Quad.Size.x * 0.5f, Quad.Position.y + Quad.Size.y * 0.5f };
}

internal Vector2 GetRectBottomLeftCorner(_Rect Quad)
{
	return { Quad.Position.x - Quad.Size.x * 0.5f, Quad.Position.y - Quad.Size.y * 0.5f };
}

internal _Quad ResizeBlock(_Quad Block,float Size)
{
	Vector3 Centre = {};
    
	int VertexOrder[Vertex_Count] = {};
	VertexOrder[0] = Vertex_TopLeft;
	VertexOrder[1] = Vertex_TopRight;
	VertexOrder[2] = Vertex_BottomRight;
	VertexOrder[3] = Vertex_BottomLeft;
    
	for (int OrderIndex = 0; OrderIndex < 4; OrderIndex++)
	{
		int StartVertexIndex = VertexOrder[OrderIndex];
		int EndVertexIndex = VertexOrder[0];
        
		if ((OrderIndex+1) < 3)
		{
			EndVertexIndex = VertexOrder[OrderIndex + 1];
		}
        
		Vector3 Start = Block.VertexPosition[StartVertexIndex];
		Vector3 End = Block.VertexPosition[StartVertexIndex];
        
		Vector3 EdgeDirection = Vector3Subtract(Start, End);
		Vector3 EdgeNormal = { -EdgeDirection.y,EdgeDirection.x,0 };
	}
    
	for (int VertexIndex = 0; VertexIndex < Vertex_Count; VertexIndex++)
	{
		Centre.x += Block.VertexPosition[VertexIndex].x;
		Centre.y += Block.VertexPosition[VertexIndex].y;
		Centre.z += Block.VertexPosition[VertexIndex].z;
	}
    
	Centre.x /= Vertex_Count;
	Centre.y /= Vertex_Count;
	Centre.z /= Vertex_Count;
    
	for (int VertexIndex = 0; VertexIndex < Vertex_Count; VertexIndex++)
	{
		Vector3 Offset = Vector3Subtract(Block.VertexPosition[VertexIndex], Centre);
		Offset = Vector3Scale(Offset, Size );
        
		Offset = Vector3Add(Offset, Centre);
        
		Block.VertexPosition[VertexIndex] = Offset;
	}
    
	return Block;
}

internal _Quad ResizeBlock(_Quad * Block, float Size)
{
	return ResizeBlock(*Block, Size);
}

internal bool CheckCollisionRect(_Rect Rect , Vector2 Point)
{
	Vector2 TopRight = GetRectTopRightCorner(Rect);
	Vector2 BottomLeft = GetRectBottomLeftCorner(Rect);
    
	if (Point.x < TopRight.x && Point.y < TopRight.y)
	{
		if (Point.x > BottomLeft.x && Point.y > BottomLeft.y)
		{
			return true;
		}
	}
    
	return false;
}

internal bool CheckCollisionRectMouse(_Rect Rect)
{
    return CheckCollisionRect( Rect , PixelToWorld(AppData->MousePosition));
}

internal bool CheckCollisionRectMouse(Vector2 TopRight , Vector2 BottomLeft)
{
    Vector2 CurrentMousePosition = PixelToWorld(AppData->MousePosition);
    
	if (CurrentMousePosition.x < TopRight.x && CurrentMousePosition.y < TopRight.y)
	{
		if (CurrentMousePosition.x > BottomLeft.x && CurrentMousePosition.y > BottomLeft.y)
		{
			return true;
		}
	}
    
	return false;
}

internal Vector3 GetEdgeDirection(Vector3 Start,Vector3 End , Vector3 Point)
{
	Vector3 Edge = Vector3Subtract(End, Start);
	
	Vector3 EdgePoint = Vector3Subtract(Point, Start);
    
	float Inside = Vector3CrossProduct(Edge, EdgePoint).z;
    
	float DX = Start.x - End.x;
	float DY = Start.y - End.y;
    
	Vector3 EdgeNormal_Inside = { DY,-DX,Inside  };
    
	return EdgeNormal_Inside;
}

internal bool CheckCollisionQuadPoint(_Quad Quad, Vector3 Point)
{
	enum
	{
		EdgeTop,
		EdgeRight,
		EdgeBottom,
		EdgeLeft,
        
		EdgeCount
	};
    
	//rename this
	Vector3 EdgeNormal_Inside_Distance[EdgeCount] = {};
    
	EdgeNormal_Inside_Distance[EdgeTop] = GetEdgeDirection(Quad.VertexPosition[Vertex_TopLeft], Quad.VertexPosition[Vertex_TopRight], Point);
	EdgeNormal_Inside_Distance[EdgeRight] = GetEdgeDirection(Quad.VertexPosition[Vertex_TopRight], Quad.VertexPosition[Vertex_BottomRight], Point);
	EdgeNormal_Inside_Distance[EdgeBottom] = GetEdgeDirection(Quad.VertexPosition[Vertex_BottomRight], Quad.VertexPosition[Vertex_BottomLeft], Point);
	EdgeNormal_Inside_Distance[EdgeLeft] = GetEdgeDirection(Quad.VertexPosition[Vertex_BottomLeft], Quad.VertexPosition[Vertex_TopLeft], Point);
    
	for (int EdgeIndex = 0; EdgeIndex < EdgeCount; EdgeIndex++)
	{
		Vector3 CurrentEdgeNormal_Inside = EdgeNormal_Inside_Distance[EdgeIndex];
        
		if (CurrentEdgeNormal_Inside.z > 0)
		{
			return false;
		}
	}
    
	return true;
}

//TODO : this is bad
internal bool CheckCollisionQuadToRect(_Quad Quad , _Rect Rect)
{
	Vector3 RectVertices[Vertex_Count] = {};
    
	RectVertices[Vertex_TopLeft].x -= Rect.Size.x * 0.5f;	
	RectVertices[Vertex_TopLeft].y += Rect.Size.y * 0.5f;
	
	RectVertices[Vertex_TopRight].x += Rect.Size.x * 0.5f;	
	RectVertices[Vertex_TopRight].y += Rect.Size.y * 0.5f;
	
	RectVertices[Vertex_BottomLeft].x -= Rect.Size.x * 0.5f;	
	RectVertices[Vertex_BottomLeft].y -= Rect.Size.y * 0.5f;
	
	RectVertices[Vertex_BottomRight].x += Rect.Size.x * 0.5f;	
	RectVertices[Vertex_BottomRight].y -= Rect.Size.y * 0.5f;
    
	for (int VertexIndex = 0 ; VertexIndex < Vertex_Count ; VertexIndex++)
	{
		RectVertices[VertexIndex] = Vector3RotateByQuaternion(RectVertices[VertexIndex] , Rect.Rotation);
        
		RectVertices[VertexIndex].x += Rect.Position.x;
		RectVertices[VertexIndex].y += Rect.Position.y;
        
		if (CheckCollisionQuadPoint(Quad, RectVertices[VertexIndex]))
		{
			return true;
		}
	}
    
	return false;
}

internal bool CheckCollisionQuadMouse(_Quad Block)
{
	Vector2 MouseWorldPosition = PixelToWorld(AppData->MousePosition);
	return CheckCollisionQuadPoint(Block, { MouseWorldPosition.x,MouseWorldPosition.y,0 });
}

internal Vector4 SelectionFromStartToEndToRect(Vector2 StartPosition, Vector2 End)
{
	Vector2 DragCentre = {};
	DragCentre.x = (StartPosition.x + End.x) / 2;
	DragCentre.y = (StartPosition.y + End.y) / 2;
    
	Vector2 DragSize = {};
    
	DragSize.x = StartPosition.x - End.x;
	if (DragSize.x < 0)
		DragSize.x *= -1;
    
	DragSize.y = StartPosition.y - End.y;
	if (DragSize.y < 0)
		DragSize.y *= -1;
    
	return { DragCentre.x,DragCentre.y,DragSize.x,DragSize.y };
}

//this is dumb
internal Vector2 GetMenuIndividualItemPositionEnd(Vector2 Position, String* OptionString, bool OnScreen,int Index,int OffsetY)
{
	if(OnScreen) Position = PixelToWorld(Position);
    
	_Rect StartingPointQuad = GetRect();
    
	float PixelSize = 20;
    
	float FontSize = 0;
    
	if (OnScreen)
		FontSize = PixelToHeight(PixelSize);
	else
		FontSize = PixelToHeight(PixelSize * (1 / CameraCurrentZoom));
    
	StartingPointQuad.Position.x = Position.x;
	StartingPointQuad.Position.y = Position.y;
    
	_Rect MenuOptionQuad = StartingPointQuad;
	MenuOptionQuad.Position.y -= Index * FontSize;
	MenuOptionQuad.Position.y -= FontSize * 0.5f;
	
	if (OnScreen)
        MenuOptionQuad.Position.x += PixelToWidth(PixelSize / 2);
    else 
        MenuOptionQuad.Position.x += PixelToWidth(PixelSize / 2) * (1/ CameraCurrentZoom);
    
	const wchar_t* String = OptionString[Index].Start;
    
	Vector2 OptionEndPosition = {};
    
	if (String)
	{
		float StringWidth = GetTotalStringWidth(String, FontSize);
        
		MenuOptionQuad.Size.x = StringWidth;
		MenuOptionQuad.Size.y = FontSize;
        
		_Rect OperationMenuTextQuad = MenuOptionQuad;
        
		MenuOptionQuad.Position.x += MenuOptionQuad.Size.x * 0.5f;
        
		if (OnScreen)
		{
			MenuOptionQuad.Size.x += PixelToWidth(PixelSize);
		}
		else
		{
			MenuOptionQuad.Size.x += PixelToWidth(PixelSize) * (1 / CameraCurrentZoom );
		}
        
		OptionEndPosition.x += MenuOptionQuad.Position.x;
		OptionEndPosition.y += MenuOptionQuad.Position.y;
        
		OptionEndPosition.x += MenuOptionQuad.Size.x * 0.5f;
		OptionEndPosition.y += MenuOptionQuad.Size.y * 0.5f;
        
		OptionEndPosition.y -= FontSize * OffsetY;
	}
    
	return OptionEndPosition;
}

internal Vector2 GetMenuIndividualItemPositionEnd(Vector2 Position, String* OptionString, bool OnScreen, int Index)
{
	return GetMenuIndividualItemPositionEnd(Position, OptionString, OnScreen, Index,0);
}

global float FontPixelSize = 20;

internal DrawingMenu StartDrawMenu(Vector2 Position , bool OnScreen , GameMenuType MenuType)
{
    
	DrawingMenu Menu = {};
	Menu.CurrentButtonPosition = Position;
	Menu.OnScreen = OnScreen;
	Menu.MenuType = MenuType;
    
    return Menu;
}

internal DrawingMenu StartDrawDraggingMenu(Vector2 * PixelPosition , GameMenuType MenuType)
{
    
    _Rect DragQuad = GetRect(90, 20);
	DragQuad.Position.x = PixelToWorld(*PixelPosition).x;
	DragQuad.Position.y = PixelToWorld(*PixelPosition).y;
    
	DragQuad.Position.y += DragQuad.Size.y * 0.8;
	DragQuad.Position.x += DragQuad.Size.x * 0.6;
    
	if (CheckCollisionRectMouse(DragQuad))
	{
        
		DrawRect(DragQuad, DragQuad.Size.y * 0.3f, Fade(WHITE, 0.6));
        
		if (MouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			ModifyingMenuPosition = PixelPosition;
		}
	}
	else
	{
		DrawRect(DragQuad, DragQuad.Size.y * 0.3f, Fade(WHITE, 0.4));
	}
    
	if (ModifyingMenuPosition == PixelPosition)
	{
		MenuInteruped = true;
        
		Vector2 MouseDelta = AppData->MousePosition;
		MouseDelta.x -= Editor->PreviousMousePosition.x;
		MouseDelta.y -= Editor->PreviousMousePosition.y;
        
		(*PixelPosition).x += MouseDelta.x;
		(*PixelPosition).y += MouseDelta.y;
        
		DrawRect(DragQuad, 10, Fade(WHITE, 0.8));
        
		if (MouseButtonReleased(MOUSE_LEFT_BUTTON))
		{
			ModifyingMenuPosition = 0;
		}
	}
    
	_Rect TextQuad = DragQuad;
	TextQuad.Position.x -= TextQuad.Size.x * 0.5f;
	TextQuad.Position.x += PixelToWidth(5);
	D_DrawText(TextQuad, L"按住拖动", DARKBLUE, false);
    
    Vector2 DraggingPosition ={DragQuad.Position.x, DragQuad.Position.y}; 
    DraggingPosition.y -= DragQuad.Size.y * 0.5f;
    DraggingPosition.x -= DragQuad.Size.y * 0.5f;
    
	DrawingMenu Menu = {};
	Menu.CurrentButtonPosition = DraggingPosition;
	Menu.OnScreen = true;
	Menu.MenuType = MenuType;
    
    return Menu;
}

internal DrawingMenu StartDrawMenuMouse()
{
	DrawingMenu Menu = {};
	Menu.CurrentButtonPosition = Editor->OperateMenuPosition;
	Menu.OnScreen = false;
	Menu.MenuType = GMT_Descend;
    
    return Menu;
}

internal bool DrawMenuButton(DrawingMenu * Menu, wchar_t * ButtonString , Color ButtonTextColor , bool ChangeButtonTextColor)
{
    
    Menu->ButtonHover = false;
    Menu->ButtonClicked = false;
    
    _Rect ButtonRect = GetRect();
    ButtonRect.Position.x = Menu->CurrentButtonPosition.x;
    ButtonRect.Position.y = Menu->CurrentButtonPosition.y;
    
	float FontSize = 0;
    
    //this "OnScreen" is absurd
	if(Menu->OnScreen)
		FontSize = PixelToHeight(FontPixelSize);
	else
		FontSize = PixelToHeight(FontPixelSize * (1 / CameraCurrentZoom));
    
    float ButtonOffsetY = FontSize;
    
    ButtonRect.Position.x += PixelToWidth(FontPixelSize / 2);
	ButtonRect.Position.y -= ButtonOffsetY;
    
    if (ButtonString)
    {
        
        float StringWidth = GetTotalStringWidth(ButtonString, FontSize);
        
        ButtonRect.Size.x = StringWidth / 2;
        ButtonRect.Size.y = FontSize;
        
        ButtonRect.Position.x += ButtonRect.Size.x * 0.5f;
        ButtonRect.Size.x += FontSize * 0.4f;
        
        _Rect ButtonTextRect = ButtonRect;
        
        if (Menu->OnScreen)
        {
            ButtonTextRect.Size.x += PixelToWidth(FontPixelSize / 2) ;
        }
        else
        {
            ButtonTextRect.Size.x += PixelToWidth(FontPixelSize / 2) * (1 / CameraCurrentZoom);
        }
        
        Color TextColor = Fade(WHITE, 0.8);
        
        if (CheckCollisionRectMouse(ButtonRect))
        {
			MenuInteruped = true;
            if (MouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                Menu->ButtonClicked = true;
            }
            
            Menu->ButtonHover = true;
            TextColor = Fade(YELLOW,0.8);
        }
        
        if(ChangeButtonTextColor)
        {
            TextColor = ButtonTextColor;
        }
        
        Menu->CurrentButtonLeft = ButtonRect.Position.x;
        Menu->CurrentButtonLeft += ButtonRect.Size.x * 0.5;
        
        DrawRect(ButtonRect, ButtonRect.Size.y*0.3, Fade(BLACK, 0.7f));
		ButtonTextRect.Size.y *= 0.5f;
        D_DrawText(ButtonTextRect, ButtonString, TextColor, true);
    }
    
	if (Menu->MenuType == GMT_Descend)
	{
		Menu->CurrentButtonPosition.y -= ButtonOffsetY;		
		Menu->CurrentButtonPosition.y -= Menu->ButtonExtraOffset;
	}
	else if (Menu->MenuType == GMT_ToTheRight)
	{
		Menu->CurrentButtonPosition.x += ButtonRect.Size.x;
		Menu->CurrentButtonPosition.x += Menu->ButtonExtraOffset;
	}
    
    return Menu->ButtonClicked;
}

internal bool DrawMenuButton(DrawingMenu * Menu, char * ButtonString , Color ButtonTextColor , bool ChangeButtonTextColor)
{
	int StringLength = strlen(ButtonString);
	wchar_t * NewString = AllocateFrame(wchar_t , StringLength + 1);
	for(int i = 0 ; i < StringLength ; i++) NewString[i] = ButtonString[i];
    
	return DrawMenuButton(Menu , NewString ,ButtonTextColor , ChangeButtonTextColor);
}

internal bool DrawMenuButton(DrawingMenu * Menu, wchar_t * ButtonString )
{
    return DrawMenuButton(Menu,ButtonString , {} , false);
}

internal bool DrawMenuButton(DrawingMenu * Menu, char * ButtonString)
{
	int StringLength = strlen(ButtonString);
	wchar_t * NewString = AllocateFrame(wchar_t , StringLength + 1);
	for(int i = 0 ; i < StringLength ; i++) NewString[i] = ButtonString[i];
    
	return DrawMenuButton(Menu , NewString);
}

//why are you broken??
internal bool RayLineSegmentIntersection_Example(Vector3 o, Vector3 d, Vector3 a, Vector3 b)
{
	Vector3 ortho = { -d.y, d.x };
	Vector3 aToO = Vector3Subtract(o , a);
	Vector3 aToB = Vector3Subtract(b , a);
    
	float denom = Vector3DotProduct(aToB, ortho);
    
	// Here would be a good time to see if denom is zero in which case the line segment and
	// the ray are parallel.
    
	// The length of this cross product can also be written as abs( aToB.x * aToO.y - aToO.x * aToB.y ).
	float t1 = Vector3Length(Vector3CrossProduct(aToB, aToO)) / denom;
	float t2 = Vector3DotProduct(aToO, ortho) / denom;
    
	return t2 >= 0 && t2 <= 1 && t1 >= 0;
}

//this is how normal people solve it
internal Vector2 RayLineSegmentIntersection(Vector3 O, Vector3 D, Vector3 A, Vector3 B)
{
	float t1 = (O.x - A.x) * (B.y - A.y) - (O.y - A.y) * (B.x - A.x);
	t1 /= (D.y * (B.x - A.x) - D.x * (B.y - A.y));
    
	float t2 = (A.x - O.x) * D.y - (A.y - O.y) * D.x;
	t2 /= (B.y - A.y) * D.x - (B.x - A.x) * D.y;
    
	return { t1 , t2 };
}

//this is how 200+ iq people solve it :)
internal Vector2 RayLineSegmentIntersection_(Vector3 O, Vector3 D, Vector3 A, Vector3 B)
{
	Vector3 N1 = Vector3Subtract(B, A);
	N1 = { -N1.y, N1.x };
    
	float t1 = Vector3DotProduct(Vector3Subtract(A, O), N1);
    
	float t1Bottom = Vector3DotProduct(D, N1);
	if (t1Bottom == 0)
	{
		t1 = -1;
	}
	else
	{
		t1 /= t1Bottom;
	}
    
	Vector3 N2 = { -D.y , D.x };
    
	float t2 = Vector3DotProduct(Vector3Subtract(O, A), N2);
	float t2Bottom = Vector3DotProduct(Vector3Subtract(B, A), N2);
	
	if (t2Bottom == 0)
	{
		t2 = -1;
	}
	else
	{
		t2 /= t2Bottom;
	}
    
#if 0
	wchar_t DebugString[128] = {};
	swprintf(DebugString, L"t1:%.2f | t2:%.2f | rate: %.2f", t1 , t2 , 1.0 / (float)D_UPDATE_RATE);
	D_DrawText(Vector3Lerp(A, B, 0.5f), 0.06, DebugString, Fade(WHITE, 0.5), true);
#endif
    
	return { t1 , t2 };
}

//TODO : rename all of these
internal bool CheckCollisionRect(_Rect RectA, _Rect RectB)
{
	Vector2 QuadBTopRight = GetRectTopRightCorner(RectB);
	Vector2 QuadBBottomLeft = GetRectBottomLeftCorner(RectB);
	Vector2 QuadATopRight = GetRectTopRightCorner(RectA);
	Vector2 QuadABottomLeft = GetRectBottomLeftCorner(RectA);
    
	bool XIntersect = false;
    
	if (QuadBBottomLeft.x < QuadABottomLeft.x)
	{
		if (QuadBTopRight.x > QuadABottomLeft.x)
		{
			XIntersect = true;
		}
	}
	else
	{
		if (QuadATopRight.x > QuadBBottomLeft.x)
		{
			XIntersect = true;
		}
	}
    
	if (XIntersect)
	{
		if (QuadBBottomLeft.y < QuadABottomLeft.y)
		{
			if (QuadBTopRight.y > QuadABottomLeft.y )
			{
				return true;
			}
		}
		else
		{
			if (QuadATopRight.y > QuadBBottomLeft.y)
			{
				return true;
			}
		}
	}
    
	return false;
}

//stolen
internal unsigned int StringToHash(wchar_t *s)
{
    unsigned int hash = 0;
    
    for(int CurrentCount = 0; *s; ++s)
    {
        hash += *s;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    
    return hash;
}

//stolen
internal int HashInt(int key)
{
	key = ~key + (key << 15); // key = (key << 15) - key - 1;
	key = key ^ (key >> 12);
	key = key + (key << 2);
	key = key ^ (key >> 4);
	key = key * 2057; // key = (key + (key << 3)) + (key << 11);
	key = key ^ (key >> 16);
	return key;
}

#define SlotToData(Slot , HashTable) (HashTable.DataArray + (Slot - HashTable.HashSlotArray))
#define DataToSlot(Data , HashTable) ( HashTable.HashSlotArray (Data - HashTable.DataArray))

#define PrintHashTable(HashTable) _PrintHashTable(HashTable.HashSlotArray , HashTable.HashSlotTail)

internal void _PrintHashTable(_HashTableSlot persist * HashSlotArray , int Count)
{
    
	printf("\n");	
	for (_HashTableSlot persist * CurrentSlot = HashSlotArray; CurrentSlot < HashSlotArray + Count; CurrentSlot++)
	{
		printf( "%d " , CurrentSlot - HashSlotArray );
        
		if (!CurrentSlot->Valid)
		{
			printf( "Emty\n" );
		}
		else
		{
			printf( "(Slot : %d)\n" , CurrentSlot->SlotValue);
		}
	}
    
	printf("\n");
}

//this look bad

//this is really really bad
//but it is working and quite robust so meh

internal void AddToHashTable(int HashValue , int DataIndex , _HashTable * HashTable , int Count)
{
	
	_HashTableSlot * SlotArray = (_HashTableSlot *)HashTable->SlotArray;
    _HashTableEntry * EntryArray = (_HashTableEntry *)HashTable->EntryArray;
    
    _HashTableEntry * Entry = EntryArray + (HashValue % Count);
    
	_HashTableSlot * EmtySlot = 0;
    
	bool StartedFromChain = false;
    
	if (Entry->HeadIndex == -1)
	{
		EmtySlot = SlotArray + (HashValue % Count);
	}
	else
	{
		EmtySlot = SlotArray + Entry->TailIndex;
	}
    
	if(EmtySlot != (SlotArray + Entry->HeadIndex) ) StartedFromChain = true;
    
	for ( ; EmtySlot->Valid ; EmtySlot++)
	{
		if (EmtySlot >= (SlotArray + Count)) EmtySlot = SlotArray;
		
		if (EmtySlot == (SlotArray + Entry->HeadIndex))
		{
			if (StartedFromChain)
			{
				ASSERT;
			}
			else
			{
				StartedFromChain = true;
			}
            
		}
	}
    
	int EmtySlotIndex = (EmtySlot - SlotArray);
    
	if (Entry->HeadIndex == -1)
	{
		Entry->HeadIndex = EmtySlotIndex;
	}
    
    if(Entry->TailIndex != -1)
    {
        SlotArray[Entry->TailIndex].NextIndex = EmtySlotIndex;
    }
    
	EmtySlot->PreviousIndex = Entry->TailIndex;
	Entry->TailIndex = EmtySlotIndex;
    
	EmtySlot->Valid = true;
	EmtySlot->SlotValue = (HashValue % Count);
	EmtySlot->DataIndex = DataIndex;
	EmtySlot->NextIndex = -1;
}

#if 0

internal void _AddToHashTable(int HashValue , int DataIndex, _HashTable * HashTable , int Count)
{
	int SlotValue = HashValue % Count;
	
	_HashTableSlot persist * SlotChainHead = HashTable->SlotArray + SlotValue;
	_HashTableSlot persist * EmtySlot = 0;
    
	if (!SlotChainHead->Valid)
	{
		int SelfIndex = SlotChainHead - HashTable->SlotArray;
        
		if (SlotChainHead->ChainExist)
		{
			_HashTableSlot persist * ChainNextSlot = HashTable->SlotArray + SlotChainHead->ChainHeadIndex;
			
			SlotChainHead->ChainHeadIndex = SlotChainHead - HashTable->SlotArray;
            
			ChainNextSlot->PreviousIndex = SlotChainHead - HashTable->SlotArray;
			
			SlotChainHead->NextIndex = ChainNextSlot - HashTable->SlotArray;
			SlotChainHead->PreviousIndex = -1;
            
		}
		else
		{
			SlotChainHead->ChainHeadIndex = SelfIndex;
			SlotChainHead->ChainTailIndex = SelfIndex;
            
			SlotChainHead->NextIndex = -1;
			SlotChainHead->PreviousIndex = -1;
		}
        
		SlotChainHead->ChainExist = true;
        
		EmtySlot = SlotChainHead;
        
		EmtySlot->Valid = true;
		EmtySlot->SlotValue = SlotValue;
        
	}
	else
	{
		_HashTableSlot persist * LastSlot = 0;
        
		if (!SlotChainHead->ChainExist)
		{
			LastSlot = SlotChainHead;
            
#if HASH_DEBUG
			printf("slot %d doesn't have chain but already occupied\n", SlotValue);
#endif
            
		}
		else
		{
			LastSlot = HashTable->SlotArray + SlotChainHead->ChainTailIndex;
#if HASH_DEBUG
			printf("slot %d start from %d \n", SlotValue, SlotChainHead->ChainTailIndex );
#endif
            
		}
        
		EmtySlot = LastSlot;
        
		for (;;)
		{
            
			EmtySlot++;
			if (EmtySlot >= HashTable->SlotArray + Count ) EmtySlot = HashTable->SlotArray;
			
#if HASH_DEBUG
			printf("jump to %d \n", EmtySlot - HashTable->SlotArray);
#endif
            
			if (EmtySlot == SlotChainHead) ASSERT;
            
			if (!EmtySlot->Valid) break;
		}
        
		EmtySlot->Valid = true;
		EmtySlot->SlotValue = SlotValue;
        
		int NewSlotIndex = EmtySlot - HashTable->SlotArray;
        
		if (!SlotChainHead->ChainExist)
		{
			EmtySlot->NextIndex = -1;
			EmtySlot->PreviousIndex = -1;
            
			SlotChainHead->ChainHeadIndex = NewSlotIndex;
			SlotChainHead->ChainTailIndex = NewSlotIndex;
            
			SlotChainHead->ChainExist = true;
		}
		else
		{
            
			EmtySlot->NextIndex = -1;
			EmtySlot->PreviousIndex = LastSlot - HashTable->SlotArray;
            
			LastSlot->NextIndex = EmtySlot - HashTable->SlotArray;
            
		}
        
		SlotChainHead->ChainTailIndex = EmtySlot - HashTable->SlotArray;
	}
    
	EmtySlot->DataIndex = DataIndex;
    
	int SlotIndex = EmtySlot - HashTable->SlotArray;
    
#if HASH_DEBUG
    
	printf("Adding Hash Slot :\n");
	_PrintHashTable(HashTable->SlotArray , Count);
    
#endif
}

#endif

internal void ClearHashTable(_HashTable * HashTable , int Count)
{
    _HashTableEntry * EntryArray = HashTable->EntryArray;
    _HashTableSlot * SlotArray = HashTable->SlotArray;
    
    for(int SlotIndex = 0 ; SlotIndex < Count ; SlotIndex++)
    {
        EntryArray[SlotIndex].HeadIndex = -1;
        EntryArray[SlotIndex].TailIndex = -1;
        
        SlotArray[SlotIndex].Valid = false;
        SlotArray[SlotIndex].SlotValue = -1;
        SlotArray[SlotIndex].DataIndex = -1;
        SlotArray[SlotIndex].NextIndex = -1;
        SlotArray[SlotIndex].PreviousIndex = -1;
        
    }
}

internal bool DeleteFromHashTable(int HashValue , int DataIndex , _HashTable * HashTable , int Count)
{
	_HashTableSlot * SlotArray = (_HashTableSlot * )HashTable->SlotArray;
	_HashTableSlot * SlotChain = SlotArray + (HashValue % Count);
    
    _HashTableEntry * EntryArray = HashTable->EntryArray;
    _HashTableEntry * Entry = EntryArray + (HashValue%Count);
    
	if (!Entry->HeadIndex == -1) return false;
    
	int SlotIndex = Entry->HeadIndex;
	for (; SlotIndex != -1; SlotIndex = SlotArray[SlotIndex].NextIndex)
	{
		_HashTableSlot * CurrentSlot = SlotArray + SlotIndex;
        
		if (CurrentSlot->DataIndex == DataIndex)
		{
			if (CurrentSlot->PreviousIndex != -1)
			{
				SlotArray[CurrentSlot->PreviousIndex].NextIndex = CurrentSlot->NextIndex;
			}
            
			if (CurrentSlot->NextIndex != -1)
			{
				SlotArray[CurrentSlot->NextIndex].PreviousIndex = CurrentSlot->PreviousIndex;
			}
            
			if (Entry->HeadIndex == SlotIndex)
			{
				Entry->HeadIndex = CurrentSlot->NextIndex;
			}
            
			if (Entry->TailIndex == SlotIndex)
			{
				Entry->TailIndex = CurrentSlot->PreviousIndex;
			}
            
			CurrentSlot->Valid = false;
			CurrentSlot->SlotValue = -1;
			CurrentSlot->DataIndex = -1;
			CurrentSlot->NextIndex = -1;
			CurrentSlot->PreviousIndex = -1;
            
			return true;
		}
	}
    
	return false;
}

#if 0
internal bool _DeleteFromHashTable(int HashValue , int DataIndex , _HashTable * HashTable , int Count)
{
	int SlotValue = HashValue % Count;
	_HashTableSlot * Slot = HashTable->SlotArray + SlotValue;
    
	if (!Slot->Valid) return;
    
	Slot->Valid = false;
	
	_HashTableSlot * SlotChainHead = HashTable->SlotArray + Slot->SlotValue;
    
	if (SlotChainHead->ChainTailIndex == SlotValue)
	{
		if (Slot->PreviousIndex != -1)
		{
			SlotChainHead->ChainTailIndex = Slot->PreviousIndex;
		}
		else
		{
			SlotChainHead->ChainExist = false;
            
			SlotChainHead->ChainTailIndex = -1;
			SlotChainHead->ChainHeadIndex = -1;
		}
	}
	
	if (SlotChainHead->ChainHeadIndex == SlotValue)
	{
		SlotChainHead->ChainHeadIndex = Slot->NextIndex;
	}
    
	if (Slot->PreviousIndex != -1)
	{
		HashTable->SlotArray[Slot->PreviousIndex].NextIndex = Slot->NextIndex;
	}
    
	if (Slot->NextIndex != -1)
	{
		HashTable->SlotArray[Slot->NextIndex].PreviousIndex = Slot->PreviousIndex;
	}
    
#if HASH_DEBUG
	printf("Deleting Hash Slot :\n");
	_PrintHashTable(HashTable->SlotArray , Count);
#endif
}
#endif

#if 0
#define GetSlotFromHashTable(Hash , HashTable) _GetSlotFromHashTable(Hash , HashTable.HashSlotArray , HashTable.HashSlotTail)

internal int _GetIndexFromHashTable(int HashValue , _HashTableSlot persist * HashTableSlotArray , int Count)
{
	int SlotValue = HashValue % Count;
	_HashTableSlot persist * SlotChainHead = HashTableSlotArray + SlotValue;
    
#if HASH_DEBUG
    
	printf("Serach Hash Slot :\n");
	_PrintHashTable(HashTableSlotArray , Count);
    
#endif
    
	if (!SlotChainHead->ChainExisted) return -1;
    
	return HashTableSlotArray + SlotChainHead->ChainHeadIndex;
}
#endif

#define HashTable_Iterate(DataIndex , HashValue , HashTableVar , Count) \
for \
(\
int SlotIndex = -1 , DataIndex = -1; \
_IterateHashTable((HashValue),  &SlotIndex , &DataIndex , HashTableVar , Count) ,SlotIndex != -1 ;\
)

internal void _IterateHashTable( int HashValue , int * SlotIndex , int * DataIndex, _HashTable * HashTable , int Count)
{
	
#if HASH_DEBUG
	
	printf("(Iterating : %d)(CurrentSlot : %d)\n" , (HashValue%Count) , (*SlotIndex));
	_PrintHashTable(HashTable->SlotArray , Count);
    
#endif
    
	_HashTableSlot * SlotArray = (_HashTableSlot *)HashTable->SlotArray;
	_HashTableSlot * SlotChainHead = (SlotArray + (HashValue%Count));
    _HashTableEntry * EntryArray = HashTable->EntryArray;
    _HashTableEntry * Entry = EntryArray + (HashValue%Count);
    
	if ( (*SlotIndex) == -1)
	{
		(*SlotIndex) = Entry->HeadIndex;
		(*DataIndex) = SlotArray[Entry->HeadIndex].DataIndex;
        return;
	}
    
	_HashTableSlot * Slot = SlotArray + (*SlotIndex);
    
    (*SlotIndex) = Slot->NextIndex;
    (*DataIndex) = SlotArray[Slot->NextIndex].DataIndex;
    
#if HASH_DEBUG
    
	printf("(inside the chain : %d) (tail index : %d)\n" , Slot->NextIndex,SlotChainHead->ChainTailIndex);
    
#endif
    
}

#define List_ForeachReverseEx(DataIndexVar , StartNodeIndex , NodeIndex , List) \
for\
( \
int NodeIndex = (List)->NodeArray[StartNodeIndex].Previous , DataIndexVar = (List)->NodeArray[NodeIndex].DataIndex ;\
DataIndexVar != -1 ; \
NodeIndex = (List)->NodeArray[NodeIndex].Previous , DataIndexVar = (List)->NodeArray[NodeIndex].DataIndex\
)

#define List_ForeachReverse(DataIndex , List) List_ForeachReverseEx(DataIndex , N_NodeTail , NodeIndex , List)

#define List_ForeachEx(DataIndexVar , StartNodeIndex , NodeIndex , List) \
for\
( \
int NodeIndex = (List)->NodeArray[StartNodeIndex].Next , DataIndexVar = (List)->NodeArray[NodeIndex].DataIndex ;\
DataIndexVar != -1 ; \
NodeIndex = (List)->NodeArray[NodeIndex].Next , DataIndexVar = (List)->NodeArray[NodeIndex].DataIndex\
)

#define List_Foreach(DataIndex , List) List_ForeachEx(DataIndex ,N_NodeHead , NodeIndex , List)

internal void ClearList(_List * List)
{
    List->UnusedIndex = 0;
    List->NodeArray[N_NodeHead].Next = N_NodeTail;
    List->NodeArray[N_NodeTail].Previous = N_NodeHead;
    List->NodeArray[N_RecycledNodeHead].Next = N_RecycledNodeHead;
    List->NodeArray[N_RecycledNodeTail].Previous = N_RecycledNodeTail;
}

internal int CreateNewNode(_List * List , int Count)
{
    int NewNodeIndex = -1;
    _ListNode * NodeArray = (_ListNode *)List->NodeArray;
    
    int RecycledNodeIndex = NodeArray[N_RecycledNodeHead].Next;
    _ListNode * RecycledNode = NodeArray + RecycledNodeIndex;
    
	if (RecycledNode->DataIndex != -1)
	{
		NewNodeIndex = RecycledNodeIndex;
		
        NodeArray[N_RecycledNodeHead].Next = RecycledNode->Next;
        NodeArray[RecycledNode->Next].Previous = RecycledNode->Previous;
        
	}
	else
	{
		NewNodeIndex = List->UnusedIndex++;
        NewNodeIndex += DummyNodeCount;
		
        if (List->UnusedIndex > Count) ASSERT;
	}
    
    return NewNodeIndex;
}

internal int GetNextNodeIndex(int NodeIndex , _List * List)
{
    int Result =List->NodeArray[NodeIndex].Next;
    
    if(List->NodeArray[Result].DataIndex == -1) return -1;
    
    return Result;
}

internal int GetHeadNodeIndex(_List * List)
{
    return List->NodeArray[N_NodeHead].Next;
}

internal int GetTailNodeIndex(_List * List)
{
    return List->NodeArray[N_NodeTail].Next;
}

internal int GetPreviousNodeIndex(int NodeIndex , _List * List)
{
    int Result =List->NodeArray[NodeIndex].Previous;
    
    if(List->NodeArray[Result].DataIndex == -1) return -1;
    
    return Result;
}

internal int GetDataIndexFromNodeIndex(int NodeIndex , _List * List)
{
    if(NodeIndex == -1 ) return -1;
    return List->NodeArray[NodeIndex].DataIndex;
}

internal int GetPreviousDataIndexFromNodeIndex(int NodeIndex , _List * List)
{
    return GetDataIndexFromNodeIndex(GetPreviousNodeIndex(NodeIndex, List) , List);
}

internal int GetNextDataIndexFromNodeIndex(int NodeIndex , _List * List)
{
    return GetDataIndexFromNodeIndex(GetNextNodeIndex(NodeIndex, List) , List);
}

internal void InsertToList
(
 bool ConnectBeforeNode , 
 int NodeToConnectIndex , 
 int NodeIndex , 
 int DataIndex , 
 _List * List
 )
{
    if(!List->NodeArray) ASSERT;
    
    _ListNode * NodeArray = (_ListNode *)List->NodeArray;
    
    _ListNode * Node = NodeArray + NodeIndex;
	Node->DataIndex = DataIndex;
    
    _ListNode * NodeToConnect = NodeArray + NodeToConnectIndex;
    
    if(ConnectBeforeNode)
    {
        _ListNode * PreviousNode = NodeArray +NodeToConnect->Previous;
        PreviousNode->Next = NodeIndex;
        
        Node->Previous = NodeToConnect->Previous;
        Node->Next = NodeToConnectIndex;
        
        NodeToConnect->Previous = NodeIndex;
    }
    else
    {
        _ListNode * NextNode = NodeArray + NodeToConnect->Next;
        NextNode->Previous = NodeIndex;
        
        Node->Next = NodeToConnect->Next;
        Node->Previous = NodeToConnectIndex;
        
        NodeToConnect->Next = NodeIndex;
    }
    
}

internal int CreateAndInsertToList(bool ConnectBeforeNode , int NodeToConnectIndex , int DataIndex , _List * List , int Count)
{
    int NewNodeIndex = CreateNewNode(List, Count);
	InsertToList( ConnectBeforeNode ,  NodeToConnectIndex , NewNodeIndex , DataIndex , List );
    
    return NewNodeIndex;
}

internal int AddToListHead(int DataIndex , _List * List , int Count)
{
    return CreateAndInsertToList(false , N_NodeHead , DataIndex , List , Count);
}

internal int AddToListTail( int DataIndex ,_List * List , int Count)
{
    return CreateAndInsertToList(true , N_NodeTail , DataIndex , List , Count);
}

internal int GetEmtyNodeFromList(_List * List , int Count)
{
    
    int NewNodeIndex = -1;
	_ListNode * NodeArray = (_ListNode *)List->NodeArray;
    
    int RecycledNodeIndex = NodeArray[N_RecycledNodeHead].Next;
    _ListNode * RecycledNode = NodeArray + RecycledNodeIndex;
    
	if (RecycledNode->DataIndex != -1)
	{
		NewNodeIndex = RecycledNodeIndex;
	}
	else
	{
		if (List->UnusedIndex >= Count) ASSERT;
		NewNodeIndex = List->UnusedIndex + DummyNodeCount;
	}
    
    return NewNodeIndex - DummyNodeCount;
}

internal bool DeleteFromList(int NodeIndexToDelete , _List * List)
{
	
    _ListNode * NodeArray = (_ListNode * )List->NodeArray;
	
    _ListNode * NodeToDelete =  NodeArray + NodeIndexToDelete;
	if (NodeToDelete->DataIndex == -1) return false;
    
    NodeArray[NodeToDelete->Previous].Next = NodeToDelete->Next;
    NodeArray[NodeToDelete->Next].Previous = NodeToDelete->Previous;
    
    InsertToList(true , N_RecycledNodeTail , NodeIndexToDelete , 1 , List);
    
	return true;
}

internal void DrawArrowRay(Vector3 StartPosition , Vector3 Direction  ,Color LineColor )
{
    DrawArrowRay(StartPosition , Direction , LineColor , LineColor);
}

internal void DrawArrowRay(Vector3 StartPosition , Vector3 Direction  ,Color StartColor , Color EndColor)
{
    Vector3 EndPosition = Vector3Add(StartPosition , Direction );
    DrawArrowLine(StartPosition , EndPosition,StartColor , EndColor);
}

internal void DrawArrowLine(Vector3 StartPosition , Vector3 EndPosition , Color LineColor)
{
    DrawArrowLine( StartPosition , EndPosition , LineColor , LineColor);
}

internal void DrawArrowLine(Vector3 StartPosition , Vector3 EndPosition , Color StartColor , Color EndColor)
{
    
    DrawRoundLine(StartPosition , EndPosition, 5, StartColor , EndColor);
    
    Vector3 LineDirection = Vector3Normalize(Vector3Subtract(EndPosition , StartPosition));
    Vector3 LineVerticalDirection = Vector3CrossProduct(LineDirection , Vector3Subtract(AppData->GameCamera.target , AppData->GameCamera.position));
    LineVerticalDirection = Vector3Normalize(LineVerticalDirection);
    
    Vector3 ArrowYOffset = Vector3Scale(LineDirection , -0.05f);
    
    Vector3 ArrowLeftLine = Vector3Scale(LineVerticalDirection , 0.05);
    Vector3 ArrowRightLine = Vector3Scale(LineVerticalDirection  , -0.05);
    
    ArrowLeftLine = Vector3Add(ArrowLeftLine , EndPosition);
    ArrowRightLine = Vector3Add(ArrowRightLine , EndPosition);
    
    ArrowLeftLine = Vector3Add(ArrowLeftLine , ArrowYOffset );
    ArrowRightLine = Vector3Add(ArrowRightLine , ArrowYOffset );
    
    DrawRoundLine(EndPosition , ArrowLeftLine , 5 , EndColor , EndColor);
    DrawRoundLine(EndPosition , ArrowRightLine , 5 , EndColor , EndColor);
}

internal int GetKeyFrameCount(_KeyFrame * StartKeyFrame , _KeyFrame * EndKeyFrame)
{
    
    int FrameCount = EndKeyFrame->FrameIndex - StartKeyFrame->FrameIndex;
    
    if(EndKeyFrame->FrameIndex < StartKeyFrame->FrameIndex)
    {
        FrameCount = Editor->EndFrameIndex - StartKeyFrame->FrameIndex + 1;
        FrameCount += EndKeyFrame->FrameIndex - Editor->StartFrameIndex;
    }
    
    return FrameCount;
}

internal bool IsRig(int BoneIndex)
{
    if(BoneIndex == B_RightHandController) return true;
    if(BoneIndex == B_RightArmPoleTarget) return true;
    if(BoneIndex == B_LeftArmPoleTarget) return true;
    if(BoneIndex == B_LeftHandController) return true;
    
    if(BoneIndex == B_RightLegController) return true;
    if(BoneIndex == B_RightLegPoleTarget) return true;
    if(BoneIndex == B_LeftLegPoleTarget) return true;
    if(BoneIndex == B_LeftLegController) return true;
    
    if(BoneIndex == B_Origin) return true;
    
    return false;
}

internal void IterateChildBone(_Bone * Bone)
{
    int BoneIndex = Bone->BoneIndex;
    
    Vector3 StartPosition = Bone->Position;
    Vector3 EndPosition = Bone->Position;
    
    EndPosition = Vector3Add(EndPosition , Vector3RotateByQuaternion(Bone->State.Direction, Bone->Rotation));
    
    DrawArrowLine(StartPosition ,EndPosition , YELLOW ,RED);
    
    //DrawBoneLine(Bone);
    HashTable_Iterate(ChildBoneIndex , BoneIndex, &Editor->BoneChildrenHashTable , BoneCount)
    {
        _Bone * ChildBone = Editor->StickMan[ChildBoneIndex];
        
        IterateChildBone(ChildBone);
    }
}

internal void MemberDataToString(_MemberMetaData* MemberMeta, unsigned char* DataHead, wchar_t* StringBuffer)
{
	const char* MemberName = MemberMeta->Name;
	int MemberOffset = MemberMeta->MemberOffset;
	unsigned char* MemberData = DataHead + MemberOffset;
    
#define TypeToTextBuffer(Type,StringFormat) case GetType(Type): swprintf(StringBuffer, L"%S :" StringFormat, MemberName ,*(Type*)MemberData); break; 
    
	switch (MemberMeta->MemberType)
	{
		TypeToTextBuffer(float, L"%f");
		TypeToTextBuffer(double, L"%f");
		TypeToTextBuffer(int, L"%d");
        case GetType(Vector2): swprintf(StringBuffer, L"%S :" "%f,%f", MemberName, (*(Vector2*)MemberData).x, (*(Vector2*)MemberData).y); break;
        case GetType(Vector3): swprintf(StringBuffer, L"%S :" "%f,%f,%f", MemberName, (*(Vector3*)MemberData).x, (*(Vector3*)MemberData).y, (*(Vector3*)MemberData).z); break;
        
        default: swprintf(StringBuffer, L"%S :" L"__", MemberName); break;
	}
}

#define PrintStructMemberInfo(Type,StructData,Quad) _PrintStructMemberInfo(GetStructMeta(Type),(unsigned char*)StructData,Quad)

internal void _PrintStructMemberInfo(_StructMetaData StructMetaData, unsigned char* StructData, _Rect TextQuad)
{
	wchar_t StringBuffer[256] = {};
    
	for (int MemberIndex = 0; MemberIndex < StructMetaData.MemberCount; MemberIndex++)
	{
		MemberDataToString(StructMetaData.MembersArray + MemberIndex, StructData, StringBuffer);
        
		D_DrawText(TextQuad, StringBuffer, Fade(WHITE, 0.6f), false);
		TextQuad.Position.y -= TextQuad.Size.y;
	}
}

//just leave it be
internal void CreateStructNameStringHash()
{
    
    _StructMetaHash = AllocateTemp(_StructMetaDataNode* , STRUCT_META_HASH_MAX);
    for(int TypeIndex = 0 ; TypeIndex<_MT_Type_Count ; TypeIndex++)
    {
        const char * TypeName = _TypeMetaName[TypeIndex];
        unsigned int TypeHash = StringToHash(TypeName);
        _StructMetaDataNode ** NodePointer = _StructMetaHash + (TypeHash %STRUCT_META_HASH_MAX);
        
        for(;(*NodePointer); NodePointer = &(*NodePointer)->Next);
        
        _StructMetaDataNode * NewNode =AllocateTemp(_StructMetaDataNode,1);
        (*NodePointer) = NewNode;
        
        (*NewNode) = {};
        NewNode->TypeIndex = TypeIndex;
        NewNode->Name = TypeName;
        NewNode->Size = _TypeMetaSize[TypeIndex];
        NewNode->StructMeta = _TypeStructMeta_[TypeIndex];
    }
    
}

internal int GetDataSize(char * TypeName)
{
	unsigned int TypeHash = StringToHash(TypeName);
	_StructMetaDataNode * Node = _StructMetaHash[TypeHash&STRUCT_META_HASH_MAX];
    
	for (;Node; Node = Node->Next)
	{
		if (!strcmp(TypeName , Node->Name))
		{
			return Node->Size;
		}
	}
    
	ASSERT;
    
	return 0;
}

//DONE :try make it patch pointer
//when the struct size is differnt the offset isn't correct at all

//hear me out
//try keep track of the "memory inflation"
//then apply the "inflated"(or deflated) size to the pointer

//there is some grabage value sneak in there ):
//try replicate that

//TODO : 2025/5/24 
//clean it up if you want
//it is done

#if 1

#define SHOW_PATCH_DETAIL 0
#define SHOW_PATCH_ERROR 1
#define SHOW_PATCH_POINTER 0

internal void RecordPointer(void** Pointer , int CurrentType, int PreviousType , _PersistMemory * PreviousMemory)
{
    if(!CurrentPointerStack)
    {        
        PointerStackHeader = AllocateFrame(PointerRecordNode,1);
        CurrentPointerStack = PointerStackHeader;
        
        CurrentPointerStack->PointerStack = AllocateFrame( void ** , MAX_POINTER_STACK);
        CurrentPointerStack->CurrentTypeStack = AllocateFrame(int , MAX_POINTER_STACK);
        CurrentPointerStack->PreviousTypeStack = AllocateFrame(int , MAX_POINTER_STACK);
        CurrentPointerStack->StackCount = 0;
    }
    
    if(CurrentPointerStack->StackCount == MAX_POINTER_STACK)
    {
        CurrentPointerStack->Next = AllocateFrame(PointerRecordNode,1);
        CurrentPointerStack = CurrentPointerStack->Next;
        
        CurrentPointerStack->PointerStack = AllocateFrame(void ** , MAX_POINTER_STACK);
        CurrentPointerStack->CurrentTypeStack = AllocateFrame(int , MAX_POINTER_STACK);
        CurrentPointerStack->PreviousTypeStack = AllocateFrame(int , MAX_POINTER_STACK);
        CurrentPointerStack->StackCount = 0;
        StackDepth++;
    }
    
    CurrentPointerStack->PointerStack[CurrentPointerStack->StackCount] = ( Pointer) ;
    CurrentPointerStack->CurrentTypeStack[CurrentPointerStack->StackCount] = CurrentType;
    CurrentPointerStack->PreviousTypeStack[CurrentPointerStack->StackCount] = PreviousType;
    
    CurrentPointerStack->StackCount++;
    
	//why i even checking pointer within current block or not?
	//unless it mean catching weird pointer from previous memory
	//current block can be smaller that the previous one so it will get false alarm , right?
    long long Start = (long long )_StartMemory_;
    long long PointerTargetLocation =(long long)(*(CurrentPointerStack->PointerStack[CurrentPointerStack->StackCount-1])); 
    
    bool PointerBroken = false;
    
    if(PointerTargetLocation < PreviousMemory->StartOffset) PointerBroken = true;
    if(PointerTargetLocation >= PreviousMemory->CurrentOffset) PointerBroken = true;
    
    if(PointerBroken)
    {
        while(1)
        {
            printf("Pointer isn't inside Memory (%lld)\n" , PointerTargetLocation);
            printf("Clear it ? (Y\\N)\n");
            char Answer = 0;
            scanf("%c" , &Answer);
            
            if(Answer == 'Y')
            {
                (*(CurrentPointerStack->PointerStack[CurrentPointerStack->StackCount-1])) = 0;
                CurrentPointerStack->StackCount--;
                break;
            }
            
            if(Answer == 'N')
            {
                break;
            }
        }
        
    }
    
#if SHOW_PATCH_POINTER
    char * CurrentName = (char * )_TypeMetaName[CurrentType];
    char * PreviousName  = PreviousTypeNameArray[PreviousType];
    
    printf("Recorded pointer (%d) (%s) %lld -> (%s) %lld\n" ,
           StackDepth * MAX_POINTER_STACK + (CurrentPointerStack->StackCount-1) ,
           CurrentName,(long long)(CurrentPointerStack->PointerStack[CurrentPointerStack->StackCount-1]) - Start, 
           PreviousName , PointerTargetLocation);
#endif
}

internal _TypeFormDifference CompareType(int CurrentType, int PreviousType)
{
    _TypeFormDifference Result ={};
    
    if(CurrentType >= 0)
    {
        if(CurrentType > _MT_Type_Count) ASSERT;
        
        Result.CurrentStruct =  _TypeStructMeta_[CurrentType];
        
        if(Result.CurrentStruct.MemberCount >0) Result.CurrentIsStruct = true;
        if(_IsTypeEnum[CurrentType]) Result.CurrentIsEnumType = true;
        if(CurrentType <= _MT_void) Result.CurrentIsBasicType = true;
        
        int CurrentTypeCheck = 0;
        
        if(Result.CurrentIsStruct) CurrentTypeCheck++;
        if(Result.CurrentIsEnumType) CurrentTypeCheck++;
        if(Result.CurrentIsBasicType) CurrentTypeCheck++;
        
        if(CurrentTypeCheck == 0) ASSERT;
        if(CurrentTypeCheck > 1) ASSERT;
    }
    
    if(PreviousType >=0 )
    {
        if(PreviousType > PreviousTypeCount) ASSERT;
        
        Result.PreviousStruct=PreviousStructMetaArray[PreviousType];
        if(Result.PreviousStruct.MemberCount >0) Result.PreviousIsStruct = true;
        if(PreviousIsEnum[PreviousType]) Result.PreviousIsEnumType = true;
        if(PreviousType <= PreviousVoidTypeIndex) Result.PreviousIsBasicType = true;
        
        int PreviousTypeCheck = 0;
        
        if(Result.PreviousIsStruct) PreviousTypeCheck++;
        if(Result.PreviousIsEnumType) PreviousTypeCheck++;
        if(Result.PreviousIsBasicType) PreviousTypeCheck++;
        
        if(PreviousTypeCheck == 0) ASSERT;
        if(PreviousTypeCheck > 1) ASSERT;
    }
    
    if(Result.CurrentIsStruct!= Result.PreviousIsStruct) Result.DifferentFormOfType = true;
    if(Result.CurrentIsEnumType != Result.PreviousIsEnumType) Result.DifferentFormOfType = true;
    if(Result.CurrentIsBasicType!= Result.PreviousIsBasicType) Result.DifferentFormOfType = true;
    
    if(PreviousType <0 )Result.DifferentFormOfType = true;
    if(CurrentType <0 )Result.DifferentFormOfType = true;
    
    return Result;
}

internal _MemberDifference CompareMember(_MemberMetaData * CurrentMember , _MemberMetaData * PreviousMember)
{
    _MemberDifference Result = {};
    Result.Same =true;
    
    Result.CurrentName = CurrentMember->Name;
    Result.CurrentTypeName = (char * )_TypeMetaName[CurrentMember->MemberType];
    
    Result.PreviousName = PreviousMember->Name;
    Result.PreviousTypeName  = PreviousTypeNameArray[PreviousMember->MemberType];
    
    if(strcmp(Result.CurrentTypeName,Result.PreviousTypeName)) Result.DifferentType = true;
    
    if(strcmp(Result.CurrentName,Result.PreviousName)) Result.DifferentName = true;
    
    if(CurrentMember->IsPointer != PreviousMember->IsPointer) Result.DifferentPointer = true;
    
    if(CurrentMember->IsArray != PreviousMember->IsArray) Result.DifferentArray = true;
    
    if(CurrentMember->ArraySize != CurrentMember->ArraySize) Result.DifferentArraySize = true;
    
    Result.TypeFormDifference = CompareType(CurrentMember->MemberType , PreviousMember->MemberType);
    
    if(Result.DifferentType) Result.Same = false;
    if(Result.DifferentName) Result.Same = false;
    if(Result.DifferentPointer) Result.Same = false;
    if(Result.DifferentArray) Result.Same = false;
    if(Result.DifferentArraySize) Result.Same = false;
    if(Result.TypeFormDifference.DifferentFormOfType) Result.Same = false;
    
    return Result;
}

internal void AddDecomposedType(int Type , int Offset , char * Name , bool IsPointer,int DataCount , _MemberNode * ReverseMemberList , bool Current)
{
    _DecomposedBasicType * BasicType = AllocateFrame(_DecomposedBasicType,1);
    BasicType->Type = Type;
    BasicType->Offset = Offset;
    BasicType->IsPointer = IsPointer;
    BasicType->DataCount = DataCount; 
    BasicType->ReverseMemberList = ReverseMemberList;
    BasicType->Name = Name;
    
    if(!Name) ASSERT;
    
    if(Current)
    {
        (*CurrentBasicTypeTail) = BasicType;
        CurrentBasicTypeTail = &(*CurrentBasicTypeTail)->Next;
    }
    else
    {
        (*PreviousBasicTypeTail) = BasicType;
        PreviousBasicTypeTail = &(*PreviousBasicTypeTail)->Next;
    }
}

internal void DecomposeType(int Type, _MemberNode * PreviousNode ,bool Current)
{
    _StructMetaData * StructList = 0;
    
    if(Current)
    {
        StructList = _TypeStructMeta_;
    }
    else
    {
        StructList = PreviousStructMetaArray;
    }
    
    _StructMetaData StructMeta = StructList[Type];
    
    for(int MemberIndex = 0 ; MemberIndex < StructMeta.MemberCount ; MemberIndex++)
    {
        _MemberMetaData * CurrentMember = StructMeta.MembersArray + MemberIndex;
        
        _TypeFormDifference Result={};
        bool IsStruct = false;
        if(Current)
        {
            Result= CompareType(CurrentMember->MemberType ,-1);
            IsStruct = Result.CurrentIsStruct;
            
        }
        else
        {
            Result = CompareType(-1,CurrentMember->MemberType);
            IsStruct = Result.PreviousIsStruct;
        }
        
        bool IsNestedStruct = false;
        
        if(IsStruct)
        {
            if(!CurrentMember->IsPointer)
            {
                IsNestedStruct = true;
            }
        }
        
        int ArraySize = 1;
        
        if(CurrentMember->IsArray) 
        {
            ArraySize = CurrentMember->ArraySize;
        }
        
        if(IsNestedStruct)
        {
            for(int ArrayIndex = 0 ; ArrayIndex < ArraySize ; ArrayIndex++ )
            {
                _MemberNode*  MemberNode= AllocateFrame(_MemberNode,1);
                MemberNode->Member = CurrentMember;
                MemberNode->DataIndex = ArrayIndex;
                MemberNode->Previous = PreviousNode;
                
                DecomposeType(CurrentMember->MemberType , MemberNode,Current);
            }
        }
        else
        {
            AddDecomposedType(CurrentMember->MemberType,CurrentMember->MemberOffset , CurrentMember->Name , CurrentMember->IsPointer,ArraySize,PreviousNode,Current);
        }
        
    }
}

//TODO: add Dynamic memory

//TODO: manually type\data conversion , so you should give each memory section a unique name. you can use identify the name to convert it to different type and dispose it after conversion

//TODO: add check for struct declare in struct <--- maybe not

internal void CheckAndPatchData(_PersistMemory * TargetMemory , FILE* DataTypeAndDetailInfoFile)
{
	PointerStackHeader = 0;
	CurrentPointerStack = 0;
	StackDepth = 0;
    
	if (!DataTypeAndDetailInfoFile) return;
    
	fread(&PreviousTypeCount, sizeof(int), 1, DataTypeAndDetailInfoFile);
    
	PreviousStructMetaArray = AllocateFrame(_StructMetaData, PreviousTypeCount);
	PreviousTypeNameArray = AllocateFrame(char*, PreviousTypeCount);
	PreviousIsEnum = AllocateFrame(bool , PreviousTypeCount);
	PreviousTypeSize = AllocateFrame(int , PreviousTypeCount);
    
	PreviousVoidTypeIndex = _MT_void;
    
	for (int TypeIndex = 0; TypeIndex < PreviousTypeCount; TypeIndex++)
	{
		_StructMetaData * PreviousStructMeta = PreviousStructMetaArray + TypeIndex;
		fread(&PreviousStructMeta->MemberCount , sizeof(int), 1 , DataTypeAndDetailInfoFile);
        
		PreviousStructMeta->MembersArray = AllocateFrame(_MemberMetaData, PreviousStructMeta->MemberCount);
        
		fread(PreviousStructMeta->MembersArray , sizeof(_MemberMetaData), PreviousStructMeta->MemberCount , DataTypeAndDetailInfoFile);
        
		int StringSize = 0;
		fread(&StringSize , sizeof(int), 1 , DataTypeAndDetailInfoFile);
		StringSize++;
        
		PreviousTypeNameArray[TypeIndex] = AllocateFrame(char , StringSize);
        
		fread(PreviousTypeNameArray[TypeIndex] , sizeof(char) , StringSize - 1 , DataTypeAndDetailInfoFile);
	}
    
	fread(PreviousIsEnum, sizeof(bool), PreviousTypeCount , DataTypeAndDetailInfoFile);
	fread(PreviousTypeSize , sizeof(int) , PreviousTypeCount , DataTypeAndDetailInfoFile);
    
	if (strcmp("void" , PreviousTypeNameArray[PreviousVoidTypeIndex]))
	{
		PreviousVoidTypeIndex = -1;
		for (int TypeIndex = 0; TypeIndex < PreviousTypeCount; TypeIndex++)
		{
			if (!strcmp("void" , PreviousTypeNameArray[TypeIndex]))
			{
				PreviousVoidTypeIndex = TypeIndex;
				break;
			}
		}
	}
    
	if (PreviousVoidTypeIndex == -1) ASSERT;
    
	_MemoryDetail * PreviousDetailArray = 0;
    
	fread(&PreviousMemoryInfo , sizeof(_PersistMemory), 1, DataTypeAndDetailInfoFile);	    
	PreviousDetailArray = AllocateFrame(_MemoryDetail, PreviousMemoryInfo.MemoryDetailCount );
	fread(PreviousDetailArray , sizeof(_MemoryDetail), PreviousMemoryInfo.MemoryDetailCount, DataTypeAndDetailInfoFile);
	
    fclose(DataTypeAndDetailInfoFile);
    
	int CurrentDetailCount = TargetMemory->MemoryDetailCount;
	CurrentBasicTypeHeadList = AllocateFrame(_DecomposedBasicType * , CurrentDetailCount);
	PreviousBasicTypeHeadList = AllocateFrame(_DecomposedBasicType * , PreviousMemoryInfo.MemoryDetailCount);
    
	//why the fuck you use index???
	int *DetailMap = AllocateFrame(int, CurrentDetailCount);
	for (int DetailIndex = 0; DetailIndex < CurrentDetailCount; DetailIndex++) DetailMap[DetailIndex] = -1;
    
	_MemoryDetail ** PreviousDetailMap = AllocateFrame(_MemoryDetail* , PreviousMemoryInfo.MemoryDetailCount);
    
	bool *IsPreviousDetailToken = AllocateFrame(bool, PreviousMemoryInfo.MemoryDetailCount);
    
	int PreviousDetailIndex = 0;
    
	_MemoryDetail *CurrentDetail = TargetMemory->MemoryDetailHead;
	for (int CurrentDetailIndex = 0; CurrentDetail; CurrentDetail = CurrentDetail->NextNode , CurrentDetailIndex++)
	{
        
		for (int PreviousDetailIndex = 0; PreviousDetailIndex < PreviousMemoryInfo.MemoryDetailCount; PreviousDetailIndex++)
		{
			_MemoryDetail *PreviousDetail = PreviousDetailArray + PreviousDetailIndex;
            
			//why i don't just use tpye name from detail to check?
			char * CurrentTypeName = (char *)_TypeMetaName[CurrentDetail->Type];
			char * PreviouTypesName = PreviousTypeNameArray[PreviousDetail->Type];
            
			if (strcmp(CurrentTypeName , PreviouTypesName)) continue;
			if (strcmp(CurrentDetail->DataName , PreviousDetail->DataName)) continue;
			if (CurrentDetail->IsPointer != PreviousDetail->IsPointer) continue;
            
#if 0
            if(PreviousDetailIndex != CurrentDetailIndex)
            {
                printf("previous section changed : %d -> %d\n" , PreviousDetailIndex , CurrentDetailIndex);
            }
#endif
            
			IsPreviousDetailToken[PreviousDetailIndex] = true;
			PreviousDetailMap[PreviousDetailIndex] = CurrentDetail;
            
			DetailMap[CurrentDetailIndex] = PreviousDetailIndex;
		}
	}
    
	CurrentDetail = TargetMemory->MemoryDetailHead;
	for (int CurrentDetailIndex = 0; CurrentDetail; CurrentDetail = CurrentDetail->NextNode , CurrentDetailIndex++)
	{
		if (DetailMap[CurrentDetailIndex] == -1)
		{
#if SHOW_PATCH_ERROR
			printf("Memory section (%d) missing : %s %s \n", CurrentDetailIndex, _TypeMetaName[CurrentDetail->Type], CurrentDetail->DataName);
#endif
            
			continue;
		}
        
		int PreviousDetailIndex = DetailMap[CurrentDetailIndex];
		_MemoryDetail * PreviousDetail = PreviousDetailArray + PreviousDetailIndex;
        
		CurrentDetail->OldDetailFromPreviousMemory = PreviousDetail;
        
		CurrentBasicTypeTail = CurrentBasicTypeHeadList + CurrentDetailIndex;
		PreviousBasicTypeTail = PreviousBasicTypeHeadList + PreviousDetailIndex;
        
		_TypeFormDifference Result = CompareType(CurrentDetail->Type , PreviousDetail->Type);
        
		bool CurrentNeedDecompose = false;
		if (Result.CurrentIsStruct)
		{
			if (!CurrentDetail->IsPointer)
			{
				CurrentNeedDecompose = true;
			}
		}
        
		if (CurrentNeedDecompose)
		{
			DecomposeType(CurrentDetail->Type , 0 , true);
		}
		else
		{
			AddDecomposedType(CurrentDetail->Type, 0, CurrentDetail->DataName, CurrentDetail->IsPointer, 1, 0, true);
		}
        
		bool PreviousNeedDecompose = false;
		if (Result.PreviousIsStruct)
		{
			if (!PreviousDetail->IsPointer)
			{
				PreviousNeedDecompose = true;
			}
		}
        
		if (PreviousNeedDecompose)
		{
			DecomposeType(PreviousDetail->Type , 0 , false);
		}
		else
		{
			AddDecomposedType(PreviousDetail->Type, 0, PreviousDetail->DataName, PreviousDetail->IsPointer, 1, 0, false);
		}
	}
    
	CurrentDetail = TargetMemory->MemoryDetailHead;
    
	char * DontPatchPassType[64] = {};
	int DontPatchPassTypeCount = 0;
	//DontPatchPassType[DontPatchPassTypeCount++] = "D_Array";
    
	for (int CurrentDetailIndex = 0; CurrentDetailIndex < CurrentDetailCount; CurrentDetailIndex++, CurrentDetail = CurrentDetail->NextNode)
	{
		int PreviousDetailIndex = DetailMap[CurrentDetailIndex];
		_MemoryDetail * PreviousDetail = PreviousDetailArray + PreviousDetailIndex;
        
		bool DontPatchThisDetail = false;
        
		for (int CheckIndex = 0; CheckIndex < DontPatchPassTypeCount; CheckIndex++)
		{
			if (!strcmp(CurrentDetail->DataName , DontPatchPassType[CheckIndex])) DontPatchThisDetail = true;
		}
        
#if SHOW_PATCH_DETAIL
		printf("\n");
        
		printf("%s\n", CurrentDetail->DataName);
        
		_DecomposedBasicType * BasicTypeHead[2] = {};
		BasicTypeHead[0] = CurrentBasicTypeHeadList[CurrentDetailIndex];
		BasicTypeHead[1] = PreviousBasicTypeHeadList[PreviousDetailIndex];
        
		for (int MemoryIndex = 0; MemoryIndex < 2; MemoryIndex++)
		{
			printf("\n");
			for (_DecomposedBasicType * CurrentBasicType = BasicTypeHead[MemoryIndex]; CurrentBasicType; CurrentBasicType = CurrentBasicType->Next)
			{
				char * CurrentTypeName = 0;
				if (MemoryIndex == 0) CurrentTypeName = (char*)_TypeMetaName[CurrentBasicType->Type];
				if (MemoryIndex == 1) CurrentTypeName = (char*)PreviousTypeNameArray[CurrentBasicType->Type];
                
				printf("(Size : %d) %s%s %s" , CurrentBasicType->DataCount, CurrentTypeName , CurrentBasicType->IsPointer?" (Pointer *)":"" , CurrentBasicType->Name?CurrentBasicType->Name: "(not member)");
                
				for (_MemberNode * CurrentNode = CurrentBasicType->ReverseMemberList; CurrentNode; CurrentNode = CurrentNode->Previous)
				{
					char * MemberName = 0;
					if (MemoryIndex == 0) MemberName = (char*)_TypeMetaName[CurrentNode->Member->MemberType];
					if (MemoryIndex == 1) MemberName = (char*)PreviousTypeNameArray[CurrentNode->Member->MemberType];
                    
					printf(" <-- (%s %s) (Index %d)" , MemberName , CurrentNode->Member->Name , CurrentNode->DataIndex);
				}
                
				printf("\n");
			}
		}
#endif
        
		for (_DecomposedBasicType * CurrentType = CurrentBasicTypeHeadList[CurrentDetailIndex]; CurrentType; CurrentType = CurrentType->Next)
		{
			for (_DecomposedBasicType * PreviousType = PreviousBasicTypeHeadList[PreviousDetailIndex]; PreviousType; PreviousType = PreviousType->Next)
			{
				if (CurrentType->DataCount != PreviousType->DataCount) continue;
                
				if (CurrentType->IsPointer != PreviousType->IsPointer) continue;
                
				if (strcmp(PreviousType->Name, CurrentType->Name)) continue;
                
				char * CurrentTypeName = (char*)_TypeMetaName[CurrentType->Type];
				char * PreviousTypeName = (char*)PreviousTypeNameArray[PreviousType->Type];
                
				if (strcmp(CurrentTypeName, PreviousTypeName)) continue;
                
				_MemberNode * CurrentNode = CurrentType->ReverseMemberList;
				_MemberNode * PreviousNode = PreviousType->ReverseMemberList;
                
                
				bool DontPathThisBasicType = false;
                
				if (DontPatchThisDetail) DontPathThisBasicType = true;
                
				for (int CheckIndex = 0; CheckIndex < DontPatchPassTypeCount; CheckIndex++)
				{
					if (!strcmp(CurrentTypeName , DontPatchPassType[CheckIndex])) DontPathThisBasicType = true;
				}
                
				bool MemberNotMatch = false;
                
				int PreviousNestedMemberOffset = 0;
				int CurrentNestedMemberOffset = 0;
                
				for (;; CurrentNode = CurrentNode->Previous , PreviousNode = PreviousNode->Previous)
				{
					if ((CurrentNode != 0) != (PreviousNode != 0))
					{
						MemberNotMatch = true;
						break;
					}
                    
					if (!CurrentNode) break;
                    
					if (CurrentNode->DataIndex != PreviousNode->DataIndex)
					{
						MemberNotMatch = true;
						break;
					}
                    
					if ((CurrentNode != 0) != (PreviousNode != 0))
					{
						MemberNotMatch = true;
						break;
					}
                    
					_MemberMetaData * CurrentMember = CurrentNode->Member;
					_MemberMetaData * PreviousMember = PreviousNode->Member;
                    
					if (CurrentMember->IsPointer != PreviousMember->IsPointer)
					{
						MemberNotMatch = true;
						break;
					}
                    
					if (strcmp(CurrentMember->Name , PreviousMember->Name))
					{
						MemberNotMatch = true;
						break;
					}
                    
					char * CurrentMemberName = (char*)_TypeMetaName[CurrentMember->MemberType];
					char * PreviousMemberName = (char*)PreviousTypeNameArray[PreviousMember->MemberType];
                    
					if (strcmp(CurrentMemberName , PreviousMemberName))
					{
						MemberNotMatch = true;
						break;
					}
                    
					for (int CheckIndex = 0; CheckIndex < DontPatchPassTypeCount; CheckIndex++)
					{
						if (!strcmp(CurrentMemberName , DontPatchPassType[CheckIndex])) DontPathThisBasicType = true;
					}
                    
					CurrentNestedMemberOffset += CurrentMember->MemberOffset + CurrentNode->DataIndex * CurrentMember->MemberSize;
					PreviousNestedMemberOffset += PreviousMember->MemberOffset + PreviousNode->DataIndex * PreviousMember->MemberSize;
				}
                
				if (MemberNotMatch) continue;
                
                
				if (PreviousType->Taken) ASSERT;
                
				PreviousType->Taken = true;
				CurrentType->Taken = true;
                
#if SHOW_PATCH_DETAIL
				printf("\n");
				printf("(Size : %d) %s%s %s" , CurrentType->DataCount, CurrentTypeName , CurrentType->IsPointer?" (Pointer *)":"" , CurrentType->Name?CurrentType->Name: "(not member)");
                
				for (_MemberNode * CurrentNode = CurrentType->ReverseMemberList; CurrentNode; CurrentNode = CurrentNode->Previous)
				{
					char * MemberName = 0;
					MemberName = (char*)_TypeMetaName[CurrentNode->Member->MemberType];
                    
					printf(" <-- (%s %s) (Index %d)" , MemberName , CurrentNode->Member->Name , CurrentNode->DataIndex);
				}
                
				printf("\n");
                
				printf("(Size : %d) %s%s %s" , PreviousType->DataCount, PreviousTypeName , PreviousType->IsPointer?" (Pointer *)":"" , PreviousType->Name?PreviousType->Name: "(not member)");
                
				for (_MemberNode * CurrentNode = PreviousType->ReverseMemberList; CurrentNode; CurrentNode = CurrentNode->Previous)
				{
					char * MemberName = 0;
					MemberName = (char*)PreviousTypeNameArray[CurrentNode->Member->MemberType];
                    
					printf(" <-- (%s %s) (Index %d)" , MemberName , CurrentNode->Member->Name , CurrentNode->DataIndex);
				}
                
				printf("\n");
#endif
                
				long long CurrentDataOffset = (long long)(CurrentNestedMemberOffset + CurrentType->Offset) + CurrentDetail->Start;
				long long PreviousDataOffset = (long long)(PreviousNestedMemberOffset + PreviousType->Offset) + PreviousDetail->Start - PreviousMemoryInfo.StartOffset;
                
				int DataSize = sizeof(void*);
                
				if (!CurrentType->IsPointer)
				{
					DataSize = _TypeMetaSize[CurrentType->Type];
					if (DataSize != PreviousTypeSize[PreviousType->Type]) ASSERT;
				}
                
                
				int DataCount = CurrentType->DataCount;
				if (PreviousType->DataCount < DataCount)
				{
					DataCount = PreviousType->DataCount;
                    
#if SHOW_PATCH_ERROR
					printf("WANRING Data loss\n");
#endif
                    
				}
                
				unsigned char * CurretMemory = (unsigned char * )_StartMemory_;
				unsigned char * PreviousMemory = PreviousSaveData;
                
				int SmallestDetailDataCount = CurrentDetail->DataCount;
                
				if (SmallestDetailDataCount > PreviousDetail->DataCount)
				{
					SmallestDetailDataCount = PreviousDetail->DataCount;
                    
#if SHOW_PATCH_ERROR
					printf("WANRING Data loss\n");
#endif
                    
				}
                
				PreviousType->CurrentMemoryLocation = CurrentNestedMemberOffset + CurrentType->Offset + CurrentDetail->Start;
				PreviousType->PreviousMemoryLocation = PreviousNestedMemberOffset + PreviousType->Offset +PreviousDetail->Start ;
                
				for (int DetailIndex = 0; DetailIndex < SmallestDetailDataCount; DetailIndex++)
				{
					long long CurrentDetailDataSize = _TypeMetaSize[CurrentDetail->Type];
					long long PreviousDetailDataSize = PreviousTypeSize[PreviousDetail->Type];
                    
					if (CurrentDetail->IsPointer) CurrentDetailDataSize = sizeof(void *);
					if (PreviousDetail->IsPointer) PreviousDetailDataSize = sizeof(void *);
                    
					if (CurrentDetail->IsPointer != PreviousDetail->IsPointer) ASSERT;
					if (CurrentDetailDataSize == 0) ASSERT;
					if (PreviousDetailDataSize == 0) ASSERT;
                    
					long long CurrentDetailDataOffset = CurrentDetailDataSize * DetailIndex;
					long long PreviousDetailDataOffset = PreviousDetailDataSize * DetailIndex;
                    
#if 0
					//too slow
					printf("from (Offset : %d) (DataIndex : %d %d)\n" , (CurrentNestedMemberOffset + CurrentType->Offset) , DetailIndex , CurrentDetailDataOffset);
                    
					printf("to (Offset : %d) (DataIndex : %d %d)\n" , (PreviousNestedMemberOffset + PreviousType->Offset) , DetailIndex , PreviousDetailDataOffset);
#endif
                    
					if (!DontPathThisBasicType)
					{
						memcpy(CurretMemory + (CurrentDataOffset + CurrentDetailDataOffset), PreviousMemory + (PreviousDataOffset + PreviousDetailDataOffset), DataCount * DataSize);
					}
					else
					{
#if SHOW_PATCH_DETAIL
						printf("Patch skipped\n");
#endif
					}
                    
					if (CurrentType->IsPointer)
					{
						//this thing mean the pointer in the "current memory" still pointing toward "previous memory"
						//and i need the pointer to this "current memory" to fix "previous pointer"
						//don't even think i got it wrong
						//i already did that 
						void** PointerStart = (void ** )(CurretMemory + (CurrentDataOffset + CurrentDetailDataOffset));
                        
						for (int DataIndex = 0; DataIndex < DataCount; DataIndex++)
						{
							if (! (*(PointerStart + DataIndex))) continue;
							
#if SHOW_PATCH_POINTER
                            
							printf("\n");
							printf("(Size : %d) %s%s %s" , CurrentType->DataCount, CurrentTypeName , CurrentType->IsPointer?" (Pointer *)":"" , CurrentType->Name?CurrentType->Name: "(not member)");
                            
							for (_MemberNode * CurrentNode = CurrentType->ReverseMemberList; CurrentNode; CurrentNode = CurrentNode->Previous)
							{
								char * MemberName = 0;
								MemberName = (char*)_TypeMetaName[CurrentNode->Member->MemberType];
                                
								printf(" <-- (%s %s) (Index %d)" , MemberName , CurrentNode->Member->Name , CurrentNode->DataIndex);
							}
                            
							printf("\n");
                            
							printf("(Size : %d) %s%s %s" , PreviousType->DataCount, PreviousTypeName , PreviousType->IsPointer?" (Pointer *)":"" , PreviousType->Name?PreviousType->Name: "(not member)");
                            
							for (_MemberNode * CurrentNode = PreviousType->ReverseMemberList; CurrentNode; CurrentNode = CurrentNode->Previous)
							{
								char * MemberName = 0;
								MemberName = (char*)PreviousTypeNameArray[CurrentNode->Member->MemberType];
                                
								printf(" <-- (%s %s) (Index %d)" , MemberName , CurrentNode->Member->Name , CurrentNode->DataIndex);
							}
                            
							printf("\n");
                            
#endif
                            
							RecordPointer(PointerStart + DataIndex , CurrentType->Type , PreviousType->Type , &PreviousMemoryInfo);
						}
					}
                    
				}
			}
		}
        
#if SHOW_PATCH_ERROR
		for (_DecomposedBasicType * CurrentType = CurrentBasicTypeHeadList[CurrentDetailIndex]; CurrentType; CurrentType = CurrentType->Next)
		{
			if (CurrentType->Taken) continue;
            
			char * CurrentTypeName = (char*)_TypeMetaName[CurrentType->Type];
			printf("ERROR member missing: (Size : %d) %s%s %s" , CurrentType->DataCount, CurrentTypeName , CurrentType->IsPointer?" (Pointer *)":"" , CurrentType->Name?CurrentType->Name: "(not member)");
            
			for (_MemberNode * CurrentNode = CurrentType->ReverseMemberList; CurrentNode; CurrentNode = CurrentNode->Previous)
			{
				char * MemberName = 0;
				MemberName = (char*)_TypeMetaName[CurrentNode->Member->MemberType];
                
				printf(" <-- (%s %s) (Index %d)" , MemberName , CurrentNode->Member->Name , CurrentNode->DataIndex);
			}
            
			printf("\n");
		}
#endif
        
#if SHOW_PATCH_DETAIL
		printf("\n");
#endif
	}
    
#if SHOW_PATCH_POINTER
	printf("\n");
#endif
    
	for (PointerRecordNode * CurrentNode = PointerStackHeader; CurrentNode; CurrentNode = CurrentNode->Next)
	{
		for (int StackIndex = 0; StackIndex < CurrentNode->StackCount; StackIndex++)
		{
			void ** PointerToPatch = CurrentNode->PointerStack[StackIndex];
			int CurrentType = CurrentNode->CurrentTypeStack[StackIndex];
            
			char * CurrentTypeName = (char *)_TypeMetaName[CurrentType];
			char * OldPointerTypeName = 0;
            
			long long PointerOffset = (long long)(*PointerToPatch);
            
			bool BasicTypeOnly = false;
			bool DeadPointer = false;
			bool PointerOutOfScope = true;
            
			int DataSize = 0;
			long long PointerRelativeToMemorySection = 0;
			CurrentDetail = 0;
            
			for (int PreviousDetailIndex = 0; PreviousDetailIndex < PreviousMemoryInfo.MemoryDetailCount; PreviousDetailIndex++)
			{
                
				_MemoryDetail * PreviousDetail = PreviousDetailArray + PreviousDetailIndex;
				if (PreviousDetail->Start > PointerOffset) continue;
				if (PreviousDetail->End <= PointerOffset) continue;
                
				PointerRelativeToMemorySection = PointerOffset - PreviousDetail->Start;
				PointerOutOfScope = false;
                
				CurrentDetail = PreviousDetailMap[PreviousDetailIndex];
                
				if (!CurrentDetail)
				{
					DeadPointer = true;
					break;
				}
                
				OldPointerTypeName = PreviousTypeNameArray[CurrentDetail->OldDetailFromPreviousMemory->Type];
                
				if (CurrentDetail->IsPointer)
				{
					if (!PreviousDetail->IsPointer) ASSERT;
                    
					DataSize = sizeof(void *);
					BasicTypeOnly = true;
                    
					break;
				}
				else
				{
					_TypeFormDifference Result = CompareType(CurrentDetail->Type , PreviousDetail->Type);
                    
					if (Result.DifferentFormOfType) ASSERT;
					int PreviousDataSize = PreviousTypeSize[PreviousDetail->Type];
                    
					if (!Result.CurrentIsStruct)
					{
						if (PreviousDataSize != _TypeMetaSize[CurrentDetail->Type]) ASSERT;
						DataSize = PreviousDataSize;
                        
						BasicTypeOnly = true;
                        
						break;
					}
					else
					{
						int DataIndex = PointerRelativeToMemorySection / PreviousDataSize;
                        
						bool PointInsideStructDataType = true;
                        
						if ((PointerRelativeToMemorySection % PreviousDataSize) == 0)
						{
							if (!strcmp(CurrentTypeName , OldPointerTypeName))
							{
								PointInsideStructDataType = false;
							}
                            
							//TODO : void can point anything
							//so there is no way to know is it pointing to the head of the struct or the first member of the struct
							if (!strcmp(CurrentTypeName, "void"))
							{
								PointInsideStructDataType = false;
							}
						}
                        
						if (PointInsideStructDataType)
						{
							if (!strcmp(CurrentTypeName, "void"))
							{
								ASSERT;
							}
                            
							int PointerRelativeToData = PointerRelativeToMemorySection % PreviousDataSize;
							int NewRelativeToData = 0;
                            
							bool FoundNothing = true;
                            
							for (_DecomposedBasicType * PreviousType = PreviousBasicTypeHeadList[PreviousDetailIndex]; PreviousType; PreviousType = PreviousType->Next)
							{
								int PreviousTypeMemoryLocation = PreviousType->PreviousMemoryLocation - PreviousDetail->Start;
                                
								if (PointerRelativeToData == PreviousTypeMemoryLocation)
								{
									if (strcmp(CurrentTypeName , PreviousTypeNameArray[PreviousType->Type]))
									{
										bool NestStructFound = false;
                                        
										int ReverseNestedCount = 0;
										//int NestedMemberPreviousType = 0;
                                        
										for (_MemberNode * CurrentMemberNode = PreviousType->ReverseMemberList; CurrentMemberNode; CurrentMemberNode = CurrentMemberNode->Previous)
										{
                                            
											ReverseNestedCount++;
                                            
											if (!strcmp(PreviousTypeNameArray[CurrentMemberNode->Member->MemberType] , CurrentTypeName))
											{
												OldPointerTypeName = PreviousTypeNameArray[CurrentMemberNode->Member->MemberType];
												NestStructFound = true;
												//NestedMemberPreviousType = CurrentMemberNode->Member->MemberType;
												NewRelativeToData = PreviousType->CurrentMemoryLocation ;
												FoundNothing = false;
                                                
												break;
											}
										}
                                        
										if (!NestStructFound)
										{
											ASSERT;
										}
                                        
										//i have no fucking clue what this is
										//the first one should be the closet one
#if 0                                        
										for (_DecomposedBasicType * SecondPreviousType = PreviousBasicTypeHeadList[PreviousDetailIndex]; SecondPreviousType; SecondPreviousType = SecondPreviousType->Next)
										{
											int CurrentReverseNestedCount = 0;
                                            
											for (_MemberNode * CurrentMemberNode = SecondPreviousType->ReverseMemberList; CurrentMemberNode; CurrentMemberNode = CurrentMemberNode->Previous)
											{
												CurrentReverseNestedCount++;
                                                
												if (NestedMemberPreviousType == CurrentMemberNode->Member->MemberType)
												{
													if (ReverseNestedCount == CurrentReverseNestedCount)
													{
														if (NewRelativeToData > SecondPreviousType->CurrentMemoryLocation)
														{
															NewRelativeToData = SecondPreviousType->CurrentMemoryLocation;
															break;
														}
													}
                                                    
												}
											}
                                            
										}
#endif
									}
									else
									{
										NewRelativeToData = PreviousType->CurrentMemoryLocation;
										FoundNothing = false;
									}
                                    
									break;
								}
							}
                            
							//TODO : something broken? it can't patch pointer pointing inside struct
							if (FoundNothing)
							{
								ASSERT;
								DeadPointer = true;
							}
                            
							long long NewPointerLocation = (DataIndex * _TypeMetaSize[CurrentDetail->Type]);
							NewPointerLocation += NewRelativeToData;
                            
							if (NewPointerLocation > CurrentDetail->End)
							{								
								ASSERT;
								DeadPointer = true;
							}
							else
							{
#if SHOW_PATCH_POINTER
								printf("Pointer point inside struct (%s -> %s)", CurrentTypeName, OldPointerTypeName);
                                
								if ((*PointerToPatch) == (void*)(NewPointerLocation))
								{
									printf(" (Not Changed %lld)" , NewPointerLocation);
								}
								else
								{
									printf(" (%lld --> %lld)" , (long long)(*PointerToPatch), NewPointerLocation);
								}
                                
								printf("\n");
#endif
                                
								(*PointerToPatch) = (void*)NewPointerLocation;
							}
                            
							break;
						}
						else
						{
                            
							long long NewPointerLocation = CurrentDetail->Start + (DataIndex * _TypeMetaSize[CurrentDetail->Type]);
                            
							if (NewPointerLocation > CurrentDetail->End)
							{
								DeadPointer = true;
							}
							else
							{
#if SHOW_PATCH_POINTER
								printf("Pointer point to struct type (%s -> %s)" , CurrentTypeName, OldPointerTypeName);
                                
								if ((*PointerToPatch) == (void*)(NewPointerLocation))
								{
									printf(" (Not Changed %lld)" , NewPointerLocation);
								}
								else
								{
									printf(" (%lld --> %lld)" , (long long)(*PointerToPatch), NewPointerLocation);
								}
                                
								printf("\n");
#endif
                                
								(*PointerToPatch) = (void*)NewPointerLocation;
							}
                            
							break;
						}
					}
				}
                
				break;
			}
            
			if (PointerOutOfScope) ASSERT;
            
			if (BasicTypeOnly)
			{
				if (!CurrentDetail) ASSERT;
                
				if (DataSize == 0) ASSERT;
				if ((PointerRelativeToMemorySection % DataSize) != 0) ASSERT;
                
				if (CurrentNode->PreviousTypeStack[StackIndex] != CurrentDetail->OldDetailFromPreviousMemory->Type)
				{
					if (strcmp(OldPointerTypeName , "void"))
					{
						DeadPointer = true;
					}
				}
                
				if ((CurrentDetail->Start + PointerRelativeToMemorySection) > CurrentDetail->End)
				{
					DeadPointer = true;
				}
				else
				{
                    
#if SHOW_PATCH_POINTER
					printf("Pointer point to basic type (%s -> %s)" , CurrentTypeName, OldPointerTypeName);
					if ((*PointerToPatch) == (void*)(CurrentDetail->Start + PointerRelativeToMemorySection))
					{
						printf(" (Not Changed %lld)" , (CurrentDetail->Start + PointerRelativeToMemorySection));
					}
					else
					{
						printf(" (%lld --> %lld)" , (long long)(*PointerToPatch), CurrentDetail->Start + PointerRelativeToMemorySection);
					}
                    
					printf("\n");
#endif
                    
					(*PointerToPatch) = (void*)(CurrentDetail->Start + PointerRelativeToMemorySection);
				}
			}
            
			if (DeadPointer)
			{
#if SHOW_PATCH_POINTER
                
				printf("Pointer (current : %s) (previous : %s) point to dead memory\n" , CurrentTypeName, OldPointerTypeName);
#endif
                
				(*PointerToPatch) = 0;
			}
            
		}
	}
    
}
#endif

internal void LoadData(char * Name , _PersistMemory * TargetMemory)
{
    
	FILE* DataFile = fopen(CombineString(Name , ".dd"), "rb");
    
	if (DataFile)
	{
		fread(&PreviousMemorySize, sizeof(long long), 1, DataFile);
		PreviousSaveData = AllocateFrame_(PreviousMemorySize);
		fread(PreviousSaveData, PreviousMemorySize, 1, DataFile);
        
		fclose(DataFile);
        
		//this a bad idea ):
		//oh actually it isn't that bad!
		
		//if pointer can be patch that will be nice...
        
		//oh wait point is patched!
        
		FILE* SerializeInfoFile = fopen(CombineString(Name,"_MemoryLayoutAndTypeInfo.mti") , "rb");
        
#if 1
		
		CheckAndPatchData(TargetMemory, SerializeInfoFile);
		
#else
		memcpy(AppData->GameWorldSave.StartMemory , PreviousSaveData , PreviousMemorySize);
		AppData->GameWorldSave.Size = PreviousMemorySize;
		AppData->GameWorldSave.CurrentMemory = AppData->GameWorldSave.StartMemory + PreviousMemorySize;
#endif
        
	}
    
}

internal void SaveData(char * Name , _PersistMemory * TargetMemory)
{
    
	FILE* SerializeInfoFile = fopen(CombineString( Name , "_MemoryLayoutAndTypeInfo.mti"), "wb");
	
	int TypeCount = _MT_Type_Count;
    
	fwrite(&TypeCount, sizeof(int), 1, SerializeInfoFile);
    
	for (int TypeIndex = 0; TypeIndex < TypeCount; TypeIndex++)
	{
		_StructMetaData StructMeta = _TypeStructMeta_[TypeIndex];
		fwrite(&StructMeta.MemberCount , sizeof(int), 1 , SerializeInfoFile);
        
		fwrite(StructMeta.MembersArray , sizeof(_MemberMetaData), StructMeta.MemberCount , SerializeInfoFile);
        
		int StringSize = strlen(_TypeMetaName[TypeIndex]);
        
		fwrite(&StringSize , sizeof(int) , 1 , SerializeInfoFile);
        
		fwrite(_TypeMetaName[TypeIndex] , sizeof(char) , StringSize , SerializeInfoFile);
	}
    
	fwrite(_IsTypeEnum, sizeof(bool), TypeCount, SerializeInfoFile);
	fwrite(_TypeMetaSize, sizeof(int), TypeCount, SerializeInfoFile);
    
	fwrite(TargetMemory, sizeof(_PersistMemory) , 1, SerializeInfoFile);            
	for (_MemoryDetail* CurrentNode = TargetMemory->MemoryDetailHead; CurrentNode; CurrentNode = CurrentNode->NextNode)
	{
		fwrite(CurrentNode, sizeof(_MemoryDetail), 1, SerializeInfoFile);
	}
	
	FILE* DataFile = fopen(CombineString(Name , ".dd"), "wb");
    
	long long MemorySize = TargetMemory->CurrentOffset- TargetMemory->StartOffset;
    
	fwrite(&MemorySize, sizeof(long long), 1, DataFile);
	fwrite(_StartMemory_ + TargetMemory->StartOffset, MemorySize, 1, DataFile);
    
	fclose(SerializeInfoFile);
	fclose(DataFile);
}

#define AllocatePersistList(Data , NodeCount , TargetMemory) _AllocatePersistList(#Data , NodeCount , TargetMemory)

internal _List _AllocatePersistList( char * DataName , int NodeCount , _PersistMemory * TargetMemory)
{
	_List Result = {};
    
    _ListNode * NewNodeArray =(_ListNode *)AllocatePersistCustomName( DataName , _ListNode , NodeCount + DummyNodeCount , TargetMemory); 
	
    NewNodeArray[N_NodeHead].Next = N_NodeTail;
    NewNodeArray[N_NodeHead].Previous = -1;
    NewNodeArray[N_NodeHead].DataIndex = -1;
    
    NewNodeArray[N_NodeTail].Next = -1;
    NewNodeArray[N_NodeTail].Previous = N_NodeHead;
    NewNodeArray[N_NodeTail].DataIndex = -1;
    
    NewNodeArray[N_RecycledNodeHead].Next = N_RecycledNodeTail;
    NewNodeArray[N_RecycledNodeHead].Previous = -1;
    NewNodeArray[N_RecycledNodeHead].DataIndex = -1;
    
    NewNodeArray[N_RecycledNodeTail].Next = -1;
    NewNodeArray[N_RecycledNodeTail].Previous = N_RecycledNodeHead;
    NewNodeArray[N_RecycledNodeTail].DataIndex = -1;
    
    Result.NodeArray = (_ListNode persist * )(NewNodeArray); 
    
	return Result;
}

#define AllocatePersistHashTable(Data , NodeCount , TargetMemory) _AllocatePersistHashTable(#Data , NodeCount , TargetMemory)

internal _HashTable _AllocatePersistHashTable( char * DataName , int SlotCount , _PersistMemory * TargetMemory)
{
	_HashTable Result = {};
    
    _HashTableSlot * NewSlotArray = (_HashTableSlot *) AllocatePersistCustomName( DataName , _HashTableSlot , SlotCount , TargetMemory);
    Result.SlotArray = ( _HashTableSlot persist *)NewSlotArray;
    
    char EntryNameBuffer[128] ={};
    sprintf(EntryNameBuffer , "%s_EntryArray" , DataName);
    
	_HashTableEntry * NewEntryArray = (_HashTableEntry * )AllocatePersistCustomName( EntryNameBuffer , _HashTableEntry , SlotCount , TargetMemory);
	Result.EntryArray = (_HashTableEntry persist *)NewEntryArray;
    
    for (int EntryIndex = 0; EntryIndex < SlotCount; EntryIndex++)
	{
		Result.EntryArray[EntryIndex].HeadIndex = -1;
		Result.EntryArray[EntryIndex].TailIndex = -1;
	}
    
	return Result;
}

internal void CreateAWholeNewWorld()
{
	
    //TODO : the save isn't working well at all
    //maybe go backing using string?
    
	Editor = (Editor_Data*)AllocatePersist(Editor, Editor_Data ,1  ,&AppData->GameEditorData);
    
	Editor->TimelineScale = 1;
    
    Editor->BoneChildrenHashTable = AllocatePersistHashTable(Editor->BoneChildrenHashTable , BoneCount , &AppData->GameEditorData);
    Editor->BoneParentHashTable = AllocatePersistHashTable(Editor->BoneParentHashTable , BoneCount ,&AppData->GameEditorData);
    
	Editor->RootBoneList = AllocatePersistList(Editor->RootBoneList , BoneCount , &AppData->GameEditorData);
    
	//AllocatePersist( Data , Type , Count , TargetMemory) 
	//Editor->KeyFrameHash.Data = AllocatePersist_(sizeof(HashNode(KeyFrame)) , MAX_ANIM_HASH ,  "Editor->KeyFrameHash->Data" , TYPE_TO_ENUM(D_HashNode_KeyFrame), false , &AppData->GameEditorData );
    
	for (int BoneIndex =  0 ;BoneIndex < BoneCount ; BoneIndex++)
	{
		char * BoneName = (char*)EnumToString(BoneTag)[BoneIndex];
        
        _Bone persist * CurrentBone = (_Bone persist *)AllocatePersistCustomName(BoneName , _Bone , BoneCount , &AppData->GameEditorData);
        Editor->StickMan[BoneIndex] = CurrentBone;
        
        CurrentBone->BoneIndex = BoneIndex;
		CurrentBone->FirstKeyFrameOnPointerLeft_OrderListIndex = -1;
		CurrentBone->State.LocalRotation = QuaternionIdentity();
		CurrentBone->State.Direction = {0,1.0f,0};
        
        //TODO : this is annoying
		char NameBuffer[256] = {};
		char HashNameBuffer[256] = {};
		char ListNameBuffer[256] = {};
		char OrderListNameBuffer[256] = {};
		char DragListNameBuffer[256] = {};
        
		sprintf(NameBuffer , "BoneKeyFrameArray_%s" , BoneName);
		sprintf(HashNameBuffer , "BoneKeyFrameHashTable_%s" , BoneName);
		sprintf(ListNameBuffer , "BoneKeyFrameList_%s" , BoneName);
		sprintf(OrderListNameBuffer , "BoneKeyFrameOrderList_%s" , BoneName);
		sprintf(DragListNameBuffer , "BoneKeyDragFrameList_%s" , BoneName);
        
		CurrentBone->KeyFrameArray = (_KeyFrame persist * )AllocatePersistCustomName( NameBuffer , _KeyFrame , MAX_KEY_FRAME , &AppData->GameEditorData);
        
        CurrentBone->KeyFrameList = _AllocatePersistList(HashNameBuffer , MAX_KEY_FRAME , &AppData->GameEditorData);
        
        CurrentBone->KeyFrameOrderList = _AllocatePersistList(OrderListNameBuffer , MAX_KEY_FRAME , &AppData->GameEditorData);
		CurrentBone->KeyFrameHashTable = _AllocatePersistHashTable(ListNameBuffer  , MAX_KEY_FRAME , &AppData->GameEditorData);
		CurrentBone->DraggingKeyFrameList = _AllocatePersistList(DragListNameBuffer , MAX_KEY_FRAME , &AppData->GameEditorData);
        
    }
}