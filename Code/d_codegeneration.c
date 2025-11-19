#include <wchar.h> //for chinese characters
#include <stdio.h> //just file io and printing stuff 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CATCH *(int *)0=1

typedef enum D_TokenType D_TokenType;
enum D_TokenType
{
	T_identifier,
	T_colon,
	T_semicolon,
	T_bracket_left,
	T_bracket_right,
	T_equal_sign,
	T_asterisk,
	T_comma,
	T_minus,
	T_divide,
	T_plus,
	T_numeric,
	T_stop_sign,
	T_square_bracket_left,
	T_square_bracket_right,
	T_pound_sign,
	T_quotation_mark,
	T_apostrophe,
	T_parentheses_left,
	T_parentheses_right,
	T_less_than,
	T_greater_than,
	T_excalmation_mark,
	T_back_slash,
	T_slash,
	T_ampersand,
	T_question_mark,
	T_vertical_bar,
	T_percent_sign,
	T_hyphen,
	T_wave_sign,
};

typedef struct Token Token;
struct Token
{
	D_TokenType type;
	char* string;
	short string_length;
	Token* next_token;
};

typedef struct TokenHeaderList TokenHeaderList;
struct TokenHeaderList
{
	Token* token_header;
	TokenHeaderList* next_header;
};

typedef struct FunctionData FunctionData;
struct FunctionData
{
	bool is_local;
    
	Token * start_token;
    
	Token * right_parentheses_token;
	Token * left_bracket_token;
    
	Token * right_bracket_token;
    
	FunctionData * local_function_list;
	FunctionData * current_local_function_data;
    
	FunctionData * next;
};

typedef struct TypeData TypeData;
struct TypeData
{
    Token name_token;
    
    Token typedef_token;
    
    bool is_struct;
    bool is_typedef;
	bool is_enum;
    
    bool not_defined;
    
    TypeData* next_data;
};

#define MAX_TOKEN_CHAIN 32

typedef struct StructMemberData StructMemberData;
struct StructMemberData
{
    
	Token * token_chain[MAX_TOKEN_CHAIN];
    
	int token_chain_length;
    
	Token* member_name_token;
	int member_type_token_array_count;
    
	bool is_pointer;
	bool is_multiple_pointer;
	int first_left_bracket_index;
	int equad_sign_index;
    
	StructMemberData* next_data;
};

//i don't need this right?
typedef struct GenericData GenericData;
struct GenericData
{
	Token * type_token;
	Token * generic_token;
    
	char * last_seen_struct_position;
    
	GenericData * next;
};

static TypeData* unknow_type_data_header = 0 ;

static Token* current_token = 0;
static char* text_string =0;
static FILE* meta_data_file=0;
static TypeData* current_type_data =0;
static StructMemberData* current_struct_member_data = 0;

static bool match_identifier(Token * token , const char * target_string)
{
    if(token->type != T_identifier) return false;
    
    int target_string_length = strlen(target_string);
    if(token->string_length != target_string_length) return false;
    
    return (!strncmp(token->string , target_string, target_string_length));
}

static bool find_string(char * source_string , const char * target_string)
{
	return (!strncmp(source_string, target_string, strlen(target_string)));
}

static bool find_string_exact(char * source_string, const char * target_string , int count)
{
	return (!strncmp(source_string, target_string, count ));
}

static void to_next_token()
{
	current_token->next_token = (Token*)malloc(sizeof(Token));
	current_token = current_token->next_token;
	(*current_token) = (Token){};
}

static void add_token(D_TokenType type)
{
	current_token->type = type;
	current_token->string = text_string;
	current_token->string_length = 1;
    
	to_next_token();
}

static void go_to_next_line()
{
	for (; (*text_string) != '\n'; text_string++);
}

static bool is_the_start_of_the_identifier(char character)
{
	if (character >= 'A' && character <= 'Z')
	{
		return true;
	}
	else if (character >= 'a' && character <= 'z')
	{
		return true;
	}
	else if (character == '_')
	{
		return true;
	}
    
	return false;
}

static bool is_part_of_the_identifier(char character)
{
	if (character >= 'A' && character <= 'Z')
	{
		return true;
	}
	else if (character >= 'a' && character <= 'z')
	{
		return true;
	}
	else if (character >= '0' && character <= '9')
	{
		return true;
	}
	else if (character == '_')
	{
		return true;
	}
    
	return false;
}

static void add_type_data(const char* string )
{
    current_type_data->name_token.string = (char*)string;
    current_type_data->name_token.string_length = strlen(string);
	
    current_type_data->next_data = (TypeData*)malloc(sizeof(TypeData));
	current_type_data = current_type_data->next_data;
	(*current_type_data) = (TypeData){};
}

static char* check_previosToken(char * rewind_string, const char * target_string)
{
	char* result = rewind_string;
    
	int target_length = strlen(target_string);
    
	rewind_string--;
	for (; (*rewind_string) == ' '; rewind_string--);
    
	if (is_part_of_the_identifier(*rewind_string))
	{
		for (; is_part_of_the_identifier(*rewind_string); rewind_string--);
		rewind_string++;
	}
    
	if (!strncmp(target_string, rewind_string, target_length))
	{
		result = rewind_string;
	}
    
	return result;
}

typedef struct IntrospectionData IntrospectionData;
struct IntrospectionData
{
	bool is_valid;
	char* struct_name;
	int struct_name_length;
	int member_count;
	StructMemberData* struct_member_header;
    
	IntrospectionData * next;
};

