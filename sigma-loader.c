/**
 * Copyright (C) 2017 PolyVection UG
 * 
 * 
 *
 * THIS SOFTWARE IS PROVIDED BY POLYVECTION "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *
 **/

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>

#include "sigma-loader.h"

#ifdef LIBXML_TREE_ENABLED

int is_name = 0;
int is_addr = 0;
int is_len = 0;
int is_data = 0;

char * name = NULL;
char * data = NULL;
char  buf_char[1024]; 
uint8_t val;
int addr = 0;
int len = 0;


static const struct backend_ops debug_backend_ops = {
	.read = NULL,
	.write = NULL,
};

static const struct backend_ops *backend_ops = &debug_backend_ops;


/*
static void printArray(uint8_t *a, int skip, int len) {
    for (int i = skip; i < skip+len; i++){
	 printf("p[%d]: %02x ", i,a[i]);
	}
}
*/

static void
print_element_names(xmlNode * a_node)
{
	
	uint8_t *val;
	size_t buf_size;
	buf_size = 1024;
	val = malloc(buf_size);

    	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
	        
			if (strcmp((char *)cur_node->name, "Name") == 0){
				name = (char *)xmlNodeGetContent(cur_node); 
				is_name=1;
			}
			if (strcmp((char *)cur_node->name, "Address") == 0){              
                        	addr = atoi((char *)xmlNodeGetContent(cur_node));
                        	is_addr=1;                             
               		} 
			if (strcmp((char *)cur_node->name, "Size") == 0){              
                      		len = atoi((char *)xmlNodeGetContent(cur_node)); 
                        	is_len=1;                             
                	} 
			if (strcmp((char *)cur_node->name, "Data") == 0){              
                        	data = (char *)xmlNodeGetContent(cur_node); 
                        	is_data=1;
				char delimiter[] = ",x ";
				char *ptr;
				ptr = strtok(data, delimiter);
				int i = 0;
				int b = 0;

				while(ptr != NULL) {
					if(i % 2){
						strcpy(buf_char+b, ptr);
						b=b+2;
					}
 					ptr = strtok(NULL, delimiter);
					i++;
				}

				size_t count = 0;
				const char *pos = buf_char;
				for(count = 0; count < len; count++) {
        				sscanf(pos, "%2hhx", &val[count]);
        				pos += 2;
    				}
                        
                	} 

			if (is_name&&is_addr&&is_len&&is_data){
				is_name=0;
				is_data=0;
				is_len=0;
				is_addr=0;
				backend_ops->write(addr, len, val);
			}   
	    	}

        	print_element_names(cur_node->children);
    	}
}


int main(int argc, char **argv){

    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    if (argc != 2)
        return(1);

    LIBXML_TEST_VERSION

    doc = xmlReadFile(argv[1], NULL, 0);

    if (doc == NULL) {
        printf("error: could not parse file %s\n", argv[1]);
    }

    int ret = 0;
    char *i2c[] = {"dsp", "i2c", "/dev/i2c-0", "0x38"};

    backend_ops = &i2c_backend_ops;
    if (backend_ops->open) {
		ret = backend_ops->open(4, i2c);
		if (ret) exit(1);
	}

    root_element = xmlDocGetRootElement(doc);

    print_element_names(root_element);
    xmlFreeDoc(doc);
    printf("Success.\n");  
    xmlCleanupParser();

    return 0;
}

#else
int main(void) {
    fprintf(stderr, "Tree support not compiled in\n");
    exit(1);
}
#endif
