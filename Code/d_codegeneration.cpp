#include <wchar.h> //for chinese characters
#include <stdio.h> //just file io and printing stuff 
#include <stdlib.h>
#include <string.h>

#define ASSERT *(int *)0=1

//i should have just use lambda
#define Local(ReturnType , Name ,VarArgsAndFunction ) \
struct _##Name##_DummyStruct { static ReturnType _Struct_##Name VarArgsAndFunction  }; \
auto Name = _##Name##_DummyStruct##::##_Struct_##Name

enum TokenType
{
	T_Identifier,
	T_Colon,
	T_Semicolon,
	T_BracketLeft,
	T_BracketRight,
	T_EqualSign,
	T_Asterisk,
	T_Comma,
	T_Minus,
	T_Divide,
	T_Plus,
	T_Numeric,
	T_StopSign,
	T_SquareBracketLeft,
	T_SquareBracketRight,
	T_PoundSign,
	T_QuotationMark,
	T_Apostrophe,
	T_ParenthesesLeft,
	T_ParenthesesRight,
	T_LessThan,
	T_GreaterThan,
	T_ExcalmationMark,
	T_BackSlash,
	T_Slash,
	T_Ampersand,
	T_QuestionMark,
	T_VerticalBar,
	T_PercentSign,
	T_Hyphen,
	T_WaveSign,
};

struct Token
{
	TokenType Type;
	char* String;
	short StringLength;
	Token* NextToken;
};

struct _TokenHeaderList
{
	Token* TokenHeader;
	_TokenHeaderList* NextHeader;
};

struct FunctionData
{
	bool Local;
    
	Token * StartToken;
    
	Token * RightParenthesesToken;
	Token * LeftBracketToken;
    
	Token * RightBracketToken;
    
	FunctionData * LocalFunctionList;
	FunctionData * CurrentLocalFunctionData;
    
	FunctionData * Next;
};

struct TypeData
{
    Token NameToken;
    
	//no fucking clue what you are
    Token TypedefToken;
    
    bool IsStruct;
    bool IsTypedef;
	bool IsEnum;
    
    bool NotDefined;
    
    TypeData* NextData;
};

static TypeData* UnknowTypeDataHeader = 0 ;

struct StructMemberData
{
#define MAX_TOKEN_CHAIN 32
	Token *TokenChain[MAX_TOKEN_CHAIN];
    
	int TokenChainLength;
    
	Token* MemberNameToken;
	int MemberTypeTokenArrayCount;
    
	bool IsPointer;
	bool IsMultiplePointer;
	int FirstLeftBracketIndex;
	int EquadSignIndex;
    
	StructMemberData* NextData;
};

struct GenericData
{
	Token * TypeToken;
	Token * GenericToken;
    
	char * LastSeenStructPosition;
    
	GenericData * Next;
};

static Token* CurrentToken = 0;
static char* TextString =0;
static FILE* MetaDataFile=0;
static TypeData* CurrentTypeData =0;
static StructMemberData* CurrentStructMemberData = 0;

static bool FindString(char * SourceString,const char * TargetString)
{
	return (!strncmp(SourceString, TargetString, strlen(TargetString)));
}

static bool FindStringExact(char * SourceString, const char * TargetString , int Count)
{
	return (!strncmp(SourceString, TargetString, Count ));
}

static void ToNextToken()
{
	CurrentToken->NextToken = (Token*)malloc(sizeof(Token));
	CurrentToken = CurrentToken->NextToken;
	(*CurrentToken) = {};
}

static void ToNextToken(TokenType Type)
{
	CurrentToken->Type = Type;
	CurrentToken->String = TextString;
	CurrentToken->StringLength = 1;
    
	ToNextToken();
}

static void GoToNextLine()
{
	for (; (*TextString) != '\n'; TextString++);
}

static bool IsTheStartOfIdentifier(char Char)
{
	if (Char >= 'A' && Char <= 'Z')
	{
		return true;
	}
	else if (Char >= 'a' && Char <= 'z')
	{
		return true;
	}
	else if (Char == '_')
	{
		return true;
	}
    
	return false;
}

static bool IsPartOfTheIdentifier(char Char)
{
	if (Char >= 'A' && Char <= 'Z')
	{
		return true;
	}
	else if (Char >= 'a' && Char <= 'z')
	{
		return true;
	}
	else if (Char >= '0' && Char <= '9')
	{
		return true;
	}
	else if (Char == '_')
	{
		return true;
	}
    
	return false;
}

static void AddTypeData(const char* String )
{
    CurrentTypeData->NameToken.String = (char*)String;
    CurrentTypeData->NameToken.StringLength = strlen(String);
	
    CurrentTypeData->NextData = (TypeData*)malloc(sizeof(TypeData));
	CurrentTypeData = CurrentTypeData->NextData;
	(*CurrentTypeData) = {};
}

static char* CheckPreviosToken(char * RewindString, const char * TargetString)
{
	char* Result = RewindString;
    
	int TargetLength = strlen(TargetString);
    
	RewindString--;
	for (; (*RewindString) == ' '; RewindString--);
    
	if (IsPartOfTheIdentifier(*RewindString))
	{
		for (; IsPartOfTheIdentifier(*RewindString); RewindString--);
		RewindString++;
	}
    
	if (!strncmp(TargetString, RewindString, TargetLength))
	{
		Result = RewindString;
	}
    
	return Result;
}

struct _IntrospectionData
{
	bool IsValid;
	char* StructName;
	int StructNameLength;
	int MemberCount;
	StructMemberData* StructMemberHeader;
    
	_IntrospectionData* Next;
};

