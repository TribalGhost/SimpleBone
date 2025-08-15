
#line 2 "../Code/d_game.cpp"
//this is dumb
#line 3 "../Code/d_game.cpp"
#define FILE_TO_STRING(Str) #Str
#line 4 "../Code/d_game.cpp"
#define _GENERATED_FILE(FileName , ExtensionName) FILE_TO_STRING(##FileName##ExtensionName)
#line 5 "../Code/d_game.cpp"

#line 6 "../Code/d_game.cpp"
#ifdef SWITCH_TO_GENERATED_FILE
#line 7 "../Code/d_game.cpp"
#define _GENERATED_FILE(FileName , ExtensionName) FILE_TO_STRING(..\CodeGenerationbuild\\##FileName##_target_to_d_game_generated##ExtensionName)
#line 8 "../Code/d_game.cpp"
#endif
#line 9 "../Code/d_game.cpp"

#line 10 "../Code/d_game.cpp"
#define GENERATED_FILE(FileName , ExtensionName) _GENERATED_FILE(FileName , ExtensionName)
#line 11 "../Code/d_game.cpp"

#line 12 "../Code/d_game.cpp"
#include GENERATED_FILE( d_header , .h)
#line 13 "../Code/d_game.cpp"
#include GENERATED_FILE( d_main , .h)
#line 14 "../Code/d_game.cpp"

#line 15 "../Code/d_game.cpp"
#include GENERATED_FILE( d_renderdata, .cpp)
#line 16 "../Code/d_game.cpp"
#include GENERATED_FILE( d_gamedata , .cpp)
#line 17 "../Code/d_game.cpp"

#line 18 "../Code/d_game.cpp"
#include "d_game_meta_generated.cpp"
#line 19 "../Code/d_game.cpp"

#line 20 "../Code/d_game.cpp"
#include "d_function.cpp"
#line 21 "../Code/d_game.cpp"

#line 22 "../Code/d_game.cpp"
#include "d_render.cpp"
#line 23 "../Code/d_game.cpp"
#include "d_text.cpp"
#line 24 "../Code/d_game.cpp"

#line 25 "../Code/d_game.cpp"
#include "d_gamefunction.cpp"
#line 26 "../Code/d_game.cpp"

#line 27 "../Code/d_game.cpp"
internal bool CheckSelectedBoneRotation(int SingleBoneIndex)
#line 28 "../Code/d_game.cpp"
{
#line 29 "../Code/d_game.cpp"
    if(IsRig(SingleBoneIndex)) return false;
#line 30 "../Code/d_game.cpp"
    
#line 31 "../Code/d_game.cpp"
    _Bone * SelectedBone = Editor->StickMan[SingleBoneIndex];
#line 32 "../Code/d_game.cpp"
    
#line 33 "../Code/d_game.cpp"
    //RotateAxisPosition = Vector3Subtract(RotateAxisPosition , Vector3RotateByQuaternion(SelectedBone->State.Direction , SelectedBone->Rotation));
#line 34 "../Code/d_game.cpp"
    
#line 35 "../Code/d_game.cpp"
    _Rect RotationRect[R_Count] = {};
#line 36 "../Code/d_game.cpp"
    RayCollision RotationRectCollision[R_Count] = {};
#line 37 "../Code/d_game.cpp"
    
#line 38 "../Code/d_game.cpp"
    RotationRect[R_Z] = GetRect();
#line 39 "../Code/d_game.cpp"
    RotationRect[R_Z].Size = { 1, 1};
#line 40 "../Code/d_game.cpp"
    RotationRect[R_Z].Position = SelectedBone->Position;
#line 41 "../Code/d_game.cpp"
    RotationRect[R_Z].Rotation = SelectedBone->Rotation;
#line 42 "../Code/d_game.cpp"
    
#line 43 "../Code/d_game.cpp"
    RotationRect[R_Y] = RotationRect[R_Z];
#line 44 "../Code/d_game.cpp"
    RotationRect[R_Y].Rotation = QuaternionMultiply(RotationRect[R_Z].Rotation , QuaternionFromEuler(DEG2RAD * 90,0,0));
#line 45 "../Code/d_game.cpp"
    
#line 46 "../Code/d_game.cpp"
    RotationRect[R_X] = RotationRect[R_Z];
#line 47 "../Code/d_game.cpp"
    RotationRect[R_X].Rotation = QuaternionMultiply(RotationRect[R_Z].Rotation , QuaternionFromEuler(0,DEG2RAD * 90,0));
#line 48 "../Code/d_game.cpp"
    
#line 49 "../Code/d_game.cpp"
    bool Hovering = false;
#line 50 "../Code/d_game.cpp"
    for(int AxisIndex = R_Z ; AxisIndex < R_Count ; AxisIndex++)
#line 51 "../Code/d_game.cpp"
    {
#line 52 "../Code/d_game.cpp"
        RotationRectCollision[AxisIndex] = GetCollisionRect3D(RotationRect[AxisIndex]);
#line 53 "../Code/d_game.cpp"
        if(RotationRectCollision[AxisIndex].hit)
#line 54 "../Code/d_game.cpp"
        {
#line 55 "../Code/d_game.cpp"
            Hovering = true;
#line 56 "../Code/d_game.cpp"
        }
#line 57 "../Code/d_game.cpp"
    }
#line 58 "../Code/d_game.cpp"
    
#line 59 "../Code/d_game.cpp"
    localPersist Vector3 PreviousDragPoint = {};
#line 60 "../Code/d_game.cpp"
    localPersist int DraggingAxis = R_None;
#line 61 "../Code/d_game.cpp"
    
#line 62 "../Code/d_game.cpp"
    if(MouseButtonPressed(MOUSE_BUTTON_LEFT))
#line 63 "../Code/d_game.cpp"
    {
#line 64 "../Code/d_game.cpp"
        DraggingAxis = R_None;
#line 65 "../Code/d_game.cpp"
    }
#line 66 "../Code/d_game.cpp"
    
#line 67 "../Code/d_game.cpp"
    RayCollision ClosestCollision = {};
#line 68 "../Code/d_game.cpp"
    ClosestCollision.distance = FLT_MAX;
#line 69 "../Code/d_game.cpp"
    
#line 70 "../Code/d_game.cpp"
    Color AxisColor = {};
#line 71 "../Code/d_game.cpp"
    if(DraggingAxis == R_Z) AxisColor = BLUE;
#line 72 "../Code/d_game.cpp"
    if(DraggingAxis == R_Y) AxisColor = GREEN;
#line 73 "../Code/d_game.cpp"
    if(DraggingAxis == R_X) AxisColor = RED;
#line 74 "../Code/d_game.cpp"
    
#line 75 "../Code/d_game.cpp"
    DrawRectLine(RotationRect[DraggingAxis] , AxisColor , 20);
#line 76 "../Code/d_game.cpp"
    
#line 77 "../Code/d_game.cpp"
    for(int AxisIndex = R_Z ; AxisIndex < R_Count ; AxisIndex++)
#line 78 "../Code/d_game.cpp"
    {
#line 79 "../Code/d_game.cpp"
        bool FoundClosestAxis = true;
#line 80 "../Code/d_game.cpp"
        
#line 81 "../Code/d_game.cpp"
        if(!RotationRectCollision[AxisIndex].hit) FoundClosestAxis = false;
#line 82 "../Code/d_game.cpp"
        if(RotationRectCollision[AxisIndex].distance > ClosestCollision.distance) FoundClosestAxis = false;
#line 83 "../Code/d_game.cpp"
        
#line 84 "../Code/d_game.cpp"
        if(FoundClosestAxis)
#line 85 "../Code/d_game.cpp"
        {
#line 86 "../Code/d_game.cpp"
            
#line 87 "../Code/d_game.cpp"
            if(MouseButtonPressed(MOUSE_BUTTON_LEFT))
#line 88 "../Code/d_game.cpp"
            {
#line 89 "../Code/d_game.cpp"
                PreviousDragPoint = RotationRectCollision[AxisIndex].point;
#line 90 "../Code/d_game.cpp"
                DraggingAxis = AxisIndex;
#line 91 "../Code/d_game.cpp"
            }
#line 92 "../Code/d_game.cpp"
            
#line 93 "../Code/d_game.cpp"
            ClosestCollision = RotationRectCollision[AxisIndex];
#line 94 "../Code/d_game.cpp"
        }
#line 95 "../Code/d_game.cpp"
    }
#line 96 "../Code/d_game.cpp"
    
#line 97 "../Code/d_game.cpp"
    if(!MouseButtonPressing(MOUSE_BUTTON_LEFT))
#line 98 "../Code/d_game.cpp"
    {
#line 99 "../Code/d_game.cpp"
        for(int AxisIndex = R_Z ; AxisIndex < R_Count ; AxisIndex++)
#line 100 "../Code/d_game.cpp"
        {
#line 101 "../Code/d_game.cpp"
            DrawRectLine(RotationRect[AxisIndex] , Fade(WHITE , 0.2 ) , 20);
#line 102 "../Code/d_game.cpp"
        }
#line 103 "../Code/d_game.cpp"
        
#line 104 "../Code/d_game.cpp"
        DraggingAxis = R_None;
#line 105 "../Code/d_game.cpp"
        
#line 106 "../Code/d_game.cpp"
    }
#line 107 "../Code/d_game.cpp"
    else
#line 108 "../Code/d_game.cpp"
    {
#line 109 "../Code/d_game.cpp"
        if (SelectedBone->FreeBone)
#line 110 "../Code/d_game.cpp"
        {
#line 111 "../Code/d_game.cpp"
            
#line 112 "../Code/d_game.cpp"
            _Quad BoneCentreQuad = GetBillboardQuad(SelectedBone->Position, 100000.0f, 100000.0f);
#line 113 "../Code/d_game.cpp"
            Vector3 HitPoint = GetCollisionQuad3D(BoneCentreQuad).point;
#line 114 "../Code/d_game.cpp"
            
#line 115 "../Code/d_game.cpp"
            _Rect AxisRect = RotationRect[DraggingAxis];
#line 116 "../Code/d_game.cpp"
            
#line 117 "../Code/d_game.cpp"
            AxisRect.Size = {10000.0,10000.0};
#line 118 "../Code/d_game.cpp"
            
#line 119 "../Code/d_game.cpp"
            HitPoint = GetCollisionRect3D(AxisRect).point;
#line 120 "../Code/d_game.cpp"
            
#line 121 "../Code/d_game.cpp"
            Vector3 BonePosition = AxisRect.Position;
#line 122 "../Code/d_game.cpp"
            
#line 123 "../Code/d_game.cpp"
            Quaternion RotationOffset = QuaternionIdentity();
#line 124 "../Code/d_game.cpp"
            
#line 125 "../Code/d_game.cpp"
            Quaternion InvertAxisRotation = QuaternionInvert(AxisRect.Rotation);
#line 126 "../Code/d_game.cpp"
            
#line 127 "../Code/d_game.cpp"
            Vector3 DirectionToPreviousPoint = Vector3Subtract(PreviousDragPoint , BonePosition);
#line 128 "../Code/d_game.cpp"
            DirectionToPreviousPoint = Vector3RotateByQuaternion(DirectionToPreviousPoint , InvertAxisRotation);
#line 129 "../Code/d_game.cpp"
            
#line 130 "../Code/d_game.cpp"
            Vector3 DirectionToPoint = Vector3Subtract(HitPoint , BonePosition);
#line 131 "../Code/d_game.cpp"
            DirectionToPoint = Vector3RotateByQuaternion(DirectionToPoint , InvertAxisRotation);
#line 132 "../Code/d_game.cpp"
            
#line 133 "../Code/d_game.cpp"
            float PreviousAngle = atan2f(DirectionToPreviousPoint.y , DirectionToPreviousPoint.x);
#line 134 "../Code/d_game.cpp"
            float CurrentAngle = atan2f(DirectionToPoint.y , DirectionToPoint.x);
#line 135 "../Code/d_game.cpp"
            
#line 136 "../Code/d_game.cpp"
            Vector3 Axis = {};
#line 137 "../Code/d_game.cpp"
            
#line 138 "../Code/d_game.cpp"
            if (DraggingAxis == R_Z) Axis = {0,0,1};
#line 139 "../Code/d_game.cpp"
            if (DraggingAxis == R_Y) Axis = {0,-1,0};
#line 140 "../Code/d_game.cpp"
            if (DraggingAxis == R_X) Axis = {1,0,0};
#line 141 "../Code/d_game.cpp"
            
#line 142 "../Code/d_game.cpp"
            RotationOffset = QuaternionFromAxisAngle(Axis, CurrentAngle - PreviousAngle);
#line 143 "../Code/d_game.cpp"
            
#line 144 "../Code/d_game.cpp"
            SelectedBone->State.LocalRotation = QuaternionMultiply(SelectedBone->State.LocalRotation, RotationOffset);
#line 145 "../Code/d_game.cpp"
            //SelectedBone->State.LocalRotation = QuaternionNormalize(SelectedBone->State.LocalRotation);
#line 146 "../Code/d_game.cpp"
            //SelectedBone->State.LocalRotation = QuaternionIdentity();
#line 147 "../Code/d_game.cpp"
            
#line 148 "../Code/d_game.cpp"
            //printf("Hit point : %f %f %f\n" ,HitPoint.x , HitPoint.y , HitPoint.z);
#line 149 "../Code/d_game.cpp"
            
#line 150 "../Code/d_game.cpp"
            PreviousDragPoint = HitPoint;
#line 151 "../Code/d_game.cpp"
        }
#line 152 "../Code/d_game.cpp"
    }
#line 153 "../Code/d_game.cpp"
    
#line 154 "../Code/d_game.cpp"
    return Hovering;
#line 155 "../Code/d_game.cpp"
}
#line 156 "../Code/d_game.cpp"