static IntrospectionData generate_introspect_data(Token* current_struct_token)
{
	IntrospectionData result = {};
	result.is_valid = false;
    
	StructMemberData* current_struct_member_data = (StructMemberData*)malloc(sizeof(StructMemberData));
	(*current_struct_member_data) = (StructMemberData){};
    
	result.struct_member_header = current_struct_member_data;
    
	current_struct_token = current_struct_token->next_token;
    
	if (current_struct_token->type != T_identifier) CATCH;
    
	result.struct_name= current_struct_token->string;
	result.struct_name_length = current_struct_token->string_length;
    
	current_struct_token = current_struct_token->next_token;
    
	if (current_struct_token->type != T_bracket_left)
    {
        if(current_struct_token->type == T_semicolon) 
        {
            result.is_valid = false;
            return result;
        }
        
        CATCH;
    }
    
	current_struct_token = current_struct_token->next_token;
    
	(*current_struct_member_data) = (StructMemberData){};
	current_struct_member_data->first_left_bracket_index = -1;
	current_struct_member_data->is_pointer = false;
	current_struct_member_data->is_multiple_pointer = false;
	current_struct_member_data->equad_sign_index = -1;
	current_struct_member_data->member_name_token = 0;
	current_struct_member_data->token_chain_length = 0;
    
	for (; ; current_struct_token = current_struct_token->next_token)
	{
		if (current_struct_token->type == T_semicolon)
		{
			int NameTokenIndex = current_struct_member_data->token_chain_length-1;
            
			if (current_struct_member_data->equad_sign_index != -1)
			{
				NameTokenIndex = current_struct_member_data->equad_sign_index - 1;
			}
            
			if (current_struct_member_data->first_left_bracket_index != -1)
			{
				NameTokenIndex = current_struct_member_data->first_left_bracket_index - 1;
			}
            
			current_struct_member_data->member_name_token = current_struct_member_data->token_chain[NameTokenIndex];
            
			int type_tndex = NameTokenIndex - 1;
            
			current_struct_member_data->member_type_token_array_count = NameTokenIndex ;
            
			result.member_count++;
            
			current_struct_member_data->next_data = (StructMemberData*)malloc(sizeof(StructMemberData));
			current_struct_member_data = current_struct_member_data->next_data;
            
			(*current_struct_member_data) = (StructMemberData){};
            
			current_struct_member_data->is_multiple_pointer = false;
			current_struct_member_data->is_pointer = false;
			current_struct_member_data->equad_sign_index = -1;
			current_struct_member_data->first_left_bracket_index = -1;
			
			current_struct_member_data->member_name_token = 0;
			current_struct_member_data->token_chain_length = 0;
			current_struct_member_data->next_data = 0;
		}
		else if (current_struct_token->type == T_equal_sign)
		{
			current_struct_member_data->equad_sign_index = current_struct_member_data->token_chain_length;
		}
		else if (current_struct_token->type == T_asterisk)
		{
			if (current_struct_member_data->is_pointer)
			{
				current_struct_member_data->is_multiple_pointer = true;
			}
            
			current_struct_member_data->is_pointer = true;
		}
		else if (current_struct_token->type == T_square_bracket_left)
		{
			if (current_struct_member_data->first_left_bracket_index == -1)
			{
				current_struct_member_data->first_left_bracket_index = current_struct_member_data->token_chain_length;
			}
		}
		else if (current_struct_token->type == T_bracket_left)
		{
			int LeftBracketMeet = 1;
            
			current_struct_token = current_struct_token->next_token;
            
			for (; LeftBracketMeet; current_struct_token = current_struct_token->next_token)
			{
				if (current_struct_token->type == T_bracket_left)
					LeftBracketMeet++;
                
				if (current_struct_token->type == T_bracket_right)
					LeftBracketMeet--;
			}
            
			(*current_struct_member_data) = (StructMemberData){};
            
			current_struct_member_data->is_multiple_pointer = false;
			current_struct_member_data->is_pointer = false;
			current_struct_member_data->equad_sign_index = -1;
			current_struct_member_data->first_left_bracket_index = -1;
            
			current_struct_member_data->member_name_token = 0;
			current_struct_member_data->token_chain_length = 0;
			current_struct_member_data->next_data = 0;
		}
		else if (current_struct_token->type == T_parentheses_left)
		{
			int LeftParenthnessMeet = 1;
            
			current_struct_token = current_struct_token->next_token;
            
			for (; LeftParenthnessMeet; current_struct_token = current_struct_token->next_token)
			{
				if (current_struct_token->type == T_parentheses_left)
					LeftParenthnessMeet++;
                
				if (current_struct_token->type == T_parentheses_right)
					LeftParenthnessMeet--;
			}
            
			if (current_struct_token->type == T_semicolon)
				current_struct_token = current_struct_token->next_token;
            
			(*current_struct_member_data) = (StructMemberData){};
			current_struct_member_data->is_pointer= false;
			current_struct_member_data->is_multiple_pointer = false;
			current_struct_member_data->equad_sign_index = -1;
			current_struct_member_data->first_left_bracket_index = -1;
            
			current_struct_member_data->member_name_token = 0;
			current_struct_member_data->token_chain_length = 0;
			current_struct_member_data->next_data = 0;
		}
        
		if (current_struct_token->type == T_bracket_right)
		{
			break;
		}
        
		current_struct_member_data->token_chain[current_struct_member_data->token_chain_length++] = current_struct_token;
	}
    
	current_struct_token = current_struct_token->next_token;
    
	result.is_valid = true;
	
	return result;
}

static Token* convert_text_to_token(char * text , int text_size)
{
    
	Token* token_header = (Token*)malloc(sizeof(Token));
	current_token = token_header;
	(*current_token) = (Token){};
    
	bool inside_string=false;
    
	for (text_string = text; text_string < text + text_size; text_string++)
	{
		if ((*text_string) == '\0')
			break;
        
		switch (*text_string)
		{
            case '\0':
			break;
            case '/':
			if (text_string[1] == '/')
			{
				go_to_next_line();
			}
			else if (text_string[1] == '*')
			{
				for (; !((text_string[0] == '*') && (text_string[1] == '/')); text_string++);
			}
			else if (text_string[1] == '\0')
			{
				break;
			}
			else
			{
				add_token(T_slash);
			}
			break;
            
            case '#':
			//no idea how to handle this
			add_token(T_pound_sign);
            
			for (; text_string[1] == ' '; text_string++);
            
			if (!strncmp(text_string+1, "define", 6))
			{
				for (; text_string[1] != '\n'; text_string++)
				{
					if ((text_string[1]) == '\\')
						text_string += 2;
				}
			}
            
			break;
            
            case ' ':
            case '\n':
            case '\r':
            case '\t':
			break;
            
            case ':': add_token(T_colon); break;
            case ';': add_token(T_semicolon); break;
            case '{': add_token(T_bracket_left); break;
            case '}': add_token(T_bracket_right); break;
            case '+': add_token(T_plus); break;
            case '-': add_token(T_minus); break;
            case '=': add_token(T_equal_sign); break;
            case '*': add_token(T_asterisk); break;
            case ',': add_token(T_comma); break;
            case '.': add_token(T_stop_sign); break;
            case '[': add_token(T_square_bracket_left); break;
            case ']': add_token(T_square_bracket_right); break;
            case '(': add_token(T_parentheses_left); break;
            case ')': add_token(T_parentheses_right); break;
            case '<': add_token(T_less_than); break;
            case '>': add_token(T_greater_than); break;
            case '!': add_token(T_excalmation_mark); break;
            case '\'': add_token(T_apostrophe); break;
            case '\\': add_token(T_back_slash); break;
            case '&': add_token(T_ampersand); break;
            case '?': add_token(T_question_mark); break;
			case '|': add_token(T_vertical_bar); break;
			case '%':add_token(T_percent_sign); break;
			case '^':add_token(T_hyphen); break;
			case '~':add_token(T_wave_sign); break;
            
            case '\"':
            {
                if (*(text_string-1)!= '\\')
                {
                    inside_string = !inside_string;
                }
                
                add_token(T_quotation_mark);
                break;
            }
            default:
			if (is_the_start_of_the_identifier(*text_string))
			{
				current_token->type = T_identifier;
				current_token->string = text_string;
				for (; is_part_of_the_identifier(text_string[1]); text_string++);
				current_token->string_length = text_string - current_token->string + 1;
				to_next_token();
			}
			else if ((*text_string) >= '0' && (*text_string) <= '9')
			{
				current_token->type = T_numeric;
				current_token->string = text_string;
				for (; (text_string[1]) >= '0' && (text_string[1]) <= '9'; text_string++);
				current_token->string_length = text_string - current_token->string + 1;
				to_next_token();
			}
			else if (inside_string)
			{
				//don't care what inside in the string
			}
			else
			{
				char current_invalid_char = *text_string;
                
				int current_length = text_string - text;
				CATCH;
			}
            
			break;
		}
	}
    
	return token_header;
}

