/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include "parser.tab.h"
#include "tables.h"

bool quoted = false;
bool processing_word = false;

int yylex();
int yyparse();

int alias_nutshell(std::string* variable, std::string* word);
int alias_print_nutshell(void);
int unalias_nutshell(std::string* variable);

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


#line 279 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 210 "parser.y"

#include <string>

#line 326 "parser.tab.c"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    BACK = 258,
    PIPE = 259,
    APP_STDERR = 260,
    APP_STDOUT = 261,
    OW_STDERR = 262,
    OW_STDOUT = 263,
    APP_STDIN = 264,
    OW_STDIN = 265,
    FLAG = 266,
    SETENV = 267,
    PRINTENV = 268,
    BYE = 269,
    WORD = 270,
    NEW_LINE = 271,
    TILDE = 272,
    META = 273,
    CD = 274,
    ALIAS = 275,
    UNSETENV = 276,
    UNALIAS = 277,
    NULL_TOKEN = 278,
    HOME = 279,
    PATH = 280,
    NOT_A_TOKEN = 281
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{

  /* setenv  */
  char* setenv;
  /* BACK  */
  std::string* BACK;
  /* PIPE  */
  std::string* PIPE;
  /* APP_STDERR  */
  std::string* APP_STDERR;
  /* APP_STDOUT  */
  std::string* APP_STDOUT;
  /* OW_STDERR  */
  std::string* OW_STDERR;
  /* OW_STDOUT  */
  std::string* OW_STDOUT;
  /* APP_STDIN  */
  std::string* APP_STDIN;
  /* OW_STDIN  */
  std::string* OW_STDIN;
  /* FLAG  */
  std::string* FLAG;
  /* SETENV  */
  std::string* SETENV;
  /* PRINTENV  */
  std::string* PRINTENV;
  /* BYE  */
  std::string* BYE;
  /* WORD  */
  std::string* WORD;
  /* NEW_LINE  */
  std::string* NEW_LINE;
  /* TILDE  */
  std::string* TILDE;
  /* META  */
  std::string* META;
  /* CD  */
  std::string* CD;
  /* ALIAS  */
  std::string* ALIAS;
  /* UNSETENV  */
  std::string* UNSETENV;
  /* UNALIAS  */
  std::string* UNALIAS;
  /* NULL_TOKEN  */
  std::string* NULL_TOKEN;
  /* HOME  */
  std::string* HOME;
  /* PATH  */
  std::string* PATH;
  /* NOT_A_TOKEN  */
  std::string* NOT_A_TOKEN;
#line 415 "parser.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   59

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  27
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  11
/* YYNRULES -- Number of rules.  */
#define YYNRULES  44
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  69

#define YYUNDEFTOK  2
#define YYMAXUTOK   281


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   222,   222,   225,   228,   229,   230,   275,   276,   277,
     278,   279,   280,   281,   282,   285,   292,   295,   300,   305,
     310,   315,   318,   324,   330,   338,   339,   340,   343,   344,
     345,   346,   347,   348,   351,   352,   353,   356,   359,   360,
     363,   364,   367,   368,   373
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "BACK", "PIPE", "APP_STDERR",
  "APP_STDOUT", "OW_STDERR", "OW_STDOUT", "APP_STDIN", "OW_STDIN", "FLAG",
  "SETENV", "PRINTENV", "BYE", "WORD", "NEW_LINE", "TILDE", "META", "CD",
  "ALIAS", "UNSETENV", "UNALIAS", "NULL_TOKEN", "HOME", "PATH",
  "NOT_A_TOKEN", "$accept", "input", "line", "command", "setenv", "cd",
  "alias", "printenv", "unsetenv", "unalias", "io", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281
};
# endif

