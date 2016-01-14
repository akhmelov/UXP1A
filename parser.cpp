#include <iostream>
#include <vector>
#include <regex>

struct command_str {
    string name;
    vector<string> args;
    pair<int, string> we;
    pair<int, string> wy;
    bool isBackground;

    command_str(){};
    command_str(const command_str &a){
        this->name = a.name;
        this->args = a.args;
        this->we = a.we;
        this->wy = a.wy;
        this->isBackground = a.isBackground;
    }
    /*command(string name, vector<string> args, pair<int, string> we, pair<int, string> wy, bool isBackground = false){
        this->name = name;
        this->args = args;
        this->we = we;
        this->wy = wy;
        this->isBackground = isBackground;
    }*/
};

class Parser {
    public:
    static vector<command_str> parse(char line[1024], vector<command_str> &commands){
        char* cmd = line;
		char* next = strchr(cmd, '|'); /* Find first '|' */
        while (next != NULL){
            command_str myCmd;
			/* 'next' points to '|' */
			*next = '\0';
			run_parse_cmd(cmd, myCmd);
			//input = run(cmd, input, first, 0);

			cmd = next + 1;
			next = strchr(cmd, '|'); /* Find next '|' */
			commands.push_back(myCmd);
		}
		command_str myCmd;
		run_parse_cmd(cmd, myCmd);
		commands.push_back(myCmd);

		return commands;
    };

    private:
    static int run_parse_cmd(char* cmd, command_str &myCmd)
    {
        parse_cmd(cmd, myCmd);
        /*
        if (myCmd.name == "exit")
            exit(0);
        else if (myCmd.name == "cd") {	//for 'cd' command support, which is not executable file
            if(chdir((myCmd.args[1].c_str())) == -1) {
                printf("Error occured while changing directory to %s\n", (myCmd.args[1].c_str()));
            }
        }*/
        return 0;//command(input, first, last);
    }

    static char* skipwhite(char* s)
    {
        while (isspace(*s)) ++s;
        return s;
    }

    static void parse_cmd(char* cmd, command_str &myCmd)
    {
        string cmdStr(cmd);
        std::regex quotationReg("(sub)(.*)");
        if (std::regex_match (cmdStr,quotationReg)){
            cmdStr = trimWhiteFromBegin(cmdStr);
            myCmd.args.push_back(string(cmdStr.c_str()));
        }else{
            cmd = skipwhite(cmd);
            char* next = strchr(cmd, ' ');
            int i = 0;

            while(next != NULL) {
                next[0] = '\0';
                //args[i] = cmd;
                if(i == 0)
                    myCmd.name = string(cmd);
                else
                   parse_parameter(cmd, myCmd);
                ++i;
                cmd = skipwhite(next + 1);
                next = strchr(cmd, ' ');
            }

            if (cmd[0] != '\0') {
                //args[i] = cmd;
                next = strchr(cmd, '\n');
                next[0] = '\0';
                if(i == 0)
                    myCmd.name = string(cmd);
                else
                    parse_parameter(cmd, myCmd);
                ++i;
            }
        }
        //args[i] = NULL;
    }

    static void parse_parameter(char *cmdPar, command_str &myCmd){
        string cmd(cmdPar);
        std::regex quotationReg("(sub)(.*)");
        if (std::regex_match (cmd,quotationReg))
            myCmd.args.push_back(string(cmd.c_str()));
        else
            myCmd.args.push_back(string(cmd.c_str()));
    }

    static string trimWhiteFromBegin(string str)
    {
        int i = 0;
        for (char c : str)
        {
            if (!isspace(c))
                break;
            i++;
        }

        string trimmed = str.substr(i, (str.length()-i));

        return trimmed;

    }
};