static Token* convert_file_text_to_token(FILE* code_file , char ** out_text , long * out_text_size)
{
    
	fseek(code_file, 0, SEEK_END);
	long file_size = ftell(code_file);
    
	fseek(code_file, 0, SEEK_SET);
    
	char* text = (char*)malloc((file_size + 1) * sizeof(char));
	fread(text, file_size, 1, code_file);
	fclose(code_file);
    
	text[file_size] = '\0';
    
	(*out_text) = text;
	(*out_text_size) = file_size;
    
	printf("%d\n" , file_size);
    
	return convert_text_to_token(text, file_size);
}

static bool is_generic_type_token(Token * keyword)
{	
	if (find_string_exact(keyword->string , "Array" , keyword->string_length)) return true;
	if (find_string_exact(keyword->string , "hash_table" , keyword->string_length)) return true;
    
	return false;
}

static void modify_generic_type_token(Token * token_header)
{
	for (current_token = token_header; current_token->next_token; current_token = current_token->next_token)
	{
		Token * generic_type_start_token = current_token->next_token;
        
		if (!generic_type_start_token->next_token) break;
        
		if (generic_type_start_token->type != T_identifier) continue;
        
		if (generic_type_start_token->next_token->type != T_parentheses_left) continue;
		
		Token * type_token = generic_type_start_token->next_token->next_token;
        
		if (type_token->type != T_identifier) continue;
        
		if (!is_generic_type_token(generic_type_start_token)) continue;
        
		char * new_token_string = (char *)malloc(sizeof(char) * 64);
		memset(new_token_string , '\0', 64);
		strcat(new_token_string , "D_");
		strncat(new_token_string , generic_type_start_token->string , generic_type_start_token->string_length);
		strcat(new_token_string , "_");
        
		strncat(new_token_string , type_token->string , type_token->string_length);
        
		if (type_token->next_token->type != T_parentheses_right) CATCH;
        
		Token * new_token = (Token *)malloc(sizeof(Token));
		(*new_token) = (Token){};
        
		new_token->string = new_token_string;
		new_token->string_length = strlen(new_token_string);
        
		current_token->next_token = new_token;
		new_token->next_token = type_token->next_token->next_token;
	}
}

static void generate_generic_type(char * current_buffer , GenericData * new_generic)
{
    
	if (find_string_exact(new_generic->generic_token->string, "Array" ,new_generic->generic_token->string_length))
	{
		current_buffer += sprintf(current_buffer, "struct D_Array_");
        
		current_buffer += sprintf(current_buffer, "%.*s" ,new_generic->type_token->string_length , new_generic->type_token->string);
        
		current_buffer += sprintf(current_buffer, "\n{\n");
        
		current_buffer += sprintf(current_buffer, "\t%.*s" , new_generic->type_token->string_length , new_generic->type_token->string);
		current_buffer += sprintf(current_buffer, " persist * DataArray;\n");
        
		current_buffer += sprintf(current_buffer, "\tint ArrayMaxSize;\n");
		current_buffer += sprintf(current_buffer, "};\n\n");
        
	}
	else if (find_string_exact(new_generic->generic_token->string, "hash_table",new_generic->generic_token->string_length))
	{
        
		current_buffer += sprintf(current_buffer, "struct D_hash_table_");
        
		current_buffer += sprintf(current_buffer, "%.*s" , new_generic->type_token->string_length , new_generic->type_token->string);
        
		current_buffer += sprintf(current_buffer, "\n{\n");
        
		current_buffer += sprintf(current_buffer, "\t%.*s" , new_generic->type_token->string_length , new_generic->type_token->string);
		current_buffer += sprintf(current_buffer, " persist * DataArray;\n");
		
		current_buffer += sprintf(current_buffer, "\t%.*s" , new_generic->type_token->string_length , new_generic->type_token->string);
		current_buffer += sprintf(current_buffer, " persist * DataTail;\n");
        
		current_buffer += sprintf(current_buffer, "\thash_tableSlot persist * HashSlotArray;\n");
		current_buffer += sprintf(current_buffer, "\thash_tableSlot persist * HashSlotTail;\n");
        
		current_buffer += sprintf(current_buffer, "};\n\n");
        
	}
	else
	{
		CATCH;
	}
    
	current_buffer += sprintf(current_buffer, "\0");
    
}

