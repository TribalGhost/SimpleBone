#include "d_header.h"
#include "d_main.h"

#include "d_renderdata.c"
#include "d_gamedata.c"

#include "d_game_meta_generated.c"

#include "d_function.c"

#include "d_render.c"
#include "d_text.c"

#include "d_gamefunction.c"
#include "d_blender_file.h"
#include "d_interface.c"

internal bool check_selected_bone_rotation( Bone * final_bone_array_copy, int single_bone_index , Clip * clip_to_assign)
{
    Bone * selected_bone = final_bone_array_copy + single_bone_index;
    
    Rect rotation_rect[R_count] = {};
    RayCollision rotation_rect_collision[R_count] = {};
    
    rotation_rect[R_Z] = get_rect();
    rotation_rect[R_Z].size = (Vector2){ 1, 1};
    rotation_rect[R_Z].position = selected_bone->position;
    rotation_rect[R_Z].rotation = selected_bone->rotation;
    
    rotation_rect[R_Y] = rotation_rect[R_Z];
    rotation_rect[R_Y].rotation = QuaternionMultiply(rotation_rect[R_Z].rotation , QuaternionFromEuler(DEG2RAD * 90,0,0));
    
    rotation_rect[R_X] = rotation_rect[R_Z];
    rotation_rect[R_X].rotation = QuaternionMultiply(rotation_rect[R_Z].rotation , QuaternionFromEuler(0,DEG2RAD * 90,0));
    
    bool hovering = false;
    for(int axis_index = R_Z ; axis_index < R_count ; axis_index++)
    {
        rotation_rect_collision[axis_index] = get_collision_rect_3D_B(rotation_rect[axis_index]);
        if(rotation_rect_collision[axis_index].hit)
        {
            hovering = true;
        }
    }
    
    local_persist Vector3 previous_drag_point = {};
    local_persist int dragging_axis = R_None;
    
    if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
    {
        dragging_axis = R_None;
    }
    
    RayCollision closest_collision = {};
    closest_collision.distance = FLT_MAX;
    
    Color axis_clor = {};
    if(dragging_axis == R_Z) axis_clor = BLUE;
    if(dragging_axis == R_Y) axis_clor = GREEN;
    if(dragging_axis == R_X) axis_clor = RED;
    
    draw_rect_line_E(rotation_rect[dragging_axis] , axis_clor , 20);
    
    for(int axis_index = R_Z ; axis_index < R_count ; axis_index++)
    {
        bool found_closest_axis = true;
        
        if(!rotation_rect_collision[axis_index].hit) found_closest_axis = false;
        if(rotation_rect_collision[axis_index].distance > closest_collision.distance) found_closest_axis = false;
        
        if(found_closest_axis)
        {
            
            if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
            {
                previous_drag_point = rotation_rect_collision[axis_index].point;
                dragging_axis = axis_index;
            }
            
            closest_collision = rotation_rect_collision[axis_index];
        }
    }
    
    if(!mouse_button_pressing(MOUSE_BUTTON_LEFT))
    {
        for(int axis_index = R_Z ; axis_index < R_count ; axis_index++)
        {
            draw_rect_line_E(rotation_rect[axis_index] , Fade(WHITE , 0.2 ) , 20);
        }
        
        dragging_axis = R_None;
        
    }
    else
    {
        
        Vector3 hit_point = mouse_on_plane(selected_bone->position);
        
        Rect axis_rect = rotation_rect[dragging_axis];
        
        axis_rect.size = (Vector2){10000.0,10000.0};
        
        hit_point = get_collision_rect_3D_B(axis_rect).point;
        
        Vector3 bone_position = axis_rect.position;
        
        Quaternion rotation_offset = QuaternionIdentity();
        
        Quaternion invert_axis_rotation = QuaternionInvert(axis_rect.rotation);
        
        Vector3 direction_to_previous_point = Vector3Subtract(previous_drag_point , bone_position);
        direction_to_previous_point = Vector3RotateByQuaternion(direction_to_previous_point , invert_axis_rotation);
        
        Vector3 direction_to_point = Vector3Subtract(hit_point , bone_position);
        direction_to_point = Vector3RotateByQuaternion(direction_to_point , invert_axis_rotation);
        
        float previous_angle = atan2f(direction_to_previous_point.y , direction_to_previous_point.x);
        float current_angle = atan2f(direction_to_point.y , direction_to_point.x);
        
        Vector3 axis = {};
        
        if (dragging_axis == R_Z) axis = (Vector3){0,0,1};
        if (dragging_axis == R_Y) axis = (Vector3){0,-1,0};
        if (dragging_axis == R_X) axis = (Vector3){1,0,0};
        
        rotation_offset = QuaternionFromAxisAngle(axis, current_angle - previous_angle);
        
        selected_bone->state.local_rotation = QuaternionMultiply(selected_bone->state.local_rotation, rotation_offset);
        
        if(!editor->playing)
        {
            if(editor->selected_clip_index != -1)
            {
                //_Clip * ClipToAssign = ClipArray[editor->SelectedClipIndex];
                
                if(editor_type == edit_base_pose)
                {
                    selected_model->all_bones[single_bone_index].state.local_rotation = QuaternionMultiply(selected_model->all_bones[single_bone_index].state.local_rotation , rotation_offset);
                }
                
                int key_frame_count = 0;
                hash_table_iterate(key_frame_index , GetKeyFrameHash(editor->current_frame_at_timeline , single_bone_index) , &clip_to_assign->key_frame_hash_table)
                {
                    KeyFrame * key_frame = all_key_frame + key_frame_index;
                    if(key_frame->bone_index != single_bone_index) continue;
                    if(key_frame->frame_index != editor->current_frame_at_timeline) continue;
                    
                    key_frame->bone_state.local_rotation = QuaternionMultiply(key_frame->bone_state.local_rotation , rotation_offset);
                    
                    if(key_frame_count>0) CATCH;
                    key_frame_count++;
                }
                
            }
            
        }
        
        //SelectedBone->State.local_rotation = QuaternionNormalize(SelectedBone->State.local_rotation);
        //SelectedBone->State.local_rotation = QuaternionIdentity();
        
        //printf("Hit point : %f %f %f\n" ,HitPoint.x , HitPoint.y , HitPoint.z);
        
        previous_drag_point = hit_point;
    }
    
    return hovering;
}

