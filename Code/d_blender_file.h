
typedef struct BlenderFileBlockHeader BlenderFileBlockHeader;
struct BlenderFileBlockHeader
{
    
    char block_name[4];
    int block_size;
    int SDNA_index;
    int SDNA_count;//multiple? why
    unsigned long long old_memory_address;
    
    unsigned char * block_memory;
    char ** all_fields;
    
    int data_block_count;
    
};

typedef struct BlenderFileBlockHeaderNode BlenderFileBlockHeaderNode;
struct BlenderFileBlockHeaderNode
{
    BlenderFileBlockHeader header;
    
    BlenderFileBlockHeaderNode * next;
};

typedef struct DNA_Structure DNA_Structure;
struct DNA_Structure
{
    short type;
    short field_count;
    short * all_fields;
    short * all_field_name_index;
};

typedef struct FlattenFieldInfo FlattenFieldInfo;
struct FlattenFieldInfo
{
    char * name;
    char * type_name;
    int type;
    int field_size;
    int field_data_count;
    bool is_pointer;
};

typedef struct FieldIterator FieldIterator;
struct FieldIterator
{
    
    bool initialized;
    int field_index;
    int field_count;
    FlattenFieldInfo * all_info;
    int data_offset;
    
    FlattenFieldInfo * field;
    unsigned char * data;
    int data_size;
    unsigned long long pointer_address;
    
};

typedef struct FieldResult FieldResult;
struct FieldResult
{
    int type;
    unsigned char * data;
    unsigned long long address;
    
    FlattenFieldInfo * field;
};

typedef struct BlenderBone BlenderBone;
struct BlenderBone
{
    
    int parent_index;
    
    FixedStringW name;
    Vector3 head;
    Vector3 tail;
    float roll;
    Quaternion rotation;
    
    Matrix rotation_matrix;//i guess it just rotation
    
};

typedef struct BlenderVertex BlenderVertex;
struct BlenderVertex
{
    Vector3 position;//it called "co", i guess it mean coordinate?
    char flag;
    char weight;
    char padding[2];
};

typedef struct BlenderEdge BlenderEdge;
struct BlenderEdge
{
    int vertex_a;
    int vertex_b;
    char crease;
    char weight;
    short flag;
};

typedef struct BlenderLoop BlenderLoop;
struct BlenderLoop
{
    int vertex_index;
    int edge_index;
};

typedef struct BlenderPolygon BlenderPolygon;
struct BlenderPolygon
{
    
    int loop_start;
    int loop_count;
    
    short material_index;
    char flag;
    char padding;
    
};

typedef struct BlenderDeformVertex BlenderDeformVertex;
struct BlenderDeformVertex
{
    unsigned long long weight_address;
    int weight_count;
    int flag;
};

typedef struct BlenderWeight BlenderWeight;
struct BlenderWeight
{
    int group_index;
    float weight;
};

global float model_scale = 1.0;

global BlenderBone * bone_buffer = 0;
global int bone_buffer_count = 0;
global int bone_buffer_capacity = 0;

global unsigned short * mesh_indices = 0;
global int mesh_indices_count = 0;
global int mesh_indices_capacity = 0;

global Vector3 * mesh_vertices = 0;
global int mesh_vertices_count = 0;
global int mesh_vertices_capacity = 0;

global HashTable block_address_hash_table = {};

global short * all_types_byte_length = 0;
global DNA_Structure * all_structures = 0;
global FlattenFieldInfo ** all_structure_field = 0;
global int * all_structure_field_count = 0;

global char ** all_types = 0;
global int * type_to_structure = 0;
global char ** all_field_name = 0;
global int blend_file_pointer_size = 8;//no way it gonna be 32 bit
global int void_type_index = -1;

global BlenderFileBlockHeader * all_block_header = 0;
global int all_block_header_count = 0;


internal char * get_type_name_by_structure_index(int structure_index)
{
    return all_types[ all_structures[structure_index].type ];
}

internal int address_hash(unsigned long long address)
{
    
    address = address % 4294967295;
    
    long singed_address = address;
    singed_address -= (4294967295 / 2);
    
    return hash_int((int)singed_address);
    
}

internal BlenderFileBlockHeader * get_block_by_address(unsigned long long address)
{
    
    hash_table_iterate(block_index , address_hash(address) , &block_address_hash_table)
    {
        
        BlenderFileBlockHeader * block = all_block_header + block_index;
        
        if(block->old_memory_address == address)
        {
            return block;
        }
        
    }
    
    CATCH;
    return 0;
}

internal char** decompose_long_string(unsigned char ** memory_pointer , int count)
{
    char ** all_strings = allocate_frame(char *, count);
    
    unsigned char * memory = (*memory_pointer);
    
    for(int name_index = 0 ; name_index < count ; memory++)
    {
        
        if(!all_strings[name_index])
        {
            all_strings[name_index] = memory;
        }
        
        if( (*memory) == '\0')
        {
            name_index++;
        }
        
    }
    
    for(; !(*memory) ;memory++); 
    (*memory_pointer) = memory;
    
    return all_strings;
}