#define YYPACT_NINF (-22)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -22,    33,   -22,   -22,    23,    -2,   -22,     7,   -22,   -13,
      19,    -8,    25,   -22,   -22,     1,   -22,   -22,     5,     9,
     -22,   -22,     7,   -22,     7,    11,    17,    29,    41,     7,
       7,   -22,   -22,    39,    27,   -22,    35,   -22,   -22,   -22,
     -22,   -22,   -22,   -22,   -22,    -2,    -2,   -22,   -22,     7,
       7,     7,     7,   -22,   -22,    42,     3,   -22,   -22,   -22,
     -22,   -22,   -22,   -22,   -22,   -22,   -22,   -22,   -22
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,     5,    27,    42,    14,     0,     4,    33,
      42,    39,     0,    13,     3,     0,    25,     7,     0,     0,
       9,    37,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    15,     6,    28,     0,     8,     0,    10,    35,    38,
      11,    40,    41,    12,    26,    42,    42,    21,    16,     0,
       0,     0,     0,    24,    23,     0,     0,    32,    34,    36,
      43,    44,    19,    17,    18,    20,    31,    30,    29
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -22,   -22,   -22,   -21,   -22,   -22,   -22,   -22,   -22,   -22,
     -10
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,    14,    32,    17,    35,    37,    20,    40,    43,
      21
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      38,    47,    33,    48,    18,    34,    19,    39,    53,    54,
      22,    23,    24,    25,    26,    27,    44,    28,    29,    67,
      45,    68,    30,    31,    46,    18,    49,    19,    62,    63,
      64,    65,    50,     2,    36,    60,    61,     3,    15,    16,
      41,    42,    56,    57,    51,     4,     5,     6,     7,     8,
      58,    59,     9,    10,    11,    12,    52,    55,    66,    13
};