#line 157 "../Code/d_game.cpp"
internal void BoneSelection()
#line 158 "../Code/d_game.cpp"
{
#line 159 "../Code/d_game.cpp"
    localPersist bool RedragSelection = true;
#line 160 "../Code/d_game.cpp"
    localPersist bool DraggingSelection = false;
#line 161 "../Code/d_game.cpp"
    
#line 162 "../Code/d_game.cpp"
                                               
                              
     
                              
                                       
                              
                                                 
                              
        
                              
                    
                              
     
#line 169 "../Code/d_game.cpp"
    
#line 170 "../Code/d_game.cpp"
    MouseButtonBlocker = SelectionCheck;
#line 171 "../Code/d_game.cpp"
    
#line 172 "../Code/d_game.cpp"
    if(MouseButtonPressedWithCheck(MOUSE_BUTTON_LEFT))
#line 173 "../Code/d_game.cpp"
    {
#line 174 "../Code/d_game.cpp"
        RedragSelection = true;
#line 175 "../Code/d_game.cpp"
    }
#line 176 "../Code/d_game.cpp"
    
#line 177 "../Code/d_game.cpp"
    //CurveVisualization
#line 178 "../Code/d_game.cpp"
    Editor->SelectingBezier = false;
#line 179 "../Code/d_game.cpp"
    
#line 180 "../Code/d_game.cpp"
    localPersist bool ModifyingPosition = false;
#line 181 "../Code/d_game.cpp"
    localPersist bool SelectedPreviousBezierPoint = false;
#line 182 "../Code/d_game.cpp"
    localPersist int SelectedBezierKeyFrameIndex = 0;
#line 183 "../Code/d_game.cpp"
    localPersist int SelectedBezierOtherKeyFrameIndex= 0;
#line 184 "../Code/d_game.cpp"
    localPersist _Bone * SelectedBezierBone = 0;
#line 185 "../Code/d_game.cpp"
    
#line 186 "../Code/d_game.cpp"
    for(int StackIndex = 0 ; StackIndex < Editor->SelectedBoneCount ; StackIndex++)
#line 187 "../Code/d_game.cpp"
    {
#line 188 "../Code/d_game.cpp"
        
#line 189 "../Code/d_game.cpp"
        int SelectedBoneIndex = Editor->SelectedBoneStack[StackIndex];
#line 190 "../Code/d_game.cpp"
        _Bone * SelectedBone = Editor->StickMan[SelectedBoneIndex];
#line 191 "../Code/d_game.cpp"
        _KeyFrame * KeyFrameArray = (_KeyFrame * )SelectedBone->KeyFrameArray;
#line 192 "../Code/d_game.cpp"
        
#line 193 "../Code/d_game.cpp"
        //TODO : i have no idea where this offset is coming from
#line 194 "../Code/d_game.cpp"
        //too bad
#line 195 "../Code/d_game.cpp"
        
#line 196 "../Code/d_game.cpp"
        //the offset was from the direction of the origin
#line 197 "../Code/d_game.cpp"
        //controllers still geting offset from it
#line 198 "../Code/d_game.cpp"
        
#line 199 "../Code/d_game.cpp"
        //Vector3 ParentOffset = SelectedBone->Position;
#line 200 "../Code/d_game.cpp"
        //ParentOffset = Vector3Subtract(ParentOffset , SelectedBone->State.LocalPosition);
#line 201 "../Code/d_game.cpp"
        //ParentOffset = {};
#line 202 "../Code/d_game.cpp"
        
#line 203 "../Code/d_game.cpp"
        int PreviousKeyFrameIndex = GetPreviousDataIndexFromNodeIndex(N_NodeTail , &SelectedBone->KeyFrameOrderList);
#line 204 "../Code/d_game.cpp"
        _KeyFrame * PreviousKeyFrame = KeyFrameArray + PreviousKeyFrameIndex;
#line 205 "../Code/d_game.cpp"
        
#line 206 "../Code/d_game.cpp"
        Color KeyFrameLineColor = Fade(WHITE , 0.2f);
#line 207 "../Code/d_game.cpp"
        
#line 208 "../Code/d_game.cpp"
        List_Foreach( KeyFrameIndex , &SelectedBone->KeyFrameOrderList)
#line 209 "../Code/d_game.cpp"
        {
#line 210 "../Code/d_game.cpp"
            
#line 211 "../Code/d_game.cpp"
            _KeyFrame * KeyFrame = KeyFrameArray + KeyFrameIndex;
#line 212 "../Code/d_game.cpp"
            Vector3 PreviousKeyFramePosition = PreviousKeyFrame->BoneState.LocalPosition;
#line 213 "../Code/d_game.cpp"
            Vector3 KeyFramePosition = KeyFrame->BoneState.LocalPosition;
#line 214 "../Code/d_game.cpp"
            
#line 215 "../Code/d_game.cpp"
            Vector3 LineDirection = Vector3Subtract(KeyFramePosition , PreviousKeyFramePosition);
#line 216 "../Code/d_game.cpp"
            LineDirection = Vector3Scale(Vector3Normalize(LineDirection),0.1f);
#line 217 "../Code/d_game.cpp"
            
#line 218 "../Code/d_game.cpp"
            Vector3 PreviousKeyFrameBezierPoint = Vector3Add(LineDirection , PreviousKeyFrame->NextBezierOffset);
#line 219 "../Code/d_game.cpp"
            PreviousKeyFrameBezierPoint = Vector3Add(PreviousKeyFrameBezierPoint , PreviousKeyFramePosition);
#line 220 "../Code/d_game.cpp"
            
#line 221 "../Code/d_game.cpp"
            Vector3 KeyFrameBezierPoint = Vector3Add(Vector3Negate(LineDirection) , KeyFrame->PreviousBezierOffset);
#line 222 "../Code/d_game.cpp"
            KeyFrameBezierPoint = Vector3Add(KeyFrameBezierPoint , KeyFramePosition);
#line 223 "../Code/d_game.cpp"
            
#line 224 "../Code/d_game.cpp"
            DrawRoundLine(PreviousKeyFramePosition , PreviousKeyFrameBezierPoint , 10 , Fade(BLUE,0.5f) );
#line 225 "../Code/d_game.cpp"
            DrawRoundLine(KeyFramePosition , KeyFrameBezierPoint , 10 , Fade(RED,0.5f) );
#line 226 "../Code/d_game.cpp"
            
#line 227 "../Code/d_game.cpp"
            _Rect PreviousBezierPointRect = GetBillboardRect(PreviousKeyFrameBezierPoint , 0.1);
#line 228 "../Code/d_game.cpp"
            _Rect BezierPointRect = GetBillboardRect(KeyFrameBezierPoint , 0.1);
#line 229 "../Code/d_game.cpp"
            
#line 230 "../Code/d_game.cpp"
            Color PreviousBezierPointColor = Fade(BLUE , 0.5f);
#line 231 "../Code/d_game.cpp"
            Color BezierPointColor = Fade(RED , 0.5f);
#line 232 "../Code/d_game.cpp"
            Color KeyFramePointColor = Fade(YELLOW , 0.5f);
#line 233 "../Code/d_game.cpp"
            
#line 234 "../Code/d_game.cpp"
            if(CheckCollisionRect3D(PreviousBezierPointRect))
#line 235 "../Code/d_game.cpp"
            {
#line 236 "../Code/d_game.cpp"
                PreviousBezierPointColor = BLUE;
#line 237 "../Code/d_game.cpp"
                if(MouseButtonPressedWithCheck(MOUSE_BUTTON_LEFT))
#line 238 "../Code/d_game.cpp"
                {
#line 239 "../Code/d_game.cpp"
                    SelectedBezierKeyFrameIndex = PreviousKeyFrameIndex;
#line 240 "../Code/d_game.cpp"
                    SelectedBezierOtherKeyFrameIndex = KeyFrameIndex;
#line 241 "../Code/d_game.cpp"
                    SelectedBezierBone = SelectedBone;
#line 242 "../Code/d_game.cpp"
                    SelectedPreviousBezierPoint = false;
#line 243 "../Code/d_game.cpp"
                    
#line 244 "../Code/d_game.cpp"
                }
#line 245 "../Code/d_game.cpp"
                
#line 246 "../Code/d_game.cpp"
            }
#line 247 "../Code/d_game.cpp"
            
#line 248 "../Code/d_game.cpp"
            if(CheckCollisionRect3D(BezierPointRect))
#line 249 "../Code/d_game.cpp"
            {
#line 250 "../Code/d_game.cpp"
                BezierPointColor = RED;
#line 251 "../Code/d_game.cpp"
                if(MouseButtonPressedWithCheck(MOUSE_BUTTON_LEFT))
#line 252 "../Code/d_game.cpp"
                {
#line 253 "../Code/d_game.cpp"
                    
#line 254 "../Code/d_game.cpp"
                    SelectedBezierKeyFrameIndex = KeyFrameIndex;
#line 255 "../Code/d_game.cpp"
                    SelectedBezierOtherKeyFrameIndex = PreviousKeyFrameIndex;
#line 256 "../Code/d_game.cpp"
                    SelectedBezierBone = SelectedBone;
#line 257 "../Code/d_game.cpp"
                    SelectedPreviousBezierPoint = true;
#line 258 "../Code/d_game.cpp"
                    
#line 259 "../Code/d_game.cpp"
                }
#line 260 "../Code/d_game.cpp"
            }
#line 261 "../Code/d_game.cpp"
            
#line 262 "../Code/d_game.cpp"
            DrawCircle( PreviousBezierPointRect , PreviousBezierPointColor);
#line 263 "../Code/d_game.cpp"
            DrawCircle( BezierPointRect , BezierPointColor);
#line 264 "../Code/d_game.cpp"
            
#line 265 "../Code/d_game.cpp"
            int FrameCount = GetKeyFrameCount(PreviousKeyFrame , KeyFrame);
#line 266 "../Code/d_game.cpp"
            
#line 267 "../Code/d_game.cpp"
            for(int FrameIndex = 0 ; FrameIndex < FrameCount ; FrameIndex++)
#line 268 "../Code/d_game.cpp"
            {
#line 269 "../Code/d_game.cpp"
                float BezierStartTime = ((float)FrameIndex) / ((float)FrameCount);
#line 270 "../Code/d_game.cpp"
                float BezierEndTime = ((float)(FrameIndex+1)) / ((float)FrameCount);
#line 271 "../Code/d_game.cpp"
                
#line 272 "../Code/d_game.cpp"
                Vector3 BezierStartPoint = Vector3CubicBezier(PreviousKeyFramePosition , PreviousKeyFrameBezierPoint , KeyFrameBezierPoint, KeyFramePosition  , BezierStartTime);
#line 273 "../Code/d_game.cpp"
                Vector3 BezierEndPoint = Vector3CubicBezier(PreviousKeyFramePosition , PreviousKeyFrameBezierPoint , KeyFrameBezierPoint, KeyFramePosition  , BezierEndTime);
#line 274 "../Code/d_game.cpp"
                
#line 275 "../Code/d_game.cpp"
                DrawRoundLine(BezierStartPoint , BezierEndPoint ,10 ,KeyFrameLineColor );
#line 276 "../Code/d_game.cpp"
            }
#line 277 "../Code/d_game.cpp"
            
#line 278 "../Code/d_game.cpp"
            DrawRoundLine(PreviousKeyFramePosition , KeyFramePosition,10 ,Fade(WHITE , 0.05f));
#line 279 "../Code/d_game.cpp"
            
#line 280 "../Code/d_game.cpp"
            _Rect KeyFrameRect = GetBillboardRect(KeyFramePosition , 0.1 );
#line 281 "../Code/d_game.cpp"
            
#line 282 "../Code/d_game.cpp"
            if(CheckCollisionRect3D(KeyFrameRect))
#line 283 "../Code/d_game.cpp"
            {
#line 284 "../Code/d_game.cpp"
                KeyFramePointColor = YELLOW;
#line 285 "../Code/d_game.cpp"
                
#line 286 "../Code/d_game.cpp"
                if(MouseButtonPressedWithCheck(MOUSE_BUTTON_LEFT))
#line 287 "../Code/d_game.cpp"
                {
#line 288 "../Code/d_game.cpp"
                    ModifyingPosition = true;
#line 289 "../Code/d_game.cpp"
                    SelectedBezierBone = SelectedBone;
#line 290 "../Code/d_game.cpp"
                    SelectedBezierKeyFrameIndex = KeyFrameIndex;
#line 291 "../Code/d_game.cpp"
                }
#line 292 "../Code/d_game.cpp"
            }
#line 293 "../Code/d_game.cpp"
            
#line 294 "../Code/d_game.cpp"
            DrawCircle(KeyFrameRect , KeyFramePointColor);
#line 295 "../Code/d_game.cpp"
            
#line 296 "../Code/d_game.cpp"
            PreviousKeyFrameIndex = KeyFrameIndex;
#line 297 "../Code/d_game.cpp"
            PreviousKeyFrame = KeyFrame;
#line 298 "../Code/d_game.cpp"
        }
#line 299 "../Code/d_game.cpp"
        
#line 300 "../Code/d_game.cpp"
        if(IsRig(SelectedBoneIndex))
#line 301 "../Code/d_game.cpp"
        {
#line 302 "../Code/d_game.cpp"
            DrawBillboardCircle(SelectedBone->Position , 0.1 , Fade(WHITE,0.2));
#line 303 "../Code/d_game.cpp"
        }
#line 304 "../Code/d_game.cpp"
        
#line 305 "../Code/d_game.cpp"
    }
#line 306 "../Code/d_game.cpp"
    
#line 307 "../Code/d_game.cpp"
    if(MouseButtonReleased(MOUSE_BUTTON_LEFT))
#line 308 "../Code/d_game.cpp"
    {
#line 309 "../Code/d_game.cpp"
        ModifyingPosition = false;
#line 310 "../Code/d_game.cpp"
        SelectedBezierBone = 0;
#line 311 "../Code/d_game.cpp"
    }
#line 312 "../Code/d_game.cpp"
    
#line 313 "../Code/d_game.cpp"
    if(SelectedBezierBone)
#line 314 "../Code/d_game.cpp"
    {
#line 315 "../Code/d_game.cpp"
        localPersist Vector3 DraggingPlanePoint = {};
#line 316 "../Code/d_game.cpp"
        localPersist Vector3 PreviousDraggingPoint = {};
#line 317 "../Code/d_game.cpp"
        
#line 318 "../Code/d_game.cpp"
        Editor->SelectingBezier = true;
#line 319 "../Code/d_game.cpp"
        _KeyFrame * KeyFrameArray = (_KeyFrame *)SelectedBezierBone->KeyFrameArray;
#line 320 "../Code/d_game.cpp"
        
#line 321 "../Code/d_game.cpp"
        _KeyFrame * KeyFrame = KeyFrameArray + SelectedBezierKeyFrameIndex;
#line 322 "../Code/d_game.cpp"
        
#line 323 "../Code/d_game.cpp"
        if(ModifyingPosition)
#line 324 "../Code/d_game.cpp"
        {
#line 325 "../Code/d_game.cpp"
            
#line 326 "../Code/d_game.cpp"
            if(MouseButtonPressed(MOUSE_BUTTON_LEFT))
#line 327 "../Code/d_game.cpp"
            {
#line 328 "../Code/d_game.cpp"
                RedragSelection = false;
#line 329 "../Code/d_game.cpp"
                DraggingPlanePoint = KeyFrame->BoneState.LocalPosition;
#line 330 "../Code/d_game.cpp"
                PreviousDraggingPoint = KeyFrame->BoneState.LocalPosition;
#line 331 "../Code/d_game.cpp"
            }
#line 332 "../Code/d_game.cpp"
            
#line 333 "../Code/d_game.cpp"
            //TODO : it just snap to the point instead adding offset
#line 334 "../Code/d_game.cpp"
            Vector3 DraggingPoint = GetCollisionQuad3D(GetBillboardQuad(DraggingPlanePoint , 100000,100000)).point;
#line 335 "../Code/d_game.cpp"
            
#line 336 "../Code/d_game.cpp"
            Vector3 NewKeyFrameOffset = Vector3Subtract(DraggingPoint , PreviousDraggingPoint);
#line 337 "../Code/d_game.cpp"
            PreviousDraggingPoint = DraggingPoint;
#line 338 "../Code/d_game.cpp"
            
#line 339 "../Code/d_game.cpp"
            //TODO : it is being overwritten by something else
#line 340 "../Code/d_game.cpp"
            if(Editor->PointerFrameIndex == KeyFrame->FrameIndex)
#line 341 "../Code/d_game.cpp"
            {
#line 342 "../Code/d_game.cpp"
                SelectedBezierBone->State.LocalPosition = Vector3Add(SelectedBezierBone->State.LocalPosition , NewKeyFrameOffset);
#line 343 "../Code/d_game.cpp"
            }
#line 344 "../Code/d_game.cpp"
            else
#line 345 "../Code/d_game.cpp"
            {
#line 346 "../Code/d_game.cpp"
                KeyFrame->BoneState.LocalPosition = Vector3Add(KeyFrame->BoneState.LocalPosition , NewKeyFrameOffset);
#line 347 "../Code/d_game.cpp"
            }
#line 348 "../Code/d_game.cpp"
            
#line 349 "../Code/d_game.cpp"
        }
#line 350 "../Code/d_game.cpp"
        else
#line 351 "../Code/d_game.cpp"
        {
#line 352 "../Code/d_game.cpp"
            _KeyFrame * OtherKeyFrame = 0;
#line 353 "../Code/d_game.cpp"
            
#line 354 "../Code/d_game.cpp"
            Vector3 StartPoint = {};
#line 355 "../Code/d_game.cpp"
            Vector3 EndPoint = {};
#line 356 "../Code/d_game.cpp"
            Vector3 BezierOffset = {};
#line 357 "../Code/d_game.cpp"
            
#line 358 "../Code/d_game.cpp"
            if(SelectedPreviousBezierPoint)
#line 359 "../Code/d_game.cpp"
            {
#line 360 "../Code/d_game.cpp"
                
#line 361 "../Code/d_game.cpp"
                BezierOffset = KeyFrame->PreviousBezierOffset;
#line 362 "../Code/d_game.cpp"
                OtherKeyFrame = KeyFrameArray + SelectedBezierOtherKeyFrameIndex;
#line 363 "../Code/d_game.cpp"
                
#line 364 "../Code/d_game.cpp"
                StartPoint = KeyFrame->BoneState.LocalPosition;
#line 365 "../Code/d_game.cpp"
                EndPoint = OtherKeyFrame->BoneState.LocalPosition;
#line 366 "../Code/d_game.cpp"
                
#line 367 "../Code/d_game.cpp"
            }
#line 368 "../Code/d_game.cpp"
            else
#line 369 "../Code/d_game.cpp"
            {
#line 370 "../Code/d_game.cpp"
                
#line 371 "../Code/d_game.cpp"
                BezierOffset = KeyFrame->NextBezierOffset;
#line 372 "../Code/d_game.cpp"
                OtherKeyFrame = KeyFrameArray + SelectedBezierOtherKeyFrameIndex;
#line 373 "../Code/d_game.cpp"
                
#line 374 "../Code/d_game.cpp"
                StartPoint = KeyFrame->BoneState.LocalPosition;
#line 375 "../Code/d_game.cpp"
                EndPoint = OtherKeyFrame->BoneState.LocalPosition;
#line 376 "../Code/d_game.cpp"
                
#line 377 "../Code/d_game.cpp"
            }
#line 378 "../Code/d_game.cpp"
            
#line 379 "../Code/d_game.cpp"
            Vector3 Direction = Vector3Subtract(EndPoint , StartPoint);
#line 380 "../Code/d_game.cpp"
            Direction = Vector3Scale(Vector3Normalize(Direction),0.1f);
#line 381 "../Code/d_game.cpp"
            
#line 382 "../Code/d_game.cpp"
            Vector3 ScaledDirection = Vector3Add(Direction , BezierOffset);
#line 383 "../Code/d_game.cpp"
            
#line 384 "../Code/d_game.cpp"
            Vector3 BezierStart = KeyFrame->BoneState.LocalPosition;
#line 385 "../Code/d_game.cpp"
            Vector3 BezierEnd = Vector3Add(BezierStart , ScaledDirection);
#line 386 "../Code/d_game.cpp"
            
#line 387 "../Code/d_game.cpp"
            _Rect BezierEndRect = GetBillboardRect(BezierEnd , 0.1);
#line 388 "../Code/d_game.cpp"
            
#line 389 "../Code/d_game.cpp"
            if(MouseButtonPressed(MOUSE_BUTTON_LEFT))
#line 390 "../Code/d_game.cpp"
            {
#line 391 "../Code/d_game.cpp"
                RedragSelection = false;
#line 392 "../Code/d_game.cpp"
                
#line 393 "../Code/d_game.cpp"
                DraggingPlanePoint = BezierEnd;
#line 394 "../Code/d_game.cpp"
                PreviousDraggingPoint = BezierEnd;
#line 395 "../Code/d_game.cpp"
            }
#line 396 "../Code/d_game.cpp"
            
#line 397 "../Code/d_game.cpp"
            //TODO : it just snap to the point instead adding offset
#line 398 "../Code/d_game.cpp"
            Vector3 DraggingPoint = GetCollisionQuad3D(GetBillboardQuad(DraggingPlanePoint , 100000,100000)).point;
#line 399 "../Code/d_game.cpp"
            Vector3 NewBezierEndPoint = Vector3Add(BezierEnd ,Vector3Subtract(DraggingPoint , PreviousDraggingPoint));
#line 400 "../Code/d_game.cpp"
            PreviousDraggingPoint = DraggingPoint;
#line 401 "../Code/d_game.cpp"
            Vector3 NewBezierOffset = Vector3Subtract(NewBezierEndPoint , BezierStart);
#line 402 "../Code/d_game.cpp"
            NewBezierOffset = Vector3Subtract(NewBezierOffset , Direction);
#line 403 "../Code/d_game.cpp"
            
#line 404 "../Code/d_game.cpp"
            if(SelectedPreviousBezierPoint)
#line 405 "../Code/d_game.cpp"
            {
#line 406 "../Code/d_game.cpp"
                KeyFrame->PreviousBezierOffset = NewBezierOffset;
#line 407 "../Code/d_game.cpp"
            }
#line 408 "../Code/d_game.cpp"
            else
#line 409 "../Code/d_game.cpp"
            {
#line 410 "../Code/d_game.cpp"
                KeyFrame->NextBezierOffset = NewBezierOffset;
#line 411 "../Code/d_game.cpp"
            }
#line 412 "../Code/d_game.cpp"
            
#line 413 "../Code/d_game.cpp"
            DrawCircle(BezierEndRect , YELLOW);
#line 414 "../Code/d_game.cpp"
            DrawRoundLine( BezierStart , BezierEnd , 20  , YELLOW );
#line 415 "../Code/d_game.cpp"
        }
#line 416 "../Code/d_game.cpp"
        
#line 417 "../Code/d_game.cpp"
    }
#line 418 "../Code/d_game.cpp"
    
#line 419 "../Code/d_game.cpp"
    //SingleSelectionAndMultiDrag
#line 420 "../Code/d_game.cpp"
    localPersist bool DraggingBonePosition = false;
#line 421 "../Code/d_game.cpp"
    localPersist Vector3 PreviousDragPoint ={};
#line 422 "../Code/d_game.cpp"
    localPersist Vector3 DragPlanePosition = {};
#line 423 "../Code/d_game.cpp"
    
#line 424 "../Code/d_game.cpp"
    if(MouseButtonPressedWithCheck(MOUSE_BUTTON_LEFT))
#line 425 "../Code/d_game.cpp"
    {
#line 426 "../Code/d_game.cpp"
        DraggingBonePosition = false;
#line 427 "../Code/d_game.cpp"
    }
#line 428 "../Code/d_game.cpp"
    
#line 429 "../Code/d_game.cpp"
    if(MouseButtonReleasedWithCheck(MOUSE_BUTTON_LEFT))
#line 430 "../Code/d_game.cpp"
    {
#line 431 "../Code/d_game.cpp"
        DraggingBonePosition = false;
#line 432 "../Code/d_game.cpp"
    }
#line 433 "../Code/d_game.cpp"
    
#line 434 "../Code/d_game.cpp"
    ChangeMatrix(ScreenMVP);
#line 435 "../Code/d_game.cpp"
    for(int BoneIndex = 0 ; BoneIndex < BoneCount ; BoneIndex++)
#line 436 "../Code/d_game.cpp"
    {
#line 437 "../Code/d_game.cpp"
        _Bone * CurrentBone = Editor->StickMan[BoneIndex];
#line 438 "../Code/d_game.cpp"
        Vector3 BoneScreenPoint = {};
#line 439 "../Code/d_game.cpp"
        //CurrentBone->hovering_and_change_position = false;
#line 440 "../Code/d_game.cpp"
        
#line 441 "../Code/d_game.cpp"
        Vector3 BoneInteractPoint ={};
#line 442 "../Code/d_game.cpp"
        if(Editor->ControlRig)
#line 443 "../Code/d_game.cpp"
        {
#line 444 "../Code/d_game.cpp"
            if(!IsRig(BoneIndex)) continue;
#line 445 "../Code/d_game.cpp"
            BoneInteractPoint = CurrentBone->Position;
#line 446 "../Code/d_game.cpp"
        }
#line 447 "../Code/d_game.cpp"
        else
#line 448 "../Code/d_game.cpp"
        {
#line 449 "../Code/d_game.cpp"
            if(IsRig(BoneIndex)) continue;
#line 450 "../Code/d_game.cpp"
            
#line 451 "../Code/d_game.cpp"
            Vector3 BonePositionEnd = Vector3RotateByQuaternion(CurrentBone->State.Direction , CurrentBone->Rotation);
#line 452 "../Code/d_game.cpp"
            BonePositionEnd = Vector3Add(BonePositionEnd , CurrentBone->Position);
#line 453 "../Code/d_game.cpp"
            
#line 454 "../Code/d_game.cpp"
            BoneInteractPoint = BonePositionEnd;
#line 455 "../Code/d_game.cpp"
        }
#line 456 "../Code/d_game.cpp"
        
#line 457 "../Code/d_game.cpp"
        BoneScreenPoint = TransformVector(BoneInteractPoint , MVP_3DWorld);
#line 458 "../Code/d_game.cpp"
        
#line 459 "../Code/d_game.cpp"
        _Rect BoneScreenRect = GetRect();
#line 460 "../Code/d_game.cpp"
        BoneScreenRect.Position = BoneScreenPoint;
#line 461 "../Code/d_game.cpp"
        BoneScreenRect.Size = PixelToSize(40 ,40);
#line 462 "../Code/d_game.cpp"
        
#line 463 "../Code/d_game.cpp"
        //TODO : this is dumb 
#line 464 "../Code/d_game.cpp"
        _Quad InteractQuad = GetBillboardQuad(BoneInteractPoint , 100000 , 100000 );
#line 465 "../Code/d_game.cpp"
        RayCollision PositionQuadResult = GetCollisionQuad3D(InteractQuad);
#line 466 "../Code/d_game.cpp"
        
#line 467 "../Code/d_game.cpp"
        Vector2 CurrentMousePosition = PixelToWorld(AppData->MousePosition);
#line 468 "../Code/d_game.cpp"
        
#line 469 "../Code/d_game.cpp"
        if(CheckCollisionRect(BoneScreenRect , CurrentMousePosition))
#line 470 "../Code/d_game.cpp"
        {
#line 471 "../Code/d_game.cpp"
            CurrentBone->Hovered = true;
#line 472 "../Code/d_game.cpp"
            
#line 473 "../Code/d_game.cpp"
            if(MouseButtonPressedWithCheck(MOUSE_BUTTON_LEFT))
#line 474 "../Code/d_game.cpp"
            {
#line 475 "../Code/d_game.cpp"
                bool SelectSingle = true;
#line 476 "../Code/d_game.cpp"
                
#line 477 "../Code/d_game.cpp"
                for(int StackIndex = 0 ; StackIndex < Editor->SelectedBoneCount ; StackIndex++)
#line 478 "../Code/d_game.cpp"
                {
#line 479 "../Code/d_game.cpp"
                    if(Editor->SelectedBoneStack[StackIndex] == BoneIndex) 
#line 480 "../Code/d_game.cpp"
                    {
#line 481 "../Code/d_game.cpp"
                        SelectSingle = false;
#line 482 "../Code/d_game.cpp"
                        break;
#line 483 "../Code/d_game.cpp"
                    }
#line 484 "../Code/d_game.cpp"
                }
#line 485 "../Code/d_game.cpp"
                
#line 486 "../Code/d_game.cpp"
                if(SelectSingle)
#line 487 "../Code/d_game.cpp"
                {
#line 488 "../Code/d_game.cpp"
                    if(KeyPressing(KEY_LEFT_SHIFT))
#line 489 "../Code/d_game.cpp"
                    {
#line 490 "../Code/d_game.cpp"
                        
#line 491 "../Code/d_game.cpp"
                        CurrentBone->Selected = true;
#line 492 "../Code/d_game.cpp"
                        Editor->SelectedBoneStack[Editor->SelectedBoneCount++] = BoneIndex;
#line 493 "../Code/d_game.cpp"
                        
#line 494 "../Code/d_game.cpp"
                    }
#line 495 "../Code/d_game.cpp"
                    else
#line 496 "../Code/d_game.cpp"
                    {
#line 497 "../Code/d_game.cpp"
                        
#line 498 "../Code/d_game.cpp"
                        for(int StackIndex = 0 ; StackIndex < Editor->SelectedBoneCount ; StackIndex++)
#line 499 "../Code/d_game.cpp"
                        {
#line 500 "../Code/d_game.cpp"
                            Editor->StickMan[Editor->SelectedBoneStack[StackIndex]]->Selected = false;
#line 501 "../Code/d_game.cpp"
                        }
#line 502 "../Code/d_game.cpp"
                        
#line 503 "../Code/d_game.cpp"
                        CurrentBone->Selected = true;
#line 504 "../Code/d_game.cpp"
                        Editor->SelectedBoneStack[0] = BoneIndex;
#line 505 "../Code/d_game.cpp"
                        Editor->SelectedBoneCount = 1;
#line 506 "../Code/d_game.cpp"
                        
#line 507 "../Code/d_game.cpp"
                    }
#line 508 "../Code/d_game.cpp"
                }
#line 509 "../Code/d_game.cpp"
                
#line 510 "../Code/d_game.cpp"
                PreviousDragPoint = PositionQuadResult.point;
#line 511 "../Code/d_game.cpp"
                DragPlanePosition = BoneInteractPoint;
#line 512 "../Code/d_game.cpp"
                
#line 513 "../Code/d_game.cpp"
                DraggingBonePosition = true;
#line 514 "../Code/d_game.cpp"
                RedragSelection = false;
#line 515 "../Code/d_game.cpp"
                
#line 516 "../Code/d_game.cpp"
            }
#line 517 "../Code/d_game.cpp"
            
#line 518 "../Code/d_game.cpp"
        }
#line 519 "../Code/d_game.cpp"
        
#line 520 "../Code/d_game.cpp"
    }
#line 521 "../Code/d_game.cpp"
    
#line 522 "../Code/d_game.cpp"
    if(DraggingBonePosition)
#line 523 "../Code/d_game.cpp"
    {
#line 524 "../Code/d_game.cpp"
        _Quad BoneCentreQuad = GetBillboardQuad(DragPlanePosition, 100000.0f, 100000.0f);
#line 525 "../Code/d_game.cpp"
        Vector3 DragPoint = GetCollisionQuad3D(BoneCentreQuad).point;
#line 526 "../Code/d_game.cpp"
        //DrawQuadLine(GetBillboardQuad(DragPoint , 0.1 , 0.1) , WHITE , 8);
#line 527 "../Code/d_game.cpp"
        
#line 528 "../Code/d_game.cpp"
        Vector3 PlaneOffset= DragPoint;
#line 529 "../Code/d_game.cpp"
        PlaneOffset = Vector3Subtract(PlaneOffset, PreviousDragPoint);
#line 530 "../Code/d_game.cpp"
        
#line 531 "../Code/d_game.cpp"
        PreviousDragPoint = DragPoint;
#line 532 "../Code/d_game.cpp"
        
#line 533 "../Code/d_game.cpp"
        for(int StackIndex = 0 ; StackIndex < Editor->SelectedBoneCount ; StackIndex++)
#line 534 "../Code/d_game.cpp"
        {
#line 535 "../Code/d_game.cpp"
            _Bone * DraggingBone = Editor->StickMan[Editor->SelectedBoneStack[StackIndex]];
#line 536 "../Code/d_game.cpp"
            
#line 537 "../Code/d_game.cpp"
            //it just feel right to do it
#line 538 "../Code/d_game.cpp"
            //also the world rotation is compute after
#line 539 "../Code/d_game.cpp"
            //something could go wrong
#line 540 "../Code/d_game.cpp"
            Quaternion RevertRotation = QuaternionMultiply(DraggingBone->Rotation , QuaternionInvert(DraggingBone->State.LocalRotation));
#line 541 "../Code/d_game.cpp"
            //RevertRotation = SelectedBone->State.LocalRotation;
#line 542 "../Code/d_game.cpp"
            PlaneOffset = Vector3RotateByQuaternion(PlaneOffset,QuaternionInvert(RevertRotation));
#line 543 "../Code/d_game.cpp"
            //printf("Offset : %f %f %f\n" ,PlaneOffset.x , PlaneOffset.y , PlaneOffset.z);
#line 544 "../Code/d_game.cpp"
            
#line 545 "../Code/d_game.cpp"
            if (DraggingBone->FreePosition)
#line 546 "../Code/d_game.cpp"
            {
#line 547 "../Code/d_game.cpp"
                DraggingBone->State.LocalPosition = Vector3Add(DraggingBone->State.LocalPosition, PlaneOffset);
#line 548 "../Code/d_game.cpp"
            }
#line 549 "../Code/d_game.cpp"
            
#line 550 "../Code/d_game.cpp"
        }
#line 551 "../Code/d_game.cpp"
    }
#line 552 "../Code/d_game.cpp"
    else
#line 553 "../Code/d_game.cpp"
    {
#line 554 "../Code/d_game.cpp"
        
#line 555 "../Code/d_game.cpp"
        //SingleBoneRotation
#line 556 "../Code/d_game.cpp"
        ChangeMatrix(MVP_3DWorld);
#line 557 "../Code/d_game.cpp"
        if(Editor->SelectedBoneCount == 1)
#line 558 "../Code/d_game.cpp"
        {
#line 559 "../Code/d_game.cpp"
            int SingleBoneIndex = Editor->SelectedBoneStack[0];
#line 560 "../Code/d_game.cpp"
            
#line 561 "../Code/d_game.cpp"
            _Bone * SelectedBone = Editor->StickMan[SingleBoneIndex];
#line 562 "../Code/d_game.cpp"
            
#line 563 "../Code/d_game.cpp"
            bool InteractingSelectedBone = CheckSelectedBoneRotation(SingleBoneIndex);
#line 564 "../Code/d_game.cpp"
            
#line 565 "../Code/d_game.cpp"
            if(MouseButtonPressedWithCheck(MOUSE_BUTTON_LEFT))
#line 566 "../Code/d_game.cpp"
            {
#line 567 "../Code/d_game.cpp"
                if(InteractingSelectedBone)
#line 568 "../Code/d_game.cpp"
                {
#line 569 "../Code/d_game.cpp"
                    RedragSelection = false;
#line 570 "../Code/d_game.cpp"
                }
#line 571 "../Code/d_game.cpp"
                else
#line 572 "../Code/d_game.cpp"
                {
#line 573 "../Code/d_game.cpp"
                    for(int StackIndex = 0 ; StackIndex < Editor->SelectedBoneCount ; StackIndex++)
#line 574 "../Code/d_game.cpp"
                    {
#line 575 "../Code/d_game.cpp"
                        Editor->StickMan[Editor->SelectedBoneStack[StackIndex]]->Selected = false;
#line 576 "../Code/d_game.cpp"
                    }
#line 577 "../Code/d_game.cpp"
                    
#line 578 "../Code/d_game.cpp"
                    Editor->SelectedBoneCount = 0;
#line 579 "../Code/d_game.cpp"
                    
#line 580 "../Code/d_game.cpp"
                }
#line 581 "../Code/d_game.cpp"
                
#line 582 "../Code/d_game.cpp"
            }
#line 583 "../Code/d_game.cpp"
        }
#line 584 "../Code/d_game.cpp"
        
#line 585 "../Code/d_game.cpp"
        ChangeMatrix(ScreenMVP);
#line 586 "../Code/d_game.cpp"
    }
#line 587 "../Code/d_game.cpp"
    
#line 588 "../Code/d_game.cpp"
    //MultiSelection
#line 589 "../Code/d_game.cpp"
    localPersist Vector3 mouse_start_drag_position = {};
#line 590 "../Code/d_game.cpp"
    Vector2 mouse_screen_position = PixelToWorld(AppData->MousePosition.x , AppData->MousePosition.y);
#line 591 "../Code/d_game.cpp"
    
#line 592 "../Code/d_game.cpp"
    if(MouseButtonPressedWithCheck(MOUSE_BUTTON_LEFT))
#line 593 "../Code/d_game.cpp"
    {
#line 594 "../Code/d_game.cpp"
        mouse_start_drag_position.x = mouse_screen_position.x;
#line 595 "../Code/d_game.cpp"
        mouse_start_drag_position.y = mouse_screen_position.y;
#line 596 "../Code/d_game.cpp"
    }
#line 597 "../Code/d_game.cpp"
    
#line 598 "../Code/d_game.cpp"
    if(RedragSelection)
#line 599 "../Code/d_game.cpp"
    {
#line 600 "../Code/d_game.cpp"
        Vector3 current_mouse_drag_position = {mouse_screen_position.x , mouse_screen_position.y , 0};
#line 601 "../Code/d_game.cpp"
        
#line 602 "../Code/d_game.cpp"
        _Rect mouse_drag_rect = GetRect();
#line 603 "../Code/d_game.cpp"
        mouse_drag_rect.Position= Vector3Add(mouse_start_drag_position , current_mouse_drag_position);
#line 604 "../Code/d_game.cpp"
        mouse_drag_rect.Position.x /= 2.0f;
#line 605 "../Code/d_game.cpp"
        mouse_drag_rect.Position.y /= 2.0f;
#line 606 "../Code/d_game.cpp"
        
#line 607 "../Code/d_game.cpp"
        mouse_drag_rect.Size.x = mouse_start_drag_position.x - current_mouse_drag_position.x;
#line 608 "../Code/d_game.cpp"
        if(mouse_drag_rect.Size.x < 0) mouse_drag_rect.Size.x *= -1;
#line 609 "../Code/d_game.cpp"
        
#line 610 "../Code/d_game.cpp"
        mouse_drag_rect.Size.y = mouse_start_drag_position.y - current_mouse_drag_position.y;
#line 611 "../Code/d_game.cpp"
        if(mouse_drag_rect.Size.y < 0) mouse_drag_rect.Size.y *= -1;
#line 612 "../Code/d_game.cpp"
        
#line 613 "../Code/d_game.cpp"
        DraggingSelection = true;
#line 614 "../Code/d_game.cpp"
        
#line 615 "../Code/d_game.cpp"
        if(MouseButtonPressedWithCheck(MOUSE_BUTTON_LEFT))
#line 616 "../Code/d_game.cpp"
        {
#line 617 "../Code/d_game.cpp"
            if(!KeyPressing(KEY_LEFT_SHIFT))
#line 618 "../Code/d_game.cpp"
            {
#line 619 "../Code/d_game.cpp"
                for(int StackIndex = 0 ; StackIndex < Editor->SelectedBoneCount ; StackIndex++)
#line 620 "../Code/d_game.cpp"
                {
#line 621 "../Code/d_game.cpp"
                    Editor->StickMan[Editor->SelectedBoneStack[StackIndex]]->Selected = false;
#line 622 "../Code/d_game.cpp"
                }
#line 623 "../Code/d_game.cpp"
                
#line 624 "../Code/d_game.cpp"
                Editor->SelectedBoneCount = 0;
#line 625 "../Code/d_game.cpp"
            }
#line 626 "../Code/d_game.cpp"
        }
#line 627 "../Code/d_game.cpp"
        
#line 628 "../Code/d_game.cpp"
        if(MouseButtonPressingWithCheck(MOUSE_BUTTON_LEFT))
#line 629 "../Code/d_game.cpp"
        {
#line 630 "../Code/d_game.cpp"
            DrawRectLine(mouse_drag_rect , WHITE , 5);
#line 631 "../Code/d_game.cpp"
        }
#line 632 "../Code/d_game.cpp"
        
#line 633 "../Code/d_game.cpp"
        for(int BoneIndex = 0 ; BoneIndex < BoneCount ; BoneIndex++)
#line 634 "../Code/d_game.cpp"
        {
#line 635 "../Code/d_game.cpp"
            _Bone * CurrentBone = Editor->StickMan[BoneIndex];
#line 636 "../Code/d_game.cpp"
            Vector3 BoneScreenPoint = {};
#line 637 "../Code/d_game.cpp"
            //CurrentBone->hovering_and_change_position = false;
#line 638 "../Code/d_game.cpp"
            
#line 639 "../Code/d_game.cpp"
            if(Editor->ControlRig)
#line 640 "../Code/d_game.cpp"
            {
#line 641 "../Code/d_game.cpp"
                if(!IsRig(BoneIndex)) continue;
#line 642 "../Code/d_game.cpp"
                
#line 643 "../Code/d_game.cpp"
                BoneScreenPoint = TransformVector(CurrentBone->Position , MVP_3DWorld);
#line 644 "../Code/d_game.cpp"
            }
#line 645 "../Code/d_game.cpp"
            else
#line 646 "../Code/d_game.cpp"
            {
#line 647 "../Code/d_game.cpp"
                if(IsRig(BoneIndex)) continue;
#line 648 "../Code/d_game.cpp"
                
#line 649 "../Code/d_game.cpp"
                Vector3 BonePositionEnd = Vector3RotateByQuaternion(CurrentBone->State.Direction , CurrentBone->Rotation);
#line 650 "../Code/d_game.cpp"
                BonePositionEnd = Vector3Add(BonePositionEnd , CurrentBone->Position);
#line 651 "../Code/d_game.cpp"
                
#line 652 "../Code/d_game.cpp"
                BoneScreenPoint = TransformVector(BonePositionEnd , MVP_3DWorld);
#line 653 "../Code/d_game.cpp"
            }
#line 654 "../Code/d_game.cpp"
            
#line 655 "../Code/d_game.cpp"
            _Rect BoneScreenRect = GetRect();
#line 656 "../Code/d_game.cpp"
            BoneScreenRect.Position = BoneScreenPoint;
#line 657 "../Code/d_game.cpp"
            BoneScreenRect.Size = PixelToSize(40 ,40);
#line 658 "../Code/d_game.cpp"
            
#line 659 "../Code/d_game.cpp"
            if(CheckCollisionRect(mouse_drag_rect , BoneScreenRect ))
#line 660 "../Code/d_game.cpp"
            {
#line 661 "../Code/d_game.cpp"
                if(MouseButtonPressingWithCheck(MOUSE_BUTTON_LEFT))
#line 662 "../Code/d_game.cpp"
                {
#line 663 "../Code/d_game.cpp"
                    CurrentBone->Hovered = true;
#line 664 "../Code/d_game.cpp"
                }
#line 665 "../Code/d_game.cpp"
                
#line 666 "../Code/d_game.cpp"
                if(MouseButtonReleasedWithCheck(MOUSE_BUTTON_LEFT))
#line 667 "../Code/d_game.cpp"
                {
#line 668 "../Code/d_game.cpp"
                    if(!CurrentBone->Selected)
#line 669 "../Code/d_game.cpp"
                    {
#line 670 "../Code/d_game.cpp"
                        CurrentBone->Selected = true;
#line 671 "../Code/d_game.cpp"
                        Editor->SelectedBoneStack[Editor->SelectedBoneCount++] = BoneIndex;
#line 672 "../Code/d_game.cpp"
                    }
#line 673 "../Code/d_game.cpp"
                }
#line 674 "../Code/d_game.cpp"
            }
#line 675 "../Code/d_game.cpp"
        }
#line 676 "../Code/d_game.cpp"
    }
#line 677 "../Code/d_game.cpp"
    
#line 678 "../Code/d_game.cpp"
    for(int BoneIndex = 0 ; BoneIndex < BoneCount ; BoneIndex++)
#line 679 "../Code/d_game.cpp"
    {
#line 680 "../Code/d_game.cpp"
        
#line 681 "../Code/d_game.cpp"
        _Bone * Bone = Editor->StickMan[BoneIndex];
#line 682 "../Code/d_game.cpp"
        
#line 683 "../Code/d_game.cpp"
        Vector3 BoneScreenPoint = {};
#line 684 "../Code/d_game.cpp"
        if(Editor->ControlRig)
#line 685 "../Code/d_game.cpp"
        {
#line 686 "../Code/d_game.cpp"
            if(!IsRig(BoneIndex)) continue;
#line 687 "../Code/d_game.cpp"
            
#line 688 "../Code/d_game.cpp"
            BoneScreenPoint = TransformVector(Bone->Position , MVP_3DWorld);
#line 689 "../Code/d_game.cpp"
        }
#line 690 "../Code/d_game.cpp"
        else
#line 691 "../Code/d_game.cpp"
        {
#line 692 "../Code/d_game.cpp"
            
#line 693 "../Code/d_game.cpp"
            if(IsRig(BoneIndex)) continue;
#line 694 "../Code/d_game.cpp"
            
#line 695 "../Code/d_game.cpp"
            Vector3 BonePositionEnd = Vector3RotateByQuaternion(Bone->State.Direction , Bone->Rotation);
#line 696 "../Code/d_game.cpp"
            BonePositionEnd = Vector3Add(BonePositionEnd , Bone->Position);
#line 697 "../Code/d_game.cpp"
            BoneScreenPoint = TransformVector(BonePositionEnd , MVP_3DWorld);
#line 698 "../Code/d_game.cpp"
            
#line 699 "../Code/d_game.cpp"
        }
#line 700 "../Code/d_game.cpp"
        
#line 701 "../Code/d_game.cpp"
        _Rect BoneScreenRect = GetRect();
#line 702 "../Code/d_game.cpp"
        BoneScreenRect.Position = BoneScreenPoint;
#line 703 "../Code/d_game.cpp"
        BoneScreenRect.Size = PixelToSize(40 ,40);
#line 704 "../Code/d_game.cpp"
        
#line 705 "../Code/d_game.cpp"
        if(Bone->Selected)
#line 706 "../Code/d_game.cpp"
        {
#line 707 "../Code/d_game.cpp"
            DrawRectLine(BoneScreenRect , BLUE , 5);
#line 708 "../Code/d_game.cpp"
        }
#line 709 "../Code/d_game.cpp"
        else if(Bone->Hovered)
#line 710 "../Code/d_game.cpp"
        {
#line 711 "../Code/d_game.cpp"
            DrawRectLine(BoneScreenRect , YELLOW , 5);
#line 712 "../Code/d_game.cpp"
        }
#line 713 "../Code/d_game.cpp"
        else
#line 714 "../Code/d_game.cpp"
        {
#line 715 "../Code/d_game.cpp"
            DrawRectLine(BoneScreenRect , Fade(YELLOW , 0.2f) , 5);
#line 716 "../Code/d_game.cpp"
        }
#line 717 "../Code/d_game.cpp"
        
#line 718 "../Code/d_game.cpp"
        Bone->Hovered = false;
#line 719 "../Code/d_game.cpp"
    }
#line 720 "../Code/d_game.cpp"
    
#line 721 "../Code/d_game.cpp"
    ChangeMatrix(MVP_3DWorld);
#line 722 "../Code/d_game.cpp"
    
#line 723 "../Code/d_game.cpp"
}
#line 724 "../Code/d_game.cpp"