internal int get_DNA_structure_flatten_field_count( DNA_Structure structure)
{
    int field_count = 0;
    
    for(int field_index = 0 ; field_index < structure.field_count ; field_index++)
    {
        
        short field_type = structure.all_fields[field_index];
        short field_name_index = structure.all_field_name_index[field_index];
        
        char * field_name = all_field_name[field_name_index];
        
        bool is_pointer =false;
        int data_count = 1;
        
        for(int char_index = 0 ; field_name[char_index] != '\0'; char_index++)
        {
            
            if(field_name[char_index] == '*' )
            {
                is_pointer = true;
            }
            else if(field_name[char_index] == '[')
            {
                
                int array_count = 0;
                sscanf(field_name + char_index , "[%d]" , &array_count);
                data_count *= array_count;
                
            }
            
        }
        
        int structure_index = type_to_structure[field_type];
        
        bool parse_field = false;
        
        if(structure_index != -1)
        {
            if(!is_pointer)
            {
                parse_field = true;
            }
        }
        
        if(parse_field)
        {
            
            DNA_Structure field_structure = all_structures[structure_index];
            field_count += get_DNA_structure_flatten_field_count(field_structure) * data_count;
            
        }
        else
        {
            
            field_count++;
            
        }
        
    }
    
    return field_count;
}

internal void parse_DNA_structure_flatten( char * previous_field_name , DNA_Structure structure , FlattenFieldInfo * all_flatten_field , int * all_flatten_field_count)
{
    
    for(int field_index = 0 ; field_index < structure.field_count ; field_index++)
    {
        
        short field_type = structure.all_fields[field_index];
        short field_name_index = structure.all_field_name_index[field_index];
        
        char * field_name = all_field_name[field_name_index];
        
        bool is_pointer =false;
        int data_count = 1;
        
        for(int char_index = 0 ; field_name[char_index] != '\0'; char_index++)
        {
            
            if(field_name[char_index] == '*' )
            {
                is_pointer = true;
            }
            else if(field_name[char_index] == '[')
            {
                int array_count = 0;
                sscanf(field_name + char_index , "[%d]" , &array_count);
                data_count *= array_count;
            }
            
        }
        
        int structure_index = type_to_structure[field_type];
        
        bool parse_field = false;
        
        if(structure_index != -1)
        {
            if(!is_pointer)
            {
                parse_field = true;
            }
        }
        
        if(previous_field_name)
        {
            char * point_to = combine_string( previous_field_name , "->");
            field_name = combine_string( point_to , field_name);
        }
        char * name = field_name;
        
        if(parse_field)
        {
            
            for(int data_index = 0 ; data_index < data_count ; data_index++)
            {
                
                if(data_count > 1)
                {
                    char index_string[16] = {};
                    sprintf(index_string , "_%d" , data_index);
                    name = combine_string(name , index_string);
                }
                
                DNA_Structure field_structure = all_structures[structure_index];
                parse_DNA_structure_flatten(field_name , field_structure , all_flatten_field , all_flatten_field_count);
                
            }
            
        }
        else
        {
            
            int field_size = all_types_byte_length[field_type] * data_count;
            if(is_pointer ) field_size = blend_file_pointer_size;
            
            FlattenFieldInfo flatten_field = {};
            flatten_field.name = field_name;
            flatten_field.type = field_type;
            flatten_field.type_name = all_types[field_type];
            flatten_field.field_size = field_size;
            flatten_field.field_data_count = data_count;
            flatten_field.is_pointer = is_pointer;
            
            all_flatten_field[(*all_flatten_field_count)++] = flatten_field;
            
        }
        
    }
    
}

internal FlattenFieldInfo * get_flatten_fields(int DNA_structure_index , int * count)
{
    
    int allocate_size = get_DNA_structure_flatten_field_count( all_structures[DNA_structure_index]);
    
    FlattenFieldInfo * all_flatten_field = allocate_frame(FlattenFieldInfo , allocate_size);
    int all_flatten_field_count = 0;
    
    parse_DNA_structure_flatten( 0, all_structures[DNA_structure_index] , all_flatten_field , &all_flatten_field_count);
    
    (*count) = all_flatten_field_count;
    return all_flatten_field;
    
}

internal void print_depth(int depth)
{
    for(int depth_index = 0 ; depth_index < depth ; depth_index++) printf("\t");
}

internal bool iterate_fields(FieldIterator * iterator , unsigned char * block_memory , int structure_index)
{
    if(!structure_index) CATCH;
    
    if(!iterator->initialized)
    {
        if(!all_structure_field[structure_index])
        {
            
            all_structure_field[structure_index] = get_flatten_fields(structure_index , all_structure_field_count + structure_index);
        }
        
        iterator->data_offset = 0;
        iterator->all_info = all_structure_field[structure_index];
        iterator->field_count = all_structure_field_count[structure_index];
        iterator->initialized = true;
    }
    else
    {
        iterator->data_offset += iterator->field->field_size;
    }
    
    if(iterator->field_index >= iterator->field_count) return false;
    
    iterator->data = block_memory + iterator->data_offset;
    iterator->field = iterator->all_info + iterator->field_index;
    iterator->data_size = iterator->field->field_size;
    
    if(iterator->field->is_pointer)
    {
        iterator->pointer_address = (unsigned long long)( *(void**)iterator->data );
    }
    
    iterator->field_index++;
    
    return true;
}

