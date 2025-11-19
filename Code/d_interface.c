internal void animation_timeline_GUI()
{
    
	Rect timeline_rect = get_rect();
	timeline_rect.position = (Vector3){app_data->window_size.x / 2 , 0};
	timeline_rect.size = (Vector2){ app_data->window_size.x - 60, 80};
	timeline_rect.position.y += timeline_rect.size.y * 0.5f;
	timeline_rect.position.y += 40;
    
	draw_rect_D(timeline_rect , 5 , Fade(BLACK , 0.2f));
    
	local_persist bool dragging_frame_pointer = false;
    
	if (mouse_button_released(MOUSE_BUTTON_LEFT))
	{
		dragging_frame_pointer = false;
	}
    
	float frame_position_offset_x = 20 * editor->timeline_scale;
    
	if (check_collision_rect_mouse(timeline_rect))
	{
		if (key_pressing(KEY_LEFT_CONTROL))
		{
			editor->timeline_scale += app_data->mouse_scroll_delta * 0.1f;
		}
		else
		{
			editor->timeline_slider_offset += app_data->mouse_scroll_delta * 0.1f;
		}
	}
    
	int scroll_offset = -editor->timeline_slider_offset / frame_position_offset_x;
    scroll_offset += editor->start_frame_index;
    
	if (scroll_offset < 0) scroll_offset = 0;
    
    float closest_frame_x = FLT_MAX;
    
	local_persist Vector3 pointer_position = {};
    
	int closest_frame_index_to_mouse = -1;
    
	for (int frame_index = scroll_offset ; ; frame_index++)
	{
		
        if (frame_index > (editor->start_frame_index + editor->timeline_frame_length - 1)) break;
        
		Rect frame_rect = get_rect();
		Vector3 frame_position = timeline_rect.position;
		frame_position.x += frame_index * frame_position_offset_x - timeline_rect.size.x * 0.5f + frame_position_offset_x * 0.5f + editor->timeline_slider_offset;
        
		if (frame_position.x > timeline_rect.size.x * 0.5 + timeline_rect.position.x) break;
        
		frame_rect.position = frame_position;
		frame_rect.size = (Vector2){ 2 , timeline_rect.size.y*0.8f};
        
		draw_rect_D(frame_rect,5,Fade(BLACK , 0.2f));
        
		wchar_t frame_index_string[64] = {};
		_swprintf(frame_index_string , L"%d" , frame_index);
        
		Rect text_rect = frame_rect;
		text_rect.size.y = 6;
        
		D_draw_text_B(text_rect, frame_index_string , WHITE  , true);
        
		float frame_to_mouse_x = mouse_position.x - frame_rect.position.x;
		if (frame_to_mouse_x < 0) frame_to_mouse_x *= -1;
        
		if (closest_frame_x > frame_to_mouse_x)
		{
			closest_frame_x = frame_to_mouse_x;
			closest_frame_index_to_mouse = frame_index;
		}
        
		if (frame_index == editor->current_frame_at_timeline)
		{
			pointer_position = frame_rect.position;
		}
        
    }
    
	if (dragging_frame_pointer)
	{
		stop_mouse_input = true;
        
        editor->current_frame_at_timeline = closest_frame_index_to_mouse;
	}
    
    local_persist bool dragging_key_frame = false;
    
    //DrawKeyFrame
	for (int frame_index = scroll_offset;; frame_index++)
	{
		if (frame_index > editor->start_frame_index + editor->timeline_frame_length - 1) break;
        
        for(int stack_index = 0 ; stack_index < editor->selected_bone_count ; stack_index++)
        {
            BoneSelection * current_bone_selection = editor->selected_bone_stack + stack_index;
            
            Vector3 frame_position = timeline_rect.position;
            frame_position.y += timeline_rect.size.y * 0.4f;
			frame_position.x += frame_index * frame_position_offset_x - timeline_rect.size.x * 0.5f + frame_position_offset_x * 0.5f + editor->timeline_slider_offset;
            
            int bone_index = current_bone_selection->bone_index;
            Clip * clip = clip_array[current_bone_selection->clip_index];
            
            hash_table_iterate(key_frame_index , GetKeyFrameHash(frame_index , bone_index), &clip->key_frame_hash_table)
            {
                
                KeyFrame * key_frame = all_key_frame + key_frame_index;
                
                if(key_frame->bone_index != bone_index) continue;
                if (key_frame->frame_index != frame_index) continue;
                
                Rect key_frame_rect = get_rect();
                key_frame_rect.position = frame_position;
                
                key_frame_rect.size = (Vector2){ 20 , 20 };
                key_frame_rect.position.y -= 1.1f * key_frame_rect.size.y * stack_index;
                
                bool draggable = true;
                
                if(dragging_key_frame) draggable = false;
                if(!check_collision_rect_mouse(key_frame_rect)) draggable = false;
                
                if (draggable)
                {
                    if (mouse_button_pressed(MOUSE_BUTTON_LEFT))
                    {
                        dragging_key_frame = true;
                        add_to_list_tail(key_frame_index , &clip->dragging_key_frame_list);
                    }
                    
                    draw_circle(key_frame_rect , Fade(ORANGE , 0.6));
                }
                else
                {
                    draw_circle(key_frame_rect , Fade(ORANGE , 0.4));
                }
                
            }
            
        }
        
	}
    
    
	local_persist int previous_frame_index = -1;
    
	if (mouse_button_pressed(MOUSE_BUTTON_LEFT))
	{
		previous_frame_index = closest_frame_index_to_mouse;
	}
    
    //DragKeyFrame
    
    if(editor->selected_clip_index != -1)
    {
        
        for(int stack_index = 0 ; stack_index < editor->selected_bone_count ; stack_index++)
        {
            BoneSelection * current_bone_selection = editor->selected_bone_stack + stack_index;
            
            Clip * clip = clip_array[current_bone_selection->clip_index];
            
            if (mouse_button_released(MOUSE_BUTTON_LEFT))
            {
                dragging_key_frame = false;
                clear_list(&clip->dragging_key_frame_list);
            }
            
            if(mouse_button_pressing(MOUSE_BUTTON_LEFT))
            {
                
                list_foreach(key_frame_being_drag_index , &clip->dragging_key_frame_list)
                {
                    
                    stop_mouse_input = true;
                    KeyFrame * key_frame_being_drag = all_key_frame + key_frame_being_drag_index;
                    
                    bool frame_existed = false;
                    hash_table_iterate(existed_key_frame_index , GetKeyFrameHash(closest_frame_index_to_mouse , key_frame_being_drag->bone_index) , &clip->key_frame_hash_table)
                    {
                        KeyFrame * key_frame = all_key_frame + existed_key_frame_index;
                        if(key_frame->bone_index != key_frame_being_drag->bone_index) continue;
                        
                        if(key_frame->frame_index == closest_frame_index_to_mouse)
                        {
                            frame_existed = true;
                            break;
                        }
                        
                    }
                    
                    if(!frame_existed)
                    {
                        
                        if(!delete_from_hash_table( GetKeyFrameHash(key_frame_being_drag->frame_index , key_frame_being_drag->bone_index ) , key_frame_being_drag - all_key_frame , &clip->key_frame_hash_table)) CATCH;
                        key_frame_being_drag->frame_index = closest_frame_index_to_mouse;
                        add_to_hash_table(GetKeyFrameHash(key_frame_being_drag->frame_index , key_frame_being_drag->bone_index ) , key_frame_being_drag_index , &clip->key_frame_hash_table);
                        //if(!delete_from_hash_table_by_slot_index( key_frame_being_drag->hash_table_slot_index ,GetKeyFrameHash(key_frame_being_drag->frame_index , key_frame_being_drag->bone_index ) , &clip->key_frame_hash_table )) CATCH;
                        
                        sort_bone_hash_table(key_frame_being_drag->bone_index , &clip->key_frame_hash_table_by_bone);
                        
#if 0
                        hash_table_iterate_ex(next_key_frame_index , slot_index , key_frame_being_drag->hash_table_by_bone_slot_index , key_frame_being_drag->bone_index , &clip->key_frame_hash_table_by_bone)
                        {
                            
                            KeyFrame * next_key_frame = all_key_frame + next_key_frame_index;
                            
                            if(next_key_frame->frame_index == key_frame_being_drag->frame_index) CATCH;
                            if(next_key_frame->frame_index > key_frame_being_drag->frame_index) break;
                            
                            HashTableSlot * next_key_frame_node = slot_array + next_key_frame->hash_table_by_bone_slot_index;
                            HashTableSlot * dragging_key_frame_node = slot_array + key_frame_being_drag->hash_table_by_bone_slot_index;
                            
                            int temp_data_index = next_key_frame_node->data_index;
                            next_key_frame_node->data_index = dragging_key_frame_node->data_index;
                            dragging_key_frame_node->data_index = temp_data_index;
                            
                            int temp_node_index = next_key_frame->hash_table_by_bone_slot_index;
                            next_key_frame->hash_table_by_bone_slot_index = key_frame_being_drag->hash_table_by_bone_slot_index;
                            key_frame_being_drag->hash_table_by_bone_slot_index = temp_node_index;
                            
                        }
                        
                        hash_table_iterate_reverse_ex(previous_key_frame_index , slot_index , key_frame_being_drag->hash_table_by_bone_slot_index , key_frame_being_drag->bone_index , &clip->key_frame_hash_table_by_bone)
                        {
                            KeyFrame * previous_key_frame = all_key_frame + previous_key_frame_index;
                            
                            if(previous_key_frame->frame_index == closest_frame_index_to_mouse) CATCH;
                            if(previous_key_frame->frame_index < closest_frame_index_to_mouse) break;
                            
                            HashTableSlot * previous_key_frame_node = slot_array + previous_key_frame->hash_table_by_bone_slot_index;
                            HashTableSlot * dragging_key_frame_node = slot_array + key_frame_being_drag->hash_table_by_bone_slot_index;
                            
                            int temp_data_index = previous_key_frame_node->data_index;
                            previous_key_frame_node->data_index = dragging_key_frame_node->data_index;
                            dragging_key_frame_node->data_index = temp_data_index;
                            
                            int temp_node_index = previous_key_frame->hash_table_by_bone_slot_index;
                            previous_key_frame->hash_table_by_bone_slot_index = key_frame_being_drag->hash_table_by_bone_slot_index;
                            key_frame_being_drag->hash_table_by_bone_slot_index = temp_node_index;
                            
                        }
                        
#endif
                    }
                }
            }
            
        }
    }
    
    
	Rect frame_pointer_rect = get_rect();
	frame_pointer_rect.position = pointer_position;
	frame_pointer_rect.size.y = timeline_rect.size.y * 1.2f;
	frame_pointer_rect.size.x = 10;
    
    bool hovering_pointer = false;
    
	if (check_collision_rect_mouse(timeline_rect))
	{
        hovering_pointer = true;
        
        if (mouse_button_pressed(MOUSE_BUTTON_LEFT))
		{
            stop_mouse_input = true;
            dragging_frame_pointer = true;
		}
	}
    
	if (dragging_frame_pointer)
	{
		draw_rect_D(frame_pointer_rect  , 0 , Fade(YELLOW,0.5));
	}
    else if(hovering_pointer)
    {
		draw_rect_D(frame_pointer_rect  , 0 , Fade(WHITE,0.5));
    }
	else
	{
		draw_rect_D(frame_pointer_rect  , 0 , Fade(WHITE ,0.2));
	}
    
	Vector3 add_new_frame_button_position = timeline_rect.position;
	add_new_frame_button_position.y += timeline_rect.size.y*0.5f;
	add_new_frame_button_position.x -= timeline_rect.size.x*0.5f;
    
	DrawingMenu timeline_operation_menu  = start_draw_menu((Vector2){add_new_frame_button_position.x , add_new_frame_button_position.y} , true ,GMT_to_the_right);
	timeline_operation_menu.current_button_position.y += 45;
	timeline_operation_menu.button_extra_offset = 5;
    
    wchar_t clip_entry_string[16] ={};
    _swprintf(clip_entry_string , L"動畫 ： %d" , editor->selected_clip_index);
    draw_menu_button_W(&timeline_operation_menu , clip_entry_string );
    
	bool play_start_or_stop = false;
    
	if (draw_menu_button_W(&timeline_operation_menu , editor->playing ? L"播放中" : L"播放" ))
	{
		play_start_or_stop = true;
	}
    
	draw_menu_int_input(&timeline_operation_menu , L"開始幀 : %d" , &editor->start_frame_index);
	draw_menu_int_input(&timeline_operation_menu , L"幀長度 : %d" , &editor->timeline_frame_length);
    
	local_persist float start_time = 0;
	local_persist float end_time = 0;
    
	start_time = ((float)editor->start_frame_index) / ((float)FRAME_PER_SECOND);
	end_time = ((float)editor->start_frame_index + editor->timeline_frame_length) / ((float)FRAME_PER_SECOND);
    
	draw_menu_float_input(&timeline_operation_menu , L"開始時間 : %f" , &start_time);
	draw_menu_float_input(&timeline_operation_menu , L"結束時間 : %f" , &end_time);
    
	editor->start_frame_index = start_time * FRAME_PER_SECOND;
	editor->timeline_frame_length = (end_time - start_time ) * FRAME_PER_SECOND;
    
	if (key_pressed(KEY_SPACE))
	{
        play_start_or_stop = true;
	}
    
	if (play_start_or_stop)
	{
		editor->playing = !editor->playing;
	}
    
	if (!editor->playing)
	{
		editor->play_timer = 0;
	}
	else
	{
		editor->play_timer += DeltaTime;
        
		if (editor->play_timer > FRAME_TIME)
		{
			editor->play_timer -= FRAME_TIME;
			editor->current_frame_at_timeline++;
            
			if (editor->current_frame_at_timeline > editor->start_frame_index + editor->timeline_frame_length - 1)
			{
				editor->current_frame_at_timeline = editor->start_frame_index;
			}
		}
	}
    
    bool add_new_key_frame = false;
    bool remove_key_frame = false;
    
    if(editor->selected_bone_count >0)
    {
        add_new_key_frame = draw_menu_button_W(&timeline_operation_menu , L"添加幀");
        remove_key_frame = draw_menu_button_W(&timeline_operation_menu , L"刪除幀");
    }
    
    for(int stack_index = 0 ; stack_index < editor->selected_bone_count ; stack_index++)
    {
        BoneSelection * current_bone_selection = editor->selected_bone_stack + stack_index;
        int selected_bone_index = current_bone_selection->bone_index;
        
        //_Bone * selected_bone = BoneArray + Selectedbone_index;
        
        Clip * clip = clip_array[current_bone_selection->clip_index];
        
        if (add_new_key_frame)
        {
            bool frame_existed = false;
            
            hash_table_iterate(key_frame_index, GetKeyFrameHash(editor->current_frame_at_timeline , selected_bone_index), &clip->key_frame_hash_table)
            {
                
                KeyFrame * key_frame = all_key_frame + key_frame_index;
                
                if(key_frame->bone_index != selected_bone_index) continue;
                
                if (key_frame->frame_index == editor->current_frame_at_timeline)
                {
                    frame_existed = true;
                    break;
                }
                
            }
            
            if (!frame_existed)
            {
                REALLOCATE_BUFFER_IF_TOO_SMALL(KeyFrame , all_key_frame , all_key_frame_capacity , all_key_frame_count , allocate_temp_);
                int new_key_frame_index = all_key_frame_count++;
                KeyFrame * new_key_frame = all_key_frame + new_key_frame_index;
                add_to_list_tail( new_key_frame_index , &clip->key_frame_active_list);
                
                (*new_key_frame) = (KeyFrame){};
                
                new_key_frame->bone_index = selected_bone_index;
                new_key_frame->frame_index = editor->current_frame_at_timeline;
                new_key_frame->bone_state.local_rotation = QuaternionIdentity();
                add_to_hash_table( GetKeyFrameHash(editor->current_frame_at_timeline , selected_bone_index) , new_key_frame_index , &clip->key_frame_hash_table);
                add_to_hash_table( selected_bone_index , new_key_frame_index , &clip->key_frame_hash_table_by_bone);
                
                sort_bone_hash_table(selected_bone_index , &clip->key_frame_hash_table_by_bone);
                
            }
        }
        
        if (remove_key_frame)
        {
            int frame_index = 0;
            KeyFrame * key_frame_array = all_key_frame;
            int key_frame_hash = GetKeyFrameHash(editor->current_frame_at_timeline , selected_bone_index);
            hash_table_iterate_ex(Keyframe_index , SlotIndex , -1 , key_frame_hash , &clip->key_frame_hash_table)
            {
                KeyFrame * key_frame = key_frame_array + Keyframe_index;
                
                if(key_frame->bone_index != selected_bone_index) continue;
                
                if (key_frame->frame_index == editor->current_frame_at_timeline)
                {
                    delete_from_hash_table_by_slot_index(SlotIndex , key_frame_hash , &clip->key_frame_hash_table);
                    if(!delete_from_list(Keyframe_index , &clip->key_frame_active_list)) CATCH;
                    //if(!delete_from_hash_table_by_slot_index( key_frame->hash_table_by_bone_slot_index ,selected_bone_index, )) CATCH;
                    if(!delete_from_hash_table(selected_bone_index , Keyframe_index , &clip->key_frame_hash_table_by_bone)) CATCH;
                    
                    break;
                }
            }
            
        }
        
    }
    
#if 1
    
    if(editor->selected_clip_index != -1)
    {
        
        if(editor->selected_bone_count == 1 )
        {
            
            BoneSelection * bone_selection = editor->selected_bone_stack;
            int selected_bone_index = bone_selection->bone_index;
            
            Clip * clip = clip_array[bone_selection->clip_index];
            
            KeyFrame * key_frame_array = all_key_frame;
            
            hash_table_iterate(key_frame_index , selected_bone_index , &clip->key_frame_hash_table_by_bone)
            {
                KeyFrame * key_frame = key_frame_array + key_frame_index;
                
                if(key_frame->bone_index != bone_selection->bone_index) continue;
                
                char key_frame_name_buffer[256] = {};
                
                sprintf(key_frame_name_buffer , "%d" , key_frame->frame_index);
                
                if (draw_menu_button(&timeline_operation_menu , key_frame_name_buffer))
                {
                    
                }
            }
        }
    }
#endif
}

