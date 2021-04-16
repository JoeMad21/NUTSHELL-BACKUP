%{
#include "parser.tab.h"
#include "tables.h"

bool quoted = false;
bool processing_word = false;

int yylex();
int yyparse();

int alias_nutshell(std::string* variable, std::string* word);
int alias_print_nutshell(void);
int unalias_nutshell(std::string* variable);

bool map_loop_check(std::map<std::string, std::string>::iterator it_original,  std::map<std::string, std::string>::iterator it_travel,  std::map<std::string, std::string>& the_map);

int env_nutshell(std::string* variable, std::string* word);
int env_print_nutshell(void);
int unenv_nutshell(std::string* variable);
int cd_nutshell(std::string* address);

void yyerror(const char* e){
	printf("Error: %s\n", e);
};

int ind = 0;
int exeSuccess = -1;
int cmd_num = 0;
char*** cmd_table = (char***)malloc(cmd_num * sizeof(char**));
int* argc_list = (int*)malloc(cmd_num * sizeof(int));
char** input_arr; 
char* cmd_name;
char* path;
bool end_input = false;

bool app_stderr = false;
bool app_stdout = false;
bool ow_stderr = false;
bool ow_stdout = false;
bool ow_stdin = false;
bool back = false;
char* out_file;
char* in_file;

void reverse(int argc, char** argv)
{
    for (int low = 0, high = argc - 1; low < high; low++, high--)
    {
        char* temp = (char*)malloc((strlen(argv[low]) + 1) * sizeof(char));
	strcpy(temp, argv[low]);
	argv[low] = (char*)realloc(argv[low], (strlen(argv[high]) + 1) * sizeof(char));
	strcpy(argv[low], argv[high]);
	argv[high] = (char*)realloc(argv[high], (strlen(temp) + 1) * sizeof(char));
	strcpy(argv[high], temp);
    }
}

void cleanArgs(int argc, char** argv) {

	for(int i = 0; i < argc; i++) {
		free(argv[i]);
	} 

	free(argv);
	ind = 0;
}

void clean(){
	for (int i = 0; i < cmd_num; i++){
		cleanArgs(argc_list[i], cmd_table[i]);
	}
	free(argc_list);
	free(cmd_table);
	if (ow_stdin)
		free(in_file);
	if (ow_stdout || app_stdout)
		free(out_file);
	cmd_num = 0;
	end_input = false;
	app_stderr = false;
	app_stdout = false;
	ow_stderr = false;
	ow_stdout = false;
	ow_stdin = false;
	back = false;
	//printf("debug 8\n");
	cmd_table = (char***)malloc(cmd_num * sizeof(char**));
	argc_list = (int*)malloc(cmd_num * sizeof(int));
}

void execute(){
	//cmd table should be ready to go
	//printf("debug 1.1\n");
	
	int temp_in = dup(0);
	int temp_out = dup(1);
	int temp_err = dup(2);	
	int fd_in;
	int fd_out;
	int fd_err;
	int a;
	exeSuccess = -1;

	//set input redirection
	if(ow_stdin){
		//printf("debug 1.1.1\n");
		fd_in = open(in_file,O_RDONLY);
	} else {
		//printf("debug 1.1.2\n");
		fd_in = dup(temp_in);
	}
	
	//printf("debug 1.2\n");

	for (int i = 0; i < cmd_num; i++){
		path = (char*)malloc((strlen(cmd_table[i][0]) + strlen("/bin/") + 1) * sizeof(char));
		strcpy(path, "/bin/");
		strcat(path, cmd_table[i][0]);
		//printf("debug 1.3\n");
		//int status = -1;

		dup2(fd_in, 0);
		close(fd_in);

		//check for last command
		if (i == cmd_num - 1){
			//check output redirection
			if (ow_stdout){
				fd_out = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
				if (app_stderr) {
					fd_err = dup(fd_out);
				}
				//printf("debug 1.4\n");
			} else if (app_stdout){
				fd_out = open(out_file, O_WRONLY | O_APPEND);
				if (app_stderr) {
					fd_err = dup(fd_out);
				}
				//printf("debug 1.5\n");
			} else if (ow_stderr){
			   	//printf("debug 1.6\n");
				fd_err = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
				fd_out = dup(temp_out);
			} else {
				fd_out = dup(temp_out);
				fd_err = dup(temp_err);
			}
			//printf("debug 2\n");
		} else {
			int p[2];
			pipe(p);
			fd_out = p[1];
			fd_in = p[0];
		}
		//printf("debug 3.0\n");
		dup2(fd_out, 1);
		dup2(fd_err, 2);
		//printf("debug 3.1\n");
		close(fd_out);
		close(fd_err);
		//printf("debug 3.2\n");
	
		//search dirs for command 
		//printf("debug 6.%d\n",i);				
		a = fork();
		if( a == 0) {
			std::map<std::string,std::string>::iterator it = env_mapper.variables.find("PATH");
			std::string s = it->second;
			std::string delimiter = ":";
			size_t pos = 0;
			std::string token;
			while ((pos = s.find(delimiter)) != std::string::npos) {
				token = s.substr(0, pos);
				if (token.back() != '/')
					 token = token + '/';
				token = token + std::string(cmd_table[i][0]);
				//printf("debug 4\n");
				execv(token.c_str(), cmd_table[i]);			
				s.erase(0, pos + delimiter.length());
			}
			token = s + std::string(cmd_table[i][0]);
			execv(token.c_str(), cmd_table[i]);			
			if (execv(cmd_table[i][0], cmd_table[i]) == -1){
				printf("Error: command not found\n");
				exit(0);
			}
			

		} else if (!back) {
			waitpid(a, NULL, 0);
		}
		//printf("debug 7.%d\n",i);				
		free(path);
	}
	
	//printf("debug 10\n");				
	dup2(temp_in, 0);
	dup2(temp_out,1);
	close(temp_in);
	close(temp_out);
	clean();	
}

void printPrompt(){
	printf("\033[0;33m");
	printf("%s", "nutshell command prompt>"); 
	printf("\033[0m");
}

%}