#line 725 "../Code/d_game.cpp"
internal void Editor_GUI()
#line 726 "../Code/d_game.cpp"
{	
#line 727 "../Code/d_game.cpp"
    
#line 728 "../Code/d_game.cpp"
    rlFramebufferAttach(RenderState.ScreenFrameBuffer, RenderState.GUITexture, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);    
#line 729 "../Code/d_game.cpp"
    if (!rlFramebufferComplete(RenderState.ScreenFrameBuffer)) ASSERT;
#line 730 "../Code/d_game.cpp"
    rlEnableFramebuffer(RenderState.ScreenFrameBuffer);
#line 731 "../Code/d_game.cpp"
	
#line 732 "../Code/d_game.cpp"
    glClearColor(0,0,0, 0);
#line 733 "../Code/d_game.cpp"
    glClear(GL_COLOR_BUFFER_BIT);
#line 734 "../Code/d_game.cpp"
	glDisable(GL_DEPTH_TEST);
#line 735 "../Code/d_game.cpp"
    
#line 736 "../Code/d_game.cpp"
	localPersist bool BoneMenuEnable = false;
#line 737 "../Code/d_game.cpp"
    
#line 738 "../Code/d_game.cpp"
	if (MouseButtonPressed(MOUSE_BUTTON_RIGHT))
#line 739 "../Code/d_game.cpp"
	{
#line 740 "../Code/d_game.cpp"
		Editor->OperateMenuPosition = PixelToWorld(AppData->MousePosition);
#line 741 "../Code/d_game.cpp"
        
#line 742 "../Code/d_game.cpp"
		if (Editor->SelectedBoneCount == 1)
#line 743 "../Code/d_game.cpp"
		{
#line 744 "../Code/d_game.cpp"
			BoneMenuEnable = true;
#line 745 "../Code/d_game.cpp"
		}
#line 746 "../Code/d_game.cpp"
	}
#line 747 "../Code/d_game.cpp"
    
#line 748 "../Code/d_game.cpp"
	if (Editor->SelectedBoneCount != 1)
#line 749 "../Code/d_game.cpp"
	{
#line 750 "../Code/d_game.cpp"
		BoneMenuEnable = false;
#line 751 "../Code/d_game.cpp"
	}
#line 752 "../Code/d_game.cpp"
    
#line 753 "../Code/d_game.cpp"
	if (BoneMenuEnable)
#line 754 "../Code/d_game.cpp"
	{
#line 755 "../Code/d_game.cpp"
		_Bone * EditingBone = Editor->StickMan[Editor->SelectedBoneStack[0]];
#line 756 "../Code/d_game.cpp"
		DrawingMenu Menu = StartDrawMenuMouse();
#line 757 "../Code/d_game.cpp"
        
#line 758 "../Code/d_game.cpp"
		DrawMenuFloatInput(&Menu , L"長度 : %f " , &EditingBone->State.Direction.y);
#line 759 "../Code/d_game.cpp"
        
#line 760 "../Code/d_game.cpp"
		if (DrawMenuButton(&Menu , L"重置位置"))
#line 761 "../Code/d_game.cpp"
		{
#line 762 "../Code/d_game.cpp"
			EditingBone->State.LocalPosition = {};
#line 763 "../Code/d_game.cpp"
		}
#line 764 "../Code/d_game.cpp"
		
#line 765 "../Code/d_game.cpp"
		if (DrawMenuButton(&Menu , L"重置旋轉"))
#line 766 "../Code/d_game.cpp"
		{
#line 767 "../Code/d_game.cpp"
			EditingBone->State.LocalRotation = QuaternionIdentity();
#line 768 "../Code/d_game.cpp"
		}
#line 769 "../Code/d_game.cpp"
        
#line 770 "../Code/d_game.cpp"
        if(DrawMenuButton(&Menu , EditingBone->FreePosition ? L"鎖定位置" : L"自由位置" ))
#line 771 "../Code/d_game.cpp"
        {
#line 772 "../Code/d_game.cpp"
            EditingBone->FreePosition = !EditingBone->FreePosition;
#line 773 "../Code/d_game.cpp"
        }
#line 774 "../Code/d_game.cpp"
        
#line 775 "../Code/d_game.cpp"
        int KeyFrameCount = 0;
#line 776 "../Code/d_game.cpp"
        _KeyFrame * KeyFrameArray = (_KeyFrame *)EditingBone->KeyFrameArray;
#line 777 "../Code/d_game.cpp"
        
#line 778 "../Code/d_game.cpp"
        HashTable_Iterate(KeyFrameIndex , Editor->PointerFrameIndex , &EditingBone->KeyFrameHashTable , MAX_KEY_FRAME)
#line 779 "../Code/d_game.cpp"
        {
#line 780 "../Code/d_game.cpp"
            if (DrawMenuButton(&Menu , L"重置貝塞爾曲綫"))
#line 781 "../Code/d_game.cpp"
            {
#line 782 "../Code/d_game.cpp"
                _KeyFrame * KeyFrame = KeyFrameArray + KeyFrameIndex;
#line 783 "../Code/d_game.cpp"
                
#line 784 "../Code/d_game.cpp"
                KeyFrame->PreviousBezierOffset = {};
#line 785 "../Code/d_game.cpp"
                KeyFrame->NextBezierOffset = {};
#line 786 "../Code/d_game.cpp"
            }
#line 787 "../Code/d_game.cpp"
            
#line 788 "../Code/d_game.cpp"
        }
#line 789 "../Code/d_game.cpp"
		
#line 790 "../Code/d_game.cpp"
	}
#line 791 "../Code/d_game.cpp"
	
#line 792 "../Code/d_game.cpp"
	_Rect TimelineRect = GetRect();
#line 793 "../Code/d_game.cpp"
	TimelineRect.Position = {0,-1};
#line 794 "../Code/d_game.cpp"
    
#line 795 "../Code/d_game.cpp"
	TimelineRect.Size = {PixelToWidth(AppData->WindowSize.x - 60), PixelToHeight(80)};
#line 796 "../Code/d_game.cpp"
	TimelineRect.Position.y += TimelineRect.Size.y * 0.5f;
#line 797 "../Code/d_game.cpp"
	TimelineRect.Position.y += PixelToHeight(40);
#line 798 "../Code/d_game.cpp"
    
#line 799 "../Code/d_game.cpp"
	DrawRect(TimelineRect , PixelToWidth(5) , Fade(BLACK , 0.2f));
#line 800 "../Code/d_game.cpp"
    
#line 801 "../Code/d_game.cpp"
	localPersist bool dragging_frame_pointer = false;
#line 802 "../Code/d_game.cpp"
    
#line 803 "../Code/d_game.cpp"
	if (MouseButtonReleased(MOUSE_BUTTON_LEFT))
#line 804 "../Code/d_game.cpp"
	{
#line 805 "../Code/d_game.cpp"
		dragging_frame_pointer = false;
#line 806 "../Code/d_game.cpp"
	}
#line 807 "../Code/d_game.cpp"
    
#line 808 "../Code/d_game.cpp"
	float FramePositionOffsetX = PixelToWidth(20) * Editor->TimelineScale;
#line 809 "../Code/d_game.cpp"
    
#line 810 "../Code/d_game.cpp"
	if (CheckCollisionRectMouse(TimelineRect))
#line 811 "../Code/d_game.cpp"
	{
#line 812 "../Code/d_game.cpp"
		MenuInteruped = true;
#line 813 "../Code/d_game.cpp"
        
#line 814 "../Code/d_game.cpp"
		if (KeyPressing(KEY_LEFT_CONTROL))
#line 815 "../Code/d_game.cpp"
		{
#line 816 "../Code/d_game.cpp"
			Editor->TimelineScale += AppData->MouseScrollDelta * 0.1f;
#line 817 "../Code/d_game.cpp"
		}
#line 818 "../Code/d_game.cpp"
		else
#line 819 "../Code/d_game.cpp"
		{
#line 820 "../Code/d_game.cpp"
			Editor->TimelineSliderOffset += AppData->MouseScrollDelta * 0.1f;
#line 821 "../Code/d_game.cpp"
		}
#line 822 "../Code/d_game.cpp"
        
#line 823 "../Code/d_game.cpp"
	}
#line 824 "../Code/d_game.cpp"
    
#line 825 "../Code/d_game.cpp"
	int ScrollOffset = -Editor->TimelineSliderOffset / FramePositionOffsetX;
#line 826 "../Code/d_game.cpp"
    ScrollOffset += Editor->StartFrameIndex;
#line 827 "../Code/d_game.cpp"
    
#line 828 "../Code/d_game.cpp"
	if (ScrollOffset < 0) ScrollOffset = 0;
#line 829 "../Code/d_game.cpp"
    
#line 830 "../Code/d_game.cpp"
    float ClosestFrameX = FLT_MAX;
#line 831 "../Code/d_game.cpp"
	
#line 832 "../Code/d_game.cpp"
	localPersist Vector3 PointerPosition = {};
#line 833 "../Code/d_game.cpp"
    
#line 834 "../Code/d_game.cpp"
	int ClosestFrameIndexToMouse = -1;
#line 835 "../Code/d_game.cpp"
    
#line 836 "../Code/d_game.cpp"
	for (int FrameIndex = ScrollOffset ; ; FrameIndex++)
#line 837 "../Code/d_game.cpp"
	{
#line 838 "../Code/d_game.cpp"
		if (FrameIndex > Editor->EndFrameIndex) break;
#line 839 "../Code/d_game.cpp"
        
#line 840 "../Code/d_game.cpp"
		_Rect FrameRect = GetRect();
#line 841 "../Code/d_game.cpp"
		Vector3 FramePosition = TimelineRect.Position;
#line 842 "../Code/d_game.cpp"
		FramePosition.x += FrameIndex * FramePositionOffsetX - TimelineRect.Size.x * 0.5f + FramePositionOffsetX * 0.5f + Editor->TimelineSliderOffset;
#line 843 "../Code/d_game.cpp"
        
#line 844 "../Code/d_game.cpp"
		if (FramePosition.x > TimelineRect.Size.x * 0.5 + TimelineRect.Position.x) break;
#line 845 "../Code/d_game.cpp"
        
#line 846 "../Code/d_game.cpp"
		FrameRect.Position = FramePosition;
#line 847 "../Code/d_game.cpp"
		FrameRect.Size = {PixelToWidth(2), TimelineRect.Size.y*0.8f};
#line 848 "../Code/d_game.cpp"
        
#line 849 "../Code/d_game.cpp"
		DrawRect(FrameRect,PixelToWidth(5),Fade(BLACK , 0.2f));
#line 850 "../Code/d_game.cpp"
        
#line 851 "../Code/d_game.cpp"
		wchar_t FrameIndexString[64] = {};
#line 852 "../Code/d_game.cpp"
		swprintf(FrameIndexString , L"%d" , FrameIndex);
#line 853 "../Code/d_game.cpp"
        
#line 854 "../Code/d_game.cpp"
		_Rect TextRect = FrameRect;
#line 855 "../Code/d_game.cpp"
		TextRect.Size.y = PixelToWidth(6);
#line 856 "../Code/d_game.cpp"
        
#line 857 "../Code/d_game.cpp"
		D_DrawText(TextRect, FrameIndexString , WHITE  , true);
#line 858 "../Code/d_game.cpp"
        
#line 859 "../Code/d_game.cpp"
		Vector2 MousePosition = PixelToWorld(AppData->MousePosition);
#line 860 "../Code/d_game.cpp"
		float FrameToMouseX = MousePosition.x - FrameRect.Position.x;
#line 861 "../Code/d_game.cpp"
		if (FrameToMouseX < 0) FrameToMouseX *= -1;
#line 862 "../Code/d_game.cpp"
        
#line 863 "../Code/d_game.cpp"
		if (ClosestFrameX > FrameToMouseX)
#line 864 "../Code/d_game.cpp"
		{
#line 865 "../Code/d_game.cpp"
			ClosestFrameX = FrameToMouseX;
#line 866 "../Code/d_game.cpp"
			ClosestFrameIndexToMouse = FrameIndex;
#line 867 "../Code/d_game.cpp"
		}
#line 868 "../Code/d_game.cpp"
        
#line 869 "../Code/d_game.cpp"
		if (FrameIndex == Editor->PointerFrameIndex)
#line 870 "../Code/d_game.cpp"
		{
#line 871 "../Code/d_game.cpp"
			PointerPosition = FrameRect.Position;
#line 872 "../Code/d_game.cpp"
		}
#line 873 "../Code/d_game.cpp"
	}
#line 874 "../Code/d_game.cpp"
	
#line 875 "../Code/d_game.cpp"
	if (dragging_frame_pointer)
#line 876 "../Code/d_game.cpp"
	{
#line 877 "../Code/d_game.cpp"
		MenuInteruped = true;
#line 878 "../Code/d_game.cpp"
        
#line 879 "../Code/d_game.cpp"
		Editor->PointerFrameIndex = ClosestFrameIndexToMouse;
#line 880 "../Code/d_game.cpp"
	}
#line 881 "../Code/d_game.cpp"
    
#line 882 "../Code/d_game.cpp"
    localPersist bool DraggingKeyFrame = false;
#line 883 "../Code/d_game.cpp"
    
#line 884 "../Code/d_game.cpp"
	for (int FrameIndex = ScrollOffset;; FrameIndex++)
#line 885 "../Code/d_game.cpp"
	{
#line 886 "../Code/d_game.cpp"
		if (FrameIndex > Editor->EndFrameIndex) break;
#line 887 "../Code/d_game.cpp"
        
#line 888 "../Code/d_game.cpp"
        for(int StackIndex = 0 ; StackIndex < Editor->SelectedBoneCount ; StackIndex++)
#line 889 "../Code/d_game.cpp"
        {
#line 890 "../Code/d_game.cpp"
            
#line 891 "../Code/d_game.cpp"
            Vector3 FramePosition = TimelineRect.Position;
#line 892 "../Code/d_game.cpp"
            FramePosition.y += TimelineRect.Size.y * 0.4f;
#line 893 "../Code/d_game.cpp"
			FramePosition.x += FrameIndex * FramePositionOffsetX - TimelineRect.Size.x * 0.5f + FramePositionOffsetX * 0.5f + Editor->TimelineSliderOffset;
#line 894 "../Code/d_game.cpp"
            
#line 895 "../Code/d_game.cpp"
			_Bone * CurrentBone = Editor->StickMan[Editor->SelectedBoneStack[StackIndex]];
#line 896 "../Code/d_game.cpp"
            
#line 897 "../Code/d_game.cpp"
			HashTable_Iterate(KeyFrameIndex , FrameIndex, &CurrentBone->KeyFrameHashTable , MAX_KEY_FRAME)
#line 898 "../Code/d_game.cpp"
			{
#line 899 "../Code/d_game.cpp"
				_KeyFrame * KeyFrame = (_KeyFrame *)(CurrentBone->KeyFrameArray + KeyFrameIndex);
#line 900 "../Code/d_game.cpp"
                
#line 901 "../Code/d_game.cpp"
				if (KeyFrame->FrameIndex != FrameIndex) continue;
#line 902 "../Code/d_game.cpp"
                
#line 903 "../Code/d_game.cpp"
				_Rect KeyFrameRect = GetRect();
#line 904 "../Code/d_game.cpp"
				KeyFrameRect.Position = FramePosition;
#line 905 "../Code/d_game.cpp"
                
#line 906 "../Code/d_game.cpp"
				KeyFrameRect.Size = { PixelToWidth(20) , PixelToHeight(20) };
#line 907 "../Code/d_game.cpp"
                KeyFrameRect.Position.y -= 1.1f * KeyFrameRect.Size.y * StackIndex;
#line 908 "../Code/d_game.cpp"
                
#line 909 "../Code/d_game.cpp"
                bool Draggable = true;
#line 910 "../Code/d_game.cpp"
                
#line 911 "../Code/d_game.cpp"
                if(DraggingKeyFrame) Draggable = false;
#line 912 "../Code/d_game.cpp"
                if(!CheckCollisionRectMouse(KeyFrameRect)) Draggable = false;
#line 913 "../Code/d_game.cpp"
                
#line 914 "../Code/d_game.cpp"
				if (Draggable)
#line 915 "../Code/d_game.cpp"
				{
#line 916 "../Code/d_game.cpp"
					if (MouseButtonPressed(MOUSE_BUTTON_LEFT))
#line 917 "../Code/d_game.cpp"
					{
#line 918 "../Code/d_game.cpp"
                        DraggingKeyFrame = true;
#line 919 "../Code/d_game.cpp"
						AddToListTail(KeyFrameIndex , &CurrentBone->DraggingKeyFrameList , MAX_KEY_FRAME);
#line 920 "../Code/d_game.cpp"
					}
#line 921 "../Code/d_game.cpp"
                    
#line 922 "../Code/d_game.cpp"
					DrawCircle(KeyFrameRect , Fade(ORANGE , 0.6));
#line 923 "../Code/d_game.cpp"
				}
#line 924 "../Code/d_game.cpp"
				else
#line 925 "../Code/d_game.cpp"
				{
#line 926 "../Code/d_game.cpp"
					DrawCircle(KeyFrameRect , Fade(ORANGE , 0.4));
#line 927 "../Code/d_game.cpp"
				}
#line 928 "../Code/d_game.cpp"
                
#line 929 "../Code/d_game.cpp"
			}
#line 930 "../Code/d_game.cpp"
            
#line 931 "../Code/d_game.cpp"
        }
#line 932 "../Code/d_game.cpp"
        
#line 933 "../Code/d_game.cpp"
	}
#line 934 "../Code/d_game.cpp"
    
#line 935 "../Code/d_game.cpp"
    //printf("%d \n" , Editor->StickMan[Editor->SelectedBoneIndex].DraggingKeyFrameList.UnusedIndex);
#line 936 "../Code/d_game.cpp"
    
#line 937 "../Code/d_game.cpp"
	localPersist int PreviousFrameIndex = -1;
#line 938 "../Code/d_game.cpp"
    
#line 939 "../Code/d_game.cpp"
	if (MouseButtonPressed(MOUSE_BUTTON_LEFT))
#line 940 "../Code/d_game.cpp"
	{
#line 941 "../Code/d_game.cpp"
		PreviousFrameIndex = ClosestFrameIndexToMouse;
#line 942 "../Code/d_game.cpp"
	}
#line 943 "../Code/d_game.cpp"
    
#line 944 "../Code/d_game.cpp"
#if 1
#line 945 "../Code/d_game.cpp"
    for(int StackIndex = 0 ; StackIndex < Editor->SelectedBoneCount; StackIndex++)
#line 946 "../Code/d_game.cpp"
	{
#line 947 "../Code/d_game.cpp"
		_Bone * CurrentBone = Editor->StickMan[Editor->SelectedBoneStack[StackIndex]];
#line 948 "../Code/d_game.cpp"
        
#line 949 "../Code/d_game.cpp"
		if (MouseButtonReleased(MOUSE_BUTTON_LEFT))
#line 950 "../Code/d_game.cpp"
		{
#line 951 "../Code/d_game.cpp"
            DraggingKeyFrame = false;
#line 952 "../Code/d_game.cpp"
            ClearList(&CurrentBone->DraggingKeyFrameList);
#line 953 "../Code/d_game.cpp"
            
#line 954 "../Code/d_game.cpp"
		}
#line 955 "../Code/d_game.cpp"
        
#line 956 "../Code/d_game.cpp"
        if(MouseButtonPressing(MOUSE_BUTTON_LEFT))
#line 957 "../Code/d_game.cpp"
        {
#line 958 "../Code/d_game.cpp"
            _KeyFrame * KeyFrameArray = (_KeyFrame *)CurrentBone->KeyFrameArray;
#line 959 "../Code/d_game.cpp"
            
#line 960 "../Code/d_game.cpp"
            List_Foreach(DraggingKeyFrameIndex , &CurrentBone->DraggingKeyFrameList)
#line 961 "../Code/d_game.cpp"
            {
#line 962 "../Code/d_game.cpp"
                MenuInteruped = true;
#line 963 "../Code/d_game.cpp"
                _KeyFrame * DraggingKeyFrame = KeyFrameArray + DraggingKeyFrameIndex;
#line 964 "../Code/d_game.cpp"
                
#line 965 "../Code/d_game.cpp"
                bool FrameExisted = false;
#line 966 "../Code/d_game.cpp"
                HashTable_Iterate(ExistedKeyFrameIndex ,ClosestFrameIndexToMouse , &CurrentBone->KeyFrameHashTable , MAX_KEY_FRAME)
#line 967 "../Code/d_game.cpp"
                {
#line 968 "../Code/d_game.cpp"
                    if(KeyFrameArray[ExistedKeyFrameIndex].FrameIndex == ClosestFrameIndexToMouse)
#line 969 "../Code/d_game.cpp"
                    {
#line 970 "../Code/d_game.cpp"
                        FrameExisted = true;
#line 971 "../Code/d_game.cpp"
                        break;
#line 972 "../Code/d_game.cpp"
                    }
#line 973 "../Code/d_game.cpp"
                }
#line 974 "../Code/d_game.cpp"
                
#line 975 "../Code/d_game.cpp"
                if(!FrameExisted)
#line 976 "../Code/d_game.cpp"
                {
#line 977 "../Code/d_game.cpp"
                    DeleteFromHashTable(DraggingKeyFrame->FrameIndex , DraggingKeyFrameIndex , &CurrentBone->KeyFrameHashTable , MAX_KEY_FRAME);
#line 978 "../Code/d_game.cpp"
                    
#line 979 "../Code/d_game.cpp"
                    DraggingKeyFrame->FrameIndex = ClosestFrameIndexToMouse;
#line 980 "../Code/d_game.cpp"
                    
#line 981 "../Code/d_game.cpp"
                    AddToHashTable(DraggingKeyFrame->FrameIndex , DraggingKeyFrameIndex , &CurrentBone->KeyFrameHashTable , MAX_KEY_FRAME);
#line 982 "../Code/d_game.cpp"
                    
#line 983 "../Code/d_game.cpp"
                    List_ForeachEx(NextKeyFrameIndex , DraggingKeyFrame->OrderListNodeIndex , NodeIndex , &CurrentBone->KeyFrameOrderList)
#line 984 "../Code/d_game.cpp"
                    {
#line 985 "../Code/d_game.cpp"
                        _KeyFrame * NextKeyFrame = KeyFrameArray + NextKeyFrameIndex;
#line 986 "../Code/d_game.cpp"
                        
#line 987 "../Code/d_game.cpp"
                        if(NextKeyFrame->FrameIndex == DraggingKeyFrame->FrameIndex) ASSERT;
#line 988 "../Code/d_game.cpp"
                        if(NextKeyFrame->FrameIndex > DraggingKeyFrame->FrameIndex) break;
#line 989 "../Code/d_game.cpp"
                        
#line 990 "../Code/d_game.cpp"
                        _ListNode * NextKeyFrameNode = CurrentBone->KeyFrameOrderList.NodeArray + NextKeyFrame->OrderListNodeIndex;
#line 991 "../Code/d_game.cpp"
                        _ListNode * DraggingKeyFrameNode = CurrentBone->KeyFrameOrderList.NodeArray + DraggingKeyFrame->OrderListNodeIndex;
#line 992 "../Code/d_game.cpp"
                        
#line 993 "../Code/d_game.cpp"
                        int TempDataIndex = NextKeyFrameNode->DataIndex;
#line 994 "../Code/d_game.cpp"
                        NextKeyFrameNode->DataIndex = DraggingKeyFrameNode->DataIndex;
#line 995 "../Code/d_game.cpp"
                        DraggingKeyFrameNode->DataIndex = TempDataIndex;
#line 996 "../Code/d_game.cpp"
                        
#line 997 "../Code/d_game.cpp"
                        int TempNodeIndex = NextKeyFrame->OrderListNodeIndex;
#line 998 "../Code/d_game.cpp"
                        NextKeyFrame->OrderListNodeIndex = DraggingKeyFrame->OrderListNodeIndex;
#line 999 "../Code/d_game.cpp"
                        DraggingKeyFrame->OrderListNodeIndex = TempNodeIndex;
#line 1000 "../Code/d_game.cpp"
                        
#line 1001 "../Code/d_game.cpp"
                    }
#line 1002 "../Code/d_game.cpp"
                    
#line 1003 "../Code/d_game.cpp"
                    List_ForeachReverseEx(PreviousKeyFrameIndex , DraggingKeyFrame->OrderListNodeIndex , NodeIndex , &CurrentBone->KeyFrameOrderList)
#line 1004 "../Code/d_game.cpp"
                    {
#line 1005 "../Code/d_game.cpp"
                        _KeyFrame * PreviousKeyFrame = KeyFrameArray + PreviousKeyFrameIndex;
#line 1006 "../Code/d_game.cpp"
                        
#line 1007 "../Code/d_game.cpp"
                        if(PreviousKeyFrame->FrameIndex == ClosestFrameIndexToMouse) ASSERT;
#line 1008 "../Code/d_game.cpp"
                        if(PreviousKeyFrame->FrameIndex < ClosestFrameIndexToMouse) break;
#line 1009 "../Code/d_game.cpp"
                        
#line 1010 "../Code/d_game.cpp"
                        _ListNode * PreviousKeyFrameNode = CurrentBone->KeyFrameOrderList.NodeArray + PreviousKeyFrame->OrderListNodeIndex;
#line 1011 "../Code/d_game.cpp"
                        _ListNode * DraggingKeyFrameNode = CurrentBone->KeyFrameOrderList.NodeArray + DraggingKeyFrame->OrderListNodeIndex;
#line 1012 "../Code/d_game.cpp"
                        
#line 1013 "../Code/d_game.cpp"
                        int TempDataIndex = PreviousKeyFrameNode->DataIndex;
#line 1014 "../Code/d_game.cpp"
                        PreviousKeyFrameNode->DataIndex = DraggingKeyFrameNode->DataIndex;
#line 1015 "../Code/d_game.cpp"
                        DraggingKeyFrameNode->DataIndex = TempDataIndex;
#line 1016 "../Code/d_game.cpp"
                        
#line 1017 "../Code/d_game.cpp"
                        int TempNodeIndex = PreviousKeyFrame->OrderListNodeIndex;
#line 1018 "../Code/d_game.cpp"
                        PreviousKeyFrame->OrderListNodeIndex = DraggingKeyFrame->OrderListNodeIndex;
#line 1019 "../Code/d_game.cpp"
                        DraggingKeyFrame->OrderListNodeIndex = TempNodeIndex;
#line 1020 "../Code/d_game.cpp"
                        
#line 1021 "../Code/d_game.cpp"
                    }
#line 1022 "../Code/d_game.cpp"
                    
#line 1023 "../Code/d_game.cpp"
                }
#line 1024 "../Code/d_game.cpp"
                
#line 1025 "../Code/d_game.cpp"
                
#line 1026 "../Code/d_game.cpp"
            }
#line 1027 "../Code/d_game.cpp"
        }
#line 1028 "../Code/d_game.cpp"
	}
#line 1029 "../Code/d_game.cpp"
#endif
#line 1030 "../Code/d_game.cpp"
    
#line 1031 "../Code/d_game.cpp"
    for (int BoneIndex = 0; BoneIndex < BoneCount; BoneIndex++)
#line 1032 "../Code/d_game.cpp"
    {
#line 1033 "../Code/d_game.cpp"
        
#line 1034 "../Code/d_game.cpp"
        _Bone * CurrentBone = Editor->StickMan[BoneIndex];
#line 1035 "../Code/d_game.cpp"
        
#line 1036 "../Code/d_game.cpp"
        _KeyFrame * KeyFrameArray = (_KeyFrame *)CurrentBone->KeyFrameArray;
#line 1037 "../Code/d_game.cpp"
        _ListNode * KeyFrameOrderNodeArray = (_ListNode *)CurrentBone->KeyFrameOrderList.NodeArray;
#line 1038 "../Code/d_game.cpp"
        
#line 1039 "../Code/d_game.cpp"
#if 1
#line 1040 "../Code/d_game.cpp"
        int ClosestKeyFrameOrderNodeIndex = CurrentBone->FirstKeyFrameOnPointerLeft_OrderListIndex;
#line 1041 "../Code/d_game.cpp"
        if(ClosestKeyFrameOrderNodeIndex == -1) ClosestKeyFrameOrderNodeIndex = CurrentBone->KeyFrameOrderList.NodeArray[N_NodeHead].Next;
#line 1042 "../Code/d_game.cpp"
        
#line 1043 "../Code/d_game.cpp"
        _ListNode * ClosestKeyFrameNode = KeyFrameOrderNodeArray + ClosestKeyFrameOrderNodeIndex;
#line 1044 "../Code/d_game.cpp"
        _KeyFrame * ClosestKeyFrame =  KeyFrameArray + ClosestKeyFrameNode->DataIndex;
#line 1045 "../Code/d_game.cpp"
        
#line 1046 "../Code/d_game.cpp"
        if(Editor->PointerFrameIndex > ClosestKeyFrame->FrameIndex)
#line 1047 "../Code/d_game.cpp"
        {
#line 1048 "../Code/d_game.cpp"
            
#line 1049 "../Code/d_game.cpp"
            List_ForeachEx(NextKeyFrameIndex , ClosestKeyFrameOrderNodeIndex , NodeIndex , &CurrentBone->KeyFrameOrderList)
#line 1050 "../Code/d_game.cpp"
            {
#line 1051 "../Code/d_game.cpp"
                
#line 1052 "../Code/d_game.cpp"
                _KeyFrame * NextKeyFrame = KeyFrameArray + NextKeyFrameIndex;
#line 1053 "../Code/d_game.cpp"
                if(NextKeyFrame->FrameIndex > Editor->PointerFrameIndex) break;
#line 1054 "../Code/d_game.cpp"
                
#line 1055 "../Code/d_game.cpp"
                ClosestKeyFrameOrderNodeIndex = NodeIndex;
#line 1056 "../Code/d_game.cpp"
            }
#line 1057 "../Code/d_game.cpp"
            
#line 1058 "../Code/d_game.cpp"
        }
#line 1059 "../Code/d_game.cpp"
        else if( Editor->PointerFrameIndex < ClosestKeyFrame->FrameIndex)
#line 1060 "../Code/d_game.cpp"
        {
#line 1061 "../Code/d_game.cpp"
            
#line 1062 "../Code/d_game.cpp"
            List_ForeachReverseEx(NextKeyFrameIndex , ClosestKeyFrameOrderNodeIndex , NodeIndex , &CurrentBone->KeyFrameOrderList)
#line 1063 "../Code/d_game.cpp"
            {
#line 1064 "../Code/d_game.cpp"
                _KeyFrame * PreviousKeyFrame = KeyFrameArray + NextKeyFrameIndex;
#line 1065 "../Code/d_game.cpp"
                if(PreviousKeyFrame->FrameIndex < Editor->PointerFrameIndex) break;
#line 1066 "../Code/d_game.cpp"
                
#line 1067 "../Code/d_game.cpp"
                
#line 1068 "../Code/d_game.cpp"
                ClosestKeyFrameOrderNodeIndex = NodeIndex;
#line 1069 "../Code/d_game.cpp"
            }
#line 1070 "../Code/d_game.cpp"
            
#line 1071 "../Code/d_game.cpp"
        }
#line 1072 "../Code/d_game.cpp"
        
#line 1073 "../Code/d_game.cpp"
        if(ClosestKeyFrameOrderNodeIndex != -1)
#line 1074 "../Code/d_game.cpp"
        {
#line 1075 "../Code/d_game.cpp"
            int KeyFrameDataIndex = CurrentBone->KeyFrameOrderList.NodeArray[ClosestKeyFrameOrderNodeIndex].DataIndex;
#line 1076 "../Code/d_game.cpp"
            
#line 1077 "../Code/d_game.cpp"
            if(KeyFrameDataIndex == -1)
#line 1078 "../Code/d_game.cpp"
            {
#line 1079 "../Code/d_game.cpp"
                ClosestKeyFrameOrderNodeIndex = -1;
#line 1080 "../Code/d_game.cpp"
            }
#line 1081 "../Code/d_game.cpp"
            else
#line 1082 "../Code/d_game.cpp"
            {
#line 1083 "../Code/d_game.cpp"
                
#line 1084 "../Code/d_game.cpp"
                int NewClosestKeyFrame_FrameIndex = KeyFrameArray[KeyFrameDataIndex].FrameIndex;
#line 1085 "../Code/d_game.cpp"
                if(Editor->PointerFrameIndex < NewClosestKeyFrame_FrameIndex ) ClosestKeyFrameOrderNodeIndex = -1;
#line 1086 "../Code/d_game.cpp"
                
#line 1087 "../Code/d_game.cpp"
            }
#line 1088 "../Code/d_game.cpp"
            
#line 1089 "../Code/d_game.cpp"
        }
#line 1090 "../Code/d_game.cpp"
        
#line 1091 "../Code/d_game.cpp"
        CurrentBone->FirstKeyFrameOnPointerLeft_OrderListIndex = ClosestKeyFrameOrderNodeIndex;
#line 1092 "../Code/d_game.cpp"
#endif
#line 1093 "../Code/d_game.cpp"
    }
#line 1094 "../Code/d_game.cpp"
    
#line 1095 "../Code/d_game.cpp"
	_Rect FramePointerRect = GetRect();
#line 1096 "../Code/d_game.cpp"
	FramePointerRect.Position = PointerPosition;
#line 1097 "../Code/d_game.cpp"
	FramePointerRect.Size.y = TimelineRect.Size.y * 1.2f;
#line 1098 "../Code/d_game.cpp"
	FramePointerRect.Size.x = PixelToWidth(10);
#line 1099 "../Code/d_game.cpp"
    
#line 1100 "../Code/d_game.cpp"
	bool dragging_or_hovering_frame_pointer = false;
#line 1101 "../Code/d_game.cpp"
    
#line 1102 "../Code/d_game.cpp"
	if (CheckCollisionRectMouse(FramePointerRect))
#line 1103 "../Code/d_game.cpp"
	{
#line 1104 "../Code/d_game.cpp"
		if (MouseButtonPressed(MOUSE_BUTTON_LEFT))
#line 1105 "../Code/d_game.cpp"
		{
#line 1106 "../Code/d_game.cpp"
			dragging_frame_pointer = true;
#line 1107 "../Code/d_game.cpp"
		}
#line 1108 "../Code/d_game.cpp"
        
#line 1109 "../Code/d_game.cpp"
		dragging_or_hovering_frame_pointer = true;
#line 1110 "../Code/d_game.cpp"
	}
#line 1111 "../Code/d_game.cpp"
    
#line 1112 "../Code/d_game.cpp"
	if (dragging_frame_pointer) dragging_or_hovering_frame_pointer = true;
#line 1113 "../Code/d_game.cpp"
    
#line 1114 "../Code/d_game.cpp"
	if (dragging_or_hovering_frame_pointer)
#line 1115 "../Code/d_game.cpp"
	{
#line 1116 "../Code/d_game.cpp"
		DrawRect(FramePointerRect , YELLOW);
#line 1117 "../Code/d_game.cpp"
	}
#line 1118 "../Code/d_game.cpp"
	else
#line 1119 "../Code/d_game.cpp"
	{
#line 1120 "../Code/d_game.cpp"
		DrawRect(FramePointerRect , WHITE);
#line 1121 "../Code/d_game.cpp"
	}
#line 1122 "../Code/d_game.cpp"
    
#line 1123 "../Code/d_game.cpp"
    
#line 1124 "../Code/d_game.cpp"
	Vector3 AddNewFrameButtonPosition = TimelineRect.Position;
#line 1125 "../Code/d_game.cpp"
	AddNewFrameButtonPosition.y += TimelineRect.Size.y*0.5f;
#line 1126 "../Code/d_game.cpp"
	AddNewFrameButtonPosition.x -= TimelineRect.Size.x*0.5f;
#line 1127 "../Code/d_game.cpp"
	
#line 1128 "../Code/d_game.cpp"
	DrawingMenu TimelineOperationMenu = StartDrawMenu({AddNewFrameButtonPosition.x , AddNewFrameButtonPosition.y} , true ,GMT_ToTheRight);
#line 1129 "../Code/d_game.cpp"
	TimelineOperationMenu.CurrentButtonPosition.y += PixelToHeight(45);
#line 1130 "../Code/d_game.cpp"
	TimelineOperationMenu.ButtonExtraOffset = PixelToWidth(5);
#line 1131 "../Code/d_game.cpp"
    
#line 1132 "../Code/d_game.cpp"
	bool PlayStartOrStop = false;
#line 1133 "../Code/d_game.cpp"
	
#line 1134 "../Code/d_game.cpp"
	if (DrawMenuButton(&TimelineOperationMenu , Editor->Playing ? L"播放中" : L"播放" ))
#line 1135 "../Code/d_game.cpp"
	{
#line 1136 "../Code/d_game.cpp"
		PlayStartOrStop = true;
#line 1137 "../Code/d_game.cpp"
	}
#line 1138 "../Code/d_game.cpp"
    
#line 1139 "../Code/d_game.cpp"
	DrawMenuIntInput(&TimelineOperationMenu , L"開始幀 : %d" , &Editor->StartFrameIndex);
#line 1140 "../Code/d_game.cpp"
	DrawMenuIntInput(&TimelineOperationMenu , L"結束幀 : %d" , &Editor->EndFrameIndex);
#line 1141 "../Code/d_game.cpp"
    
#line 1142 "../Code/d_game.cpp"
	localPersist float StartTime = 0;
#line 1143 "../Code/d_game.cpp"
	localPersist float EndTime = 0;
#line 1144 "../Code/d_game.cpp"
    
#line 1145 "../Code/d_game.cpp"
	StartTime = ((float)Editor->StartFrameIndex) / ((float)FRAME_PER_SECOND);
#line 1146 "../Code/d_game.cpp"
	EndTime = ((float)Editor->EndFrameIndex) / ((float)FRAME_PER_SECOND);
#line 1147 "../Code/d_game.cpp"
    
#line 1148 "../Code/d_game.cpp"
	DrawMenuFloatInput(&TimelineOperationMenu , L"開始時間 : %f" , &StartTime);
#line 1149 "../Code/d_game.cpp"
	DrawMenuFloatInput(&TimelineOperationMenu , L"結束時間 : %f" , &EndTime);
#line 1150 "../Code/d_game.cpp"
    
#line 1151 "../Code/d_game.cpp"
	Editor->StartFrameIndex = StartTime * FRAME_PER_SECOND;	
#line 1152 "../Code/d_game.cpp"
	Editor->EndFrameIndex = EndTime * FRAME_PER_SECOND;
#line 1153 "../Code/d_game.cpp"
    
#line 1154 "../Code/d_game.cpp"
	if (KeyPressed(KEY_SPACE))
#line 1155 "../Code/d_game.cpp"
	{
#line 1156 "../Code/d_game.cpp"
		PlayStartOrStop = true;
#line 1157 "../Code/d_game.cpp"
	}
#line 1158 "../Code/d_game.cpp"
    
#line 1159 "../Code/d_game.cpp"
	if (PlayStartOrStop)
#line 1160 "../Code/d_game.cpp"
	{
#line 1161 "../Code/d_game.cpp"
		Editor->Playing = !Editor->Playing;
#line 1162 "../Code/d_game.cpp"
	}
#line 1163 "../Code/d_game.cpp"
    
#line 1164 "../Code/d_game.cpp"
	if (!Editor->Playing)
#line 1165 "../Code/d_game.cpp"
	{
#line 1166 "../Code/d_game.cpp"
		Editor->PlayTimer = 0;
#line 1167 "../Code/d_game.cpp"
	}
#line 1168 "../Code/d_game.cpp"
	else
#line 1169 "../Code/d_game.cpp"
	{
#line 1170 "../Code/d_game.cpp"
		Editor->PlayTimer += DeltaTime;
#line 1171 "../Code/d_game.cpp"
        
#line 1172 "../Code/d_game.cpp"
		if (Editor->PlayTimer > FRAME_TIME)
#line 1173 "../Code/d_game.cpp"
		{
#line 1174 "../Code/d_game.cpp"
			Editor->PlayTimer -= FRAME_TIME;
#line 1175 "../Code/d_game.cpp"
			Editor->PointerFrameIndex++;
#line 1176 "../Code/d_game.cpp"
            
#line 1177 "../Code/d_game.cpp"
			if (Editor->PointerFrameIndex > Editor->EndFrameIndex)
#line 1178 "../Code/d_game.cpp"
			{
#line 1179 "../Code/d_game.cpp"
				Editor->PointerFrameIndex = Editor->StartFrameIndex;
#line 1180 "../Code/d_game.cpp"
			}
#line 1181 "../Code/d_game.cpp"
		}
#line 1182 "../Code/d_game.cpp"
	}
#line 1183 "../Code/d_game.cpp"
    
#line 1184 "../Code/d_game.cpp"
    bool AddNewKeyFrame = false;
#line 1185 "../Code/d_game.cpp"
    bool RemoveKeyFrame = false;
#line 1186 "../Code/d_game.cpp"
    
#line 1187 "../Code/d_game.cpp"
    if(Editor->SelectedBoneCount >0)
#line 1188 "../Code/d_game.cpp"
    {
#line 1189 "../Code/d_game.cpp"
        AddNewKeyFrame = DrawMenuButton(&TimelineOperationMenu , L"添加幀");
#line 1190 "../Code/d_game.cpp"
        RemoveKeyFrame = DrawMenuButton(&TimelineOperationMenu , L"刪除幀");
#line 1191 "../Code/d_game.cpp"
    }
#line 1192 "../Code/d_game.cpp"
    
#line 1193 "../Code/d_game.cpp"
    for(int StackIndex = 0 ; StackIndex < Editor->SelectedBoneCount ; StackIndex++)
#line 1194 "../Code/d_game.cpp"
    {
#line 1195 "../Code/d_game.cpp"
        _Bone * SelectedBone = Editor->StickMan[Editor->SelectedBoneStack[StackIndex]];
#line 1196 "../Code/d_game.cpp"
        
#line 1197 "../Code/d_game.cpp"
        if (AddNewKeyFrame)
#line 1198 "../Code/d_game.cpp"
        {
#line 1199 "../Code/d_game.cpp"
            
#line 1200 "../Code/d_game.cpp"
            bool FrameExisted = false;
#line 1201 "../Code/d_game.cpp"
            
#line 1202 "../Code/d_game.cpp"
            HashTable_Iterate(KeyFrameIndex, Editor->PointerFrameIndex, &SelectedBone->KeyFrameHashTable , MAX_KEY_FRAME )
#line 1203 "../Code/d_game.cpp"
            {
#line 1204 "../Code/d_game.cpp"
                
#line 1205 "../Code/d_game.cpp"
                _KeyFrame persist * KeyFrame = SelectedBone->KeyFrameArray + KeyFrameIndex;
#line 1206 "../Code/d_game.cpp"
                
#line 1207 "../Code/d_game.cpp"
                if (KeyFrame->FrameIndex == Editor->PointerFrameIndex)
#line 1208 "../Code/d_game.cpp"
                {
#line 1209 "../Code/d_game.cpp"
                    FrameExisted = true;
#line 1210 "../Code/d_game.cpp"
                    break;
#line 1211 "../Code/d_game.cpp"
                }
#line 1212 "../Code/d_game.cpp"
                
#line 1213 "../Code/d_game.cpp"
            }
#line 1214 "../Code/d_game.cpp"
            
#line 1215 "../Code/d_game.cpp"
            if (!FrameExisted)
#line 1216 "../Code/d_game.cpp"
            {
#line 1217 "../Code/d_game.cpp"
                
#line 1218 "../Code/d_game.cpp"
                int NewKeyFrameIndex = GetEmtyNodeFromList(&SelectedBone->KeyFrameList , MAX_KEY_FRAME);
#line 1219 "../Code/d_game.cpp"
                
#line 1220 "../Code/d_game.cpp"
                int NewKeyFrameNodeIndex = AddToListTail( NewKeyFrameIndex  ,&SelectedBone->KeyFrameList , MAX_KEY_FRAME);
#line 1221 "../Code/d_game.cpp"
                AddToHashTable( Editor->PointerFrameIndex , NewKeyFrameIndex , &SelectedBone->KeyFrameHashTable , MAX_KEY_FRAME);
#line 1222 "../Code/d_game.cpp"
                
#line 1223 "../Code/d_game.cpp"
                _KeyFrame * KeyFrameArray = (_KeyFrame *)SelectedBone->KeyFrameArray;
#line 1224 "../Code/d_game.cpp"
                _ListNode * KeyFrameNodeArray = (_ListNode *)SelectedBone->KeyFrameList.NodeArray;
#line 1225 "../Code/d_game.cpp"
                
#line 1226 "../Code/d_game.cpp"
                _KeyFrame * NewKeyFrame = KeyFrameArray + NewKeyFrameIndex;
#line 1227 "../Code/d_game.cpp"
                
#line 1228 "../Code/d_game.cpp"
                (*NewKeyFrame) = {};
#line 1229 "../Code/d_game.cpp"
                
#line 1230 "../Code/d_game.cpp"
                NewKeyFrame->FrameIndex = Editor->PointerFrameIndex;
#line 1231 "../Code/d_game.cpp"
                NewKeyFrame->BoneState = SelectedBone->State;
#line 1232 "../Code/d_game.cpp"
                
#line 1233 "../Code/d_game.cpp"
                int NewKeyFrameOrderNodeIndex = -1;
#line 1234 "../Code/d_game.cpp"
                
#line 1235 "../Code/d_game.cpp"
                if(SelectedBone->FirstKeyFrameOnPointerLeft_OrderListIndex == -1)
#line 1236 "../Code/d_game.cpp"
                {
#line 1237 "../Code/d_game.cpp"
                    NewKeyFrameOrderNodeIndex = AddToListHead( NewKeyFrameIndex , &SelectedBone->KeyFrameOrderList , MAX_KEY_FRAME);
#line 1238 "../Code/d_game.cpp"
                }
#line 1239 "../Code/d_game.cpp"
                else
#line 1240 "../Code/d_game.cpp"
                {
#line 1241 "../Code/d_game.cpp"
                    NewKeyFrameOrderNodeIndex = CreateAndInsertToList( false , SelectedBone->FirstKeyFrameOnPointerLeft_OrderListIndex , NewKeyFrameIndex , &SelectedBone->KeyFrameOrderList , MAX_KEY_FRAME);
#line 1242 "../Code/d_game.cpp"
                }
#line 1243 "../Code/d_game.cpp"
                
#line 1244 "../Code/d_game.cpp"
                if(NewKeyFrameOrderNodeIndex == -1) ASSERT;
#line 1245 "../Code/d_game.cpp"
                
#line 1246 "../Code/d_game.cpp"
                SelectedBone->FirstKeyFrameOnPointerLeft_OrderListIndex = NewKeyFrameOrderNodeIndex;
#line 1247 "../Code/d_game.cpp"
                
#line 1248 "../Code/d_game.cpp"
                NewKeyFrame->NodeIndex = NewKeyFrameNodeIndex;
#line 1249 "../Code/d_game.cpp"
                NewKeyFrame->OrderListNodeIndex = NewKeyFrameOrderNodeIndex;
#line 1250 "../Code/d_game.cpp"
            }
#line 1251 "../Code/d_game.cpp"
            
#line 1252 "../Code/d_game.cpp"
        }
#line 1253 "../Code/d_game.cpp"
        
#line 1254 "../Code/d_game.cpp"
        if (RemoveKeyFrame)
#line 1255 "../Code/d_game.cpp"
        {
#line 1256 "../Code/d_game.cpp"
            
#line 1257 "../Code/d_game.cpp"
            int FrameIndex = 0; 
#line 1258 "../Code/d_game.cpp"
            _KeyFrame * KeyFrameArray = (_KeyFrame *)SelectedBone->KeyFrameArray;
#line 1259 "../Code/d_game.cpp"
            HashTable_Iterate(KeyFrameIndex , Editor->PointerFrameIndex , &SelectedBone->KeyFrameHashTable ,MAX_KEY_FRAME)
#line 1260 "../Code/d_game.cpp"
            {
#line 1261 "../Code/d_game.cpp"
                _KeyFrame * KeyFrame = KeyFrameArray + KeyFrameIndex;
#line 1262 "../Code/d_game.cpp"
                
#line 1263 "../Code/d_game.cpp"
                if (KeyFrame->FrameIndex == Editor->PointerFrameIndex)
#line 1264 "../Code/d_game.cpp"
                {
#line 1265 "../Code/d_game.cpp"
                    SelectedBone->FirstKeyFrameOnPointerLeft_OrderListIndex = GetPreviousNodeIndex(SelectedBone->FirstKeyFrameOnPointerLeft_OrderListIndex , &SelectedBone->KeyFrameOrderList);
#line 1266 "../Code/d_game.cpp"
                    
#line 1267 "../Code/d_game.cpp"
                    DeleteFromHashTable(KeyFrame->FrameIndex, KeyFrameIndex , &SelectedBone->KeyFrameHashTable,MAX_KEY_FRAME);
#line 1268 "../Code/d_game.cpp"
                    if(!DeleteFromList(KeyFrame->NodeIndex , &SelectedBone->KeyFrameList)) ASSERT;
#line 1269 "../Code/d_game.cpp"
                    if(!DeleteFromList(KeyFrame->OrderListNodeIndex , &SelectedBone->KeyFrameOrderList)) ASSERT;
#line 1270 "../Code/d_game.cpp"
                    
#line 1271 "../Code/d_game.cpp"
                    break;
#line 1272 "../Code/d_game.cpp"
                }
#line 1273 "../Code/d_game.cpp"
                
#line 1274 "../Code/d_game.cpp"
                //printf( " %d %d %d\n", KeyFrame->FrameIndex , KeyFrame->Bone - Editor->StickMan , KeyFrame - Editor->KeyFrameHash.DataArray);
#line 1275 "../Code/d_game.cpp"
            }
#line 1276 "../Code/d_game.cpp"
            
#line 1277 "../Code/d_game.cpp"
        }
#line 1278 "../Code/d_game.cpp"
        
#line 1279 "../Code/d_game.cpp"
    }
#line 1280 "../Code/d_game.cpp"
    
#line 1281 "../Code/d_game.cpp"
    
#line 1282 "../Code/d_game.cpp"
#if 1
#line 1283 "../Code/d_game.cpp"
    
#line 1284 "../Code/d_game.cpp"
    if(Editor->SelectedBoneCount == 1 )
#line 1285 "../Code/d_game.cpp"
    {
#line 1286 "../Code/d_game.cpp"
        _Bone * SelectedBone = Editor->StickMan[Editor->SelectedBoneStack[0]];
#line 1287 "../Code/d_game.cpp"
        
#line 1288 "../Code/d_game.cpp"
        _KeyFrame * KeyFrameArray = (_KeyFrame *)SelectedBone->KeyFrameArray;
#line 1289 "../Code/d_game.cpp"
        int FirstKeyFrame_OrderListIndex = SelectedBone->FirstKeyFrameOnPointerLeft_OrderListIndex;
#line 1290 "../Code/d_game.cpp"
        
#line 1291 "../Code/d_game.cpp"
        if(FirstKeyFrame_OrderListIndex != -1)
#line 1292 "../Code/d_game.cpp"
        {
#line 1293 "../Code/d_game.cpp"
            
#line 1294 "../Code/d_game.cpp"
            int FirstKeyFrameIndex = GetDataIndexFromNodeIndex(FirstKeyFrame_OrderListIndex , &SelectedBone->KeyFrameOrderList);
#line 1295 "../Code/d_game.cpp"
            _KeyFrame * FirstKeyFrame = KeyFrameArray + FirstKeyFrameIndex;
#line 1296 "../Code/d_game.cpp"
            
#line 1297 "../Code/d_game.cpp"
            char FirstKeyFrameNameBuffer[256] = {};
#line 1298 "../Code/d_game.cpp"
            sprintf(FirstKeyFrameNameBuffer , "First : %d" , FirstKeyFrame->FrameIndex);
#line 1299 "../Code/d_game.cpp"
            
#line 1300 "../Code/d_game.cpp"
            DrawMenuButton(&TimelineOperationMenu , FirstKeyFrameNameBuffer);
#line 1301 "../Code/d_game.cpp"
            
#line 1302 "../Code/d_game.cpp"
        }
#line 1303 "../Code/d_game.cpp"
        else
#line 1304 "../Code/d_game.cpp"
        {
#line 1305 "../Code/d_game.cpp"
            DrawMenuButton(&TimelineOperationMenu , "First : None");
#line 1306 "../Code/d_game.cpp"
        }
#line 1307 "../Code/d_game.cpp"
        
#line 1308 "../Code/d_game.cpp"
        List_Foreach(KeyFrameIndex,&SelectedBone->KeyFrameOrderList )
#line 1309 "../Code/d_game.cpp"
        {
#line 1310 "../Code/d_game.cpp"
            _KeyFrame * KeyFrame = KeyFrameArray + KeyFrameIndex;
#line 1311 "../Code/d_game.cpp"
            
#line 1312 "../Code/d_game.cpp"
            char KeyFrameNameBuffer[256] = {};
#line 1313 "../Code/d_game.cpp"
            
#line 1314 "../Code/d_game.cpp"
            sprintf(KeyFrameNameBuffer , "%d" , KeyFrame->FrameIndex);
#line 1315 "../Code/d_game.cpp"
            
#line 1316 "../Code/d_game.cpp"
            if (DrawMenuButton(&TimelineOperationMenu , KeyFrameNameBuffer))
#line 1317 "../Code/d_game.cpp"
            {
#line 1318 "../Code/d_game.cpp"
                
#line 1319 "../Code/d_game.cpp"
            }
#line 1320 "../Code/d_game.cpp"
        }
#line 1321 "../Code/d_game.cpp"
    }
#line 1322 "../Code/d_game.cpp"
    
#line 1323 "../Code/d_game.cpp"
#endif
#line 1324 "../Code/d_game.cpp"
    
#line 1325 "../Code/d_game.cpp"
	D_GameDraw();
#line 1326 "../Code/d_game.cpp"
	rlDisableFramebuffer();
#line 1327 "../Code/d_game.cpp"
    
#line 1328 "../Code/d_game.cpp"
}
#line 1329 "../Code/d_game.cpp"

