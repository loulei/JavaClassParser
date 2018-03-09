/*
 ============================================================================
 Name        : JavaClassParser.c
 Author      : loulei
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "JavaClassParser.h"

int main(void) {
	FILE *classFile = fopen(CLASS_FILE, "rb");
	if(!classFile){
		printf("open file %s error:%s\n", CLASS_FILE, strerror(errno));
		return EXIT_FAILURE;
	}

	if(!is_class(classFile)){
		printf("invalid class file\n");
		return EXIT_FAILURE;
	}

	const ClassFile class_file = {
			CLASS_FILE,
			classFile
	};

	Class *class = read_class(class_file);

	if(!class){
		printf("parse class error\n");
		return EXIT_FAILURE;
	}

	print_class(stdout, class);

	free(class);
	return EXIT_SUCCESS;
}

bool is_class(FILE *class_file){
	uint32_t magicNum;
	size_t num_read = fread(&magicNum, sizeof(uint32_t), 1, class_file);
	return num_read == 1 && be32toh(magicNum) == 0xcafebabe;
}

Class *read_class(const ClassFile class_file){
	Class *class = (Class*)malloc(sizeof(Class));
	parse_header(class_file, class);
	parse_const_pool(class, class->const_pool_count, class_file);

	fread(&class->flags, sizeof(class->flags), 1, class_file.file);
	class->flags = be16toh(class->flags);

	fread(&class->this_class, sizeof(class->this_class), 1, class_file.file);
	class->this_class = be16toh(class->this_class);

	fread(&class->super_class, sizeof(class->super_class), 1, class_file.file);
	class->super_class = be16toh(class->super_class);

	fread(&class->interfaces_count, sizeof(class->interfaces_count), 1, class_file.file);
	class->interfaces_count = be16toh(class->interfaces_count);

	class->interfaces = calloc(class->interfaces_count, sizeof(Ref));
	int i = 0;
	while(i < class->interfaces_count){
		fread(&class->interfaces[i].class_idx, sizeof(class->interfaces[i].class_idx), 1, class_file.file);
		class->interfaces[i].class_idx = be16toh(class->interfaces[i].class_idx);
		i++;
	}

	fread(&class->fields_count, sizeof(class->fields_count), 1, class_file.file);
	class->fields_count = be16toh(class->fields_count);
	return class;
}

void parse_header(ClassFile class_file, Class *class){
	class->file_name = class_file.file_name;
	fread(&class->minor_version, sizeof(uint16_t), 1, class_file.file);
	fread(&class->major_version, sizeof(uint16_t), 1, class_file.file);
	fread(&class->const_pool_count, sizeof(uint16_t), 1, class_file.file);

	class->minor_version = be16toh(class->minor_version);
	class->major_version = be16toh(class->major_version);
	class->const_pool_count = be16toh(class->const_pool_count);
}

void parse_const_pool(Class *class, const uint16_t const_pool_count, const ClassFile class_file){
	const int MAX_ITEMS = const_pool_count - 1;
	int i;
	char tag_byte;
	Ref ref;

	class->items = calloc(MAX_ITEMS, sizeof(Item));
	for(i=0; i<MAX_ITEMS; i++){
		fread(&tag_byte, sizeof(char), 1, class_file.file);
		if(tag_byte < MIN_CPOOL_TAG || tag_byte > MAX_CPOOL_TAG){
			printf("invalid tag %d\n", tag_byte);
			break;
		}

		String s;
		uint16_t ptr_idx = i;
		Item *item = class->items + ptr_idx;
		item->tag = tag_byte;

		switch (tag_byte){
		case STRING_UTF8:
			fread(&s.length, sizeof(s.length), 1, class_file.file);
			s.length = be16toh(s.length);
			s.value = malloc(sizeof(char) * s.length);
			fread(s.value, sizeof(char), s.length, class_file.file);
			item->value.string = s;
			break;
		case INTEGER:
			fread(&item->value.integer, sizeof(item->value.integer), 1, class_file.file);
			item->value.integer = be32toh(item->value.integer);
			break;
		case FLOAT:
			fread(&item->value.flt, sizeof(item->value.flt), 1, class_file.file);
			item->value.flt = be32toh(item->value.flt);
			break;
		case LONG:
			fread(&item->value.lng.high, sizeof(item->value.lng.high), 1, class_file.file);
			fread(&item->value.lng.low, sizeof(item->value.lng.low), 1, class_file.file);
			item->value.lng.high = be32toh(item->value.lng.high);
			item->value.lng.low = be32toh(item->value.lng.low);
			break;
		case DOUBLE:
			fread(&item->value.dbl.high, sizeof(item->value.dbl.high), 1, class_file.file);
			fread(&item->value.dbl.low, sizeof(item->value.dbl.low), 1, class_file.file);
			item->value.dbl.high = be32toh(item->value.dbl.high);
			item->value.dbl.low = be32toh(item->value.dbl.low);
			break;
		case CLASS:
			fread(&ref.class_idx, sizeof(ref.class_idx), 1, class_file.file);
			ref.class_idx = be16toh(ref.class_idx);
			item->value.ref = ref;
			break;
		case STRING:
			fread(&ref.class_idx, sizeof(ref.class_idx), 1, class_file.file);
			ref.class_idx = be16toh(ref.class_idx);
			item->value.ref = ref;
			break;
		case FIELD:
		case METHOD:
		case INTERFACE_METHOD:
			fread(&ref.class_idx, sizeof(ref.class_idx), 1, class_file.file);
			fread(&ref.name_idx, sizeof(ref.name_idx), 1, class_file.file);
			ref.class_idx = be16toh(ref.class_idx);
			ref.name_idx = be16toh(ref.name_idx);
			item->value.ref = ref;
			break;
		case NAME:
			fread(&ref.class_idx, sizeof(ref.class_idx), 1, class_file.file);
			fread(&ref.name_idx, sizeof(ref.name_idx), 1, class_file.file);
			ref.class_idx = be16toh(ref.class_idx);
			ref.name_idx = be16toh(ref.name_idx);
			item->value.ref = ref;
			break;
		default:
			printf("unknown tag %d \n", tag_byte);
			item = NULL;
			break;
		}
		if(item){
			class->items[i] = *item;
		}
	}

}

void print_class(FILE *stream, const Class *class){
	fprintf(stream, "File: %s\n", class->file_name);
	fprintf(stream, "Minor version: %u\n", class->minor_version);
	fprintf(stream, "Major version: %u\n", class->major_version);
	fprintf(stream, "Constant pool size: %u\n", class->const_pool_count);

	Item *item;
	uint16_t i = 1;
	while(i < class->const_pool_count){
		item = get_item(class, i-1);
		fprintf(stream, "Item #%u, %s: ", i, tag2Str(item->tag));
		if(item->tag == STRING_UTF8){
			fprintf(stream, "%s\n", item->value.string.value);
		}else if(item->tag == INTEGER){
			fprintf(stream, "%d\n", item->value.integer);
		}else if(item->tag == FLOAT){
			fprintf(stream, "%f\n", item->value.flt);
		}else if(item->tag == LONG){
			fprintf(stream, "%ld\n", to_long(item->value.lng));
		}else if(item->tag == DOUBLE){
			fprintf(stream, "%lf\n", to_double(item->value.dbl));
		}else if(item->tag == STRING || item->tag == CLASS){
			fprintf(stream, "%u\n", item->value.ref.class_idx);
		}else if(item->tag == FIELD || item->tag == METHOD || item->tag == INTERFACE_METHOD || item->tag == NAME){
			fprintf(stream, "%u.%u\n", item->value.ref.class_idx, item->value.ref.name_idx);
		}
		i++;
	}

	fprintf(stream, "Access flags: 0x%x\n", class->flags);
	Item *this_class_item = get_class_string(class, class->this_class);
	fprintf(stream, "This class: %s\n", this_class_item->value.string.value);
	Item *super_class_item = get_class_string(class, class->super_class);
	fprintf(stream, "Super class: %s\n", super_class_item->value.string.value);
	fprintf(stream, "Interface count: %u\n", class->interfaces_count);
	if(class->interfaces_count > 0){
		Ref *iface = class->interfaces;
		Item *interface_item_class;
		i = 0;
		while(i < class->interfaces_count){
			interface_item_class = get_item(class, iface->class_idx-1);
			Item *item = get_item(class, interface_item_class->value.ref.class_idx-1);
			fprintf(stream, "Interface %u: %s\n", i, item->value.string.value);
			i++;
			iface = class->interfaces + i;
		}
	}

	fprintf(stream, "Field count: %u\n", class->fields_count);
}

Item *get_item(const Class *class, const uint16_t idx){
	if(idx < class->const_pool_count)
		return &class->items[idx];
	else
		return NULL;
}

long to_long(const Long lng){
	return (long)(be32toh(lng.high) << 32) + be32toh(lng.low);
}

double to_double(const Double dbl){
	return -dbl.high;
}

Item *get_class_string(const Class *class, const uint16_t index){
	Item *item = get_item(class, index-1);
	return get_item(class, item->value.ref.class_idx-1);
}


