//a bit crazy
//matrix did changed here
internal void bone_selection_and_edit_bone_state( int current_frame_index)
{
    
    if(key_pressed(KEY_E))
    {
        editor->control_rig = !editor->control_rig;
    }
    
    //this thing is messy
    local_persist bool redrag_selection = true;
    
    if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
    {
        redrag_selection = true;
    }
    
    bool curve_visualization = true;
    
    if(editor_type != edit_animation) curve_visualization = false;
    if(editor->turn_off_bezier_curve) curve_visualization = false;
    if(clip_bone_stack_count == 0) curve_visualization = false;
    
    if(curve_visualization)
    {
        editor->selecting_bezier = false;
        
        local_persist bool interacting_with_bezier_curve = false;
        local_persist bool modify_key_frame_position_only = false;
        local_persist bool selected_previous_bezier_point = false;
        local_persist KeyFrame * selected_bezier_key_frame = 0;
        local_persist KeyFrame * selected_bezier_other_key_frame = 0;
        local_persist Vector3 selected_bezier_base_bone_offset = {};
        local_persist Quaternion selected_bezier_base_bone_rotation = {};
        //local_persist _Bone * SelectedBezierBone = 0;
        
        for(int select_stack_index = 0 ; select_stack_index < editor->selected_bone_count ; select_stack_index++)
        {
            BoneSelection * bone_selection = editor->selected_bone_stack + select_stack_index;
            ClipBone * selected_clip_bone = clip_bone_stack + bone_selection->clip_bone_stack_index;
            Clip * selected_clip = clip_array[bone_selection->clip_index];
            
            int selected_bone_index = bone_selection->bone_index;
            
            Bone * selected_final_bone = selected_clip_bone->final_bone_pose + selected_bone_index;
            Bone * selected_offset_bone = selected_clip_bone->bone_pose_offset + selected_bone_index;
            
            int previous_key_frame_index = -1;
            hash_table_iterate_reverse(key_frame_index , selected_bone_index , &selected_clip->key_frame_hash_table_by_bone)
            {
                previous_key_frame_index = key_frame_index;
                break;
            }
            
            KeyFrame * previous_key_frame = all_key_frame + previous_key_frame_index;
            
			Color key_frame_line_color = Fade(WHITE , 0.2f);
            
            //TODO : this isn't the correct path
            //this only take account the bone pose offset in current frame
            //to get the correct path you need to account other bone and keyframe offset
            //which mean you need have know every bone pose on every frame 
            
            //i haven't account for rotation yet
            
            //the rotation is only half done
            //origin doesn't work at all
            
            //draw_bezier_path
            hash_table_iterate(key_frame_index , selected_bone_index , &selected_clip->key_frame_hash_table_by_bone)
            {
                
                KeyFrame * key_frame = all_key_frame + key_frame_index;
                if(key_frame->bone_index != selected_bone_index) continue;
                
                Quaternion base_bone_rotation = selected_final_bone->rotation;
                Vector3 base_bone_offset =  Vector3RotateByQuaternion(selected_offset_bone->state.local_position , base_bone_rotation);
				Vector3 base_bone_position = Vector3Subtract( selected_final_bone->position , base_bone_offset);
                
                //DrawArrowRay(SelectedFinalBone->position , Vector3Negate(BaseBoneOffset) , RED);
                
                Vector3 previous_key_frame_offset_position = Vector3RotateByQuaternion(previous_key_frame->bone_state.local_position , base_bone_rotation);
                Vector3 previous_key_frame_position = Vector3Add( base_bone_position , previous_key_frame_offset_position);
                
				Vector3 key_frame_offset_position = Vector3RotateByQuaternion(key_frame->bone_state.local_position , base_bone_rotation);
                Vector3 key_frame_position = Vector3Add( base_bone_position , key_frame_offset_position);
                
                //DrawRoundLine(BaseBoneposition , KeyFrameposition , 5 , RED , WHITE);
                //DrawRoundLine(SelectedFinalBone->position , KeyFrameposition , 5 , GREEN , WHITE);
                
                Vector3 line_direction = Vector3Subtract(key_frame_position , previous_key_frame_position);
                line_direction = Vector3Scale(Vector3Normalize(line_direction),0.1f);
                
                Vector3 previous_key_frame_bezier_point = Vector3Add(line_direction , Vector3RotateByQuaternion(previous_key_frame->next_bezier_offset , base_bone_rotation));
                previous_key_frame_bezier_point = Vector3Add(previous_key_frame_bezier_point , previous_key_frame_position);
                
                Vector3 key_frame_bezier_point = Vector3Add(Vector3Negate(line_direction) , Vector3RotateByQuaternion(key_frame->previous_bezier_offset , base_bone_rotation));
                key_frame_bezier_point = Vector3Add(key_frame_bezier_point , key_frame_position);
                
                draw_round_line_B(previous_key_frame_position , previous_key_frame_bezier_point , 10 , Fade(BLUE,0.5f) );
                draw_round_line_B(key_frame_position , key_frame_bezier_point , 10 , Fade(RED,0.5f) );
                
                Rect previous_bezier_point_rect = get_billboard_rect(previous_key_frame_bezier_point , 0.1);
                Rect bezier_point_rect = get_billboard_rect(key_frame_bezier_point , 0.1);
                
                Color previous_bezier_point_color = Fade(BLUE , 0.5f);
                Color bezier_point_color = Fade(RED , 0.5f);
                Color key_frame_point_color = Fade(YELLOW , 0.5f);
                
                if(check_collision_rect_3D(previous_bezier_point_rect))
                {
                    
                    previous_bezier_point_color = BLUE;
                    if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
                    {
                        
                        interacting_with_bezier_curve = true;
                        selected_bezier_key_frame = previous_key_frame;
                        selected_bezier_other_key_frame = key_frame;
                        selected_bezier_base_bone_offset = base_bone_position;
                        selected_bezier_base_bone_rotation = base_bone_rotation;
                        selected_previous_bezier_point = false;
                        
                    }
                    
                }
                
                if(check_collision_rect_3D(bezier_point_rect))
                {
                    bezier_point_color = RED;
                    if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
                    {
                        
                        interacting_with_bezier_curve = true;
                        selected_bezier_key_frame = key_frame;
                        selected_bezier_other_key_frame = previous_key_frame;
                        selected_bezier_base_bone_offset = base_bone_position;
                        selected_bezier_base_bone_rotation = base_bone_rotation;
                        selected_previous_bezier_point = true;
                        
                    }
                }
                
                draw_circle( previous_bezier_point_rect , previous_bezier_point_color);
                draw_circle( bezier_point_rect , bezier_point_color);
                
                int frame_count = get_key_frame_count(previous_key_frame , key_frame ,editor->start_frame_index  , editor->timeline_frame_length );
                
                for(int frame_index = 0 ; frame_index < frame_count ; frame_index++)
                {
                    float bezier_start_time = ((float)frame_index) / ((float)frame_count);
                    float bezier_end_time = ((float)(frame_index+1)) / ((float)frame_count);
                    
                    Vector3 bezier_start_point = vector3_cubic_bezier(previous_key_frame_position , previous_key_frame_bezier_point , key_frame_bezier_point, key_frame_position  , bezier_start_time);
                    Vector3 bezier_end_point = vector3_cubic_bezier(previous_key_frame_position , previous_key_frame_bezier_point , key_frame_bezier_point, key_frame_position  , bezier_end_time);
                    
                    //BezierStartPoint = Vector3RotateByQuaternion(BezierStartPoint , QuaternionInvert(BaseBonerotation));
                    //BezierEndPoint = Vector3RotateByQuaternion(BezierEndPoint , QuaternionInvert(BaseBonerotation));
                    
                    //BezierStartPoint = Vector3Add(BezierStartPoint , BaseBoneposition);
                    //BezierEndPoint = Vector3Add(BezierEndPoint , BaseBoneposition);
                    
                    draw_round_line_B(bezier_start_point , bezier_end_point ,10 ,key_frame_line_color );
                }
                
                draw_round_line_B(previous_key_frame_position , key_frame_position,10 ,Fade(WHITE , 0.05f));
                
                Rect key_frame_rect = get_billboard_rect(key_frame_position , 0.1 );
                
                if(check_collision_rect_3D(key_frame_rect))
                {
                    key_frame_point_color = YELLOW;
                    
                    if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
                    {
                        
                        interacting_with_bezier_curve = true;
                        modify_key_frame_position_only = true;
                        selected_bezier_base_bone_offset = base_bone_position;
                        selected_bezier_base_bone_rotation = base_bone_rotation;
                        selected_bezier_key_frame = key_frame;
                        
                    }
                }
                
                draw_circle(key_frame_rect , key_frame_point_color);
                
                previous_key_frame_index = key_frame_index;
                previous_key_frame = key_frame;
            }
            
        }
        
        if(mouse_button_released(MOUSE_BUTTON_LEFT))
        {
            modify_key_frame_position_only = false;
            
            interacting_with_bezier_curve = false;
        }
        
        if(interacting_with_bezier_curve)
        {
            
            local_persist Vector3 dragging_plane_point = {};
            local_persist Vector3 previous_dragging_point = {};
            
            editor->selecting_bezier = true;
            
            if(modify_key_frame_position_only)
            {
                
                if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
                {
                    stop_mouse_input = true;
                    
                    redrag_selection = false;
                    //DraggingPlanePoint = SelectedBezierKeyFrame->bone_state.local_position;
                    dragging_plane_point = Vector3Add( Vector3RotateByQuaternion(selected_bezier_key_frame->bone_state.local_position,selected_bezier_base_bone_rotation) , selected_bezier_base_bone_offset);
                    previous_dragging_point = dragging_plane_point;
                }
                
                //TODO : it just snap to the point instead adding offset
                Vector3 dragging_point = get_collision_quad_3D(get_billboard_quad(dragging_plane_point , 100000,100000)).point;
                
                Vector3 new_key_frame_offset = Vector3Subtract(dragging_point , previous_dragging_point);
                previous_dragging_point = dragging_point;
                new_key_frame_offset = Vector3RotateByQuaternion(new_key_frame_offset , QuaternionInvert(selected_bezier_base_bone_rotation));
                
                selected_bezier_key_frame->bone_state.local_position = Vector3Add(selected_bezier_key_frame->bone_state.local_position , new_key_frame_offset);
            }
            else
            {
                
                Vector3 start_point = {};
                Vector3 end_point = {};
                Vector3 bezier_offset = {};
                
                if(selected_previous_bezier_point)
                {
                    bezier_offset = selected_bezier_key_frame->previous_bezier_offset;
                }
                else
                {
                    bezier_offset = selected_bezier_key_frame->next_bezier_offset;
                }
                
                bezier_offset = Vector3RotateByQuaternion(bezier_offset , selected_bezier_base_bone_rotation);
                
                start_point = Vector3RotateByQuaternion(selected_bezier_key_frame->bone_state.local_position , selected_bezier_base_bone_rotation );
                end_point = Vector3RotateByQuaternion(selected_bezier_other_key_frame->bone_state.local_position , selected_bezier_base_bone_rotation );
                
                Vector3 direction = Vector3Subtract(end_point , start_point);
                direction = Vector3Scale(Vector3Normalize(direction),0.1f);
                
                Vector3 bezier_direction = Vector3Add(direction , bezier_offset);
                //BezierDirection = Vector3RotateByQuaternion(BezierDirection , SelectedBezierBaseBonerotation);
                
                Vector3 bezier_start = Vector3Add(Vector3RotateByQuaternion(selected_bezier_key_frame->bone_state.local_position , selected_bezier_base_bone_rotation) , selected_bezier_base_bone_offset);
                Vector3 bezier_end = Vector3Add(bezier_start , bezier_direction );
                
                Rect bezier_end_rect = get_billboard_rect(bezier_end , 0.1);
                
                if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
                {
                    redrag_selection = false;
                    stop_mouse_input = true;
                    
                    dragging_plane_point = bezier_end;
                    previous_dragging_point = bezier_end;
                }
                
                //TODO : it just snap to the point instead adding offset
                Vector3 dragging_point = get_collision_quad_3D(get_billboard_quad(dragging_plane_point , 100000,100000)).point;
                Vector3 new_bezier_end_point = Vector3Add(bezier_end ,Vector3Subtract(dragging_point , previous_dragging_point));
                previous_dragging_point = dragging_point;
                Vector3 new_bezier_offset = Vector3Subtract(new_bezier_end_point , bezier_start);
                new_bezier_offset = Vector3Subtract(new_bezier_offset , direction);
                
                new_bezier_offset = Vector3RotateByQuaternion(new_bezier_offset , QuaternionInvert(selected_bezier_base_bone_rotation));
                
                if(selected_previous_bezier_point)
                {
                    selected_bezier_key_frame->previous_bezier_offset = new_bezier_offset;
                }
                else
                {
                    selected_bezier_key_frame->next_bezier_offset = new_bezier_offset;
                }
                
                draw_circle(bezier_end_rect , YELLOW);
                draw_round_line_B( bezier_start , bezier_end , 20  , YELLOW );
            }
            
        }
        
    }
    
    //hated it
    //terrible control
    
    //SingleSelectionAndMultiDrag
    local_persist bool dragging_bone_position = false;
    local_persist Vector3 previous_drag_point ={};
	local_persist Vector3 drag_plane_position = {};
    
    if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
    {
        dragging_bone_position = false;
    }
    
    if(mouse_button_released(MOUSE_BUTTON_LEFT))
    {
        dragging_bone_position = false;
    }
    
    pruning_3D_line = false;
    change_matrix(viewport_screen_pixel_matrix);
    
    for(int clip_bone_stack_index = 0 ; clip_bone_stack_index  < clip_bone_stack_count ; clip_bone_stack_index++)
    {
        ClipBone * clip_bone = clip_bone_stack + clip_bone_stack_index;
        
        for(int bone_index = 0 ; bone_index < selected_model->bone_count ; bone_index++)
        {
            
            clip_bone->hovered_bone[bone_index] = false;
            
            Bone * final_bone = clip_bone->final_bone_pose + bone_index;
			Vector3 bone_interact_point ={};
            
            if(editor->control_rig)
            {
                if(!is_rig(bone_index)) continue;
                bone_interact_point = final_bone->position;
            }
            else
            {
                
                if(is_rig(bone_index)) continue;
                
                Vector3 bone_position_end = Vector3RotateByQuaternion(final_bone->state.end_point_offset , final_bone->rotation);
                bone_position_end = Vector3Add(bone_position_end , final_bone->position);
                
                bone_interact_point = bone_position_end;
                
            }
            
            Vector3 bone_screen_point = transform_vector(bone_interact_point , world_3D_to_screen_matrix);
            
            Rect bone_screen_rect = get_rect();
            bone_screen_rect.position = bone_screen_point;
            bone_screen_rect.size = (Vector2){40 ,40};
            
            Vector3 hit_point = mouse_on_plane(bone_interact_point);
            
            //TODO : this is dumb
            //Quad interact_quad = get_billboard_quad(bone_interact_point , 100000 , 100000 );
            //RayCollision position_quad_result = get_collision_quad_3D(interact_quad);
            
            Vector2 current_mouse_position = app_data->mouse_position;
            
            //click_and_select_bone
            if(check_collision_rect(bone_screen_rect , current_mouse_position))
            {
                clip_bone->hovered_bone[bone_index] = true;
                
                if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
                {
                    
                    if(!clip_bone->selected_bone[bone_index])
                    {
                        if(key_pressing(KEY_LEFT_SHIFT))
                        {
                            BoneSelection * NewBoneSelection = editor->selected_bone_stack + editor->selected_bone_count++;
                            NewBoneSelection->clip_bone_stack_index = clip_bone_stack_index;
                            NewBoneSelection->clip_index = clip_bone->clip_index;
                            NewBoneSelection->bone_index = bone_index;
                            
                            clip_bone->selected_bone[bone_index] = true;
                        }
                        else
                        {
                            for(int StackIndex = 0 ; StackIndex < editor->selected_bone_count ; StackIndex++)
                            {
                                ClipBone * ClipBoneToUnSelect = clip_bone_stack + StackIndex;
                                
                                for(int select_bone_index = 0 ; select_bone_index < selected_model->bone_count ; select_bone_index++)
                                {
                                    ClipBoneToUnSelect->selected_bone[select_bone_index] = false;
                                }
                            }
                            
                            editor->selected_bone_count = 0;
                            
                            BoneSelection * NewBoneSelection = editor->selected_bone_stack + editor->selected_bone_count++;
                            NewBoneSelection->clip_bone_stack_index = clip_bone_stack_index;
                            NewBoneSelection->clip_index = clip_bone->clip_index;
                            NewBoneSelection->bone_index = bone_index;
                            
                            clip_bone->selected_bone[bone_index] = true;
                        }
                    }
                    
                    previous_drag_point = hit_point;
                    drag_plane_position = bone_interact_point;
                    
                    dragging_bone_position = true;
                    redrag_selection = false;
                    
                }
                
            }
            
        }
    }
    
    //ApplyDragOffsetTo_existKeyFrame
    if(dragging_bone_position)
    {
        
        Vector3 drag_point = mouse_on_plane(drag_plane_position);
        
        //Quad bone_centre_quad = get_billboard_quad(drag_plane_position, 100000.0f, 100000.0f);
        //Vector3 drag_point = get_collision_quad_3D(bone_centre_quad).point;
        //DrawQuadLine(GetBillboardQuad(DragPoint , 0.1 , 0.1) , WHITE , 8);
        
        Vector3 plane_offset= drag_point;
        plane_offset = Vector3Subtract(plane_offset, previous_drag_point);
        
        previous_drag_point = drag_point;
        
        for(int selected_bone_stack_index = 0 ; selected_bone_stack_index < editor->selected_bone_count ; selected_bone_stack_index++)
        {
            BoneSelection * dragging_bone_selection = editor->selected_bone_stack + selected_bone_stack_index;
            
            int dragging_bone_index = dragging_bone_selection->bone_index;
            ClipBone * dragging_clip_bone = clip_bone_stack + dragging_bone_selection->clip_bone_stack_index;
            Bone * dragging_bone = dragging_clip_bone->final_bone_pose + dragging_bone_index;
            
            //it just feel right to do it
            //also the world rotation is compute after
            //something could go wrong
            Quaternion revert_rotation = QuaternionMultiply(dragging_bone->rotation , QuaternionInvert(dragging_bone->state.local_rotation));
            //Revertrotation = selected_bone->state.local_rotation;
            plane_offset = Vector3RotateByQuaternion(plane_offset,QuaternionInvert(revert_rotation));
            //printf("Offset : %f %f %f\n" ,PlaneOffset.x , PlaneOffset.y , PlaneOffset.z);
            
            int assign_key_frame_count = 0;
            
            if (dragging_bone->free_position)
            {
                if(!editor->playing)
                {
                    
                    Clip * current_clip = clip_array[dragging_clip_bone->clip_index];
                    
                    hash_table_iterate(key_frame_index , GetKeyFrameHash(editor->current_frame_at_timeline , dragging_bone_index) , &current_clip->key_frame_hash_table)
                    {
                        
                        KeyFrame * key_frame = all_key_frame + key_frame_index;
                        if(key_frame->bone_index != dragging_bone_index) continue;
                        if(key_frame->frame_index != editor->current_frame_at_timeline) continue;
                        
                        key_frame->bone_state.local_position = Vector3Add(key_frame->bone_state.local_position , plane_offset);
                        
                        if(assign_key_frame_count>0) CATCH;
                        assign_key_frame_count++;
                    }
                    
                    if(editor_type == edit_base_pose)
                    {
                        
                        selected_model->all_bones[dragging_bone_index].state.local_position = Vector3Add(selected_model->all_bones[dragging_bone_index].state.local_position , plane_offset);
                        
                    }
                }
                
            }
            
        }
    }
    else
    {
        
        //SingleBonerotation
        
        pruning_3D_line = true;
        change_matrix(world_3D_matrix);
        if(editor->selected_bone_count == 1)
        {
            
            BoneSelection * single_bone_selection = editor->selected_bone_stack;
            
            int single_bone_index = single_bone_selection->bone_index;
            Bone * final_bone_array_copy = clip_bone_stack[single_bone_selection->clip_bone_stack_index].final_bone_pose;
            Bone * selected_bone =  final_bone_array_copy + single_bone_index;
            
            bool interacting_selected_bone = check_selected_bone_rotation( final_bone_array_copy , single_bone_index , clip_array[single_bone_selection->clip_index]);
            
            if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
            {
                if(interacting_selected_bone)
                {
                    redrag_selection = false;
                }
                else
                {
                    editor->selected_bone_count = 0;
                    
                }
                
            }
        }
        
        pruning_3D_line = false;
        change_matrix(viewport_screen_pixel_matrix);
    }
    
    //MultiSelection
    local_persist Vector3 mouse_start_drag_position = {};
    Vector2 mouse_screen_position = app_data->mouse_position;
    
    if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
    {
        mouse_start_drag_position.x = mouse_screen_position.x;
        mouse_start_drag_position.y = mouse_screen_position.y;
    }
    
    local_persist bool dragging_selection = false;
    
    if(redrag_selection)
    {
        
        Vector3 current_mouse_drag_position = {mouse_screen_position.x , mouse_screen_position.y , 0};
        
        Rect mouse_drag_rect = get_rect();
        mouse_drag_rect.position= Vector3Add(mouse_start_drag_position , current_mouse_drag_position);
        mouse_drag_rect.position.x /= 2.0f;
        mouse_drag_rect.position.y /= 2.0f;
        
        mouse_drag_rect.size.x = mouse_start_drag_position.x - current_mouse_drag_position.x;
        if(mouse_drag_rect.size.x < 0) mouse_drag_rect.size.x *= -1;
        
        mouse_drag_rect.size.y = mouse_start_drag_position.y - current_mouse_drag_position.y;
        if(mouse_drag_rect.size.y < 0) mouse_drag_rect.size.y *= -1;
        
        if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
        {
            dragging_selection = true;
            if(!key_pressing(KEY_LEFT_SHIFT))
            {
                
                for(int StackIndex = 0 ; StackIndex < clip_bone_stack_count ; StackIndex++)
                {
                    ClipBone * ClipBoneToUnSelect = clip_bone_stack + StackIndex;
                    
                    for(int bone_index = 0 ; bone_index < selected_model->bone_count ; bone_index++)
                    {
                        ClipBoneToUnSelect->selected_bone[bone_index] = false;
                    }
                }
                
                editor->selected_bone_count = 0;
            }
        }
        
        if(dragging_selection)
        {
            draw_rect_line_E(mouse_drag_rect , WHITE , 5);
        }
        
        for(int clip_bone_stack_index = 0 ; clip_bone_stack_index < clip_bone_stack_count ; clip_bone_stack_index++)
        {
            ClipBone * current_clipBone = clip_bone_stack + clip_bone_stack_index;
            
            for(int bone_index = 0 ; bone_index < selected_model->bone_count ; bone_index++)
            {
                Bone * current_bone = current_clipBone->final_bone_pose + bone_index;
				Vector3 bone_screen_point = {};
                
                if(editor->control_rig)
                {
                    if(!is_rig(bone_index)) continue;
                    
                    bone_screen_point = current_bone->position;
                }
                else
                {
                    if(is_rig(bone_index)) continue;
                    
					Vector3 bone_position_end = Vector3RotateByQuaternion(current_bone->state.end_point_offset , current_bone->rotation);
                    bone_position_end = Vector3Add(bone_position_end , current_bone->position);
                    
                    bone_screen_point = bone_position_end;
                }
                
                bone_screen_point = transform_vector( bone_screen_point , world_3D_to_screen_matrix);
                
                Rect bone_screen_rect = get_rect();
                bone_screen_rect.position = bone_screen_point;
                bone_screen_rect.size = (Vector2){40 ,40};
                
                if(check_collision_rect_with_rect(mouse_drag_rect , bone_screen_rect ))
                {
                    if(dragging_selection)
                    {
                        current_clipBone->hovered_bone[bone_index] = true;
                        
                        if(mouse_button_released(MOUSE_BUTTON_LEFT))
                        {
                            if(!current_clipBone->selected_bone[bone_index])
                            {
                                current_clipBone->selected_bone[bone_index] = true;
                                
                                BoneSelection * new_selection = editor->selected_bone_stack + editor->selected_bone_count++;
                                
                                new_selection->clip_bone_stack_index = clip_bone_stack_index;
                                new_selection->clip_index = current_clipBone->clip_index;
                                new_selection->bone_index = bone_index;
                            }
                            
                        }
                    }
                    
                }
            }
        }
        
    }
    
    
    if(mouse_button_released(MOUSE_BUTTON_LEFT))
    {
        dragging_selection = false;
    }
    
    
    for(int clip_bone_stack_index = 0 ; clip_bone_stack_index < clip_bone_stack_count ; clip_bone_stack_index++)
    {
        ClipBone * current_clip_bone = clip_bone_stack + clip_bone_stack_index;
        
        for(int bone_index = 0 ; bone_index < selected_model->bone_count ; bone_index++)
        {
            
            Bone * bone = current_clip_bone->final_bone_pose + bone_index;
            
            Vector3 bone_screen_point = {};
            if(editor->control_rig)
            {
                if(!is_rig(bone_index)) continue;
                
                bone_screen_point = bone->position;
            }
            else
            {
                
                if(is_rig(bone_index)) continue;
                
                Vector3 bone_position_end = Vector3RotateByQuaternion(bone->state.end_point_offset , bone->rotation);
                bone_position_end = Vector3Add(bone_position_end , bone->position);
                bone_screen_point = bone_position_end;
                
            }
            
            bone_screen_point = transform_vector( bone_screen_point , world_3D_to_screen_matrix);
            
            Rect bone_screen_rect = get_rect();
            bone_screen_rect.position = bone_screen_point;
            bone_screen_rect.size = (Vector2){40 ,40};
            
            //DrawBoneSelection
#if 1
            if(current_clip_bone->selected_bone[bone_index])
            {
                draw_rect_line_E(bone_screen_rect , BLUE , 5);
            }
            else if(current_clip_bone->hovered_bone[bone_index])
            {
                draw_rect_line_E(bone_screen_rect , YELLOW , 5);
            }
            else
            {
                draw_rect_line_E(bone_screen_rect , Fade(YELLOW , 0.2f) , 5);
            }
            
            current_clip_bone->hovered_bone[bone_index] = false;
#endif
        }
    }
    
    pruning_3D_line = true;
    change_matrix(world_3D_matrix);
}

