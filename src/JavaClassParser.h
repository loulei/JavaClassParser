/*
 * JavaClassParser.h
 *
 *  Created on: 2018年3月7日
 *      Author: loulei
 */

#ifndef JAVACLASSPARSER_H_
#define JAVACLASSPARSER_H_

#define u2 uint16_t;
#define u4 uint32_t;

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
	uint16_t name_idx;
	uint32_t length;
	char *info;
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

#define CLASS_FILE "file/Hello.class"

#endif /* JAVACLASSPARSER_H_ */



