#line 1330 "../Code/d_game.cpp"
internal void IterateBoneStructure(_Bone * RootBone)
#line 1331 "../Code/d_game.cpp"
{
#line 1332 "../Code/d_game.cpp"
    _Bone * BoneStack[256] = {};
#line 1333 "../Code/d_game.cpp"
    int BoneStackCount = 0;
#line 1334 "../Code/d_game.cpp"
    
#line 1335 "../Code/d_game.cpp"
    BoneStack[BoneStackCount++] = RootBone;
#line 1336 "../Code/d_game.cpp"
    
#line 1337 "../Code/d_game.cpp"
    //printf("\nBone Start \n");
#line 1338 "../Code/d_game.cpp"
    
#line 1339 "../Code/d_game.cpp"
    for (; BoneStackCount > 0;)
#line 1340 "../Code/d_game.cpp"
    {
#line 1341 "../Code/d_game.cpp"
        _Bone * IteratingBone = BoneStack[--BoneStackCount];
#line 1342 "../Code/d_game.cpp"
        int IteratingBoneIndex = IteratingBone->BoneIndex;
#line 1343 "../Code/d_game.cpp"
        
#line 1344 "../Code/d_game.cpp"
        HashTable_Iterate(ChildBoneIndex , IteratingBoneIndex , &Editor->BoneChildrenHashTable , MAX_KEY_FRAME)
#line 1345 "../Code/d_game.cpp"
        {
#line 1346 "../Code/d_game.cpp"
            
#line 1347 "../Code/d_game.cpp"
#if 0
#line 1348 "../Code/d_game.cpp"
            printf("%s <--- %s\n" , 
#line 1349 "../Code/d_game.cpp"
                   EnumToString(BoneTag)[IteratingBoneIndex] , 
#line 1350 "../Code/d_game.cpp"
                   EnumToString(BoneTag)[ChildBoneIndex]);
#line 1351 "../Code/d_game.cpp"
#endif
#line 1352 "../Code/d_game.cpp"
            
#line 1353 "../Code/d_game.cpp"
            _Bone * ChildBone = Editor->StickMan[ChildBoneIndex];
#line 1354 "../Code/d_game.cpp"
            
#line 1355 "../Code/d_game.cpp"
            Vector3 IteratingBoneEndPosition = IteratingBone->Position;
#line 1356 "../Code/d_game.cpp"
            IteratingBoneEndPosition = Vector3Add(IteratingBoneEndPosition , Vector3RotateByQuaternion( IteratingBone->State.Direction , IteratingBone->Rotation));
#line 1357 "../Code/d_game.cpp"
            
#line 1358 "../Code/d_game.cpp"
            ChildBone->Rotation = QuaternionMultiply(IteratingBone->Rotation, ChildBone->State.LocalRotation);
#line 1359 "../Code/d_game.cpp"
            
#line 1360 "../Code/d_game.cpp"
            ChildBone->Position = Vector3RotateByQuaternion(ChildBone->State.LocalPosition , IteratingBone->Rotation );
#line 1361 "../Code/d_game.cpp"
            
#line 1362 "../Code/d_game.cpp"
            ChildBone->Position = Vector3Add(ChildBone->Position, IteratingBoneEndPosition);
#line 1363 "../Code/d_game.cpp"
            //ChildBone->Position = Vector3Add(ChildBone->Position, Vector3RotateByQuaternion(ChildBone->State.Direction , ChildBone->Rotation));
#line 1364 "../Code/d_game.cpp"
            
#line 1365 "../Code/d_game.cpp"
            BoneStack[BoneStackCount++] = ChildBone;
#line 1366 "../Code/d_game.cpp"
            
#line 1367 "../Code/d_game.cpp"
        }
#line 1368 "../Code/d_game.cpp"
        
#line 1369 "../Code/d_game.cpp"
    }
#line 1370 "../Code/d_game.cpp"
    
#line 1371 "../Code/d_game.cpp"
}
#line 1372 "../Code/d_game.cpp"