static _IntrospectionData GenerateIntrospectData(Token* CurrentStructToken)
{
	_IntrospectionData Result = {};
	Result.IsValid = false;
    
	StructMemberData* CurrentStructMemberData = (StructMemberData*)malloc(sizeof(StructMemberData));
	(*CurrentStructMemberData) = {};
    
	Result.StructMemberHeader = CurrentStructMemberData;
    
	CurrentStructToken = CurrentStructToken->NextToken;
    
	if (CurrentStructToken->Type != T_Identifier) ASSERT;
    
	Result.StructName= CurrentStructToken->String;
	Result.StructNameLength = CurrentStructToken->StringLength;
    
	CurrentStructToken = CurrentStructToken->NextToken;
    
	if (CurrentStructToken->Type != T_BracketLeft)
    {
        if(CurrentStructToken->Type == T_Semicolon) 
        {
            Result.IsValid = false;
            return Result;
        }
        
        ASSERT;
    }
    
	CurrentStructToken = CurrentStructToken->NextToken;
    
	(*CurrentStructMemberData) = {};
	CurrentStructMemberData->FirstLeftBracketIndex = -1;
	CurrentStructMemberData->IsPointer = false;
	CurrentStructMemberData->IsMultiplePointer = false;
	CurrentStructMemberData->EquadSignIndex = -1;
	CurrentStructMemberData->MemberNameToken = 0;
	CurrentStructMemberData->TokenChainLength = 0;
    
	for (; ; CurrentStructToken = CurrentStructToken->NextToken)
	{
		if (CurrentStructToken->Type == T_Semicolon)
		{
			int NameTokenIndex = CurrentStructMemberData->TokenChainLength-1;
            
			if (CurrentStructMemberData->EquadSignIndex != -1)
			{
				NameTokenIndex = CurrentStructMemberData->EquadSignIndex - 1;
			}
            
			if (CurrentStructMemberData->FirstLeftBracketIndex != -1)
			{
				NameTokenIndex = CurrentStructMemberData->FirstLeftBracketIndex - 1;
			}
            
			CurrentStructMemberData->MemberNameToken = CurrentStructMemberData->TokenChain[NameTokenIndex];
            
			int TypeIndex = NameTokenIndex - 1;
            
			CurrentStructMemberData->MemberTypeTokenArrayCount = NameTokenIndex ;
            
			Result.MemberCount++;
            
			CurrentStructMemberData->NextData = (StructMemberData*)malloc(sizeof(StructMemberData));
			CurrentStructMemberData = CurrentStructMemberData->NextData;
            
			(*CurrentStructMemberData) = {};
            
			CurrentStructMemberData->IsMultiplePointer = false;
			CurrentStructMemberData->IsPointer = false;
			CurrentStructMemberData->EquadSignIndex = -1;
			CurrentStructMemberData->FirstLeftBracketIndex = -1;
			
			CurrentStructMemberData->MemberNameToken = 0;
			CurrentStructMemberData->TokenChainLength = 0;
			CurrentStructMemberData->NextData = 0;
		}
		else if (CurrentStructToken->Type == T_EqualSign)
		{
			CurrentStructMemberData->EquadSignIndex = CurrentStructMemberData->TokenChainLength;
		}
		else if (CurrentStructToken->Type == T_Asterisk)
		{
			if (CurrentStructMemberData->IsPointer)
			{
				CurrentStructMemberData->IsMultiplePointer = true;
			}
            
			CurrentStructMemberData->IsPointer = true;
		}
		else if (CurrentStructToken->Type == T_SquareBracketLeft)
		{
			if (CurrentStructMemberData->FirstLeftBracketIndex == -1)
			{
				CurrentStructMemberData->FirstLeftBracketIndex = CurrentStructMemberData->TokenChainLength;
			}
		}
		else if (CurrentStructToken->Type == T_BracketLeft)
		{
			int LeftBracketMeet = 1;
            
			CurrentStructToken = CurrentStructToken->NextToken;
            
			for (; LeftBracketMeet; CurrentStructToken = CurrentStructToken->NextToken)
			{
				if (CurrentStructToken->Type == T_BracketLeft)
					LeftBracketMeet++;
                
				if (CurrentStructToken->Type == T_BracketRight)
					LeftBracketMeet--;
			}
            
			(*CurrentStructMemberData) = {};
            
			CurrentStructMemberData->IsMultiplePointer = false;
			CurrentStructMemberData->IsPointer = false;
			CurrentStructMemberData->EquadSignIndex = -1;
			CurrentStructMemberData->FirstLeftBracketIndex = -1;
            
			CurrentStructMemberData->MemberNameToken = 0;
			CurrentStructMemberData->TokenChainLength = 0;
			CurrentStructMemberData->NextData = 0;
		}
		else if (CurrentStructToken->Type == T_ParenthesesLeft)
		{
			int LeftParenthnessMeet = 1;
            
			CurrentStructToken = CurrentStructToken->NextToken;
            
			for (; LeftParenthnessMeet; CurrentStructToken = CurrentStructToken->NextToken)
			{
				if (CurrentStructToken->Type == T_ParenthesesLeft)
					LeftParenthnessMeet++;
                
				if (CurrentStructToken->Type == T_ParenthesesRight)
					LeftParenthnessMeet--;
			}
            
			if (CurrentStructToken->Type == T_Semicolon)
				CurrentStructToken = CurrentStructToken->NextToken;
            
			(*CurrentStructMemberData) = {};
			CurrentStructMemberData->IsPointer= false;
			CurrentStructMemberData->IsMultiplePointer = false;
			CurrentStructMemberData->EquadSignIndex = -1;
			CurrentStructMemberData->FirstLeftBracketIndex = -1;
            
			CurrentStructMemberData->MemberNameToken = 0;
			CurrentStructMemberData->TokenChainLength = 0;
			CurrentStructMemberData->NextData = 0;
		}
        
		if (CurrentStructToken->Type == T_BracketRight)
		{
			break;
		}
        
		CurrentStructMemberData->TokenChain[CurrentStructMemberData->TokenChainLength++] = CurrentStructToken;
	}
    
	CurrentStructToken = CurrentStructToken->NextToken;
    
	Result.IsValid = true;
	
	return Result;
}

static Token* ConvertTextToToken(char * Text , int TextSize)
{
	Token* TokenHeader = (Token*)malloc(sizeof(Token));
	CurrentToken = TokenHeader;
	(*CurrentToken) = {};
    
	bool InsideString=false;
    
	for (TextString = Text; TextString < Text + TextSize; TextString++)
	{
		if ((*TextString) == '\0')
			break;
        
		switch (*TextString)
		{
            case '\0':
			break;
            case '/':
			if (TextString[1] == '/')
			{
				GoToNextLine();
			}
			else if (TextString[1] == '*')
			{
				for (; !((TextString[0] == '*') && (TextString[1] == '/')); TextString++);
			}
			else if (TextString[1] == '\0')
			{
				break;
			}
			else
			{
				ToNextToken(T_Slash);
			}
			break;
            
            case '#':
			//no idea how to handle this
			ToNextToken(T_PoundSign);
            
			for (; TextString[1] == ' '; TextString++);
            
			if (!strncmp(TextString+1, "define", 6))
			{
				for (; TextString[1] != '\n'; TextString++)
				{
					if ((TextString[1]) == '\\')
						TextString += 2;
				}
			}
            
			break;
            
            case ' ':
            case '\n':
            case '\r':
            case '\t':
			break;
            
            case ':': ToNextToken(T_Colon); break;
            case ';': ToNextToken(T_Semicolon); break;
            case '{': ToNextToken(T_BracketLeft); break;
            case '}': ToNextToken(T_BracketRight); break;
            case '+': ToNextToken(T_Plus); break;
            case '-': ToNextToken(T_Minus); break;
            case '=': ToNextToken(T_EqualSign); break;
            case '*': ToNextToken(T_Asterisk); break;
            case ',': ToNextToken(T_Comma); break;
            case '.': ToNextToken(T_StopSign); break;
            case '[': ToNextToken(T_SquareBracketLeft); break;
            case ']': ToNextToken(T_SquareBracketRight); break;
            case '(': ToNextToken(T_ParenthesesLeft); break;
            case ')': ToNextToken(T_ParenthesesRight); break;
            case '<': ToNextToken(T_LessThan); break;
            case '>': ToNextToken(T_GreaterThan); break;
            case '!': ToNextToken(T_ExcalmationMark); break;
            case '\'': ToNextToken(T_Apostrophe); break;
            case '\\': ToNextToken(T_BackSlash); break;
            case '&': ToNextToken(T_Ampersand); break;
            case '?': ToNextToken(T_QuestionMark); break;
			case '|': ToNextToken(T_VerticalBar); break;
			case '%':ToNextToken(T_PercentSign); break;
			case '^':ToNextToken(T_Hyphen); break;
			case '~':ToNextToken(T_WaveSign); break;
            
            case '\"':
            {
                if (*(TextString-1)!= '\\')
                {
                    InsideString = !InsideString;
                }
                
                ToNextToken(T_QuotationMark);
                break;
            }
            default:
			if (IsTheStartOfIdentifier(*TextString))
			{
				CurrentToken->Type = T_Identifier;
				CurrentToken->String = TextString;
				for (; IsPartOfTheIdentifier(TextString[1]); TextString++);
				CurrentToken->StringLength = TextString - CurrentToken->String + 1;
				ToNextToken();
			}
			else if ((*TextString) >= '0' && (*TextString) <= '9')
			{
				CurrentToken->Type = T_Numeric;
				CurrentToken->String = TextString;
				for (; (TextString[1]) >= '0' && (TextString[1]) <= '9'; TextString++);
				CurrentToken->StringLength = TextString - CurrentToken->String + 1;
				ToNextToken();
			}
			else if (InsideString)
			{
				//don't care what inside in the string
			}
			else
			{
				char CurrentInvalidChar = *TextString;
                
				int CurrentLength = TextString - Text;
				ASSERT;
			}
            
			break;
		}
	}
    
	return TokenHeader;
}