static const yytype_int8 yycheck[] =
{
      10,    22,    15,    24,     6,    18,     8,    15,    29,    30,
       3,     4,     5,     6,     7,     8,    15,    10,    11,    16,
      15,    18,    15,    16,    15,     6,    15,     8,    49,    50,
      51,    52,    15,     0,    15,    45,    46,     4,    15,    16,
      15,    16,    15,    16,    15,    12,    13,    14,    15,    16,
      15,    16,    19,    20,    21,    22,    15,    18,    16,    26
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    28,     0,     4,    12,    13,    14,    15,    16,    19,
      20,    21,    22,    26,    29,    15,    16,    31,     6,     8,
      34,    37,     3,     4,     5,     6,     7,     8,    10,    11,
      15,    16,    30,    15,    18,    32,    15,    33,    37,    15,
      35,    15,    16,    36,    15,    15,    15,    30,    30,    15,
      15,    15,    15,    30,    30,    18,    15,    16,    15,    16,
      37,    37,    30,    30,    30,    30,    16,    16,    18
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    27,    28,    28,    29,    29,    29,    29,    29,    29,
      29,    29,    29,    29,    29,    30,    30,    30,    30,    30,
      30,    30,    30,    30,    30,    31,    31,    31,    32,    32,
      32,    32,    32,    32,    33,    33,    33,    34,    35,    35,
      36,    36,    37,    37,    37
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     2,     2,     2,     2,
       2,     2,     2,     1,     1,     1,     2,     3,     3,     3,
       3,     2,     1,     2,     2,     1,     2,     0,     1,     3,
       3,     3,     2,     0,     2,     1,     2,     1,     1,     0,
       1,     1,     0,     3,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 222 "parser.y"
                                        { 

}
#line 1630 "parser.tab.c"
    break;

  case 4:
#line 228 "parser.y"
                                        { printPrompt();}
#line 1636 "parser.tab.c"
    break;

  case 5:
#line 229 "parser.y"
                                        {}
#line 1642 "parser.tab.c"
    break;

  case 6:
#line 230 "parser.y"
                                        {
//printf("debug 3\n");
cmd_table[cmd_num] = (char**)realloc(cmd_table[cmd_num], (ind + 1) * sizeof(char*));
cmd_table[cmd_num][ind] = (char*)malloc((strlen((yyvsp[-1].WORD)->c_str()) + 1) * sizeof(char));
strcpy(cmd_table[cmd_num][ind], (char*)(yyvsp[-1].WORD)->c_str());
ind ++;
argc_list[cmd_num] = ind;
if((!quoted) && (!processing_word) && end_input)
{
	cmd_num ++;
	cmd_name = (char*)malloc((strlen((yyvsp[-1].WORD)->c_str()) + 1) * sizeof(char));
	strcpy(cmd_name, (char*)(yyvsp[-1].WORD)->c_str());
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
#line 1692 "parser.tab.c"
    break;

  case 14:
#line 282 "parser.y"
                                        { printf("%s", "thanks for using nutshell\n"); free(cmd_table); free(argc_list); exit(0); }
#line 1698 "parser.tab.c"
    break;

  case 15:
#line 285 "parser.y"
                                        { 
//printf("debug 1 cmd_num %d\n", cmd_num);
argc_list = (int*)realloc(argc_list, (cmd_num + 1) * sizeof(int));
cmd_table = (char***)realloc(cmd_table, (cmd_num + 1) * sizeof(char**));
cmd_table[cmd_num] = (char**)malloc(ind * sizeof(char*)); 
end_input = true;      
}
#line 1710 "parser.tab.c"
    break;

  case 16:
#line 292 "parser.y"
                                        {
app_stderr = true;
}
#line 1718 "parser.tab.c"
    break;

  case 17:
#line 295 "parser.y"
                                        {
ow_stderr = true;
out_file = (char*)malloc(strlen((yyvsp[-1].WORD)->c_str()) * sizeof(char));
strcpy(out_file, (char*)(yyvsp[-1].WORD)->c_str());
}
#line 1728 "parser.tab.c"
    break;

  case 18:
#line 300 "parser.y"
                                        {
ow_stdout = true;
out_file = (char*)malloc(strlen((yyvsp[-1].WORD)->c_str()) * sizeof(char));
strcpy(out_file, (char*)(yyvsp[-1].WORD)->c_str());
}
#line 1738 "parser.tab.c"
    break;

  case 19:
#line 305 "parser.y"
                                        {
app_stdout = true;
out_file = (char*)malloc(strlen((yyvsp[-1].WORD)->c_str()) * sizeof(char));
strcpy(out_file, (char*)(yyvsp[-1].WORD)->c_str());
}
#line 1748 "parser.tab.c"
    break;

  case 20:
#line 310 "parser.y"
                                        {
ow_stdin = true;
in_file = (char*)malloc(strlen((yyvsp[-1].WORD)->c_str()) * sizeof(char));
strcpy(in_file, (char*)(yyvsp[-1].WORD)->c_str());
}
#line 1758 "parser.tab.c"
    break;

  case 21:
#line 315 "parser.y"
                                        {
back = true;
}
#line 1766 "parser.tab.c"
    break;

  case 22:
#line 318 "parser.y"
                                        {
//printf("debug 2 cmd_num %d\n", cmd_num);
argc_list = (int*)realloc(argc_list, (cmd_num + 1) * sizeof(int));
cmd_table = (char***)realloc(cmd_table, (cmd_num + 1) * sizeof(char**));
cmd_table[cmd_num] = (char**)malloc(ind * sizeof(char*)); 
}
#line 1777 "parser.tab.c"
    break;

  case 23:
#line 324 "parser.y"
                                        {
cmd_table[cmd_num] = (char**)realloc(cmd_table[cmd_num],  (ind + 1) * sizeof(char*));
cmd_table[cmd_num][ind] = (char*)malloc((strlen((yyvsp[-1].WORD)->c_str()) + 1) * sizeof(char));
strcpy(cmd_table[cmd_num][ind], (char*)(yyvsp[-1].WORD)->c_str());
ind ++;
}
#line 1788 "parser.tab.c"
    break;

  case 24:
#line 330 "parser.y"
                                        {
cmd_table[cmd_num] = (char**)realloc(cmd_table[cmd_num],  (ind + 1) * sizeof(char*));
cmd_table[cmd_num][ind] = (char*)malloc((strlen((yyvsp[-1].FLAG)->c_str()) + 1) * sizeof(char));
strcpy(cmd_table[cmd_num][ind], (char*)(yyvsp[-1].FLAG)->c_str());
ind ++;
}
#line 1799 "parser.tab.c"
    break;

  case 25:
#line 338 "parser.y"
                                        { printf("Error: syntax error.\n"); }
#line 1805 "parser.tab.c"
    break;

  case 26:
#line 339 "parser.y"
                                        { env_nutshell((yyvsp[-1].WORD),(yyvsp[0].WORD)); }
#line 1811 "parser.tab.c"
    break;

  case 27:
#line 340 "parser.y"
                                        { printf("Error: please enter a variable and a value.\n"); }
#line 1817 "parser.tab.c"
    break;

  case 28:
#line 343 "parser.y"
                                        { cd_nutshell((yyvsp[0].WORD)); }
#line 1823 "parser.tab.c"
    break;

  case 29:
#line 344 "parser.y"
                                        { cd_nutshell((yyvsp[-1].WORD)); }
#line 1829 "parser.tab.c"
    break;

  case 30:
#line 345 "parser.y"
                                        { printf("Error: syntax error.\n"); }
#line 1835 "parser.tab.c"
    break;

  case 31:
#line 346 "parser.y"
                                        { printf("Error: syntax error.\n"); }
#line 1841 "parser.tab.c"
    break;

  case 32:
#line 347 "parser.y"
                                        { printf("Error: syntax error.\n"); }
#line 1847 "parser.tab.c"
    break;

  case 33:
#line 348 "parser.y"
                                        { printf("Error: enter a directory.\n"); }
#line 1853 "parser.tab.c"
    break;

  case 34:
#line 351 "parser.y"
                                        { alias_nutshell((yyvsp[-1].WORD),(yyvsp[0].WORD)); }
#line 1859 "parser.tab.c"
    break;

  case 35:
#line 352 "parser.y"
                                        { alias_print_nutshell(); }
#line 1865 "parser.tab.c"
    break;

  case 36:
#line 353 "parser.y"
                                        { printf("Error: please enter another value.\n"); }
#line 1871 "parser.tab.c"
    break;

  case 37:
#line 356 "parser.y"
                                        { env_print_nutshell();}
#line 1877 "parser.tab.c"
    break;

  case 38:
#line 359 "parser.y"
                                        { unenv_nutshell((yyvsp[0].WORD)); }
#line 1883 "parser.tab.c"
    break;

  case 39:
#line 360 "parser.y"
                                        { printf("Error: enter an alias."); }
#line 1889 "parser.tab.c"
    break;

  case 40:
#line 363 "parser.y"
                                        { unalias_nutshell((yyvsp[0].WORD)); }
#line 1895 "parser.tab.c"
    break;

  case 41:
#line 364 "parser.y"
                                        { printf("Error: enter an alias."); }
#line 1901 "parser.tab.c"
    break;

  case 43:
#line 368 "parser.y"
                                        { 
app_stdout = true;
out_file = (char*)malloc(strlen((yyvsp[-1].WORD)->c_str()) * sizeof(char));
strcpy(out_file, (char*)(yyvsp[-1].WORD)->c_str());
}
#line 1911 "parser.tab.c"
    break;

  case 44:
#line 373 "parser.y"
                                        {
ow_stdout = true;
out_file = (char*)malloc(strlen((yyvsp[-1].WORD)->c_str()) * sizeof(char));
strcpy(out_file, (char*)(yyvsp[-1].WORD)->c_str());
}
#line 1921 "parser.tab.c"
    break;


#line 1925 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 379 "parser.y"


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
	for (std::map<std::string,std::string>::iterator it=alias_mapper.aliases.begin(); it!=alias_mapper.aliases.end(); ++it)
	if(it->second == *variable)
	{
		alias_mapper.aliases.erase(*variable);
		std::cout << "Illegal alias, would cause infinite loop." << std::endl;
	}
	return 1;
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
