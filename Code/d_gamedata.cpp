//my ide can't read macro anymore
#ifndef global

#define global static
#define internal static

#define persist __based(_StartMemory_)

#endif

//i should have just use lambda

#define Local(ReturnType , Name ,VarArgsAndFunction ) \
struct _##Name##_DummyStruct { internal ReturnType Name VarArgsAndFunction  }; \
auto Name = _##Name##_DummyStruct##::##Name

global const char* GameTextureDataName = "Game\\GameTexture.gts";

global char * GameStateSaveName = "Game\\GameState";

global bool ShaderCompileFailed;

enum BoneTag
{
	B_Origin,
    
    B_RightHandController,
    B_RightArmPoleTarget,
    
    B_LeftArmPoleTarget,
    B_LeftHandController,
    
    B_RightLegController,
    B_RightLegPoleTarget,
    
    B_LeftLegController,
    B_LeftLegPoleTarget,
    
    B_BodyOrigin,
    
	B_Head,
    
	B_UpperBody,
	B_LowerBody,
    
	B_RightHand,
	B_RightUpperArm,
	B_RightLowerArm,
    
    B_LeftUpperArm,
	B_LeftLowerArm,
	B_LeftHand,
    
    B_Pelvic,
    
    B_LeftUpperLeg,
	B_LeftLowerLeg,
	B_LeftFoot,
    
    B_RightUpperLeg,
	B_RightLowerLeg,
    B_RightFoot,
    
    BoneCount,
};

struct _BoneState
{
	Vector3 LocalPosition;
	Quaternion LocalRotation;
	Vector3 Direction;
};

struct _KeyFrame
{
	_BoneState BoneState;
	int FrameIndex;
    
    //i don't think i should name it like this
    Vector3 PreviousBezierOffset;
    Vector3 NextBezierOffset;
    
    int NodeIndex;
    int OrderListNodeIndex;
};

//apparently you can move nearby slot that have hash collision before to deleted slot 
//sadly i can't move slot

//i choose link list because i can't think of a better way
//i can't swap thing

//this is terrible

struct _HashTableEntry
{
    //node chain entry	
	int HeadIndex;
	int TailIndex;
};

struct _HashTableSlot
{
	//node
	bool Valid;
    
	int SlotValue;
    
	int DataIndex;
    
	int NextIndex;
	int PreviousIndex;
};

struct _ListNode
{
	int DataIndex;
    
	int Next;
	int Previous;
};

enum NodeTag
{
    N_NodeHead,
    N_NodeTail,
    
    N_RecycledNodeHead,
    N_RecycledNodeTail,
    
    DummyNodeCount,
};

//the first node is where the chain start
//the second node is recycled node chain
struct _List
{
	
    _ListNode persist * NodeArray;
	int UnusedIndex;
};

struct _HashTable
{
    _HashTableEntry persist * EntryArray;
	_HashTableSlot persist * SlotArray;
};

enum RotationAxis
{
    R_None,
    
    R_Z,
    R_Y,
    R_X,
    
    R_Count,
};


struct RotationAxisData
{
    _Rect Rect;
    
    Color TargetAxisColor;
    float TargetAxisLineSize;
    
    Color AxisColor;
    float AxisLineSize;
};

struct _Bone
{
    
    //use for look up parent and children bone only
    //try not forget updating it
    int BoneIndex;
    
    //don't use this
    //only use for patching
    char BoneName[64];
    
	_BoneState State;
	
	Vector3 Position;
	Vector3 EndPosition;
	Quaternion Rotation;
    
    bool FreePosition;
	bool FreeBone;
    
    bool Hovered;
    bool Selected;
    
	//hash make display more easier
	//these thing can bundle together
    
    //i'm using list to mange data now?
	_KeyFrame persist * KeyFrameArray;
	_List KeyFrameList;
	
    int FirstKeyFrameOnPointerLeft_OrderListIndex;
    _List KeyFrameOrderList;
    
	_HashTable KeyFrameHashTable;
	
	_List DraggingKeyFrameList;
    
};

global float CameraCurrentZoom = 1.0;

enum GameMenuType
{
	GMT_Descend,
	GMT_ToTheRight,
};

struct DrawingMenu
{
	Vector2 CurrentButtonPosition;
	bool OnScreen;
    
	float CurrentButtonLeft;
    
	bool ButtonHover;
	bool ButtonClicked;
    
	int MenuType;
	float ButtonExtraOffset;
};

#define MAX_KEY_FRAME 256
#define FRAME_PER_SECOND 60
#define FRAME_TIME 1.0 / ((double)FRAME_PER_SECOND)

struct Editor_Data
{
    bool ControlRig;
    
	int PointerFrameIndex;
	_Bone persist * StickMan[BoneCount];
	_List RootBoneList;
    
    _HashTable BoneParentHashTable;
	_HashTable BoneChildrenHashTable;
	
    bool SelectingBezier;
    
    int SelectedBoneCount;
    int SelectedBoneStack[BoneCount];
    
	bool Playing;
	float PlayTimer;
    
	float TimelineSliderOffset;
    
	int StartFrameIndex;
	int EndFrameIndex;
    
	float TimelineScale;
    
	float TargetCameraZoom;
	float CurrentCameraZoom;
	
    Vector3 Right;
    Vector3 Up;
    Quaternion BillboardRotation;
    
	Vector3 CameraEuler;
	Vector3 CameraOffset;
	
	Vector2 CurrentMousePosition;
	Vector2 PreviousMousePosition;
	Vector2 OperateMenuPosition;
    
	bool FlatColor;
    
};

global Vector2 * ModifyingMenuPosition = {};
global Matrix ScreenMVP = {};
global Matrix MVP_3DWorld = {};
global Matrix ViewMatrix = {};
global Matrix ProjectMatrix = {};

global Editor_Data * Editor = 0;

global long long GameUpdateCount = 0;

global float GridSize = 0.5f;