//this one is weird as fuck
internal void bone_mouse_menu( Bone * single_editing_bone , Clip * clip , int current_frame_index)
{
    
    local_persist bool menu_enable = false;
    
	if (mouse_button_pressed(MOUSE_BUTTON_RIGHT))
	{
        if (editor->selected_bone_count == 1)
		{
			menu_enable = !menu_enable;
		}
	}
    
	if (menu_enable)
	{
        KeyFrame * editing_key_frame = 0;
        
        if(clip)
        {
            
            hash_table_iterate(key_frame_index , GetKeyFrameHash(current_frame_index , single_editing_bone->bone_index) , &clip->key_frame_hash_table)
            {
                
                KeyFrame * key_frame = all_key_frame + key_frame_index;
                
                if(key_frame->bone_index != single_editing_bone->bone_index) continue;
                if(key_frame->frame_index != current_frame_index) continue;
                
                editing_key_frame = key_frame;
                break;
            }
        }
        
        DrawingMenu menu = start_draw_menu_mouse();
        
		draw_menu_float_input(&menu , L"長度 : %f " , &single_editing_bone->state.end_point_offset.y);
        
		if (draw_menu_button_W(&menu , L"重置位置"))
		{
			if(editing_key_frame)
            {
                editing_key_frame->bone_state.local_position = (Vector3){};
            }
            else
            {
                single_editing_bone->state.local_position = (Vector3){};
            }
            
		}
        
		if (draw_menu_button_W(&menu , L"重置旋轉"))
		{
            if(editing_key_frame)
            {
                editing_key_frame->bone_state.local_rotation = QuaternionIdentity();
            }
            else
            {
                single_editing_bone->state.local_rotation = QuaternionIdentity();
            }
		}
        
        if(draw_menu_button_W(&menu , single_editing_bone->free_position ? L"鎖定位置" : L"自由位置" ))
        {
            single_editing_bone->free_position = !single_editing_bone->free_position;
        }
        
        int key_frame_count = 0;
        
        if(editing_key_frame)
        {
            if (draw_menu_button_W(&menu , L"重置貝塞爾曲綫"))
            {
                editing_key_frame->previous_bezier_offset = (Vector3){};
                editing_key_frame->next_bezier_offset = (Vector3){};
            }
        }
        
	}
    
}

internal void iterate_bone_structure(Bone * bone_array , Bone * root_bone)
{
    
    Bone * bone_stack[256] = {};
    int bone_stack_count = 0;
    
    bone_stack[bone_stack_count++] = root_bone;
    
    //printf("\nBone Start \n");
    
    for (; bone_stack_count > 0;)
    {
        
        Bone * iterating_bone = bone_stack[--bone_stack_count];
        int iterating_bone_index = iterating_bone->bone_index;
        
        //Vector3 iterating_bone_end_position = Vector3Add(iterating_bone->position , iterating_bone->state.direction);
        
        hash_table_iterate(child_bone_index , iterating_bone_index , &selected_model->bone_children_hash_table)
        {
            
            Bone * child_bone = bone_array + child_bone_index;
            if(child_bone->parent_bone_index != iterating_bone_index)
            {
                continue;
            }
            
            Quaternion rotation = QuaternionMultiply(iterating_bone->rotation , child_bone->state.local_rotation);
            child_bone->rotation = rotation;
            
            Vector3 child_head = Vector3RotateByQuaternion(child_bone->state.local_position , iterating_bone->rotation);
            Vector3 child_tail = Vector3RotateByQuaternion(child_bone->state.end_point_offset, rotation);
            child_tail = Vector3Add(child_head , child_tail);
            
            child_bone->position = Vector3Add( child_head , iterating_bone->position);
            child_bone->end_position = Vector3Add( child_tail , iterating_bone->position);
            
            bone_stack[bone_stack_count++] = child_bone;
            
        }
        
    }
    
}

internal void update_bone_structure( Bone * bone_array)
{
    
    //what?
	list_foreach(root_bone_index , &selected_model->root_bone_list )
	{
        Bone * root_bone = bone_array + root_bone_index;
        
        root_bone->position = root_bone->state.local_position;
        root_bone->end_position = root_bone->state.end_point_offset;
        root_bone->rotation = root_bone->state.local_rotation;
        root_bone->end_position = Vector3RotateByQuaternion(root_bone->end_position , root_bone->rotation);
        root_bone->end_position = Vector3Add(root_bone->end_position , root_bone->position);
        
        //printf("%f %f %f \n" , RootBone->position.x , RootBone->position.y , RootBone->position.z);
        
        iterate_bone_structure(bone_array , root_bone);
	}
    
}

#define DEBUG_IK_DISPLAY 0
#define DEBUG_IK_POLE_DISPLAY 0

internal void bone_IK_update_B(Bone * bone_array , Bone * base_pose_bone_array , int target_bone_index , int pole_bone_index , int IK_bone_index , int iteration_count , int bone_chain_max_length)
{
    
    Bone * target_bone = bone_array + target_bone_index;
    Bone * pole_bone = bone_array + pole_bone_index;
    Bone * IK_bone = bone_array + IK_bone_index;
    
    bone_IK_update( bone_array , base_pose_bone_array , target_bone , pole_bone , IK_bone , iteration_count , bone_chain_max_length);
    
}

internal void bone_IK_update( Bone * bone_array , Bone * base_pose_bone_array , Bone * target_bone , Bone * pole_bone , Bone * IK_bone , int iteration_count , int bone_chain_max_length)
{
    
    Bone * bone_stack[256] = {};
    int bone_stack_count = 0;
    
    Bone * bone_chain[256] = {};
    int bone_chain_count = 0;
    
    bone_stack[bone_stack_count++] = IK_bone;
    bone_chain[bone_chain_count++] = IK_bone;
    
    int bone_chain_length = 0;
    
    for(;;)
    {
        
        if(bone_stack_count <= 0) break;
        if(bone_chain_length >= bone_chain_max_length) break;
        bone_chain_length++;
        
        Bone * iterating_bone = bone_stack[--bone_stack_count];
        int iterating_bone_index = iterating_bone->bone_index;
        iterating_bone->state.local_position = (Vector3){};
        
        Bone * parent_bone = bone_array + iterating_bone->parent_bone_index;
        
        bone_chain[bone_chain_count++] = parent_bone;
        bone_stack[bone_stack_count++] = parent_bone;
        
    }
    
    Bone * start_bone = bone_chain[bone_chain_count - 1];
    Bone * end_bone = bone_chain[0];
    
    iterate_bone_structure(bone_array , start_bone);
    
    Vector3 chain_end_position = Vector3RotateByQuaternion(end_bone->state.end_point_offset , end_bone->state.local_rotation);
    chain_end_position = Vector3Add(chain_end_position , end_bone->position);
    
    Vector3 chain_start_to_target_direction = Vector3Subtract(target_bone->position , start_bone->position);
    Vector3 chain_start_to_end_direction = Vector3Subtract(chain_end_position , start_bone->position);
    chain_start_to_target_direction = Vector3Normalize(chain_start_to_target_direction);
    chain_start_to_end_direction = Vector3Normalize(chain_start_to_end_direction);
    
    Quaternion chain_rotation = QuaternionFromVector3ToVector3(chain_start_to_end_direction , chain_start_to_target_direction);
    
#if DEBUG_IK_POLE_DISPLAY
    draw_arrow_ray_D(start_bone->position, chain_start_to_target_direction , BLUE);
    draw_arrow_ray_D(start_bone->position, chain_start_to_end_direction , GREEN);
#endif
    
    Bone * pole_bone_in_edit = base_pose_bone_array + pole_bone->bone_index;
    Vector3 chain_start_to_base_pole_direction = Vector3Subtract(pole_bone_in_edit->position , start_bone->position );
    chain_start_to_base_pole_direction = Vector3Normalize(chain_start_to_base_pole_direction);
    
    Vector3 current_chain_right = Vector3CrossProduct(chain_start_to_base_pole_direction , chain_start_to_end_direction );
    Vector3 current_chain_forward = Vector3CrossProduct(chain_start_to_end_direction , current_chain_right);
    
    current_chain_right = Vector3RotateByQuaternion(current_chain_right , chain_rotation);
    current_chain_forward = Vector3RotateByQuaternion(current_chain_forward , chain_rotation);
    current_chain_forward = Vector3Normalize(current_chain_forward);
    
    Vector3 chain_start_to_pole_direction = Vector3Subtract(pole_bone->position , start_bone->position);
    Vector3 target_chain_right = Vector3CrossProduct( chain_start_to_pole_direction , chain_start_to_target_direction);
    Vector3 target_chain_forward = Vector3CrossProduct( chain_start_to_target_direction , target_chain_right );
    target_chain_forward = Vector3Normalize(target_chain_forward);
    
#if DEBUG_IK_POLE_DISPLAY
    draw_arrow_ray_D(start_bone->position, target_chain_right , PURPLE);
    draw_arrow_ray_D(start_bone->position, target_chain_forward , PINK);
    draw_arrow_ray_D(start_bone->position, current_chain_forward , RED);
#endif
    
    Quaternion rotate_to_toward_pole = QuaternionFromVector3ToVector3( current_chain_forward , target_chain_forward);
    chain_rotation = QuaternionMultiply(rotate_to_toward_pole , chain_rotation);
    
    float chain_length = 0;
    
    for(int bone_chain_index = bone_chain_count - 1 ; bone_chain_index >= 0  ; bone_chain_index--)
    {
        Bone * parent_bone = bone_chain[bone_chain_index];
        parent_bone->rotation = QuaternionMultiply( parent_bone->state.local_rotation , chain_rotation);
        
        chain_length += Vector3Length(parent_bone->state.end_point_offset);
        
        hash_table_iterate(child_index , parent_bone->bone_index , &selected_model->bone_children_hash_table)
        {
            
            Bone * child_bone = bone_array + child_index;
            
            child_bone->position = Vector3RotateByQuaternion(parent_bone->state.end_point_offset , parent_bone->rotation);
            child_bone->position = Vector3Add(child_bone->position , parent_bone->position);
            
        }
        
    }
    
    float start_to_target_distance = Vector3Distance(target_bone->position , start_bone->position);
    if(start_to_target_distance > chain_length) 
    {
        iterate_bone_structure(bone_array , bone_chain[0]);
        return;
    }
    
    for(int iteration_index = 0 ; iteration_index < iteration_count ; iteration_index++)
    {
        
        Vector3 end_position = IK_bone->position;
        end_position = Vector3Add(end_position , Vector3RotateByQuaternion(IK_bone->state.end_point_offset , IK_bone->rotation) );
        //DrawQuadLine(GetBillboardQuad(Endposition , 0.1 , 0.1 ) ,WHITE);
        
        Vector3 start_position = start_bone->position;
        //Startposition = Vector3Add(Startposition , StartBone->position);
        
        for(int bone_chain_index = bone_chain_count - 1 ; bone_chain_index >= 0 ; bone_chain_index--)
        {
            
            Bone * parent_bone = bone_chain[bone_chain_index];
            Bone * child_bone = bone_chain[bone_chain_index - 1];
            
            Vector3 direction_to_end_bone = Vector3Subtract(end_position ,parent_bone->position);
            Vector3 direction_to_target_bone = Vector3Subtract(target_bone->position ,parent_bone->position);
            
            //wtf???
            //normalizing it actually cause problem when bone get too close
            //there is more quaternion magic i have to learn yet
            
            //direction_to_end_bone = Vector3Normalize(direction_to_end_bone);
            //direction_to_target_bone = Vector3Normalize(direction_to_target_bone);
            
#if DEBUG_IK_DISPLAY
            draw_arrow_ray_D(parent_bone->position, direction_to_end_bone , BLUE);
            draw_arrow_ray_D(parent_bone->position, direction_to_target_bone , GREEN);
#endif
            
            //TODO : try Fabrik?
            Quaternion bone_rotation = QuaternionFromVector3ToVector3(direction_to_end_bone , direction_to_target_bone );
            parent_bone->rotation = QuaternionMultiply(bone_rotation , parent_bone->rotation);
            
            Vector3 parent_bone_end_offset = Vector3RotateByQuaternion( parent_bone->state.end_point_offset , parent_bone->rotation );
            
            if(bone_chain_index > 0)
            {
                child_bone->position = Vector3Add(parent_bone->position , parent_bone_end_offset);
            }
        }
        
    }
    
    iterate_bone_structure(bone_array , bone_chain[0]);
}

