%{
#include "parser.tab.h"
#include <glob.h>
#include "tables.h"
int wordCount = 0;
extern bool processing_word;
extern bool quoted;
glob_t glob_result;
bool aliasing = false;
bool in_string = false;
bool is_escape = false;
extern int split;
%}

%option noyywrap
%x expect_string
%x expect_unalias
%x expect_unsetenv
%x expect_pathing
%x escape_processing
%x escape_character

QUOTE ["]
NOTQUOTE [^"]
DIGIT [0-9]
WORD [a-zA-Z\/\.\\\<\>\|\"\\\&]+
PATTERN [a-zA-Z\/\.?*~]+ 
ESCAPE [a-zA-Z\/\.\\\<\>\&\|\"]
QUOTED [a-zA-Z\/\.:()<>\|\\& ]+
IO >>|<<|>|<|2>>|2>
%%

{QUOTE}                         { BEGIN(expect_string); return META; }
<expect_string>$\{              { BEGIN(expect_pathing); return META; }
<expect_string>{NOTQUOTE}*      {
yylval.WORD = new std::string(yytext);
wordCount++;
return WORD;
}
<expect_string>{QUOTE}          { BEGIN(INITIAL); return META; }

{ESCAPE} { BEGIN(INITIAL); std::string* temp = new std::string(yytext); temp->erase(remove(temp->begin(), temp->end(), '\\'), temp->end());
yylval.WORD = temp; return WORD; }

[ \t\r]	
2>&1				{ return APP_STDERR; }
>>				{ return APP_STDOUT; }
2>				{ return OW_STDERR; }
[>]				{ return OW_STDOUT; }
[<]				{ return OW_STDIN; }
&				{ return BACK; }
\|				{ return PIPE; }
-[A-Za-z0-9]+			{ wordCount++; yylval.FLAG = new std::string(yytext); return FLAG; }	
$\{                     	{ BEGIN(expect_pathing); return META; }
<expect_pathing>{WORD}* 	{
    auto it = env_mapper.variables.find(yytext);
    auto it_backup = alias_mapper.aliases.find(yytext);
    if (it != env_mapper.variables.end() && !processing_word)
    {
        wordCount++;
        yylval.WORD = new std::string(it->second);
        return WORD;
    }
    else if(it_backup != alias_mapper.aliases.end())
    {
        wordCount++;
        yylval.WORD = new std::string(it_backup->second);
        return WORD;
    }
    else if(!processing_word)
    {
        wordCount++;
        yylval.WORD = new std::string(yytext);
        return WORD;
    }
    processing_word = false;
}
<expect_pathing>\} {BEGIN(INITIAL); return META;}
setenv				{ wordCount++; yylval.SETENV = new std::string(yytext); return SETENV; }
cd		                { wordCount++; yylval.CD = new std::string(yytext); return CD; }
printenv            		{ wordCount++; yylval.PRINTENV = new std::string(yytext); return PRINTENV; }
alias               		{ aliasing = true; wordCount++; yylval.ALIAS = new std::string(yytext); return ALIAS; }
unsetenv               		{ wordCount++; BEGIN(expect_unsetenv); yylval.UNSETENV = new std::string(yytext); return UNSETENV; }
<expect_unsetenv>{WORD}+        { BEGIN(INITIAL); yylval.WORD = new std::string(yytext); return WORD;}
unalias               		{ wordCount++; BEGIN(expect_unalias); yylval.UNALIAS = new std::string(yytext); return UNALIAS;}
<expect_unalias>{WORD}+         { wordCount++; BEGIN(INITIAL); yylval.WORD = new std::string(yytext); return WORD;}
bye|exit|quit|\Z		{ wordCount++; yylval.BYE = new std::string(yytext); return BYE; }
{WORD}				{ 
    BEGIN(escape_processing);
    if(std::string(yytext).find('\\') != 0)
    {
        is_escape = true;
    }
    auto it = alias_mapper.aliases.find(yytext);
    if (it != alias_mapper.aliases.end() && wordCount == 0 && !processing_word)
    {
        if(0 == 0)
        {
            processing_word = true;
            wordCount++;
            for (std::string::iterator string_it=it->second.end() -1; string_it!=it->second.begin(); --string_it)
            {
                unput(*string_it);
            }
            unput(*it->second.begin());
        }
        else
        {
            wait(NULL);
        }
    }
    else if(!processing_word)
    {
        BEGIN(INITIAL);
        wordCount++;
        if(is_escape)
        {
            std::string* clean_str = new std::string(yytext);
            clean_str->erase( remove(clean_str->begin(), clean_str->end(), '\\'), clean_str->end());
            yylval.WORD = clean_str;
            return WORD;
        }
    }
    processing_word = false;
    BEGIN(INITIAL);
}

{PATTERN}*     			{ 
    std::string pattern = std::string(yytext);
    int success = glob(pattern.c_str(), GLOB_TILDE, NULL, &glob_result);
    std::vector<std::string> filenames;
    std::string* temp;
    if(success == 0 && (!aliasing))
    {
        for(int i = 0; i < glob_result.gl_pathc; ++i)  //the gl_pathc member is the number of valid filenames
        {
            temp = new std::string(glob_result.gl_pathv[i]);
            for (std::string::iterator string_it=temp->end() -1; string_it!=temp->begin(); --string_it)
            {
                unput(*string_it);
            }
            unput(*temp->begin());
            unput(' ');
        }
    }
    else
    {
        wordCount++;
        yylval.WORD = new std::string(yytext);
        aliasing = false;
        return WORD;
    }
}

\n				{ wordCount = 0; yylval.NEW_LINE = new std::string(yytext); return NEW_LINE; }
\0				{ yylval.NULL_TOKEN = new std::string(yytext); return NULL_TOKEN; }
.				return NOT_A_TOKEN;