static char * found_and_generate_generic_type(GenericData** tail , GenericData* generic_list_to_compare , char * text , int text_size, Token * token_header , FILE * output_file)
{
    
	GenericData * current_generic = (*tail);
	char * last_seen_struct_position = 0;
	GenericData * new_generic = current_generic;
    
	for (current_token = token_header; current_token->next_token; current_token = current_token->next_token)
	{
		if (current_token->type == T_identifier)
		{
			if (find_string(current_token->string , "struct"))
			{
				last_seen_struct_position = current_token->string;
			}
		}
        
		Token * generic_type_start_token = current_token->next_token;
        
		if (!generic_type_start_token->next_token) break;
        
		if (generic_type_start_token->type != T_identifier) continue;
        
		if (generic_type_start_token->next_token->type != T_parentheses_left) continue;
		Token * type_token = generic_type_start_token->next_token->next_token;
        
		if (type_token->type != T_identifier) continue;
        
		if (!is_generic_type_token(generic_type_start_token)) continue;
        
		bool already_exist = false;
        
		for (GenericData * comparing_generic = generic_list_to_compare ; comparing_generic->next ;comparing_generic= comparing_generic->next)
		{
            
			if (!strncmp(comparing_generic->type_token->string , type_token->string , comparing_generic->type_token->string_length))
			{
				if (!strncmp(comparing_generic->generic_token->string , generic_type_start_token->string , comparing_generic->generic_token->string_length))
				{
					already_exist = true;
					break;
				}
			}
		}
        
		if (!already_exist)
		{
			current_generic->last_seen_struct_position = last_seen_struct_position;
			current_generic->type_token = type_token;
			current_generic->generic_token = generic_type_start_token;
            
			current_generic->next = (GenericData *) malloc(sizeof(GenericData));
			current_generic = current_generic->next;
			(*current_generic) = (GenericData){};
		}
        
	}
    
	typedef struct TextBlock TextBlock;
	struct TextBlock
	{
		bool is_insert_text;
        
		char * start;
		char * end;
        
		TextBlock * previous;
		TextBlock * next;
	};
    
	TextBlock * text_block_head = (TextBlock * )malloc(sizeof(TextBlock));
    
	(*text_block_head) = (TextBlock){};
    
	text_block_head->next = (TextBlock *)malloc(sizeof(TextBlock));
	(*text_block_head->next) = (TextBlock){};
    
	text_block_head->next->previous = text_block_head;
    
	text_block_head->next->start = text;
	text_block_head->next->end = text + text_size -1;
    
	for (;new_generic->next;new_generic= new_generic->next)
	{
		bool no_block_found = true;
		
		TextBlock * previous_block = 0;
		TextBlock * next_block = 0;
        
		for (TextBlock * current_text_block = text_block_head->next ; current_text_block ; current_text_block= current_text_block->next)
		{
			if (current_text_block->is_insert_text) continue;
            
			if (new_generic->last_seen_struct_position == current_text_block->start)
			{
				no_block_found = false;
			}
			else if(new_generic->last_seen_struct_position > current_text_block->start)
			{
                
				if (new_generic->last_seen_struct_position < current_text_block->end)
				{
					no_block_found = false;
                    
					TextBlock * first_half_block = (TextBlock*)malloc(sizeof(TextBlock));
                    
					(*first_half_block) = (TextBlock){};
                    
					first_half_block->start = current_text_block->start;
					first_half_block->end = new_generic->last_seen_struct_position - 1;
                    
					first_half_block->next = current_text_block;
					first_half_block->previous = current_text_block->previous;
					first_half_block->previous->next = first_half_block;
                    
					current_text_block->start = new_generic->last_seen_struct_position;
					current_text_block->previous = first_half_block;
                    
				}
			}
            
			if (!no_block_found)
			{
                
				previous_block = current_text_block->previous;
				next_block = current_text_block;
				break;
                
			}
		}
        
		if (no_block_found) CATCH;
        
		TextBlock * insert_text_block = (TextBlock*)malloc(sizeof(TextBlock));
		(*insert_text_block) = (TextBlock){};
        
		insert_text_block->is_insert_text = true;
        
		insert_text_block->start = (char*)malloc(1024);
		memset(insert_text_block->start , '\0', 1024);
		
		insert_text_block->end = insert_text_block->start + 1024 - 1;
        
		insert_text_block->next = next_block;
		next_block->previous = insert_text_block;
        
		insert_text_block->previous = previous_block;
		previous_block->next = insert_text_block;
        
		char * current_buffer = insert_text_block->start;
        
		generate_generic_type(current_buffer , new_generic);
	}
    
	char * current_inserted_text = (char*)malloc(1024 * 256);
	char * inserted_text = current_inserted_text;
    
	for (TextBlock * current_text_block = text_block_head->next; current_text_block; current_text_block = current_text_block->next)
	{
		if (current_text_block->is_insert_text)
		{			
			current_inserted_text += sprintf(current_inserted_text, current_text_block->start);
            
		}
		else
		{
			current_inserted_text += sprintf(current_inserted_text , "%.*s" ,  (current_text_block->end - current_text_block->start) + 1 , current_text_block->start);
		}
        
	}
    
	current_inserted_text += sprintf(current_inserted_text , "\0");
    
	fprintf(output_file , inserted_text);
    
	(*tail) = current_generic;
    
	return inserted_text;
}