//TODO : bone go crazy with high iteration count(100) when it is out of reach of its target
internal void _bone_IK_update( Bone * bone_array , Bone * target_bone , Bone * pole_bone , Bone * IK_bone , int iteration_count , int bone_chain_max_length)
{
    
    Bone * bone_stack[256] = {};
    int bone_stack_count = 0;
    
    Bone * bone_chain[256] = {};
    int bone_chain_count = 0;
    
    bone_stack[bone_stack_count++] = IK_bone;
    bone_chain[bone_chain_count++] = IK_bone;
    
    int bone_chain_length = 0;
    
    for(;;)
    {
        
        if(bone_stack_count <= 0) break;
        if(bone_chain_length >= bone_chain_max_length) break;
        bone_chain_length++;
        
        Bone * iterating_bone = bone_stack[--bone_stack_count];
        int iterating_bone_index = iterating_bone->bone_index;
        iterating_bone->state.local_position = (Vector3){};
        
        Bone * parent_bone = bone_array + iterating_bone->parent_bone_index;
        
        bone_chain[bone_chain_count++] = parent_bone;
        bone_stack[bone_stack_count++] = parent_bone;
        
    }
    
    for(int iteration_index = 0 ; iteration_index < iteration_count ; iteration_index++)
    {
        
        Vector3 end_position = IK_bone->position;
        end_position = Vector3Add(end_position , Vector3RotateByQuaternion(IK_bone->state.end_point_offset , IK_bone->rotation) );
        //DrawQuadLine(GetBillboardQuad(Endposition , 0.1 , 0.1 ) ,WHITE);
        
        Bone * start_bone = bone_chain[bone_chain_count -1];
        Vector3 start_position = start_bone->position;
        //Startposition = Vector3Add(Startposition , StartBone->position);
        
        for(int bone_chain_index = bone_chain_count - 1 ; bone_chain_index >= 0 ; bone_chain_index--)
        {
            
            Bone * parent_bone = bone_chain[bone_chain_index];
            Bone * child_bone = bone_chain[bone_chain_index - 1];
            
            Vector3 direction_to_end_bone = Vector3Subtract(end_position ,parent_bone->position);
            direction_to_end_bone = Vector3Normalize(direction_to_end_bone);
            
            Vector3 direction_to_target_bone = Vector3Subtract(target_bone->position ,parent_bone->position);
            direction_to_target_bone = Vector3Normalize(direction_to_target_bone);
            
#if DEBUG_IK_DISPLAY
            draw_arrow_ray_D(parent_bone->position, direction_to_end_bone , BLUE);
            draw_arrow_ray_D(parent_bone->position, direction_to_target_bone , GREEN);
#endif
            
            //printf("%f \n" , Vector3Angle(directionToEndBone , directionToTargetBone) * RAD2DEG);
            
            //TODO : try Fabrik?
            Quaternion bone_rotation = QuaternionFromVector3ToVector3(direction_to_end_bone , direction_to_target_bone );
            //Bonerotation = QuaternionMultiply( Bonerotation, QuaternionInvert(IteratingBone->rotation));
            
            parent_bone->rotation = QuaternionMultiply(bone_rotation , parent_bone->rotation);
            
            //IteratingBone->state.local_rotation = QuaternionIdentity();
            
            Vector3 parent_bone_end_offset = Vector3RotateByQuaternion( parent_bone->state.end_point_offset , parent_bone->rotation );
            
            if(bone_chain_index > 0)
            {
                child_bone->position = Vector3Add(parent_bone->position , parent_bone_end_offset);
            }
        }
        
    }
    
    Vector3 end_position = IK_bone->position;
    end_position = Vector3Add(end_position , Vector3RotateByQuaternion(IK_bone->state.end_point_offset , IK_bone->rotation) );
    
    Bone * start_bone = bone_chain[bone_chain_count -1];
    
    Vector3 start_position = start_bone->position;
    
    Vector3 bone_chain_direction = Vector3Subtract(end_position , start_position);
    
#if DEBUG_IK_POLE_DISPLAY
    draw_arrow_ray_D(start_position , bone_chain_direction , Fade(WHITE,0.5f) );
#endif
    
    Vector3 start_to_pole_direction = Vector3Subtract(pole_bone->position , start_position);
    
    Vector3 pole_project_point_on_bone_chain = Vector3Project(start_to_pole_direction , bone_chain_direction);
    pole_project_point_on_bone_chain = Vector3Add(start_position , pole_project_point_on_bone_chain);
    Vector3 pole_direction = Vector3Subtract(pole_bone->position , pole_project_point_on_bone_chain);
    pole_direction = Vector3Normalize(pole_direction);
    
    Vector3 bone_chain_centre_point = Vector3Lerp(start_position , end_position , 0.5);
    
    Vector3 bone_direction_sum = {};
    
    Vector3 bone_chain_vertical_direction = {};
    
    for(int bone_chain_index = bone_chain_count - 1 ; bone_chain_index >= 1 ; bone_chain_index--)
    {
        Bone * current_bone = bone_chain[bone_chain_index];
        
        Vector3 bone_end_direction = Vector3RotateByQuaternion(current_bone->state.end_point_offset , current_bone->rotation);
        
        Vector3 bone_end = Vector3Add(current_bone->position , bone_end_direction);
        
        bone_direction_sum = Vector3Add(bone_end_direction , bone_direction_sum);
        
        Vector3 bone_end_direction_project = Vector3Project(bone_direction_sum , bone_chain_direction);
        
        Vector3 bone_chain_start_point_to_bone_end = Vector3Add(start_position , bone_end_direction_project);
        Vector3 bone_vertical_direction_to_bone_chain = Vector3Subtract(bone_end , bone_chain_start_point_to_bone_end);
        
#if DEBUG_IK_POLE_DISPLAY
        draw_arrow_ray_D(bone_chain_start_point_to_bone_end , bone_vertical_direction_to_bone_chain , Fade(GREEN , 0.5f) );
#endif
        
#if 0
        bone_vertical_direction_to_bone_chain = Vector3Normalize(bone_vertical_direction_to_bone_chain);
        Quaternion rotate_toward_pole = QuaternionFromVector3ToVector3(bone_vertical_direction_to_bone_chain , pole_direction);
        current_bone->rotation = QuaternionMultiply(rotate_toward_pole , current_bone->rotation );
        if(bone_chain_index != 0)
        {
            Bone * child_bone = bone_chain[bone_chain_index -1];
            
            child_bone->position = Vector3RotateByQuaternion(current_bone->state.end_point_offset ,current_bone->rotation );
            child_bone->position = Vector3Add(child_bone->position , current_bone->position);
        }
#endif
        
        bone_chain_vertical_direction = Vector3Add(bone_chain_vertical_direction , bone_vertical_direction_to_bone_chain);
    }
    
    bone_chain_vertical_direction = Vector3Normalize(bone_chain_vertical_direction);
    
#if DEBUG_IK_POLE_DISPLAY
    draw_arrow_ray_D(bone_chain_centre_point , bone_chain_vertical_direction , Fade(RED, 0.5) );
    draw_arrow_ray_D(bone_chain_centre_point , pole_direction , Fade(PINK, 0.5) );
#endif
    
    Quaternion bone_chain_to_pole_rotation = QuaternionFromVector3ToVector3(bone_chain_vertical_direction , pole_direction);
    
    for(int bone_chain_index = bone_chain_count - 1 ; bone_chain_index >= 0 ; bone_chain_index--)
    {
        Bone * current_bone = bone_chain[bone_chain_index];
        
#if 0
        current_bone->rotation = QuaternionMultiply(bone_chain_to_pole_rotation , current_bone->rotation );
        
        Vector3 current_bone_to_pole = Vector3Subtract( pole_bone->position, current_bone->position );
        
        Vector3 current_bone_direction = Vector3RotateByQuaternion(current_bone->state.end_point_offset , current_bone->rotation);
        Vector3 bone_right_direction = Vector3CrossProduct(current_bone_to_pole , current_bone_direction);
        
        Vector3 bone_forward_direction = Vector3CrossProduct(current_bone_direction , bone_right_direction);
        Vector3 bone_current_foward_direction = Vector3RotateByQuaternion( (Vector3){0,0,1} ,current_bone->rotation);
        
        bone_current_foward_direction = Vector3Normalize(bone_current_foward_direction);
        bone_forward_direction = Vector3Normalize(bone_forward_direction);
        Quaternion forward_rotation= QuaternionFromVector3ToVector3(bone_current_foward_direction  , bone_forward_direction);
        
#if 0
        draw_arrow_ray_D(current_bone->position , bone_forward_direction , Fade(RED , 0.5));
        draw_arrow_ray_D(current_bone->position , bone_current_foward_direction , Fade(WHITE , 0.5));
#endif
        
        current_bone->rotation = QuaternionMultiply( forward_rotation , current_bone->rotation);
        
        if(bone_chain_index != 0)
        {
            Bone * child_bone = bone_chain[bone_chain_index -1];
            
            child_bone->position = Vector3RotateByQuaternion(current_bone->state.end_point_offset ,current_bone->rotation );
            child_bone->position = Vector3Add(child_bone->position , current_bone->position);
        }
#endif
        
    }
    
    iterate_bone_structure(bone_array , bone_chain[0]);
}

internal void edit_map(Vector3 origin)
{
    
    Vector3 camera_direction = Vector3Subtract(game_camera.target , game_camera.position);
    
    Vector3 all_direction[3] = {up_direction  , right_direction , forward_direction};
    float dot_product = 0;
    grid_direction_index = -1;
    
    for(int i = 0 ; i < 3 ; i++)
    {
        float current_dot_product = Vector3DotProduct(camera_direction , all_direction[i]);
        current_dot_product = fabs(current_dot_product);
        
        if(current_dot_product > dot_product)
        {
            dot_product = current_dot_product;
            grid_direction_index = i;
        }
    }
    
#if 0
    if(within_viewport)
    {
        float closest_distance = FLT_MAX;
        
        for(int i = 0 ; i < 3 ; i++)
        {
            Vector3 mouse_ray_target = Vector3Add(mouse_ray_3D.position , mouse_ray_3D.direction);
            Vector3 mouse_ray_position = mouse_ray_3D.position;
            float intersect_time = get_line_intersect_with_plane_time(mouse_ray_position , mouse_ray_target , all_direction[i] , origin);
            Vector3 intersect_point = Vector3Lerp(mouse_ray_position , mouse_ray_target , intersect_time);
            
            float distance = Vector3DistanceSqr(intersect_point , game_camera.position);
            
            float current_dot_product = Vector3DotProduct(camera_direction , all_direction[i]);
            current_dot_product = fabs(current_dot_product);
            
            if(current_dot_product > 0.6)
            {
                if(distance < closest_distance) 
                {
                    grid_direction_index = i;
                    closest_distance = distance;
                }
            }
            
        }
    }
#endif
    
    if(editor_type == edit_world)
    {
        //Vector3 origin
        Vector3 grid_direction = all_direction[grid_direction_index];
        grid_direction = Vector3Normalize(grid_direction);
        
        Vector3 mouse_ray_target = Vector3Add(mouse_ray_3D.position , mouse_ray_3D.direction);
        Vector3 mouse_ray_position = mouse_ray_3D.position;
        float intersect_time = get_line_intersect_with_plane_time(mouse_ray_position , mouse_ray_target , grid_direction , origin);
        Vector3 intersect_point = Vector3Lerp(mouse_ray_position , mouse_ray_target , intersect_time);
        
        Rect rect_in_cell = get_rect();
        intersect_point = Vector3Subtract(intersect_point , origin);
        rect_in_cell.position = position_to_grid(Vector3Add(intersect_point , Vector3Scale(grid_direction , 0.0001f)) , GRID_SIZE);
        
        rect_in_cell.position.x -= GRID_SIZE * 0.5;
        rect_in_cell.position.y -= GRID_SIZE * 0.5;
        rect_in_cell.position.z -= GRID_SIZE * 0.5;
        
        rect_in_cell.position = Vector3Subtract(rect_in_cell.position , Vector3Scale(grid_direction , GRID_SIZE * 0.5) );
        rect_in_cell.position = Vector3Add(rect_in_cell.position , origin);
        
        rect_in_cell.size = (Vector2){GRID_SIZE , GRID_SIZE};
        rect_in_cell.rotation = QuaternionFromVector3ToVector3( (Vector3){0,0,1} , grid_direction);
        
        if(edit_quad)
        {
            draw_rect_D(rect_in_cell , 0 , Fade(WHITE , 0.3));
        }
        
        if(within_viewport)
        {
            if(edit_quad)
            {
                bool collided = false;
                
                array_foreach(quad_index , &quads_in_map_array)
                {
                    RayCollision collision = get_collision_quad_3D(quads_in_map[quad_index]);
                    if(collision.hit)
                    {
                        collided = true;
                        
                        if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
                        {
                            if(!delete_from_array(&quads_in_map_array , quad_index)) CATCH;
                        }
                        
                        break;
                    }
                }
                
                if(!collided)
                {
                    if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
                    {
                        REALLOCATE_ARRAY_IF_TOO_SMALL(Quad , quads_in_map , &quads_in_map_array);
                        Quad * new_quad = quads_in_map + add_to_array(&quads_in_map_array);
                        
                        (*new_quad) = rect_to_quad(rect_in_cell);
                    }
                }
            }
        }
    }
    
}

internal void draw_origin_grid(Vector3 origin)
{
    
#if 1
	int grid_hint_extend = 50;
    
    for (int axis_index = 0 ; axis_index < 3; axis_index++)
	{
		Color grid_line_color = {};
		float grid_line_size = 8;
        
        if (axis_index == 0) grid_line_color = Fade(RED,0.2f);
		if (axis_index == 1) grid_line_color = Fade(GREEN, 0.2f);
		if (axis_index == 2) grid_line_color = Fade(BLUE, 0.2f);
        
        if(grid_direction_index != axis_index) continue;
        
		for (int horizontal_index = 0; horizontal_index < grid_hint_extend * 2 +1; horizontal_index++)
		{
            float line_offset = grid_hint_extend * UNIT_SIZE;
			int line_index = horizontal_index - grid_hint_extend;
            
			Vector3 start_position = origin;
            
			if (axis_index == 2)
			{
				start_position.y -= UNIT_SIZE * grid_hint_extend;
				start_position.y += UNIT_SIZE * horizontal_index;
			}
			else if (axis_index == 1)
			{
				start_position.y -= UNIT_SIZE * grid_hint_extend;
				start_position.y += UNIT_SIZE * horizontal_index;
			}
			else
			{
				start_position.z -= UNIT_SIZE * grid_hint_extend;
				start_position.z += UNIT_SIZE * horizontal_index;
			}
            
			Vector3 left_point = start_position;
			Vector3 right_point = start_position;
            
			if (axis_index == 2)
			{
				left_point.x -= line_offset;
				right_point.x += line_offset;
			}
			else if (axis_index == 1)
			{
				left_point.z -= line_offset;
				right_point.z += line_offset;
			}
			else
			{
				left_point.x -= line_offset;
				right_point.x += line_offset;
			}
            
			Color line_color_H = grid_line_color;
			if (horizontal_index == grid_hint_extend) line_color_H = Fade(BLUE,0.5);
            
            if( horizontal_index % SUBDIVISION != 0) line_color_H = Fade(line_color_H , 0.05);
            
			int grid_distance = horizontal_index - grid_hint_extend;
            
			if (grid_distance < 0) grid_distance *= -1;
            
			line_color_H = Fade(line_color_H, ((float)line_color_H.a /255.0)*(1.0-(float)grid_distance/ (float)grid_hint_extend));
            
			draw_round_line(start_position, left_point,grid_line_size ,line_color_H , Fade(line_color_H ,0));
			draw_round_line(start_position, right_point ,grid_line_size ,line_color_H ,Fade(line_color_H ,0));
            
		}
        
		for (int vertical_index = 0; vertical_index < grid_hint_extend * 2 + 1; vertical_index++)
		{
            
			float line_offset = grid_hint_extend * UNIT_SIZE;
            
			int line_index = vertical_index - grid_hint_extend;
            
			Vector3 start_position = origin;
            
			if (axis_index == 2)
			{
				start_position.x -= UNIT_SIZE * grid_hint_extend;
				start_position.x += UNIT_SIZE * vertical_index;
			}
			else if (axis_index == 1)
			{
				start_position.z -= UNIT_SIZE * grid_hint_extend;
				start_position.z += UNIT_SIZE * vertical_index;
			}
			else
			{
				start_position.x -= UNIT_SIZE * grid_hint_extend;
				start_position.x += UNIT_SIZE * vertical_index;
			}
            
			Vector3 top_point = start_position;
			Vector3 bottom_point = start_position;
            
			if (axis_index == 2)
			{
				top_point.y += line_offset;
				bottom_point.y -= line_offset;
			}
			else if (axis_index == 1)
			{
				top_point.y += line_offset;
				bottom_point.y -= line_offset;
			}
			else
			{
				top_point.z += line_offset;
				bottom_point.z -= line_offset;
			}
            
			Color line_color_V = grid_line_color;
			if (vertical_index == grid_hint_extend) line_color_V = Fade(GREEN,0.5f);
            
			int grid_distance = vertical_index- grid_hint_extend;
            
			if (grid_distance < 0) grid_distance *= -1;
            
            if( vertical_index % SUBDIVISION != 0) line_color_V = Fade(line_color_V , 0.05);
            
			line_color_V = Fade(line_color_V, ((float)line_color_V.a / 255.0) * (1 - (float)grid_distance / (float)grid_hint_extend));
            
			draw_round_line(start_position, top_point,grid_line_size ,line_color_V ,Fade(line_color_V ,0));
			draw_round_line(start_position, bottom_point,grid_line_size ,line_color_V,Fade(line_color_V ,0));
		}
        
	}
#endif
    
}

internal void get_bone_from_existing_key_frame(Bone * bone_array , Clip * clip , int target_frame , int target_start_frame , int target_frame_count)
{
    
	for (int bone_index = 0; bone_index < selected_model->bone_count; bone_index++)
	{
        
        Bone * current_bone = bone_array + bone_index;
        
		bool no_interpolation = true;
        
		current_bone->free_bone = false;
        
        KeyFrame * start_key_frame = 0;
        
        KeyFrame * end_key_frame = 0;
        
        //TODO : this one is finding the last key frame for looping
        hash_table_iterate(key_frame_index , bone_index , &clip->key_frame_hash_table_by_bone)
        {
            
			KeyFrame * key_frame = all_key_frame + key_frame_index;
            if(key_frame->bone_index != bone_index) continue;
            
            end_key_frame = all_key_frame + key_frame_index;
            
        }
        
        KeyFrame * first_key_frame = 0;
        bool not_the_last_key_frame = false;
        
        //TODO : it is iterating frame to find closest frame
        hash_table_iterate(key_frame_index , bone_index , &clip->key_frame_hash_table_by_bone)
        {
            KeyFrame * key_frame = all_key_frame + key_frame_index;
            
            if(key_frame->bone_index != bone_index) continue;
            if(!first_key_frame) first_key_frame = key_frame;
            
            start_key_frame = end_key_frame;
            end_key_frame = key_frame;
            
            if(key_frame->frame_index > target_frame)
            {
                not_the_last_key_frame = true;
                break;
            }
        }
        
        if(!not_the_last_key_frame)
        {
            start_key_frame = end_key_frame;
            end_key_frame = first_key_frame;
        }
        
        KeyFrame * target_key_frame = 0;
        
        if(start_key_frame)
        {
            
            if(start_key_frame->frame_index == target_frame)
            {
                target_key_frame = start_key_frame;
                current_bone->free_bone = true;
            }
            
        }
        else
        {
            if(!end_key_frame)
            {
                current_bone->free_bone = true;
            }
        }
        
        if(start_key_frame)
        {
            if(end_key_frame)
            {
                no_interpolation = false;
            }
            
            if(start_key_frame == end_key_frame)
            {
                no_interpolation = true;
            }
            
        }
        
		if (no_interpolation)
		{
			if (target_key_frame)
			{
				current_bone->state = target_key_frame->bone_state;
			}
		}
		else
		{
            //printf("%d %d\n" ,  StartKeyFrame->frame_index , EndKeyFrame->frame_index);
            
			float frame_count = get_key_frame_count(start_key_frame , end_key_frame , target_start_frame , target_frame_count);
			float frame_offset = target_frame - start_key_frame->frame_index;
            
            if(target_frame < start_key_frame->frame_index)
            {
                int end_frame_index = target_start_frame + target_frame_count - 1;
                frame_offset = end_frame_index - start_key_frame->frame_index + target_frame - target_start_frame + 1;
            }
            
            float frame_time = frame_offset / frame_count;
            //printf("%f %f %f\n" , FrameOffset, FrameCount , FrameTime);
            
            Vector3 line_direction = Vector3Subtract(end_key_frame->bone_state.local_position , start_key_frame->bone_state.local_position);
            line_direction =Vector3Scale( Vector3Normalize(line_direction) , 0.1f);
            
            Vector3 bezier_line_start_position = start_key_frame->bone_state.local_position;
            Vector3 bezier_start_point = Vector3Add(line_direction , start_key_frame->next_bezier_offset);
            bezier_start_point = Vector3Add(bezier_line_start_position , bezier_start_point);
            
            Vector3 bezier_line_end_position = end_key_frame->bone_state.local_position;
            Vector3 bezier_end_point = Vector3Add( Vector3Negate(line_direction) , end_key_frame->previous_bezier_offset);
            bezier_end_point = Vector3Add(bezier_line_end_position , bezier_end_point);
            
			current_bone->state.local_position = vector3_cubic_bezier(bezier_line_start_position , bezier_start_point , bezier_end_point ,bezier_line_end_position ,frame_time );
            
            float ease_time = ease_in_out_cubic(frame_time);
			current_bone->state.local_rotation = QuaternionLerp(start_key_frame->bone_state.local_rotation , end_key_frame->bone_state.local_rotation ,ease_time );
            
		}
	}
    
}

