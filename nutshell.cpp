#include <stdio.h>
#include <string>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include "tables.h"

ENV_MAP env_mapper;
ALIAS_MAP alias_mapper;

char* HOME_POINTER = getenv("HOME");
char* PATH_POINTER = getenv("PATH");

std::string h = "HOME";
std::string p = "PATH";

std::string HOME_STRING(HOME_POINTER);
std::string PATH_STRING(PATH_POINTER);

void print_prompt(){
	printf("%s", "THIS_ IS_THE_PROMPT>");
}

int yyparse();
int yylex();
extern char* yytext;

int main(){

	PATH_STRING.append(":");
	PATH_STRING.append(get_current_dir_name());
	
	env_mapper.variables.emplace(h, HOME_STRING);
	env_mapper.variables.emplace(p, PATH_STRING);

	int a = 1;

	printf("\033[0;33m");
	printf("%s", "---Welcome to the Nutshell! written by Joe and Nick---\n"); 
	printf("\033[0m");

	printf("\033[0;33m");
	printf("%s", "nutshell command prompt>"); 
	printf("\033[0m");

	while(a > 0)
	{
		a = yyparse();
	}

	return(1);
}

/*
shell_init()
{
    //char* cmd_table = malloc(20 * 25 * sizeof(char)); // command can be of size 20 and there can be 25 of them
    //char* alias_table = malloc(20 * 25 * sizeof(char)); // alias can be of size 20 and there can be 25 of them

    std::map<std::string, std::string> alias_map;
	//

    const char* HOME_POINTER = getenv("HOME"); //
    char* path_pointer = getenv("PATH");
    //kill()
}
*/