%code requires {
#include <string>
}

%define api.value.type union

%token<std::string*>	BACK PIPE APP_STDERR APP_STDOUT OW_STDERR OW_STDOUT APP_STDIN OW_STDIN FLAG SETENV PRINTENV BYE WORD NEW_LINE TILDE META CD ALIAS UNSETENV UNALIAS NULL_TOKEN HOME PATH NOT_A_TOKEN
%type<char*> 	setenv
//%type<char*>	built_in

%%

input:	  /* empty */			{ 

}		
     	| input line
	;

line:  	  NEW_LINE			{ printPrompt();}
	| PIPE	 			{}
	| WORD command			{
//printf("debug 3\n");
cmd_table[cmd_num] = (char**)realloc(cmd_table[cmd_num], (ind + 1) * sizeof(char*));
cmd_table[cmd_num][ind] = (char*)malloc((strlen($1->c_str()) + 1) * sizeof(char));
strcpy(cmd_table[cmd_num][ind], (char*)$1->c_str());
ind ++;
argc_list[cmd_num] = ind;
if((!quoted) && (!processing_word) && end_input)
{
	cmd_num ++;
	cmd_name = (char*)malloc((strlen($1->c_str()) + 1) * sizeof(char));
	strcpy(cmd_name, (char*)$1->c_str());
	//printf("debug 11\n\n");
	
	//reverse
	for (int i = 0; i < cmd_num; i++){
		reverse(argc_list[i], cmd_table[i]);
		cmd_table[i] = (char**)realloc(cmd_table[i], (argc_list[i] + 1) * sizeof(char*));
		cmd_table[i][argc_list[i]] = NULL;
	}

	/*
	printf("cmd_num: %d\n", cmd_num);
	for (int i = 0; i < cmd_num; i++){
		printf("command %d has %d args\n",i,argc_list[i]);
		for (int j = 0; j < argc_list[i]; j++){
			printf("%s\n", cmd_table[i][j]);
		}
	}
	printf("ow_stdout: %d\n", ow_stdout);
	printf("ow_stdin: %d\n", ow_stdin);
	//printf("%s\n", out_file);
	printf("\n");
	*/
	execute();

	printPrompt();
} else  {
	//printf("debug 9\n");
	cmd_num ++;
	ind = 0;
}

} 
	| SETENV setenv
	| CD cd
	| PRINTENV printenv
	| ALIAS alias
	| UNSETENV unsetenv
	| UNALIAS unalias
	| NOT_A_TOKEN
	| BYE 				{ printf("%s", "thanks for using nutshell\n"); free(cmd_table); free(argc_list); exit(0); }	
;

