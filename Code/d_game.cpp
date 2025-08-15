
//this is dumb
#define FILE_TO_STRING(Str) #Str
#define _GENERATED_FILE(FileName , ExtensionName) FILE_TO_STRING(##FileName##ExtensionName)

#ifdef SWITCH_TO_GENERATED_FILE
#define _GENERATED_FILE(FileName , ExtensionName) FILE_TO_STRING(..\CodeGenerationbuild\\##FileName##_target_to_d_game_generated##ExtensionName)
#endif

#define GENERATED_FILE(FileName , ExtensionName) _GENERATED_FILE(FileName , ExtensionName)

#include GENERATED_FILE( d_header , .h)
#include GENERATED_FILE( d_main , .h)

#include GENERATED_FILE( d_renderdata, .cpp)
#include GENERATED_FILE( d_gamedata , .cpp)

#include "d_game_meta_generated.cpp"

#include "d_function.cpp"

#include "d_render.cpp"
#include "d_text.cpp"

#include "d_gamefunction.cpp"

internal bool CheckSelectedBoneRotation(int SingleBoneIndex)
{
    if(IsRig(SingleBoneIndex)) return false;
    
    _Bone * SelectedBone = Editor->StickMan[SingleBoneIndex];
    
    //RotateAxisPosition = Vector3Subtract(RotateAxisPosition , Vector3RotateByQuaternion(SelectedBone->State.Direction , SelectedBone->Rotation));
    
    _Rect RotationRect[R_Count] = {};
    RayCollision RotationRectCollision[R_Count] = {};
    
    RotationRect[R_Z] = GetRect();
    RotationRect[R_Z].Size = { 1, 1};
    RotationRect[R_Z].Position = SelectedBone->Position;
    RotationRect[R_Z].Rotation = SelectedBone->Rotation;
    
    RotationRect[R_Y] = RotationRect[R_Z];
    RotationRect[R_Y].Rotation = QuaternionMultiply(RotationRect[R_Z].Rotation , QuaternionFromEuler(DEG2RAD * 90,0,0));
    
    RotationRect[R_X] = RotationRect[R_Z];
    RotationRect[R_X].Rotation = QuaternionMultiply(RotationRect[R_Z].Rotation , QuaternionFromEuler(0,DEG2RAD * 90,0));
    
    bool Hovering = false;
    for(int AxisIndex = R_Z ; AxisIndex < R_Count ; AxisIndex++)
    {
        RotationRectCollision[AxisIndex] = GetCollisionRect3D(RotationRect[AxisIndex]);
        if(RotationRectCollision[AxisIndex].hit)
        {
            Hovering = true;
        }
    }
    
    localPersist Vector3 PreviousDragPoint = {};
    localPersist int DraggingAxis = R_None;
    
    if(MouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        DraggingAxis = R_None;
    }
    
    RayCollision ClosestCollision = {};
    ClosestCollision.distance = FLT_MAX;
    
    Color AxisColor = {};
    if(DraggingAxis == R_Z) AxisColor = BLUE;
    if(DraggingAxis == R_Y) AxisColor = GREEN;
    if(DraggingAxis == R_X) AxisColor = RED;
    
    DrawRectLine(RotationRect[DraggingAxis] , AxisColor , 20);
    
    for(int AxisIndex = R_Z ; AxisIndex < R_Count ; AxisIndex++)
    {
        bool FoundClosestAxis = true;
        
        if(!RotationRectCollision[AxisIndex].hit) FoundClosestAxis = false;
        if(RotationRectCollision[AxisIndex].distance > ClosestCollision.distance) FoundClosestAxis = false;
        
        if(FoundClosestAxis)
        {
            
            if(MouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                PreviousDragPoint = RotationRectCollision[AxisIndex].point;
                DraggingAxis = AxisIndex;
            }
            
            ClosestCollision = RotationRectCollision[AxisIndex];
        }
    }
    
    if(!MouseButtonPressing(MOUSE_BUTTON_LEFT))
    {
        for(int AxisIndex = R_Z ; AxisIndex < R_Count ; AxisIndex++)
        {
            DrawRectLine(RotationRect[AxisIndex] , Fade(WHITE , 0.2 ) , 20);
        }
        
        DraggingAxis = R_None;
        
    }
    else
    {
        if (SelectedBone->FreeBone)
        {
            
            _Quad BoneCentreQuad = GetBillboardQuad(SelectedBone->Position, 100000.0f, 100000.0f);
            Vector3 HitPoint = GetCollisionQuad3D(BoneCentreQuad).point;
            
            _Rect AxisRect = RotationRect[DraggingAxis];
            
            AxisRect.Size = {10000.0,10000.0};
            
            HitPoint = GetCollisionRect3D(AxisRect).point;
            
            Vector3 BonePosition = AxisRect.Position;
            
            Quaternion RotationOffset = QuaternionIdentity();
            
            Quaternion InvertAxisRotation = QuaternionInvert(AxisRect.Rotation);
            
            Vector3 DirectionToPreviousPoint = Vector3Subtract(PreviousDragPoint , BonePosition);
            DirectionToPreviousPoint = Vector3RotateByQuaternion(DirectionToPreviousPoint , InvertAxisRotation);
            
            Vector3 DirectionToPoint = Vector3Subtract(HitPoint , BonePosition);
            DirectionToPoint = Vector3RotateByQuaternion(DirectionToPoint , InvertAxisRotation);
            
            float PreviousAngle = atan2f(DirectionToPreviousPoint.y , DirectionToPreviousPoint.x);
            float CurrentAngle = atan2f(DirectionToPoint.y , DirectionToPoint.x);
            
            Vector3 Axis = {};
            
            if (DraggingAxis == R_Z) Axis = {0,0,1};
            if (DraggingAxis == R_Y) Axis = {0,-1,0};
            if (DraggingAxis == R_X) Axis = {1,0,0};
            
            RotationOffset = QuaternionFromAxisAngle(Axis, CurrentAngle - PreviousAngle);
            
            SelectedBone->State.LocalRotation = QuaternionMultiply(SelectedBone->State.LocalRotation, RotationOffset);
            //SelectedBone->State.LocalRotation = QuaternionNormalize(SelectedBone->State.LocalRotation);
            //SelectedBone->State.LocalRotation = QuaternionIdentity();
            
            //printf("Hit point : %f %f %f\n" ,HitPoint.x , HitPoint.y , HitPoint.z);
            
            PreviousDragPoint = HitPoint;
        }
    }
    
    return Hovering;
}