internal FieldResult get_field( char * name , unsigned char * block_memory , int structure_index)
{
    
    FieldResult result = {};
    
    for(FieldIterator iterator = {} ; iterate_fields(&iterator , block_memory , structure_index) ;)
    {
        
        if(compare_string( iterator.field->name , name))
        {
            
            result.data = iterator.data;
            result.address = iterator.pointer_address;
            result.type = iterator.field->type;
            result.field = iterator.field;
            
            return result;
            
        }
        
    }
    
    CATCH;
    return (FieldResult){};
}

internal void print_position(unsigned char * data)
{
    Vector3 position = (*((Vector3*)data));
    printf( " (%f,%f,%f)" , position.x, position.y , position.z);
}

internal void print_all_fields(unsigned char * block_memory , int structure_index)
{
    
    for(FieldIterator iterator = {} ; iterate_fields(&iterator , block_memory , structure_index) ;)
    {
        
        printf("field: %s %s    size(%d) offset(%d)" , 
               iterator.field->name , 
               iterator.field->type_name , 
               iterator.field->field_size , 
               iterator.data_offset);
        
        char * field_name = iterator.field->name;
        unsigned char * data = iterator.data;
        
        if(iterator.field->is_pointer)
        {
            printf("    pointer_address(%llu)" , iterator.pointer_address);
        }
        else if(compare_string(field_name , "roll"))
        {
            printf( "  (%f)" , *((float*)data));
        }
        else if(compare_string(field_name , "head[3]"))
        {
            print_position(data);
        }
        else if(compare_string(field_name , "tail[3]"))
        {
            print_position(data);
        }
        else if(compare_string(field_name , "bone_mat[3][3]"))
        {
            //i'm guess this is rotation matrix?
            //there is no scale
            print_position(data);
            print_position(data + 12);
            print_position(data + 24);
            
            float * value = (float *)data;
            
            Matrix rotation_matrix = MatrixIdentity();
            rotation_matrix.m0 = value[0];
            rotation_matrix.m4 = value[1];
            rotation_matrix.m8 = value[2];
            rotation_matrix.m1 = value[3];
            rotation_matrix.m5 = value[4];
            rotation_matrix.m9 = value[5];
            rotation_matrix.m2 = value[6];
            rotation_matrix.m6 = value[7];
            rotation_matrix.m10 = value[8];
            
            Quaternion rotation = QuaternionFromMatrix(rotation_matrix);
            
            printf( "->(%f,%f,%f %f)" , rotation.x, rotation.y , rotation.z , rotation.w);
        }
        else if(compare_string(field_name , "totweight"))
        {
            printf("  %d" , *((int*)data));
        }
        else if(compare_string(field_name , "id->name[66]"))
        {
            printf( "  %.*s" , 66 , (char*)data);
        }
        else if(compare_string(field_name , "name[64]"))
        {
            printf( "  %.*s" , 64 , (char*)data);
        }
        
        printf("\n");
        
    }
    
    printf("\n");
}

internal void blender_iterate_bone(unsigned long long address , int type , int stack_count , int parent_index)
{
    
    for(;;)
    {
        
        if(!address) return;
        BlenderFileBlockHeader * data_block = get_block_by_address(address);
        
        //print_all_fields(data_block->block_memory , data_block->SDNA_index);
        
        //if there is no type from the field does that mean i need to use SDNA index the data block?
        int structure_index = type_to_structure[type];
        if(type == void_type_index) structure_index = data_block->SDNA_index;
        
        REALLOCATE_BUFFER_IF_TOO_SMALL(BlenderBone , bone_buffer , bone_buffer_capacity , bone_buffer_count , allocate_frame_);
        int bone_index = bone_buffer_count;
        BlenderBone * new_bone = bone_buffer + bone_buffer_count++;
        
        new_bone->parent_index = parent_index;
        
#define get_field_raw(name) get_field(name , data_block->block_memory , structure_index).data
#define get_field_data(name , type) (*((type *)get_field(name , data_block->block_memory , structure_index).data))
        
        float * value = (float *)get_field_raw("arm_mat[4][4]");
        
        Matrix rotation_matrix = MatrixIdentity();
        rotation_matrix.m0 = value[0];
        rotation_matrix.m4 = value[1];
        rotation_matrix.m8 = value[2];
        rotation_matrix.m12 = value[3];
        rotation_matrix.m1 = value[4];
        rotation_matrix.m5 = value[5];
        rotation_matrix.m9 = value[6];
        rotation_matrix.m13 = value[7];
        rotation_matrix.m2 = value[8];
        rotation_matrix.m6 = value[9];
        rotation_matrix.m10 = value[10];
        rotation_matrix.m14 = value[11];
        rotation_matrix.m3 = value[12];
        rotation_matrix.m7 = value[13];
        rotation_matrix.m11 = value[14];
        rotation_matrix.m15 = value[15];
        
        Quaternion rotation = QuaternionFromMatrix(rotation_matrix);
        
        new_bone->rotation = rotation;
        new_bone->rotation_matrix = rotation_matrix;
        new_bone->head = get_field_data( "arm_head[3]" , Vector3);
        new_bone->tail = get_field_data( "arm_tail[3]" , Vector3);
        new_bone->roll = get_field_data( "arm_roll" , float);
        
        FixedString bone_name_in_char = get_field_data("name[64]" , FixedString);
        
        char_to_wide_char(new_bone->name.string , bone_name_in_char.string , FIXED_STRING_SIZE);
        
        FieldResult child_bone_field = get_field("childbase->*first" , data_block->block_memory , structure_index);
        
        blender_iterate_bone(child_bone_field.address , child_bone_field.type , stack_count + 1 , bone_index);
        
        FieldResult next_bone_result = get_field("*next" , data_block->block_memory , structure_index);
        
        address = next_bone_result.address;
        type = next_bone_result.type;
        
    }
    
}

