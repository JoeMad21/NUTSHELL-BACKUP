#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <iterator>
#include <vector>
#include <map>
#include <algorithm>

struct ENV_MAP
{
    std::map <std::string, std::string> variables;
};

struct ALIAS_MAP
{
    std::map <std::string, std::string> aliases;
};

extern ENV_MAP env_mapper;
extern ALIAS_MAP alias_mapper;

static std::string parser_catch;

extern char* HOME_POINTER;
extern char* PATH_POINTER;