internal void BoneSelection()
{
    localPersist bool RedragSelection = true;
    localPersist bool DraggingSelection = false;
    
    internal MOUSE_BUTTON_CHECK(SelectionCheck)
    {
        if(MenuInteruped) return false;
        if(Editor->SelectingBezier) return false;
        
        return true;
    }
    
    MouseButtonBlocker = SelectionCheck;
    
    if(MouseButtonPressedWithCheck(MOUSE_BUTTON_LEFT))
    {
        RedragSelection = true;
    }
    
    //CurveVisualization
    Editor->SelectingBezier = false;
    
    localPersist bool ModifyingPosition = false;
    localPersist bool SelectedPreviousBezierPoint = false;
    localPersist int SelectedBezierKeyFrameIndex = 0;
    localPersist int SelectedBezierOtherKeyFrameIndex= 0;
    localPersist _Bone * SelectedBezierBone = 0;
    
    for(int StackIndex = 0 ; StackIndex < Editor->SelectedBoneCount ; StackIndex++)
    {
        
        int SelectedBoneIndex = Editor->SelectedBoneStack[StackIndex];
        _Bone * SelectedBone = Editor->StickMan[SelectedBoneIndex];
        _KeyFrame * KeyFrameArray = (_KeyFrame * )SelectedBone->KeyFrameArray;
        
        //TODO : i have no idea where this offset is coming from
        //too bad
        
        //the offset was from the direction of the origin
        //controllers still geting offset from it
        
        //Vector3 ParentOffset = SelectedBone->Position;
        //ParentOffset = Vector3Subtract(ParentOffset , SelectedBone->State.LocalPosition);
        //ParentOffset = {};
        
        int PreviousKeyFrameIndex = GetPreviousDataIndexFromNodeIndex(N_NodeTail , &SelectedBone->KeyFrameOrderList);
        _KeyFrame * PreviousKeyFrame = KeyFrameArray + PreviousKeyFrameIndex;
        
        Color KeyFrameLineColor = Fade(WHITE , 0.2f);
        
        List_Foreach( KeyFrameIndex , &SelectedBone->KeyFrameOrderList)
        {
            
            _KeyFrame * KeyFrame = KeyFrameArray + KeyFrameIndex;
            Vector3 PreviousKeyFramePosition = PreviousKeyFrame->BoneState.LocalPosition;
            Vector3 KeyFramePosition = KeyFrame->BoneState.LocalPosition;
            
            Vector3 LineDirection = Vector3Subtract(KeyFramePosition , PreviousKeyFramePosition);
            LineDirection = Vector3Scale(Vector3Normalize(LineDirection),0.1f);
            
            Vector3 PreviousKeyFrameBezierPoint = Vector3Add(LineDirection , PreviousKeyFrame->NextBezierOffset);
            PreviousKeyFrameBezierPoint = Vector3Add(PreviousKeyFrameBezierPoint , PreviousKeyFramePosition);
            
            Vector3 KeyFrameBezierPoint = Vector3Add(Vector3Negate(LineDirection) , KeyFrame->PreviousBezierOffset);
            KeyFrameBezierPoint = Vector3Add(KeyFrameBezierPoint , KeyFramePosition);
            
            DrawRoundLine(PreviousKeyFramePosition , PreviousKeyFrameBezierPoint , 10 , Fade(BLUE,0.5f) );
            DrawRoundLine(KeyFramePosition , KeyFrameBezierPoint , 10 , Fade(RED,0.5f) );
            
            _Rect PreviousBezierPointRect = GetBillboardRect(PreviousKeyFrameBezierPoint , 0.1);
            _Rect BezierPointRect = GetBillboardRect(KeyFrameBezierPoint , 0.1);
            
            Color PreviousBezierPointColor = Fade(BLUE , 0.5f);
            Color BezierPointColor = Fade(RED , 0.5f);
            Color KeyFramePointColor = Fade(YELLOW , 0.5f);
            
            if(CheckCollisionRect3D(PreviousBezierPointRect))
            {
                PreviousBezierPointColor = BLUE;
                if(MouseButtonPressedWithCheck(MOUSE_BUTTON_LEFT))
                {
                    SelectedBezierKeyFrameIndex = PreviousKeyFrameIndex;
                    SelectedBezierOtherKeyFrameIndex = KeyFrameIndex;
                    SelectedBezierBone = SelectedBone;
                    SelectedPreviousBezierPoint = false;
                    
                }
                
            }
            
            if(CheckCollisionRect3D(BezierPointRect))
            {
                BezierPointColor = RED;
                if(MouseButtonPressedWithCheck(MOUSE_BUTTON_LEFT))
                {
                    
                    SelectedBezierKeyFrameIndex = KeyFrameIndex;
                    SelectedBezierOtherKeyFrameIndex = PreviousKeyFrameIndex;
                    SelectedBezierBone = SelectedBone;
                    SelectedPreviousBezierPoint = true;
                    
                }
            }
            
            DrawCircle( PreviousBezierPointRect , PreviousBezierPointColor);
            DrawCircle( BezierPointRect , BezierPointColor);
            
            int FrameCount = GetKeyFrameCount(PreviousKeyFrame , KeyFrame);
            
            for(int FrameIndex = 0 ; FrameIndex < FrameCount ; FrameIndex++)
            {
                float BezierStartTime = ((float)FrameIndex) / ((float)FrameCount);
                float BezierEndTime = ((float)(FrameIndex+1)) / ((float)FrameCount);
                
                Vector3 BezierStartPoint = Vector3CubicBezier(PreviousKeyFramePosition , PreviousKeyFrameBezierPoint , KeyFrameBezierPoint, KeyFramePosition  , BezierStartTime);
                Vector3 BezierEndPoint = Vector3CubicBezier(PreviousKeyFramePosition , PreviousKeyFrameBezierPoint , KeyFrameBezierPoint, KeyFramePosition  , BezierEndTime);
                
                DrawRoundLine(BezierStartPoint , BezierEndPoint ,10 ,KeyFrameLineColor );
            }
            
            DrawRoundLine(PreviousKeyFramePosition , KeyFramePosition,10 ,Fade(WHITE , 0.05f));
            
            _Rect KeyFrameRect = GetBillboardRect(KeyFramePosition , 0.1 );
            
            if(CheckCollisionRect3D(KeyFrameRect))
            {
                KeyFramePointColor = YELLOW;
                
                if(MouseButtonPressedWithCheck(MOUSE_BUTTON_LEFT))
                {
                    ModifyingPosition = true;
                    SelectedBezierBone = SelectedBone;
                    SelectedBezierKeyFrameIndex = KeyFrameIndex;
                }
            }
            
            DrawCircle(KeyFrameRect , KeyFramePointColor);
            
            PreviousKeyFrameIndex = KeyFrameIndex;
            PreviousKeyFrame = KeyFrame;
        }
        
        if(IsRig(SelectedBoneIndex))
        {
            DrawBillboardCircle(SelectedBone->Position , 0.1 , Fade(WHITE,0.2));
        }
        
    }
    
    if(MouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        ModifyingPosition = false;
        SelectedBezierBone = 0;
    }
    
    if(SelectedBezierBone)
    {
        localPersist Vector3 DraggingPlanePoint = {};
        localPersist Vector3 PreviousDraggingPoint = {};
        
        Editor->SelectingBezier = true;
        _KeyFrame * KeyFrameArray = (_KeyFrame *)SelectedBezierBone->KeyFrameArray;
        
        _KeyFrame * KeyFrame = KeyFrameArray + SelectedBezierKeyFrameIndex;
        
        if(ModifyingPosition)
        {
            
            if(MouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                RedragSelection = false;
                DraggingPlanePoint = KeyFrame->BoneState.LocalPosition;
                PreviousDraggingPoint = KeyFrame->BoneState.LocalPosition;
            }
            
            //TODO : it just snap to the point instead adding offset
            Vector3 DraggingPoint = GetCollisionQuad3D(GetBillboardQuad(DraggingPlanePoint , 100000,100000)).point;
            
            Vector3 NewKeyFrameOffset = Vector3Subtract(DraggingPoint , PreviousDraggingPoint);
            PreviousDraggingPoint = DraggingPoint;
            
            //TODO : it is being overwritten by something else
            if(Editor->PointerFrameIndex == KeyFrame->FrameIndex)
            {
                SelectedBezierBone->State.LocalPosition = Vector3Add(SelectedBezierBone->State.LocalPosition , NewKeyFrameOffset);
            }
            else
            {
                KeyFrame->BoneState.LocalPosition = Vector3Add(KeyFrame->BoneState.LocalPosition , NewKeyFrameOffset);
            }
            
        }
        else
        {
            _KeyFrame * OtherKeyFrame = 0;
            
            Vector3 StartPoint = {};
            Vector3 EndPoint = {};
            Vector3 BezierOffset = {};
            
            if(SelectedPreviousBezierPoint)
            {
                
                BezierOffset = KeyFrame->PreviousBezierOffset;
                OtherKeyFrame = KeyFrameArray + SelectedBezierOtherKeyFrameIndex;
                
                StartPoint = KeyFrame->BoneState.LocalPosition;
                EndPoint = OtherKeyFrame->BoneState.LocalPosition;
                
            }
            else
            {
                
                BezierOffset = KeyFrame->NextBezierOffset;
                OtherKeyFrame = KeyFrameArray + SelectedBezierOtherKeyFrameIndex;
                
                StartPoint = KeyFrame->BoneState.LocalPosition;
                EndPoint = OtherKeyFrame->BoneState.LocalPosition;
                
            }
            
            Vector3 Direction = Vector3Subtract(EndPoint , StartPoint);
            Direction = Vector3Scale(Vector3Normalize(Direction),0.1f);
            
            Vector3 ScaledDirection = Vector3Add(Direction , BezierOffset);
            
            Vector3 BezierStart = KeyFrame->BoneState.LocalPosition;
            Vector3 BezierEnd = Vector3Add(BezierStart , ScaledDirection);
            
            _Rect BezierEndRect = GetBillboardRect(BezierEnd , 0.1);
            
            if(MouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                RedragSelection = false;
                
                DraggingPlanePoint = BezierEnd;
                PreviousDraggingPoint = BezierEnd;
            }
            
            //TODO : it just snap to the point instead adding offset
            Vector3 DraggingPoint = GetCollisionQuad3D(GetBillboardQuad(DraggingPlanePoint , 100000,100000)).point;
            Vector3 NewBezierEndPoint = Vector3Add(BezierEnd ,Vector3Subtract(DraggingPoint , PreviousDraggingPoint));
            PreviousDraggingPoint = DraggingPoint;
            Vector3 NewBezierOffset = Vector3Subtract(NewBezierEndPoint , BezierStart);
            NewBezierOffset = Vector3Subtract(NewBezierOffset , Direction);
            
            if(SelectedPreviousBezierPoint)
            {
                KeyFrame->PreviousBezierOffset = NewBezierOffset;
            }
            else
            {
                KeyFrame->NextBezierOffset = NewBezierOffset;
            }
            
            DrawCircle(BezierEndRect , YELLOW);
            DrawRoundLine( BezierStart , BezierEnd , 20  , YELLOW );
        }
        
    }
    
    //SingleSelectionAndMultiDrag
    localPersist bool DraggingBonePosition = false;
    localPersist Vector3 PreviousDragPoint ={};
    localPersist Vector3 DragPlanePosition = {};
    
    if(MouseButtonPressedWithCheck(MOUSE_BUTTON_LEFT))
    {
        DraggingBonePosition = false;
    }
    
    if(MouseButtonReleasedWithCheck(MOUSE_BUTTON_LEFT))
    {
        DraggingBonePosition = false;
    }
    
    ChangeMatrix(ScreenMVP);
    for(int BoneIndex = 0 ; BoneIndex < BoneCount ; BoneIndex++)
    {
        _Bone * CurrentBone = Editor->StickMan[BoneIndex];
        Vector3 BoneScreenPoint = {};
        //CurrentBone->hovering_and_change_position = false;
        
        Vector3 BoneInteractPoint ={};
        if(Editor->ControlRig)
        {
            if(!IsRig(BoneIndex)) continue;
            BoneInteractPoint = CurrentBone->Position;
        }
        else
        {
            if(IsRig(BoneIndex)) continue;
            
            Vector3 BonePositionEnd = Vector3RotateByQuaternion(CurrentBone->State.Direction , CurrentBone->Rotation);
            BonePositionEnd = Vector3Add(BonePositionEnd , CurrentBone->Position);
            
            BoneInteractPoint = BonePositionEnd;
        }
        
        BoneScreenPoint = TransformVector(BoneInteractPoint , MVP_3DWorld);
        
        _Rect BoneScreenRect = GetRect();
        BoneScreenRect.Position = BoneScreenPoint;
        BoneScreenRect.Size = PixelToSize(40 ,40);
        
        //TODO : this is dumb 
        _Quad InteractQuad = GetBillboardQuad(BoneInteractPoint , 100000 , 100000 );
        RayCollision PositionQuadResult = GetCollisionQuad3D(InteractQuad);
        
        Vector2 CurrentMousePosition = PixelToWorld(AppData->MousePosition);
        
        if(CheckCollisionRect(BoneScreenRect , CurrentMousePosition))
        {
            CurrentBone->Hovered = true;
            
            if(MouseButtonPressedWithCheck(MOUSE_BUTTON_LEFT))
            {
                bool SelectSingle = true;
                
                for(int StackIndex = 0 ; StackIndex < Editor->SelectedBoneCount ; StackIndex++)
                {
                    if(Editor->SelectedBoneStack[StackIndex] == BoneIndex) 
                    {
                        SelectSingle = false;
                        break;
                    }
                }
                
                if(SelectSingle)
                {
                    if(KeyPressing(KEY_LEFT_SHIFT))
                    {
                        
                        CurrentBone->Selected = true;
                        Editor->SelectedBoneStack[Editor->SelectedBoneCount++] = BoneIndex;
                        
                    }
                    else
                    {
                        
                        for(int StackIndex = 0 ; StackIndex < Editor->SelectedBoneCount ; StackIndex++)
                        {
                            Editor->StickMan[Editor->SelectedBoneStack[StackIndex]]->Selected = false;
                        }
                        
                        CurrentBone->Selected = true;
                        Editor->SelectedBoneStack[0] = BoneIndex;
                        Editor->SelectedBoneCount = 1;
                        
                    }
                }
                
                PreviousDragPoint = PositionQuadResult.point;
                DragPlanePosition = BoneInteractPoint;
                
                DraggingBonePosition = true;
                RedragSelection = false;
                
            }
            
        }
        
    }
    
    if(DraggingBonePosition)
    {
        _Quad BoneCentreQuad = GetBillboardQuad(DragPlanePosition, 100000.0f, 100000.0f);
        Vector3 DragPoint = GetCollisionQuad3D(BoneCentreQuad).point;
        //DrawQuadLine(GetBillboardQuad(DragPoint , 0.1 , 0.1) , WHITE , 8);
        
        Vector3 PlaneOffset= DragPoint;
        PlaneOffset = Vector3Subtract(PlaneOffset, PreviousDragPoint);
        
        PreviousDragPoint = DragPoint;
        
        for(int StackIndex = 0 ; StackIndex < Editor->SelectedBoneCount ; StackIndex++)
        {
            _Bone * DraggingBone = Editor->StickMan[Editor->SelectedBoneStack[StackIndex]];
            
            //it just feel right to do it
            //also the world rotation is compute after
            //something could go wrong
            Quaternion RevertRotation = QuaternionMultiply(DraggingBone->Rotation , QuaternionInvert(DraggingBone->State.LocalRotation));
            //RevertRotation = SelectedBone->State.LocalRotation;
            PlaneOffset = Vector3RotateByQuaternion(PlaneOffset,QuaternionInvert(RevertRotation));
            //printf("Offset : %f %f %f\n" ,PlaneOffset.x , PlaneOffset.y , PlaneOffset.z);
            
            if (DraggingBone->FreePosition)
            {
                DraggingBone->State.LocalPosition = Vector3Add(DraggingBone->State.LocalPosition, PlaneOffset);
            }
            
        }
    }
    else
    {
        
        //SingleBoneRotation
        ChangeMatrix(MVP_3DWorld);
        if(Editor->SelectedBoneCount == 1)
        {
            int SingleBoneIndex = Editor->SelectedBoneStack[0];
            
            _Bone * SelectedBone = Editor->StickMan[SingleBoneIndex];
            
            bool InteractingSelectedBone = CheckSelectedBoneRotation(SingleBoneIndex);
            
            if(MouseButtonPressedWithCheck(MOUSE_BUTTON_LEFT))
            {
                if(InteractingSelectedBone)
                {
                    RedragSelection = false;
                }
                else
                {
                    for(int StackIndex = 0 ; StackIndex < Editor->SelectedBoneCount ; StackIndex++)
                    {
                        Editor->StickMan[Editor->SelectedBoneStack[StackIndex]]->Selected = false;
                    }
                    
                    Editor->SelectedBoneCount = 0;
                    
                }
                
            }
        }
        
        ChangeMatrix(ScreenMVP);
    }
    
    //MultiSelection
    localPersist Vector3 mouse_start_drag_position = {};
    Vector2 mouse_screen_position = PixelToWorld(AppData->MousePosition.x , AppData->MousePosition.y);
    
    if(MouseButtonPressedWithCheck(MOUSE_BUTTON_LEFT))
    {
        mouse_start_drag_position.x = mouse_screen_position.x;
        mouse_start_drag_position.y = mouse_screen_position.y;
    }
    
    if(RedragSelection)
    {
        Vector3 current_mouse_drag_position = {mouse_screen_position.x , mouse_screen_position.y , 0};
        
        _Rect mouse_drag_rect = GetRect();
        mouse_drag_rect.Position= Vector3Add(mouse_start_drag_position , current_mouse_drag_position);
        mouse_drag_rect.Position.x /= 2.0f;
        mouse_drag_rect.Position.y /= 2.0f;
        
        mouse_drag_rect.Size.x = mouse_start_drag_position.x - current_mouse_drag_position.x;
        if(mouse_drag_rect.Size.x < 0) mouse_drag_rect.Size.x *= -1;
        
        mouse_drag_rect.Size.y = mouse_start_drag_position.y - current_mouse_drag_position.y;
        if(mouse_drag_rect.Size.y < 0) mouse_drag_rect.Size.y *= -1;
        
        DraggingSelection = true;
        
        if(MouseButtonPressedWithCheck(MOUSE_BUTTON_LEFT))
        {
            if(!KeyPressing(KEY_LEFT_SHIFT))
            {
                for(int StackIndex = 0 ; StackIndex < Editor->SelectedBoneCount ; StackIndex++)
                {
                    Editor->StickMan[Editor->SelectedBoneStack[StackIndex]]->Selected = false;
                }
                
                Editor->SelectedBoneCount = 0;
            }
        }
        
        if(MouseButtonPressingWithCheck(MOUSE_BUTTON_LEFT))
        {
            DrawRectLine(mouse_drag_rect , WHITE , 5);
        }
        
        for(int BoneIndex = 0 ; BoneIndex < BoneCount ; BoneIndex++)
        {
            _Bone * CurrentBone = Editor->StickMan[BoneIndex];
            Vector3 BoneScreenPoint = {};
            //CurrentBone->hovering_and_change_position = false;
            
            if(Editor->ControlRig)
            {
                if(!IsRig(BoneIndex)) continue;
                
                BoneScreenPoint = TransformVector(CurrentBone->Position , MVP_3DWorld);
            }
            else
            {
                if(IsRig(BoneIndex)) continue;
                
                Vector3 BonePositionEnd = Vector3RotateByQuaternion(CurrentBone->State.Direction , CurrentBone->Rotation);
                BonePositionEnd = Vector3Add(BonePositionEnd , CurrentBone->Position);
                
                BoneScreenPoint = TransformVector(BonePositionEnd , MVP_3DWorld);
            }
            
            _Rect BoneScreenRect = GetRect();
            BoneScreenRect.Position = BoneScreenPoint;
            BoneScreenRect.Size = PixelToSize(40 ,40);
            
            if(CheckCollisionRect(mouse_drag_rect , BoneScreenRect ))
            {
                if(MouseButtonPressingWithCheck(MOUSE_BUTTON_LEFT))
                {
                    CurrentBone->Hovered = true;
                }
                
                if(MouseButtonReleasedWithCheck(MOUSE_BUTTON_LEFT))
                {
                    if(!CurrentBone->Selected)
                    {
                        CurrentBone->Selected = true;
                        Editor->SelectedBoneStack[Editor->SelectedBoneCount++] = BoneIndex;
                    }
                }
            }
        }
    }
    
    for(int BoneIndex = 0 ; BoneIndex < BoneCount ; BoneIndex++)
    {
        
        _Bone * Bone = Editor->StickMan[BoneIndex];
        
        Vector3 BoneScreenPoint = {};
        if(Editor->ControlRig)
        {
            if(!IsRig(BoneIndex)) continue;
            
            BoneScreenPoint = TransformVector(Bone->Position , MVP_3DWorld);
        }
        else
        {
            
            if(IsRig(BoneIndex)) continue;
            
            Vector3 BonePositionEnd = Vector3RotateByQuaternion(Bone->State.Direction , Bone->Rotation);
            BonePositionEnd = Vector3Add(BonePositionEnd , Bone->Position);
            BoneScreenPoint = TransformVector(BonePositionEnd , MVP_3DWorld);
            
        }
        
        _Rect BoneScreenRect = GetRect();
        BoneScreenRect.Position = BoneScreenPoint;
        BoneScreenRect.Size = PixelToSize(40 ,40);
        
        if(Bone->Selected)
        {
            DrawRectLine(BoneScreenRect , BLUE , 5);
        }
        else if(Bone->Hovered)
        {
            DrawRectLine(BoneScreenRect , YELLOW , 5);
        }
        else
        {
            DrawRectLine(BoneScreenRect , Fade(YELLOW , 0.2f) , 5);
        }
        
        Bone->Hovered = false;
    }
    
    ChangeMatrix(MVP_3DWorld);
    
}