internal void load_header_and_DNA(FILE * blend_file)
{
    
    BlenderFileBlockHeaderNode * block_list_head = allocate_temp(BlenderFileBlockHeaderNode , 1);
    BlenderFileBlockHeaderNode * block_list_current = block_list_head;
    
    int * main_block_data_block_count = 0;
    int data_block_count = 0;
    
    for(;;)
    {
        
        char block_name[4] = {};
        int block_size = 0;
        int SDNA_index = 0;
        int SDNA_count = 0;
        unsigned long long old_memory_address = 0;
        
        fread(block_name , 4 , 1 , blend_file);
        fread(&block_size , 4 , 1 , blend_file);
        fread(&old_memory_address , 8 , 1 , blend_file);
        fread(&SDNA_index , 4 , 1 , blend_file);
        fread(&SDNA_count , 4 , 1 , blend_file);
        
        unsigned char * block_memory = allocate_frame_(block_size);
        fread(block_memory , block_size , 1 , blend_file);
        
        //printf("%.*s\n" , 4,block_name);
        
        BlenderFileBlockHeader new_block = {};
        
        new_block.block_name[0] = block_name[0];
        new_block.block_name[1] = block_name[1];
        new_block.block_name[2] = block_name[2];
        new_block.block_name[3] = block_name[3];
        new_block.block_size = block_size;
        new_block.SDNA_index = SDNA_index;
        new_block.SDNA_count = SDNA_count;
        new_block.block_memory = block_memory;
        new_block.old_memory_address = old_memory_address;
        
        block_list_current->header = new_block;
        
        if(!compare_string_C(block_name , "DATA" , 4))
        {
            if(main_block_data_block_count) (*main_block_data_block_count) = data_block_count;
            main_block_data_block_count = &block_list_current->header.data_block_count;
            data_block_count = 0;
        }
        else
        {
            data_block_count++;
        }
        
        block_list_current->next = allocate_temp(BlenderFileBlockHeaderNode , 1);
        (*block_list_current->next) = (BlenderFileBlockHeaderNode){};
        block_list_current = block_list_current->next;
        all_block_header_count++;
        
        if(compare_string_C(block_name , "DNA1" , 4))
        {
            
            //i'm not gonna write any auto-serialization from now on
            //i have been writing all these "DNA" parsing bs for half a day and still no idea where the bones are
            //this is so annoying
            //backward compatibility my ass
            unsigned char * DNA_memory = block_memory;
            
            DNA_memory += 8;
            
            int name_count = *((int*)DNA_memory);
            
            DNA_memory += 4;
            
            all_field_name = decompose_long_string(&DNA_memory , name_count);
            
            DNA_memory += 4;
            int type_count = *((int *)DNA_memory);
            DNA_memory += 4;
            
            all_types = decompose_long_string(&DNA_memory , type_count);
            for(int type_index = 0 ; type_index < type_count ; type_index++)
            {
                if(compare_string(all_types[type_index] , "void"))
                {
                    void_type_index = type_index;
                    break;
                }
            }
            
            if(void_type_index == -1) CATCH;
            
            type_to_structure = allocate_frame( int , type_count);
            
            for(int i = 0 ; i < type_count ; i++) type_to_structure[i] = -1;
            
            DNA_memory += 4;
            
            all_types_byte_length = allocate_frame(short , type_count);
            
            for(int type_index = 0 ; type_index < type_count ; type_index++ )
            {
                
                all_types_byte_length[type_index] = *((short *)DNA_memory);
                DNA_memory += 2;
                
            }
            
            //there is 2 extra bytes here?
            for(; !(*DNA_memory) ;DNA_memory++); 
            
            DNA_memory += 4;
            
            int structure_count = *((int *)DNA_memory);
            DNA_memory+=4;
            
            all_structures = allocate_frame(DNA_Structure , structure_count);
            all_structure_field = allocate_frame(FlattenFieldInfo * , structure_count);
            all_structure_field_count = allocate_frame(int , structure_count);
            
            for(int structure_index = 0 ; structure_index < structure_count ; structure_index++ )
            {
                
                all_structure_field[structure_index] = 0;
                DNA_Structure * current_structure = all_structures + structure_index;
                
                current_structure->type = *((short *)DNA_memory);
                DNA_memory += 2;
                
                type_to_structure[current_structure->type] = structure_index;
                
                int field_count = (*((short*) DNA_memory));
                DNA_memory += 2;
                
                short * all_fields = allocate_frame(short , field_count);
                short * all_field_name_index = allocate_frame(short , field_count);
                
                current_structure->field_count = field_count;
                current_structure->all_fields = all_fields;
                current_structure->all_field_name_index = all_field_name_index;
                
                for(int field_index = 0; field_index < field_count ; field_index++)
                {
                    all_fields[field_index] = (*((short*) DNA_memory));
                    DNA_memory += 2;
                    
                    all_field_name_index[field_index] = (* ( (short*)DNA_memory ));
                    DNA_memory +=2;
                }
                
            }
            
            break;
            
        }
        
    }
    
    all_block_header = allocate_temp( BlenderFileBlockHeader , all_block_header_count);
    block_address_hash_table = allocate_hash_table(all_block_header_count * 1.5);
    
    int all_block_header_index = 0;
    
    for(BlenderFileBlockHeaderNode * current_node = block_list_head ; current_node->next ; current_node = current_node->next)
    {
        
        add_to_hash_table( address_hash(current_node->header.old_memory_address) , all_block_header_index , &block_address_hash_table);
        all_block_header[all_block_header_index++] = current_node->header;
        
    }
    
}