static TypeData* generate_type_data(TokenHeaderList * token_header_list)
{
    
    unknow_type_data_header = (TypeData*)malloc(sizeof(TypeData));
    (*unknow_type_data_header) = (TypeData){};
    
    TypeData* current_unknow_type = unknow_type_data_header;
	
	TypeData* type_data_header = (TypeData*)malloc(sizeof(TypeData));
	(*type_data_header) = (TypeData){};
    
	current_type_data = type_data_header;
    
	add_type_data("float");
	add_type_data("double");
	add_type_data("bool");
    
	add_type_data("int");
	add_type_data("short");
	add_type_data("long");
	add_type_data("long long");
	add_type_data("char");
    add_type_data("wchar_t");
    
	add_type_data("unsigned int");
	add_type_data("unsigned short");
	add_type_data("unsigned long long");
	add_type_data("unsigned long");
	add_type_data("unsigned char");
    
	add_type_data("void");
    add_type_data("HDC");
    
	for (; token_header_list->next_header; token_header_list = token_header_list->next_header)
	{
        
		Token* previous_token = 0;
		for (current_token = token_header_list->token_header; current_token->next_token; current_token = current_token->next_token)
		{
            
			if (current_token->type == T_identifier)
			{
                
                Token * typedef_token = 0;
                bool undefine_struct = false;
                
				bool is_type = false;
				bool is_enum = false;
                bool is_struct = false;
                bool is_typedef = false;
                
				if (match_identifier(current_token,"struct"))
				{
					if (current_token->next_token->next_token->type == T_bracket_left)
					{
                        current_token = current_token->next_token;
                        is_struct = true;
						is_type = true;
					}
                    else
                    {
                        current_token = current_token->next_token;
                        undefine_struct = true;
                    }
				}
                
				if (match_identifier(current_token, "enum"))
				{
                    current_token = current_token->next_token;
					is_type = true;
                    is_enum = true;
				}
                
                if(match_identifier(current_token , "typedef"))
                {
                    Token * first_token = current_token->next_token;
                    
                    typedef_token = current_token;
                    Token * typedef_name_token = 0;
                    Token * typedef_end_token =current_token;
                    
                    for(;typedef_end_token;typedef_end_token = typedef_end_token->next_token)
                    {
                        if(typedef_end_token->type== T_semicolon) break;
                        
                        if(typedef_end_token->type == T_identifier) 
                        {
                            typedef_name_token =typedef_end_token;
                        }
                        else
                        {
                            undefine_struct = true;
                        }
                    }
                    
                    if(!typedef_name_token) CATCH;
                    
                    current_token = typedef_name_token; 
                    
                    //loose check(function pointer , macro)
                    if(!match_identifier(first_token , "struct"))
                    {
                        if(!match_identifier(first_token , "enum"))
                        {
                            if(!match_identifier(first_token , "union"))
                            {
                                is_type = true;
                                is_typedef = true;
                            }
                        }
                    }
                    
                    if(undefine_struct) is_type = false;
                }
                
				if (is_type)
				{
					current_type_data->name_token = (*current_token);
					
                    current_type_data->is_struct = is_struct;;
                    current_type_data->is_typedef =is_typedef;
                    current_type_data->is_enum = is_enum;
                    
                    if(typedef_token) current_type_data->typedef_token = (*typedef_token);
                    
					current_type_data->next_data = (TypeData*)malloc(sizeof(TypeData));
					current_type_data = current_type_data->next_data;
					(*current_type_data) = (TypeData){};
				}
                else if(undefine_struct)
                {
                    current_unknow_type->name_token = (*current_token);
                    
					current_unknow_type->next_data = (TypeData*)malloc(sizeof(TypeData));
					current_unknow_type = current_unknow_type->next_data;
					(*current_unknow_type) = (TypeData){};
                }
			}
            
			previous_token = current_token;
		}
	}
    
    for(current_unknow_type = unknow_type_data_header ; current_unknow_type->next_data ; current_unknow_type = current_unknow_type->next_data)
    {
        bool not_define_at_all = true;
        for(TypeData * defined_type = type_data_header ; defined_type->next_data ; defined_type = defined_type->next_data)
        {
            if(!strncmp(current_unknow_type->name_token.string , defined_type->name_token.string , current_unknow_type->name_token.string_length))
            {
                not_define_at_all = false;
                break;
            }
        }
        
        if(not_define_at_all)
        {
            current_type_data->name_token = current_unknow_type->name_token;
            
            current_type_data->is_struct = true;;
            current_type_data->is_typedef =false;
            current_type_data->is_enum = false;
            
            current_type_data->typedef_token = (Token){};
            
            current_type_data->not_defined = true;
            
            current_type_data->next_data = (TypeData*)malloc(sizeof(TypeData));
            current_type_data = current_type_data->next_data;
            (*current_type_data) = (TypeData){};
        }
    }
    
	fprintf(meta_data_file, "typedef enum _type_meta _type_meta;\nenum _type_meta\n{\n");
    
	for (TypeData* type_data_pointer = type_data_header; type_data_pointer->next_data; type_data_pointer = type_data_pointer->next_data)
	{
		fprintf(meta_data_file, "\t_MT_");
        
		for (int char_index = 0; char_index < type_data_pointer->name_token.string_length; char_index++)
		{
			char current_char = type_data_pointer->name_token.string[char_index];
			if (current_char == ' ')
				current_char = '_';
            
			fputc(current_char, meta_data_file);
		}
        
		fprintf(meta_data_file, ",\n");
        
		printf("Type : %.*s\n", type_data_pointer->name_token.string_length, type_data_pointer->name_token.string);
	}
    fprintf(meta_data_file, "\t_MT_type_count\n");
    
	fprintf(meta_data_file, "};\n\n");
    
    fprintf(meta_data_file, "const int _type_meta_size[] = \n{\n");
    
    for (TypeData* type_data_pointer = type_data_header; type_data_pointer->next_data;  type_data_pointer = type_data_pointer->next_data)
    {
        bool size_not_sure = false;
        
        if (find_string(type_data_pointer->name_token.string, "void"))
        {
            size_not_sure = true;
        }
        else if(type_data_pointer->not_defined)
        {
            size_not_sure = true;
        }
        
        if(size_not_sure)
        {
            fprintf(meta_data_file, "0,");
        }
        else
        {
            fprintf(meta_data_file, "sizeof(");
            
            if(type_data_pointer->is_typedef)
            {
                for(Token * original_type = type_data_pointer->typedef_token.next_token ; original_type->string != type_data_pointer->name_token.string ; original_type = original_type->next_token)
                {
                    bool is_original_type = false;
                    if(original_type->type == T_identifier) is_original_type = true;
                    if(original_type->type == T_asterisk) is_original_type = true;
                    
                    if(is_original_type) fprintf(meta_data_file, "%.*s",original_type->string_length, original_type->string);
                }
            }
            else
            {
                for (int char_index = 0; char_index < type_data_pointer->name_token.string_length; char_index++)
                {
                    char current_char = type_data_pointer->name_token.string[char_index];
                    
                    fputc(current_char, meta_data_file);
                }
            }
            
            fprintf(meta_data_file, "),");
        }
        
        fprintf(meta_data_file , "\/\/%.*s \n",type_data_pointer->name_token.string_length, type_data_pointer->name_token.string);
        
    }
    
    fprintf(meta_data_file, "};\n\n");
    
	fprintf(meta_data_file, "const char * _type_meta_name[] = \n{\n");
    
	for (TypeData* type_data_pointer = type_data_header; type_data_pointer->next_data; type_data_pointer = type_data_pointer->next_data)
	{
		fprintf(meta_data_file, "\t\"");
        
		for (int char_index = 0; char_index < type_data_pointer->name_token.string_length; char_index++)
		{
			char current_char = type_data_pointer->name_token.string[char_index];
            
			fputc(current_char, meta_data_file);
		}
        
		fprintf(meta_data_file, "\",\n");
	}
    
	fprintf(meta_data_file, "};\n\n");
    
    
    fprintf(meta_data_file, "bool _is_type_enum[] = \n{\n");
    
	for (TypeData* type_data_pointer = type_data_header; type_data_pointer->next_data; type_data_pointer = type_data_pointer->next_data)
	{
        fprintf(meta_data_file,type_data_pointer->is_enum ? "true,\n" : "false,\n");
    }
    
	fprintf(meta_data_file, "};\n\n");
    
	return type_data_header;
}