internal void add_bone_state(Bone * bone_array , Bone * add_bone_array , int bone_index)
{
    BoneState bone_state = bone_array[bone_index].state;
    BoneState bone_state_to_add = add_bone_array[bone_index].state;
    
    bone_state.local_position = Vector3Add(bone_state.local_position , bone_state_to_add.local_position);
    bone_state.end_point_offset = Vector3Add(bone_state.end_point_offset , bone_state_to_add.end_point_offset);
    bone_state.local_rotation = QuaternionMultiply(bone_state.local_rotation ,bone_state_to_add.local_rotation );
    
    bone_array[bone_index].state = bone_state;
}

internal void add_multiple_bone_state(Bone * bone_array , Bone * add_bone_array , int bone_array_count)
{
    for(int bone_index = 0 ; bone_index < bone_array_count ; bone_index++)
    {
        add_bone_state(bone_array , add_bone_array , bone_index);
    }
}

internal Bone * get_multiple_bone(int bone_array_count)
{
    Bone * temp_bone = allocate_frame(Bone ,bone_array_count);
    
    for(int bone_index = 0 ; bone_index < bone_array_count ; bone_index++)
    {
        temp_bone[bone_index] = (Bone){};
        temp_bone[bone_index].rotation = QuaternionIdentity();
        temp_bone[bone_index].state.local_rotation = QuaternionIdentity();
    }
    
    return temp_bone;
}

internal Bone * get_bone_pose_offset_from_clip( int clip_index , int target_frame , int target_frame_start, int target_frame_length)
{
    
    Bone * bone_pose_offset = get_multiple_bone(selected_model->bone_count);
    
    for(int bone_index = 0 ; bone_index < selected_model->bone_count ; bone_index++)
    {
        get_bone_from_existing_key_frame(bone_pose_offset ,  clip_array[clip_index], target_frame , target_frame_start , target_frame_length);
    }
    
    return bone_pose_offset;
}

internal float get_corner_weight(float vertical , float horizontal)
{
	if(vertical > 0.5) vertical = 0.5f;
	if(horizontal > 0.5) horizontal = 0.5f;
    
	float result = vertical + horizontal;
    
	result -= 0.5f;
	result *= 2;
    
	if(result < 0) result = 0;
	//printf("v : %f , h : %f , %f \n" , Vertical , Horizontal , Result );
    
	return result;
}

internal void viewport_update()
{
    draw_background();
    
    //Vector3 ray_position = Vector3Add(mouse_ray_3D.position , mouse_ray_3D.direction);
    //draw_billboard_circle(ray_position , 0.1 , YELLOW);
    
    if(editor_type == edit_animation)
    {
        for(int stack_index = 0 ; stack_index < clip_bone_stack_count ; stack_index++)
        {
            ClipBone * current_clip_bone = clip_bone_stack + stack_index;
            Clip * current_clip = clip_array[current_clip_bone->clip_index];
            
            Bone * final_bone = current_clip_bone->final_bone_pose;
            
            update_bone_structure(base_pose_bone);
            
            if(editor_type == edit_animation)
            {
                for(int bone_index = 0 ; bone_index < selected_model->bone_count ; bone_index++)
                {
                    Bone * bone = final_bone + bone_index;
                    if(!bone->IK_enable) continue;
                    if(bone->IK_target_bone_index == -1) continue;
                    if(bone->IK_pole_bone_index == -1) continue;
                    
                    bone_IK_update_B(final_bone , base_pose_bone , bone->IK_target_bone_index , bone->IK_pole_bone_index , bone->bone_index , editor->IK_iteration_count , bone->IK_chain_length);
                }
            }
        }
    }
    
    Bone * bone_pose_to_draw_stack[64] = {};
    int bone_pose_to_draw_stack_count = 0;
    
    if(editor_type == edit_animation)
    {
        for(int stack_index = 0 ; stack_index < clip_bone_stack_count ; stack_index++)
        {
            bone_pose_to_draw_stack[bone_pose_to_draw_stack_count++] = clip_bone_stack[stack_index].final_bone_pose;
        }
    }
    else if(editor_type == edit_base_pose)
    {
        Bone * bone_array = allocate_frame(Bone , selected_model->bone_count);
        for(int bone_index = 0 ; bone_index < selected_model->bone_count ; bone_index++)
        {
            bone_array[bone_index] = selected_model->all_bones[bone_index];
        }
        
        update_bone_structure(bone_array);
        bone_pose_to_draw_stack[bone_pose_to_draw_stack_count++] = bone_array;
    }
    
    glClear(GL_DEPTH_BUFFER_BIT);
    
    Vector3 grid_origin = {};
    if(editor_type == edit_world)
    {
        if(selected_reference_frame_index != -1)
        {
            grid_origin = all_reference_frame[selected_reference_frame_index];
        }
    }
    
    edit_map(grid_origin);
    draw_origin_grid(grid_origin);
    
    D_game_draw();
    
    glClearDepth(1);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    Box origin_box = get_box();
    origin_box.size = (Vector3){UNIT_SIZE , UNIT_SIZE , UNIT_SIZE};
    origin_box.size = Vector3Scale(origin_box.size , 0.5f);
    
    draw_box(origin_box , RED);
    
    Box test_box = get_box();
    test_box.position = (Vector3){0.6,2.2,-5.3};
    //test_box.position = (Vector3){};
    test_box.rotation = QuaternionFromVector3ToVector3( (Vector3){0,1,0} , Vector3Normalize( (Vector3){1,2,3}) );
    test_box.size = (Vector3){0.65,1.36,0.6};
    draw_box(test_box , BLUE);
    
    Vector3 ray_direction = Vector3Subtract( (Vector3){ 1.2 , -0.6 , 0.3} , test_box.position);
    draw_arrow_ray_D( test_box.position , ray_direction , Fade(WHITE , 0.5f));
    
    Vector3 * test_box_vertices = box_to_point(test_box);
    
    if(within_viewport)
    {
        if(editor_type == edit_world || editor_type == demo)
        {
            local_persist Vector3 player_position = {};
            local_persist Vector3 player_velocity = {};
            Box player_box = get_box();
            player_box.position = player_position;
            player_box.size = (Vector3){0.56 , 0.6 , 0.4};
            player_box.rotation = QuaternionFromVector3ToVector3( (Vector3){0,1,0} , Vector3Normalize( (Vector3){0.3,-2.5,1}) );
            Vector3 * player_box_vertices = box_to_point(player_box);
            
            player_velocity = Vector3Add(player_velocity , (Vector3){0,-UNIT_SIZE * 0.05f,0});
            bool player_impacted = false;
            float closest_hit_time = FLT_MAX;
            Vector3 surface_normal = {};
            
            float player_forward = 0;
            float player_right = 0;
            
            if(key_pressing(KEY_W)) player_forward += 1;
            if(key_pressing(KEY_S)) player_forward -= 1;
            if(key_pressing(KEY_D)) player_right += 1;
            if(key_pressing(KEY_A)) player_right -= 1;
            
            player_forward *= UNIT_SIZE * 0.1f;
            player_right *= UNIT_SIZE * 0.1f;
            
            Vector3 player_forward_direction = Vector3Subtract(game_camera.target , game_camera.position);
            Vector3 player_right_direction = Vector3CrossProduct(player_forward_direction , (Vector3){0,1,0});
            player_forward_direction = Vector3CrossProduct((Vector3){0,1,0} , player_right_direction);
            
            player_forward_direction = Vector3Normalize(player_forward_direction);
            player_right_direction = Vector3Normalize(player_right_direction);
            
            player_velocity = Vector3Add(player_velocity , Vector3Scale(player_forward_direction , player_forward));
            player_velocity = Vector3Add(player_velocity , Vector3Scale(player_right_direction , player_right));
            
            if(key_pressed(KEY_SPACE))
            {
                player_velocity = Vector3Add(player_velocity , (Vector3){0,UNIT_SIZE * 2.0f,0});
            }
            
            player_velocity = Vector3Scale(player_velocity , 0.9f);
            
            array_foreach(quad_index , &quads_in_map_array)
            {
                Quad quad = quads_in_map[quad_index];
                
                vertices_b = quad.vertex_position;
                vertices_b_count = quad_vertex_count;
                
                vertices_a = test_box_vertices;
                vertices_a_count = box_vertex_count;
                
#if 0
                for(int vertex_index = 0 ; vertex_index < quad_vertex_count ; vertex_index++)
                {
                    Box temp_box = test_box;
                    temp_box.position = quad.vertex_position[vertex_index];
                    draw_box_line(temp_box , Fade(WHITE, 0.05) , 5);
                }
#endif
                
                ShapeImpactData test_impact = {};
                test_impact.ray_direction = ray_direction;
#if 0
                if(check_shape_impact(&test_impact))
                {
                    Box collided_box = test_box;
                    collided_box.position = Vector3Add(collided_box.position , Vector3Scale(ray_direction , test_impact.time_of_impact));
                    draw_box(collided_box , Fade(BLUE , 0.4f));
                }
#endif
                
                RayCollision collision = get_collision_quad_3D( quad );
                if(collision.hit)
                {
                    draw_quad_line( quad , BLACK , 5);
                }
                
                float player_impact_time = 0;
                float player_impact_position = {};
                
                ShapeImpactData player_impact = {};
                player_impact.ray_direction = player_velocity;
                player_impact.stop_if_too_far = true;
                
                vertices_a = player_box_vertices;
                vertices_a_count = box_vertex_count;
#if 1
                if(check_shape_impact(&player_impact))
                {
                    //float player_impact_time = player_impact.time_of_impact;
                    
                    player_impacted = true;
                    if(closest_hit_time > player_impact.time_of_impact) 
                    {
                        closest_hit_time = player_impact.time_of_impact;
                        surface_normal = player_impact.impact_normal;
                    }
                    
                    draw_quad_D(quad , MAROON);
                }
                else
                {
                    draw_quad_D(quad , PURPLE);
                }
#endif
            }
            
            if(player_impacted)
            {
                Vector3 normal_axis = Vector3CrossProduct(surface_normal , player_velocity);
                
                if(Vector3LengthSqr(normal_axis) > 0.00001f)
                {
                    printf("?? %lld\n" , game_update_count);
                    player_velocity = project_direction_on_plane(surface_normal , player_velocity);
                }
                else
                {
                    if(closest_hit_time >= 0)
                    {
                        player_velocity = Vector3Scale(player_velocity , closest_hit_time);
                    }
                }
            }
            
            player_position = Vector3Add(player_position , player_velocity);
            draw_box(player_box , GOLD);
            draw_box_line(player_box , BLACK , 2);
        }
    }
    
    if(bone_pose_to_draw_stack_count)
    {
        for(int model_index = 0 ; model_index < model_count ; model_index++)
        {
            D_Model * model = all_models + model_index;
            
            update_bone_structure(model->all_initial_bone);
            draw_model(model , bone_pose_to_draw_stack[0] , model->all_initial_bone);
        }
    }
    
    D_game_draw();
    
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    
    for(int stack_index = 0 ; stack_index < bone_pose_to_draw_stack_count ; stack_index++)
    {
        list_foreach(root_bone_index , &selected_model->root_bone_list)
        {
            iterate_and_draw_bone_arrow( bone_pose_to_draw_stack[stack_index] , bone_pose_to_draw_stack[stack_index] + root_bone_index , 0);
        }
    }
    
    if(within_viewport)
    {
        
        bool edit_bone_state = false;
        
        if(editor_type == edit_base_pose) edit_bone_state = true;
        if(editor_type == edit_animation) edit_bone_state = true;
        
        if(editor->assigning_parent_bone) edit_bone_state = false;
        if(editor->assigning_IK_target_bone) edit_bone_state = false;
        if(editor->assigning_IK_pole_bone) edit_bone_state = false;
        
        if(edit_bone_state)
        {
            
            bone_selection_and_edit_bone_state( editor->current_frame_at_timeline);
        }
        
        if(editor_type == edit_world)
        {
            if(selected_reference_frame_index != -1)
            {
                
                Box right_box = get_box();
                Box up_box = get_box();
                Box forward_box = get_box();
                
                right_box.position = Vector3Lerp((Vector3){} , right_direction , 0.5f);
                up_box.position = Vector3Lerp((Vector3){} , up_direction , 0.5f);
                forward_box.position = Vector3Lerp((Vector3){} , forward_direction , 0.5f);
                
                right_box.position = Vector3Add(grid_origin , right_box.position);
                up_box.position = Vector3Add(grid_origin , up_box.position);
                forward_box.position = Vector3Add(grid_origin , forward_box.position);
                
                right_box.size = Vector3Add(right_direction , (Vector3){UNIT_SIZE , UNIT_SIZE , UNIT_SIZE});
                up_box.size = Vector3Add(up_direction , (Vector3){UNIT_SIZE , UNIT_SIZE , UNIT_SIZE});
                forward_box.size = Vector3Add(forward_direction , (Vector3){UNIT_SIZE , UNIT_SIZE , UNIT_SIZE});
                
                Vector3 camera_direction = Vector3Subtract(game_camera.target , game_camera.position);
                camera_direction = Vector3Negate(camera_direction);
                
                Color right_color = Fade(RED , 0.2f);
                Color up_color = Fade(GREEN , 0.2f);
                Color forward_color = Fade(BLUE , 0.2f);
                
                local_persist Vector3 origin_position = {};
                local_persist Vector3 previous_drag_point = {};
                local_persist bool dragging_origin = false;
                local_persist Vector3 drag_direction = {};
                
                Vector3 mouse_ray_position = mouse_ray_3D.position;
                Vector3 mouse_ray_target = Vector3Add(mouse_ray_3D.position , mouse_ray_3D.direction);
                
                float intersect_time = get_line_intersect_with_plane_time( mouse_ray_position , mouse_ray_target , camera_direction , grid_origin);
                Vector3 current_drag_point = Vector3Lerp(mouse_ray_position , mouse_ray_target , intersect_time);
                
                if(mouse_button_pressed_no_check(MOUSE_BUTTON_LEFT))
                {
                    previous_drag_point = current_drag_point;
                    origin_position = grid_origin;
                }
                
                Vector3 drag_offset = Vector3Subtract(current_drag_point , previous_drag_point);
                previous_drag_point = current_drag_point;
                
                if(box_collision_ray(mouse_ray_3D.position , mouse_ray_3D.direction , right_box)) 
                {
                    if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
                    {
                        dragging_origin = true;
                        drag_direction = right_direction;
                    }
                    
                    right_color = Fade(right_color , 0.6);
                }
                
                if(box_collision_ray(mouse_ray_3D.position , mouse_ray_3D.direction , up_box)) 
                {
                    if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
                    {
                        dragging_origin = true;
                        drag_direction = up_direction;
                    }
                    
                    up_color = Fade(up_color , 1);
                }
                
                if(box_collision_ray(mouse_ray_3D.position , mouse_ray_3D.direction , forward_box)) 
                {
                    if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
                    {
                        dragging_origin = true;
                        drag_direction = forward_direction;
                    }
                    
                    forward_color = Fade(forward_color , 1);
                }
                
                if(dragging_origin)
                {
                    if(mouse_button_released(MOUSE_BUTTON_LEFT))
                    {
                        dragging_origin = false;
                    }
                    
                    Vector3 origin_offset = Vector3Project(drag_offset , drag_direction);
                    origin_position = Vector3Add(origin_position , origin_offset);
                    
                    Vector3 position_in_cell = position_to_grid(origin_position , UNIT_SIZE);
                    
                    Vector3 * referece_frame = all_reference_frame + selected_reference_frame_index;
                    
                    (*referece_frame) = position_in_cell;
                    //(*referece_frame) = origin_position;
                }
                
                draw_arrow_ray_D(grid_origin , right_direction , right_color);
                draw_arrow_ray_D(grid_origin , up_direction , up_color);
                draw_arrow_ray_D(grid_origin , forward_direction , forward_color);
            }
        }
    }
    
	render_state.fake_depth = 0;
	D_game_draw();
    
}