command:  NEW_LINE			{ 
//printf("debug 1 cmd_num %d\n", cmd_num);
argc_list = (int*)realloc(argc_list, (cmd_num + 1) * sizeof(int));
cmd_table = (char***)realloc(cmd_table, (cmd_num + 1) * sizeof(char**));
cmd_table[cmd_num] = (char**)malloc(ind * sizeof(char*)); 
end_input = true;      
}	
	| APP_STDERR command		{
app_stderr = true;
}
	| OW_STDERR WORD command	{
ow_stderr = true;
out_file = (char*)malloc(strlen($2->c_str()) * sizeof(char));
strcpy(out_file, (char*)$2->c_str());
}
	| OW_STDOUT WORD command	{
ow_stdout = true;
out_file = (char*)malloc(strlen($2->c_str()) * sizeof(char));
strcpy(out_file, (char*)$2->c_str());
}
	| APP_STDOUT WORD command	{
app_stdout = true;
out_file = (char*)malloc(strlen($2->c_str()) * sizeof(char));
strcpy(out_file, (char*)$2->c_str());
}
	| OW_STDIN WORD command		{
ow_stdin = true;
in_file = (char*)malloc(strlen($2->c_str()) * sizeof(char));
strcpy(in_file, (char*)$2->c_str());
}
	| BACK	command			{
back = true;
}
	| PIPE				{
//printf("debug 2 cmd_num %d\n", cmd_num);
argc_list = (int*)realloc(argc_list, (cmd_num + 1) * sizeof(int));
cmd_table = (char***)realloc(cmd_table, (cmd_num + 1) * sizeof(char**));
cmd_table[cmd_num] = (char**)malloc(ind * sizeof(char*)); 
}
	| WORD command			{
cmd_table[cmd_num] = (char**)realloc(cmd_table[cmd_num],  (ind + 1) * sizeof(char*));
cmd_table[cmd_num][ind] = (char*)malloc((strlen($1->c_str()) + 1) * sizeof(char));
strcpy(cmd_table[cmd_num][ind], (char*)$1->c_str());
ind ++;
}
	| FLAG command			{
cmd_table[cmd_num] = (char**)realloc(cmd_table[cmd_num],  (ind + 1) * sizeof(char*));
cmd_table[cmd_num][ind] = (char*)malloc((strlen($1->c_str()) + 1) * sizeof(char));
strcpy(cmd_table[cmd_num][ind], (char*)$1->c_str());
ind ++;
}
	| NOT_A_TOKEN command
;

setenv:	  NEW_LINE			{ printf("Error: syntax error.\n"); }
        | WORD WORD			{ env_nutshell($1,$2); }
      	|				{ printf("Error: please enter a variable and a value.\n"); }
;

cd: 	  WORD				{ cd_nutshell($1); }
	| META WORD META		{ cd_nutshell($2); }
	| META WORD NEW_LINE		{ printf("Error: syntax error.\n"); }
	| WORD META NEW_LINE		{ printf("Error: syntax error.\n"); }
	| META NEW_LINE			{ printf("Error: syntax error.\n"); }
  	| 				{ printf("Error: enter a directory.\n"); }
;

alias:	  WORD WORD			{ alias_nutshell($1,$2); }
	| io				{ alias_print_nutshell(); }
      	| WORD NEW_LINE			{ printf("Error: please enter another value.\n"); }
;

printenv: io				{ env_print_nutshell();}
;

unsetenv: WORD				{ unenv_nutshell($1); }
	| 				{ printf("Error: enter an alias."); }
;

unalias:  WORD				{ unalias_nutshell($1); }
       	| NEW_LINE			{ printf("Error: enter an alias."); }
;

io: 
  	| APP_STDOUT WORD io		{ 
app_stdout = true;
out_file = (char*)malloc(strlen($2->c_str()) * sizeof(char));
strcpy(out_file, (char*)$2->c_str());
}
	| OW_STDOUT WORD io		{
ow_stdout = true;
out_file = (char*)malloc(strlen($2->c_str()) * sizeof(char));
strcpy(out_file, (char*)$2->c_str());
}

%%

int alias_nutshell(std::string* variable, std::string* word)
{
	std::pair<std::map<std::string,std::string>::iterator,bool> ret;
	if(*variable == *word)
	{
		std::cout << "Illegal alias, would cause infinite loop." << std::endl;
		return 1;
	}
  	ret = alias_mapper.aliases.insert ( std::pair<std::string,std::string>(*variable, *word) );
  	if (ret.second==false) {
       		std::cout << "That alias is already in use" << std::endl;
	}
	auto it_original = alias_mapper.aliases.find(*variable);
	auto it_travel = alias_mapper.aliases.find(*word);
	auto& the_map = alias_mapper.aliases;
	bool is_loop;
	for (std::map<std::string,std::string>::iterator it=alias_mapper.aliases.begin(); it!=alias_mapper.aliases.end(); ++it)
	if(it->second == *variable)
	{
		is_loop = map_loop_check(it_original, it_travel, the_map);
	}

	if(is_loop)
	{
		std::cout << "This alias would cause an infinite loop." << std::endl;
		alias_mapper.aliases.erase(*variable);
	}

	return 1;
}