#line 1373 "../Code/d_game.cpp"
internal void UpdateBoneStructure()
#line 1374 "../Code/d_game.cpp"
{
#line 1375 "../Code/d_game.cpp"
    
#line 1376 "../Code/d_game.cpp"
	List_Foreach(RootBoneIndex , &Editor->RootBoneList )
#line 1377 "../Code/d_game.cpp"
	{
#line 1378 "../Code/d_game.cpp"
        _Bone * RootBone = Editor->StickMan[RootBoneIndex];
#line 1379 "../Code/d_game.cpp"
        
#line 1380 "../Code/d_game.cpp"
        RootBone->Position = RootBone->State.LocalPosition;
#line 1381 "../Code/d_game.cpp"
        RootBone->Rotation = RootBone->State.LocalRotation;
#line 1382 "../Code/d_game.cpp"
        
#line 1383 "../Code/d_game.cpp"
        IterateBoneStructure(RootBone);
#line 1384 "../Code/d_game.cpp"
	}
#line 1385 "../Code/d_game.cpp"
    
#line 1386 "../Code/d_game.cpp"
}
#line 1387 "../Code/d_game.cpp"

#line 1388 "../Code/d_game.cpp"
#define DEBUG_IK_DISPLAY 0
#line 1389 "../Code/d_game.cpp"

#line 1390 "../Code/d_game.cpp"
internal void BoneIKUpdate(_Bone * TargetBone , _Bone * PoleBone , _Bone * IKBone , int IterationCount , int BoneChainMaxLength)
#line 1391 "../Code/d_game.cpp"
{
#line 1392 "../Code/d_game.cpp"
    _Bone * BoneStack[256] = {};
#line 1393 "../Code/d_game.cpp"
    int BoneStackCount = 0;
#line 1394 "../Code/d_game.cpp"
    
#line 1395 "../Code/d_game.cpp"
    _Bone * BoneChain[256] = {};
#line 1396 "../Code/d_game.cpp"
    int BoneChainCount = 0;
#line 1397 "../Code/d_game.cpp"
    
#line 1398 "../Code/d_game.cpp"
    BoneStack[BoneStackCount++] = IKBone;
#line 1399 "../Code/d_game.cpp"
    BoneChain[BoneChainCount++] = IKBone;
#line 1400 "../Code/d_game.cpp"
    
#line 1401 "../Code/d_game.cpp"
    int BoneChainLength = 0;
#line 1402 "../Code/d_game.cpp"
    
#line 1403 "../Code/d_game.cpp"
    for(;;)
#line 1404 "../Code/d_game.cpp"
    {
#line 1405 "../Code/d_game.cpp"
        
#line 1406 "../Code/d_game.cpp"
        if(BoneStackCount <= 0) break;
#line 1407 "../Code/d_game.cpp"
        if(BoneChainLength >= BoneChainMaxLength) break;
#line 1408 "../Code/d_game.cpp"
        BoneChainLength++;
#line 1409 "../Code/d_game.cpp"
        
#line 1410 "../Code/d_game.cpp"
        _Bone * IteratingBone = BoneStack[--BoneStackCount];
#line 1411 "../Code/d_game.cpp"
        int IteratingBoneIndex = IteratingBone->BoneIndex;
#line 1412 "../Code/d_game.cpp"
        IteratingBone->State.LocalPosition = {};
#line 1413 "../Code/d_game.cpp"
        
#line 1414 "../Code/d_game.cpp"
        HashTable_Iterate(ParentBoneIndex , IteratingBoneIndex , &Editor->BoneParentHashTable , BoneCount)
#line 1415 "../Code/d_game.cpp"
        {
#line 1416 "../Code/d_game.cpp"
            BoneChain[BoneChainCount++] = Editor->StickMan[ParentBoneIndex];
#line 1417 "../Code/d_game.cpp"
            BoneStack[BoneStackCount++] = Editor->StickMan[ParentBoneIndex];
#line 1418 "../Code/d_game.cpp"
        }
#line 1419 "../Code/d_game.cpp"
        
#line 1420 "../Code/d_game.cpp"
    }
#line 1421 "../Code/d_game.cpp"
    
#line 1422 "../Code/d_game.cpp"
    for(int IterationIndex = 0 ; IterationIndex < IterationCount ; IterationIndex++)
#line 1423 "../Code/d_game.cpp"
    {
#line 1424 "../Code/d_game.cpp"
        Vector3 EndPosition = IKBone->Position; 
#line 1425 "../Code/d_game.cpp"
        EndPosition = Vector3Add(EndPosition , Vector3RotateByQuaternion(IKBone->State.Direction , IKBone->Rotation) );
#line 1426 "../Code/d_game.cpp"
        //DrawQuadLine(GetBillboardQuad(EndPosition , 0.1 , 0.1 ) ,WHITE);
#line 1427 "../Code/d_game.cpp"
        
#line 1428 "../Code/d_game.cpp"
        _Bone * StartBone = BoneChain[BoneChainCount -1];
#line 1429 "../Code/d_game.cpp"
        Vector3 StartPosition = StartBone->Position;
#line 1430 "../Code/d_game.cpp"
        //StartPosition = Vector3Add(StartPosition , StartBone->Position);
#line 1431 "../Code/d_game.cpp"
        
#line 1432 "../Code/d_game.cpp"
        Vector3 bone_chain_direction = Vector3Subtract(EndPosition , StartPosition);
#line 1433 "../Code/d_game.cpp"
        
#line 1434 "../Code/d_game.cpp"
#if DEBUG_IK_DISPLAY
#line 1435 "../Code/d_game.cpp"
        DrawArrowRay(StartPosition ,bone_chain_direction , Fade(WHITE,0.5f) );
#line 1436 "../Code/d_game.cpp"
#endif
#line 1437 "../Code/d_game.cpp"
        
#line 1438 "../Code/d_game.cpp"
        Vector3 BoneDirectionSum = {};
#line 1439 "../Code/d_game.cpp"
        
#line 1440 "../Code/d_game.cpp"
        Vector3 bone_chain_vertical_direction = {};
#line 1441 "../Code/d_game.cpp"
        
#line 1442 "../Code/d_game.cpp"
        //continue;
#line 1443 "../Code/d_game.cpp"
        
#line 1444 "../Code/d_game.cpp"
        for(int BoneChainIndex = BoneChainCount - 1 ; BoneChainIndex >= 1 ; BoneChainIndex--)
#line 1445 "../Code/d_game.cpp"
        {
#line 1446 "../Code/d_game.cpp"
            _Bone * CurrentBone = BoneChain[BoneChainIndex];
#line 1447 "../Code/d_game.cpp"
            
#line 1448 "../Code/d_game.cpp"
            
#line 1449 "../Code/d_game.cpp"
            if(IterationIndex == 0)
#line 1450 "../Code/d_game.cpp"
            {
#line 1451 "../Code/d_game.cpp"
                CurrentBone->Rotation = QuaternionIdentity();
#line 1452 "../Code/d_game.cpp"
            }
#line 1453 "../Code/d_game.cpp"
            
#line 1454 "../Code/d_game.cpp"
            
#line 1455 "../Code/d_game.cpp"
            Vector3 BoneEndDirection = Vector3RotateByQuaternion(CurrentBone->State.Direction , CurrentBone->Rotation);
#line 1456 "../Code/d_game.cpp"
            
#line 1457 "../Code/d_game.cpp"
            Vector3 BoneEnd = Vector3Add(CurrentBone->Position , BoneEndDirection);
#line 1458 "../Code/d_game.cpp"
            
#line 1459 "../Code/d_game.cpp"
            BoneDirectionSum = Vector3Add(BoneEndDirection , BoneDirectionSum);
#line 1460 "../Code/d_game.cpp"
            
#line 1461 "../Code/d_game.cpp"
            Vector3 bone_end_direction_project = Vector3Project(BoneDirectionSum , bone_chain_direction);
#line 1462 "../Code/d_game.cpp"
            
#line 1463 "../Code/d_game.cpp"
            Vector3 bone_chain_start_point_to_bone_end = Vector3Add(StartPosition , bone_end_direction_project);
#line 1464 "../Code/d_game.cpp"
            Vector3 bone_vertical_direction_to_bone_chain = Vector3Subtract(BoneEnd , bone_chain_start_point_to_bone_end);
#line 1465 "../Code/d_game.cpp"
            
#line 1466 "../Code/d_game.cpp"
#if DEBUG_IK_DISPLAY
#line 1467 "../Code/d_game.cpp"
            DrawArrowRay(bone_chain_start_point_to_bone_end , bone_vertical_direction_to_bone_chain , Fade(WHITE , 0.5f) );
#line 1468 "../Code/d_game.cpp"
#endif
#line 1469 "../Code/d_game.cpp"
            
#line 1470 "../Code/d_game.cpp"
            bone_chain_vertical_direction = Vector3Add(bone_chain_vertical_direction , bone_vertical_direction_to_bone_chain);
#line 1471 "../Code/d_game.cpp"
        }
#line 1472 "../Code/d_game.cpp"
        
#line 1473 "../Code/d_game.cpp"
        bone_chain_vertical_direction = Vector3Normalize(bone_chain_vertical_direction);
#line 1474 "../Code/d_game.cpp"
        
#line 1475 "../Code/d_game.cpp"
        Vector3 start_to_pole_direction = Vector3Subtract(PoleBone->Position , StartPosition);
#line 1476 "../Code/d_game.cpp"
        
#line 1477 "../Code/d_game.cpp"
        Vector3 pole_project_point_on_bone_chain = Vector3Project(start_to_pole_direction , bone_chain_direction);
#line 1478 "../Code/d_game.cpp"
        pole_project_point_on_bone_chain = Vector3Add(StartPosition , pole_project_point_on_bone_chain);
#line 1479 "../Code/d_game.cpp"
        Vector3 pole_direction = Vector3Subtract(PoleBone->Position , pole_project_point_on_bone_chain);
#line 1480 "../Code/d_game.cpp"
        pole_direction = Vector3Normalize(pole_direction);
#line 1481 "../Code/d_game.cpp"
        
#line 1482 "../Code/d_game.cpp"
        Vector3 bone_chain_centre_point = Vector3Lerp(StartPosition , EndPosition , 0.5);
#line 1483 "../Code/d_game.cpp"
        
#line 1484 "../Code/d_game.cpp"
#if DEBUG_IK_DISPLAY
#line 1485 "../Code/d_game.cpp"
        DrawArrowRay(bone_chain_centre_point , bone_chain_vertical_direction , Fade(RED, 0.5) );
#line 1486 "../Code/d_game.cpp"
        DrawArrowRay(bone_chain_centre_point , pole_direction , Fade(WHITE, 0.5) );
#line 1487 "../Code/d_game.cpp"
#endif
#line 1488 "../Code/d_game.cpp"
        
#line 1489 "../Code/d_game.cpp"
        Quaternion bone_chain_to_pole_rotation = QuaternionFromVector3ToVector3(bone_chain_vertical_direction , pole_direction);
#line 1490 "../Code/d_game.cpp"
        
#line 1491 "../Code/d_game.cpp"
        for(int  BoneChainIndex = BoneChainCount - 1 ; BoneChainIndex >= 0 ; BoneChainIndex--)
#line 1492 "../Code/d_game.cpp"
        {
#line 1493 "../Code/d_game.cpp"
            _Bone * CurrentBone = BoneChain[BoneChainIndex];
#line 1494 "../Code/d_game.cpp"
            
#line 1495 "../Code/d_game.cpp"
            CurrentBone->Rotation = QuaternionMultiply(bone_chain_to_pole_rotation , CurrentBone->Rotation );
#line 1496 "../Code/d_game.cpp"
            
#line 1497 "../Code/d_game.cpp"
            if(BoneChainIndex != 0)
#line 1498 "../Code/d_game.cpp"
            {
#line 1499 "../Code/d_game.cpp"
                _Bone * ChildBone = BoneChain[BoneChainIndex -1];
#line 1500 "../Code/d_game.cpp"
                
#line 1501 "../Code/d_game.cpp"
                ChildBone->Position = Vector3RotateByQuaternion(CurrentBone->State.Direction ,CurrentBone->Rotation );
#line 1502 "../Code/d_game.cpp"
                ChildBone->Position = Vector3Add(ChildBone->Position , CurrentBone->Position);
#line 1503 "../Code/d_game.cpp"
            }
#line 1504 "../Code/d_game.cpp"
        }
#line 1505 "../Code/d_game.cpp"
        
#line 1506 "../Code/d_game.cpp"
        for(int  BoneChainIndex = BoneChainCount - 1 ; BoneChainIndex >= 0 ; BoneChainIndex--)
#line 1507 "../Code/d_game.cpp"
        {
#line 1508 "../Code/d_game.cpp"
            _Bone * ParentBone = BoneChain[BoneChainIndex];
#line 1509 "../Code/d_game.cpp"
            _Bone * ChildBone = BoneChain[BoneChainIndex - 1];
#line 1510 "../Code/d_game.cpp"
            
#line 1511 "../Code/d_game.cpp"
            Vector3 DirectionToEndBone = Vector3Subtract(EndPosition ,ParentBone->Position);
#line 1512 "../Code/d_game.cpp"
            DirectionToEndBone = Vector3Normalize(DirectionToEndBone);
#line 1513 "../Code/d_game.cpp"
            
#line 1514 "../Code/d_game.cpp"
            Vector3 DirectionToTargetBone = Vector3Subtract(TargetBone->Position ,ParentBone->Position);
#line 1515 "../Code/d_game.cpp"
            DirectionToTargetBone = Vector3Normalize(DirectionToTargetBone);
#line 1516 "../Code/d_game.cpp"
            
#line 1517 "../Code/d_game.cpp"
#if DEBUG_IK_DISPLAY
#line 1518 "../Code/d_game.cpp"
            DrawArrowRay(ParentBone->Position, DirectionToEndBone , BLUE);
#line 1519 "../Code/d_game.cpp"
            DrawArrowRay(ParentBone->Position, DirectionToTargetBone , GREEN);
#line 1520 "../Code/d_game.cpp"
#endif
#line 1521 "../Code/d_game.cpp"
            
#line 1522 "../Code/d_game.cpp"
            //printf("%f \n" , Vector3Angle(DirectionToEndBone , DirectionToTargetBone) * RAD2DEG);
#line 1523 "../Code/d_game.cpp"
            
#line 1524 "../Code/d_game.cpp"
            //TODO : try Fabrik?
#line 1525 "../Code/d_game.cpp"
            Quaternion BoneRotation = QuaternionFromVector3ToVector3(DirectionToEndBone , DirectionToTargetBone );
#line 1526 "../Code/d_game.cpp"
            //BoneRotation = QuaternionMultiply( BoneRotation, QuaternionInvert(IteratingBone->Rotation));
#line 1527 "../Code/d_game.cpp"
            
#line 1528 "../Code/d_game.cpp"
            ParentBone->Rotation = QuaternionMultiply(BoneRotation , ParentBone->Rotation);
#line 1529 "../Code/d_game.cpp"
            
#line 1530 "../Code/d_game.cpp"
            //IteratingBone->State.LocalRotation = QuaternionIdentity();
#line 1531 "../Code/d_game.cpp"
            
#line 1532 "../Code/d_game.cpp"
            Vector3 ParentBoneEndOffset = Vector3RotateByQuaternion( ParentBone->State.Direction , ParentBone->Rotation );
#line 1533 "../Code/d_game.cpp"
            
#line 1534 "../Code/d_game.cpp"
            if(BoneChainIndex > 0)
#line 1535 "../Code/d_game.cpp"
            {
#line 1536 "../Code/d_game.cpp"
                ChildBone->Position = Vector3Add(ParentBone->Position , ParentBoneEndOffset);
#line 1537 "../Code/d_game.cpp"
            }
#line 1538 "../Code/d_game.cpp"
        }
#line 1539 "../Code/d_game.cpp"
        
#line 1540 "../Code/d_game.cpp"
        
#line 1541 "../Code/d_game.cpp"
    }
#line 1542 "../Code/d_game.cpp"
    
#line 1543 "../Code/d_game.cpp"
    //printf("%s \n", EnumToString(BoneTag)[ BoneChain[0] - StickMan]);
#line 1544 "../Code/d_game.cpp"
    IterateBoneStructure(BoneChain[0]);
#line 1545 "../Code/d_game.cpp"
    
#line 1546 "../Code/d_game.cpp"
}
#line 1547 "../Code/d_game.cpp"

#line 1548 "../Code/d_game.cpp"
internal void DrawAllBone()
#line 1549 "../Code/d_game.cpp"
{
#line 1550 "../Code/d_game.cpp"
	
#line 1551 "../Code/d_game.cpp"
#if 0
#line 1552 "../Code/d_game.cpp"
    
#line 1553 "../Code/d_game.cpp"
    _Rect HeadRect = GetRect();
#line 1554 "../Code/d_game.cpp"
	HeadRect.Position = Editor->StickMan[B_Head].Position;
#line 1555 "../Code/d_game.cpp"
	HeadRect.Size = {0.5,0.5};
#line 1556 "../Code/d_game.cpp"
    
#line 1557 "../Code/d_game.cpp"
	HeadRect.Rotation = QuaternionFromVector3ToVector3({0,1,0} , Editor->Up);
#line 1558 "../Code/d_game.cpp"
	HeadRect.Rotation = QuaternionMultiply(HeadRect.Rotation , QuaternionFromVector3ToVector3( { 1, 0, 0 } , Editor->Right));
#line 1559 "../Code/d_game.cpp"
	DrawRect(HeadRect , 0.25, WHITE );
#line 1560 "../Code/d_game.cpp"
    
#line 1561 "../Code/d_game.cpp"
	                                                                  
                               
  
                               
                                                                                                                           
                               
  
#line 1565 "../Code/d_game.cpp"
    
#line 1566 "../Code/d_game.cpp"
	DrawRoundLine(B_Head , B_UpperBody);
#line 1567 "../Code/d_game.cpp"
	DrawRoundLine(B_LowerBody , B_Pelvic);
#line 1568 "../Code/d_game.cpp"
	DrawRoundLine(B_UpperBody , B_LowerBody);
#line 1569 "../Code/d_game.cpp"
    
#line 1570 "../Code/d_game.cpp"
	DrawRoundLine(B_RightUpperArm , B_UpperBody);
#line 1571 "../Code/d_game.cpp"
	DrawRoundLine(B_RightUpperArm , B_RightLowerArm);
#line 1572 "../Code/d_game.cpp"
	
#line 1573 "../Code/d_game.cpp"
	DrawRoundLine(B_LeftUpperArm , B_UpperBody);
#line 1574 "../Code/d_game.cpp"
	DrawRoundLine(B_LeftUpperArm , B_LeftLowerArm);
#line 1575 "../Code/d_game.cpp"
    
#line 1576 "../Code/d_game.cpp"
	DrawRoundLine(B_Pelvic , B_LeftUpperLeg );
#line 1577 "../Code/d_game.cpp"
	DrawRoundLine(B_LeftUpperLeg , B_LeftLowerLeg);
#line 1578 "../Code/d_game.cpp"
	//DrawRoundLine(LeftLowerLeg , LeftFoot);
#line 1579 "../Code/d_game.cpp"
	
#line 1580 "../Code/d_game.cpp"
	DrawRoundLine(B_Pelvic , B_RightUpperLeg );
#line 1581 "../Code/d_game.cpp"
	DrawRoundLine(B_RightUpperLeg , B_RightLowerLeg);
#line 1582 "../Code/d_game.cpp"
	//DrawRoundLine(RightLowerLeg , RightFoot);
#line 1583 "../Code/d_game.cpp"
#endif
#line 1584 "../Code/d_game.cpp"
    
#line 1585 "../Code/d_game.cpp"
    IterateChildBone(Editor->StickMan[B_BodyOrigin]);
#line 1586 "../Code/d_game.cpp"
    
#line 1587 "../Code/d_game.cpp"
}
#line 1588 "../Code/d_game.cpp"