static Token* ConvertFileTextToToken(FILE* CodeFile , char ** OutText , long * OutTextSize)
{
    
	fseek(CodeFile, 0, SEEK_END);
	long FileSize = ftell(CodeFile);
    
	fseek(CodeFile, 0, SEEK_SET);
    
	char* Text = (char*)malloc((FileSize + 1) * sizeof(char));
	fread(Text, FileSize, 1, CodeFile);
	fclose(CodeFile);
    
	Text[FileSize] = '\0';
    
	(*OutText) = Text;
	(*OutTextSize) = FileSize;
    
	printf("%d\n" , FileSize);
    
	return ConvertTextToToken(Text, FileSize);
}

static bool IsGenericTypeToken(Token * Keyword)
{	
	if (FindStringExact(Keyword->String , "Array" , Keyword->StringLength)) return true;
	if (FindStringExact(Keyword->String , "HashTable" , Keyword->StringLength)) return true;
    
	return false;
}

static void modify_generic_type_token(Token * TokenHeader)
{
	for (CurrentToken = TokenHeader; CurrentToken->NextToken; CurrentToken = CurrentToken->NextToken)
	{
		Token * GenericTypeStartToken = CurrentToken->NextToken;
        
		if (!GenericTypeStartToken->NextToken) break;
        
		if (GenericTypeStartToken->Type != T_Identifier) continue;
        
		if (GenericTypeStartToken->NextToken->Type != T_ParenthesesLeft) continue;
		
		Token * TypeToken = GenericTypeStartToken->NextToken->NextToken;
        
		if (TypeToken->Type != T_Identifier) continue;
        
		if (!IsGenericTypeToken(GenericTypeStartToken)) continue;
        
		char * NewTokenString = (char *)malloc(sizeof(char) * 64);
		memset(NewTokenString , '\0', 64);
		strcat(NewTokenString , "D_");
		strncat(NewTokenString , GenericTypeStartToken->String , GenericTypeStartToken->StringLength);
		strcat(NewTokenString , "_");
        
		strncat(NewTokenString , TypeToken->String , TypeToken->StringLength);
        
		if (TypeToken->NextToken->Type != T_ParenthesesRight) ASSERT;
        
		Token * NewToken = (Token *)malloc(sizeof(Token));
		(*NewToken) = {};
        
		NewToken->String = NewTokenString;
		NewToken->StringLength = strlen(NewTokenString);
        
		CurrentToken->NextToken = NewToken;
		NewToken->NextToken = TypeToken->NextToken->NextToken;
	}
}

static void GenerateGenericType(char * CurrentBuffer , GenericData * NewGeneric)
{
    
	if (FindStringExact(NewGeneric->GenericToken->String, "Array" ,NewGeneric->GenericToken->StringLength))
	{
		CurrentBuffer += sprintf(CurrentBuffer, "struct D_Array_");
        
		CurrentBuffer += sprintf(CurrentBuffer, "%.*s" ,NewGeneric->TypeToken->StringLength , NewGeneric->TypeToken->String);
        
		CurrentBuffer += sprintf(CurrentBuffer, "\n{\n");
        
		CurrentBuffer += sprintf(CurrentBuffer, "\t%.*s" , NewGeneric->TypeToken->StringLength , NewGeneric->TypeToken->String);
		CurrentBuffer += sprintf(CurrentBuffer, " persist * DataArray;\n");
        
		CurrentBuffer += sprintf(CurrentBuffer, "\tint ArrayMaxSize;\n");
		CurrentBuffer += sprintf(CurrentBuffer, "};\n\n");
        
	}
	else if (FindStringExact(NewGeneric->GenericToken->String, "HashTable",NewGeneric->GenericToken->StringLength))
	{
        
		CurrentBuffer += sprintf(CurrentBuffer, "struct D_HashTable_");
        
		CurrentBuffer += sprintf(CurrentBuffer, "%.*s" , NewGeneric->TypeToken->StringLength , NewGeneric->TypeToken->String);
        
		CurrentBuffer += sprintf(CurrentBuffer, "\n{\n");
        
		CurrentBuffer += sprintf(CurrentBuffer, "\t%.*s" , NewGeneric->TypeToken->StringLength , NewGeneric->TypeToken->String);
		CurrentBuffer += sprintf(CurrentBuffer, " persist * DataArray;\n");
		
		CurrentBuffer += sprintf(CurrentBuffer, "\t%.*s" , NewGeneric->TypeToken->StringLength , NewGeneric->TypeToken->String);
		CurrentBuffer += sprintf(CurrentBuffer, " persist * DataTail;\n");
        
		CurrentBuffer += sprintf(CurrentBuffer, "\tHashTableSlot persist * HashSlotArray;\n");
		CurrentBuffer += sprintf(CurrentBuffer, "\tHashTableSlot persist * HashSlotTail;\n");
        
		CurrentBuffer += sprintf(CurrentBuffer, "};\n\n");
        
	}
	else
	{
		ASSERT;
	}
    
	CurrentBuffer += sprintf(CurrentBuffer, "\0");
    
}