internal void game_update()
{
    
    bool window_size_changed = false;
	local_persist Vector2 previous_windows_size = { -1 , -1};
    
	if (previous_windows_size.x != app_data->window_size.x)
	{
		window_size_changed = true;
	}
    
	if (previous_windows_size.y != app_data->window_size.y)
	{
		window_size_changed = true;
	}
    
	previous_windows_size = app_data->window_size;
    
	if (window_size_changed)
	{
		
        //why windows size changed to 0 ???????
		//TODO: wrap rlloadtexture
		if (app_data->window_size.x != 0)
		{
            
			if (app_data->window_size.y != 0)
			{
				
                if (render_state.game_world_depth_texture != 0)
				{
					rlUnloadTexture(render_state.game_world_depth_texture);
				}
                
                //TODO: what was i thinking
                //the texture didn't get clear at all
                if(render_state.game_world_depth_texture != 0)
                {
                    rlUnloadTexture(render_state.game_world_depth_texture);
                    render_state.game_world_depth_texture = load_depth_texture_multi_sample( app_data->window_size.x , app_data->window_size.y );
                }
                else
                {
                    render_state.game_world_depth_texture = load_depth_texture_multi_sample( app_data->window_size.x , app_data->window_size.y );
                    add_to_list_tail(render_state.game_world_depth_texture , &render_state.texture_list_to_delete);
                }
                
				update_texture_multi_sample(&render_state.game_world_texture);
				update_texture_multi_sample(&render_state.interface_texture);
                //update_texture(&render_state.lighting_texture);
				//update_texture(&render_state.bloom_texture);
				//update_texture(&render_state.first_blur_texture);
				//update_texture(&render_state.blur_texture);
                
			}
		}
	}
    
	GL_CATCH;
    
	editor->flat_color = true;
    
	double mouse_x = 0;
	double mouse_y = 0;
    
	glfwGetCursorPos(app_data->current_window, &mouse_x, &mouse_y);
    
	mouse_position = (Vector2){ mouse_x, app_data->window_size.y - mouse_y };
    
	app_data->mouse_position = mouse_position;
	
	if (!stop_mouse_input)
	{
		editor->target_camera_zoom -= app_data->mouse_scroll_delta * 0.5f;
	}
    
	if (editor->target_camera_zoom < 0.01f) editor->target_camera_zoom = 0.01f;
	if (editor->target_camera_zoom > 20.0f) editor->target_camera_zoom = 20.0f;
    
	app_data->top = tan((game_camera.fovy * 0.5) * DEG2RAD);
	app_data->right = app_data->top * (app_data->window_size.x / app_data->window_size.y);
    full_screen_pixel_matrix = MatrixOrtho(0 , app_data->window_size.x , 0 , app_data->window_size.y , screen_near_clip , screen_far_clip);
    
	update_float_input();
    
	editor->previous_mouse_position = editor->current_mouse_position;
	editor->current_mouse_position = app_data->mouse_position;
    
	Vector2 mouse_position_delta = editor->current_mouse_position;
	mouse_position_delta.x -= editor->previous_mouse_position.x;
	mouse_position_delta.y -= editor->previous_mouse_position.y;
    
    if(key_pressed(KEY_UP)) editor->IK_iteration_count++;
    if(key_pressed(KEY_DOWN)) editor->IK_iteration_count--;
    
    if(editor->IK_iteration_count < 0) editor->IK_iteration_count = 0;
    
    base_pose_bone = allocate_frame( Bone , selected_model->bone_count);
    
    for(int bone_index = 0 ; bone_index < selected_model->bone_count ; bone_index++)
    {
        base_pose_bone[bone_index] = selected_model->all_bones[bone_index];
    }
    
    if(add_clip_bone_at_next_frame)
    {
        add_clip_bone_at_next_frame = false;
        clip_bone_stack_count++;
    }
    
    for(int stack_index = 0 ; stack_index < clip_bone_stack_count ; stack_index++)
    {
        
        ClipBone * current_clip_bone = clip_bone_stack + stack_index;
        
        int allocate_bone_count = selected_model->bone_count;
        //TODO:make it not temp
        current_clip_bone->bone_pose_offset = allocate_frame(Bone , allocate_bone_count);
        current_clip_bone->final_bone_pose = allocate_frame(Bone , allocate_bone_count);
        current_clip_bone->selected_bone = allocate_frame(bool , allocate_bone_count);
        current_clip_bone->hovered_bone = allocate_frame(bool , allocate_bone_count);
        
        for(int bone_index = 0 ; bone_index < selected_model->bone_count ; bone_index++)
        {
            
            current_clip_bone->bone_pose_offset[bone_index] = (Bone){};
            current_clip_bone->bone_pose_offset[bone_index].rotation = QuaternionIdentity();
            current_clip_bone->bone_pose_offset[bone_index].state.local_rotation = QuaternionIdentity();
            
            current_clip_bone->final_bone_pose[bone_index] = base_pose_bone[bone_index];
        }
        
    }
    
    for(int stack_index = 0 ; stack_index < clip_bone_stack_count ; stack_index++)
    {
        ClipBone * current_clip_bone = clip_bone_stack + stack_index;
        Clip * current_clip = clip_array[current_clip_bone->clip_index];
        
        if(editor_type == edit_animation)
        {
            Bone * temp_bone = allocate_frame( Bone , selected_model->bone_count);
            
            for(int bone_index = 0 ; bone_index < selected_model->bone_count ; bone_index++)
            {
                temp_bone[bone_index] = (Bone){};
                temp_bone[bone_index].rotation = QuaternionIdentity();
                temp_bone[bone_index].state.local_rotation = QuaternionIdentity();
            }
            
            get_bone_from_existing_key_frame(temp_bone , clip_array[current_clip_bone->clip_index] , editor->current_frame_at_timeline , editor->start_frame_index , editor->timeline_frame_length);
            add_multiple_bone_state(current_clip_bone->bone_pose_offset , temp_bone , selected_model->bone_count);
            
        }
        
        Bone * final_bone = current_clip_bone->final_bone_pose;
        add_multiple_bone_state(final_bone, current_clip_bone->bone_pose_offset , selected_model->bone_count);
        
        update_bone_structure(final_bone);
    }
    
    pruning_3D_line = false;
	change_matrix(full_screen_pixel_matrix);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    glViewport(0 , 0 ,app_data->window_size.x, app_data->window_size.y);
	
	stop_mouse_input = false;
    
    editor_GUI();
    
#if 1
	GL_CATCH;
    
    glBindFramebuffer(GL_FRAMEBUFFER, render_state.screen_frame_buffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D_MULTISAMPLE, render_state.game_world_texture, 0);
    
    GL_CATCH;
    
    if (!rlFramebufferComplete(render_state.screen_frame_buffer)) CATCH;
    
    glBindFramebuffer(GL_FRAMEBUFFER, render_state.screen_frame_buffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, render_state.game_world_depth_texture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
	if (!rlFramebufferComplete(render_state.screen_frame_buffer)) CATCH;
    rlEnableFramebuffer(render_state.screen_frame_buffer);
#endif
    
	GL_CATCH;
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
    
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glDisable(GL_DEPTH_TEST);
    
    float vertical_split = 0.8;
    float horizontal_split = 0.5;
    
    D_Rectangle primary_viewport = {};
    D_Rectangle secondary_viewport = {};
    D_Rectangle tertiary_viewport = {};
    
    primary_viewport.x = 0;
    primary_viewport.y = 0;
    primary_viewport.width = app_data->window_size.x * vertical_split;
    primary_viewport.height = app_data->window_size.y ;
    
    secondary_viewport.x = app_data->window_size.x * vertical_split;
    secondary_viewport.y = app_data->window_size.y * (1.0 - horizontal_split);
    secondary_viewport.width = app_data->window_size.x * (1.0 - vertical_split);
    secondary_viewport.height = app_data->window_size.y * horizontal_split;
    
    tertiary_viewport.x = app_data->window_size.x * vertical_split;
    tertiary_viewport.y = 0;
    tertiary_viewport.width = app_data->window_size.x * (1.0 - vertical_split);
    tertiary_viewport.height = app_data->window_size.y * (1.0 - horizontal_split);
    
    SplitViewport all_viewport[16] = {};
    int viewport_count = 0;
    
    editor->current_camera_zoom = Lerp(editor->current_camera_zoom , editor->target_camera_zoom , 0.2f);
    
#if 1
    SplitViewport * primary_split = all_viewport + viewport_count++;
    primary_split->viewport = primary_viewport;
    primary_split->camera_euler = editor->editor_camera_euler;
    primary_split->camera_offset = editor->editor_camera_offset;
    primary_split->camera_zoom = editor->current_camera_zoom; 
    primary_split->camera_up = (Vector3){0,1,0};
    primary_split->ortho = false;
    
#endif
    
    Vector3 primary_euler = editor->editor_camera_euler;
    Quaternion primary_rotation = QuaternionFromEuler(primary_euler.x * DEG2RAD, primary_euler.y * DEG2RAD, primary_euler.z * DEG2RAD);
    Vector3 primary_camera_forward = {0,0,-editor->current_camera_zoom};
    primary_camera_forward = Vector3RotateByQuaternion(primary_camera_forward , primary_rotation);
    
    SplitViewport * secondary_spilt = all_viewport + viewport_count++;
    secondary_spilt->viewport = secondary_viewport;
    secondary_spilt->camera_euler = editor->editor_camera_euler;
    secondary_spilt->camera_euler.y += 90;
    secondary_spilt->camera_offset = editor->editor_camera_offset;
    secondary_spilt->camera_zoom = editor->current_camera_zoom;
    secondary_spilt->camera_up = (Vector3){0,1,0};
    secondary_spilt->ortho = true;
    
    SplitViewport * tertiary_spilt = all_viewport + viewport_count++;
    tertiary_spilt->viewport = tertiary_viewport;
    tertiary_spilt->camera_euler = editor->editor_camera_euler;
    tertiary_spilt->camera_euler.x += 90;
    tertiary_spilt->camera_offset = editor->editor_camera_offset;
    tertiary_spilt->camera_zoom = editor->current_camera_zoom;
    tertiary_spilt->camera_up = Vector3Negate(Vector3Normalize(primary_camera_forward));
    tertiary_spilt->ortho = true;
    
    for(int viewport_index = 0 ; viewport_index < viewport_count ; viewport_index++)
    {
        
        SplitViewport current_split = all_viewport[viewport_index];
        current_viewport = current_split.viewport;
        
        Quaternion camera_rotation = QuaternionFromEuler(current_split.camera_euler.x * DEG2RAD, current_split.camera_euler.y * DEG2RAD,current_split.camera_euler.z * DEG2RAD);
        float camera_zoom = current_split.camera_zoom;
        
        Vector3 camera_direction = Vector3RotateByQuaternion((Vector3){0,0,-camera_zoom} , camera_rotation);
        game_camera.position = Vector3Add( camera_direction , current_split.camera_offset);
        
        game_camera.target = current_split.camera_offset;
        game_camera.up = current_split.camera_up;
        
        view_matrix = MatrixLookAt(game_camera.position, game_camera.target, game_camera.up);
        double aspect = (double)current_viewport.width / (double)current_viewport.height;
        
        if(!current_split.ortho)
        {
            project_matrix = MatrixPerspective(game_camera.fovy * DEG2RAD, aspect , 0.1, 200);
            mouse_ray_3D = get_screen_to_world_ray_EX( (Vector2){mouse_x , mouse_y} ,  game_camera , current_viewport.x , current_viewport.y , current_viewport.width, current_viewport.height);
        }
        else
        {
            project_matrix = MatrixOrtho(-aspect * camera_zoom , aspect * camera_zoom , -camera_zoom , camera_zoom , 0.1, 200);
            
        }
        
        world_3D_matrix = MatrixMultiply(view_matrix, project_matrix);
        
        viewport_screen_pixel_matrix = MatrixOrtho(current_viewport.x , current_viewport.x + current_viewport.width , current_viewport.y , current_viewport.y + current_viewport.height , screen_near_clip , screen_far_clip);
        
        world_3D_to_screen_matrix = MatrixMultiply(world_3D_matrix , MatrixInvert(viewport_screen_pixel_matrix));
        
        if(current_split.ortho)
        {
            
            mouse_ray_3D.direction = Vector3Negate(Vector3Normalize(camera_direction));
            
            Vector3 mouse_on_screen = {mouse_position.x , mouse_position.y  , 0};
            mouse_on_screen = transform_vector(mouse_on_screen , (viewport_screen_pixel_matrix));
            mouse_on_screen = transform_vector(mouse_on_screen , MatrixInvert(project_matrix));
            mouse_on_screen = transform_vector(mouse_on_screen , MatrixInvert(view_matrix));
            mouse_ray_3D.position = mouse_on_screen;
            
        }
        
        editor->right = (Vector3){view_matrix.m0 , view_matrix.m4 , view_matrix.m8 };
        editor->up = (Vector3){ view_matrix.m1 , view_matrix.m5 , view_matrix.m9 };
        
        editor->billboard_rotation = QuaternionFromVector3ToVector3((Vector3){0,1,0} , editor->up );
        editor->billboard_rotation = QuaternionMultiply( editor->billboard_rotation ,  QuaternionFromVector3ToVector3( (Vector3){1,0,0} , editor->right));
        
        Vector3 camera_offset_result = {};
        Vector3 camera_euler_result = {};
        
        within_viewport = CheckCollisionPointRec(mouse_position , current_viewport);
        
        if(within_viewport)
        {
            
            if (mouse_button_pressing(MOUSE_BUTTON_MIDDLE))
            {
                if (key_pressing(KEY_LEFT_SHIFT))
                {
                    
                    Vector3 mouse_right = Vector3Scale( editor->right , -mouse_position_delta.x * 0.005f);
                    Vector3 mouse_up = Vector3Scale(editor->up , -mouse_position_delta.y * 0.005f);
                    
                    Vector3 mouse_drag_offset = Vector3Add(mouse_right , mouse_up);
                    
                    camera_offset_result = mouse_drag_offset;
                    
                }
                else
                {
                    
                    camera_euler_result.y = -mouse_position_delta.x * 0.1;
                    camera_euler_result.x = -mouse_position_delta.y * 0.1;
                    
                }
            }
            
            editor->editor_camera_offset = Vector3Add(editor->editor_camera_offset , camera_offset_result);
            editor->editor_camera_euler = Vector3Add(editor->editor_camera_euler , camera_euler_result);
            
        }
        
        glViewport(current_viewport.x , current_viewport.y ,current_viewport.width, current_viewport.height);
        
        pruning_3D_line = true;
        change_matrix(world_3D_matrix);
        
        viewport_update();
        
    }
    
    
	glDisable(GL_DEPTH_TEST);
    
	rlDisableFramebuffer();
    
    editor->flat_color = true;
    
    glViewport(0 , 0 ,app_data->window_size.x , app_data->window_size.y);
    
#if 1
	if (editor->flat_color)
	{
        
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        GL_CATCH;
        
		draw_screen_flat_B(render_state.game_world_texture , WHITE , true);
	}
	else
	{
        
#if 0
        
		rlFramebufferAttach(render_state.screen_frame_buffer, render_state.lighting_texture, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
		if (!rlFramebufferComplete(render_state.screen_frame_buffer)) CATCH;
        
		rlEnableFramebuffer(render_state.screen_frame_buffer);
        
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_DST_ALPHA, GL_ONE, GL_ONE);
		glEnable(GL_BLEND);
        
		D_foreach(D_Light, CurrentLight, GameWorld->LightArray)
		{
			Vector4 LightColor = CurrentLight->LightColor;
			LightColor.w *= CurrentLight->LightStrength;
			AddLight(CurrentLight->Lightposition, CurrentLight->LightSize, LightColor);
		}
        
		ShaderInput * LightShaderInput = all_shader_inputs + S_point_light;
        
		rlEnableShader(LightShaderInput->Shader);
		glBindVertexArray(LightShaderInput->VertexArrayObject);
        
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, render_state.game_world_texture);
		glUniform1i(LightShaderInput->UniformLocation[SU_texture_0], 0);
        
		rlSetUniformMatrix(LightShaderInput->UniformLocation[SU_MVP] , MVP_2DWorld);
        
		update_all_GPU_vertex_buffer(LightShaderInput, light_data.light_count);
        
		glDrawArrays(GL_POINTS, 0, light_data.light_count);
        
        
		float TargetLightStrength = 0.6f;
        
		local_persist float LightStrength = 0.3f;
		LightStrength = Lerp(LightStrength , TargetLightStrength , 0.36f);
        
		draw_screen_flat(render_state.game_world_texture, Fade(WHITE, LightStrength ));
        
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		rlDisableFramebuffer();
        light_data.light_count = 0;
        
		rlFramebufferAttach(render_state.screen_frame_buffer, render_state.bloom_texture, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
		if (!rlFramebufferComplete(render_state.screen_frame_buffer)) CATCH;
        
		rlEnableFramebuffer(render_state.screen_frame_buffer);
        
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
		ShaderInput * bloom_shader = all_shader_inputs + S_bloom;
        
		rlEnableShader(bloom_shader->shader);
		glBindVertexArray(bloom_shader->vertex_array_object);
        
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D , render_state.lighting_texture);
		glUniform1i(bloom_shader->uniform_location[SU_texture_0], 0);
        
		glUniform2f(bloom_shader->uniform_location[SU_screen_size] , app_data->window_size.x , app_data->window_size.y);
        
		Vector3 * sreen_quad_pos = vertex_data.position;
		Vector4 * screen_quad_color_data = vertex_data.color;
		Vector2 * screen_quad_texCoord_data = vertex_data.tex_coord;
        
		int vertex_count = 0;
        
		sreen_quad_pos[vertex_count++] = (Vector3){ -1, 1, 0 };
		sreen_quad_pos[vertex_count++] = (Vector3){ 1, 1, 0 };
		sreen_quad_pos[vertex_count++] = (Vector3){ 1, -1, 0 };
        
		sreen_quad_pos[vertex_count++] = (Vector3){ -1, -1, 0 };
		sreen_quad_pos[vertex_count++] = (Vector3){ -1, 1, 0 };
		sreen_quad_pos[vertex_count++] = (Vector3){ 1, -1, 0 };
        
		vertex_count = 0;
        
		screen_quad_texCoord_data[vertex_count++] = (Vector2){ 0, 1 };
		screen_quad_texCoord_data[vertex_count++] = (Vector2){ 1, 1 };
		screen_quad_texCoord_data[vertex_count++] = (Vector2){ 1, 0 };
        
		screen_quad_texCoord_data[vertex_count++] = (Vector2){ 0, 0 };
		screen_quad_texCoord_data[vertex_count++] = (Vector2){ 0, 1 };
		screen_quad_texCoord_data[vertex_count++] = (Vector2){ 1, 0 };
        
		vertex_count = 0;
        
		screen_quad_color_data[vertex_count++] = (Vector4){1,1,1,1};
		screen_quad_color_data[vertex_count++] = (Vector4){1,1,1,1};
		screen_quad_color_data[vertex_count++] = (Vector4){1,1,1,1};
        
		screen_quad_color_data[vertex_count++] = (Vector4){1,1,1,1};
		screen_quad_color_data[vertex_count++] = (Vector4){1,1,1,1};
		screen_quad_color_data[vertex_count++] = (Vector4){1,1,1,1};
        
		update_all_GPU_vertex_buffer(bloom_shader, 6);
		glDrawArrays(GL_TRIANGLES, 0, 6);
        
		rlDisableFramebuffer();
        
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
		update_all_GPU_vertex_buffer(bloom_shader, 6);
        
		for (int shader_index = S_blur_H; shader_index <= S_blur_V; shader_index++)
		{
			unsigned int target_blur_texture = render_state.blur_texture;
            
			if (shader_index == S_blur_H) target_blur_texture = render_state.first_blur_texture;
            
			rlFramebufferAttach(render_state.screen_frame_buffer, target_blur_texture , RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
			if (!rlFramebufferComplete(render_state.screen_frame_buffer)) CATCH;
            
			rlEnableFramebuffer(render_state.screen_frame_buffer);
            
			ShaderInput * blur_shader = all_shader_inputs + shader_index;
            
			rlEnableShader(blur_shader ->shader);
			glBindVertexArray(blur_shader ->vertex_array_object);
            
			unsigned int base_texture = render_state.first_blur_texture;
			if (shader_index == S_blur_H) base_texture = render_state.bloom_texture;
            
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D , base_texture);
			glUniform1i(blur_shader ->uniform_location[SU_texture_0], 0);
            
			Vector2 screen_world = {app_data->window_size.x , app_data->window_size.y};
            
			glUniform2f(blur_shader->uniform_location[SU_screen_size] , 1.0f / app_data->window_size.x , 1.0f / app_data->window_size.x);
			glUniform1f(blur_shader->uniform_location[SU_grid_size] ,11);
			glUniform1f(blur_shader->uniform_location[SU_spread] , 10);
            
			//i just reusing data already exist in buffer
			update_all_GPU_vertex_buffer(blur_shader , 6);
			glDrawArrays(GL_TRIANGLES, 0, 6);
            
			rlDisableFramebuffer();
		}
        
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_DST_ALPHA, GL_ONE, GL_ONE);
        
		draw_screen_flat_B(render_state.lighting_texture ,Fade(WHITE,1.0f) , false);
		draw_screen_flat_B(render_state.blur_texture ,Fade(WHITE,0.4f), false);
        
		GL_CATCH;
        
#endif
        
	}