static void print_name(char *member_name,int length)
{
    
	for (int current_char_index = 0; current_char_index < length; current_char_index++)
	{
		char current_char = member_name[current_char_index];
        
		if (current_char == ' ')
		{
			current_char = '_';
		}
        
		fputc(current_char, meta_data_file);
	}
}

static void generate_struct_member_meta(IntrospectionData* current_intropection_data)
{
	fprintf(meta_data_file, "global int member_meta_count_%.*s = %d;\n", current_intropection_data->struct_name_length, current_intropection_data->struct_name, current_intropection_data->member_count);
    
	fprintf(meta_data_file, "global const MemberMetaData member_meta_%.*s[%d] =\n{\n", current_intropection_data->struct_name_length, current_intropection_data->struct_name, current_intropection_data->member_count);
    
	for (StructMemberData* current_member = current_intropection_data->struct_member_header; current_member->next_data; current_member = current_member->next_data)
	{
        
		fprintf(meta_data_file, "\t{ \"%.*s\" , ", current_member->member_name_token->string_length, current_member->member_name_token->string);
        
		fprintf(meta_data_file, current_member->is_multiple_pointer ?"true,":"false,");
        
		fprintf(meta_data_file, current_member->is_pointer ?"true,":"false,");
        
		fprintf(meta_data_file, current_member->first_left_bracket_index != -1?"true,":"false,");
        
        if(current_member->first_left_bracket_index != -1)
        {
            bool is_array_size = false;
            bool ended = false;
            
            for(int token_index = current_member->first_left_bracket_index ;token_index < current_member->token_chain_length ; token_index++)
            {
                Token * current_member_token = current_member->token_chain[token_index];
                
                if(current_member_token->type == T_square_bracket_right)
                {
                    ended = true;
                    is_array_size = false;
                }
                
                if(is_array_size)
                {
                    fprintf(meta_data_file, "%.*s", current_member_token->string_length, current_member_token->string);
                    
                }
                
                if(current_member_token->type == T_square_bracket_left)
                {
                    is_array_size = true;
                    
                    if(ended)
                    {
                        
                        fprintf(meta_data_file, " * ");
                        ended = false;
                    }
                }
            }
        }
        else
        {
            fprintf(meta_data_file,"0");
        }
        
        fprintf(meta_data_file,",");
        
		fprintf(meta_data_file, "_MT_");
        
		bool the_first_token = true;
        
		the_first_token = true;
		for (int type_token_index = 0 ; type_token_index < current_member->member_type_token_array_count ; type_token_index++ )
		{
			
			Token * current_type_token = current_member->token_chain[type_token_index];
            
			if (current_type_token->type != T_identifier) continue;
			if (find_string(current_type_token->string , "const")) continue;
			if (find_string(current_type_token->string , "persist")) continue;
            
			if (the_first_token)
			{
				the_first_token = false;
			}
			else
			{
				fprintf(meta_data_file , "_");
			}
            
			print_name(current_type_token->string,current_type_token->string_length);			
		}
		
		fputc(',', meta_data_file);
		fputc('\"', meta_data_file);
		
		the_first_token = true;		
		for (int type_token_index = 0 ; type_token_index < current_member->member_type_token_array_count ; type_token_index++ )
		{
			Token * current_type_token = current_member->token_chain[type_token_index];
            
			if (current_type_token->type != T_identifier) continue;
			if (find_string(current_type_token->string , "const")) continue;
			if (find_string(current_type_token->string , "persist")) continue;
            
			if (the_first_token)
			{
				the_first_token = false;
			}
			else
			{
				fprintf(meta_data_file , "_");
			}
            
			print_name(current_type_token->string,current_type_token->string_length);			
		}
		
		fputc('\"', meta_data_file);
		fputc(',', meta_data_file);
        
		fprintf(meta_data_file, "(int)&((%.*s *)", current_intropection_data->struct_name_length, current_intropection_data->struct_name);
        
		fprintf(meta_data_file, "0)->%.*s ,", current_member->member_name_token->string_length, current_member->member_name_token->string);
        
#if 1
		if(current_member->is_pointer)
        {
            fprintf(meta_data_file, "sizeof(void *),");
            
        }
        else
        {
            
            fprintf(meta_data_file, "sizeof(");
            
            the_first_token = true;
            for (int type_token_index = 0 ; type_token_index < current_member->member_type_token_array_count ; type_token_index++ )
            {
                Token * current_type_token = current_member->token_chain[type_token_index];
                
                if (current_type_token->type != T_identifier) continue;
                if (find_string(current_type_token->string , "const")) continue;
                if (find_string(current_type_token->string , "persist")) continue;
                
                if (the_first_token)
                {
                    the_first_token = false;
                }
                else
                {
                    fprintf(meta_data_file , " ");
                }
                
                fprintf(meta_data_file, "%.*s", current_type_token->string_length, current_type_token->string);
                
            }
            
            fprintf(meta_data_file, "),");
        }
        
#endif
        //fprintf(meta_data_file, "0,");
        
        
		fprintf(meta_data_file, "}, \n");
	}
    
	fprintf(meta_data_file, "};\n\n");
}

static void generate_enum_string(Token * name_token)
{	
    
	fprintf(meta_data_file , "global const char * %.*s_String[] = \n{\n" , name_token->string_length , name_token->string);
    
    bool member_found = true;
    
	for (Token * member_token = name_token->next_token->next_token;; member_token = member_token->next_token)
	{
        
		if (member_token->type != T_identifier)
		{
            if(member_token->type == T_equal_sign)
            {
                member_found = false;
                continue;
            }
            
			if(member_token->type == T_comma) 
            {
                member_found = true;
                continue;
            }
            
            if(!member_found) continue;
            
            if (member_token->type == T_bracket_right) break;
            
			CATCH;
		}
        
        if(member_found)
        {
            fprintf(meta_data_file, " \"%.*s\",\n" , member_token->string_length , member_token->string);
        }
		
		if (member_token->next_token->type == T_bracket_right) break;
	}
    
	fprintf(meta_data_file , "};\n\n");
}