static char * found_and_generate_generic_type(GenericData** Tail , GenericData* GenericListToCompare , char * Text , int TextSize, Token * TokenHeader , FILE * OutputFile)
{
	GenericData * CurrentGeneric = (*Tail);
	char * LastSeenStructPosition = 0;
	GenericData * NewGeneric = CurrentGeneric;
    
	for (CurrentToken = TokenHeader; CurrentToken->NextToken; CurrentToken = CurrentToken->NextToken)
	{
		if (CurrentToken->Type == T_Identifier)
		{
			if (FindString(CurrentToken->String , "struct"))
			{
				LastSeenStructPosition = CurrentToken->String;
			}
		}
        
		Token * GenericTypeStartToken = CurrentToken->NextToken;
        
		if (!GenericTypeStartToken->NextToken) break;
        
		if (GenericTypeStartToken->Type != T_Identifier) continue;
        
		if (GenericTypeStartToken->NextToken->Type != T_ParenthesesLeft) continue;
		Token * TypeToken = GenericTypeStartToken->NextToken->NextToken;
        
		if (TypeToken->Type != T_Identifier) continue;
        
		if (!IsGenericTypeToken(GenericTypeStartToken)) continue;
        
		bool AlreadyExist = false;
        
		for (GenericData * ComparingGeneric = GenericListToCompare ; ComparingGeneric->Next ;ComparingGeneric= ComparingGeneric->Next)
		{
            
			if (!strncmp(ComparingGeneric->TypeToken->String , TypeToken->String , ComparingGeneric->TypeToken->StringLength))
			{
				if (!strncmp(ComparingGeneric->GenericToken->String , GenericTypeStartToken->String , ComparingGeneric->GenericToken->StringLength))
				{
					AlreadyExist = true;
					break;
				}
			}
		}
        
		if (!AlreadyExist)
		{
			CurrentGeneric->LastSeenStructPosition = LastSeenStructPosition;
			CurrentGeneric->TypeToken = TypeToken;
			CurrentGeneric->GenericToken = GenericTypeStartToken;
            
			CurrentGeneric->Next = (GenericData *) malloc(sizeof(GenericData));
			CurrentGeneric = CurrentGeneric->Next;
			(*CurrentGeneric) = {};
		}
        
	}
	
	struct TextBlock
	{
		bool IsInsertText;
        
		char * Start;
		char * End;
        
		TextBlock * Previous;
		TextBlock * Next;
	};
    
	TextBlock * TextBlockHead = (TextBlock * )malloc(sizeof(TextBlock));
    
	(*TextBlockHead) = {};
    
	TextBlockHead->Next = (TextBlock *)malloc(sizeof(TextBlock));
	(*TextBlockHead->Next) = {};
    
	TextBlockHead->Next->Previous = TextBlockHead;
    
	TextBlockHead->Next->Start = Text;
	TextBlockHead->Next->End = Text + TextSize -1;
    
	for (;NewGeneric->Next;NewGeneric= NewGeneric->Next)
	{
		bool NoBlockFound = true;
		
		TextBlock * PreviousBlock = 0;
		TextBlock * NextBlock = 0;
        
		for (TextBlock * CurrentTextBlock = TextBlockHead->Next ; CurrentTextBlock ; CurrentTextBlock= CurrentTextBlock->Next)
		{
			if (CurrentTextBlock->IsInsertText) continue;
            
			if (NewGeneric->LastSeenStructPosition == CurrentTextBlock->Start)
			{
				NoBlockFound = false;
			}
			else if(NewGeneric->LastSeenStructPosition > CurrentTextBlock->Start)
			{
                
				if (NewGeneric->LastSeenStructPosition < CurrentTextBlock->End)
				{
					NoBlockFound = false;
                    
					TextBlock * FirstHalfBlock = (TextBlock*)malloc(sizeof(TextBlock));
                    
					(*FirstHalfBlock) = {};
                    
					FirstHalfBlock->Start = CurrentTextBlock->Start;
					FirstHalfBlock->End = NewGeneric->LastSeenStructPosition - 1;
                    
					FirstHalfBlock->Next = CurrentTextBlock;
					FirstHalfBlock->Previous = CurrentTextBlock->Previous;
					FirstHalfBlock->Previous->Next = FirstHalfBlock;
                    
					CurrentTextBlock->Start = NewGeneric->LastSeenStructPosition;
					CurrentTextBlock->Previous = FirstHalfBlock;
                    
				}
			}
            
			if (!NoBlockFound)
			{
                
				PreviousBlock = CurrentTextBlock->Previous;
				NextBlock = CurrentTextBlock;
				break;
                
			}
		}
        
		if (NoBlockFound) ASSERT;
        
		TextBlock * InsertTextBlock = (TextBlock*)malloc(sizeof(TextBlock));
		(*InsertTextBlock) = {};
        
		InsertTextBlock->IsInsertText = true;
        
		InsertTextBlock->Start = (char*)malloc(1024);
		memset(InsertTextBlock->Start , '\0', 1024);
		
		InsertTextBlock->End = InsertTextBlock->Start + 1024 - 1;
        
		InsertTextBlock->Next = NextBlock;
		NextBlock->Previous = InsertTextBlock;
        
		InsertTextBlock->Previous = PreviousBlock;
		PreviousBlock->Next = InsertTextBlock;
        
		char * CurrentBuffer = InsertTextBlock->Start;
        
		GenerateGenericType(CurrentBuffer , NewGeneric);
	}
    
	char * CurrentInsertedText = (char*)malloc(1024 * 256);
	char * InsertedText = CurrentInsertedText;
    
	for (TextBlock * CurrentTextBlock = TextBlockHead->Next; CurrentTextBlock; CurrentTextBlock = CurrentTextBlock->Next)
	{
		if (CurrentTextBlock->IsInsertText)
		{			
			CurrentInsertedText += sprintf(CurrentInsertedText, CurrentTextBlock->Start);
            
		}
		else
		{
			CurrentInsertedText += sprintf(CurrentInsertedText , "%.*s" ,  (CurrentTextBlock->End - CurrentTextBlock->Start) + 1 , CurrentTextBlock->Start);
		}
        
	}
    
	CurrentInsertedText += sprintf(CurrentInsertedText , "\0");
    
	fprintf(OutputFile , InsertedText);
    
	(*Tail) = CurrentGeneric;
    
	return InsertedText;
}

