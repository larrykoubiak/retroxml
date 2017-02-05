#include <stdio.h>
#include <stdlib.h>
#include "retroxml.h"

int main(int argc, char** argv) {
	if(argc < 2) {
		printf("Usage:\n%s <xmlfile> [args ...]\n",*argv);
		exit(1);
	}
	argc--;
	argv++;
	return parse_xml_file(*argv);
}