internal void Editor_GUI()
{	
    
    rlFramebufferAttach(RenderState.ScreenFrameBuffer, RenderState.GUITexture, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);    
    if (!rlFramebufferComplete(RenderState.ScreenFrameBuffer)) ASSERT;
    rlEnableFramebuffer(RenderState.ScreenFrameBuffer);
	
    glClearColor(0,0,0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
    
	localPersist bool BoneMenuEnable = false;
    
	if (MouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		Editor->OperateMenuPosition = PixelToWorld(AppData->MousePosition);
        
		if (Editor->SelectedBoneCount == 1)
		{
			BoneMenuEnable = true;
		}
	}
    
	if (Editor->SelectedBoneCount != 1)
	{
		BoneMenuEnable = false;
	}
    
	if (BoneMenuEnable)
	{
		_Bone * EditingBone = Editor->StickMan[Editor->SelectedBoneStack[0]];
		DrawingMenu Menu = StartDrawMenuMouse();
        
		DrawMenuFloatInput(&Menu , L"長度 : %f " , &EditingBone->State.Direction.y);
        
		if (DrawMenuButton(&Menu , L"重置位置"))
		{
			EditingBone->State.LocalPosition = {};
		}
		
		if (DrawMenuButton(&Menu , L"重置旋轉"))
		{
			EditingBone->State.LocalRotation = QuaternionIdentity();
		}
        
        if(DrawMenuButton(&Menu , EditingBone->FreePosition ? L"鎖定位置" : L"自由位置" ))
        {
            EditingBone->FreePosition = !EditingBone->FreePosition;
        }
        
        int KeyFrameCount = 0;
        _KeyFrame * KeyFrameArray = (_KeyFrame *)EditingBone->KeyFrameArray;
        
        HashTable_Iterate(KeyFrameIndex , Editor->PointerFrameIndex , &EditingBone->KeyFrameHashTable , MAX_KEY_FRAME)
        {
            if (DrawMenuButton(&Menu , L"重置貝塞爾曲綫"))
            {
                _KeyFrame * KeyFrame = KeyFrameArray + KeyFrameIndex;
                
                KeyFrame->PreviousBezierOffset = {};
                KeyFrame->NextBezierOffset = {};
            }
            
        }
		
	}
	
	_Rect TimelineRect = GetRect();
	TimelineRect.Position = {0,-1};
    
	TimelineRect.Size = {PixelToWidth(AppData->WindowSize.x - 60), PixelToHeight(80)};
	TimelineRect.Position.y += TimelineRect.Size.y * 0.5f;
	TimelineRect.Position.y += PixelToHeight(40);
    
	DrawRect(TimelineRect , PixelToWidth(5) , Fade(BLACK , 0.2f));
    
	localPersist bool dragging_frame_pointer = false;
    
	if (MouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		dragging_frame_pointer = false;
	}
    
	float FramePositionOffsetX = PixelToWidth(20) * Editor->TimelineScale;
    
	if (CheckCollisionRectMouse(TimelineRect))
	{
		MenuInteruped = true;
        
		if (KeyPressing(KEY_LEFT_CONTROL))
		{
			Editor->TimelineScale += AppData->MouseScrollDelta * 0.1f;
		}
		else
		{
			Editor->TimelineSliderOffset += AppData->MouseScrollDelta * 0.1f;
		}
        
	}
    
	int ScrollOffset = -Editor->TimelineSliderOffset / FramePositionOffsetX;
    ScrollOffset += Editor->StartFrameIndex;
    
	if (ScrollOffset < 0) ScrollOffset = 0;
    
    float ClosestFrameX = FLT_MAX;
	
	localPersist Vector3 PointerPosition = {};
    
	int ClosestFrameIndexToMouse = -1;
    
	for (int FrameIndex = ScrollOffset ; ; FrameIndex++)
	{
		if (FrameIndex > Editor->EndFrameIndex) break;
        
		_Rect FrameRect = GetRect();
		Vector3 FramePosition = TimelineRect.Position;
		FramePosition.x += FrameIndex * FramePositionOffsetX - TimelineRect.Size.x * 0.5f + FramePositionOffsetX * 0.5f + Editor->TimelineSliderOffset;
        
		if (FramePosition.x > TimelineRect.Size.x * 0.5 + TimelineRect.Position.x) break;
        
		FrameRect.Position = FramePosition;
		FrameRect.Size = {PixelToWidth(2), TimelineRect.Size.y*0.8f};
        
		DrawRect(FrameRect,PixelToWidth(5),Fade(BLACK , 0.2f));
        
		wchar_t FrameIndexString[64] = {};
		swprintf(FrameIndexString , L"%d" , FrameIndex);
        
		_Rect TextRect = FrameRect;
		TextRect.Size.y = PixelToWidth(6);
        
		D_DrawText(TextRect, FrameIndexString , WHITE  , true);
        
		Vector2 MousePosition = PixelToWorld(AppData->MousePosition);
		float FrameToMouseX = MousePosition.x - FrameRect.Position.x;
		if (FrameToMouseX < 0) FrameToMouseX *= -1;
        
		if (ClosestFrameX > FrameToMouseX)
		{
			ClosestFrameX = FrameToMouseX;
			ClosestFrameIndexToMouse = FrameIndex;
		}
        
		if (FrameIndex == Editor->PointerFrameIndex)
		{
			PointerPosition = FrameRect.Position;
		}
	}
	
	if (dragging_frame_pointer)
	{
		MenuInteruped = true;
        
		Editor->PointerFrameIndex = ClosestFrameIndexToMouse;
	}
    
    localPersist bool DraggingKeyFrame = false;
    
	for (int FrameIndex = ScrollOffset;; FrameIndex++)
	{
		if (FrameIndex > Editor->EndFrameIndex) break;
        
        for(int StackIndex = 0 ; StackIndex < Editor->SelectedBoneCount ; StackIndex++)
        {
            
            Vector3 FramePosition = TimelineRect.Position;
            FramePosition.y += TimelineRect.Size.y * 0.4f;
			FramePosition.x += FrameIndex * FramePositionOffsetX - TimelineRect.Size.x * 0.5f + FramePositionOffsetX * 0.5f + Editor->TimelineSliderOffset;
            
			_Bone * CurrentBone = Editor->StickMan[Editor->SelectedBoneStack[StackIndex]];
            
			HashTable_Iterate(KeyFrameIndex , FrameIndex, &CurrentBone->KeyFrameHashTable , MAX_KEY_FRAME)
			{
				_KeyFrame * KeyFrame = (_KeyFrame *)(CurrentBone->KeyFrameArray + KeyFrameIndex);
                
				if (KeyFrame->FrameIndex != FrameIndex) continue;
                
				_Rect KeyFrameRect = GetRect();
				KeyFrameRect.Position = FramePosition;
                
				KeyFrameRect.Size = { PixelToWidth(20) , PixelToHeight(20) };
                KeyFrameRect.Position.y -= 1.1f * KeyFrameRect.Size.y * StackIndex;
                
                bool Draggable = true;
                
                if(DraggingKeyFrame) Draggable = false;
                if(!CheckCollisionRectMouse(KeyFrameRect)) Draggable = false;
                
				if (Draggable)
				{
					if (MouseButtonPressed(MOUSE_BUTTON_LEFT))
					{
                        DraggingKeyFrame = true;
						AddToListTail(KeyFrameIndex , &CurrentBone->DraggingKeyFrameList , MAX_KEY_FRAME);
					}
                    
					DrawCircle(KeyFrameRect , Fade(ORANGE , 0.6));
				}
				else
				{
					DrawCircle(KeyFrameRect , Fade(ORANGE , 0.4));
				}
                
			}
            
        }
        
	}
    
    //printf("%d \n" , Editor->StickMan[Editor->SelectedBoneIndex].DraggingKeyFrameList.UnusedIndex);
    
	localPersist int PreviousFrameIndex = -1;
    
	if (MouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		PreviousFrameIndex = ClosestFrameIndexToMouse;
	}
    
#if 1
    for(int StackIndex = 0 ; StackIndex < Editor->SelectedBoneCount; StackIndex++)
	{
		_Bone * CurrentBone = Editor->StickMan[Editor->SelectedBoneStack[StackIndex]];
        
		if (MouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
            DraggingKeyFrame = false;
            ClearList(&CurrentBone->DraggingKeyFrameList);
            
		}
        
        if(MouseButtonPressing(MOUSE_BUTTON_LEFT))
        {
            _KeyFrame * KeyFrameArray = (_KeyFrame *)CurrentBone->KeyFrameArray;
            
            List_Foreach(DraggingKeyFrameIndex , &CurrentBone->DraggingKeyFrameList)
            {
                MenuInteruped = true;
                _KeyFrame * DraggingKeyFrame = KeyFrameArray + DraggingKeyFrameIndex;
                
                bool FrameExisted = false;
                HashTable_Iterate(ExistedKeyFrameIndex ,ClosestFrameIndexToMouse , &CurrentBone->KeyFrameHashTable , MAX_KEY_FRAME)
                {
                    if(KeyFrameArray[ExistedKeyFrameIndex].FrameIndex == ClosestFrameIndexToMouse)
                    {
                        FrameExisted = true;
                        break;
                    }
                }
                
                if(!FrameExisted)
                {
                    DeleteFromHashTable(DraggingKeyFrame->FrameIndex , DraggingKeyFrameIndex , &CurrentBone->KeyFrameHashTable , MAX_KEY_FRAME);
                    
                    DraggingKeyFrame->FrameIndex = ClosestFrameIndexToMouse;
                    
                    AddToHashTable(DraggingKeyFrame->FrameIndex , DraggingKeyFrameIndex , &CurrentBone->KeyFrameHashTable , MAX_KEY_FRAME);
                    
                    List_ForeachEx(NextKeyFrameIndex , DraggingKeyFrame->OrderListNodeIndex , NodeIndex , &CurrentBone->KeyFrameOrderList)
                    {
                        _KeyFrame * NextKeyFrame = KeyFrameArray + NextKeyFrameIndex;
                        
                        if(NextKeyFrame->FrameIndex == DraggingKeyFrame->FrameIndex) ASSERT;
                        if(NextKeyFrame->FrameIndex > DraggingKeyFrame->FrameIndex) break;
                        
                        _ListNode * NextKeyFrameNode = CurrentBone->KeyFrameOrderList.NodeArray + NextKeyFrame->OrderListNodeIndex;
                        _ListNode * DraggingKeyFrameNode = CurrentBone->KeyFrameOrderList.NodeArray + DraggingKeyFrame->OrderListNodeIndex;
                        
                        int TempDataIndex = NextKeyFrameNode->DataIndex;
                        NextKeyFrameNode->DataIndex = DraggingKeyFrameNode->DataIndex;
                        DraggingKeyFrameNode->DataIndex = TempDataIndex;
                        
                        int TempNodeIndex = NextKeyFrame->OrderListNodeIndex;
                        NextKeyFrame->OrderListNodeIndex = DraggingKeyFrame->OrderListNodeIndex;
                        DraggingKeyFrame->OrderListNodeIndex = TempNodeIndex;
                        
                    }
                    
                    List_ForeachReverseEx(PreviousKeyFrameIndex , DraggingKeyFrame->OrderListNodeIndex , NodeIndex , &CurrentBone->KeyFrameOrderList)
                    {
                        _KeyFrame * PreviousKeyFrame = KeyFrameArray + PreviousKeyFrameIndex;
                        
                        if(PreviousKeyFrame->FrameIndex == ClosestFrameIndexToMouse) ASSERT;
                        if(PreviousKeyFrame->FrameIndex < ClosestFrameIndexToMouse) break;
                        
                        _ListNode * PreviousKeyFrameNode = CurrentBone->KeyFrameOrderList.NodeArray + PreviousKeyFrame->OrderListNodeIndex;
                        _ListNode * DraggingKeyFrameNode = CurrentBone->KeyFrameOrderList.NodeArray + DraggingKeyFrame->OrderListNodeIndex;
                        
                        int TempDataIndex = PreviousKeyFrameNode->DataIndex;
                        PreviousKeyFrameNode->DataIndex = DraggingKeyFrameNode->DataIndex;
                        DraggingKeyFrameNode->DataIndex = TempDataIndex;
                        
                        int TempNodeIndex = PreviousKeyFrame->OrderListNodeIndex;
                        PreviousKeyFrame->OrderListNodeIndex = DraggingKeyFrame->OrderListNodeIndex;
                        DraggingKeyFrame->OrderListNodeIndex = TempNodeIndex;
                        
                    }
                    
                }
                
                
            }
        }
	}
#endif
    
    for (int BoneIndex = 0; BoneIndex < BoneCount; BoneIndex++)
    {
        
        _Bone * CurrentBone = Editor->StickMan[BoneIndex];
        
        _KeyFrame * KeyFrameArray = (_KeyFrame *)CurrentBone->KeyFrameArray;
        _ListNode * KeyFrameOrderNodeArray = (_ListNode *)CurrentBone->KeyFrameOrderList.NodeArray;
        
#if 1
        int ClosestKeyFrameOrderNodeIndex = CurrentBone->FirstKeyFrameOnPointerLeft_OrderListIndex;
        if(ClosestKeyFrameOrderNodeIndex == -1) ClosestKeyFrameOrderNodeIndex = CurrentBone->KeyFrameOrderList.NodeArray[N_NodeHead].Next;
        
        _ListNode * ClosestKeyFrameNode = KeyFrameOrderNodeArray + ClosestKeyFrameOrderNodeIndex;
        _KeyFrame * ClosestKeyFrame =  KeyFrameArray + ClosestKeyFrameNode->DataIndex;
        
        if(Editor->PointerFrameIndex > ClosestKeyFrame->FrameIndex)
        {
            
            List_ForeachEx(NextKeyFrameIndex , ClosestKeyFrameOrderNodeIndex , NodeIndex , &CurrentBone->KeyFrameOrderList)
            {
                
                _KeyFrame * NextKeyFrame = KeyFrameArray + NextKeyFrameIndex;
                if(NextKeyFrame->FrameIndex > Editor->PointerFrameIndex) break;
                
                ClosestKeyFrameOrderNodeIndex = NodeIndex;
            }
            
        }
        else if( Editor->PointerFrameIndex < ClosestKeyFrame->FrameIndex)
        {
            
            List_ForeachReverseEx(NextKeyFrameIndex , ClosestKeyFrameOrderNodeIndex , NodeIndex , &CurrentBone->KeyFrameOrderList)
            {
                _KeyFrame * PreviousKeyFrame = KeyFrameArray + NextKeyFrameIndex;
                if(PreviousKeyFrame->FrameIndex < Editor->PointerFrameIndex) break;
                
                
                ClosestKeyFrameOrderNodeIndex = NodeIndex;
            }
            
        }
        
        if(ClosestKeyFrameOrderNodeIndex != -1)
        {
            int KeyFrameDataIndex = CurrentBone->KeyFrameOrderList.NodeArray[ClosestKeyFrameOrderNodeIndex].DataIndex;
            
            if(KeyFrameDataIndex == -1)
            {
                ClosestKeyFrameOrderNodeIndex = -1;
            }
            else
            {
                
                int NewClosestKeyFrame_FrameIndex = KeyFrameArray[KeyFrameDataIndex].FrameIndex;
                if(Editor->PointerFrameIndex < NewClosestKeyFrame_FrameIndex ) ClosestKeyFrameOrderNodeIndex = -1;
                
            }
            
        }
        
        CurrentBone->FirstKeyFrameOnPointerLeft_OrderListIndex = ClosestKeyFrameOrderNodeIndex;
#endif
    }
    
	_Rect FramePointerRect = GetRect();
	FramePointerRect.Position = PointerPosition;
	FramePointerRect.Size.y = TimelineRect.Size.y * 1.2f;
	FramePointerRect.Size.x = PixelToWidth(10);
    
	bool dragging_or_hovering_frame_pointer = false;
    
	if (CheckCollisionRectMouse(FramePointerRect))
	{
		if (MouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			dragging_frame_pointer = true;
		}
        
		dragging_or_hovering_frame_pointer = true;
	}
    
	if (dragging_frame_pointer) dragging_or_hovering_frame_pointer = true;
    
	if (dragging_or_hovering_frame_pointer)
	{
		DrawRect(FramePointerRect , YELLOW);
	}
	else
	{
		DrawRect(FramePointerRect , WHITE);
	}
    
    
	Vector3 AddNewFrameButtonPosition = TimelineRect.Position;
	AddNewFrameButtonPosition.y += TimelineRect.Size.y*0.5f;
	AddNewFrameButtonPosition.x -= TimelineRect.Size.x*0.5f;
	
	DrawingMenu TimelineOperationMenu = StartDrawMenu({AddNewFrameButtonPosition.x , AddNewFrameButtonPosition.y} , true ,GMT_ToTheRight);
	TimelineOperationMenu.CurrentButtonPosition.y += PixelToHeight(45);
	TimelineOperationMenu.ButtonExtraOffset = PixelToWidth(5);
    
	bool PlayStartOrStop = false;
	
	if (DrawMenuButton(&TimelineOperationMenu , Editor->Playing ? L"播放中" : L"播放" ))
	{
		PlayStartOrStop = true;
	}
    
	DrawMenuIntInput(&TimelineOperationMenu , L"開始幀 : %d" , &Editor->StartFrameIndex);
	DrawMenuIntInput(&TimelineOperationMenu , L"結束幀 : %d" , &Editor->EndFrameIndex);
    
	localPersist float StartTime = 0;
	localPersist float EndTime = 0;
    
	StartTime = ((float)Editor->StartFrameIndex) / ((float)FRAME_PER_SECOND);
	EndTime = ((float)Editor->EndFrameIndex) / ((float)FRAME_PER_SECOND);
    
	DrawMenuFloatInput(&TimelineOperationMenu , L"開始時間 : %f" , &StartTime);
	DrawMenuFloatInput(&TimelineOperationMenu , L"結束時間 : %f" , &EndTime);
    
	Editor->StartFrameIndex = StartTime * FRAME_PER_SECOND;	
	Editor->EndFrameIndex = EndTime * FRAME_PER_SECOND;
    
	if (KeyPressed(KEY_SPACE))
	{
		PlayStartOrStop = true;
	}
    
	if (PlayStartOrStop)
	{
		Editor->Playing = !Editor->Playing;
	}
    
	if (!Editor->Playing)
	{
		Editor->PlayTimer = 0;
	}
	else
	{
		Editor->PlayTimer += DeltaTime;
        
		if (Editor->PlayTimer > FRAME_TIME)
		{
			Editor->PlayTimer -= FRAME_TIME;
			Editor->PointerFrameIndex++;
            
			if (Editor->PointerFrameIndex > Editor->EndFrameIndex)
			{
				Editor->PointerFrameIndex = Editor->StartFrameIndex;
			}
		}
	}
    
    bool AddNewKeyFrame = false;
    bool RemoveKeyFrame = false;
    
    if(Editor->SelectedBoneCount >0)
    {
        AddNewKeyFrame = DrawMenuButton(&TimelineOperationMenu , L"添加幀");
        RemoveKeyFrame = DrawMenuButton(&TimelineOperationMenu , L"刪除幀");
    }
    
    for(int StackIndex = 0 ; StackIndex < Editor->SelectedBoneCount ; StackIndex++)
    {
        _Bone * SelectedBone = Editor->StickMan[Editor->SelectedBoneStack[StackIndex]];
        
        if (AddNewKeyFrame)
        {
            
            bool FrameExisted = false;
            
            HashTable_Iterate(KeyFrameIndex, Editor->PointerFrameIndex, &SelectedBone->KeyFrameHashTable , MAX_KEY_FRAME )
            {
                
                _KeyFrame persist * KeyFrame = SelectedBone->KeyFrameArray + KeyFrameIndex;
                
                if (KeyFrame->FrameIndex == Editor->PointerFrameIndex)
                {
                    FrameExisted = true;
                    break;
                }
                
            }
            
            if (!FrameExisted)
            {
                
                int NewKeyFrameIndex = GetEmtyNodeFromList(&SelectedBone->KeyFrameList , MAX_KEY_FRAME);
                
                int NewKeyFrameNodeIndex = AddToListTail( NewKeyFrameIndex  ,&SelectedBone->KeyFrameList , MAX_KEY_FRAME);
                AddToHashTable( Editor->PointerFrameIndex , NewKeyFrameIndex , &SelectedBone->KeyFrameHashTable , MAX_KEY_FRAME);
                
                _KeyFrame * KeyFrameArray = (_KeyFrame *)SelectedBone->KeyFrameArray;
                _ListNode * KeyFrameNodeArray = (_ListNode *)SelectedBone->KeyFrameList.NodeArray;
                
                _KeyFrame * NewKeyFrame = KeyFrameArray + NewKeyFrameIndex;
                
                (*NewKeyFrame) = {};
                
                NewKeyFrame->FrameIndex = Editor->PointerFrameIndex;
                NewKeyFrame->BoneState = SelectedBone->State;
                
                int NewKeyFrameOrderNodeIndex = -1;
                
                if(SelectedBone->FirstKeyFrameOnPointerLeft_OrderListIndex == -1)
                {
                    NewKeyFrameOrderNodeIndex = AddToListHead( NewKeyFrameIndex , &SelectedBone->KeyFrameOrderList , MAX_KEY_FRAME);
                }
                else
                {
                    NewKeyFrameOrderNodeIndex = CreateAndInsertToList( false , SelectedBone->FirstKeyFrameOnPointerLeft_OrderListIndex , NewKeyFrameIndex , &SelectedBone->KeyFrameOrderList , MAX_KEY_FRAME);
                }
                
                if(NewKeyFrameOrderNodeIndex == -1) ASSERT;
                
                SelectedBone->FirstKeyFrameOnPointerLeft_OrderListIndex = NewKeyFrameOrderNodeIndex;
                
                NewKeyFrame->NodeIndex = NewKeyFrameNodeIndex;
                NewKeyFrame->OrderListNodeIndex = NewKeyFrameOrderNodeIndex;
            }
            
        }
        
        if (RemoveKeyFrame)
        {
            
            int FrameIndex = 0; 
            _KeyFrame * KeyFrameArray = (_KeyFrame *)SelectedBone->KeyFrameArray;
            HashTable_Iterate(KeyFrameIndex , Editor->PointerFrameIndex , &SelectedBone->KeyFrameHashTable ,MAX_KEY_FRAME)
            {
                _KeyFrame * KeyFrame = KeyFrameArray + KeyFrameIndex;
                
                if (KeyFrame->FrameIndex == Editor->PointerFrameIndex)
                {
                    SelectedBone->FirstKeyFrameOnPointerLeft_OrderListIndex = GetPreviousNodeIndex(SelectedBone->FirstKeyFrameOnPointerLeft_OrderListIndex , &SelectedBone->KeyFrameOrderList);
                    
                    DeleteFromHashTable(KeyFrame->FrameIndex, KeyFrameIndex , &SelectedBone->KeyFrameHashTable,MAX_KEY_FRAME);
                    if(!DeleteFromList(KeyFrame->NodeIndex , &SelectedBone->KeyFrameList)) ASSERT;
                    if(!DeleteFromList(KeyFrame->OrderListNodeIndex , &SelectedBone->KeyFrameOrderList)) ASSERT;
                    
                    break;
                }
                
                //printf( " %d %d %d\n", KeyFrame->FrameIndex , KeyFrame->Bone - Editor->StickMan , KeyFrame - Editor->KeyFrameHash.DataArray);
            }
            
        }
        
    }
    
    
#if 1
    
    if(Editor->SelectedBoneCount == 1 )
    {
        _Bone * SelectedBone = Editor->StickMan[Editor->SelectedBoneStack[0]];
        
        _KeyFrame * KeyFrameArray = (_KeyFrame *)SelectedBone->KeyFrameArray;
        int FirstKeyFrame_OrderListIndex = SelectedBone->FirstKeyFrameOnPointerLeft_OrderListIndex;
        
        if(FirstKeyFrame_OrderListIndex != -1)
        {
            
            int FirstKeyFrameIndex = GetDataIndexFromNodeIndex(FirstKeyFrame_OrderListIndex , &SelectedBone->KeyFrameOrderList);
            _KeyFrame * FirstKeyFrame = KeyFrameArray + FirstKeyFrameIndex;
            
            char FirstKeyFrameNameBuffer[256] = {};
            sprintf(FirstKeyFrameNameBuffer , "First : %d" , FirstKeyFrame->FrameIndex);
            
            DrawMenuButton(&TimelineOperationMenu , FirstKeyFrameNameBuffer);
            
        }
        else
        {
            DrawMenuButton(&TimelineOperationMenu , "First : None");
        }
        
        List_Foreach(KeyFrameIndex,&SelectedBone->KeyFrameOrderList )
        {
            _KeyFrame * KeyFrame = KeyFrameArray + KeyFrameIndex;
            
            char KeyFrameNameBuffer[256] = {};
            
            sprintf(KeyFrameNameBuffer , "%d" , KeyFrame->FrameIndex);
            
            if (DrawMenuButton(&TimelineOperationMenu , KeyFrameNameBuffer))
            {
                
            }
        }
    }
    
#endif
    
	D_GameDraw();
	rlDisableFramebuffer();
    
}

internal void IterateBoneStructure(_Bone * RootBone)
{
    _Bone * BoneStack[256] = {};
    int BoneStackCount = 0;
    
    BoneStack[BoneStackCount++] = RootBone;
    
    //printf("\nBone Start \n");
    
    for (; BoneStackCount > 0;)
    {
        _Bone * IteratingBone = BoneStack[--BoneStackCount];
        int IteratingBoneIndex = IteratingBone->BoneIndex;
        
        HashTable_Iterate(ChildBoneIndex , IteratingBoneIndex , &Editor->BoneChildrenHashTable , MAX_KEY_FRAME)
        {
            
#if 0
            printf("%s <--- %s\n" , 
                   EnumToString(BoneTag)[IteratingBoneIndex] , 
                   EnumToString(BoneTag)[ChildBoneIndex]);
#endif
            
            _Bone * ChildBone = Editor->StickMan[ChildBoneIndex];
            
            Vector3 IteratingBoneEndPosition = IteratingBone->Position;
            IteratingBoneEndPosition = Vector3Add(IteratingBoneEndPosition , Vector3RotateByQuaternion( IteratingBone->State.Direction , IteratingBone->Rotation));
            
            ChildBone->Rotation = QuaternionMultiply(IteratingBone->Rotation, ChildBone->State.LocalRotation);
            
            ChildBone->Position = Vector3RotateByQuaternion(ChildBone->State.LocalPosition , IteratingBone->Rotation );
            
            ChildBone->Position = Vector3Add(ChildBone->Position, IteratingBoneEndPosition);
            //ChildBone->Position = Vector3Add(ChildBone->Position, Vector3RotateByQuaternion(ChildBone->State.Direction , ChildBone->Rotation));
            
            BoneStack[BoneStackCount++] = ChildBone;
            
        }
        
    }
    
}

internal void UpdateBoneStructure()
{
    
	List_Foreach(RootBoneIndex , &Editor->RootBoneList )
	{
        _Bone * RootBone = Editor->StickMan[RootBoneIndex];
        
        RootBone->Position = RootBone->State.LocalPosition;
        RootBone->Rotation = RootBone->State.LocalRotation;
        
        IterateBoneStructure(RootBone);
	}
    
}

#define DEBUG_IK_DISPLAY 0

internal void BoneIKUpdate(_Bone * TargetBone , _Bone * PoleBone , _Bone * IKBone , int IterationCount , int BoneChainMaxLength)
{
    _Bone * BoneStack[256] = {};
    int BoneStackCount = 0;
    
    _Bone * BoneChain[256] = {};
    int BoneChainCount = 0;
    
    BoneStack[BoneStackCount++] = IKBone;
    BoneChain[BoneChainCount++] = IKBone;
    
    int BoneChainLength = 0;
    
    for(;;)
    {
        
        if(BoneStackCount <= 0) break;
        if(BoneChainLength >= BoneChainMaxLength) break;
        BoneChainLength++;
        
        _Bone * IteratingBone = BoneStack[--BoneStackCount];
        int IteratingBoneIndex = IteratingBone->BoneIndex;
        IteratingBone->State.LocalPosition = {};
        
        HashTable_Iterate(ParentBoneIndex , IteratingBoneIndex , &Editor->BoneParentHashTable , BoneCount)
        {
            BoneChain[BoneChainCount++] = Editor->StickMan[ParentBoneIndex];
            BoneStack[BoneStackCount++] = Editor->StickMan[ParentBoneIndex];
        }
        
    }
    
    for(int IterationIndex = 0 ; IterationIndex < IterationCount ; IterationIndex++)
    {
        Vector3 EndPosition = IKBone->Position; 
        EndPosition = Vector3Add(EndPosition , Vector3RotateByQuaternion(IKBone->State.Direction , IKBone->Rotation) );
        //DrawQuadLine(GetBillboardQuad(EndPosition , 0.1 , 0.1 ) ,WHITE);
        
        _Bone * StartBone = BoneChain[BoneChainCount -1];
        Vector3 StartPosition = StartBone->Position;
        //StartPosition = Vector3Add(StartPosition , StartBone->Position);
        
        Vector3 bone_chain_direction = Vector3Subtract(EndPosition , StartPosition);
        
#if DEBUG_IK_DISPLAY
        DrawArrowRay(StartPosition ,bone_chain_direction , Fade(WHITE,0.5f) );
#endif
        
        Vector3 BoneDirectionSum = {};
        
        Vector3 bone_chain_vertical_direction = {};
        
        //continue;
        
        for(int BoneChainIndex = BoneChainCount - 1 ; BoneChainIndex >= 1 ; BoneChainIndex--)
        {
            _Bone * CurrentBone = BoneChain[BoneChainIndex];
            
            
            if(IterationIndex == 0)
            {
                CurrentBone->Rotation = QuaternionIdentity();
            }
            
            
            Vector3 BoneEndDirection = Vector3RotateByQuaternion(CurrentBone->State.Direction , CurrentBone->Rotation);
            
            Vector3 BoneEnd = Vector3Add(CurrentBone->Position , BoneEndDirection);
            
            BoneDirectionSum = Vector3Add(BoneEndDirection , BoneDirectionSum);
            
            Vector3 bone_end_direction_project = Vector3Project(BoneDirectionSum , bone_chain_direction);
            
            Vector3 bone_chain_start_point_to_bone_end = Vector3Add(StartPosition , bone_end_direction_project);
            Vector3 bone_vertical_direction_to_bone_chain = Vector3Subtract(BoneEnd , bone_chain_start_point_to_bone_end);
            
#if DEBUG_IK_DISPLAY
            DrawArrowRay(bone_chain_start_point_to_bone_end , bone_vertical_direction_to_bone_chain , Fade(WHITE , 0.5f) );
#endif
            
            bone_chain_vertical_direction = Vector3Add(bone_chain_vertical_direction , bone_vertical_direction_to_bone_chain);
        }
        
        bone_chain_vertical_direction = Vector3Normalize(bone_chain_vertical_direction);
        
        Vector3 start_to_pole_direction = Vector3Subtract(PoleBone->Position , StartPosition);
        
        Vector3 pole_project_point_on_bone_chain = Vector3Project(start_to_pole_direction , bone_chain_direction);
        pole_project_point_on_bone_chain = Vector3Add(StartPosition , pole_project_point_on_bone_chain);
        Vector3 pole_direction = Vector3Subtract(PoleBone->Position , pole_project_point_on_bone_chain);
        pole_direction = Vector3Normalize(pole_direction);
        
        Vector3 bone_chain_centre_point = Vector3Lerp(StartPosition , EndPosition , 0.5);
        
#if DEBUG_IK_DISPLAY
        DrawArrowRay(bone_chain_centre_point , bone_chain_vertical_direction , Fade(RED, 0.5) );
        DrawArrowRay(bone_chain_centre_point , pole_direction , Fade(WHITE, 0.5) );
#endif
        
        Quaternion bone_chain_to_pole_rotation = QuaternionFromVector3ToVector3(bone_chain_vertical_direction , pole_direction);
        
        for(int  BoneChainIndex = BoneChainCount - 1 ; BoneChainIndex >= 0 ; BoneChainIndex--)
        {
            _Bone * CurrentBone = BoneChain[BoneChainIndex];
            
            CurrentBone->Rotation = QuaternionMultiply(bone_chain_to_pole_rotation , CurrentBone->Rotation );
            
            if(BoneChainIndex != 0)
            {
                _Bone * ChildBone = BoneChain[BoneChainIndex -1];
                
                ChildBone->Position = Vector3RotateByQuaternion(CurrentBone->State.Direction ,CurrentBone->Rotation );
                ChildBone->Position = Vector3Add(ChildBone->Position , CurrentBone->Position);
            }
        }
        
        for(int  BoneChainIndex = BoneChainCount - 1 ; BoneChainIndex >= 0 ; BoneChainIndex--)
        {
            _Bone * ParentBone = BoneChain[BoneChainIndex];
            _Bone * ChildBone = BoneChain[BoneChainIndex - 1];
            
            Vector3 DirectionToEndBone = Vector3Subtract(EndPosition ,ParentBone->Position);
            DirectionToEndBone = Vector3Normalize(DirectionToEndBone);
            
            Vector3 DirectionToTargetBone = Vector3Subtract(TargetBone->Position ,ParentBone->Position);
            DirectionToTargetBone = Vector3Normalize(DirectionToTargetBone);
            
#if DEBUG_IK_DISPLAY
            DrawArrowRay(ParentBone->Position, DirectionToEndBone , BLUE);
            DrawArrowRay(ParentBone->Position, DirectionToTargetBone , GREEN);
#endif
            
            //printf("%f \n" , Vector3Angle(DirectionToEndBone , DirectionToTargetBone) * RAD2DEG);
            
            //TODO : try Fabrik?
            Quaternion BoneRotation = QuaternionFromVector3ToVector3(DirectionToEndBone , DirectionToTargetBone );
            //BoneRotation = QuaternionMultiply( BoneRotation, QuaternionInvert(IteratingBone->Rotation));
            
            ParentBone->Rotation = QuaternionMultiply(BoneRotation , ParentBone->Rotation);
            
            //IteratingBone->State.LocalRotation = QuaternionIdentity();
            
            Vector3 ParentBoneEndOffset = Vector3RotateByQuaternion( ParentBone->State.Direction , ParentBone->Rotation );
            
            if(BoneChainIndex > 0)
            {
                ChildBone->Position = Vector3Add(ParentBone->Position , ParentBoneEndOffset);
            }
        }
        
        
    }
    
    //printf("%s \n", EnumToString(BoneTag)[ BoneChain[0] - StickMan]);
    IterateBoneStructure(BoneChain[0]);
    
}

internal void DrawAllBone()
{
	
#if 0
    
    _Rect HeadRect = GetRect();
	HeadRect.Position = Editor->StickMan[B_Head].Position;
	HeadRect.Size = {0.5,0.5};
    
	HeadRect.Rotation = QuaternionFromVector3ToVector3({0,1,0} , Editor->Up);
	HeadRect.Rotation = QuaternionMultiply(HeadRect.Rotation , QuaternionFromVector3ToVector3( { 1, 0, 0 } , Editor->Right));
	DrawRect(HeadRect , 0.25, WHITE );
    
	internal void DrawRoundLine(int StartBoneIndex , int EndBoneIndex)
	{
		DrawRoundLine(Editor->StickMan[StartBoneIndex]->Position, Editor->StickMan[EndBoneIndex]->Position , 80 , WHITE , WHITE);
	}
    
	DrawRoundLine(B_Head , B_UpperBody);
	DrawRoundLine(B_LowerBody , B_Pelvic);
	DrawRoundLine(B_UpperBody , B_LowerBody);
    
	DrawRoundLine(B_RightUpperArm , B_UpperBody);
	DrawRoundLine(B_RightUpperArm , B_RightLowerArm);
	
	DrawRoundLine(B_LeftUpperArm , B_UpperBody);
	DrawRoundLine(B_LeftUpperArm , B_LeftLowerArm);
    
	DrawRoundLine(B_Pelvic , B_LeftUpperLeg );
	DrawRoundLine(B_LeftUpperLeg , B_LeftLowerLeg);
	//DrawRoundLine(LeftLowerLeg , LeftFoot);
	
	DrawRoundLine(B_Pelvic , B_RightUpperLeg );
	DrawRoundLine(B_RightUpperLeg , B_RightLowerLeg);
	//DrawRoundLine(RightLowerLeg , RightFoot);
#endif
    
    IterateChildBone(Editor->StickMan[B_BodyOrigin]);
    
}

internal void GameUpdate()
{
	bool WindowSizeChanged = false;
	localPersist Vector2 PreviousWindowsSize = {};
    
	if (PreviousWindowsSize.x != AppData->WindowSize.x)
	{
		WindowSizeChanged = true;
	}
    
	if (PreviousWindowsSize.y != AppData->WindowSize.y)
	{
		WindowSizeChanged = true;
	}
    
	PreviousWindowsSize = AppData->WindowSize;
    
	if (WindowSizeChanged)
	{
		//why windows size changed to 0 ???????
		//wrap rlloadtexture 
		if (AppData->WindowSize.x != 0)
		{
			if (AppData->WindowSize.y != 0)
			{
				if (RenderState.ScreenFrameBuffer != 0)
				{
					rlUnloadFramebuffer(RenderState.ScreenFrameBuffer); //do i need to unload you?
				}
                
				RenderState.ScreenFrameBuffer = rlLoadFramebuffer();
                
				if (RenderState.LightingTexture != 0)
				{
					rlUnloadTexture(RenderState.LightingTexture );
				}
                
				RenderState.LightingTexture = rlLoadTexture(0, AppData->WindowSize.x, AppData->WindowSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
                
				if (RenderState.GameWorldTexture != 0)
				{
					rlUnloadTexture(RenderState.GameWorldTexture);
				}
                
				RenderState.GameWorldTexture = rlLoadTexture(0, AppData->WindowSize.x, AppData->WindowSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
                
				if (RenderState.GUITexture != 0)
				{
					rlUnloadTexture(RenderState.GUITexture);
				}
                
				RenderState.GUITexture = rlLoadTexture(0, AppData->WindowSize.x, AppData->WindowSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
                
				if (RenderState.GameWorldDepthTexture != 0)
				{
					rlUnloadTexture(RenderState.GameWorldDepthTexture);
				}				
				RenderState.GameWorldDepthTexture = LoadDepthTexture(AppData->WindowSize.x , AppData->WindowSize.y );
                
				if (RenderState.BloomTexture != 0)
				{
					rlUnloadTexture(RenderState.BloomTexture);
				}
                
				RenderState.BloomTexture = rlLoadTexture(0, AppData->WindowSize.x, AppData->WindowSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
                
				if (RenderState.FirstBlurTexture!= 0)
				{
					rlUnloadTexture(RenderState.FirstBlurTexture);
				}
                
				RenderState.FirstBlurTexture = rlLoadTexture(0, AppData->WindowSize.x, AppData->WindowSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
                
                
				if (RenderState.BlurTexture != 0)
				{
					rlUnloadTexture(RenderState.BlurTexture);
				}
                
				RenderState.BlurTexture = rlLoadTexture(0, AppData->WindowSize.x, AppData->WindowSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
                
			}
		}
	}
    
	GL_CATCH;
    
	Editor->FlatColor = true;
    
	double MouseX = 0;
	double MouseY = 0;
    
	glfwGetCursorPos(AppData->CurrentWindow, &MouseX, &MouseY);
    
	Vector2 MousePosition = { MouseX, MouseY };
    
	AppData->MousePosition = MousePosition;
	AppData->MouseRay3D = GetScreenToWorldRayEx(MousePosition, AppData->GameCamera, AppData->WindowSize.x, AppData->WindowSize.y);
    
	if (!MenuInteruped)
	{
		Editor->TargetCameraZoom -= AppData->MouseScrollDelta * 0.5f;
	}
    
	if (Editor->TargetCameraZoom < 0.01f) Editor->TargetCameraZoom = 0.01f;
	if (Editor->TargetCameraZoom > 20.0f) Editor->TargetCameraZoom = 20.0f;
	
	Quaternion CameraRotation = QuaternionFromEuler(Editor->CameraEuler.x * DEG2RAD, Editor->CameraEuler.y * DEG2RAD,Editor->CameraEuler.z * DEG2RAD);	
    
	Editor->CurrentCameraZoom = Lerp(Editor->CurrentCameraZoom , Editor->TargetCameraZoom , 0.2f);
	//Editor->CurrentCameraZoom = Editor->TargetCameraZoom ;
	
	AppData->GameCamera.position = Vector3RotateByQuaternion({0,0,-Editor->CurrentCameraZoom} , CameraRotation );
	AppData->GameCamera.position = Vector3Add(AppData->GameCamera.position  , Editor->CameraOffset);
	
	AppData->GameCamera.target = Editor->CameraOffset;
    
	AppData->Top = tan((AppData->GameCamera.fovy * 0.5) * DEG2RAD);
	AppData->Right = AppData->Top * (AppData->WindowSize.x / AppData->WindowSize.y);
    
	ViewMatrix = MatrixLookAt(AppData->GameCamera.position, AppData->GameCamera.target, AppData->GameCamera.up);
	ProjectMatrix = MatrixPerspective(AppData->GameCamera.fovy * DEG2RAD, (double)AppData->WindowSize.x / (double)AppData->WindowSize.y, 0.0001, 2000);
    
	MVP_3DWorld = MatrixMultiply(ViewMatrix, ProjectMatrix);
    
    ScreenMVP = MatrixOrtho(-AppData->Right, AppData->Right, -AppData->Top, AppData->Top, 0, 200);
    
	UpdateFloatInput();
    
	ChangeMatrix(ScreenMVP);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);	
    
	MenuInteruped = false;
	Editor_GUI();
    
	ChangeMatrix(MVP_3DWorld);
    
	rlDisableBackfaceCulling();
	
#if 1
	rlFramebufferAttach(RenderState.ScreenFrameBuffer, RenderState.GameWorldTexture, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
	rlFramebufferAttach(RenderState.ScreenFrameBuffer, RenderState.GameWorldDepthTexture , RL_ATTACHMENT_DEPTH ,RL_ATTACHMENT_TEXTURE2D,  0);
    
	if (!rlFramebufferComplete(RenderState.ScreenFrameBuffer)) ASSERT;
	rlEnableFramebuffer(RenderState.ScreenFrameBuffer);
#endif
    
	GL_CATCH;
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
    
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);  	
    
	DrawBackground();
    
	Editor->PreviousMousePosition = Editor->CurrentMousePosition;
	Editor->CurrentMousePosition = AppData->MousePosition;
    
	Vector2 MousePositionDelta = Editor->CurrentMousePosition;
	MousePositionDelta.x -= Editor->PreviousMousePosition.x;
	MousePositionDelta.y -= Editor->PreviousMousePosition.y;
    
	Editor->Right = {ViewMatrix.m0 , ViewMatrix.m4 , ViewMatrix.m8 };
	Editor->Up = { ViewMatrix.m1 , ViewMatrix.m5 , ViewMatrix.m9 };
    
    Editor->BillboardRotation = QuaternionFromVector3ToVector3({0,1,0} , Editor->Up );
    Editor->BillboardRotation = QuaternionMultiply( Editor->BillboardRotation ,  QuaternionFromVector3ToVector3( {1,0,0} , Editor->Right));
    
	if (MouseButtonPressing(MOUSE_BUTTON_MIDDLE))
	{
        
		if (KeyPressing(KEY_LEFT_SHIFT))
		{
			Vector3 MouseRight = Vector3Scale( Editor->Right , -MousePositionDelta.x * 0.005f);
			Vector3 MouseUp = Vector3Scale(Editor->Up , MousePositionDelta.y * 0.005f);
            
			Vector3 MouseDragOffset = Vector3Add(MouseRight , MouseUp);
            
			Editor->CameraOffset = Vector3Add(Editor->CameraOffset ,MouseDragOffset);
		}
		else
		{
			Editor->CameraEuler.y -= MousePositionDelta.x * 0.1;
			Editor->CameraEuler.x += MousePositionDelta.y * 0.1;
		}
        
	}
    
	int GridHintExtend = 15;
    
#if 1
	for (int AxisIndex = 0 ; AxisIndex < 1; AxisIndex++)
	{
        
		Color GridLineColor = Fade(RED,0.2f);
		float GridLineSize = 8;
        
		if (AxisIndex == 1) GridLineColor = Fade(GREEN, 0.2f);
		if (AxisIndex == 2) GridLineColor = Fade(BLUE, 0.2f);
        
		for (int HorizontalIndex = 0; HorizontalIndex < GridHintExtend * 2 +1; HorizontalIndex++)
		{
            
			float LineOffset = GridHintExtend * GridSize;
			int LineIndex = HorizontalIndex - GridHintExtend;
            
			Vector3 StartPosition = {};
            
			if (AxisIndex == 2)
			{
				StartPosition.y -= GridSize * GridHintExtend;        
				StartPosition.y += GridSize * HorizontalIndex;
			}
			else if (AxisIndex == 1)
			{				
				StartPosition.y -= GridSize * GridHintExtend;        
				StartPosition.y += GridSize * HorizontalIndex;
			}
			else
			{
				StartPosition.z -= GridSize * GridHintExtend;        
				StartPosition.z += GridSize * HorizontalIndex;
			}
            
			Vector3 LeftPoint = StartPosition;
			Vector3 RightPoint = StartPosition;
            
			if (AxisIndex == 2)
			{
				LeftPoint.x -= LineOffset;
				RightPoint.x += LineOffset;
			}
			else if (AxisIndex == 1)
			{
				LeftPoint.z -= LineOffset;
				RightPoint.z += LineOffset;
			}
			else
			{
				LeftPoint.x -= LineOffset;
				RightPoint.x += LineOffset;
			}
            
			Color HLineColor = GridLineColor;
			if (HorizontalIndex == GridHintExtend) HLineColor = Fade(BLUE,0.5);
            
			int GridDistance = HorizontalIndex - GridHintExtend;
            
			if (GridDistance < 0) GridDistance *= -1;
            
			HLineColor = Fade(HLineColor, ((float)HLineColor.a /255.0)*(1.0-(float)GridDistance/ (float)GridHintExtend));
            
			DrawRoundLine(StartPosition, LeftPoint,GridLineSize ,HLineColor , Fade(HLineColor ,0));		
			DrawRoundLine(StartPosition, RightPoint ,GridLineSize ,HLineColor ,Fade(HLineColor ,0));
            
		}
        
		for (int VerticalIndex = 0; VerticalIndex < GridHintExtend * 2 + 1; VerticalIndex++)
		{
            
			float LineOffset = GridHintExtend * GridSize;
            
			int LineIndex = VerticalIndex - GridHintExtend;
            
			Vector3 StartPosition = {};
            
			if (AxisIndex == 2)
			{
				StartPosition.x -= GridSize * GridHintExtend;
				StartPosition.x += GridSize * VerticalIndex;
			}
			else if (AxisIndex == 1)
			{				
				StartPosition.z -= GridSize * GridHintExtend;
				StartPosition.z += GridSize * VerticalIndex;
			}
			else
			{
				StartPosition.x -= GridSize * GridHintExtend;
				StartPosition.x += GridSize * VerticalIndex;
			}
            
			Vector3 TopPoint = StartPosition;
			Vector3 BottomPoint = StartPosition;
            
			if (AxisIndex == 2)
			{
				TopPoint.y += LineOffset;        
				BottomPoint.y -= LineOffset;
			}
			else if (AxisIndex == 1)
			{
				TopPoint.y += LineOffset;        
				BottomPoint.y -= LineOffset;
			}
			else
			{
				TopPoint.z += LineOffset;        
				BottomPoint.z -= LineOffset;
			}
            
			Color VLineColor = GridLineColor;        
			if (VerticalIndex == GridHintExtend) VLineColor = Fade(GREEN,0.5f);
            
			int GridDistance = VerticalIndex- GridHintExtend;
            
			if (GridDistance < 0) GridDistance *= -1;
            
			VLineColor = Fade(VLineColor, ((float)VLineColor.a / 255.0) * (1 - (float)GridDistance / (float)GridHintExtend));
            
			DrawRoundLine(StartPosition, TopPoint,GridLineSize ,VLineColor ,Fade(VLineColor ,0));        
			DrawRoundLine(StartPosition, BottomPoint,GridLineSize ,VLineColor,Fade(VLineColor ,0));
            
		}
        
	}
    
#endif
	
    if(KeyPressed(KEY_E))
    {
        Editor->ControlRig = !Editor->ControlRig;
    }
    
	for (int BoneIndex = 0; BoneIndex < BoneCount; BoneIndex++)
	{
        
        _Bone * CurrentBone = Editor->StickMan[BoneIndex];
		_KeyFrame * KeyFrameArray = (_KeyFrame*)CurrentBone->KeyFrameArray;
        
		bool NoInterpolation = true;
		
		CurrentBone->FreeBone = false;
        
        int StartKeyFrameNodeIndex = CurrentBone->FirstKeyFrameOnPointerLeft_OrderListIndex;
        int EndKeyFrameNodeIndex = -1;
        
        if(StartKeyFrameNodeIndex == -1)
        {
            
            StartKeyFrameNodeIndex = GetPreviousNodeIndex(N_NodeTail , &CurrentBone->KeyFrameOrderList);
            EndKeyFrameNodeIndex = GetNextNodeIndex(N_NodeHead , &CurrentBone->KeyFrameOrderList);
            
        }
        else
        {
            EndKeyFrameNodeIndex = GetNextNodeIndex(StartKeyFrameNodeIndex , &CurrentBone->KeyFrameOrderList);
        }
        
        if(EndKeyFrameNodeIndex == -1)
        {
            EndKeyFrameNodeIndex = GetNextNodeIndex(N_NodeHead , &CurrentBone->KeyFrameOrderList);
        }
        
        int TargetKeyFrameIndex = -1;
        
        int StartKeyFrameIndex = GetDataIndexFromNodeIndex(StartKeyFrameNodeIndex , &CurrentBone->KeyFrameOrderList);
        int EndKeyFrameIndex = GetDataIndexFromNodeIndex(EndKeyFrameNodeIndex, &CurrentBone->KeyFrameOrderList);
        
        if(StartKeyFrameIndex != -1)
        {
            
            if(EndKeyFrameIndex == -1)
            {
                EndKeyFrameNodeIndex = GetNextDataIndexFromNodeIndex(N_NodeHead , &CurrentBone->KeyFrameOrderList);
            }
            
            if(KeyFrameArray[StartKeyFrameIndex].FrameIndex == Editor->PointerFrameIndex )
            {
                TargetKeyFrameIndex = StartKeyFrameIndex;
                CurrentBone->FreeBone = true;
            }
            
        }
        else
        {
            if(EndKeyFrameIndex == -1)
            {
                CurrentBone->FreeBone = true;
            }
        }
        
        if(StartKeyFrameIndex != -1) 
        {
            if(EndKeyFrameIndex != -1)
            {
                NoInterpolation = false;
            }
            
            if(StartKeyFrameIndex == EndKeyFrameIndex)
            {
                NoInterpolation = true;
            }
            
        }
        
		if (NoInterpolation)
		{
			if (TargetKeyFrameIndex != -1)
			{
				CurrentBone->State = KeyFrameArray[TargetKeyFrameIndex].BoneState;
			}
            
		}
		else
		{
            
			_KeyFrame * StartKeyFrame= KeyFrameArray + StartKeyFrameIndex;
			_KeyFrame * EndKeyFrame= KeyFrameArray + EndKeyFrameIndex;
            
			float FrameCount = GetKeyFrameCount(StartKeyFrame , EndKeyFrame);
			float FrameOffset = Editor->PointerFrameIndex - StartKeyFrame->FrameIndex;
			
            if(Editor->PointerFrameIndex < StartKeyFrame->FrameIndex)
            {
                FrameOffset = Editor->EndFrameIndex - StartKeyFrame->FrameIndex + Editor->PointerFrameIndex - Editor->StartFrameIndex + 1;
            }
            
            float FrameTime = FrameOffset / FrameCount;
            //printf("%f %f %f\n" , FrameOffset, FrameCount , FrameTime);
            
            Vector3 LineDirection = Vector3Subtract(EndKeyFrame->BoneState.LocalPosition , StartKeyFrame->BoneState.LocalPosition);
            LineDirection =Vector3Scale( Vector3Normalize(LineDirection) , 0.1f);
            
            Vector3 BezierLineStartPosition = StartKeyFrame->BoneState.LocalPosition;
            Vector3 BezierStartPoint = Vector3Add(LineDirection , StartKeyFrame->NextBezierOffset);
            BezierStartPoint = Vector3Add(BezierLineStartPosition , BezierStartPoint);
            
            Vector3 BezierLineEndPosition = EndKeyFrame->BoneState.LocalPosition;
            Vector3 BezierEndPoint = Vector3Add( Vector3Negate(LineDirection) , EndKeyFrame->PreviousBezierOffset);
            BezierEndPoint = Vector3Add(BezierLineEndPosition , BezierEndPoint);
            
			CurrentBone->State.LocalPosition = Vector3CubicBezier(BezierLineStartPosition , BezierStartPoint , BezierEndPoint ,BezierLineEndPosition ,FrameTime );
			
            float EaseTime = EaseInOutCubic(FrameTime);
			CurrentBone->State.LocalRotation = QuaternionLerp(StartKeyFrame->BoneState.LocalRotation , EndKeyFrame->BoneState.LocalRotation ,EaseTime );
            
		}
	}
    
    BoneSelection();
    
    if(!Editor->Playing)
    {
        
        for(int BoneIndex = 0 ; BoneIndex < BoneCount ; BoneIndex++)
        {
            _Bone * CurrentBone = Editor->StickMan[BoneIndex];
            
            _KeyFrame * KeyFrameArray = (_KeyFrame * )CurrentBone->KeyFrameArray;
            
            int KeyFrameCount = 0;
            HashTable_Iterate(KeyFrameIndex , Editor->PointerFrameIndex , &CurrentBone->KeyFrameHashTable , MAX_KEY_FRAME)
            {
                
                KeyFrameArray[KeyFrameIndex].BoneState = CurrentBone->State;
                
                if(KeyFrameCount>0) ASSERT;
                KeyFrameCount++;
            }
            
        }
        
    }
    
    UpdateBoneStructure();
    
    internal void BoneIKUpdate(int TargetBoneIndex , int PoleBoneIndex , int IKBoneIndex , int IterationCount , int BoneChainMaxLength)
    {
        _Bone * TargetBone = Editor->StickMan[TargetBoneIndex];
        _Bone * PoleBone = Editor->StickMan[PoleBoneIndex];
        _Bone * IKBone = Editor->StickMan[IKBoneIndex];
        
        BoneIKUpdate(TargetBone , PoleBone , IKBone , IterationCount , BoneChainMaxLength);
    }
    
    BoneIKUpdate(B_LeftHandController , B_LeftArmPoleTarget , B_LeftLowerArm , 20 , 1 );
    BoneIKUpdate(B_RightHandController , B_RightArmPoleTarget , B_RightLowerArm , 20 , 1 );
    
    BoneIKUpdate(B_LeftLegController  , B_LeftLegPoleTarget, B_LeftLowerLeg , 20 , 1 );
    BoneIKUpdate(B_RightLegController , B_RightLegPoleTarget , B_RightLowerLeg , 20 , 1 );
    
    D_GameDraw();
    
    DrawAllBone();
    
	//something causing it 30 fps when minimize
	//printf("FPS %f : %f\n", 1.0/(EndTimeStamp(PreviousLoopCounter)/(1000.0*1000.0)), 1.0 / (EndTimeStamp(GameCounterStamp) / (1000.0 * 1000.0)));
	RenderState.FakeDepth = 0;    
	D_GameDraw();
	
	glDisable(GL_DEPTH_TEST);
    
	rlDisableFramebuffer();
    
#if 1
	if (Editor->FlatColor)
	{
        
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
		DrawScreenFlat(RenderState.GameWorldTexture , WHITE);
        
	}
	else
	{
        
		rlFramebufferAttach(RenderState.ScreenFrameBuffer, RenderState.LightingTexture, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
		if (!rlFramebufferComplete(RenderState.ScreenFrameBuffer)) ASSERT;
        
		rlEnableFramebuffer(RenderState.ScreenFrameBuffer);
        
#if 0
        
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_DST_ALPHA, GL_ONE, GL_ONE);
		glEnable(GL_BLEND);    
        
		D_Foreach(D_Light, CurrentLight, GameWorld->LightArray)
		{
			Vector4 LightColor = CurrentLight->LightColor;
			LightColor.w *= CurrentLight->LightStrength;
			AddLight(CurrentLight->LightPosition, CurrentLight->LightSize, LightColor);
		}
        
		_ShaderInput * LightShaderInput = AllShaderInputs + S_Point_Light;
        
		rlEnableShader(LightShaderInput->Shader);
		glBindVertexArray(LightShaderInput->VertexArrayObject);
        
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, RenderState.GameWorldTexture);
		glUniform1i(LightShaderInput->UniformLocation[SU_Texture0], 0);
        
		rlSetUniformMatrix(LightShaderInput->UniformLocation[SU_MVP] , MVP_2DWorld);
        
		Update_GPU_VertexBufferAll(LightShaderInput, LightData.LightCount);
        
		glDrawArrays(GL_POINTS, 0, LightData.LightCount);
        
        
		float TargetLightStrength = 0.6f;
        
		localPersist float LightStrength = 0.3f;
		LightStrength = Lerp(LightStrength , TargetLightStrength , 0.36f);
        
		DrawScreenFlat(RenderState.GameWorldTexture, Fade(WHITE, LightStrength ));
        
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
#endif
        
		rlDisableFramebuffer();
        
		LightData.LightCount = 0;
        
		rlFramebufferAttach(RenderState.ScreenFrameBuffer, RenderState.BloomTexture, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
		if (!rlFramebufferComplete(RenderState.ScreenFrameBuffer)) ASSERT;
        
		rlEnableFramebuffer(RenderState.ScreenFrameBuffer);
        
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
		_ShaderInput * BloomShader = AllShaderInputs + S_Bloom;
        
		rlEnableShader(BloomShader->Shader);
		glBindVertexArray(BloomShader->VertexArrayObject);
        
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D , RenderState.LightingTexture);
		glUniform1i(BloomShader->UniformLocation[SU_Texture0], 0);
        
		glUniform2f(BloomShader->UniformLocation[SU_ScreenSize] , AppData->WindowSize.x , AppData->WindowSize.y);
        
		Vector3 * SreenQuadPos = VertexData.Position;
		Vector4 * ScreenQuadColorData = VertexData.Color;
		Vector2 * ScreenQuadTexCoordData = VertexData.TexCoord;
        
		int VertCount = 0;
        
		SreenQuadPos[VertCount++] = { -1, 1, 0 };
		SreenQuadPos[VertCount++] = { 1, 1, 0 };
		SreenQuadPos[VertCount++] = { 1, -1, 0 };
        
		SreenQuadPos[VertCount++] = { -1, -1, 0 };
		SreenQuadPos[VertCount++] = { -1, 1, 0 };
		SreenQuadPos[VertCount++] = { 1, -1, 0 };
        
		VertCount = 0;
        
		ScreenQuadTexCoordData[VertCount++] = { 0, 1 };
		ScreenQuadTexCoordData[VertCount++] = { 1, 1 };
		ScreenQuadTexCoordData[VertCount++] = { 1, 0 };
        
		ScreenQuadTexCoordData[VertCount++] = { 0, 0 };
		ScreenQuadTexCoordData[VertCount++] = { 0, 1 };
		ScreenQuadTexCoordData[VertCount++] = { 1, 0 };
        
		VertCount = 0;
        
		ScreenQuadColorData[VertCount++] = {1,1,1,1};
		ScreenQuadColorData[VertCount++] = {1,1,1,1};
		ScreenQuadColorData[VertCount++] = {1,1,1,1};
        
		ScreenQuadColorData[VertCount++] = {1,1,1,1};
		ScreenQuadColorData[VertCount++] = {1,1,1,1};
		ScreenQuadColorData[VertCount++] = {1,1,1,1};
        
		Update_GPU_VertexBufferAll(BloomShader, 6);    
		glDrawArrays(GL_TRIANGLES, 0, 6);
        
		rlDisableFramebuffer();
        
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
		Update_GPU_VertexBufferAll(BloomShader, 6);
        
		for (int ShaderIndex = S_Blur_H; ShaderIndex <= S_Blur_V; ShaderIndex++)
		{
			unsigned int TargetBlurTexture = RenderState.BlurTexture;
            
			if (ShaderIndex == S_Blur_H) TargetBlurTexture = RenderState.FirstBlurTexture;
            
			rlFramebufferAttach(RenderState.ScreenFrameBuffer, TargetBlurTexture , RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
			if (!rlFramebufferComplete(RenderState.ScreenFrameBuffer)) ASSERT;
            
			rlEnableFramebuffer(RenderState.ScreenFrameBuffer);
            
			_ShaderInput * BlurShader = AllShaderInputs + ShaderIndex;
            
			rlEnableShader(BlurShader ->Shader);
			glBindVertexArray(BlurShader ->VertexArrayObject);
            
			unsigned int BaseTexture = RenderState.FirstBlurTexture;
			if (ShaderIndex == S_Blur_H) BaseTexture = RenderState.BloomTexture;
            
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D , BaseTexture);
			glUniform1i(BlurShader ->UniformLocation[SU_Texture0], 0);
            
			Vector2 ScreenWorld = PixelToWorld(AppData->WindowSize.x , AppData->WindowSize.y);
            
			glUniform2f(BlurShader->UniformLocation[SU_ScreenSize] , 1.0f / AppData->WindowSize.x , 1.0f / AppData->WindowSize.x);
			glUniform1f(BlurShader->UniformLocation[SU_GridSize] ,11);
			glUniform1f(BlurShader->UniformLocation[SU_Spread] , 10);
            
			//i just reusing data already exist in buffer
			Update_GPU_VertexBufferAll(BlurShader , 6);    
			glDrawArrays(GL_TRIANGLES, 0, 6);
            
			rlDisableFramebuffer();
		}
        
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_DST_ALPHA, GL_ONE, GL_ONE);
        
		DrawScreenFlat(RenderState.LightingTexture ,Fade(WHITE,1.0f));
		DrawScreenFlat(RenderState.BlurTexture ,Fade(WHITE,0.4f));
        
		GL_CATCH;
        
	}
#endif
    
	ChangeMatrix(ScreenMVP);
	glBlendFunc(GL_ONE , GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
    
	//i just reusing texture
	//maybe i should reuse more?
	DrawScreenFlat(RenderState.GUITexture , WHITE);
    
    
	D_GameDraw();
    
	RenderState.ElapsedTime += DeltaTime;
    
}

internal GAME_LOOP(GameLoop)
{
    if (ShaderCompileFailed) return;
    
    localPersist double UpdateTimer = 0;
    localPersist double PreviousLoopTime = TimeStamp();
    localPersist double ElapsedTime = 0;
    
    double CurrentTime = TimeStamp();
    ElapsedTime += (CurrentTime - PreviousLoopTime) / (1000.0 * 1000.0);
    
    double UpdateTime = 1.0f / (double)D_UPDATE_RATE;
    
    if (ElapsedTime > UpdateTime * 10) ElapsedTime = 0;
    
    int UpdateCount = 0;
    
    for (;ElapsedTime > UpdateTime ; ElapsedTime -= UpdateTime )
    {   
        glfwPollEvents();
        
        double GameUpdateTime = TimeStamp();
        GameUpdate();
        GameUpdateCount++;
        
        double GameUpateTimeDelta = (TimeStamp()-GameUpdateTime) / (1000.0 * 1000.0);
        
        if (GameUpateTimeDelta > UpdateTime)
        {
            printf( "update is lagging : %f \n", GameUpateTimeDelta );
        }
        
        AppData->FrameTimeMemory.CurrentMemory = AppData->FrameTimeMemory.StartMemory;
        glfwSwapBuffers(AppData->CurrentWindow);
        
    }
    
    PreviousLoopTime = CurrentTime;
    
    
    //these things doesn't help
    //it still lock on 30 fps
    
    //glFinish();
    
    //this things freeze opengl ?
    //SwapBuffers(AppData->_HDC);
}

internal GAME_UNLOAD(GameUnload)
{
    mkdir(GetAppFilePath("Game"));
    
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    glfwSwapBuffers(AppData->CurrentWindow);
    //SwapBuffers(AppData->_HDC);
	
    if (!ShaderCompileFailed)
    {
        SaveData(GetAppFilePath(GameStateSaveName),&AppData->GameEditorData);
        
    }
    
    for (int TextureIndex = 0; TextureIndex < RenderState.TextureCount; TextureIndex++)
    {
        if (RenderState.AllTextures[TextureIndex].id != 0)
        {
            UnloadTexture(RenderState.AllTextures[TextureIndex]);
        }
    }
    
    if (RenderState.ScreenFrameBuffer != 0)
    {
        rlUnloadFramebuffer(RenderState.ScreenFrameBuffer); //do i need to delete you?
        
        rlUnloadTexture(RenderState.GameWorldTexture);
		rlUnloadTexture(RenderState.GUITexture);
		rlUnloadTexture(RenderState.GameWorldDepthTexture);
        rlUnloadTexture(RenderState.LightingTexture);
        rlUnloadTexture(RenderState.BloomTexture);
        rlUnloadTexture(RenderState.FirstBlurTexture);
        rlUnloadTexture(RenderState.BlurTexture);
    }
    
    glDeleteTextures(1, &AppData->DefaultFont.GlyphAtlasTexture.id);
    
    for (int BufferIndex = 0; BufferIndex < ShaderBufferCount; BufferIndex++)
    {
        glDeleteBuffers(1, &All_GPU_Buffers[BufferIndex]->BufferHandle);
    }
    
    for (int ShaderIndex = 0; ShaderIndex < S_COUNT; ShaderIndex++)
    {
        glDeleteProgram(AllShaderInputs[ShaderIndex].Shader);
    }
    
    free(AppData->FrameTimeMemory.StartMemory);
    free(AppData->RunTimeMemory.StartMemory);
    free(PersistMemoryBlock);
    
}

internal void LoadTextureFolder()
{
    
    FILE* GameTextureDataFile = fopen(GetAppFilePath(GameTextureDataName), "rb");
	
    bool TryLoadPreviousTextureIndex = false;
    RenderState.TextureCount = 2;
    RenderState.AllTexturesName = AllocateTemp(String, MAX_TEXTURE);
    RenderState.AllTextures = AllocateTemp(Texture, MAX_TEXTURE);
    
    if (GameTextureDataFile)
    {
        fread(&RenderState.TextureCount, sizeof(int), 1, GameTextureDataFile);
        
        for (int TextureIndex = 0; TextureIndex < RenderState.TextureCount; TextureIndex++)
        {
            fread(&RenderState.AllTexturesName[TextureIndex].Count, sizeof(int), 1, GameTextureDataFile);
            RenderState.AllTexturesName[TextureIndex].Start = AllocateTemp(wchar_t, RenderState.AllTexturesName[TextureIndex].Count);
            fread(RenderState.AllTexturesName[TextureIndex].Start , sizeof(wchar_t), RenderState.AllTexturesName[TextureIndex].Count, GameTextureDataFile);
        }
        
        fclose(GameTextureDataFile);
    }
    
    Image DefaultWhiteImage = GenImageColor(20, 20, WHITE);
    RenderState.AllTextures[0] = LoadTextureFromImage(DefaultWhiteImage);
    RenderState.AllTexturesName[0].Start = L"無貼圖";
    
    Image MissingTexture = GenImageCellular(20, 20, 2);
    RenderState.AllTextures[1] = LoadTextureFromImage(MissingTexture);
    RenderState.AllTexturesName[1].Start = L"";
    RenderState.AllTexturesName[1].Start = L"ERROR_TEXTURE";
    
#define TEXTURE_META_HASH 16
    TextureMetaNode * TextureMetaNodeHash[TEXTURE_META_HASH] = {};
    
    for (int TextureIndex = 0; TextureIndex < RenderState.TextureCount; TextureIndex++)
    {
        TextureMetaNode ** NodePointer = TextureMetaNodeHash + (StringToHash(RenderState.AllTexturesName[TextureIndex].Start) % TEXTURE_META_HASH);
        
        for (; (*NodePointer); NodePointer = &(*NodePointer)->Next);
        
        TextureMetaNode* NewNode = AllocateTemp(TextureMetaNode, 1);
        (*NodePointer) = NewNode;
        
        NewNode->Index = TextureIndex;
    }
    
    FilePathList TexturePathList = LoadDirectoryFilesEx(GetAppFilePath("Asset\\Texture"), ".png", false);
	
    for (int PathIndex = 0; PathIndex < TexturePathList.count; PathIndex++)
    {
        char * TexturePath = TexturePathList.paths[PathIndex];
        const char * FileName = GetFileName(TexturePath);
        
        wchar_t FileNameString[256] = {};
        
        //can't convert anything beside english
        for (int CharIndex = 0; FileName[CharIndex]; CharIndex++) FileNameString[CharIndex] = (int)FileName[CharIndex];
        
        TextureMetaNode ** NodePointer = TextureMetaNodeHash + (StringToHash(FileNameString) % TEXTURE_META_HASH);
        
        for (; (*NodePointer) && (wcscmp(RenderState.AllTexturesName[(*NodePointer)->Index].Start , FileNameString)); NodePointer = &(*NodePointer)->Next);
        
        if (*NodePointer)
        {
            int TextureIndex = (*NodePointer)->Index;
            RenderState.AllTextures[TextureIndex] = LoadTexture(TexturePath);
        }
        else
        {
            RenderState.AllTextures[RenderState.TextureCount] = LoadTexture(TexturePath);
            
            int CharIndex = 0;
            for (; FileNameString[CharIndex]; CharIndex++);
            
            RenderState.AllTexturesName[RenderState.TextureCount].Count = CharIndex + 1;
            RenderState.AllTexturesName[RenderState.TextureCount].Start = AllocateTemp(wchar_t, CharIndex + 2);
            
            for (int CharIndex = 0; FileNameString[CharIndex]; CharIndex++)
                RenderState.AllTexturesName[RenderState.TextureCount].Start[CharIndex] = FileNameString[CharIndex];
            
            RenderState.TextureCount++;
        }
    }
    
    UnloadDirectoryFiles(TexturePathList);
}

internal void MakeAStickMan()
{
    printf("\n");
    
    //TODO : patch enum?
    //thing is messy
    //it seem the data moved to the wrong place
    //maybe it isn't ? just the pointer being written over i think
    
    for(;;)
    {
        bool AllMatch = true;
        printf("\n");
        
        for(int BoneIndex = 0 ; BoneIndex < BoneCount ; BoneIndex++)
        {
            char** BoneNames = (char **)EnumToString(BoneTag);
            
            char * OldBoneName = Editor->StickMan[BoneIndex]->BoneName;
            char * CurrentBoneName = BoneNames[BoneIndex];
            
            if(strcmp(OldBoneName , CurrentBoneName))
            {
                bool MissingBone = true;
                
                for(int OldBoneIndex = 0 ; OldBoneIndex < BoneCount ; OldBoneIndex++)
                {
                    if(!strcmp(OldBoneName , BoneNames[OldBoneIndex]))
                    {
                        AllMatch = false;
                        
                        printf("Bone swaped : %s <-> %s\n" , OldBoneName , CurrentBoneName);
                        
                        _Bone persist * TempBone = Editor->StickMan[OldBoneIndex];
                        
                        Editor->StickMan[OldBoneIndex] = Editor->StickMan[BoneIndex];
                        Editor->StickMan[BoneIndex] = TempBone;
                        
                        Editor->StickMan[BoneIndex]->BoneIndex = BoneIndex;
                        Editor->StickMan[OldBoneIndex]->BoneIndex = OldBoneIndex;
                        
                        MissingBone = false;
                        break;
                    }
                }
                
                if(MissingBone)
                {
                    printf("Bone missing : %s\n" , OldBoneName);
                }
            }
            
            sprintf(Editor->StickMan[BoneIndex]->BoneName , CurrentBoneName);
            
        }
        
        if(AllMatch) break;
        
    }
    
    for(int BoneIndex = 0 ; BoneIndex < BoneCount ; BoneIndex++)
    {
        char** BoneNames = (char **)EnumToString(BoneTag);
        
        char * OldBoneName = Editor->StickMan[BoneIndex]->BoneName;
        char * CurrentBoneName = BoneNames[BoneIndex];
        
        if(strcmp(OldBoneName , CurrentBoneName))
        {
            printf("something wrong\n");
        }
        
        if(BoneIndex != Editor->StickMan[BoneIndex]->BoneIndex)
        {
            Editor->StickMan[BoneIndex]->BoneIndex = BoneIndex;
            printf("wrong index \n");
        }
    }
    
    Editor->StickMan[B_Origin]->State.Direction = {0,0.0f,0};
    Editor->StickMan[B_UpperBody]->State.Direction = {0,0.6f,0};
    Editor->StickMan[B_LowerBody]->State.Direction = {0,0.3f,0};
    Editor->StickMan[B_Pelvic]->State.Direction = {0,0.5f,0};
    Editor->StickMan[B_Head]->State.Direction = {0,0.3,0};
    
    Editor->StickMan[B_Origin]->FreePosition = true;
    Editor->StickMan[B_BodyOrigin]->FreePosition = true;
    Editor->StickMan[B_LeftArmPoleTarget]->FreePosition = true;
    Editor->StickMan[B_LeftHandController]->FreePosition = true;
    Editor->StickMan[B_RightHandController]->FreePosition = true;
    Editor->StickMan[B_RightArmPoleTarget]->FreePosition = true;
    
    Editor->StickMan[B_LeftLegPoleTarget]->FreePosition = true;
    Editor->StickMan[B_LeftLegController]->FreePosition = true;
    Editor->StickMan[B_RightLegController]->FreePosition = true;
    Editor->StickMan[B_RightLegPoleTarget]->FreePosition = true;
    
    ClearList(&Editor->RootBoneList);
	AddToListTail(B_Origin , &Editor->RootBoneList , BoneCount);
    
    ClearHashTable(&Editor->BoneChildrenHashTable , BoneCount);
    ClearHashTable(&Editor->BoneParentHashTable, BoneCount);
    
	internal void AttachChildBone(int BoneIndex , int ChildBoneIndex)
    {
        AddToHashTable(ChildBoneIndex , BoneIndex , &Editor->BoneParentHashTable , BoneCount);
        AddToHashTable(BoneIndex , ChildBoneIndex , &Editor->BoneChildrenHashTable , BoneCount);
    };
    
#if 1
	
    AttachChildBone(B_Origin ,B_LeftLegController);
    AttachChildBone(B_Origin ,B_LeftLegPoleTarget);
    
    AttachChildBone(B_Origin ,B_RightLegController);
    AttachChildBone(B_Origin ,B_RightLegPoleTarget);
    
    AttachChildBone(B_Origin ,B_LeftHandController);
    AttachChildBone(B_Origin ,B_LeftArmPoleTarget);
    
    AttachChildBone(B_Origin ,B_RightHandController);
    AttachChildBone(B_Origin ,B_RightArmPoleTarget);
    
    AttachChildBone(B_Origin ,B_BodyOrigin);
    
    AttachChildBone(B_UpperBody , B_RightUpperArm);
	AttachChildBone(B_UpperBody , B_LeftUpperArm);
	
	AttachChildBone(B_RightUpperArm , B_RightLowerArm);
	AttachChildBone(B_RightLowerArm , B_RightHand);
	
	AttachChildBone(B_LeftUpperArm , B_LeftLowerArm);
	AttachChildBone(B_LeftLowerArm , B_LeftHand);
	
	AttachChildBone(B_Pelvic , B_LeftUpperLeg);
	AttachChildBone(B_LeftUpperLeg , B_LeftLowerLeg);
	AttachChildBone(B_LeftLowerLeg , B_LeftFoot);
	
	AttachChildBone(B_Pelvic, B_RightUpperLeg);
	AttachChildBone(B_RightUpperLeg , B_RightLowerLeg);
	AttachChildBone(B_RightLowerLeg , B_RightFoot);
    
	AttachChildBone(B_BodyOrigin , B_LowerBody);
	AttachChildBone(B_BodyOrigin , B_Pelvic);
    
	AttachChildBone(B_LowerBody , B_UpperBody);
	AttachChildBone(B_UpperBody , B_Head);
#endif
}

internal void GameInit()
{
	AppData->GameCamera.target = {};
	AppData->GameCamera.position = { 0,0,-1 };
    
    timeBeginPeriod(1);
    glfwSwapInterval(0);
    
    CreateStructNameStringHash();
	
    CreateAWholeNewWorld();
	
    RenderStateInit();	
    ShaderInit();
    
    if (ShaderCompileFailed) return;
    
	double PatchTime = TimeStamp();
	printf("\npatch start\n");
    
    LoadData(GetAppFilePath(GameStateSaveName) , &AppData->GameEditorData);
    
    PatchTime = TimeStamp() - PatchTime;
	printf("patch finished (%f) \n\n" , PatchTime / (1000.0 * 1000.0));
    
    LoadTextureFolder();
    MakeAStickMan();
}

extern "C" GAME_LOAD(GameLoad)
{
    double GameLoadTime = TimeStamp();
    
    AppData = _AppData;
    AppData->_GameLoop = GameLoop;
    AppData->_GameUnload = GameUnload;
    
    AppData->FrameTimeMemory = {};
    AppData->FrameTimeMemory.Size = 1024 * 1024 * 128;
    AppData->FrameTimeMemory.StartMemory = (unsigned char*)malloc(AppData->FrameTimeMemory.Size);
    AppData->FrameTimeMemory.CurrentMemory = AppData->FrameTimeMemory.StartMemory;
    
    AppData->RunTimeMemory = {};
    AppData->RunTimeMemory.Size = 1024 * 1024 * 128;
    AppData->RunTimeMemory.StartMemory = (unsigned char*)malloc(AppData->RunTimeMemory.Size);
    AppData->RunTimeMemory.CurrentMemory = AppData->RunTimeMemory.StartMemory;
    
    AppData->GameEditorData = {};
    AppData->GameEditorData.Size = 1024 * 1024 * 128;
    
    PersistMemoryBlock = (unsigned char*)malloc(AppData->GameEditorData.Size );
    
    _StartMemory_ = PersistMemoryBlock;
    
    AppData->GameEditorData.StartMemoryAddress = PersistMemoryBlock;
    AppData->GameEditorData.StartOffset = 0;
    AppData->GameEditorData.CurrentOffset = AppData->GameEditorData.StartOffset;
    
    GameInit();
    
    printf("\nGame Load Time: %f\n" , (TimeStamp() - GameLoadTime) / (1000.0 * 1000.0));
    
}