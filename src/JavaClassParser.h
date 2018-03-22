/*
 * JavaClassParser.h
 *
 *  Created on: 2018年3月7日
 *      Author: loulei
 */

#ifndef JAVACLASSPARSER_H_
#define JAVACLASSPARSER_H_


#define u1 uint8_t;
#define u2 uint16_t;
#define u4 uint32_t;

typedef struct _Code Code;

typedef enum {
	ACC_PUBLIC		= 0x0001,
	ACC_FINAL		= 0x0010,
	ACC_SUPER		= 0x0020,
	ACC_INTERFACE	= 0x0200,
	ACC_ABSTRACT	= 0x0400,
	ACC_SYNTHETIC	= 0x1000,
	ACC_ANNOTATION	= 0x2000,
	ACC_ENUM 		= 0x4000
} AccessFlags;

typedef struct {
	uint16_t start_pc;
	uint16_t line_number;
} LineNumberTable;

typedef struct {
	uint16_t name_idx;
	uint32_t length;
	uint16_t table_length;
	union {
		Code *code;
		LineNumberTable *table;
		unsigned char *info;
		uint16_t nameIndex;
	}value;
} Attribute;

typedef struct {
	char *file_name;
	FILE *file;
} ClassFile;

typedef struct {
	uint32_t high;
	uint32_t low;
} Double;

typedef struct {
	uint16_t flags;
	uint16_t name_idx;
	uint16_t desc_idx;
	uint16_t attrs_count;
	Attribute *attrs;
} Field;

typedef struct {
	uint32_t high;
	uint32_t low;
} Long;

typedef struct {
	uint16_t flags;
	uint16_t name_idx;
	uint16_t desc_idx;
	uint16_t attrs_count;
	Attribute *attrs;
} Method;

typedef struct {
	uint16_t class_idx;
	uint16_t name_idx;
} Ref;

typedef struct {
	uint16_t length;
	char *value;
} String;

typedef struct {
	uint8_t tag;
	union {
		String string;
		float flt;
		Double dbl;
		Long lng;
		int32_t integer;
		Ref ref;
	} value;
} Item;

typedef struct{
	uint16_t start_pc;
	uint16_t end_pc;
	uint16_t handler_pc;
	uint16_t catch_type;
}Exception_table;

struct _Code {
	uint16_t max_stack;
	uint16_t max_locals;
	uint32_t code_length;
	unsigned char *code;
	uint16_t exception_table_length;
	Exception_table *excep_table;
	uint16_t attributes_count;
	Attribute *attribute;
} ;


typedef struct{
	char *file_name;
	uint16_t minor_version;
	uint16_t major_version;
	uint16_t const_pool_count;
//	uint32_t pool_size_bytes;
	Item *items;
	uint16_t flags;
	uint16_t this_class;
	uint16_t super_class;
	uint16_t interfaces_count;
	Ref *interfaces;
	uint16_t fields_count;
	Field *fields;
	uint16_t methods_count;
	Method *methods;
	uint16_t attributes_count;
	Attribute *attribute;
} Class;

/* Stack Frame */
#define STACK_ENTRY_NONE        0
#define STACK_ENTRY_INT         1
#define STACK_ENTRY_REF         2
#define STACK_ENTRY_LONG        3
#define STACK_ENTRY_DOUBLE      4
#define STACK_ENTRY_FLOAT       5

typedef struct {
	unsigned char entry[8];
	int type;
} StackEntry;

typedef struct{
	int max_size;
	int size;
	StackEntry *store;
} StackFrame;

typedef int (*opCodeFunc)(unsigned char **opCode, StackFrame *stack, Class *class);

typedef struct {
    char *name;
    unsigned char opCode;
    int offset;
    opCodeFunc func;
} byteCode;

static char *CPool_strings[] = {
		"Undefined",
		"String_UTF8",
		"Undefined",
		"Integer",
		"Float",
		"Long",
		"Double",
		"Class",
		"String",
		"Field",
		"Method",
		"InterfaceMethod",
		"Name",
		"Undefined",
		"Undefined",
		"MethodHandle",
		"MethodType",
		"InvokeDynamic"
};

enum RANGES{
	MIN_CPOOL_TAG = 1,
	MAX_CPOOL_TAG = 18
};

typedef enum {
	STRING_UTF8			= 1,
	INTEGER				= 3,
	FLOAT				= 4,
	LONG				= 5,
	DOUBLE				= 6,
	CLASS				= 7,
	STRING				= 8,
	FIELD				= 9,
	METHOD				= 10,
	INTERFACE_METHOD	= 11,
	NAME				= 12,
	METHOD_HANDLE		= 15,
	METHOD_TYPE			= 16,
	INVOKE_DYNAMIC		= 18
} CPool_t;


static inline char *tag2Str(uint8_t tag){
	return CPool_strings[tag];
}


