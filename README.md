Features not implemented:


Features implemented:
1. Built-in commands
2. Non-built-in commands
3. Redirecting I/O with NBIC
4. Using pipes with NBIC
5. Running NBI in background
6. Using Pipes an I/O redirect with NBIC
7. Enviroment variable expansion
8. Alias expansion
9. Wilcard matching
10. Error messages
11. Tilde expansion
12. File name completion

README:
JOSEPH:
     The built-in commands were added by myself, Joseph Madden, except for bye. I utilized an initial structure that Nicholas provided with the lexer and parser to construct those features. The environment and aliases both utilize C++ maps to assign placeholders to either commands, environments, or just general strings. The print commands iterate through the associated map and print out each key/data pair. The "un-" commands utilize the erase feature in maps to get rid of the associated pair. I utilize a recursive function to check for loops in the alias map. It will use the find command to keep navigating nested aliases until it either reaches the end of the map or returns to its original position. I also used the C library function getenv to add the pointers to HOME and PATH before the shell begins parsing.
NICHOLAS:
	I got the lexer and the parser up and functional as a starting point for the project. I also implemented the non built in commands and structured the lexer and the parse to tokenize and parse the commands. The parser reads the commands in a recursive fassion until it reaches a newline character, ending the command. I created a triple pointer to store the commands in which repersents a table of commands with the first column being the name of the commands and the following colums as arguments. Each row in the table repersents a new command. Because the parser flows in a recursive function I had to implement a reverse function to reverse the order of the arguments for each command. The list of commands is then read by a loop which searches the PATH for the command and when it is found the command path and arguments are passed to execv.
	I Implemented I/O with all of the commands i do this by setting a boolean to true if input or output redirection is needed. These bolleans are checked before fork and execv and the appropriate file descriptors are redirecte to the commands or output files. Pipes were also implemented by creating a pipe for intermediate commands and redirectiong the input and output to the pipes using dup and dup2. runnning commands in the background was done by checking if there was a & token at the end and if there was the parent would skip waiting on its children.  

JOSEPH:
	The expansion was my contribution. I added the functions and logic in the lexer that replace yytext using unput to create the correct input. I also added wildcard matching via the glob command in the C library. Tilde features was a simple addition once I got wildcard matching going. I configured the glob function to correctly add directory and filenames that fit the profile. My last contribution was composing most of this README for theh dcoumentation.
