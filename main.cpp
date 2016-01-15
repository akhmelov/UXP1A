/* Compile with: g++ -Wall –Werror -o shell shell.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <iostream>
#include <vector>
#include <list>
#include <regex>

#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

#define READ  0
#define WRITE 1

#include "structures.cpp"
#include "parser.cpp"
#include "executor.cpp"

#include "help_functions.cpp"

void run(string cmd);

int main(int argc, char ** argv)
{
    static char line[1024];

	printf("SIMPLE SHELL: Type 'exit' or send EOF to exit.\n");
	while (1) {
		/* Print the command prompt */
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			printf("%s$> ", cwd);
		} else {
			printf("$> ");
		}
		fflush(NULL);

		/* Read a command line */
        if ( argc > 1) strcpy(line,argv[1]);
		else if (!fgets(line, 1024, stdin))
			return 0;

        run(string(line));

        /*Old solution
        comm_str comm;
        string answer;

        Parser::parse(line, comm);
        answer = MyExecutor::run(comm);

        Help_class::print_command_vector(comm);
        */

	}
	return 0;
}


void run(string cmd){
    comm_str comm;
    string answer;

    Parser::parse(cmd, comm);
    answer = MyExecutor::run(comm);

    Help_class::print_command_vector(comm);

    if(comm.isCondition){ //if there was condition
        cout<<comm.str1.c_str() <<" --------------- "<<comm.str2.c_str()<<endl;
        if(answer != ""){
            run(comm.str1.c_str());
        }else{
            run(comm.str2.c_str());
        }
    }
}
