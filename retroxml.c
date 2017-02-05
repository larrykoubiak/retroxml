#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

typedef enum {
	XML_NODE_ELEMENT,
	XML_NODE_ATTRIBUTE,
	XML_NODE_TEXT,
} xml_node_type_enum;

void parse_xml_file(const char* path) {
	FILE* xml_file;
	size_t xml_file_size;
	char* xml_buffer;
	xml_file = fopen(path,"r");
	if(!xml_file) {
		printf("Could not open xml file '%s': %s\n",path,strerror(errno));
		exit(1);
	}
	fseek(xml_file,0,SEEK_END);
	xml_file_size = ftell(xml_file);
	fseek(xml_file,0,SEEK_SET);
	xml_buffer = (char*)malloc(xml_file_size+1);
	fread(xml_buffer,1,xml_file_size, xml_file);
	fclose(xml_file);
	xml_buffer[xml_file_size] = '\0';
	printf("Content: %s\n",xml_buffer);
	free(xml_buffer);
}


int main(int argc, char** argv) {
	
	if(argc < 2) {
		printf("Usage:\n%s <xmlfile> [args ...]\n",*argv);
		exit(1);
	}
	argc--;
	argv++;
	parse_xml_file(*argv);
	return 0;
}