bool map_loop_check(std::map<std::string, std::string>::iterator it_original,  std::map<std::string, std::string>::iterator it_travel,  std::map<std::string, std::string>& the_map)
{
	if(it_travel == the_map.end())
	{
		return false;
	}
	else if((it_travel->first == it_original->first) &&(it_travel->second == it_original->second))
	{
		return true;
	}
	else
	{
		it_travel = the_map.find(it_travel->second);
		map_loop_check(it_original, it_travel, the_map);
	}
	return NULL;
}

int alias_print_nutshell()
{
	int temp_out = dup(1);
	int fd_out;

	//open fie
	if(app_stdout){
		fd_out = open(out_file, O_WRONLY | O_APPEND);
	} else if(ow_stdout) {
		fd_out = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	} else {
		fd_out = dup(temp_out);
	}
	dup2(fd_out, 1);
	close(fd_out);

	if(alias_mapper.aliases.size() == 0)
	{
		std::cout << "You haven't entered any aliases" << std::endl;
		return 1;
	}

	for (std::map<std::string,std::string>::iterator it=alias_mapper.aliases.begin(); it!=alias_mapper.aliases.end(); ++it)
    		std::cout << it->first <<  "=" << it->second << '\n';

	if (app_stdout || ow_stdout){
		dup2(temp_out, 1);
		free(out_file);
		app_stdout = false;
		ow_stdout = false;
	}
	return 1;
}

int env_nutshell(std::string* variable, std::string* word)
{
	if(*variable == "PATH")
	{
		auto it = env_mapper.variables.find("PATH");
		it->second.append(":");
		it->second.append(*word);
		return 1;
	}
	std::pair<std::map<std::string,std::string>::iterator,bool> ret;
  	ret = env_mapper.variables.insert ( std::pair<std::string,std::string>(*variable, *word) );
  	if (ret.second==false) {
    		std::cout << "That name is already in use" << std::endl;
	}
	auto it_original = env_mapper.variables.find(*variable);
	auto it_travel = env_mapper.variables.find(*word);
	auto& the_map = env_mapper.variables;
	bool is_loop;
	for (std::map<std::string,std::string>::iterator it=env_mapper.variables.begin(); it!=env_mapper.variables.end(); ++it)
	if(it->second == *variable)
	{
		is_loop = map_loop_check(it_original, it_travel, the_map);
	}

	if(is_loop)
	{
		std::cout << "This alias would cause an infinite loop." << std::endl;
		env_mapper.variables.erase(*variable);
	}
	return 1;
}

int cd_nutshell(std::string* address)
{
	int result = chdir(address->c_str());
	if(result == -1)
	{
		std::cout << "Not a valid directory path, will remain in current directory." << std::endl;
	}
	return 1;
}

int env_print_nutshell()
{
	int temp_out = dup(1);
	int fd_out;

	//open fie
	if(app_stdout){
		fd_out = open(out_file, O_WRONLY | O_APPEND);
	} else if(ow_stdout) {
		fd_out = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	} else {
		fd_out = dup(temp_out);
	}
	dup2(fd_out, 1);
	close(fd_out);

	if(env_mapper.variables.size() == 0)
	{
		std::cout << "You have somehow removed all the environment variables!" << std::endl;
		return 1;
	}

	for (std::map<std::string,std::string>::iterator it=env_mapper.variables.begin(); it!=env_mapper.variables.end(); ++it)
		std::cout << it->first << "=" << it->second << '\n';

	if (app_stdout || ow_stdout){
		dup2(temp_out, 1);
		free(out_file);
		app_stdout = false;
		ow_stdout = false;
	}
	return 1;
}

int unalias_nutshell(std::string* variable)
{
	auto it = alias_mapper.aliases.find( *variable );
	if(it == alias_mapper.aliases.end())
	{
		std::cout << "This alias does not exist." << std::endl;
	}
	else
	{
		alias_mapper.aliases.erase( *variable );
	}
	return 1;
}

int unenv_nutshell(std::string* variable)
{
  	auto it = env_mapper.variables.find( *variable );
	if(it == env_mapper.variables.end())
	{
		std::cout << "This variable does not exist." << std::endl;
	}
	else
	{
		env_mapper.variables.erase( *variable );
	}
	return 1;
}