static TypeData* GenerateTypeEnum(_TokenHeaderList * TokenHeaderList)
{
    UnknowTypeDataHeader = (TypeData*)malloc(sizeof(TypeData));
    (*UnknowTypeDataHeader) = {};
    
    TypeData* CurrentUnknowType = UnknowTypeDataHeader;
	
	TypeData* TypeDataHeader = (TypeData*)malloc(sizeof(TypeData));
	(*TypeDataHeader) = {};
    
	CurrentTypeData = TypeDataHeader;
    
	AddTypeData("float");
	AddTypeData("double");
	AddTypeData("bool");
    
	AddTypeData("int");
	AddTypeData("short");
	AddTypeData("long");
	AddTypeData("long long");
	AddTypeData("char");
    AddTypeData("wchar_t");
    
	AddTypeData("unsigned int");
	AddTypeData("unsigned short");
	AddTypeData("unsigned long");
	AddTypeData("unsigned long long");
	AddTypeData("unsigned char");
    
	AddTypeData("void");
    AddTypeData("HDC");
    
	for (; TokenHeaderList->NextHeader; TokenHeaderList = TokenHeaderList->NextHeader)
	{
		Token* PreviousToken = 0;
		for (CurrentToken = TokenHeaderList->TokenHeader; CurrentToken->NextToken; CurrentToken = CurrentToken->NextToken)
		{
			if (CurrentToken->Type == T_Identifier)
			{
                Token * TypedefToken = 0;
                bool UndefineStruct = false;
                
				bool IsType = false;
				bool IsEnum = false;
                bool IsStruct = false;
                bool IsTypedef = false;
                
				if (FindString(CurrentToken->String,"struct"))
				{
					if (CurrentToken->NextToken->NextToken->Type == T_BracketLeft)
					{
                        CurrentToken = CurrentToken->NextToken;
                        IsStruct = true;
						IsType = true;
					}
                    else
                    {
                        CurrentToken = CurrentToken->NextToken;
                        UndefineStruct = true;
                    }
				}
                
				if (FindString(CurrentToken->String, "enum"))
				{
                    CurrentToken = CurrentToken->NextToken;
					IsType = true;
                    IsEnum = true;
				}
                
                if(FindString(CurrentToken->String, "typedef"))
                {
                    TypedefToken = CurrentToken;
                    Token * TypeDefNameToken = 0;
                    Token * TypeDefEndToken =CurrentToken;
                    
                    for(;TypeDefEndToken;TypeDefEndToken = TypeDefEndToken->NextToken)
                    {
                        if(TypeDefEndToken->Type== T_Semicolon) break;
                        
                        if(TypeDefEndToken->Type == T_Identifier) 
                        {
                            TypeDefNameToken =TypeDefEndToken;
                        }
                        else
                        {
                            UndefineStruct = true;
                        }
                    }
                    
                    if(!TypeDefNameToken) ASSERT;
                    
                    CurrentToken = TypeDefNameToken; 
                    IsType = true;
                    IsTypedef = true;
                    
                    if(UndefineStruct) IsType = false;
                }
                
				if (IsType)
				{
					CurrentTypeData->NameToken = (*CurrentToken);
					
                    CurrentTypeData->IsStruct = IsStruct;;
                    CurrentTypeData->IsTypedef =IsTypedef;
                    CurrentTypeData->IsEnum = IsEnum;
                    
                    if(TypedefToken) CurrentTypeData->TypedefToken = (*TypedefToken);
                    
					CurrentTypeData->NextData = (TypeData*)malloc(sizeof(TypeData));
					CurrentTypeData = CurrentTypeData->NextData;
					(*CurrentTypeData) = {};
				}
                else if(UndefineStruct)
                {
                    CurrentUnknowType->NameToken = (*CurrentToken);
                    
					CurrentUnknowType->NextData = (TypeData*)malloc(sizeof(TypeData));
					CurrentUnknowType = CurrentUnknowType->NextData;
					(*CurrentUnknowType) = {};
                }
			}
            
			PreviousToken = CurrentToken;
		}
	}
    
    for(CurrentUnknowType = UnknowTypeDataHeader ; CurrentUnknowType->NextData ; CurrentUnknowType = CurrentUnknowType->NextData)
    {
        bool NotDefineAtAll = true;
        for(TypeData * DefinedType = TypeDataHeader ; DefinedType->NextData ; DefinedType = DefinedType->NextData)
        {
            if(!strncmp(CurrentUnknowType->NameToken.String , DefinedType->NameToken.String , CurrentUnknowType->NameToken.StringLength))
            {
                NotDefineAtAll = false;
                break;
            }
        }
        
        if(NotDefineAtAll)
        {
            CurrentTypeData->NameToken = CurrentUnknowType->NameToken;
            
            CurrentTypeData->IsStruct = true;;
            CurrentTypeData->IsTypedef =false;
            CurrentTypeData->IsEnum = false;
            
            CurrentTypeData->TypedefToken = {};
            
            CurrentTypeData->NotDefined = true;
            
            CurrentTypeData->NextData = (TypeData*)malloc(sizeof(TypeData));
            CurrentTypeData = CurrentTypeData->NextData;
            (*CurrentTypeData) = {};
        }
    }
    
	fprintf(MetaDataFile, "enum _TypeMeta\n{\n");
    
	for (TypeData* TypeDataPointer = TypeDataHeader; TypeDataPointer->NextData; TypeDataPointer = TypeDataPointer->NextData)
	{
		fprintf(MetaDataFile, "\t_MT_");
        
		for (int CharIndex = 0; CharIndex < TypeDataPointer->NameToken.StringLength; CharIndex++)
		{
			char CurrentChar = TypeDataPointer->NameToken.String[CharIndex];
			if (CurrentChar == ' ')
				CurrentChar = '_';
            
			fputc(CurrentChar, MetaDataFile);
		}
        
		fprintf(MetaDataFile, ",\n");
        
		printf("Type : %.*s\n", TypeDataPointer->NameToken.StringLength, TypeDataPointer->NameToken.String);
	}
    fprintf(MetaDataFile, "\t_MT_Type_Count\n");
    
	fprintf(MetaDataFile, "};\n\n");
    
    fprintf(MetaDataFile, "int _TypeMetaSize[] = \n{\n");
    
    for (TypeData* TypeDataPointer = TypeDataHeader; TypeDataPointer->NextData;  TypeDataPointer = TypeDataPointer->NextData)
    {
        bool SizeNotSure = false;
        
        if (FindString(TypeDataPointer->NameToken.String, "void"))
        {
            SizeNotSure = true;
        }
        else if(TypeDataPointer->NotDefined)
        {
            SizeNotSure = true;
        }
        
        if(SizeNotSure)
        {
            fprintf(MetaDataFile, "0,");
        }
        else
        {
            fprintf(MetaDataFile, "sizeof(");
            
            if(TypeDataPointer->IsTypedef)
            {
                for(Token * OriginalType = TypeDataPointer->TypedefToken.NextToken ; OriginalType->String != TypeDataPointer->NameToken.String ; OriginalType = OriginalType->NextToken)
                {
                    bool IsOriginalType = false;
                    if(OriginalType->Type == T_Identifier) IsOriginalType = true;
                    if(OriginalType->Type == T_Asterisk) IsOriginalType = true;
                    
                    if(IsOriginalType) fprintf(MetaDataFile, "%.*s",OriginalType->StringLength, OriginalType->String);
                }
            }
            else
            {
                for (int CharIndex = 0; CharIndex < TypeDataPointer->NameToken.StringLength; CharIndex++)
                {
                    char CurrentChar = TypeDataPointer->NameToken.String[CharIndex];
                    
                    fputc(CurrentChar, MetaDataFile);
                }
            }
            
            fprintf(MetaDataFile, "),");
        }
        
        fprintf(MetaDataFile , "\/\/%.*s \n",TypeDataPointer->NameToken.StringLength, TypeDataPointer->NameToken.String);
        
    }
    
    fprintf(MetaDataFile, "};\n\n");
    
	fprintf(MetaDataFile, "const char * _TypeMetaName[] = \n{\n");
    
	for (TypeData* TypeDataPointer = TypeDataHeader; TypeDataPointer->NextData; TypeDataPointer = TypeDataPointer->NextData)
	{
		fprintf(MetaDataFile, "\t\"");
        
		for (int CharIndex = 0; CharIndex < TypeDataPointer->NameToken.StringLength; CharIndex++)
		{
			char CurrentChar = TypeDataPointer->NameToken.String[CharIndex];
            
			fputc(CurrentChar, MetaDataFile);
		}
        
		fprintf(MetaDataFile, "\",\n");
	}
    
	fprintf(MetaDataFile, "};\n\n");
    
    
    fprintf(MetaDataFile, "bool _IsTypeEnum[] = \n{\n");
    
	for (TypeData* TypeDataPointer = TypeDataHeader; TypeDataPointer->NextData; TypeDataPointer = TypeDataPointer->NextData)
	{
        fprintf(MetaDataFile,TypeDataPointer->IsEnum ? "true,\n" : "false,\n");
    }
    
	fprintf(MetaDataFile, "};\n\n");
    
	return TypeDataHeader;
}

static void PrintName(char *MemberName,int Length)
{
	for (int CurrentCharIndex = 0; CurrentCharIndex < Length; CurrentCharIndex++)
	{
		char CurrentChar = MemberName[CurrentCharIndex];
        
		if (CurrentChar == ' ')
		{
			CurrentChar = '_';
		}
        
		fputc(CurrentChar, MetaDataFile);
	}
}