#endif
    
    
    pruning_3D_line = false;
	change_matrix(full_screen_pixel_matrix);
	glBlendFunc(GL_ONE , GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
    
	//i just reusing texture
	//maybe i should reuse more?
	draw_screen_flat_B(render_state.interface_texture , WHITE , true);
    
	D_game_draw();
    
	render_state.elapsed_time += DeltaTime;
    
}

internal GAME_LOOP(game_loop)
{
    if (shader_compile_failed) return;
    
    local_persist bool initialized_timer = false;
    local_persist double update_timer = 0;
    local_persist double previous_loop_time = 0;
    local_persist double elapsed_time = 0;
    
    if(!initialized_timer)
    {
        initialized_timer = true;
        previous_loop_time = time_stamp();
    }
    
    double current_time = time_stamp();
    elapsed_time += (current_time - previous_loop_time) / (1000.0 * 1000.0);
    
    double update_time = 1.0f / (double)D_UPDATE_RATE;
    
    //it got interuped by debugger or something
    if (elapsed_time > update_time * 10) elapsed_time = 0;
    
    int update_count = 0;
    
    for (;elapsed_time > update_time ; elapsed_time -= update_time )
    {
		app_data->mouse_scroll_delta = 0;
		app_data->codepoint_queue_count = 0;
		app_data->codepoint_queue_indedx = 0;
		app_data->pressed_mouse_array_count = 0;
		app_data->released_mouse_array_count = 0;
		app_data->pressed_key_array_count = 0;
        app_data->released_key_array_count = 0;
        
        glfwPollEvents();
        
        double game_update_time = time_stamp();
        game_update();
        game_update_count++;
        
        double game_upate_time_delta = (time_stamp()-game_update_time) / (1000.0 * 1000.0);
        
        if (game_upate_time_delta > update_time)
        {
            printf( "update is lagging : %f \n", game_upate_time_delta );
        }
        
        app_data->frame_time_memory.current_memory = app_data->frame_time_memory.start_memory;
        glfwSwapBuffers(app_data->current_window);
        
    }
    
    previous_loop_time = current_time;
    
    //these things doesn't help
    //it still lock on 30 fps
    
    //glFinish();
    
    //this things freeze opengl ?
    //SwapBuffers(AppData->_HDC);
    
}

internal bool compare_string_C(char * string_A, char * string_B , int count)
{
    
    for(int char_index = 0 ; char_index < count ; char_index++)
    {
        if(string_A[char_index] != string_B[char_index])
        {
            return false;
        }
    }
    
    return true;
}

internal bool compare_string_W( wchar_t * string_A , wchar_t * string_B)
{
    
    int string_B_length = 0;
    for(int char_index = 0 ; string_B[char_index] != '\0' ; char_index++ ,string_B_length++);
    
    int name_length = 0;
    for(int char_index = 0 ; string_A[char_index] != '\0' ; char_index++ , name_length++);
    
    if(name_length != string_B_length) return false;
    
    for(int char_index = 0 ; char_index < name_length ; char_index++)
    {
        if(string_A[char_index] != string_B[char_index])
        {
            return false;
        }
    }
    
    return true;
}

internal bool compare_string( char * string_A, char * string_B)
{
    
    int string_B_length = 0;
    for(int char_index = 0 ; string_B[char_index] != '\0' ; char_index++ ,string_B_length++);
    
    int name_length = 0;
    for(int char_index = 0 ; string_A[char_index] != '\0' ; char_index++ , name_length++);
    
    if(name_length != string_B_length) return false;
    
    for(int char_index = 0 ; char_index < name_length ; char_index++)
    {
        if(string_A[char_index] != string_B[char_index])
        {
            return false;
        }
    }
    
    return true;
}

#define read_buffer(data_to_assign , name , type , index) \
{\
local_persist bool initialized = false;\
local_persist type * buffer = 0;\
if(!initialized) {initialized = true; buffer = (type *)get_data_buffer_by_name(name);}\
if(buffer) (data_to_assign) = buffer[index];\
}

internal unsigned char * get_data_buffer_by_name(char * name)
{
    
    for(int header_index = 0 ; header_index < save_header_count ; header_index++)
    {
        DataHeader * current_header = data_header_array + header_index;
        
        if(compare_string(current_header->name.string , name))
        {
            return save_memory + current_header->data_offset;
        }
        
    }
    
    return 0;
    
}

#define read_data(data , name , type) { type * data_pointer = (type *)get_data_buffer_by_name(name) ; if(data_pointer) data = (*data_pointer); }

#define allocate_to_file(name , type , count) (type *)allocate_to_file_(name , sizeof(type) , count )

#define write_buffer( data , name , type , index ,  count) \
{ \
local_persist bool initialized = false;\
local_persist type * buffer = 0;\
if(!initialized) { initialized = true ; buffer = allocate_to_file(name , type , count); }\
buffer[index] = data;\
}

#define write_data( data , name , type ) (*(type*)allocate_to_file(name  , type , 1)) = data

internal unsigned char * allocate_to_file_(char * name , int size , int count)
{
    
    DataHeader * new_data_header = add_data_header();
    
    strcpy(new_data_header->name.string , name);
    new_data_header->data_offset = (current_save_memory_location - save_memory);
    
    return allocate_save_(size * count);
    
}

internal void save_file()
{
    
    save_memory = malloc(MAX_SAVE_SIZE);
    current_save_memory_location = save_memory;
    
    data_header_array = malloc(MAX_HEADER_SIZE);
    current_data_header = data_header_array;
    
    write_data(editor_type , "editor_type" , int);
    write_data(editor->current_frame_at_timeline , "current_frame_index" , int);
    write_data(editor->start_frame_index , "start_frame_index" , int);
    write_data(editor->timeline_frame_length , "timeline_frame_length" , int);
    
    write_data(editor->selected_clip_index , "editor_selected_clip_index" , int);
    write_data(editor->editor_camera_offset , "editor_camera_offset" , Vector3);
    write_data(editor->editor_camera_euler , "editor_camera_euler" , Vector3);
    
    write_data(editor->target_camera_zoom , "editor_target_zoom" , float);
    write_data(editor->current_camera_zoom , "editor_current_zoom" , float);
    
    int clip_count = 0;
    int key_frame_count = 0;
    for(int clip_index = 0 ; clip_index < MAX_CLIP; clip_index++) 
    {
        Clip * current_clip = clip_array[clip_index];
        
        if(current_clip)
        {
            clip_count++;
            
            current_clip->key_frame_count_to_save = 0;
            list_foreach(key_frame_index , &current_clip->key_frame_active_list) 
            {
                current_clip->key_frame_count_to_save++;
                key_frame_count++;
            }
        }
    }
    
    write_data( clip_count , "clip_count" , int);
    write_data( all_key_frame_count , "key_frame_count" , int);
    write_data( selected_model->bone_count , "base_bone_count" , int );
    
    for(int clip_array_index = 0 , clip_index = 0 , key_frame_index = 0 
        ; clip_array_index < MAX_CLIP ; 
        clip_array_index++)
    {
        
        Clip * current_clip = clip_array[clip_array_index];
        
        if(!current_clip) continue;
        
        write_buffer(current_clip->clip_name ,   "clip_name" ,FixedString ,clip_index , clip_count);
        write_buffer(current_clip->clip_index , "clip_index" ,  int ,clip_index , clip_count);
        write_buffer(current_clip->not_loop ,"clip_not_loop" , bool , clip_index ,  clip_count);
        
        write_buffer(key_frame_index , "clip_key_frame_offset" , int ,clip_index , clip_count);
        write_buffer(current_clip->key_frame_count_to_save , "clip_key_frame_count" , int ,clip_index , clip_count);
        
        list_foreach(array_index , &current_clip->key_frame_active_list)
        {
            
            KeyFrame * key_frame = all_key_frame + array_index;
            
            write_buffer(key_frame->bone_index , "bone_index" , int ,key_frame_index ,  key_frame_count);
            write_buffer(key_frame->bone_state.local_position , "local_position" ,  Vector3 ,key_frame_index , key_frame_count);
            write_buffer(key_frame->bone_state.local_rotation , "local_rotation" , Quaternion, key_frame_index  , key_frame_count);
            write_buffer(key_frame->bone_state.end_point_offset , "end_point_offset"  , Vector3 , key_frame_index, key_frame_count);
            write_buffer(key_frame->frame_index , "frame_index" , int , key_frame_index , key_frame_count);
            write_buffer(key_frame->next_bezier_offset , "next_bezier_offset" , Vector3 ,key_frame_index ,  key_frame_count);
            write_buffer(key_frame->previous_bezier_offset ,"previous_bezier_offset" , Vector3 , key_frame_index ,  key_frame_count);
            
            key_frame_index++;
            
        }
        
        clip_index++;
    }
    
    for(int bone_index = 0 ; bone_index < selected_model->bone_count ; bone_index++)
    {
        Bone * current_bone = selected_model->all_bones + bone_index;
        
        FixedStringW fixed_string_name = {};
        fixed_string_name = current_bone->bone_name;
        
        write_buffer(fixed_string_name ,  "base_bone_name" , FixedStringW ,bone_index , selected_model->bone_count);
        write_buffer(current_bone->from_blend_file , "bone_from_blend" , bool , bone_index , selected_model->bone_count);
        write_buffer(current_bone->IK_enable , "IK_enable" , bool , bone_index , selected_model->bone_count);
        write_buffer(current_bone->IK_chain_length , "IK_chain_length" , int , bone_index , selected_model->bone_count);
        write_buffer(current_bone->IK_target_bone_index , "IK_target" , int , bone_index , selected_model->bone_count);
        write_buffer(current_bone->IK_pole_bone_index , "IK_pole" , int , bone_index , selected_model->bone_count);
        write_buffer(current_bone->free_position , "base_bone_free_position" , bool , bone_index , selected_model->bone_count);
        
        write_buffer(current_bone->state.local_position ,"base_bone_position" , Vector3 , bone_index ,  selected_model->bone_count);
        write_buffer(current_bone->state.local_rotation ,  "base_bone_quaternion" , Quaternion ,bone_index , selected_model->bone_count);
        write_buffer(current_bone->state.end_point_offset ,  "base_bone_end_point_offset" , Vector3 ,bone_index , selected_model->bone_count);
        
        write_buffer(current_bone->parent_bone_index , "base_bone_parent" , int , bone_index , selected_model->bone_count);
    }
    
    int quads_count = 0;
    
    array_foreach( quad_index , &quads_in_map_array )
    {
        quads_count++;
    }
    
    write_data(quads_count , "map_quad_count" , int);
    
    int quad_index = 0;
    array_foreach( array_index , &quads_in_map_array )
    {
        Quad quad = quads_in_map[array_index];
        
        write_buffer(quad.vertex_position[vertex_top_left] , "map_quad_top_left_vertex" , Vector3 , quad_index , quads_count);
        write_buffer(quad.vertex_position[vertex_top_right] , "map_quad_top_right_vertex" , Vector3 , quad_index , quads_count);
        write_buffer(quad.vertex_position[vertex_bottom_left] , "map_quad_bottom_left_vertex" , Vector3 , quad_index , quads_count);
        write_buffer(quad.vertex_position[vertex_bottom_right] , "map_quad_bottom_right_vertex" , Vector3 , quad_index , quads_count);
        quad_index++;
    }
    
    write_data(selected_reference_frame_index , "selected reference frame" , int);
    
    int reference_frame_count = 0;
    list_foreach(array_index , &reference_frame_list)
    {
        reference_frame_count++;
    }
    
    write_data(reference_frame_count , "reference frame count" , int);
    int reference_frame_index = 0;
    list_foreach(array_index , &reference_frame_list)
    {
        Vector3 reference_frame = all_reference_frame[array_index];
        write_buffer(reference_frame , "reference frame" , Vector3 , reference_frame_index , reference_frame_count);
        reference_frame_index++;
    }
    
    int save_header_size = (current_data_header - data_header_array) * sizeof(DataHeader);
    int save_size = current_save_memory_location - save_memory;
    
    FILE * game_save_file = fopen(get_app_file_path("Game\\save.an") , "wb");
    
    fwrite(&save_header_size , sizeof(int) , 1, game_save_file);
    fwrite(&save_size , sizeof(int) , 1, game_save_file);
    
    fwrite( data_header_array , save_header_size , 1, game_save_file);
    fwrite( save_memory , save_size , 1, game_save_file);
    
    fclose(game_save_file);
    
    free(save_memory);
}

