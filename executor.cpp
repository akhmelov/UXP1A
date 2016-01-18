
const int ARGS_ARRAY_SIZE = 512;

static char* args[ARGS_ARRAY_SIZE];
static char* argsInitCopy[ARGS_ARRAY_SIZE];

string ret; ///returns answer (output of process)

class MyExecutor{

    public:

        static string run(comm_str &comm){

            ret = "";
            vector<command_str> & commands = comm.commands;
            int n = 0; //how many process are running

            int input = 0;
            int first = 1;
            int last = 0;

            stringstream buffer;

            for(std::vector<command_str>::iterator it = commands.begin(); it != commands.end(); ++it) {
                for(int i = 0; i < ARGS_ARRAY_SIZE; i++) //reset array
                    args[i] = argsInitCopy[i];

                if((it + 1 == commands.end()))
                    last = 1;
                //char* args[512];
                args[0] = const_cast<char*>((*it).name.c_str());

                int i = 1;
                for(std::vector<string>::iterator itargs = (*it).args.begin(); itargs != (*it).args.end(); ++itargs) {
                    args[i] = const_cast<char*>((*itargs).c_str());
                    i++;
                }

                input = run_command(comm, *it, args, input, first, last);
                n++;
                first = 0;
            }

            if(!comm.isBackground)
                cleanup(comm, n); //wait for end process

            return ret;
        }

    private:
        static int run_command(comm_str &comm, command_str cmd, char* args[512], int input, int first, int last){
            //basic command
            if (cmd.name == "exit")
                exit(0);
            else if (cmd.name == "cd") {	//for 'cd' command support, which is not executable file
                if(chdir((cmd.args[0].c_str())) == -1) {
                    printf("Error occured while changing directory to %s\n", (cmd.args[0].c_str()));
                }
            }
            return command(comm, cmd, input, first, last);
        }

        static int command(comm_str &comm, command_str cmd, int input, int first, int last)
        {
            int pipettes[2];
            pid_t pid;

            /* Invoke pipe */
            pipe( pipettes );
            pid = fork();

            /*
             SCHEME:
                STDIN --> O --> O --> O --> STDOUT
            */

            if (pid == 0) {
                if (first == 1 && last == 0 && input == 0) {
                    // First command
                    dup2( pipettes[WRITE], STDOUT_FILENO );
                } else if (first == 0 && last == 0 && input != 0) {
                    // Middle command
                    dup2(input, STDIN_FILENO);
                    dup2(pipettes[WRITE], STDOUT_FILENO);
                } else {
                    // Last command
                    dup2( input, STDIN_FILENO );
                    if((comm.isInversedQ || comm.isCondition) && !comm.isBackground){
                        dup2(pipettes[1], 1);  // send stdout to the pipe
                        //dup2(pipettes[WRITE], STDOUT_FILENO);
                    }
                }

                if (execvp(args[0], args) == -1)
                    _exit(EXIT_FAILURE); // If child fails

            }else if((comm.isInversedQ) && !comm.isBackground && last == 1){
                char buffer[1024];
                read(pipettes[0], buffer, sizeof(buffer));
                ret += string(buffer);
            }

            if (input != 0)
                close(input);

            // Nothing more needs to be written
            close(pipettes[WRITE]);

            // If it's the last command, nothing more needs to be read
            if (last == 1)
                close(pipettes[READ]);

            return pipettes[READ];
        }

        static void cleanup(comm_str &comm, int n)
        {
            int statval;
            if(comm.isCondition)
                ret = "";
            for (int i = 0; i < n; ++i){
                if(comm.isCondition){
                    wait(&statval);
                    if(WIFEXITED(statval))
                        if(WEXITSTATUS(statval) != 0)
                            ret += "error";
                }
                else
                    wait(NULL);
            }
        }
};
