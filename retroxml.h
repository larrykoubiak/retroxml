#ifndef RETROXML_H__
#define RETROXML_H__

typedef enum {
	XML_NODE_ELEMENT,
	XML_NODE_ATTRIBUTE,
	XML_NODE_TEXT,
} xml_node_type_enum;

typedef struct {
	xml_node_type_enum node_type;
	const char* label;
	int line_no;
	int column;
} xml_token_t;

typedef struct {
	xml_token_t* tokens;
	int count;
	int capacity;
} xml_token_list_t;

int parse_xml_file(const char* path);


#endif