static void print_all_function_declaration_or_definition(FunctionData * function_data_head, char * text , long text_size , const char * output_name , bool declaration)
{
	
	for (FunctionData * current_function_data = function_data_head; current_function_data->next; current_function_data = current_function_data->next)
	{
		if (!current_function_data->start_token) continue;
        
		if (current_function_data->local_function_list)
		{
			print_all_function_declaration_or_definition(current_function_data->local_function_list, text , text_size ,output_name, declaration);
		}
        
		Token * name_token = current_function_data->start_token;
		Token * right_bracket_token = current_function_data->right_bracket_token;
		Token * left_bracket_token = current_function_data->left_bracket_token;
		Token * right_parentheses_token = current_function_data->right_parentheses_token;
        
		bool print_body = false;
        
		if (!declaration)
		{
			if (current_function_data->is_local)
			{
				print_body = true;
			}
		}
        
		if(print_body)
		{
			int string_length = right_bracket_token->string - name_token->string + 1;
            
			fprintf(meta_data_file, "%.*s" ,string_length, name_token->string);
			fprintf(meta_data_file, "\n");
            
#if 1
			if (output_name)
			{
				for (int char_index = 0 ; char_index < string_length ; char_index++)
				{
					if (name_token->string[char_index] == '\n') continue;
					if (name_token->string[char_index] == '\r') continue;
                    
					name_token->string[char_index] = ' ';
				}
			}
#endif
            
		}
		else if(declaration)
		{
			int name_length = right_parentheses_token->string - name_token->string + right_parentheses_token->string_length;
			if (*(name_token->string + name_length - 1) == '\n') name_length--;
            
			fprintf(meta_data_file, "%.*s" ,name_length , name_token->string);
			fprintf(meta_data_file, ";\n");
		}
		
	}    
}

static char * insert_line(char * text , long text_size , const char * output_name , const char * source_file_name)
{
	int line_index = 2;
    
	char * sratch_buffer = (char *)malloc(1024 * 512);
	memset(sratch_buffer , '\0' , 1024 * 256);
	char * sratch_buffer_head = sratch_buffer;
    
	for (int text_index = 0; text_index < text_size ;text_index++)
	{
		char character = text[text_index];
        
		(*sratch_buffer) = character;
		sratch_buffer++;
        
#if 1
		if (character == '\n')
		{
            
			sratch_buffer += sprintf(sratch_buffer , "#line %d \"%s\"\n", line_index , source_file_name);
			line_index++;
		}
        
#endif
	}
    
	return sratch_buffer_head;
    
}


static FunctionData * find_function_and_move_local_function(Token * current_token, Token ** tail_token_result , bool nested)
{
    
	int nested_level = 0;
	bool entered_function = false;
	bool found_function = false;
    
	FunctionData * current_function_data = (FunctionData *)malloc(sizeof(FunctionData));
	FunctionData * function_data_head = current_function_data;
    
	(*current_function_data) = (FunctionData){};
    
	for ( ; current_token->next_token; current_token = current_token->next_token)
	{
        
		if (find_string_exact(current_token->string , "internal" , strlen("internal")))
		{
			if (entered_function)
			{
				Token * token_result = 0;
                
				for (; find_string_exact(current_token->string , "internal" , strlen("internal"));)
				{
					
					FunctionData * local_function_data = find_function_and_move_local_function(current_token , &token_result , true);
					current_token = token_result;
                    
					FunctionData * local_function_data_tail = local_function_data;
					for( ; local_function_data_tail->next ; local_function_data_tail=local_function_data_tail->next);
                    
					if (!current_function_data->local_function_list)
					{
						current_function_data->local_function_list = local_function_data;
					}
					else
					{	
						current_function_data->current_local_function_data->next = local_function_data;
					}
					
					current_function_data->current_local_function_data = local_function_data_tail;
				}
                
			}
			else
			{
				current_function_data->start_token = current_token;
				found_function = true;
                
			}
            
		}
        
		if (found_function)
		{
			if(current_token->type == T_parentheses_right)
			{
                
				if (!current_function_data->right_parentheses_token)
				{
					current_function_data->right_parentheses_token = current_token;
				}
                
			}
            
		}
        
		if (current_token->type == T_bracket_left)
		{
			if (found_function)
			{
				if (!entered_function)
				{
					current_function_data->left_bracket_token = current_token;
					entered_function = true;
                    
				}
                
			}
            
			nested_level++;
		}
        
		if (current_token->type == T_bracket_right)
		{
			nested_level--;
		}
        
		if (nested_level == 0)
		{
			if (entered_function)
			{
				current_function_data->right_bracket_token = current_token;
				current_function_data->is_local = nested;
                
				current_function_data->next = (FunctionData * )malloc(sizeof(FunctionData));
				current_function_data = current_function_data->next;
				(*current_function_data) = (FunctionData){};
                
				if (nested)
				{
					(*tail_token_result) = current_token->next_token;
					break;
				}
                
				entered_function = false;
				found_function = false;
                
			}
            
		}
        
	}
    
	return function_data_head;
}