#line 1589 "../Code/d_game.cpp"
internal void GameUpdate()
#line 1590 "../Code/d_game.cpp"
{
#line 1591 "../Code/d_game.cpp"
	bool WindowSizeChanged = false;
#line 1592 "../Code/d_game.cpp"
	localPersist Vector2 PreviousWindowsSize = {};
#line 1593 "../Code/d_game.cpp"
    
#line 1594 "../Code/d_game.cpp"
	if (PreviousWindowsSize.x != AppData->WindowSize.x)
#line 1595 "../Code/d_game.cpp"
	{
#line 1596 "../Code/d_game.cpp"
		WindowSizeChanged = true;
#line 1597 "../Code/d_game.cpp"
	}
#line 1598 "../Code/d_game.cpp"
    
#line 1599 "../Code/d_game.cpp"
	if (PreviousWindowsSize.y != AppData->WindowSize.y)
#line 1600 "../Code/d_game.cpp"
	{
#line 1601 "../Code/d_game.cpp"
		WindowSizeChanged = true;
#line 1602 "../Code/d_game.cpp"
	}
#line 1603 "../Code/d_game.cpp"
    
#line 1604 "../Code/d_game.cpp"
	PreviousWindowsSize = AppData->WindowSize;
#line 1605 "../Code/d_game.cpp"
    
#line 1606 "../Code/d_game.cpp"
	if (WindowSizeChanged)
#line 1607 "../Code/d_game.cpp"
	{
#line 1608 "../Code/d_game.cpp"
		//why windows size changed to 0 ???????
#line 1609 "../Code/d_game.cpp"
		//wrap rlloadtexture 
#line 1610 "../Code/d_game.cpp"
		if (AppData->WindowSize.x != 0)
#line 1611 "../Code/d_game.cpp"
		{
#line 1612 "../Code/d_game.cpp"
			if (AppData->WindowSize.y != 0)
#line 1613 "../Code/d_game.cpp"
			{
#line 1614 "../Code/d_game.cpp"
				if (RenderState.ScreenFrameBuffer != 0)
#line 1615 "../Code/d_game.cpp"
				{
#line 1616 "../Code/d_game.cpp"
					rlUnloadFramebuffer(RenderState.ScreenFrameBuffer); //do i need to unload you?
#line 1617 "../Code/d_game.cpp"
				}
#line 1618 "../Code/d_game.cpp"
                
#line 1619 "../Code/d_game.cpp"
				RenderState.ScreenFrameBuffer = rlLoadFramebuffer();
#line 1620 "../Code/d_game.cpp"
                
#line 1621 "../Code/d_game.cpp"
				if (RenderState.LightingTexture != 0)
#line 1622 "../Code/d_game.cpp"
				{
#line 1623 "../Code/d_game.cpp"
					rlUnloadTexture(RenderState.LightingTexture );
#line 1624 "../Code/d_game.cpp"
				}
#line 1625 "../Code/d_game.cpp"
                
#line 1626 "../Code/d_game.cpp"
				RenderState.LightingTexture = rlLoadTexture(0, AppData->WindowSize.x, AppData->WindowSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
#line 1627 "../Code/d_game.cpp"
                
#line 1628 "../Code/d_game.cpp"
				if (RenderState.GameWorldTexture != 0)
#line 1629 "../Code/d_game.cpp"
				{
#line 1630 "../Code/d_game.cpp"
					rlUnloadTexture(RenderState.GameWorldTexture);
#line 1631 "../Code/d_game.cpp"
				}
#line 1632 "../Code/d_game.cpp"
                
#line 1633 "../Code/d_game.cpp"
				RenderState.GameWorldTexture = rlLoadTexture(0, AppData->WindowSize.x, AppData->WindowSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
#line 1634 "../Code/d_game.cpp"
                
#line 1635 "../Code/d_game.cpp"
				if (RenderState.GUITexture != 0)
#line 1636 "../Code/d_game.cpp"
				{
#line 1637 "../Code/d_game.cpp"
					rlUnloadTexture(RenderState.GUITexture);
#line 1638 "../Code/d_game.cpp"
				}
#line 1639 "../Code/d_game.cpp"
                
#line 1640 "../Code/d_game.cpp"
				RenderState.GUITexture = rlLoadTexture(0, AppData->WindowSize.x, AppData->WindowSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
#line 1641 "../Code/d_game.cpp"
                
#line 1642 "../Code/d_game.cpp"
				if (RenderState.GameWorldDepthTexture != 0)
#line 1643 "../Code/d_game.cpp"
				{
#line 1644 "../Code/d_game.cpp"
					rlUnloadTexture(RenderState.GameWorldDepthTexture);
#line 1645 "../Code/d_game.cpp"
				}				
#line 1646 "../Code/d_game.cpp"
				RenderState.GameWorldDepthTexture = LoadDepthTexture(AppData->WindowSize.x , AppData->WindowSize.y );
#line 1647 "../Code/d_game.cpp"
                
#line 1648 "../Code/d_game.cpp"
				if (RenderState.BloomTexture != 0)
#line 1649 "../Code/d_game.cpp"
				{
#line 1650 "../Code/d_game.cpp"
					rlUnloadTexture(RenderState.BloomTexture);
#line 1651 "../Code/d_game.cpp"
				}
#line 1652 "../Code/d_game.cpp"
                
#line 1653 "../Code/d_game.cpp"
				RenderState.BloomTexture = rlLoadTexture(0, AppData->WindowSize.x, AppData->WindowSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
#line 1654 "../Code/d_game.cpp"
                
#line 1655 "../Code/d_game.cpp"
				if (RenderState.FirstBlurTexture!= 0)
#line 1656 "../Code/d_game.cpp"
				{
#line 1657 "../Code/d_game.cpp"
					rlUnloadTexture(RenderState.FirstBlurTexture);
#line 1658 "../Code/d_game.cpp"
				}
#line 1659 "../Code/d_game.cpp"
                
#line 1660 "../Code/d_game.cpp"
				RenderState.FirstBlurTexture = rlLoadTexture(0, AppData->WindowSize.x, AppData->WindowSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
#line 1661 "../Code/d_game.cpp"
                
#line 1662 "../Code/d_game.cpp"
                
#line 1663 "../Code/d_game.cpp"
				if (RenderState.BlurTexture != 0)
#line 1664 "../Code/d_game.cpp"
				{
#line 1665 "../Code/d_game.cpp"
					rlUnloadTexture(RenderState.BlurTexture);
#line 1666 "../Code/d_game.cpp"
				}
#line 1667 "../Code/d_game.cpp"
                
#line 1668 "../Code/d_game.cpp"
				RenderState.BlurTexture = rlLoadTexture(0, AppData->WindowSize.x, AppData->WindowSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
#line 1669 "../Code/d_game.cpp"
                
#line 1670 "../Code/d_game.cpp"
			}
#line 1671 "../Code/d_game.cpp"
		}
#line 1672 "../Code/d_game.cpp"
	}
#line 1673 "../Code/d_game.cpp"
    
#line 1674 "../Code/d_game.cpp"
	GL_CATCH;
#line 1675 "../Code/d_game.cpp"
    
#line 1676 "../Code/d_game.cpp"
	Editor->FlatColor = true;
#line 1677 "../Code/d_game.cpp"
    
#line 1678 "../Code/d_game.cpp"
	double MouseX = 0;
#line 1679 "../Code/d_game.cpp"
	double MouseY = 0;
#line 1680 "../Code/d_game.cpp"
    
#line 1681 "../Code/d_game.cpp"
	glfwGetCursorPos(AppData->CurrentWindow, &MouseX, &MouseY);
#line 1682 "../Code/d_game.cpp"
    
#line 1683 "../Code/d_game.cpp"
	Vector2 MousePosition = { MouseX, MouseY };
#line 1684 "../Code/d_game.cpp"
    
#line 1685 "../Code/d_game.cpp"
	AppData->MousePosition = MousePosition;
#line 1686 "../Code/d_game.cpp"
	AppData->MouseRay3D = GetScreenToWorldRayEx(MousePosition, AppData->GameCamera, AppData->WindowSize.x, AppData->WindowSize.y);
#line 1687 "../Code/d_game.cpp"
    
#line 1688 "../Code/d_game.cpp"
	if (!MenuInteruped)
#line 1689 "../Code/d_game.cpp"
	{
#line 1690 "../Code/d_game.cpp"
		Editor->TargetCameraZoom -= AppData->MouseScrollDelta * 0.5f;
#line 1691 "../Code/d_game.cpp"
	}
#line 1692 "../Code/d_game.cpp"
    
#line 1693 "../Code/d_game.cpp"
	if (Editor->TargetCameraZoom < 0.01f) Editor->TargetCameraZoom = 0.01f;
#line 1694 "../Code/d_game.cpp"
	if (Editor->TargetCameraZoom > 20.0f) Editor->TargetCameraZoom = 20.0f;
#line 1695 "../Code/d_game.cpp"
	
#line 1696 "../Code/d_game.cpp"
	Quaternion CameraRotation = QuaternionFromEuler(Editor->CameraEuler.x * DEG2RAD, Editor->CameraEuler.y * DEG2RAD,Editor->CameraEuler.z * DEG2RAD);	
#line 1697 "../Code/d_game.cpp"
    
#line 1698 "../Code/d_game.cpp"
	Editor->CurrentCameraZoom = Lerp(Editor->CurrentCameraZoom , Editor->TargetCameraZoom , 0.2f);
#line 1699 "../Code/d_game.cpp"
	//Editor->CurrentCameraZoom = Editor->TargetCameraZoom ;
#line 1700 "../Code/d_game.cpp"
	
#line 1701 "../Code/d_game.cpp"
	AppData->GameCamera.position = Vector3RotateByQuaternion({0,0,-Editor->CurrentCameraZoom} , CameraRotation );
#line 1702 "../Code/d_game.cpp"
	AppData->GameCamera.position = Vector3Add(AppData->GameCamera.position  , Editor->CameraOffset);
#line 1703 "../Code/d_game.cpp"
	
#line 1704 "../Code/d_game.cpp"
	AppData->GameCamera.target = Editor->CameraOffset;
#line 1705 "../Code/d_game.cpp"
    
#line 1706 "../Code/d_game.cpp"
	AppData->Top = tan((AppData->GameCamera.fovy * 0.5) * DEG2RAD);
#line 1707 "../Code/d_game.cpp"
	AppData->Right = AppData->Top * (AppData->WindowSize.x / AppData->WindowSize.y);
#line 1708 "../Code/d_game.cpp"
    
#line 1709 "../Code/d_game.cpp"
	ViewMatrix = MatrixLookAt(AppData->GameCamera.position, AppData->GameCamera.target, AppData->GameCamera.up);
#line 1710 "../Code/d_game.cpp"
	ProjectMatrix = MatrixPerspective(AppData->GameCamera.fovy * DEG2RAD, (double)AppData->WindowSize.x / (double)AppData->WindowSize.y, 0.0001, 2000);
#line 1711 "../Code/d_game.cpp"
    
#line 1712 "../Code/d_game.cpp"
	MVP_3DWorld = MatrixMultiply(ViewMatrix, ProjectMatrix);
#line 1713 "../Code/d_game.cpp"
    
#line 1714 "../Code/d_game.cpp"
    ScreenMVP = MatrixOrtho(-AppData->Right, AppData->Right, -AppData->Top, AppData->Top, 0, 200);
#line 1715 "../Code/d_game.cpp"
    
#line 1716 "../Code/d_game.cpp"
	UpdateFloatInput();
#line 1717 "../Code/d_game.cpp"
    
#line 1718 "../Code/d_game.cpp"
	ChangeMatrix(ScreenMVP);
#line 1719 "../Code/d_game.cpp"
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);	
#line 1720 "../Code/d_game.cpp"
    
#line 1721 "../Code/d_game.cpp"
	MenuInteruped = false;
#line 1722 "../Code/d_game.cpp"
	Editor_GUI();
#line 1723 "../Code/d_game.cpp"
    
#line 1724 "../Code/d_game.cpp"
	ChangeMatrix(MVP_3DWorld);
#line 1725 "../Code/d_game.cpp"
    
#line 1726 "../Code/d_game.cpp"
	rlDisableBackfaceCulling();
#line 1727 "../Code/d_game.cpp"
	
#line 1728 "../Code/d_game.cpp"
#if 1
#line 1729 "../Code/d_game.cpp"
	rlFramebufferAttach(RenderState.ScreenFrameBuffer, RenderState.GameWorldTexture, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
#line 1730 "../Code/d_game.cpp"
	rlFramebufferAttach(RenderState.ScreenFrameBuffer, RenderState.GameWorldDepthTexture , RL_ATTACHMENT_DEPTH ,RL_ATTACHMENT_TEXTURE2D,  0);
#line 1731 "../Code/d_game.cpp"
    
#line 1732 "../Code/d_game.cpp"
	if (!rlFramebufferComplete(RenderState.ScreenFrameBuffer)) ASSERT;
#line 1733 "../Code/d_game.cpp"
	rlEnableFramebuffer(RenderState.ScreenFrameBuffer);
#line 1734 "../Code/d_game.cpp"
#endif
#line 1735 "../Code/d_game.cpp"
    
#line 1736 "../Code/d_game.cpp"
	GL_CATCH;
#line 1737 "../Code/d_game.cpp"
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#line 1738 "../Code/d_game.cpp"
	glEnable(GL_BLEND);
#line 1739 "../Code/d_game.cpp"
    
#line 1740 "../Code/d_game.cpp"
	glClearColor(0,0,0,0);
#line 1741 "../Code/d_game.cpp"
	glClear(GL_COLOR_BUFFER_BIT);
#line 1742 "../Code/d_game.cpp"
	glClear(GL_DEPTH_BUFFER_BIT);  	
#line 1743 "../Code/d_game.cpp"
    
#line 1744 "../Code/d_game.cpp"
	DrawBackground();
#line 1745 "../Code/d_game.cpp"
    
#line 1746 "../Code/d_game.cpp"
	Editor->PreviousMousePosition = Editor->CurrentMousePosition;
#line 1747 "../Code/d_game.cpp"
	Editor->CurrentMousePosition = AppData->MousePosition;
#line 1748 "../Code/d_game.cpp"
    
#line 1749 "../Code/d_game.cpp"
	Vector2 MousePositionDelta = Editor->CurrentMousePosition;
#line 1750 "../Code/d_game.cpp"
	MousePositionDelta.x -= Editor->PreviousMousePosition.x;
#line 1751 "../Code/d_game.cpp"
	MousePositionDelta.y -= Editor->PreviousMousePosition.y;
#line 1752 "../Code/d_game.cpp"
    
#line 1753 "../Code/d_game.cpp"
	Editor->Right = {ViewMatrix.m0 , ViewMatrix.m4 , ViewMatrix.m8 };
#line 1754 "../Code/d_game.cpp"
	Editor->Up = { ViewMatrix.m1 , ViewMatrix.m5 , ViewMatrix.m9 };
#line 1755 "../Code/d_game.cpp"
    
#line 1756 "../Code/d_game.cpp"
    Editor->BillboardRotation = QuaternionFromVector3ToVector3({0,1,0} , Editor->Up );
#line 1757 "../Code/d_game.cpp"
    Editor->BillboardRotation = QuaternionMultiply( Editor->BillboardRotation ,  QuaternionFromVector3ToVector3( {1,0,0} , Editor->Right));
#line 1758 "../Code/d_game.cpp"
    
#line 1759 "../Code/d_game.cpp"
	if (MouseButtonPressing(MOUSE_BUTTON_MIDDLE))
#line 1760 "../Code/d_game.cpp"
	{
#line 1761 "../Code/d_game.cpp"
        
#line 1762 "../Code/d_game.cpp"
		if (KeyPressing(KEY_LEFT_SHIFT))
#line 1763 "../Code/d_game.cpp"
		{
#line 1764 "../Code/d_game.cpp"
			Vector3 MouseRight = Vector3Scale( Editor->Right , -MousePositionDelta.x * 0.005f);
#line 1765 "../Code/d_game.cpp"
			Vector3 MouseUp = Vector3Scale(Editor->Up , MousePositionDelta.y * 0.005f);
#line 1766 "../Code/d_game.cpp"
            
#line 1767 "../Code/d_game.cpp"
			Vector3 MouseDragOffset = Vector3Add(MouseRight , MouseUp);
#line 1768 "../Code/d_game.cpp"
            
#line 1769 "../Code/d_game.cpp"
			Editor->CameraOffset = Vector3Add(Editor->CameraOffset ,MouseDragOffset);
#line 1770 "../Code/d_game.cpp"
		}
#line 1771 "../Code/d_game.cpp"
		else
#line 1772 "../Code/d_game.cpp"
		{
#line 1773 "../Code/d_game.cpp"
			Editor->CameraEuler.y -= MousePositionDelta.x * 0.1;
#line 1774 "../Code/d_game.cpp"
			Editor->CameraEuler.x += MousePositionDelta.y * 0.1;
#line 1775 "../Code/d_game.cpp"
		}
#line 1776 "../Code/d_game.cpp"
        
#line 1777 "../Code/d_game.cpp"
	}
#line 1778 "../Code/d_game.cpp"
    
#line 1779 "../Code/d_game.cpp"
	int GridHintExtend = 15;
#line 1780 "../Code/d_game.cpp"
    
#line 1781 "../Code/d_game.cpp"
#if 1
#line 1782 "../Code/d_game.cpp"
	for (int AxisIndex = 0 ; AxisIndex < 1; AxisIndex++)
#line 1783 "../Code/d_game.cpp"
	{
#line 1784 "../Code/d_game.cpp"
        
#line 1785 "../Code/d_game.cpp"
		Color GridLineColor = Fade(RED,0.2f);
#line 1786 "../Code/d_game.cpp"
		float GridLineSize = 8;
#line 1787 "../Code/d_game.cpp"
        
#line 1788 "../Code/d_game.cpp"
		if (AxisIndex == 1) GridLineColor = Fade(GREEN, 0.2f);
#line 1789 "../Code/d_game.cpp"
		if (AxisIndex == 2) GridLineColor = Fade(BLUE, 0.2f);
#line 1790 "../Code/d_game.cpp"
        
#line 1791 "../Code/d_game.cpp"
		for (int HorizontalIndex = 0; HorizontalIndex < GridHintExtend * 2 +1; HorizontalIndex++)
#line 1792 "../Code/d_game.cpp"
		{
#line 1793 "../Code/d_game.cpp"
            
#line 1794 "../Code/d_game.cpp"
			float LineOffset = GridHintExtend * GridSize;
#line 1795 "../Code/d_game.cpp"
			int LineIndex = HorizontalIndex - GridHintExtend;
#line 1796 "../Code/d_game.cpp"
            
#line 1797 "../Code/d_game.cpp"
			Vector3 StartPosition = {};
#line 1798 "../Code/d_game.cpp"
            
#line 1799 "../Code/d_game.cpp"
			if (AxisIndex == 2)
#line 1800 "../Code/d_game.cpp"
			{
#line 1801 "../Code/d_game.cpp"
				StartPosition.y -= GridSize * GridHintExtend;        
#line 1802 "../Code/d_game.cpp"
				StartPosition.y += GridSize * HorizontalIndex;
#line 1803 "../Code/d_game.cpp"
			}
#line 1804 "../Code/d_game.cpp"
			else if (AxisIndex == 1)
#line 1805 "../Code/d_game.cpp"
			{				
#line 1806 "../Code/d_game.cpp"
				StartPosition.y -= GridSize * GridHintExtend;        
#line 1807 "../Code/d_game.cpp"
				StartPosition.y += GridSize * HorizontalIndex;
#line 1808 "../Code/d_game.cpp"
			}
#line 1809 "../Code/d_game.cpp"
			else
#line 1810 "../Code/d_game.cpp"
			{
#line 1811 "../Code/d_game.cpp"
				StartPosition.z -= GridSize * GridHintExtend;        
#line 1812 "../Code/d_game.cpp"
				StartPosition.z += GridSize * HorizontalIndex;
#line 1813 "../Code/d_game.cpp"
			}
#line 1814 "../Code/d_game.cpp"
            
#line 1815 "../Code/d_game.cpp"
			Vector3 LeftPoint = StartPosition;
#line 1816 "../Code/d_game.cpp"
			Vector3 RightPoint = StartPosition;
#line 1817 "../Code/d_game.cpp"
            
#line 1818 "../Code/d_game.cpp"
			if (AxisIndex == 2)
#line 1819 "../Code/d_game.cpp"
			{
#line 1820 "../Code/d_game.cpp"
				LeftPoint.x -= LineOffset;
#line 1821 "../Code/d_game.cpp"
				RightPoint.x += LineOffset;
#line 1822 "../Code/d_game.cpp"
			}
#line 1823 "../Code/d_game.cpp"
			else if (AxisIndex == 1)
#line 1824 "../Code/d_game.cpp"
			{
#line 1825 "../Code/d_game.cpp"
				LeftPoint.z -= LineOffset;
#line 1826 "../Code/d_game.cpp"
				RightPoint.z += LineOffset;
#line 1827 "../Code/d_game.cpp"
			}
#line 1828 "../Code/d_game.cpp"
			else
#line 1829 "../Code/d_game.cpp"
			{
#line 1830 "../Code/d_game.cpp"
				LeftPoint.x -= LineOffset;
#line 1831 "../Code/d_game.cpp"
				RightPoint.x += LineOffset;
#line 1832 "../Code/d_game.cpp"
			}
#line 1833 "../Code/d_game.cpp"
            
#line 1834 "../Code/d_game.cpp"
			Color HLineColor = GridLineColor;
#line 1835 "../Code/d_game.cpp"
			if (HorizontalIndex == GridHintExtend) HLineColor = Fade(BLUE,0.5);
#line 1836 "../Code/d_game.cpp"
            
#line 1837 "../Code/d_game.cpp"
			int GridDistance = HorizontalIndex - GridHintExtend;
#line 1838 "../Code/d_game.cpp"
            
#line 1839 "../Code/d_game.cpp"
			if (GridDistance < 0) GridDistance *= -1;
#line 1840 "../Code/d_game.cpp"
            
#line 1841 "../Code/d_game.cpp"
			HLineColor = Fade(HLineColor, ((float)HLineColor.a /255.0)*(1.0-(float)GridDistance/ (float)GridHintExtend));
#line 1842 "../Code/d_game.cpp"
            
#line 1843 "../Code/d_game.cpp"
			DrawRoundLine(StartPosition, LeftPoint,GridLineSize ,HLineColor , Fade(HLineColor ,0));		
#line 1844 "../Code/d_game.cpp"
			DrawRoundLine(StartPosition, RightPoint ,GridLineSize ,HLineColor ,Fade(HLineColor ,0));
#line 1845 "../Code/d_game.cpp"
            
#line 1846 "../Code/d_game.cpp"
		}
#line 1847 "../Code/d_game.cpp"
        
#line 1848 "../Code/d_game.cpp"
		for (int VerticalIndex = 0; VerticalIndex < GridHintExtend * 2 + 1; VerticalIndex++)
#line 1849 "../Code/d_game.cpp"
		{
#line 1850 "../Code/d_game.cpp"
            
#line 1851 "../Code/d_game.cpp"
			float LineOffset = GridHintExtend * GridSize;
#line 1852 "../Code/d_game.cpp"
            
#line 1853 "../Code/d_game.cpp"
			int LineIndex = VerticalIndex - GridHintExtend;
#line 1854 "../Code/d_game.cpp"
            
#line 1855 "../Code/d_game.cpp"
			Vector3 StartPosition = {};
#line 1856 "../Code/d_game.cpp"
            
#line 1857 "../Code/d_game.cpp"
			if (AxisIndex == 2)
#line 1858 "../Code/d_game.cpp"
			{
#line 1859 "../Code/d_game.cpp"
				StartPosition.x -= GridSize * GridHintExtend;
#line 1860 "../Code/d_game.cpp"
				StartPosition.x += GridSize * VerticalIndex;
#line 1861 "../Code/d_game.cpp"
			}
#line 1862 "../Code/d_game.cpp"
			else if (AxisIndex == 1)
#line 1863 "../Code/d_game.cpp"
			{				
#line 1864 "../Code/d_game.cpp"
				StartPosition.z -= GridSize * GridHintExtend;
#line 1865 "../Code/d_game.cpp"
				StartPosition.z += GridSize * VerticalIndex;
#line 1866 "../Code/d_game.cpp"
			}
#line 1867 "../Code/d_game.cpp"
			else
#line 1868 "../Code/d_game.cpp"
			{
#line 1869 "../Code/d_game.cpp"
				StartPosition.x -= GridSize * GridHintExtend;
#line 1870 "../Code/d_game.cpp"
				StartPosition.x += GridSize * VerticalIndex;
#line 1871 "../Code/d_game.cpp"
			}
#line 1872 "../Code/d_game.cpp"
            
#line 1873 "../Code/d_game.cpp"
			Vector3 TopPoint = StartPosition;
#line 1874 "../Code/d_game.cpp"
			Vector3 BottomPoint = StartPosition;
#line 1875 "../Code/d_game.cpp"
            
#line 1876 "../Code/d_game.cpp"
			if (AxisIndex == 2)
#line 1877 "../Code/d_game.cpp"
			{
#line 1878 "../Code/d_game.cpp"
				TopPoint.y += LineOffset;        
#line 1879 "../Code/d_game.cpp"
				BottomPoint.y -= LineOffset;
#line 1880 "../Code/d_game.cpp"
			}
#line 1881 "../Code/d_game.cpp"
			else if (AxisIndex == 1)
#line 1882 "../Code/d_game.cpp"
			{
#line 1883 "../Code/d_game.cpp"
				TopPoint.y += LineOffset;        
#line 1884 "../Code/d_game.cpp"
				BottomPoint.y -= LineOffset;
#line 1885 "../Code/d_game.cpp"
			}
#line 1886 "../Code/d_game.cpp"
			else
#line 1887 "../Code/d_game.cpp"
			{
#line 1888 "../Code/d_game.cpp"
				TopPoint.z += LineOffset;        
#line 1889 "../Code/d_game.cpp"
				BottomPoint.z -= LineOffset;
#line 1890 "../Code/d_game.cpp"
			}
#line 1891 "../Code/d_game.cpp"
            
#line 1892 "../Code/d_game.cpp"
			Color VLineColor = GridLineColor;        
#line 1893 "../Code/d_game.cpp"
			if (VerticalIndex == GridHintExtend) VLineColor = Fade(GREEN,0.5f);
#line 1894 "../Code/d_game.cpp"
            
#line 1895 "../Code/d_game.cpp"
			int GridDistance = VerticalIndex- GridHintExtend;
#line 1896 "../Code/d_game.cpp"
            
#line 1897 "../Code/d_game.cpp"
			if (GridDistance < 0) GridDistance *= -1;
#line 1898 "../Code/d_game.cpp"
            
#line 1899 "../Code/d_game.cpp"
			VLineColor = Fade(VLineColor, ((float)VLineColor.a / 255.0) * (1 - (float)GridDistance / (float)GridHintExtend));
#line 1900 "../Code/d_game.cpp"
            
#line 1901 "../Code/d_game.cpp"
			DrawRoundLine(StartPosition, TopPoint,GridLineSize ,VLineColor ,Fade(VLineColor ,0));        
#line 1902 "../Code/d_game.cpp"
			DrawRoundLine(StartPosition, BottomPoint,GridLineSize ,VLineColor,Fade(VLineColor ,0));
#line 1903 "../Code/d_game.cpp"
            
#line 1904 "../Code/d_game.cpp"
		}
#line 1905 "../Code/d_game.cpp"
        
#line 1906 "../Code/d_game.cpp"
	}
#line 1907 "../Code/d_game.cpp"
    
#line 1908 "../Code/d_game.cpp"
#endif
#line 1909 "../Code/d_game.cpp"
	
#line 1910 "../Code/d_game.cpp"
    if(KeyPressed(KEY_E))
#line 1911 "../Code/d_game.cpp"
    {
#line 1912 "../Code/d_game.cpp"
        Editor->ControlRig = !Editor->ControlRig;
#line 1913 "../Code/d_game.cpp"
    }
#line 1914 "../Code/d_game.cpp"
    
#line 1915 "../Code/d_game.cpp"
	for (int BoneIndex = 0; BoneIndex < BoneCount; BoneIndex++)
#line 1916 "../Code/d_game.cpp"
	{
#line 1917 "../Code/d_game.cpp"
        
#line 1918 "../Code/d_game.cpp"
        _Bone * CurrentBone = Editor->StickMan[BoneIndex];
#line 1919 "../Code/d_game.cpp"
		_KeyFrame * KeyFrameArray = (_KeyFrame*)CurrentBone->KeyFrameArray;
#line 1920 "../Code/d_game.cpp"
        
#line 1921 "../Code/d_game.cpp"
		bool NoInterpolation = true;
#line 1922 "../Code/d_game.cpp"
		
#line 1923 "../Code/d_game.cpp"
		CurrentBone->FreeBone = false;
#line 1924 "../Code/d_game.cpp"
        
#line 1925 "../Code/d_game.cpp"
        int StartKeyFrameNodeIndex = CurrentBone->FirstKeyFrameOnPointerLeft_OrderListIndex;
#line 1926 "../Code/d_game.cpp"
        int EndKeyFrameNodeIndex = -1;
#line 1927 "../Code/d_game.cpp"
        
#line 1928 "../Code/d_game.cpp"
        if(StartKeyFrameNodeIndex == -1)
#line 1929 "../Code/d_game.cpp"
        {
#line 1930 "../Code/d_game.cpp"
            
#line 1931 "../Code/d_game.cpp"
            StartKeyFrameNodeIndex = GetPreviousNodeIndex(N_NodeTail , &CurrentBone->KeyFrameOrderList);
#line 1932 "../Code/d_game.cpp"
            EndKeyFrameNodeIndex = GetNextNodeIndex(N_NodeHead , &CurrentBone->KeyFrameOrderList);
#line 1933 "../Code/d_game.cpp"
            
#line 1934 "../Code/d_game.cpp"
        }
#line 1935 "../Code/d_game.cpp"
        else
#line 1936 "../Code/d_game.cpp"
        {
#line 1937 "../Code/d_game.cpp"
            EndKeyFrameNodeIndex = GetNextNodeIndex(StartKeyFrameNodeIndex , &CurrentBone->KeyFrameOrderList);
#line 1938 "../Code/d_game.cpp"
        }
#line 1939 "../Code/d_game.cpp"
        
#line 1940 "../Code/d_game.cpp"
        if(EndKeyFrameNodeIndex == -1)
#line 1941 "../Code/d_game.cpp"
        {
#line 1942 "../Code/d_game.cpp"
            EndKeyFrameNodeIndex = GetNextNodeIndex(N_NodeHead , &CurrentBone->KeyFrameOrderList);
#line 1943 "../Code/d_game.cpp"
        }
#line 1944 "../Code/d_game.cpp"
        
#line 1945 "../Code/d_game.cpp"
        int TargetKeyFrameIndex = -1;
#line 1946 "../Code/d_game.cpp"
        
#line 1947 "../Code/d_game.cpp"
        int StartKeyFrameIndex = GetDataIndexFromNodeIndex(StartKeyFrameNodeIndex , &CurrentBone->KeyFrameOrderList);
#line 1948 "../Code/d_game.cpp"
        int EndKeyFrameIndex = GetDataIndexFromNodeIndex(EndKeyFrameNodeIndex, &CurrentBone->KeyFrameOrderList);
#line 1949 "../Code/d_game.cpp"
        
#line 1950 "../Code/d_game.cpp"
        if(StartKeyFrameIndex != -1)
#line 1951 "../Code/d_game.cpp"
        {
#line 1952 "../Code/d_game.cpp"
            
#line 1953 "../Code/d_game.cpp"
            if(EndKeyFrameIndex == -1)
#line 1954 "../Code/d_game.cpp"
            {
#line 1955 "../Code/d_game.cpp"
                EndKeyFrameNodeIndex = GetNextDataIndexFromNodeIndex(N_NodeHead , &CurrentBone->KeyFrameOrderList);
#line 1956 "../Code/d_game.cpp"
            }
#line 1957 "../Code/d_game.cpp"
            
#line 1958 "../Code/d_game.cpp"
            if(KeyFrameArray[StartKeyFrameIndex].FrameIndex == Editor->PointerFrameIndex )
#line 1959 "../Code/d_game.cpp"
            {
#line 1960 "../Code/d_game.cpp"
                TargetKeyFrameIndex = StartKeyFrameIndex;
#line 1961 "../Code/d_game.cpp"
                CurrentBone->FreeBone = true;
#line 1962 "../Code/d_game.cpp"
            }
#line 1963 "../Code/d_game.cpp"
            
#line 1964 "../Code/d_game.cpp"
        }
#line 1965 "../Code/d_game.cpp"
        else
#line 1966 "../Code/d_game.cpp"
        {
#line 1967 "../Code/d_game.cpp"
            if(EndKeyFrameIndex == -1)
#line 1968 "../Code/d_game.cpp"
            {
#line 1969 "../Code/d_game.cpp"
                CurrentBone->FreeBone = true;
#line 1970 "../Code/d_game.cpp"
            }
#line 1971 "../Code/d_game.cpp"
        }
#line 1972 "../Code/d_game.cpp"
        
#line 1973 "../Code/d_game.cpp"
        if(StartKeyFrameIndex != -1) 
#line 1974 "../Code/d_game.cpp"
        {
#line 1975 "../Code/d_game.cpp"
            if(EndKeyFrameIndex != -1)
#line 1976 "../Code/d_game.cpp"
            {
#line 1977 "../Code/d_game.cpp"
                NoInterpolation = false;
#line 1978 "../Code/d_game.cpp"
            }
#line 1979 "../Code/d_game.cpp"
            
#line 1980 "../Code/d_game.cpp"
            if(StartKeyFrameIndex == EndKeyFrameIndex)
#line 1981 "../Code/d_game.cpp"
            {
#line 1982 "../Code/d_game.cpp"
                NoInterpolation = true;
#line 1983 "../Code/d_game.cpp"
            }
#line 1984 "../Code/d_game.cpp"
            
#line 1985 "../Code/d_game.cpp"
        }
#line 1986 "../Code/d_game.cpp"
        
#line 1987 "../Code/d_game.cpp"
		if (NoInterpolation)
#line 1988 "../Code/d_game.cpp"
		{
#line 1989 "../Code/d_game.cpp"
			if (TargetKeyFrameIndex != -1)
#line 1990 "../Code/d_game.cpp"
			{
#line 1991 "../Code/d_game.cpp"
				CurrentBone->State = KeyFrameArray[TargetKeyFrameIndex].BoneState;
#line 1992 "../Code/d_game.cpp"
			}
#line 1993 "../Code/d_game.cpp"
            
#line 1994 "../Code/d_game.cpp"
		}
#line 1995 "../Code/d_game.cpp"
		else
#line 1996 "../Code/d_game.cpp"
		{
#line 1997 "../Code/d_game.cpp"
            
#line 1998 "../Code/d_game.cpp"
			_KeyFrame * StartKeyFrame= KeyFrameArray + StartKeyFrameIndex;
#line 1999 "../Code/d_game.cpp"
			_KeyFrame * EndKeyFrame= KeyFrameArray + EndKeyFrameIndex;
#line 2000 "../Code/d_game.cpp"
            
#line 2001 "../Code/d_game.cpp"
			float FrameCount = GetKeyFrameCount(StartKeyFrame , EndKeyFrame);
#line 2002 "../Code/d_game.cpp"
			float FrameOffset = Editor->PointerFrameIndex - StartKeyFrame->FrameIndex;
#line 2003 "../Code/d_game.cpp"
			
#line 2004 "../Code/d_game.cpp"
            if(Editor->PointerFrameIndex < StartKeyFrame->FrameIndex)
#line 2005 "../Code/d_game.cpp"
            {
#line 2006 "../Code/d_game.cpp"
                FrameOffset = Editor->EndFrameIndex - StartKeyFrame->FrameIndex + Editor->PointerFrameIndex - Editor->StartFrameIndex + 1;
#line 2007 "../Code/d_game.cpp"
            }
#line 2008 "../Code/d_game.cpp"
            
#line 2009 "../Code/d_game.cpp"
            float FrameTime = FrameOffset / FrameCount;
#line 2010 "../Code/d_game.cpp"
            //printf("%f %f %f\n" , FrameOffset, FrameCount , FrameTime);
#line 2011 "../Code/d_game.cpp"
            
#line 2012 "../Code/d_game.cpp"
            Vector3 LineDirection = Vector3Subtract(EndKeyFrame->BoneState.LocalPosition , StartKeyFrame->BoneState.LocalPosition);
#line 2013 "../Code/d_game.cpp"
            LineDirection =Vector3Scale( Vector3Normalize(LineDirection) , 0.1f);
#line 2014 "../Code/d_game.cpp"
            
#line 2015 "../Code/d_game.cpp"
            Vector3 BezierLineStartPosition = StartKeyFrame->BoneState.LocalPosition;
#line 2016 "../Code/d_game.cpp"
            Vector3 BezierStartPoint = Vector3Add(LineDirection , StartKeyFrame->NextBezierOffset);
#line 2017 "../Code/d_game.cpp"
            BezierStartPoint = Vector3Add(BezierLineStartPosition , BezierStartPoint);
#line 2018 "../Code/d_game.cpp"
            
#line 2019 "../Code/d_game.cpp"
            Vector3 BezierLineEndPosition = EndKeyFrame->BoneState.LocalPosition;
#line 2020 "../Code/d_game.cpp"
            Vector3 BezierEndPoint = Vector3Add( Vector3Negate(LineDirection) , EndKeyFrame->PreviousBezierOffset);
#line 2021 "../Code/d_game.cpp"
            BezierEndPoint = Vector3Add(BezierLineEndPosition , BezierEndPoint);
#line 2022 "../Code/d_game.cpp"
            
#line 2023 "../Code/d_game.cpp"
			CurrentBone->State.LocalPosition = Vector3CubicBezier(BezierLineStartPosition , BezierStartPoint , BezierEndPoint ,BezierLineEndPosition ,FrameTime );
#line 2024 "../Code/d_game.cpp"
			
#line 2025 "../Code/d_game.cpp"
            float EaseTime = EaseInOutCubic(FrameTime);
#line 2026 "../Code/d_game.cpp"
			CurrentBone->State.LocalRotation = QuaternionLerp(StartKeyFrame->BoneState.LocalRotation , EndKeyFrame->BoneState.LocalRotation ,EaseTime );
#line 2027 "../Code/d_game.cpp"
            
#line 2028 "../Code/d_game.cpp"
		}
#line 2029 "../Code/d_game.cpp"
	}
#line 2030 "../Code/d_game.cpp"
    
#line 2031 "../Code/d_game.cpp"
    BoneSelection();
#line 2032 "../Code/d_game.cpp"
    
#line 2033 "../Code/d_game.cpp"
    if(!Editor->Playing)
#line 2034 "../Code/d_game.cpp"
    {
#line 2035 "../Code/d_game.cpp"
        
#line 2036 "../Code/d_game.cpp"
        for(int BoneIndex = 0 ; BoneIndex < BoneCount ; BoneIndex++)
#line 2037 "../Code/d_game.cpp"
        {
#line 2038 "../Code/d_game.cpp"
            _Bone * CurrentBone = Editor->StickMan[BoneIndex];
#line 2039 "../Code/d_game.cpp"
            
#line 2040 "../Code/d_game.cpp"
            _KeyFrame * KeyFrameArray = (_KeyFrame * )CurrentBone->KeyFrameArray;
#line 2041 "../Code/d_game.cpp"
            
#line 2042 "../Code/d_game.cpp"
            int KeyFrameCount = 0;
#line 2043 "../Code/d_game.cpp"
            HashTable_Iterate(KeyFrameIndex , Editor->PointerFrameIndex , &CurrentBone->KeyFrameHashTable , MAX_KEY_FRAME)
#line 2044 "../Code/d_game.cpp"
            {
#line 2045 "../Code/d_game.cpp"
                
#line 2046 "../Code/d_game.cpp"
                KeyFrameArray[KeyFrameIndex].BoneState = CurrentBone->State;
#line 2047 "../Code/d_game.cpp"
                
#line 2048 "../Code/d_game.cpp"
                if(KeyFrameCount>0) ASSERT;
#line 2049 "../Code/d_game.cpp"
                KeyFrameCount++;
#line 2050 "../Code/d_game.cpp"
            }
#line 2051 "../Code/d_game.cpp"
            
#line 2052 "../Code/d_game.cpp"
        }
#line 2053 "../Code/d_game.cpp"
        
#line 2054 "../Code/d_game.cpp"
    }
#line 2055 "../Code/d_game.cpp"
    
#line 2056 "../Code/d_game.cpp"
    UpdateBoneStructure();
#line 2057 "../Code/d_game.cpp"
    
#line 2058 "../Code/d_game.cpp"
                                                                                                                                       
                               
     
                               
                                                               
                               
                                                           
                               
                                                       
                               
        
                               
                                                                                           
                               
     
#line 2066 "../Code/d_game.cpp"
    
#line 2067 "../Code/d_game.cpp"
    BoneIKUpdate(B_LeftHandController , B_LeftArmPoleTarget , B_LeftLowerArm , 20 , 1 );
#line 2068 "../Code/d_game.cpp"
    BoneIKUpdate(B_RightHandController , B_RightArmPoleTarget , B_RightLowerArm , 20 , 1 );
#line 2069 "../Code/d_game.cpp"
    
#line 2070 "../Code/d_game.cpp"
    BoneIKUpdate(B_LeftLegController  , B_LeftLegPoleTarget, B_LeftLowerLeg , 20 , 1 );
#line 2071 "../Code/d_game.cpp"
    BoneIKUpdate(B_RightLegController , B_RightLegPoleTarget , B_RightLowerLeg , 20 , 1 );
#line 2072 "../Code/d_game.cpp"
    
#line 2073 "../Code/d_game.cpp"
    D_GameDraw();
#line 2074 "../Code/d_game.cpp"
    
#line 2075 "../Code/d_game.cpp"
    DrawAllBone();
#line 2076 "../Code/d_game.cpp"
    
#line 2077 "../Code/d_game.cpp"
	//something causing it 30 fps when minimize
#line 2078 "../Code/d_game.cpp"
	//printf("FPS %f : %f\n", 1.0/(EndTimeStamp(PreviousLoopCounter)/(1000.0*1000.0)), 1.0 / (EndTimeStamp(GameCounterStamp) / (1000.0 * 1000.0)));
#line 2079 "../Code/d_game.cpp"
	RenderState.FakeDepth = 0;    
#line 2080 "../Code/d_game.cpp"
	D_GameDraw();
#line 2081 "../Code/d_game.cpp"
	
#line 2082 "../Code/d_game.cpp"
	glDisable(GL_DEPTH_TEST);
#line 2083 "../Code/d_game.cpp"
    
#line 2084 "../Code/d_game.cpp"
	rlDisableFramebuffer();
#line 2085 "../Code/d_game.cpp"
    
#line 2086 "../Code/d_game.cpp"
#if 1
#line 2087 "../Code/d_game.cpp"
	if (Editor->FlatColor)
#line 2088 "../Code/d_game.cpp"
	{
#line 2089 "../Code/d_game.cpp"
        
#line 2090 "../Code/d_game.cpp"
		glClearColor(0, 0, 0, 1);
#line 2091 "../Code/d_game.cpp"
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#line 2092 "../Code/d_game.cpp"
        
#line 2093 "../Code/d_game.cpp"
		DrawScreenFlat(RenderState.GameWorldTexture , WHITE);
#line 2094 "../Code/d_game.cpp"
        
#line 2095 "../Code/d_game.cpp"
	}
#line 2096 "../Code/d_game.cpp"
	else
#line 2097 "../Code/d_game.cpp"
	{
#line 2098 "../Code/d_game.cpp"
        
#line 2099 "../Code/d_game.cpp"
		rlFramebufferAttach(RenderState.ScreenFrameBuffer, RenderState.LightingTexture, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
#line 2100 "../Code/d_game.cpp"
		if (!rlFramebufferComplete(RenderState.ScreenFrameBuffer)) ASSERT;
#line 2101 "../Code/d_game.cpp"
        
#line 2102 "../Code/d_game.cpp"
		rlEnableFramebuffer(RenderState.ScreenFrameBuffer);
#line 2103 "../Code/d_game.cpp"
        
#line 2104 "../Code/d_game.cpp"
#if 0
#line 2105 "../Code/d_game.cpp"
        
#line 2106 "../Code/d_game.cpp"
		glClearColor(0, 0, 0, 1);
#line 2107 "../Code/d_game.cpp"
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#line 2108 "../Code/d_game.cpp"
        
#line 2109 "../Code/d_game.cpp"
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_DST_ALPHA, GL_ONE, GL_ONE);
#line 2110 "../Code/d_game.cpp"
		glEnable(GL_BLEND);    
#line 2111 "../Code/d_game.cpp"
        
#line 2112 "../Code/d_game.cpp"
		D_Foreach(D_Light, CurrentLight, GameWorld->LightArray)
#line 2113 "../Code/d_game.cpp"
		{
#line 2114 "../Code/d_game.cpp"
			Vector4 LightColor = CurrentLight->LightColor;
#line 2115 "../Code/d_game.cpp"
			LightColor.w *= CurrentLight->LightStrength;
#line 2116 "../Code/d_game.cpp"
			AddLight(CurrentLight->LightPosition, CurrentLight->LightSize, LightColor);
#line 2117 "../Code/d_game.cpp"
		}
#line 2118 "../Code/d_game.cpp"
        
#line 2119 "../Code/d_game.cpp"
		_ShaderInput * LightShaderInput = AllShaderInputs + S_Point_Light;
#line 2120 "../Code/d_game.cpp"
        
#line 2121 "../Code/d_game.cpp"
		rlEnableShader(LightShaderInput->Shader);
#line 2122 "../Code/d_game.cpp"
		glBindVertexArray(LightShaderInput->VertexArrayObject);
#line 2123 "../Code/d_game.cpp"
        
#line 2124 "../Code/d_game.cpp"
		glActiveTexture(GL_TEXTURE0);
#line 2125 "../Code/d_game.cpp"
		glBindTexture(GL_TEXTURE_2D, RenderState.GameWorldTexture);
#line 2126 "../Code/d_game.cpp"
		glUniform1i(LightShaderInput->UniformLocation[SU_Texture0], 0);
#line 2127 "../Code/d_game.cpp"
        
#line 2128 "../Code/d_game.cpp"
		rlSetUniformMatrix(LightShaderInput->UniformLocation[SU_MVP] , MVP_2DWorld);
#line 2129 "../Code/d_game.cpp"
        
#line 2130 "../Code/d_game.cpp"
		Update_GPU_VertexBufferAll(LightShaderInput, LightData.LightCount);
#line 2131 "../Code/d_game.cpp"
        
#line 2132 "../Code/d_game.cpp"
		glDrawArrays(GL_POINTS, 0, LightData.LightCount);
#line 2133 "../Code/d_game.cpp"
        
#line 2134 "../Code/d_game.cpp"
        
#line 2135 "../Code/d_game.cpp"
		float TargetLightStrength = 0.6f;
#line 2136 "../Code/d_game.cpp"
        
#line 2137 "../Code/d_game.cpp"
		localPersist float LightStrength = 0.3f;
#line 2138 "../Code/d_game.cpp"
		LightStrength = Lerp(LightStrength , TargetLightStrength , 0.36f);
#line 2139 "../Code/d_game.cpp"
        
#line 2140 "../Code/d_game.cpp"
		DrawScreenFlat(RenderState.GameWorldTexture, Fade(WHITE, LightStrength ));
#line 2141 "../Code/d_game.cpp"
        
#line 2142 "../Code/d_game.cpp"
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#line 2143 "../Code/d_game.cpp"
		glEnable(GL_BLEND);
#line 2144 "../Code/d_game.cpp"
#endif
#line 2145 "../Code/d_game.cpp"
        
#line 2146 "../Code/d_game.cpp"
		rlDisableFramebuffer();
#line 2147 "../Code/d_game.cpp"
        
#line 2148 "../Code/d_game.cpp"
		LightData.LightCount = 0;
#line 2149 "../Code/d_game.cpp"
        
#line 2150 "../Code/d_game.cpp"
		rlFramebufferAttach(RenderState.ScreenFrameBuffer, RenderState.BloomTexture, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
#line 2151 "../Code/d_game.cpp"
		if (!rlFramebufferComplete(RenderState.ScreenFrameBuffer)) ASSERT;
#line 2152 "../Code/d_game.cpp"
        
#line 2153 "../Code/d_game.cpp"
		rlEnableFramebuffer(RenderState.ScreenFrameBuffer);
#line 2154 "../Code/d_game.cpp"
        
#line 2155 "../Code/d_game.cpp"
		glClearColor(0, 0, 0, 1);
#line 2156 "../Code/d_game.cpp"
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#line 2157 "../Code/d_game.cpp"
        
#line 2158 "../Code/d_game.cpp"
		_ShaderInput * BloomShader = AllShaderInputs + S_Bloom;
#line 2159 "../Code/d_game.cpp"
        
#line 2160 "../Code/d_game.cpp"
		rlEnableShader(BloomShader->Shader);
#line 2161 "../Code/d_game.cpp"
		glBindVertexArray(BloomShader->VertexArrayObject);
#line 2162 "../Code/d_game.cpp"
        
#line 2163 "../Code/d_game.cpp"
		glActiveTexture(GL_TEXTURE0);
#line 2164 "../Code/d_game.cpp"
		glBindTexture(GL_TEXTURE_2D , RenderState.LightingTexture);
#line 2165 "../Code/d_game.cpp"
		glUniform1i(BloomShader->UniformLocation[SU_Texture0], 0);
#line 2166 "../Code/d_game.cpp"
        
#line 2167 "../Code/d_game.cpp"
		glUniform2f(BloomShader->UniformLocation[SU_ScreenSize] , AppData->WindowSize.x , AppData->WindowSize.y);
#line 2168 "../Code/d_game.cpp"
        
#line 2169 "../Code/d_game.cpp"
		Vector3 * SreenQuadPos = VertexData.Position;
#line 2170 "../Code/d_game.cpp"
		Vector4 * ScreenQuadColorData = VertexData.Color;
#line 2171 "../Code/d_game.cpp"
		Vector2 * ScreenQuadTexCoordData = VertexData.TexCoord;
#line 2172 "../Code/d_game.cpp"
        
#line 2173 "../Code/d_game.cpp"
		int VertCount = 0;
#line 2174 "../Code/d_game.cpp"
        
#line 2175 "../Code/d_game.cpp"
		SreenQuadPos[VertCount++] = { -1, 1, 0 };
#line 2176 "../Code/d_game.cpp"
		SreenQuadPos[VertCount++] = { 1, 1, 0 };
#line 2177 "../Code/d_game.cpp"
		SreenQuadPos[VertCount++] = { 1, -1, 0 };
#line 2178 "../Code/d_game.cpp"
        
#line 2179 "../Code/d_game.cpp"
		SreenQuadPos[VertCount++] = { -1, -1, 0 };
#line 2180 "../Code/d_game.cpp"
		SreenQuadPos[VertCount++] = { -1, 1, 0 };
#line 2181 "../Code/d_game.cpp"
		SreenQuadPos[VertCount++] = { 1, -1, 0 };
#line 2182 "../Code/d_game.cpp"
        
#line 2183 "../Code/d_game.cpp"
		VertCount = 0;
#line 2184 "../Code/d_game.cpp"
        
#line 2185 "../Code/d_game.cpp"
		ScreenQuadTexCoordData[VertCount++] = { 0, 1 };
#line 2186 "../Code/d_game.cpp"
		ScreenQuadTexCoordData[VertCount++] = { 1, 1 };
#line 2187 "../Code/d_game.cpp"
		ScreenQuadTexCoordData[VertCount++] = { 1, 0 };
#line 2188 "../Code/d_game.cpp"
        
#line 2189 "../Code/d_game.cpp"
		ScreenQuadTexCoordData[VertCount++] = { 0, 0 };
#line 2190 "../Code/d_game.cpp"
		ScreenQuadTexCoordData[VertCount++] = { 0, 1 };
#line 2191 "../Code/d_game.cpp"
		ScreenQuadTexCoordData[VertCount++] = { 1, 0 };
#line 2192 "../Code/d_game.cpp"
        
#line 2193 "../Code/d_game.cpp"
		VertCount = 0;
#line 2194 "../Code/d_game.cpp"
        
#line 2195 "../Code/d_game.cpp"
		ScreenQuadColorData[VertCount++] = {1,1,1,1};
#line 2196 "../Code/d_game.cpp"
		ScreenQuadColorData[VertCount++] = {1,1,1,1};
#line 2197 "../Code/d_game.cpp"
		ScreenQuadColorData[VertCount++] = {1,1,1,1};
#line 2198 "../Code/d_game.cpp"
        
#line 2199 "../Code/d_game.cpp"
		ScreenQuadColorData[VertCount++] = {1,1,1,1};
#line 2200 "../Code/d_game.cpp"
		ScreenQuadColorData[VertCount++] = {1,1,1,1};
#line 2201 "../Code/d_game.cpp"
		ScreenQuadColorData[VertCount++] = {1,1,1,1};
#line 2202 "../Code/d_game.cpp"
        
#line 2203 "../Code/d_game.cpp"
		Update_GPU_VertexBufferAll(BloomShader, 6);    
#line 2204 "../Code/d_game.cpp"
		glDrawArrays(GL_TRIANGLES, 0, 6);
#line 2205 "../Code/d_game.cpp"
        
#line 2206 "../Code/d_game.cpp"
		rlDisableFramebuffer();
#line 2207 "../Code/d_game.cpp"
        
#line 2208 "../Code/d_game.cpp"
		glClearColor(0, 0, 0, 1);
#line 2209 "../Code/d_game.cpp"
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#line 2210 "../Code/d_game.cpp"
        
#line 2211 "../Code/d_game.cpp"
		Update_GPU_VertexBufferAll(BloomShader, 6);
#line 2212 "../Code/d_game.cpp"
        
#line 2213 "../Code/d_game.cpp"
		for (int ShaderIndex = S_Blur_H; ShaderIndex <= S_Blur_V; ShaderIndex++)
#line 2214 "../Code/d_game.cpp"
		{
#line 2215 "../Code/d_game.cpp"
			unsigned int TargetBlurTexture = RenderState.BlurTexture;
#line 2216 "../Code/d_game.cpp"
            
#line 2217 "../Code/d_game.cpp"
			if (ShaderIndex == S_Blur_H) TargetBlurTexture = RenderState.FirstBlurTexture;
#line 2218 "../Code/d_game.cpp"
            
#line 2219 "../Code/d_game.cpp"
			rlFramebufferAttach(RenderState.ScreenFrameBuffer, TargetBlurTexture , RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
#line 2220 "../Code/d_game.cpp"
			if (!rlFramebufferComplete(RenderState.ScreenFrameBuffer)) ASSERT;
#line 2221 "../Code/d_game.cpp"
            
#line 2222 "../Code/d_game.cpp"
			rlEnableFramebuffer(RenderState.ScreenFrameBuffer);
#line 2223 "../Code/d_game.cpp"
            
#line 2224 "../Code/d_game.cpp"
			_ShaderInput * BlurShader = AllShaderInputs + ShaderIndex;
#line 2225 "../Code/d_game.cpp"
            
#line 2226 "../Code/d_game.cpp"
			rlEnableShader(BlurShader ->Shader);
#line 2227 "../Code/d_game.cpp"
			glBindVertexArray(BlurShader ->VertexArrayObject);
#line 2228 "../Code/d_game.cpp"
            
#line 2229 "../Code/d_game.cpp"
			unsigned int BaseTexture = RenderState.FirstBlurTexture;
#line 2230 "../Code/d_game.cpp"
			if (ShaderIndex == S_Blur_H) BaseTexture = RenderState.BloomTexture;
#line 2231 "../Code/d_game.cpp"
            
#line 2232 "../Code/d_game.cpp"
			glActiveTexture(GL_TEXTURE0);
#line 2233 "../Code/d_game.cpp"
			glBindTexture(GL_TEXTURE_2D , BaseTexture);
#line 2234 "../Code/d_game.cpp"
			glUniform1i(BlurShader ->UniformLocation[SU_Texture0], 0);
#line 2235 "../Code/d_game.cpp"
            
#line 2236 "../Code/d_game.cpp"
			Vector2 ScreenWorld = PixelToWorld(AppData->WindowSize.x , AppData->WindowSize.y);
#line 2237 "../Code/d_game.cpp"
            
#line 2238 "../Code/d_game.cpp"
			glUniform2f(BlurShader->UniformLocation[SU_ScreenSize] , 1.0f / AppData->WindowSize.x , 1.0f / AppData->WindowSize.x);
#line 2239 "../Code/d_game.cpp"
			glUniform1f(BlurShader->UniformLocation[SU_GridSize] ,11);
#line 2240 "../Code/d_game.cpp"
			glUniform1f(BlurShader->UniformLocation[SU_Spread] , 10);
#line 2241 "../Code/d_game.cpp"
            
#line 2242 "../Code/d_game.cpp"
			//i just reusing data already exist in buffer
#line 2243 "../Code/d_game.cpp"
			Update_GPU_VertexBufferAll(BlurShader , 6);    
#line 2244 "../Code/d_game.cpp"
			glDrawArrays(GL_TRIANGLES, 0, 6);
#line 2245 "../Code/d_game.cpp"
            
#line 2246 "../Code/d_game.cpp"
			rlDisableFramebuffer();
#line 2247 "../Code/d_game.cpp"
		}
#line 2248 "../Code/d_game.cpp"
        
#line 2249 "../Code/d_game.cpp"
		glClearColor(0, 0, 0, 1);
#line 2250 "../Code/d_game.cpp"
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#line 2251 "../Code/d_game.cpp"
        
#line 2252 "../Code/d_game.cpp"
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_DST_ALPHA, GL_ONE, GL_ONE);
#line 2253 "../Code/d_game.cpp"
        
#line 2254 "../Code/d_game.cpp"
		DrawScreenFlat(RenderState.LightingTexture ,Fade(WHITE,1.0f));
#line 2255 "../Code/d_game.cpp"
		DrawScreenFlat(RenderState.BlurTexture ,Fade(WHITE,0.4f));
#line 2256 "../Code/d_game.cpp"
        
#line 2257 "../Code/d_game.cpp"
		GL_CATCH;
#line 2258 "../Code/d_game.cpp"
        
#line 2259 "../Code/d_game.cpp"
	}
#line 2260 "../Code/d_game.cpp"
#endif
#line 2261 "../Code/d_game.cpp"
    
#line 2262 "../Code/d_game.cpp"
	ChangeMatrix(ScreenMVP);
#line 2263 "../Code/d_game.cpp"
	glBlendFunc(GL_ONE , GL_ONE_MINUS_SRC_ALPHA);
#line 2264 "../Code/d_game.cpp"
	glDisable(GL_DEPTH_TEST);
#line 2265 "../Code/d_game.cpp"
    
#line 2266 "../Code/d_game.cpp"
	//i just reusing texture
#line 2267 "../Code/d_game.cpp"
	//maybe i should reuse more?
#line 2268 "../Code/d_game.cpp"
	DrawScreenFlat(RenderState.GUITexture , WHITE);
#line 2269 "../Code/d_game.cpp"
    
#line 2270 "../Code/d_game.cpp"
    
#line 2271 "../Code/d_game.cpp"
	D_GameDraw();
#line 2272 "../Code/d_game.cpp"
    
#line 2273 "../Code/d_game.cpp"
	RenderState.ElapsedTime += DeltaTime;
#line 2274 "../Code/d_game.cpp"
    
#line 2275 "../Code/d_game.cpp"
}
#line 2276 "../Code/d_game.cpp"

#line 2277 "../Code/d_game.cpp"
internal GAME_LOOP(GameLoop)
#line 2278 "../Code/d_game.cpp"
{
#line 2279 "../Code/d_game.cpp"
    if (ShaderCompileFailed) return;
#line 2280 "../Code/d_game.cpp"
    
#line 2281 "../Code/d_game.cpp"
    localPersist double UpdateTimer = 0;
#line 2282 "../Code/d_game.cpp"
    localPersist double PreviousLoopTime = TimeStamp();
#line 2283 "../Code/d_game.cpp"
    localPersist double ElapsedTime = 0;
#line 2284 "../Code/d_game.cpp"
    
#line 2285 "../Code/d_game.cpp"
    double CurrentTime = TimeStamp();
#line 2286 "../Code/d_game.cpp"
    ElapsedTime += (CurrentTime - PreviousLoopTime) / (1000.0 * 1000.0);
#line 2287 "../Code/d_game.cpp"
    
#line 2288 "../Code/d_game.cpp"
    double UpdateTime = 1.0f / (double)D_UPDATE_RATE;
#line 2289 "../Code/d_game.cpp"
    
#line 2290 "../Code/d_game.cpp"
    if (ElapsedTime > UpdateTime * 10) ElapsedTime = 0;
#line 2291 "../Code/d_game.cpp"
    
#line 2292 "../Code/d_game.cpp"
    int UpdateCount = 0;
#line 2293 "../Code/d_game.cpp"
    
#line 2294 "../Code/d_game.cpp"
    for (;ElapsedTime > UpdateTime ; ElapsedTime -= UpdateTime )
#line 2295 "../Code/d_game.cpp"
    {   
#line 2296 "../Code/d_game.cpp"
        glfwPollEvents();
#line 2297 "../Code/d_game.cpp"
        
#line 2298 "../Code/d_game.cpp"
        double GameUpdateTime = TimeStamp();
#line 2299 "../Code/d_game.cpp"
        GameUpdate();
#line 2300 "../Code/d_game.cpp"
        GameUpdateCount++;
#line 2301 "../Code/d_game.cpp"
        
#line 2302 "../Code/d_game.cpp"
        double GameUpateTimeDelta = (TimeStamp()-GameUpdateTime) / (1000.0 * 1000.0);
#line 2303 "../Code/d_game.cpp"
        
#line 2304 "../Code/d_game.cpp"
        if (GameUpateTimeDelta > UpdateTime)
#line 2305 "../Code/d_game.cpp"
        {
#line 2306 "../Code/d_game.cpp"
            printf( "update is lagging : %f \n", GameUpateTimeDelta );
#line 2307 "../Code/d_game.cpp"
        }
#line 2308 "../Code/d_game.cpp"
        
#line 2309 "../Code/d_game.cpp"
        AppData->FrameTimeMemory.CurrentMemory = AppData->FrameTimeMemory.StartMemory;
#line 2310 "../Code/d_game.cpp"
        glfwSwapBuffers(AppData->CurrentWindow);
#line 2311 "../Code/d_game.cpp"
        
#line 2312 "../Code/d_game.cpp"
    }
#line 2313 "../Code/d_game.cpp"
    
#line 2314 "../Code/d_game.cpp"
    PreviousLoopTime = CurrentTime;
#line 2315 "../Code/d_game.cpp"
    
#line 2316 "../Code/d_game.cpp"
    
#line 2317 "../Code/d_game.cpp"
    //these things doesn't help
#line 2318 "../Code/d_game.cpp"
    //it still lock on 30 fps
#line 2319 "../Code/d_game.cpp"
    
#line 2320 "../Code/d_game.cpp"
    //glFinish();
#line 2321 "../Code/d_game.cpp"
    
#line 2322 "../Code/d_game.cpp"
    //this things freeze opengl ?
#line 2323 "../Code/d_game.cpp"
    //SwapBuffers(AppData->_HDC);
#line 2324 "../Code/d_game.cpp"
}
#line 2325 "../Code/d_game.cpp"

#line 2326 "../Code/d_game.cpp"
internal GAME_UNLOAD(GameUnload)
#line 2327 "../Code/d_game.cpp"
{
#line 2328 "../Code/d_game.cpp"
    mkdir(GetAppFilePath("Game"));
#line 2329 "../Code/d_game.cpp"
    
#line 2330 "../Code/d_game.cpp"
    glClearColor(0, 0, 0, 1);
#line 2331 "../Code/d_game.cpp"
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#line 2332 "../Code/d_game.cpp"
    
#line 2333 "../Code/d_game.cpp"
    glfwSwapBuffers(AppData->CurrentWindow);
#line 2334 "../Code/d_game.cpp"
    //SwapBuffers(AppData->_HDC);
#line 2335 "../Code/d_game.cpp"
	
#line 2336 "../Code/d_game.cpp"
    if (!ShaderCompileFailed)
#line 2337 "../Code/d_game.cpp"
    {
#line 2338 "../Code/d_game.cpp"
        SaveData(GetAppFilePath(GameStateSaveName),&AppData->GameEditorData);
#line 2339 "../Code/d_game.cpp"
        
#line 2340 "../Code/d_game.cpp"
    }
#line 2341 "../Code/d_game.cpp"
    
#line 2342 "../Code/d_game.cpp"
    for (int TextureIndex = 0; TextureIndex < RenderState.TextureCount; TextureIndex++)
#line 2343 "../Code/d_game.cpp"
    {
#line 2344 "../Code/d_game.cpp"
        if (RenderState.AllTextures[TextureIndex].id != 0)
#line 2345 "../Code/d_game.cpp"
        {
#line 2346 "../Code/d_game.cpp"
            UnloadTexture(RenderState.AllTextures[TextureIndex]);
#line 2347 "../Code/d_game.cpp"
        }
#line 2348 "../Code/d_game.cpp"
    }
#line 2349 "../Code/d_game.cpp"
    
#line 2350 "../Code/d_game.cpp"
    if (RenderState.ScreenFrameBuffer != 0)
#line 2351 "../Code/d_game.cpp"
    {
#line 2352 "../Code/d_game.cpp"
        rlUnloadFramebuffer(RenderState.ScreenFrameBuffer); //do i need to delete you?
#line 2353 "../Code/d_game.cpp"
        
#line 2354 "../Code/d_game.cpp"
        rlUnloadTexture(RenderState.GameWorldTexture);
#line 2355 "../Code/d_game.cpp"
		rlUnloadTexture(RenderState.GUITexture);
#line 2356 "../Code/d_game.cpp"
		rlUnloadTexture(RenderState.GameWorldDepthTexture);
#line 2357 "../Code/d_game.cpp"
        rlUnloadTexture(RenderState.LightingTexture);
#line 2358 "../Code/d_game.cpp"
        rlUnloadTexture(RenderState.BloomTexture);
#line 2359 "../Code/d_game.cpp"
        rlUnloadTexture(RenderState.FirstBlurTexture);
#line 2360 "../Code/d_game.cpp"
        rlUnloadTexture(RenderState.BlurTexture);
#line 2361 "../Code/d_game.cpp"
    }
#line 2362 "../Code/d_game.cpp"
    
#line 2363 "../Code/d_game.cpp"
    glDeleteTextures(1, &AppData->DefaultFont.GlyphAtlasTexture.id);
#line 2364 "../Code/d_game.cpp"
    
#line 2365 "../Code/d_game.cpp"
    for (int BufferIndex = 0; BufferIndex < ShaderBufferCount; BufferIndex++)
#line 2366 "../Code/d_game.cpp"
    {
#line 2367 "../Code/d_game.cpp"
        glDeleteBuffers(1, &All_GPU_Buffers[BufferIndex]->BufferHandle);
#line 2368 "../Code/d_game.cpp"
    }
#line 2369 "../Code/d_game.cpp"
    
#line 2370 "../Code/d_game.cpp"
    for (int ShaderIndex = 0; ShaderIndex < S_COUNT; ShaderIndex++)
#line 2371 "../Code/d_game.cpp"
    {
#line 2372 "../Code/d_game.cpp"
        glDeleteProgram(AllShaderInputs[ShaderIndex].Shader);
#line 2373 "../Code/d_game.cpp"
    }
#line 2374 "../Code/d_game.cpp"
    
#line 2375 "../Code/d_game.cpp"
    free(AppData->FrameTimeMemory.StartMemory);
#line 2376 "../Code/d_game.cpp"
    free(AppData->RunTimeMemory.StartMemory);
#line 2377 "../Code/d_game.cpp"
    free(PersistMemoryBlock);
#line 2378 "../Code/d_game.cpp"
    
#line 2379 "../Code/d_game.cpp"
}
#line 2380 "../Code/d_game.cpp"

#line 2381 "../Code/d_game.cpp"
internal void LoadTextureFolder()
#line 2382 "../Code/d_game.cpp"
{
#line 2383 "../Code/d_game.cpp"
    
#line 2384 "../Code/d_game.cpp"
    FILE* GameTextureDataFile = fopen(GetAppFilePath(GameTextureDataName), "rb");
#line 2385 "../Code/d_game.cpp"
	
#line 2386 "../Code/d_game.cpp"
    bool TryLoadPreviousTextureIndex = false;
#line 2387 "../Code/d_game.cpp"
    RenderState.TextureCount = 2;
#line 2388 "../Code/d_game.cpp"
    RenderState.AllTexturesName = AllocateTemp(String, MAX_TEXTURE);
#line 2389 "../Code/d_game.cpp"
    RenderState.AllTextures = AllocateTemp(Texture, MAX_TEXTURE);
#line 2390 "../Code/d_game.cpp"
    
#line 2391 "../Code/d_game.cpp"
    if (GameTextureDataFile)
#line 2392 "../Code/d_game.cpp"
    {
#line 2393 "../Code/d_game.cpp"
        fread(&RenderState.TextureCount, sizeof(int), 1, GameTextureDataFile);
#line 2394 "../Code/d_game.cpp"
        
#line 2395 "../Code/d_game.cpp"
        for (int TextureIndex = 0; TextureIndex < RenderState.TextureCount; TextureIndex++)
#line 2396 "../Code/d_game.cpp"
        {
#line 2397 "../Code/d_game.cpp"
            fread(&RenderState.AllTexturesName[TextureIndex].Count, sizeof(int), 1, GameTextureDataFile);
#line 2398 "../Code/d_game.cpp"
            RenderState.AllTexturesName[TextureIndex].Start = AllocateTemp(wchar_t, RenderState.AllTexturesName[TextureIndex].Count);
#line 2399 "../Code/d_game.cpp"
            fread(RenderState.AllTexturesName[TextureIndex].Start , sizeof(wchar_t), RenderState.AllTexturesName[TextureIndex].Count, GameTextureDataFile);
#line 2400 "../Code/d_game.cpp"
        }
#line 2401 "../Code/d_game.cpp"
        
#line 2402 "../Code/d_game.cpp"
        fclose(GameTextureDataFile);
#line 2403 "../Code/d_game.cpp"
    }
#line 2404 "../Code/d_game.cpp"
    
#line 2405 "../Code/d_game.cpp"
    Image DefaultWhiteImage = GenImageColor(20, 20, WHITE);
#line 2406 "../Code/d_game.cpp"
    RenderState.AllTextures[0] = LoadTextureFromImage(DefaultWhiteImage);
#line 2407 "../Code/d_game.cpp"
    RenderState.AllTexturesName[0].Start = L"無貼圖";
#line 2408 "../Code/d_game.cpp"
    
#line 2409 "../Code/d_game.cpp"
    Image MissingTexture = GenImageCellular(20, 20, 2);
#line 2410 "../Code/d_game.cpp"
    RenderState.AllTextures[1] = LoadTextureFromImage(MissingTexture);
#line 2411 "../Code/d_game.cpp"
    RenderState.AllTexturesName[1].Start = L"";
#line 2412 "../Code/d_game.cpp"
    RenderState.AllTexturesName[1].Start = L"ERROR_TEXTURE";
#line 2413 "../Code/d_game.cpp"
    
#line 2414 "../Code/d_game.cpp"
#define TEXTURE_META_HASH 16
#line 2415 "../Code/d_game.cpp"
    TextureMetaNode * TextureMetaNodeHash[TEXTURE_META_HASH] = {};
#line 2416 "../Code/d_game.cpp"
    
#line 2417 "../Code/d_game.cpp"
    for (int TextureIndex = 0; TextureIndex < RenderState.TextureCount; TextureIndex++)
#line 2418 "../Code/d_game.cpp"
    {
#line 2419 "../Code/d_game.cpp"
        TextureMetaNode ** NodePointer = TextureMetaNodeHash + (StringToHash(RenderState.AllTexturesName[TextureIndex].Start) % TEXTURE_META_HASH);
#line 2420 "../Code/d_game.cpp"
        
#line 2421 "../Code/d_game.cpp"
        for (; (*NodePointer); NodePointer = &(*NodePointer)->Next);
#line 2422 "../Code/d_game.cpp"
        
#line 2423 "../Code/d_game.cpp"
        TextureMetaNode* NewNode = AllocateTemp(TextureMetaNode, 1);
#line 2424 "../Code/d_game.cpp"
        (*NodePointer) = NewNode;
#line 2425 "../Code/d_game.cpp"
        
#line 2426 "../Code/d_game.cpp"
        NewNode->Index = TextureIndex;
#line 2427 "../Code/d_game.cpp"
    }
#line 2428 "../Code/d_game.cpp"
    
#line 2429 "../Code/d_game.cpp"
    FilePathList TexturePathList = LoadDirectoryFilesEx(GetAppFilePath("Asset\\Texture"), ".png", false);
#line 2430 "../Code/d_game.cpp"
	
#line 2431 "../Code/d_game.cpp"
    for (int PathIndex = 0; PathIndex < TexturePathList.count; PathIndex++)
#line 2432 "../Code/d_game.cpp"
    {
#line 2433 "../Code/d_game.cpp"
        char * TexturePath = TexturePathList.paths[PathIndex];
#line 2434 "../Code/d_game.cpp"
        const char * FileName = GetFileName(TexturePath);
#line 2435 "../Code/d_game.cpp"
        
#line 2436 "../Code/d_game.cpp"
        wchar_t FileNameString[256] = {};
#line 2437 "../Code/d_game.cpp"
        
#line 2438 "../Code/d_game.cpp"
        //can't convert anything beside english
#line 2439 "../Code/d_game.cpp"
        for (int CharIndex = 0; FileName[CharIndex]; CharIndex++) FileNameString[CharIndex] = (int)FileName[CharIndex];
#line 2440 "../Code/d_game.cpp"
        
#line 2441 "../Code/d_game.cpp"
        TextureMetaNode ** NodePointer = TextureMetaNodeHash + (StringToHash(FileNameString) % TEXTURE_META_HASH);
#line 2442 "../Code/d_game.cpp"
        
#line 2443 "../Code/d_game.cpp"
        for (; (*NodePointer) && (wcscmp(RenderState.AllTexturesName[(*NodePointer)->Index].Start , FileNameString)); NodePointer = &(*NodePointer)->Next);
#line 2444 "../Code/d_game.cpp"
        
#line 2445 "../Code/d_game.cpp"
        if (*NodePointer)
#line 2446 "../Code/d_game.cpp"
        {
#line 2447 "../Code/d_game.cpp"
            int TextureIndex = (*NodePointer)->Index;
#line 2448 "../Code/d_game.cpp"
            RenderState.AllTextures[TextureIndex] = LoadTexture(TexturePath);
#line 2449 "../Code/d_game.cpp"
        }
#line 2450 "../Code/d_game.cpp"
        else
#line 2451 "../Code/d_game.cpp"
        {
#line 2452 "../Code/d_game.cpp"
            RenderState.AllTextures[RenderState.TextureCount] = LoadTexture(TexturePath);
#line 2453 "../Code/d_game.cpp"
            
#line 2454 "../Code/d_game.cpp"
            int CharIndex = 0;
#line 2455 "../Code/d_game.cpp"
            for (; FileNameString[CharIndex]; CharIndex++);
#line 2456 "../Code/d_game.cpp"
            
#line 2457 "../Code/d_game.cpp"
            RenderState.AllTexturesName[RenderState.TextureCount].Count = CharIndex + 1;
#line 2458 "../Code/d_game.cpp"
            RenderState.AllTexturesName[RenderState.TextureCount].Start = AllocateTemp(wchar_t, CharIndex + 2);
#line 2459 "../Code/d_game.cpp"
            
#line 2460 "../Code/d_game.cpp"
            for (int CharIndex = 0; FileNameString[CharIndex]; CharIndex++)
#line 2461 "../Code/d_game.cpp"
                RenderState.AllTexturesName[RenderState.TextureCount].Start[CharIndex] = FileNameString[CharIndex];
#line 2462 "../Code/d_game.cpp"
            
#line 2463 "../Code/d_game.cpp"
            RenderState.TextureCount++;
#line 2464 "../Code/d_game.cpp"
        }
#line 2465 "../Code/d_game.cpp"
    }
#line 2466 "../Code/d_game.cpp"
    
#line 2467 "../Code/d_game.cpp"
    UnloadDirectoryFiles(TexturePathList);
#line 2468 "../Code/d_game.cpp"
}
#line 2469 "../Code/d_game.cpp"

#line 2470 "../Code/d_game.cpp"
internal void MakeAStickMan()
#line 2471 "../Code/d_game.cpp"
{
#line 2472 "../Code/d_game.cpp"
    printf("\n");
#line 2473 "../Code/d_game.cpp"
    
#line 2474 "../Code/d_game.cpp"
    //TODO : patch enum?
#line 2475 "../Code/d_game.cpp"
    //thing is messy
#line 2476 "../Code/d_game.cpp"
    //it seem the data moved to the wrong place
#line 2477 "../Code/d_game.cpp"
    //maybe it isn't ? just the pointer being written over i think
#line 2478 "../Code/d_game.cpp"
    
#line 2479 "../Code/d_game.cpp"
    for(;;)
#line 2480 "../Code/d_game.cpp"
    {
#line 2481 "../Code/d_game.cpp"
        bool AllMatch = true;
#line 2482 "../Code/d_game.cpp"
        printf("\n");
#line 2483 "../Code/d_game.cpp"
        
#line 2484 "../Code/d_game.cpp"
        for(int BoneIndex = 0 ; BoneIndex < BoneCount ; BoneIndex++)
#line 2485 "../Code/d_game.cpp"
        {
#line 2486 "../Code/d_game.cpp"
            char** BoneNames = (char **)EnumToString(BoneTag);
#line 2487 "../Code/d_game.cpp"
            
#line 2488 "../Code/d_game.cpp"
            char * OldBoneName = Editor->StickMan[BoneIndex]->BoneName;
#line 2489 "../Code/d_game.cpp"
            char * CurrentBoneName = BoneNames[BoneIndex];
#line 2490 "../Code/d_game.cpp"
            
#line 2491 "../Code/d_game.cpp"
            if(strcmp(OldBoneName , CurrentBoneName))
#line 2492 "../Code/d_game.cpp"
            {
#line 2493 "../Code/d_game.cpp"
                bool MissingBone = true;
#line 2494 "../Code/d_game.cpp"
                
#line 2495 "../Code/d_game.cpp"
                for(int OldBoneIndex = 0 ; OldBoneIndex < BoneCount ; OldBoneIndex++)
#line 2496 "../Code/d_game.cpp"
                {
#line 2497 "../Code/d_game.cpp"
                    if(!strcmp(OldBoneName , BoneNames[OldBoneIndex]))
#line 2498 "../Code/d_game.cpp"
                    {
#line 2499 "../Code/d_game.cpp"
                        AllMatch = false;
#line 2500 "../Code/d_game.cpp"
                        
#line 2501 "../Code/d_game.cpp"
                        printf("Bone swaped : %s <-> %s\n" , OldBoneName , CurrentBoneName);
#line 2502 "../Code/d_game.cpp"
                        
#line 2503 "../Code/d_game.cpp"
                        _Bone persist * TempBone = Editor->StickMan[OldBoneIndex];
#line 2504 "../Code/d_game.cpp"
                        
#line 2505 "../Code/d_game.cpp"
                        Editor->StickMan[OldBoneIndex] = Editor->StickMan[BoneIndex];
#line 2506 "../Code/d_game.cpp"
                        Editor->StickMan[BoneIndex] = TempBone;
#line 2507 "../Code/d_game.cpp"
                        
#line 2508 "../Code/d_game.cpp"
                        Editor->StickMan[BoneIndex]->BoneIndex = BoneIndex;
#line 2509 "../Code/d_game.cpp"
                        Editor->StickMan[OldBoneIndex]->BoneIndex = OldBoneIndex;
#line 2510 "../Code/d_game.cpp"
                        
#line 2511 "../Code/d_game.cpp"
                        MissingBone = false;
#line 2512 "../Code/d_game.cpp"
                        break;
#line 2513 "../Code/d_game.cpp"
                    }
#line 2514 "../Code/d_game.cpp"
                }
#line 2515 "../Code/d_game.cpp"
                
#line 2516 "../Code/d_game.cpp"
                if(MissingBone)
#line 2517 "../Code/d_game.cpp"
                {
#line 2518 "../Code/d_game.cpp"
                    printf("Bone missing : %s\n" , OldBoneName);
#line 2519 "../Code/d_game.cpp"
                }
#line 2520 "../Code/d_game.cpp"
            }
#line 2521 "../Code/d_game.cpp"
            
#line 2522 "../Code/d_game.cpp"
            sprintf(Editor->StickMan[BoneIndex]->BoneName , CurrentBoneName);
#line 2523 "../Code/d_game.cpp"
            
#line 2524 "../Code/d_game.cpp"
        }
#line 2525 "../Code/d_game.cpp"
        
#line 2526 "../Code/d_game.cpp"
        if(AllMatch) break;
#line 2527 "../Code/d_game.cpp"
        
#line 2528 "../Code/d_game.cpp"
    }
#line 2529 "../Code/d_game.cpp"
    
#line 2530 "../Code/d_game.cpp"
    for(int BoneIndex = 0 ; BoneIndex < BoneCount ; BoneIndex++)
#line 2531 "../Code/d_game.cpp"
    {
#line 2532 "../Code/d_game.cpp"
        char** BoneNames = (char **)EnumToString(BoneTag);
#line 2533 "../Code/d_game.cpp"
        
#line 2534 "../Code/d_game.cpp"
        char * OldBoneName = Editor->StickMan[BoneIndex]->BoneName;
#line 2535 "../Code/d_game.cpp"
        char * CurrentBoneName = BoneNames[BoneIndex];
#line 2536 "../Code/d_game.cpp"
        
#line 2537 "../Code/d_game.cpp"
        if(strcmp(OldBoneName , CurrentBoneName))
#line 2538 "../Code/d_game.cpp"
        {
#line 2539 "../Code/d_game.cpp"
            printf("something wrong\n");
#line 2540 "../Code/d_game.cpp"
        }
#line 2541 "../Code/d_game.cpp"
        
#line 2542 "../Code/d_game.cpp"
        if(BoneIndex != Editor->StickMan[BoneIndex]->BoneIndex)
#line 2543 "../Code/d_game.cpp"
        {
#line 2544 "../Code/d_game.cpp"
            Editor->StickMan[BoneIndex]->BoneIndex = BoneIndex;
#line 2545 "../Code/d_game.cpp"
            printf("wrong index \n");
#line 2546 "../Code/d_game.cpp"
        }
#line 2547 "../Code/d_game.cpp"
    }
#line 2548 "../Code/d_game.cpp"
    
#line 2549 "../Code/d_game.cpp"
    Editor->StickMan[B_Origin]->State.Direction = {0,0.0f,0};
#line 2550 "../Code/d_game.cpp"
    Editor->StickMan[B_UpperBody]->State.Direction = {0,0.6f,0};
#line 2551 "../Code/d_game.cpp"
    Editor->StickMan[B_LowerBody]->State.Direction = {0,0.3f,0};
#line 2552 "../Code/d_game.cpp"
    Editor->StickMan[B_Pelvic]->State.Direction = {0,0.5f,0};
#line 2553 "../Code/d_game.cpp"
    Editor->StickMan[B_Head]->State.Direction = {0,0.3,0};
#line 2554 "../Code/d_game.cpp"
    
#line 2555 "../Code/d_game.cpp"
    Editor->StickMan[B_Origin]->FreePosition = true;
#line 2556 "../Code/d_game.cpp"
    Editor->StickMan[B_BodyOrigin]->FreePosition = true;
#line 2557 "../Code/d_game.cpp"
    Editor->StickMan[B_LeftArmPoleTarget]->FreePosition = true;
#line 2558 "../Code/d_game.cpp"
    Editor->StickMan[B_LeftHandController]->FreePosition = true;
#line 2559 "../Code/d_game.cpp"
    Editor->StickMan[B_RightHandController]->FreePosition = true;
#line 2560 "../Code/d_game.cpp"
    Editor->StickMan[B_RightArmPoleTarget]->FreePosition = true;
#line 2561 "../Code/d_game.cpp"
    
#line 2562 "../Code/d_game.cpp"
    Editor->StickMan[B_LeftLegPoleTarget]->FreePosition = true;
#line 2563 "../Code/d_game.cpp"
    Editor->StickMan[B_LeftLegController]->FreePosition = true;
#line 2564 "../Code/d_game.cpp"
    Editor->StickMan[B_RightLegController]->FreePosition = true;
#line 2565 "../Code/d_game.cpp"
    Editor->StickMan[B_RightLegPoleTarget]->FreePosition = true;
#line 2566 "../Code/d_game.cpp"
    
#line 2567 "../Code/d_game.cpp"
    ClearList(&Editor->RootBoneList);
#line 2568 "../Code/d_game.cpp"
	AddToListTail(B_Origin , &Editor->RootBoneList , BoneCount);
#line 2569 "../Code/d_game.cpp"
    
#line 2570 "../Code/d_game.cpp"
    ClearHashTable(&Editor->BoneChildrenHashTable , BoneCount);
#line 2571 "../Code/d_game.cpp"
    ClearHashTable(&Editor->BoneParentHashTable, BoneCount);
#line 2572 "../Code/d_game.cpp"
    
#line 2573 "../Code/d_game.cpp"
	                                                                 
                               
     
                               
                                                                                              
                               
                                                                                                
                               
     ;
#line 2578 "../Code/d_game.cpp"
    
#line 2579 "../Code/d_game.cpp"
#if 1
#line 2580 "../Code/d_game.cpp"
	
#line 2581 "../Code/d_game.cpp"
    AttachChildBone(B_Origin ,B_LeftLegController);
#line 2582 "../Code/d_game.cpp"
    AttachChildBone(B_Origin ,B_LeftLegPoleTarget);
#line 2583 "../Code/d_game.cpp"
    
#line 2584 "../Code/d_game.cpp"
    AttachChildBone(B_Origin ,B_RightLegController);
#line 2585 "../Code/d_game.cpp"
    AttachChildBone(B_Origin ,B_RightLegPoleTarget);
#line 2586 "../Code/d_game.cpp"
    
#line 2587 "../Code/d_game.cpp"
    AttachChildBone(B_Origin ,B_LeftHandController);
#line 2588 "../Code/d_game.cpp"
    AttachChildBone(B_Origin ,B_LeftArmPoleTarget);
#line 2589 "../Code/d_game.cpp"
    
#line 2590 "../Code/d_game.cpp"
    AttachChildBone(B_Origin ,B_RightHandController);
#line 2591 "../Code/d_game.cpp"
    AttachChildBone(B_Origin ,B_RightArmPoleTarget);
#line 2592 "../Code/d_game.cpp"
    
#line 2593 "../Code/d_game.cpp"
    AttachChildBone(B_Origin ,B_BodyOrigin);
#line 2594 "../Code/d_game.cpp"
    
#line 2595 "../Code/d_game.cpp"
    AttachChildBone(B_UpperBody , B_RightUpperArm);
#line 2596 "../Code/d_game.cpp"
	AttachChildBone(B_UpperBody , B_LeftUpperArm);
#line 2597 "../Code/d_game.cpp"
	
#line 2598 "../Code/d_game.cpp"
	AttachChildBone(B_RightUpperArm , B_RightLowerArm);
#line 2599 "../Code/d_game.cpp"
	AttachChildBone(B_RightLowerArm , B_RightHand);
#line 2600 "../Code/d_game.cpp"
	
#line 2601 "../Code/d_game.cpp"
	AttachChildBone(B_LeftUpperArm , B_LeftLowerArm);
#line 2602 "../Code/d_game.cpp"
	AttachChildBone(B_LeftLowerArm , B_LeftHand);
#line 2603 "../Code/d_game.cpp"
	
#line 2604 "../Code/d_game.cpp"
	AttachChildBone(B_Pelvic , B_LeftUpperLeg);
#line 2605 "../Code/d_game.cpp"
	AttachChildBone(B_LeftUpperLeg , B_LeftLowerLeg);
#line 2606 "../Code/d_game.cpp"
	AttachChildBone(B_LeftLowerLeg , B_LeftFoot);
#line 2607 "../Code/d_game.cpp"
	
#line 2608 "../Code/d_game.cpp"
	AttachChildBone(B_Pelvic, B_RightUpperLeg);
#line 2609 "../Code/d_game.cpp"
	AttachChildBone(B_RightUpperLeg , B_RightLowerLeg);
#line 2610 "../Code/d_game.cpp"
	AttachChildBone(B_RightLowerLeg , B_RightFoot);
#line 2611 "../Code/d_game.cpp"
    
#line 2612 "../Code/d_game.cpp"
	AttachChildBone(B_BodyOrigin , B_LowerBody);
#line 2613 "../Code/d_game.cpp"
	AttachChildBone(B_BodyOrigin , B_Pelvic);
#line 2614 "../Code/d_game.cpp"
    
#line 2615 "../Code/d_game.cpp"
	AttachChildBone(B_LowerBody , B_UpperBody);
#line 2616 "../Code/d_game.cpp"
	AttachChildBone(B_UpperBody , B_Head);
#line 2617 "../Code/d_game.cpp"
#endif
#line 2618 "../Code/d_game.cpp"
}
#line 2619 "../Code/d_game.cpp"

#line 2620 "../Code/d_game.cpp"
internal void GameInit()
#line 2621 "../Code/d_game.cpp"
{
#line 2622 "../Code/d_game.cpp"
	AppData->GameCamera.target = {};
#line 2623 "../Code/d_game.cpp"
	AppData->GameCamera.position = { 0,0,-1 };
#line 2624 "../Code/d_game.cpp"
    
#line 2625 "../Code/d_game.cpp"
    timeBeginPeriod(1);
#line 2626 "../Code/d_game.cpp"
    glfwSwapInterval(0);
#line 2627 "../Code/d_game.cpp"
    
#line 2628 "../Code/d_game.cpp"
    CreateStructNameStringHash();
#line 2629 "../Code/d_game.cpp"
	
#line 2630 "../Code/d_game.cpp"
    CreateAWholeNewWorld();
#line 2631 "../Code/d_game.cpp"
	
#line 2632 "../Code/d_game.cpp"
    RenderStateInit();	
#line 2633 "../Code/d_game.cpp"
    ShaderInit();
#line 2634 "../Code/d_game.cpp"
    
#line 2635 "../Code/d_game.cpp"
    if (ShaderCompileFailed) return;
#line 2636 "../Code/d_game.cpp"
    
#line 2637 "../Code/d_game.cpp"
	double PatchTime = TimeStamp();
#line 2638 "../Code/d_game.cpp"
	printf("\npatch start\n");
#line 2639 "../Code/d_game.cpp"
    
#line 2640 "../Code/d_game.cpp"
    LoadData(GetAppFilePath(GameStateSaveName) , &AppData->GameEditorData);
#line 2641 "../Code/d_game.cpp"
    
#line 2642 "../Code/d_game.cpp"
    PatchTime = TimeStamp() - PatchTime;
#line 2643 "../Code/d_game.cpp"
	printf("patch finished (%f) \n\n" , PatchTime / (1000.0 * 1000.0));
#line 2644 "../Code/d_game.cpp"
    
#line 2645 "../Code/d_game.cpp"
    LoadTextureFolder();
#line 2646 "../Code/d_game.cpp"
    MakeAStickMan();
#line 2647 "../Code/d_game.cpp"
}
#line 2648 "../Code/d_game.cpp"

#line 2649 "../Code/d_game.cpp"
extern "C" GAME_LOAD(GameLoad)
#line 2650 "../Code/d_game.cpp"
{
#line 2651 "../Code/d_game.cpp"
    double GameLoadTime = TimeStamp();
#line 2652 "../Code/d_game.cpp"
    
#line 2653 "../Code/d_game.cpp"
    AppData = _AppData;
#line 2654 "../Code/d_game.cpp"
    AppData->_GameLoop = GameLoop;
#line 2655 "../Code/d_game.cpp"
    AppData->_GameUnload = GameUnload;
#line 2656 "../Code/d_game.cpp"
    
#line 2657 "../Code/d_game.cpp"
    AppData->FrameTimeMemory = {};
#line 2658 "../Code/d_game.cpp"
    AppData->FrameTimeMemory.Size = 1024 * 1024 * 128;
#line 2659 "../Code/d_game.cpp"
    AppData->FrameTimeMemory.StartMemory = (unsigned char*)malloc(AppData->FrameTimeMemory.Size);
#line 2660 "../Code/d_game.cpp"
    AppData->FrameTimeMemory.CurrentMemory = AppData->FrameTimeMemory.StartMemory;
#line 2661 "../Code/d_game.cpp"
    
#line 2662 "../Code/d_game.cpp"
    AppData->RunTimeMemory = {};
#line 2663 "../Code/d_game.cpp"
    AppData->RunTimeMemory.Size = 1024 * 1024 * 128;
#line 2664 "../Code/d_game.cpp"
    AppData->RunTimeMemory.StartMemory = (unsigned char*)malloc(AppData->RunTimeMemory.Size);
#line 2665 "../Code/d_game.cpp"
    AppData->RunTimeMemory.CurrentMemory = AppData->RunTimeMemory.StartMemory;
#line 2666 "../Code/d_game.cpp"
    
#line 2667 "../Code/d_game.cpp"
    AppData->GameEditorData = {};
#line 2668 "../Code/d_game.cpp"
    AppData->GameEditorData.Size = 1024 * 1024 * 128;
#line 2669 "../Code/d_game.cpp"
    
#line 2670 "../Code/d_game.cpp"
    PersistMemoryBlock = (unsigned char*)malloc(AppData->GameEditorData.Size );
#line 2671 "../Code/d_game.cpp"
    
#line 2672 "../Code/d_game.cpp"
    _StartMemory_ = PersistMemoryBlock;
#line 2673 "../Code/d_game.cpp"
    
#line 2674 "../Code/d_game.cpp"
    AppData->GameEditorData.StartMemoryAddress = PersistMemoryBlock;
#line 2675 "../Code/d_game.cpp"
    AppData->GameEditorData.StartOffset = 0;
#line 2676 "../Code/d_game.cpp"
    AppData->GameEditorData.CurrentOffset = AppData->GameEditorData.StartOffset;
#line 2677 "../Code/d_game.cpp"
    
#line 2678 "../Code/d_game.cpp"
    GameInit();
#line 2679 "../Code/d_game.cpp"
    
#line 2680 "../Code/d_game.cpp"
    printf("\nGame Load Time: %f\n" , (TimeStamp() - GameLoadTime) / (1000.0 * 1000.0));
#line 2681 "../Code/d_game.cpp"
    
#line 2682 "../Code/d_game.cpp"
}