internal void add_to_mesh_indices(unsigned short index)
{
    
    REALLOCATE_BUFFER_IF_TOO_SMALL(unsigned short , mesh_indices , mesh_indices_capacity , mesh_indices_count , allocate_frame_);
    mesh_indices[mesh_indices_count++] = index;
}

internal void load_blend_file()
{
    
    FILE * blend_file = fopen(get_app_file_path("Asset\\test.blend") , "rb");
    
    mesh_indices_capacity = 1024;
    mesh_indices = allocate_frame(unsigned short , mesh_indices_capacity);
    
    mesh_vertices_capacity = 1024;
    mesh_vertices = allocate_frame(Vector3 , mesh_vertices_capacity);
    
    bone_buffer_capacity = 64;
    bone_buffer = allocate_frame(BlenderBone , bone_buffer_capacity);
    
    if(blend_file)
    {
        
        char header[12] = {};
        
        fread(header , 12 , 1 , blend_file);
        
        if(header[7] == '-') blend_file_pointer_size = 8;
        
        load_header_and_DNA(blend_file);
        
        for(int block_index = 0 ; block_index < all_block_header_count ; block_index++)
        {
            
            BlenderFileBlockHeader * current_block = all_block_header + block_index;
            
            if(compare_string_C(current_block->block_name , "GLOB" , 4))
            {
                //there is nothing here
                //print_all_fields(current_block->block_memory , current_block->SDNA_index);
                //printf("\n");
                
            }
            else if(compare_string_C(current_block->block_name , "OB" , 4))
            {
                
                //the type in data is different from DNA
                //same as modifier, i can only get the type from DNA structure
                
                unsigned long long data_address = get_field("*data" , current_block->block_memory , current_block->SDNA_index).address;
                unsigned long long modifier_address = get_field("modifiers->*first" , current_block->block_memory , current_block->SDNA_index).address;
                
                //print_all_fields(current_block->block_memory , current_block->SDNA_index);
                
                BlenderFileBlockHeader * data_block = get_block_by_address(data_address);
                char * data_type_name = get_type_name_by_structure_index(data_block->SDNA_index);
                
                if(compare_string(data_type_name , "Mesh"))
                {
                    
                    D_Model * new_model = all_models + model_count++;
                    
                    mesh_indices_count =0;
                    mesh_vertices_count = 0;
                    bone_buffer_count = 0;
                    
                    //print_all_fields(data_block->block_memory , data_block->SDNA_index);
                    
                    FieldResult vertex_field_result = get_field("*mvert" , data_block->block_memory , data_block->SDNA_index);
                    BlenderFileBlockHeader * vertex_block = get_block_by_address(vertex_field_result.address);
                    
                    FieldResult edge_field_result = get_field("*medge" , data_block->block_memory , data_block->SDNA_index);
                    BlenderFileBlockHeader * edge_block = get_block_by_address(edge_field_result.address);
                    
                    FieldResult polygon_field_result = get_field("*mpoly" , data_block->block_memory , data_block->SDNA_index);
                    BlenderFileBlockHeader * polygon_block = get_block_by_address(polygon_field_result.address);
                    
                    FieldResult loop_field_result = get_field("*mloop" , data_block->block_memory , data_block->SDNA_index);
                    BlenderFileBlockHeader * loop_block = get_block_by_address(loop_field_result.address);
                    
                    BlenderVertex * all_vertex = (BlenderVertex *)vertex_block->block_memory;
                    BlenderEdge * all_edge = (BlenderEdge *)edge_block->block_memory;
                    BlenderLoop * all_loop = (BlenderLoop *)loop_block->block_memory;
                    BlenderPolygon * all_polygon = (BlenderPolygon *)polygon_block->block_memory;
                    
                    for(int vertex_index = 0; vertex_index < vertex_block->SDNA_count ; vertex_index++)
                    {
                        
                        BlenderVertex * vertex = all_vertex + vertex_index;
                        
                        REALLOCATE_BUFFER_IF_TOO_SMALL(Vector3 , mesh_vertices , mesh_vertices_capacity , mesh_vertices_count , allocate_frame_);
                        mesh_vertices[mesh_vertices_count++] = vertex->position;
                    }
                    
                    for(int polygon_index = 0 ; polygon_index < polygon_block->SDNA_count ; polygon_index++)
                    {
                        
                        BlenderPolygon * polygon = all_polygon + polygon_index;
                        
                        if(polygon->loop_count >= 4)
                        {
                            //hope this just work
                            BlenderLoop * loop_slice = all_loop + polygon->loop_start;
                            
                            for(int triangle_index = 0 ; triangle_index < polygon->loop_count - 2 ; triangle_index++)
                            {
                                
                                add_to_mesh_indices(loop_slice[0].vertex_index);
                                add_to_mesh_indices(loop_slice[triangle_index + 1].vertex_index);
                                add_to_mesh_indices(loop_slice[triangle_index + 2].vertex_index);
                                
                            }
                            
                        }
                        else
                        {
                            
                            for(int slice_index = 0 ; slice_index < polygon->loop_count ; slice_index++)
                            {
                                
                                int loop_index = polygon->loop_start + slice_index;
                                
                                add_to_mesh_indices(all_loop[loop_index].vertex_index);
                                
                            }
                        }
                    }
                    
                    int vertex_count = mesh_vertices_count;
                    int index_count = mesh_indices_count;
                    
                    new_model->vertex_count = vertex_count;
                    new_model->index_count = index_count;
                    
                    new_model->vertices = allocate_temp(Vector3  , vertex_count);
                    new_model->indices = allocate_temp(unsigned short  , index_count);
                    new_model->normals = allocate_temp(Vector3 , index_count);
                    
                    for(int vertex_index = 0 ; vertex_index < vertex_count ; vertex_index++)
                    {
                        new_model->vertices[vertex_index] = Vector3Scale(mesh_vertices[vertex_index], model_scale);
                    }
                    
                    //memcpy( ,  , sizeof(Vector3) * vertex_count );
                    memcpy(new_model->indices , mesh_indices , sizeof(unsigned short) * index_count);
                    
                    for(int index = 0 ; index <  index_count ; )
                    {
                        
                        Vector3 position_a = mesh_vertices[mesh_indices[index]];
                        Vector3 position_b = mesh_vertices[mesh_indices[index+1]];
                        Vector3 position_c = mesh_vertices[mesh_indices[index+2]];
                        
                        Vector3 a_to_b = Vector3Subtract(position_b , position_a);
                        Vector3 a_to_c = Vector3Subtract(position_c , position_a);
                        
                        Vector3 normal = Vector3CrossProduct( a_to_b , a_to_c );
                        normal = Vector3Normalize(normal);
                        
                        new_model->normals[index] = normal;
                        new_model->normals[index + 1] = normal;
                        new_model->normals[index + 2] = normal;
                        
                        index+=3;
                        
                    }
                    
                    //print_all_fields(vertex_block->block_memory , type_to_structure[vertex_field_result.type]);
                    //print_all_fields(edge_block->block_memory , type_to_structure[edge_field_result.type]);
                    //print_all_fields(polygon_block->block_memory , type_to_structure[polygon_field_result.type]);
                    //print_all_fields(loop_block->block_memory , type_to_structure[loop_field_result.type]);
                    
                    FixedStringW * vertex_group_name_buffer = 0;
                    int vertex_group_name_buffer_count = 0;
                    int vertex_group_name_buffer_capacity = 1024;
                    
                    vertex_group_name_buffer = allocate_frame(FixedStringW , vertex_group_name_buffer_capacity);
                    
                    FieldResult vertex_group_field_result = get_field("vertex_group_names->*first" , data_block->block_memory , data_block->SDNA_index);
                    
                    if(vertex_group_field_result.address)
                    {
                        BlenderFileBlockHeader * vertex_group_block = get_block_by_address(vertex_group_field_result.address);
                        int vertex_group_structure_index = vertex_group_block->SDNA_index;
                        
                        for(;;)
                        {
                            
                            print_all_fields(vertex_group_block->block_memory , vertex_group_structure_index);
                            
                            FieldResult name_result = get_field("name[64]" , vertex_group_block->block_memory , vertex_group_structure_index);
                            
                            REALLOCATE_BUFFER_IF_TOO_SMALL(FixedStringW , vertex_group_name_buffer , vertex_group_name_buffer_capacity , vertex_group_name_buffer_count , allocate_frame_);
                            FixedStringW * new_group_name = vertex_group_name_buffer + vertex_group_name_buffer_count++;
                            FixedString * name = ((FixedString*)name_result.data);
                            
                            char_to_wide_char(new_group_name->string , name->string , FIXED_STRING_SIZE);
                            
                            vertex_group_field_result = get_field("*next" , vertex_group_block->block_memory , vertex_group_structure_index);
                            if(!vertex_group_field_result.address) break;
                            
                            vertex_group_block = get_block_by_address(vertex_group_field_result.address);
                            vertex_group_structure_index = type_to_structure[vertex_group_field_result.type];
                            
                        }
                        
                    }
                    
                    DeformVertexSlice * all_deform_vertex_slice = allocate_temp(DeformVertexSlice , vertex_count);
                    
                    DeformVertex * deform_vertex_buffer = 0;
                    int deform_vertex_buffer_count = 0;
                    int deform_vertex_buffer_capacity = 1024;
                    
                    deform_vertex_buffer = allocate_frame(DeformVertex , deform_vertex_buffer_capacity);
                    
                    FieldResult deform_vertex_field_result = get_field("*dvert" , data_block->block_memory , data_block->SDNA_index);
                    
                    unsigned long long deform_vertex_address = deform_vertex_field_result.address;
                    
                    BlenderFileBlockHeader * deform_vertex_block = get_block_by_address(deform_vertex_field_result.address);
                    //print_all_fields(deform_vertex_block->block_memory , type_to_structure[deform_vertex_field_result.type]);
                    
                    BlenderDeformVertex * all_deform_vertex = (BlenderDeformVertex*)deform_vertex_block->block_memory;
                    
                    int deform_vertex_count = 0;
                    
                    for(int vertex_index = 0 ; vertex_index < vertex_count ; vertex_index++)
                    {
                        DeformVertexSlice * slice = all_deform_vertex_slice + vertex_index;
                        BlenderDeformVertex * deform_vertex = all_deform_vertex + vertex_index;
                        
                        BlenderFileBlockHeader * deform_weight_block = get_block_by_address(deform_vertex->weight_address);
                        BlenderWeight * all_weights = (BlenderWeight *)deform_weight_block->block_memory;
                        
                        slice->start = deform_vertex_count;
                        
                        for(int weight_index = 0 ; weight_index < deform_vertex->weight_count ; weight_index++)
                        {
                            
                            BlenderWeight weight = all_weights[weight_index];
                            
                            if(weight.weight < 0.001f) continue;
                            
                            REALLOCATE_BUFFER_IF_TOO_SMALL(DeformVertex , deform_vertex_buffer , deform_vertex_buffer_capacity , deform_vertex_buffer_count , allocate_frame_);
                            DeformVertex * new_vertex = deform_vertex_buffer + deform_vertex_buffer_count++;
                            
                            new_vertex->bone_index = weight.group_index;
                            new_vertex->weight = weight.weight;
                            deform_vertex_count++;
                            slice->count++;
                            
                            //printf("%s : %f\n" , vertex_group_name_buffer[weight.group_index].string , weight.weight);
                            
                        }
                        
                        //printf("\n");
                        //print_all_fields(deform_weight_block->block_memory , type_to_structure[deform_weight_result.type]);
                        
                    }
                    
                    new_model->deform_vertex_slice = all_deform_vertex_slice;
                    new_model->all_deform_vertex = allocate_temp(DeformVertex , deform_vertex_count);
                    new_model->deform_vertex_count = deform_vertex_count;
                    
                    for(int deform_vertex_index = 0 ; deform_vertex_index < deform_vertex_count ; deform_vertex_index++)
                    {
                        new_model->all_deform_vertex[deform_vertex_index] = deform_vertex_buffer[deform_vertex_index];
                    }
                    
                    for(;modifier_address;)
                    {
                        
                        BlenderFileBlockHeader * modifier_block = get_block_by_address(modifier_address);
                        if(modifier_block->SDNA_index == 0) CATCH;//what am i suppose to do with this?
                        
                        char * modifier_type_name = get_type_name_by_structure_index(modifier_block->SDNA_index);
                        
                        //it seem there can be 2 or more armature?
                        //having that many bone sure make a good soup
                        if(compare_string(modifier_type_name , "ArmatureModifierData"))
                        {
                            
                            //print_all_fields(modifier_block->block_memory , modifier_block->SDNA_index);
                            
                            unsigned long long armature_object_address = get_field("*object" , modifier_block->block_memory , modifier_block->SDNA_index).address;
                            BlenderFileBlockHeader * armature_object_block = get_block_by_address(armature_object_address);
                            
                            unsigned long long armature_address = get_field("*data" , armature_object_block->block_memory , armature_object_block->SDNA_index).address;
                            BlenderFileBlockHeader * armature_block = get_block_by_address(armature_address);
                            
                            //print_all_fields(armature_block->block_memory , armature_block->SDNA_index);
                            
                            FieldResult bone_result = get_field("bonebase->*first" , armature_block->block_memory , armature_block->SDNA_index);
                            
                            blender_iterate_bone(bone_result.address , bone_result.type ,0  , -1);
                            
                            int bone_count = bone_buffer_count;
                            new_model->bone_count = bone_count;
                            new_model->initial_bone_count = bone_count;
                            
                            int bone_capacity = 1;
                            for( ; bone_capacity < bone_count ; bone_capacity *= 2);
                            new_model->bone_capacity = bone_capacity;
                            new_model->initial_bone_capacity = bone_capacity;
                            
                            new_model->all_bones = allocate_temp(Bone , bone_capacity);
                            new_model->all_initial_bone = allocate_temp(Bone , bone_capacity);
                            new_model->root_bone_list = allocate_list(bone_capacity);
                            new_model->bone_children_hash_table = allocate_hash_table(bone_capacity);
                            
                            for(int bone_index = 0 ; bone_index < bone_count ; bone_index++)
                            {
                                
                                Bone * model_bone = new_model->all_bones + bone_index;
                                BlenderBone * blender_bone = bone_buffer + bone_index;
                                
                                Vector3 head = {};
                                head.x = blender_bone->head.x;
                                head.y = blender_bone->head.z;
                                head.z = -blender_bone->head.y;
                                
                                Vector3 tail = {};
                                tail.x = blender_bone->tail.x;
                                tail.y = blender_bone->tail.z;
                                tail.z = -blender_bone->tail.y;
                                
                                tail = Vector3Scale(tail , model_scale);
                                head = Vector3Scale(head , model_scale);
                                
                                model_bone->parent_bone_index = blender_bone->parent_index;
                                model_bone->from_blend_file = true;
                                model_bone->bone_index = bone_index;
                                model_bone->bone_name = blender_bone->name;
                                model_bone->state.local_position = head;
                                model_bone->state.end_point_offset = Vector3Subtract(tail , head);
                                model_bone->IK_target_bone_index = -1;
                                model_bone->IK_pole_bone_index = -1;
                                //model_bone->state.direction = tail;
                                
                                //model_bone->state.local_rotation = blender_bone->rotation;
                                model_bone->state.local_rotation = QuaternionIdentity();
                                
                                //printf("%s : %f %f %f -->" , blender_bone->name.string , head.x , head.y , head.z);
                                //printf("%f %f %f\n" , tail.x , tail.y , tail.z);
                                //printf("%s : %f\n" , blender_bone->name.string , blender_bone->roll * RAD2DEG);
                                
                                if(blender_bone->parent_index == -1)
                                {
                                    add_to_list_tail(bone_index, &new_model->root_bone_list);
                                }
                                else
                                {
                                    add_to_hash_table( blender_bone->parent_index , bone_index , &new_model->bone_children_hash_table);
                                }
                                
                            }
                            
                            //i couldn't construct armature from base bone so i used armature bone ,which store data in relavtive to armature
                            //had to travsel to subtract its parent offset
                            //honestly this just much better, i don't what i did wrong but i have try mixing the offset from base bone and the bone matrix.
                            list_foreach(root_bone_index , &new_model->root_bone_list)
                            {
                                Bone * root_bone = new_model->all_bones + root_bone_index;
                                
                                int parent_bone_index_stack[256] = {};
                                Vector3 parent_bone_position_stack[256] = {};
                                int bone_stack_count = 0;
                                
                                parent_bone_index_stack[bone_stack_count] = root_bone_index;
                                parent_bone_position_stack[bone_stack_count] = (Vector3){};
                                bone_stack_count++;
                                
                                for(;bone_stack_count > 0;)
                                {
                                    
                                    bone_stack_count--;
                                    int parent_bone_index = parent_bone_index_stack[bone_stack_count];
                                    Vector3 parent_position = parent_bone_position_stack[bone_stack_count];
                                    
                                    Bone * parent_bone = new_model->all_bones + parent_bone_index;
                                    
                                    parent_bone->state.local_position = Vector3Subtract(parent_bone->state.local_position , parent_position);
                                    parent_position = Vector3Add(parent_bone->state.local_position , parent_position);
                                    
                                    hash_table_iterate(child_index , parent_bone_index , &new_model->bone_children_hash_table)
                                    {
                                        
                                        parent_bone_index_stack[bone_stack_count] = child_index;
                                        parent_bone_position_stack[bone_stack_count] = parent_position;
                                        bone_stack_count++;
                                        
                                    }
                                    
                                }
                                
                            }
                            
                            int * bone_map = allocate_frame(int , new_model->bone_count);
                            
                            for(int bone_index = 0 ; bone_index < bone_count ; bone_index++)
                            {
                                
                                BlenderBone * blender_bone = bone_buffer + bone_index;
                                
                                for(int group_bone_index = 0 ; group_bone_index < bone_count ; group_bone_index++)
                                {
                                    FixedStringW group_bone_name = vertex_group_name_buffer[group_bone_index];
                                    
                                    if(compare_string_W( group_bone_name.string , blender_bone->name.string))
                                    {
                                        bone_map[group_bone_index] = bone_index;
                                        break;
                                    }
                                    
                                }
                                
                            }
                            
                            for(int deform_vertex_index = 0 ; deform_vertex_index < new_model->deform_vertex_count ; deform_vertex_index++)
                            {
                                DeformVertex * deform_vertex = new_model->all_deform_vertex + deform_vertex_index;
                                deform_vertex->bone_index = bone_map[deform_vertex->bone_index];
                            }
                            
                            selected_model_index = 0;
                            selected_model = all_models + selected_model_index;
                            
                            break;
                        }
                        
                        modifier_address = get_field("modifier->*next" , modifier_block->block_memory , modifier_block->SDNA_index).address;
                    }
                    
                }
                
            }
            
        }
        
        fclose(blend_file);
    }
    
}