internal void editor_GUI()
{
    
    glBindFramebuffer(GL_FRAMEBUFFER, render_state.screen_frame_buffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, render_state.interface_texture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    if (!rlFramebufferComplete(render_state.screen_frame_buffer)) CATCH;
    rlEnableFramebuffer(render_state.screen_frame_buffer);
    
    glClearDepth(0);
    glClearColor(0,0,0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
    
	if (mouse_button_pressed(MOUSE_BUTTON_RIGHT))
	{
        editor->operate_menu_position = app_data->mouse_position;
    }
    
    if(editor->selected_bone_count == 1)
    {
        BoneSelection * bone_selection = editor->selected_bone_stack;
        bool clip_not_found = true;
        
        ClipBone * clip_bone = clip_bone_stack + bone_selection->clip_bone_stack_index;
        Bone * editing_bone = clip_bone->final_bone_pose + bone_selection->bone_index;
        
        if(editor_type == edit_base_pose)
        {
            editing_bone = selected_model->all_bones + bone_selection->bone_index;
        }
        
        bone_mouse_menu( editing_bone , clip_array[clip_bone->clip_index] , editor->current_frame_at_timeline);
    }
    
    if(editor_type == edit_animation)
    {
        animation_timeline_GUI();
    }
    
    DrawingMenu bone_menu = start_draw_menu((Vector2){app_data->window_size.x - (500) ,app_data->window_size.y - 20} , true , GMT_to_the_right);
    bone_menu.button_extra_offset = 5;
    bone_menu.menu_type = GMT_descend;
    
    if(editor->selected_bone_count == 1)
    {
        
        Bone * selected_bone = selected_model->all_bones + editor->selected_bone_stack->bone_index;
        
        local_persist bool inputing_text = false;
        local_persist int input_cursor = 0;
        local_persist wchar_t all_character[64]={};
        local_persist wchar_t * bone_string = 0;
        
        if(!inputing_text)
        {
            
            if(draw_menu_button_W( &bone_menu , selected_bone->bone_name.string))
            {
                if(!selected_bone->from_blend_file)
                {
                    
                    inputing_text = true;
                    input_cursor = 0;
                    bone_string = selected_bone->bone_name.string;
                    
                }
            }
            
            wchar_t * parent_string = L"none";
            int parent_bone_index = selected_bone->parent_bone_index;
            
            if(parent_bone_index != -1)
            {
                parent_string = combine_string_W( L"parent bone : " , selected_model->all_bones[parent_bone_index].bone_name.string);
            }
            
            if(editor->assigning_parent_bone)
            {
                
                BoneSelectionResult result = bone_selection( (Vector2){40 , 40} , Fade(GREEN, 0.2) , Fade(GREEN , 0.8));
                
                for(int data_index = 0 ; data_index < result.data_count ; data_index++)
                {
                    //TODO: put model in clip bone
                    Bone * final_bone = clip_bone_stack[0].final_bone_pose;
                    Bone * selected_bone_final = final_bone + selected_bone->bone_index;
                    Bone * new_parent_bone = final_bone + result.data[data_index].bone_index;
                    
                    if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
                    {
                        if(new_parent_bone->bone_index != selected_bone->bone_index)
                        {
                            
                            int previous_parent_index = selected_bone->parent_bone_index;
                            selected_bone->parent_bone_index = new_parent_bone->bone_index;
                            
                            list_foreach_EX(root_index , N_node_head , node_index , &selected_model->root_bone_list)
                            {
                                
                                if(root_index == selected_bone->bone_index)
                                {
                                    if(!delete_from_list(node_index , &selected_model->root_bone_list)) CATCH;
                                    break;
                                }
                                
                            };
                            
                            if(previous_parent_index != -1)
                            {
                                if(!delete_from_hash_table(previous_parent_index , selected_bone->bone_index , &selected_model->bone_children_hash_table))
                                {
                                    CATCH;
                                }
                            }
                            
                            selected_bone->parent_bone_index = new_parent_bone->bone_index;
                            add_to_hash_table( new_parent_bone->bone_index , selected_bone->bone_index , &selected_model->bone_children_hash_table);
                            editor->assigning_parent_bone = false;
                            
                            selected_bone->state.local_rotation = QuaternionMultiply(  QuaternionInvert(new_parent_bone->rotation) , selected_bone_final->rotation);
                            
                            selected_bone->state.local_position = Vector3Subtract(selected_bone_final->position , new_parent_bone->position);
                            selected_bone->state.local_position = Vector3RotateByQuaternion(selected_bone->state.local_position , QuaternionInvert(new_parent_bone->rotation));
                            
                            break;
                        }
                        
                    }
                    
                }
                
            }
            
            if(editor->assigning_IK_target_bone || editor->assigning_IK_pole_bone)
            {
                BoneSelectionResult result = bone_selection( (Vector2){40 , 40} , Fade(GREEN, 0.2) , Fade(GREEN , 0.8));
                
                if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
                {
                    for(int data_index = 0 ; data_index < result.data_count ; data_index++)
                    {
                        int target_bone_index = result.data[data_index].bone_index;
                        if(target_bone_index != selected_bone->bone_index)
                        {
                            
                            if(editor->assigning_IK_target_bone)
                            {
                                selected_bone->IK_target_bone_index = target_bone_index;
                            }
                            else if(editor->assigning_IK_pole_bone)
                            {
                                selected_bone->IK_pole_bone_index = target_bone_index;
                            }
                            
                            editor->assigning_IK_target_bone = false;
                            editor->assigning_IK_pole_bone = false;
                            
                            break;
                        }
                        
                    }
                }
                
            }
            
            if(mouse_button_pressed(MOUSE_BUTTON_LEFT))
            {
                editor->assigning_parent_bone = false;
                editor->assigning_IK_target_bone = false;
                editor->assigning_IK_pole_bone = false;
            }
            
            if(draw_menu_button_W(&bone_menu , parent_string))
            {
                editor->assigning_parent_bone = true;
            }
            
            if(draw_menu_button_W(&bone_menu , selected_bone->IK_enable ? L"IK enable" : L"IK disable"))
            {
                selected_bone->IK_enable = !selected_bone->IK_enable;
            }
            
            if(selected_bone->IK_enable)
            {
                draw_menu_int_input(&bone_menu , L"chain length : %d" , &selected_bone->IK_chain_length);
                
                wchar_t * target_bone_string = L"none";
                wchar_t target_string[64] = {};
                if(selected_bone->IK_target_bone_index != -1) target_bone_string = selected_model->all_bones[selected_bone->IK_target_bone_index].bone_name.string;
                _swprintf(target_string , L"IK target : %s" , target_bone_string );
                
                wchar_t * pole_bone_string = L"none";
                wchar_t pole_string[64] = {};
                if(selected_bone->IK_pole_bone_index != -1) pole_bone_string = selected_model->all_bones[selected_bone->IK_pole_bone_index].bone_name.string;
                _swprintf(pole_string , L"IK pole : %s" , pole_bone_string);
                
                if(draw_menu_button_W(&bone_menu , target_string))
                {
                    editor->assigning_IK_target_bone = !editor->assigning_IK_target_bone;
                }
                
                if(draw_menu_button_W(&bone_menu , pole_string))
                {
                    editor->assigning_IK_pole_bone = !editor->assigning_IK_pole_bone;
                }
                
            }
            
        }
        else
        {
            
            int all_character_count = 0;
            
            draw_menu_button_W_EX( &bone_menu , selected_bone->bone_name.string , YELLOW , true);
            
            bool exit_input = false;
            
            if(key_pressed(KEY_ENTER)) exit_input = true;
            if(key_pressed(KEY_ESCAPE)) exit_input = true;
            if(mouse_button_pressed(MOUSE_BUTTON_LEFT)) exit_input = true;
            if(mouse_button_pressed(MOUSE_BUTTON_RIGHT)) exit_input = true;
            
            if(exit_input)
            {
                inputing_text = false;
            }
            
            int start_index = 0;
            for( ; bone_string[start_index]&&(start_index < FIXED_STRING_SIZE)  ; start_index++);
            
            if(key_pressed(KEY_BACKSPACE))
            {
                if(start_index>= 0)
                {
                    bone_string[start_index-1] = L'\0';
                }
            }
            
            for(;;)
            {
                unsigned int character = char_pressed();
                if(!character) break;
                all_character[all_character_count++] = character;
            }
            
            if(all_character_count > 0)
            {
                for(int buffer_index = 0; buffer_index < all_character_count ; buffer_index++)
                {
                    int string_index = buffer_index + start_index;
                    if(string_index >= FIXED_STRING_SIZE) break;
                    bone_string[string_index] = all_character[buffer_index];
                }
            }
            
        }
        
    }
    
    Vector2 top_right_bar_position = {20,app_data->window_size.y - 20};
    DrawingMenu top_right_bar = start_draw_menu(top_right_bar_position , true , GMT_to_the_right);
    top_right_bar.button_extra_offset = 8;
    
    DrawingMenu side_list_menu = top_right_bar;
    side_list_menu.menu_type = GMT_descend;
    side_list_menu.button_extra_offset = 0;
    draw_menu_button_W(&side_list_menu , L"" );
    
    for(int type_index = 0 ; type_index < edit_type_count ; type_index++)
    {
        char * edit_type_name = enum_to_string(EditorType)[type_index];
        
        if(draw_menu_button_EX(&top_right_bar , edit_type_name , YELLOW , editor_type == type_index))
        {
            editor_type = type_index;
        }
    }
    
    if(editor_type == edit_animation)
    {
        
        if(draw_menu_button_W(&top_right_bar , L"添加動畫"))
        {
            int clip_index = 0;
            bool emty_clip = false;
            
            for(; clip_index < MAX_CLIP ; clip_index++ )
            {
                if(!clip_array[clip_index])
                {
                    emty_clip = true;
                    break;
                }
            }
            
            if(!emty_clip) CATCH;
            
            Clip * new_clip = allocate_temp( Clip , 1);
            
            clip_array[clip_index] =new_clip;
            new_clip->clip_index = clip_index;
            new_clip->key_frame_active_list = allocate_list(CLIP_START_CAPACITY);
            new_clip->dragging_key_frame_list = allocate_list(CLIP_START_CAPACITY);
            new_clip->key_frame_hash_table = allocate_hash_table( CLIP_START_CAPACITY );
            new_clip->key_frame_hash_table_by_bone = allocate_hash_table( CLIP_START_CAPACITY );
        }
        
        if(editor->selected_clip_index != -1)
        {
            Clip * clip = clip_array[editor->selected_clip_index];
            
            if(draw_menu_button_W(&top_right_bar , L"刪除動畫"))
            {
                clip_array[editor->selected_clip_index] = 0;
                editor->selected_clip_index = -1;
                
                clip_bone_stack_count = 0;
                
                editor->selected_bone_count = 0;
            }
            
            if(draw_menu_button_W(&top_right_bar , clip->not_loop ? L"非循環" : L"循環")) clip->not_loop = !clip->not_loop;
        }
        
        
        draw_menu_int_input(&top_right_bar , L"IK迭代次數：%d"  , &editor->IK_iteration_count);
        
        if(draw_menu_button_W(&top_right_bar , editor->turn_off_bezier_curve ? L"開啓貝塞爾曲綫" : L"關閉貝塞爾曲綫")) editor->turn_off_bezier_curve = !editor->turn_off_bezier_curve;
        
        if(editor->selected_bone_count == 1)
        {
            BoneSelection * current_bone_selection = editor->selected_bone_stack;
            //draw_menu_button( &top_right_bar , enum_to_string(BoneTag)[current_bone_selection->bone_index]);
        }
        
        for(int clip_index = 0 ; clip_index < MAX_CLIP ; clip_index++)
        {
            Clip * clip = clip_array[clip_index];
            
            if(!clip) continue;
            
            const char * aniamtion_name = enum_to_string(AnimationTag)[clip_index];
            if(clip_index >= animation_tag_count) aniamtion_name = " None ";
            
            wchar_t clip_name[256] = {};
            _swprintf(clip_name , L"%d 動畫 : %S" , clip->clip_index , aniamtion_name );
            
            if(draw_menu_button_W_EX(&side_list_menu , clip_name , YELLOW , editor->selected_clip_index == clip_index ))
            {
                editor->selected_clip_index = clip_index;
                
                clip_bone_stack_count = 0;
                add_clip_bone_at_next_frame = true;
                ClipBone * new_clip_bone = clip_bone_stack + clip_bone_stack_count;
                
                (*new_clip_bone) = (ClipBone){};
                new_clip_bone->clip_index = clip_index;
                
                editor->selected_bone_count = 0;
            }
            
        }
        
    }
    
    if(editor_type == edit_base_pose)
    {
        
        if(draw_menu_button_W(&top_right_bar , L"添加骨頭"))
        {
            
            REALLOCATE_BUFFER_IF_TOO_SMALL(Bone , selected_model->all_bones , selected_model->bone_capacity , selected_model->bone_count , allocate_temp_);
            REALLOCATE_BUFFER_IF_TOO_SMALL(Bone , selected_model->all_initial_bone , selected_model->initial_bone_capacity , selected_model->initial_bone_count , allocate_temp_);
            
            int new_bone_index = selected_model->bone_count++;
            Bone * new_bone = selected_model->all_bones + new_bone_index;
            
            new_bone->bone_index = new_bone_index;
            new_bone->parent_bone_index = -1;
            new_bone->state.local_position = (Vector3){0,0,0};
            new_bone->state.local_rotation = QuaternionIdentity();
            new_bone->state.end_point_offset = (Vector3){0,1,0};
            new_bone->free_position = true;
            new_bone->from_blend_file = false;
            
            new_bone->IK_enable = false;
            new_bone->IK_chain_length = 0;
            new_bone->IK_target_bone_index = -1;
            new_bone->IK_pole_bone_index = -1;
            
            add_to_list_tail(new_bone_index , &selected_model->root_bone_list);
            
            int new_initial_bone_index = selected_model->initial_bone_count++;
            Bone * new_initial_bone = selected_model->all_initial_bone + new_initial_bone_index;
            (*new_initial_bone) = (*new_bone);
        }
    }
    
    if(editor_type == edit_map)
    {
        
        if(draw_menu_button_W(&top_right_bar , L"添加參考坐標"))
        {
            REALLOCATE_LIST_IF_TOO_SMALL(Vector3 , all_reference_frame , &reference_frame_list);
            selected_reference_frame_index = add_to_list_tail_B(&reference_frame_list);
            all_reference_frame[selected_reference_frame_index] = (Vector3){};
        }
        
        if(selected_reference_frame_index != -1)
        {
            if(draw_menu_button_W(&top_right_bar , L"刪除參考坐標"))
            {
                if(!delete_from_list(selected_reference_frame_index , &reference_frame_list)) CATCH;
                selected_reference_frame_index = -1;
            }
        }
        
        if(draw_menu_button_W_EX(&side_list_menu , L"原點" , YELLOW , selected_reference_frame_index == -1))
        {
            selected_reference_frame_index = -1;
        }
        
        list_foreach(reference_frame_index , &reference_frame_list)
        {
            Vector3 reference_frame = all_reference_frame[reference_frame_index];
            wchar_t temp[256] = {};
            _swprintf(temp , L"%f %f %f" , reference_frame.x ,reference_frame.y , reference_frame.z);
            if(draw_menu_button_W_EX(&side_list_menu , temp , YELLOW , reference_frame_index == selected_reference_frame_index))
            {
                selected_reference_frame_index = reference_frame_index;
            }
        }
    }
    
    D_game_draw();
    rlDisableFramebuffer();
}