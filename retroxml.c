#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "retroxml.h"

static xml_token_list_t* xml_token_list_create(void) {
	xml_token_list_t* list = malloc(sizeof(*list));
	list->count = 0;
	list->capacity = 1;
	list->tokens = (xml_token_t*)malloc(sizeof(*list->tokens) * list->capacity);
	return list;
}

static void xml_token_list_free(xml_token_list_t* list) {
	if(!list)
		return;
	free(list->tokens);
	free(list);
}

static void xml_token_list_append(xml_token_list_t* dst, void* item) {
	dst->capacity++;
	dst->tokens = realloc(dst->tokens, sizeof(*dst->tokens) * dst->capacity);
	xml_token_t* token = (xml_token_t*) item;
	dst->tokens[dst->count] = *token;
	dst->count++;
}

static xml_token_list_t* xml_lexer(char* src) {
	xml_token_list_t* list = xml_token_list_create();
	xml_token_t token = {XML_NODE_ELEMENT,NULL,1,1};
	char in_element = 0;
	char in_quote = 0;
	while (*src) {
		if(*src == '<') {
			*src = '\0';
			src++;
			token.column++;
			token.label = NULL;
			token.node_type = XML_NODE_ELEMENT;
			in_element = 1;
			continue;
		}
		if(*src == '>') {
			*src = '\0';
			src++;
			token.column++;
			token.label = NULL;
			token.node_type = XML_NODE_TEXT;
			in_element = 0;
			continue;
		}
		if((in_element==1 && in_quote==0) && (*src == ' ' || *src == '\t' || *src == '\r' || *src == '/')) {
			token.node_type = XML_NODE_ATTRIBUTE;
			token.label = NULL;
			*src= '\0';
			src++;
			token.column++;
			continue;
		}
		if(*src == '"') {
			*src= '\0';
			src++;
			token.column++;
			in_quote = !in_quote;
			token.label = NULL;
/*			if(in_quote) {
				token.label = src;
				xml_token_list_append(list, &token);
			}*/
			continue;
		}
		if (*src == '\n') {
			*src = '\0';
			src++;
			while(*src==' ' || *src=='\t')
				src++;
			token.column = 1;
			token.line_no++;
			token.label = NULL;
			continue;
		}
		if(!token.label) {
			token.label = src;
			xml_token_list_append(list, &token);
		}
		src++;
		token.column++;
	}
	xml_token_list_append(list, &token);
	return list;
}

int parse_xml_file(const char* path) {
	FILE* xml_file;
	size_t xml_file_size;
	char* xml_buffer;
	int i;
	char* node_type_text;
	xml_token_list_t* token_list = NULL;
	xml_token_t token;
	xml_file = fopen(path,"r");
	if(!xml_file) {
		printf("Could not open xml file '%s': %s\n",path,strerror(errno));
		return 1;
	}	
	fseek(xml_file,0,SEEK_END);
	xml_file_size = ftell(xml_file);
	fseek(xml_file,0,SEEK_SET);
	xml_buffer = (char*)malloc(xml_file_size+1);
	fread(xml_buffer,1,xml_file_size, xml_file);
	fclose(xml_file);
	xml_buffer[xml_file_size] = '\0';
	token_list = xml_lexer(xml_buffer);
	for(i=0;i<token_list->count;i++) {
		token = token_list->tokens[i];
		switch(token.node_type) {
			case XML_NODE_ATTRIBUTE:
				node_type_text = "ATTRIBUTE";
				break;
			case XML_NODE_ELEMENT:
				node_type_text = "ELEMENT";
				break;
			case XML_NODE_TEXT:
				node_type_text = "TEXT";
				break;
			default:
				break;
		}
		printf("Line:%i Column:%i Type:%s Value:%s\n",token.line_no,token.column,node_type_text,token.label);
	}
	xml_token_list_free(token_list);
	free(xml_buffer);
	return 0;
}