internal int reassign_bone_index(int previous_bone_index)
{
    if(previous_bone_index == -1) return -1;
    
    FixedStringW previous_bone_name = {};
    read_buffer(previous_bone_name , "base_bone_name" , FixedStringW , previous_bone_index);
    
    int parent_index = -1;
    
    for(int current_bone_index = 0 ; current_bone_index < selected_model->bone_count ; current_bone_index++)
    {
        Bone * parent_bone = selected_model->all_bones + current_bone_index;
        if(compare_string_W( previous_bone_name.string , parent_bone->bone_name.string ))
        {
            parent_index = current_bone_index;
            break;
        }
    }
    
    return parent_index;
}

//TODO: reassign bone
internal void load_file()
{
    
    FILE * game_save_file = fopen(get_app_file_path("Game\\save.an") , "rb");
    
    if(!game_save_file) return;
    
    int save_header_size = 0;
    fread(&save_header_size , sizeof(int), 1, game_save_file);
    
    save_header_count = save_header_size / sizeof(DataHeader);
    
    int save_size = 0;
    fread(&save_size , sizeof(int), 1, game_save_file);
    
    unsigned char * whole_data = malloc(save_header_size + save_size);
    
    fread(whole_data , save_header_size + save_size , 1 , game_save_file);
    
    fclose(game_save_file);
    
    data_header_array = (DataHeader*)whole_data;
    current_data_header = data_header_array;
    
    save_memory = whole_data + save_header_size;
    current_save_memory_location = save_memory;
    
    read_data(editor_type , "editor_type" , int);
    read_data(editor->current_frame_at_timeline , "current_frame_index" , int);
    read_data(editor->timeline_frame_length , "timeline_frame_length" , int);
    read_data(editor->start_frame_index , "start_frame_index" , int);
    
    read_data(editor->selected_clip_index , "editor_selected_clip_index" , int);
    read_data(editor->editor_camera_offset , "editor_camera_offset" , Vector3);
    read_data(editor->editor_camera_euler , "editor_camera_euler" , Vector3);
    read_data(editor->target_camera_zoom , "editor_target_zoom" , float);
    read_data(editor->current_camera_zoom , "editor_current_zoom" , float);
    
    int previous_key_frame_count = 0;
    read_data(previous_key_frame_count , "key_frame_count" , int);
    
    all_key_frame_count = previous_key_frame_count;
    for(;all_key_frame_capacity < all_key_frame_count;all_key_frame_capacity*=2);
    all_key_frame = allocate_temp(KeyFrame , all_key_frame_capacity);
    
    for(int key_frame_index = 0 ; key_frame_index < all_key_frame_count ; key_frame_index++)
    {
        int key_frame_owner_clip_index = -1;
        
        KeyFrame * key_frame = all_key_frame + key_frame_index;
        
        read_buffer(key_frame->bone_index , "bone_index" , int ,key_frame_index);
        read_buffer(key_frame->bone_state.local_position , "local_position" ,  Vector3 ,key_frame_index);
        read_buffer(key_frame->bone_state.local_rotation , "local_rotation" , Quaternion, key_frame_index);
        read_buffer(key_frame->bone_state.end_point_offset , "end_point_offset"  , Vector3 , key_frame_index);
        read_buffer(key_frame->frame_index , "frame_index" , int , key_frame_index);
        read_buffer(key_frame->next_bezier_offset , "next_bezier_offset" , Vector3 ,key_frame_index);
        read_buffer(key_frame->previous_bezier_offset ,"previous_bezier_offset" , Vector3 , key_frame_index);
    }
    
    int clip_count = -1;
    read_data( clip_count , "clip_count" , int);
    Clip * new_clip_array = allocate_temp(Clip , clip_count);
    
    for(int clip_index = 0 ; clip_index < clip_count ; clip_index++)
    {
        
        clip_array[clip_index] = new_clip_array + clip_index;
        
        Clip * new_clip = new_clip_array + clip_index;
        new_clip->key_frame_active_list = allocate_list(CLIP_START_CAPACITY);
        new_clip->dragging_key_frame_list = allocate_list(CLIP_START_CAPACITY);
        new_clip->key_frame_hash_table_by_bone = allocate_hash_table(CLIP_START_CAPACITY);
        new_clip->key_frame_hash_table = allocate_hash_table(CLIP_START_CAPACITY);
        
        read_buffer(new_clip->clip_name ,   "clip_name" ,FixedString ,clip_index );
        read_buffer(new_clip->clip_index , "clip_index" ,  int ,clip_index );
        read_buffer(new_clip->not_loop ,"clip_not_loop" , bool , clip_index );
        
        int key_frame_start = -999999999;
        int key_frame_end = -999999999;
        
        read_buffer(key_frame_start , "clip_key_frame_offset" , int ,clip_index);
        read_buffer(key_frame_end , "clip_key_frame_count" , int ,clip_index);
        
        key_frame_end += key_frame_start;
        
        for(int key_frame_index = key_frame_start ; key_frame_index < key_frame_end ; key_frame_index++)
        {
            KeyFrame * key_frame = all_key_frame + key_frame_index;
            
            add_to_list_tail( key_frame_index , &new_clip->key_frame_active_list);
            
            add_to_hash_table(key_frame->bone_index , key_frame_index , &new_clip->key_frame_hash_table_by_bone);
            sort_bone_hash_table(key_frame->bone_index , &new_clip->key_frame_hash_table_by_bone);
            
            add_to_hash_table(GetKeyFrameHash(key_frame->frame_index , key_frame->bone_index) , key_frame_index , &new_clip->key_frame_hash_table);
        }
        
        //read_buffer(new_clip->key_frame_array_count ,"clip_key_frame_count" , int , clip_index);
        //new_clip->key_frame_array = allocate_temp(KeyFrame , new_clip->key_frame_count);
        
    }
    
    int previous_bone_count = -1;
    read_data(previous_bone_count , "base_bone_count",int);
    
#if 1
    for(int previous_bone_index = 0 ; previous_bone_index < previous_bone_count ; previous_bone_index++)
    {
        
        FixedStringW previous_name = {};
        bool is_from_blend_file = 0;
        
        read_buffer(previous_name , "base_bone_name" , FixedStringW , previous_bone_index);
        read_buffer(is_from_blend_file , "bone_from_blend" , bool , previous_bone_index);
        
        Bone* bone_to_assign = 0;
        
        if(!is_from_blend_file)
        {
            REALLOCATE_BUFFER_IF_TOO_SMALL(Bone , selected_model->all_bones , selected_model->bone_count , selected_model->bone_capacity , allocate_temp_);
            REALLOCATE_BUFFER_IF_TOO_SMALL(Bone , selected_model->all_initial_bone , selected_model->initial_bone_count , selected_model->initial_bone_capacity , allocate_temp_);
            
            selected_model->initial_bone_count++;
            int new_bone_index = selected_model->bone_count++;
            bone_to_assign = selected_model->all_bones + new_bone_index;
            bone_to_assign->bone_name = previous_name;
            bone_to_assign->from_blend_file = is_from_blend_file;
            bone_to_assign->bone_index = new_bone_index;
            
        }
        else
        {
            
            for(int current_bone_index = 0 ; current_bone_index < selected_model->bone_count ; current_bone_index++)
            {
                Bone * current_bone = selected_model->all_bones + current_bone_index;
                if(compare_string_W(current_bone->bone_name.string , previous_name.string))
                {
                    bone_to_assign = current_bone;
                    break;
                }
            }
            
        }
        
        read_buffer(bone_to_assign->IK_enable , "IK_enable" , bool , previous_bone_index);
        read_buffer(bone_to_assign->IK_chain_length , "IK_chain_length" , int , previous_bone_index);
        read_buffer(bone_to_assign->IK_target_bone_index , "IK_target" , int , previous_bone_index);
        read_buffer(bone_to_assign->IK_pole_bone_index , "IK_pole" , int , previous_bone_index);
        read_buffer(bone_to_assign->free_position , "base_bone_free_position" , bool , previous_bone_index);
        
        if(!bone_to_assign->from_blend_file)
        {
            
            read_buffer(bone_to_assign->state.local_position ,"base_bone_position" , Vector3 , previous_bone_index);
            read_buffer(bone_to_assign->state.local_rotation ,  "base_bone_quaternion" , Quaternion ,previous_bone_index);
            read_buffer(bone_to_assign->state.end_point_offset ,  "base_bone_end_point_offset" , Vector3 ,previous_bone_index);
            read_buffer(bone_to_assign->parent_bone_index , "base_bone_parent" , int , previous_bone_index);
            
        }
    }
    
    for(int bone_index = 0 ; bone_index < selected_model->bone_count ; bone_index++)
    {
        Bone * current_bone = selected_model->all_bones + bone_index;
        
        current_bone->IK_target_bone_index = reassign_bone_index(current_bone->IK_target_bone_index);
        current_bone->IK_pole_bone_index = reassign_bone_index(current_bone->IK_pole_bone_index);
        
        if(!current_bone->from_blend_file)
        {
            current_bone->parent_bone_index = reassign_bone_index(current_bone->parent_bone_index);
            
            if(current_bone->parent_bone_index == -1)
            {
                add_to_list_tail(bone_index, &selected_model->root_bone_list);
            }
            else
            {
                add_to_hash_table( current_bone->parent_bone_index , bone_index , &selected_model->bone_children_hash_table);
            }
            
        }
    }
    
    int quads_count = 0;
    int quads_capacity = 1;
    read_data(quads_count , "map_quad_count" , int);
    for( ;quads_capacity < quads_count; quads_capacity *=2 );
    quads_in_map_array = allocate_array(quads_capacity);
    quads_in_map = allocate_temp(Quad , quads_capacity);
    
    for(int quad_index = 0 ; quad_index < quads_count ; quad_index++)
    {
        Quad * quad = quads_in_map + add_to_array(&quads_in_map_array);
        
        read_buffer(quad->vertex_position[vertex_top_left] , "map_quad_top_left_vertex" , Vector3 , quad_index);
        read_buffer(quad->vertex_position[vertex_top_right] , "map_quad_top_right_vertex" , Vector3 , quad_index);
        read_buffer(quad->vertex_position[vertex_bottom_left] , "map_quad_bottom_left_vertex" , Vector3 , quad_index);
        read_buffer(quad->vertex_position[vertex_bottom_right] , "map_quad_bottom_right_vertex" , Vector3 , quad_index);
    }
    
#endif
    
    read_data(selected_reference_frame_index , "selected reference frame" , int);
    
    int reference_frame_count = 0;
    read_data(reference_frame_count , "reference frame count" , int);
    
    for(int reference_frame_index = 0 ; reference_frame_index < reference_frame_count ; reference_frame_index++)
    {
        Vector3 refernce_frame = {};
        read_buffer(refernce_frame , "reference frame" , Vector3 , reference_frame_index);
        
        REALLOCATE_LIST_IF_TOO_SMALL(Vector3 , all_reference_frame , &reference_frame_list);
        int new_reference_frame_index= add_to_list_tail_B(&reference_frame_list);
        all_reference_frame[new_reference_frame_index] = refernce_frame;
    }
    
    free(whole_data);
}

internal GAME_UNLOAD(game_unload)
{
    
    mkdir(get_app_file_path("Game"));
    
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    glfwSwapBuffers(app_data->current_window);
    
    if (!shader_compile_failed)
    {
        save_file();
    }
    
    if (render_state.screen_frame_buffer != 0)
    {
        rlUnloadFramebuffer(render_state.screen_frame_buffer); //do i need to delete you?
    }
    
    list_foreach(texture_id, &render_state.texture_list_to_delete)
    {
        rlUnloadTexture(texture_id);
    }
    
    glDeleteTextures(1, &app_data->default_font.glyph_atlas_texture.id);
    
    for (int buffer_index = 0; buffer_index < shader_buffer_count; buffer_index++)
    {
        glDeleteBuffers(1, &all_GPU_buffers[buffer_index]->buffer_handle);
    }
    
    for (int shader_index = 0; shader_index < S_count; shader_index++)
    {
        glDeleteProgram(all_shader_inputs[shader_index].shader);
    }
    
    free(_type_struct_meta_);
    free(app_data->frame_time_memory.start_memory);
    free(app_data->run_time_memory.start_memory);
    
}

internal void game_init()
{
	
    game_camera.target = (Vector3){};
	game_camera.position = (Vector3){ 0,0,-1 };
    game_camera.projection = CAMERA_PERSPECTIVE;
	game_camera.up = (Vector3){ 0,1,0 };
	game_camera.fovy = 90;
    
    timeBeginPeriod(1);
    
    create_a_whole_new_world();
    
    double blend_file_load_time = time_stamp();
    load_blend_file();//hope this doesn't screw me
    printf("\nBlend. File Load Time: %f\n" , (time_stamp() - blend_file_load_time) / (1000.0 * 1000.0));
    
    render_state_init();
    shader_init();
    
    if (shader_compile_failed) return;
    
    double game_load_time = time_stamp();
    load_file();
    printf("\nGame Load Time: %f\n" , (time_stamp() - game_load_time) / (1000.0 * 1000.0));
    
    for(int bone_index = 0 ; bone_index < selected_model->bone_count ; bone_index++)
    {
        selected_model->all_initial_bone[bone_index] = selected_model->all_bones[bone_index];
    }
    
    Image default_white_image = GenImageColor(20, 20, WHITE);
    render_state.default_white_image = D_load_texture_from_image(default_white_image);
    UnloadImage(default_white_image);
    
    Image missing_texture = GenImageCellular(20, 20, 2);
    render_state.missing_texture = D_load_texture_from_image(missing_texture);
    UnloadImage(missing_texture);
    
    //make_a_stick_man();
    
    box_rect[face_top].rotation = QuaternionFromEuler(90 * DEG2RAD , 0 , 0);
    box_rect[face_bottom].rotation = QuaternionFromEuler( -90 * DEG2RAD , 0 , 0);
    box_rect[face_right].rotation = QuaternionFromEuler(0 , 90 * DEG2RAD , 0);
    box_rect[face_left].rotation = QuaternionFromEuler(0 , -90 * DEG2RAD , 0);
    box_rect[face_front].rotation = QuaternionFromEuler(180 * DEG2RAD , 0 , 0);
    box_rect[face_back].rotation = QuaternionFromEuler(0 , 0 , 0);
    
    if(editor->selected_clip_index != -1)
    {
        if(clip_array[editor->selected_clip_index])
        {
            if(selected_model->all_bones)
            {
                
                ClipBone * new_clip_bone = clip_bone_stack + clip_bone_stack_count++;
                (*new_clip_bone) = (ClipBone){};
                
            }
            
        }
    }
    
}

extern GAME_LOAD(game_load)
{
	
    app_data = _AppData;
    app_data->game_loop = game_loop;
    app_data->game_unload = game_unload;
    
    app_data->frame_time_memory = (TemporayMemory){};
    app_data->frame_time_memory.size = 1024 * 1024 * 128;
    app_data->frame_time_memory.start_memory = (unsigned char*)malloc(app_data->frame_time_memory.size);
    app_data->frame_time_memory.current_memory = app_data->frame_time_memory.start_memory;
    
    app_data->run_time_memory = (TemporayMemory){};
    app_data->run_time_memory.size = 1024 * 1024 * 128;
    app_data->run_time_memory.start_memory = (unsigned char*)malloc(app_data->run_time_memory.size);
    app_data->run_time_memory.current_memory = app_data->run_time_memory.start_memory;
    
    _type_struct_meta_ = get_all_type_member_info();
    
    create_struct_name_string_hash();
    game_init();
    
}