static void GenerateStructMemberMeta(_IntrospectionData* CurrentIntropectionData)
{
	fprintf(MetaDataFile, "global int MemberMetaCount_%.*s = %d;\n", CurrentIntropectionData->StructNameLength, CurrentIntropectionData->StructName, CurrentIntropectionData->MemberCount);
    
	fprintf(MetaDataFile, "global _MemberMetaData MemberMeta_%.*s[%d] =\n{\n", CurrentIntropectionData->StructNameLength, CurrentIntropectionData->StructName, CurrentIntropectionData->MemberCount);
    
	for (StructMemberData* CurrentMember = CurrentIntropectionData->StructMemberHeader; CurrentMember->NextData; CurrentMember = CurrentMember->NextData)
	{
        
		fprintf(MetaDataFile, "\t{ \"%.*s\" , ", CurrentMember->MemberNameToken->StringLength, CurrentMember->MemberNameToken->String);
        
		fprintf(MetaDataFile, CurrentMember->IsMultiplePointer ?"true,":"false,");
        
		fprintf(MetaDataFile, CurrentMember->IsPointer ?"true,":"false,");
        
		fprintf(MetaDataFile, CurrentMember->FirstLeftBracketIndex != -1?"true,":"false,");
        
        if(CurrentMember->FirstLeftBracketIndex != -1)
        {
            bool IsArraySize = false;
            bool Ended = false;
            
            for(int TokenIndex = CurrentMember->FirstLeftBracketIndex ;TokenIndex < CurrentMember->TokenChainLength ; TokenIndex++)
            {
                Token * CurrentMemberToken = CurrentMember->TokenChain[TokenIndex];
                
                if(CurrentMemberToken->Type == T_SquareBracketRight)
                {
                    Ended = true;
                    IsArraySize = false;
                }
                
                if(IsArraySize)
                {
                    fprintf(MetaDataFile, "%.*s", CurrentMemberToken->StringLength, CurrentMemberToken->String);
                    
                }
                
                if(CurrentMemberToken->Type == T_SquareBracketLeft)
                {
                    IsArraySize = true;
                    
                    if(Ended)
                    {
                        
                        fprintf(MetaDataFile, " * ");
                        Ended = false;
                    }
                }
            }
        }
        else
        {
            fprintf(MetaDataFile,"0");
        }
        
        fprintf(MetaDataFile,",");
        
		fprintf(MetaDataFile, "_MT_");
        
		bool TheFirstToken = true;
        
		TheFirstToken = true;
		for (int TypeTokenIndex = 0 ; TypeTokenIndex < CurrentMember->MemberTypeTokenArrayCount ; TypeTokenIndex++ )
		{
			Token * CurrentTypeToken = CurrentMember->TokenChain[TypeTokenIndex];
            
			if (CurrentTypeToken->Type != T_Identifier) continue;
			if (FindString(CurrentTypeToken->String , "const")) continue;
			if (FindString(CurrentTypeToken->String , "persist")) continue;
            
			if (TheFirstToken)
			{
				TheFirstToken = false;
			}
			else
			{
				fprintf(MetaDataFile , "_");
			}
            
			PrintName(CurrentTypeToken->String,CurrentTypeToken->StringLength);			
		}
		
		fputc(',', MetaDataFile);
		fputc('\"', MetaDataFile);
		
		TheFirstToken = true;		
		for (int TypeTokenIndex = 0 ; TypeTokenIndex < CurrentMember->MemberTypeTokenArrayCount ; TypeTokenIndex++ )
		{
			Token * CurrentTypeToken = CurrentMember->TokenChain[TypeTokenIndex];
            
			if (CurrentTypeToken->Type != T_Identifier) continue;
			if (FindString(CurrentTypeToken->String , "const")) continue;
			if (FindString(CurrentTypeToken->String , "persist")) continue;
            
			if (TheFirstToken)
			{
				TheFirstToken = false;
			}
			else
			{
				fprintf(MetaDataFile , "_");
			}
            
			PrintName(CurrentTypeToken->String,CurrentTypeToken->StringLength);			
		}
		
		fputc('\"', MetaDataFile);
		fputc(',', MetaDataFile);
        
		fprintf(MetaDataFile, "(int)&((%.*s *)", CurrentIntropectionData->StructNameLength, CurrentIntropectionData->StructName);
        
		fprintf(MetaDataFile, "0)->%.*s ,", CurrentMember->MemberNameToken->StringLength, CurrentMember->MemberNameToken->String);
        
		fprintf(MetaDataFile, "_TypeMetaSize[ _MT_");
        
		TheFirstToken = true;
		for (int TypeTokenIndex = 0 ; TypeTokenIndex < CurrentMember->MemberTypeTokenArrayCount ; TypeTokenIndex++ )
		{
			Token * CurrentTypeToken = CurrentMember->TokenChain[TypeTokenIndex];
			
			if (CurrentTypeToken->Type != T_Identifier) continue;
			if (FindString(CurrentTypeToken->String , "const")) continue;
			if (FindString(CurrentTypeToken->String , "persist")) continue;
            
			if (TheFirstToken)
			{
				TheFirstToken = false;
			}
			else
			{
				fprintf(MetaDataFile , "_");
			}
            
			fprintf(MetaDataFile, "%.*s", CurrentTypeToken->StringLength, CurrentTypeToken->String);
		}
        
        fprintf(MetaDataFile, "],");
        
		fprintf(MetaDataFile, "}, \n");
	}
    
	fprintf(MetaDataFile, "};\n\n");
}

static void GenerateEnumString(Token * NameToken)
{	
	fprintf(MetaDataFile , "global const char * %.*s_String[] = \n{\n" , NameToken->StringLength , NameToken->String);
    
	for (Token * MemberToken = NameToken->NextToken->NextToken;; MemberToken = MemberToken->NextToken->NextToken)
	{
		if (MemberToken->Type != T_Identifier)
		{
			if (MemberToken->Type == T_BracketRight) break;
            
			ASSERT;
		}
        
		fprintf(MetaDataFile, " \"%.*s\",\n" , MemberToken->StringLength , MemberToken->String);
		
		if (MemberToken->NextToken->Type == T_BracketRight) break;
	}
    
	fprintf(MetaDataFile , "};\n\n");
}

static void print_all_function_declaration_or_definition(FunctionData * FunctionDataHead,char * Text , long TextSize , const char * OutputName , bool Declaration)
{
	
	for (FunctionData * CurrentFunctionData = FunctionDataHead; CurrentFunctionData->Next; CurrentFunctionData = CurrentFunctionData->Next)
	{
		if (!CurrentFunctionData->StartToken) continue;
        
		if (CurrentFunctionData->LocalFunctionList)
		{
			print_all_function_declaration_or_definition(CurrentFunctionData->LocalFunctionList, Text , TextSize ,OutputName, Declaration);
		}
        
		Token * NameToken = CurrentFunctionData->StartToken;
		Token * RightBracketToken = CurrentFunctionData->RightBracketToken;
		Token * LeftBracketToken = CurrentFunctionData->LeftBracketToken;
		Token * RightParenthesesToken = CurrentFunctionData->RightParenthesesToken;
        
		bool PrintBody = false;
        
		if (!Declaration)
		{
			if (CurrentFunctionData->Local)
			{
				PrintBody = true;
			}
		}
        
		if(PrintBody)
		{
			int StringLength = RightBracketToken->String - NameToken->String + 1;
            
			fprintf(MetaDataFile, "%.*s" ,StringLength, NameToken->String);
			fprintf(MetaDataFile, "\n");
            
#if 1
			if (OutputName)
			{
				for (int CharIndex = 0 ; CharIndex < StringLength ; CharIndex++)
				{
					if (NameToken->String[CharIndex] == '\n') continue;
					if (NameToken->String[CharIndex] == '\r') continue;
                    
					NameToken->String[CharIndex] = ' ';
				}
			}
#endif
            
		}
		else if(Declaration)
		{
			int NameLength = RightParenthesesToken->String - NameToken->String + RightParenthesesToken->StringLength;
			if (*(NameToken->String + NameLength - 1) == '\n') NameLength--;
            
			fprintf(MetaDataFile, "%.*s" ,NameLength , NameToken->String);
			fprintf(MetaDataFile, ";\n");
		}
		
	}    
}