static void generate_meta_data(TokenHeaderList* token_list,TypeData * type_data_header)
{
	IntrospectionData* introspection_header = (IntrospectionData*)malloc(sizeof(IntrospectionData));
	IntrospectionData* current_intropection_data = introspection_header;
	*current_intropection_data = (IntrospectionData){};
    
	for (TokenHeaderList* current_token_list = token_list; current_token_list->next_header; current_token_list = current_token_list->next_header)
	{
        
		Token* token_header = current_token_list->token_header;
        
		for (current_token = token_header; current_token->next_token; current_token = current_token->next_token)
		{
			if (find_string(current_token->string , "enum"))
			{
                if(current_token->next_token->next_token->type == T_bracket_left)
                {
                    
                    generate_enum_string(current_token->next_token);
                    
                }
                
			}
		}
	}
	
	int token_total_count = 0;
	for (TokenHeaderList* current_token_list = token_list; current_token_list->next_header; current_token_list = current_token_list->next_header)
	{
		Token* token_header = current_token_list->token_header;
		for (current_token = token_header; current_token->next_token; current_token = current_token->next_token)
		{
			printf("%.*s \n" ,current_token->string_length , current_token->string);
			token_total_count++;
		}
	}
    
	for (TokenHeaderList* current_token_list = token_list; current_token_list->next_header; current_token_list = current_token_list->next_header)
	{
		Token* token_header = current_token_list->token_header;
        
		for (current_token = token_header; current_token->next_token; current_token = current_token->next_token)
		{
#if 0
			for (int CharIndex = 0; CharIndex < current_token->string_length; CharIndex++)
			{
				putchar(current_token->string[CharIndex]);
			}
			putchar('\n');
#endif  
            
			if(match_identifier(current_token,"typedef"))
            {
                if(match_identifier(current_token->next_token,"struct"))
                {
                    current_token = current_token->next_token->next_token;
                    continue;
                }
            }
            
            if (current_token->type != T_identifier) continue;
			
            if (!match_identifier(current_token ,"struct")) continue;
            
            *current_intropection_data = generate_introspect_data(current_token);
            
            if (current_intropection_data->is_valid)
            {
                current_intropection_data->next = (IntrospectionData*)malloc(sizeof(IntrospectionData));
                current_intropection_data = current_intropection_data->next;
            }
            
            (*current_intropection_data) = (IntrospectionData){};
			
		}
	}
    
	fprintf(meta_data_file, "typedef enum introspected_struct introspected_struct;\nenum introspected_struct\n{\n");
    
	for (IntrospectionData* current_introspection_data = introspection_header; current_introspection_data->next; current_introspection_data = current_introspection_data->next)
	{
		if (current_introspection_data->is_valid)
		{
			fprintf(meta_data_file, "\tIS_%.*s, \n", current_introspection_data->struct_name_length, current_introspection_data->struct_name);
		}
	}
    
	fprintf(meta_data_file, "};\n\n");
    
	for (IntrospectionData* current_introspection_data = introspection_header; current_introspection_data->next; current_introspection_data = current_introspection_data->next)
	{
		if (current_introspection_data->is_valid)
		{
			generate_struct_member_meta(current_introspection_data);
		}
	}
    
	fprintf(meta_data_file, "internal StructMetaData * get_all_type_member_info()\n{\nStructMetaData * all_struct = malloc(sizeof(StructMetaData) * _MT_type_count);\n");
    
	for (TypeData* current_data = type_data_header; current_data->next_data; current_data = current_data->next_data)
	{
        bool have_members = false;
		if (current_data->is_struct)
		{
            if(!current_data->not_defined)
            {
                have_members = true;
            }
        }
        
        fprintf(meta_data_file, "\tall_struct[_MT_");
        
        print_name(current_data->name_token.string,current_data->name_token.string_length);
        
        fprintf(meta_data_file, "] = ");
        
        if(have_members)
        {
            fprintf(meta_data_file, "(StructMetaData)GetStructMeta(%.*s);\n", current_data->name_token.string_length , current_data->name_token.string );
        }
		else
		{
			fprintf(meta_data_file, "(StructMetaData){};\n");
		}
	}
    
	fprintf(meta_data_file, "\n\nreturn all_struct;\n}\n");
    
	fprintf(meta_data_file, "\n");
}

extern int main()
{
    
	//weir stuff
	//if i read in text it read /r/n to /n which make the whole file smaller
	//so i need to use rb?
    
	//TODO: these crap just written in 30 mins
	//make it nicer
    
	typedef struct FunctionFile FunctionFile;
	struct FunctionFile
	{
		const char* name;
		const char* output_name;
	};
    
	struct
	{
		const char* output_meta_data_name;
		const char* data_file_to_scan[128];
        
		FunctionFile function_file[128];
	} output_files[] =
	{
		{ 
			"../Code/d_main_meta_generated.c", 
			{
				"../Code/d_header.h",
				"../Code/d_main.h",
				0
			},
			0
		},
        
		{ 
			"../Code/d_game_meta_generated.c",
			{
				"../Code/d_header.h",
				"../Code/d_main.h",
				"../Code/d_renderdata.c",
				"../Code/d_gamedata.c",
                0
			},
			{
				{ "../Code/d_function.c"  , 0  } ,
				{ "../Code/d_render.c" , 0  },
				{ "../Code/d_text.c" , 0  },
				{ "../Code/d_gamefunction.c" , 0  },
				//{ "../Code/d_blender_file.h" , 0} ,
                { "../Code/d_game.c" , 0} ,
				
				{ 0 , 0},
			}
		},
        
		{0,0,0}
	};
    
	for (int output_file_index = 0; output_files[output_file_index].output_meta_data_name; output_file_index++)
	{
		meta_data_file = fopen(output_files[output_file_index].output_meta_data_name, "w");
        
		TokenHeaderList * token_header_list = (TokenHeaderList*)malloc(sizeof(token_header_list));
        
		TokenHeaderList * current_tokenHeader = token_header_list;
		(*current_tokenHeader) = (TokenHeaderList){};
        
		GenericData * current_generic = (GenericData *)malloc(sizeof(GenericData));
		(*current_generic) = (GenericData){};
		GenericData * generic_head = current_generic;
        
		for (const char** current_scanning_file= output_files[output_file_index].data_file_to_scan; (*current_scanning_file); current_scanning_file++)
		{
            
			const char * input_file = (*current_scanning_file);
			FILE* code_file = fopen(input_file , "rb");
            
			char* current_text = 0;
			long current_text_size = 0;
			current_tokenHeader->token_header = convert_file_text_to_token(code_file , &current_text , &current_text_size);
            
			current_tokenHeader->next_header = (TokenHeaderList*)malloc(sizeof(token_header_list));
			current_tokenHeader = current_tokenHeader->next_header;
			(*current_tokenHeader) = (TokenHeaderList){};
            
		}
        
		current_tokenHeader = token_header_list;
		
		TypeData * type_data_header = generate_type_data(current_tokenHeader);
        
		generate_meta_data(current_tokenHeader, type_data_header);
        
		current_tokenHeader = current_tokenHeader->next_header;
        
		for (FunctionFile * current_scanning_file = output_files[output_file_index].function_file; current_scanning_file->name; current_scanning_file++)
		{
			const char * input_file = current_scanning_file->name;
			FILE* code_file = fopen(input_file , "rb");
            
			char* current_text = 0;
			long current_text_size = 0;
            
			Token * token_head = convert_file_text_to_token(code_file,&current_text , &current_text_size  );			
			if (current_scanning_file->output_name)
			{
				current_text = insert_line(current_text , current_text_size,current_scanning_file->output_name ,input_file );
				token_head = convert_text_to_token(current_text , 1024*256);
			}
            
			FunctionData * function_data_head = find_function_and_move_local_function(token_head,0, false);
			print_all_function_declaration_or_definition(function_data_head ,current_text , current_text_size,current_scanning_file->output_name , true);
			print_all_function_declaration_or_definition(function_data_head ,current_text , current_text_size,current_scanning_file->output_name , false);
            
			if (current_scanning_file->output_name)
			{
                
				FILE * modified_file = fopen(current_scanning_file->output_name , "w");
				//fprintf(ModifiedFile , CurrentText );
				
				fputs(current_text , modified_file);
                
				fclose(modified_file);
                
			}
            
		}
        
		fclose(meta_data_file);
	}
	
	return 0;
}