bool is_class(FILE *class_file);
Class *read_class(const ClassFile class_file);
void parse_header(ClassFile class_file, Class *class);
void print_class(FILE *stream, const Class *class);
void parse_const_pool(Class *class, const uint16_t const_pool_count, const ClassFile class_file);
Item *get_item(const Class *class, const uint16_t idx);
long to_long(const Long lng);
double to_double(const Double dbl);
Item *get_class_string(const Class *class, const uint16_t index);
char *field2Str(const char fld_type);

void run(Class *class);
Method *findMethod(Class *class, char *method_name);
void stackInit(StackFrame *stack, int entry_size);
int executeMethod(Method *method, StackFrame *stack, Class *class);

void pushInt(StackFrame *stack, int value);

static int op_aload_0(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_bipush(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_dup(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_getstatic(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_iadd(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_iconst_0(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_iconst_1(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_iconst_2(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_iconst_3(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_iconst_4(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_iconst_5(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_dconst_1(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_idiv(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_imul(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_dadd(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_dmul(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_d2i(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_invokespecial(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_invokevirtual(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_invokestatic(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_iload(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_iload_1(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_iload_2(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_iload_3(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_istore(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_istore_1(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_istore_2(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_istore_3(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_isub(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_ldc(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_ldc2_w(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_new(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_irem(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_sipush(unsigned char **opCode, StackFrame *stack, Class *class);
static int op_return(unsigned char **opCode, StackFrame *stack, Class *class);


static byteCode byteCodes[] = {
    { "aload_0"         , 0x2A, 1,  op_aload_0          },
    { "bipush"          , 0x10, 2,  op_bipush           },
    { "dup"             , 0x59, 1,  op_dup              },
    { "getstatic"       , 0xB2, 3,  op_getstatic        },
    { "iadd"            , 0x60, 1,  op_iadd             },
    { "iconst_0"        , 0x03, 1,  op_iconst_0         },
    { "iconst_1"        , 0x04, 1,  op_iconst_1         },
    { "iconst_2"        , 0x05, 1,  op_iconst_2         },
    { "iconst_3"        , 0x06, 1,  op_iconst_3         },
    { "iconst_4"        , 0x07, 1,  op_iconst_4         },
    { "iconst_5"        , 0x08, 1,  op_iconst_5         },
    { "dconst_1"        , 0x0F, 1,  op_dconst_1         },
    { "idiv"            , 0x6C, 1,  op_idiv             },
    { "imul"            , 0x68, 1,  op_imul             },
    { "dadd"            , 0x63, 1,  op_dadd             },
    { "dmul"            , 0x6B, 1,  op_dmul             },
    { "d2i"             , 0x8e, 1,  op_d2i              },
    { "invokespecial"   , 0xB7, 3,  op_invokespecial    },
    { "invokevirtual"   , 0xB6, 3,  op_invokevirtual    },
    { "invokestatic"    , 0xB8, 3,  op_invokestatic     },
    { "iload"           , 0x15, 2,  op_iload            },
    { "iload_1"         , 0x1B, 1,  op_iload_1          },
    { "iload_2"         , 0x1C, 1,  op_iload_2          },
    { "iload_3"         , 0x1D, 1,  op_iload_3          },
    { "istore"          , 0x36, 2,  op_istore           },
    { "istore_1"        , 0x3C, 1,  op_istore_1         },
    { "istore_2"        , 0x3D, 1,  op_istore_2         },
    { "istore_3"        , 0x3E, 1,  op_istore_3         },
    { "isub"            , 0x64, 1,  op_isub             },
    { "ldc"             , 0x12, 2,  op_ldc              },
    { "ldc2_w"          , 0x14, 3,  op_ldc2_w           },
    { "new"             , 0xBB, 3,  op_new              },
    { "irem"            , 0x70, 1,  op_irem             },
    { "sipush"          , 0x11, 3,  op_sipush           },
    { "return"          , 0xB1, 1,  op_return           }
};

static size_t byteCode_size = sizeof(byteCodes) / sizeof(byteCode);

static opCodeFunc findOpCodeFunc(unsigned char op){
	int i;
	for(i=0; i<byteCode_size; i++){
		if(op == byteCodes[i].opCode)
			return byteCodes[i].func;
	}
	return NULL;
}

static int findOpCodeOffset(unsigned char op){
	int i;
	for(i=0; i<byteCode_size; i++){
		if(op == byteCodes[i].opCode)
			return byteCodes[i].offset;
	}
	return NULL;
}

static char *findOpCode(unsigned char op){
	int i;
	for(i=0; i<byteCode_size; i++){
		if(op == byteCodes[i].opCode)
			return byteCodes[i].name;
	}
	return NULL;
}


#define CLASS_FILE "file/Foo1.class"
#define OP_CODE_FILE "file/ins.txt"

#endif /* JAVACLASSPARSER_H_ */



