static char * insert_line(char * Text , long TextSize , const char * OutputName , const char * SourceFileName)
{
	int LineIndex = 2;
    
	char * SratchBuffer = (char *)malloc(1024 * 256);
	memset(SratchBuffer , '\0' , 1024 * 256);
	char * SratchBufferHead = SratchBuffer;
    
	for (int TextIndex =0; TextIndex < TextSize ;TextIndex++)
	{
		char Character = Text[TextIndex];
        
		(*SratchBuffer) = Character;
		SratchBuffer++;
        
#if 1
		if (Character == '\n')
		{
            
			SratchBuffer += sprintf(SratchBuffer , "#line %d \"%s\"\n", LineIndex , SourceFileName);
			LineIndex++;
		}
        
#endif
	}
    
	return SratchBufferHead;
    
}


static FunctionData * find_function_and_move_local_function(Token * CurrentToken, Token ** TailTokenResult , bool Nested)
{
    
	int NestedLevel = 0;
	bool EnteredFunction = false;
	bool FoundFunction = false;
    
	FunctionData * CurrentFunctionData = (FunctionData *)malloc(sizeof(FunctionData));
	FunctionData * FunctionDataHead = CurrentFunctionData;
    
	(*CurrentFunctionData) = {};
    
	for ( ; CurrentToken->NextToken; CurrentToken = CurrentToken->NextToken)
	{
        
		if (FindStringExact(CurrentToken->String , "internal" , strlen("internal")))
		{
			if (EnteredFunction)
			{
				Token * TokenResult = 0;
                
				for (; FindStringExact(CurrentToken->String , "internal" , strlen("internal"));)
				{
					
					FunctionData * LocalFunctionData = find_function_and_move_local_function(CurrentToken , &TokenResult , true);
					CurrentToken = TokenResult;
                    
					FunctionData * LocalFunctionDataTail = LocalFunctionData;
					for( ; LocalFunctionDataTail->Next ; LocalFunctionDataTail=LocalFunctionDataTail->Next);
                    
					if (!CurrentFunctionData->LocalFunctionList)
					{
						CurrentFunctionData->LocalFunctionList = LocalFunctionData;
					}
					else
					{	
						CurrentFunctionData->CurrentLocalFunctionData->Next = LocalFunctionData;
					}
					
					CurrentFunctionData->CurrentLocalFunctionData = LocalFunctionDataTail;
				}
                
			}
			else
			{
				CurrentFunctionData->StartToken = CurrentToken;
				FoundFunction = true;
                
			}
            
		}
        
		if (FoundFunction)
		{
			if(CurrentToken->Type == T_ParenthesesRight)
			{
                
				if (!CurrentFunctionData->RightParenthesesToken)
				{
					CurrentFunctionData->RightParenthesesToken = CurrentToken;
				}
                
			}
            
		}
        
		if (CurrentToken->Type == T_BracketLeft)
		{
			if (FoundFunction)
			{
				if (!EnteredFunction)
				{
					CurrentFunctionData->LeftBracketToken = CurrentToken;
					EnteredFunction = true;
                    
				}
                
			}
            
			NestedLevel++;
		}
        
		if (CurrentToken->Type == T_BracketRight)
		{
			NestedLevel--;
		}
        
		if (NestedLevel == 0)
		{
			if (EnteredFunction)
			{
				CurrentFunctionData->RightBracketToken = CurrentToken;
				CurrentFunctionData->Local = Nested;
                
				CurrentFunctionData->Next = (FunctionData * )malloc(sizeof(FunctionData));
				CurrentFunctionData = CurrentFunctionData->Next;
				(*CurrentFunctionData) = {};
                
				if (Nested)
				{
					(*TailTokenResult) = CurrentToken->NextToken;
					break;
				}
                
				EnteredFunction = false;
				FoundFunction = false;
                
			}
            
		}
        
	}
    
	return FunctionDataHead;
}

static void GenerateMetaData(_TokenHeaderList* TokenList,TypeData * TypeDataHeader)
{
	_IntrospectionData* IntrospectionHeader = (_IntrospectionData*)malloc(sizeof(_IntrospectionData));
	_IntrospectionData* CurrentIntropectionData = IntrospectionHeader;
	*CurrentIntropectionData = {};
    
	for (_TokenHeaderList* CurrentTokenList = TokenList; CurrentTokenList->NextHeader; CurrentTokenList = CurrentTokenList->NextHeader)
	{
		Token* TokenHeader = CurrentTokenList->TokenHeader;
        
		for (CurrentToken = TokenHeader; CurrentToken->NextToken; CurrentToken = CurrentToken->NextToken)
		{
			if (FindString(CurrentToken->String , "enum"))
			{
				GenerateEnumString(CurrentToken->NextToken);
			}
		}
	}
	
	int TokenTotalCount = 0;
	for (_TokenHeaderList* CurrentTokenList = TokenList; CurrentTokenList->NextHeader; CurrentTokenList = CurrentTokenList->NextHeader)
	{
		Token* TokenHeader = CurrentTokenList->TokenHeader;
		for (CurrentToken = TokenHeader; CurrentToken->NextToken; CurrentToken = CurrentToken->NextToken)
		{
			printf("%.*s \n" ,CurrentToken->StringLength , CurrentToken->String);
			TokenTotalCount++;
		}
	}
    
	for (_TokenHeaderList* CurrentTokenList = TokenList; CurrentTokenList->NextHeader; CurrentTokenList = CurrentTokenList->NextHeader)
	{
		Token* TokenHeader = CurrentTokenList->TokenHeader;
        
		for (CurrentToken = TokenHeader; CurrentToken->NextToken; CurrentToken = CurrentToken->NextToken)
		{
#if 0
			for (int CharIndex = 0; CharIndex < CurrentToken->StringLength; CharIndex++)
			{
				putchar(CurrentToken->String[CharIndex]);
			}
			putchar('\n');
#endif  
			
			if(FindString(CurrentToken->String,"typedef"))
            {
                if(FindString(CurrentToken->NextToken->String,"struct"))
                {
                    CurrentToken = CurrentToken->NextToken;
                    continue;
                }
            }
            
            if (CurrentToken->Type != T_Identifier) continue;
			
            if (!FindString(CurrentToken->String,"struct")) continue;
            
            *CurrentIntropectionData = GenerateIntrospectData(CurrentToken);
            
            if (CurrentIntropectionData->IsValid)
            {
                CurrentIntropectionData->Next = (_IntrospectionData*)malloc(sizeof(_IntrospectionData));
                CurrentIntropectionData = CurrentIntropectionData->Next;
            }
            
            (*CurrentIntropectionData) = {};
			
		}
	}
    
	fprintf(MetaDataFile, "enum IntrospectedStruct\n{\n");
    
	for (_IntrospectionData* CurrentIntrospectionData = IntrospectionHeader; CurrentIntrospectionData->Next; CurrentIntrospectionData = CurrentIntrospectionData->Next)
	{
		if (CurrentIntrospectionData->IsValid)
		{
			fprintf(MetaDataFile, "\tIS_%.*s, \n", CurrentIntrospectionData->StructNameLength, CurrentIntrospectionData->StructName);
		}
	}
    
	fprintf(MetaDataFile, "};\n\n");
    
	for (_IntrospectionData* CurrentIntrospectionData = IntrospectionHeader; CurrentIntrospectionData->Next; CurrentIntrospectionData = CurrentIntrospectionData->Next)
	{
		if (CurrentIntrospectionData->IsValid)
		{
			GenerateStructMemberMeta(CurrentIntrospectionData);
		}
	}
    
	fprintf(MetaDataFile, "global _StructMetaData _TypeStructMeta_[] = \n{\n");
    
	for (TypeData* CurrentData = TypeDataHeader; CurrentData->NextData; CurrentData = CurrentData->NextData)
	{
        bool HaveMembers = false;
		if (CurrentData->IsStruct)
		{
            if(!CurrentData->NotDefined)
            {
                HaveMembers = true;
            }
        }
        
        if(HaveMembers)
        {
            fprintf(MetaDataFile, "\tGetStructMeta(%.*s),\n", CurrentData->NameToken.StringLength, CurrentData->NameToken.String);
        }
		else
		{
			fprintf(MetaDataFile, "\t{},\n");
		}
	}
    
	fprintf(MetaDataFile, "};\n");
    
	fprintf(MetaDataFile, "\n");
}

extern "C" int main()
{
    
	//weir stuff
	//if i read in text it read /r/n to /n which make the whole file smaller
	//so i need to use rb?
    
	//TODO: these crap just written in 30 mins
	//make it nicer
    
	struct _FunctionFile
	{
		const char* Name;
		const char* OutputName;
	};
    
	struct
	{
		const char* OutputMetaDataName;
		const char* DataFileToScan[128];
        
		_FunctionFile FunctionFile[128];
	} OutputFiles[] =
	{
		{ 
			"../Code/d_main_meta_generated.cpp", 
			{
				"../Code/d_header.h",
				"../Code/d_main.h",
				0
			},
			0
		},
        
		{ 
			"../Code/d_game_meta_generated.cpp",
			{
				"../Code/d_header.h",
				"../Code/d_main.h",
				"../Code/d_renderdata.cpp",
				"../Code/d_gamedata.cpp",
				0
			},
			{
				{ "../Code/d_function.cpp"  , 0  } ,
				{ "../Code/d_render.cpp" , 0  },
				{ "../Code/d_text.cpp" , 0  },
				{ "../Code/d_gamefunction.cpp" , 0  },
				{ "../Code/d_game.cpp" , "../Code/d_game_generated.cpp"} ,
				
				{ 0 , 0},
			}
		},
        
		{0,0,0}
	};
    
	for (int OutputFileIndex = 0; OutputFiles[OutputFileIndex].OutputMetaDataName; OutputFileIndex++)
	{
		MetaDataFile = fopen(OutputFiles[OutputFileIndex].OutputMetaDataName, "w");
        
		_TokenHeaderList * TokenHeaderList = (_TokenHeaderList*)malloc(sizeof(_TokenHeaderList));
        
		_TokenHeaderList * CurrentTokenHeader = TokenHeaderList;
		(*CurrentTokenHeader) = {};
        
		GenericData * CurrentGeneric = (GenericData *)malloc(sizeof(GenericData));
		(*CurrentGeneric) = {};
		GenericData * GenericHead = CurrentGeneric;
        
		for (const char** CurrentScanningFile= OutputFiles[OutputFileIndex].DataFileToScan; (*CurrentScanningFile); CurrentScanningFile++)
		{
            
			const char * InputFile = (*CurrentScanningFile);
			FILE* CodeFile = fopen(InputFile , "rb");
            
			char* CurrentText = 0;
			long CurrentTextSize = 0;
			CurrentTokenHeader->TokenHeader = ConvertFileTextToToken(CodeFile , &CurrentText , &CurrentTextSize);
            
#if 0			
			const char* OutputFile = OutputFiles[OutputFileIndex].OutputMetaDataName;
			
			Local( int , GetLastCharPosition, (const char * Text , char TargetChar)
				  {
					  int TargetCharPosition = 0;			
					  for (int CharIndex= 0 ; Text[CharIndex]; CharIndex++)
					  {
						  if (Text[CharIndex] == TargetChar) TargetCharPosition = CharIndex;
					  }
                      
					  return TargetCharPosition;
				  });
            
			char ModifiedFileName[256] = {};
            
			int InputNameStart = GetLastCharPosition(InputFile , '/') + 1;
			int InputNameCount = GetLastCharPosition(InputFile , '.') - InputNameStart ;
            
			int OutputNameStart = GetLastCharPosition(OutputFile , '/') + 1;
			int OutputNameCount = GetLastCharPosition(OutputFile , '.') - OutputNameStart;
            
			strncat(ModifiedFileName , InputFile + InputNameStart,InputNameCount);
			strcat(ModifiedFileName , "_target_to_");
			strncat(ModifiedFileName , OutputFile + OutputNameStart,OutputNameCount);
			strcat(ModifiedFileName , InputFile + GetLastCharPosition(InputFile , '.'));
            
			FILE * ModifiedCodeFile = fopen(ModifiedFileName , "wb");
            
			char * ModifiedText = found_and_generate_generic_type( &CurrentGeneric ,GenericHead , CurrentText ,CurrentTextSize ,CurrentTokenHeader->TokenHeader ,ModifiedCodeFile);
			CurrentTokenHeader->TokenHeader = ConvertTextToToken(ModifiedText, 1024 * 256);
			modify_generic_type_token(CurrentTokenHeader->TokenHeader);
            
			fclose(ModifiedCodeFile);
#endif
            
			CurrentTokenHeader->NextHeader = (_TokenHeaderList*)malloc(sizeof(_TokenHeaderList));
			CurrentTokenHeader = CurrentTokenHeader->NextHeader;
			(*CurrentTokenHeader) = {};
            
		}
        
		CurrentTokenHeader = TokenHeaderList;
		
		TypeData * TypeDataHeader = GenerateTypeEnum(CurrentTokenHeader);
        
		GenerateMetaData(CurrentTokenHeader, TypeDataHeader);
        
		CurrentTokenHeader = CurrentTokenHeader->NextHeader;
        
		for (_FunctionFile * CurrentScanningFile = OutputFiles[OutputFileIndex].FunctionFile; CurrentScanningFile->Name; CurrentScanningFile++)
		{
			const char * InputFile = CurrentScanningFile->Name;
			FILE* CodeFile = fopen(InputFile , "rb");
            
			char* CurrentText = 0;
			long CurrentTextSize = 0;
            
			Token * TokenHead = ConvertFileTextToToken(CodeFile,&CurrentText , &CurrentTextSize  );			
			if (CurrentScanningFile->OutputName)
			{
				CurrentText = insert_line(CurrentText , CurrentTextSize,CurrentScanningFile->OutputName ,InputFile );
				TokenHead = ConvertTextToToken(CurrentText , 1024*256);
			}
            
			FunctionData * FunctionDataHead = find_function_and_move_local_function(TokenHead,0, false);
			print_all_function_declaration_or_definition(FunctionDataHead ,CurrentText , CurrentTextSize,CurrentScanningFile->OutputName , true);
			print_all_function_declaration_or_definition(FunctionDataHead ,CurrentText , CurrentTextSize,CurrentScanningFile->OutputName , false);
            
			if (CurrentScanningFile->OutputName)
			{
                
				FILE * ModifiedFile = fopen(CurrentScanningFile->OutputName , "w");
				//fprintf(ModifiedFile , CurrentText );
				
				fputs(CurrentText , ModifiedFile);
                
				fclose(ModifiedFile);
                
			}
            
		}
        
		fclose(MetaDataFile);
	}
	
	return 0;
}