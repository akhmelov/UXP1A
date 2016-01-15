

typedef pair<int, string> tokenS;

class Parser {
    public:

    static vector<command_str> parse(char line[1024], comm_str &comm){
        vector<command_str> & commands = comm.commands;
        /*string character;
        string cmd(line);
        cmd.erase(std::remove(cmd.begin(), cmd.end(), '\n'), cmd.end());

        try {
            refCmd(cmd, comm);
        }
        catch (std::exception &e) {
            cout<<"Caught exception: "<<e.what()<<"\n";
        }

        passFirstWhitespaces(cmd);
        character = cmd[0];
        if(character == "&")
            comm.isBackground = true;
*/
        std::cout.flush();
        vector<tokenS> tokens = getTokens(string(line));
        for(std::vector<tokenS>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
            std::cout.flush();
            int n = ((*it).first);
            string name = (*it).second;
            std::cout << n << " - " + name << endl;
        }

        /*char* cmd = line;
		char* next = strchr(cmd, '|');
        while (next != NULL){
            command_str myCmd;

			*next = '\0';
			run_parse_cmd(cmd, myCmd);
			//input = run(cmd, input, first, 0);

			cmd = next + 1;
			next = strchr(cmd, '|');
			commands.push_back(myCmd);
		}
		command_str myCmd;
		run_parse_cmd(cmd, myCmd);
		commands.push_back(myCmd);
        */
		return commands;
    };


    private:
        /*
        * litery - 1,
        * liczby - 2,
        * ( - 3, ) - 4, | - 5, ' - 6, ` - 7, & - 8, > - 9, < - 10, ? - 11, : - 12, space - 13
        */
        static vector<tokenS> getTokens(string cmd){
            vector<tokenS> tokens;
            string letters = "qwertyuiopasdfghjklzxcvbnQWERTYUIOPASDFGHJKLZXCVBNM";
            string digitals = "1234567890";
            int tokenState = -1;
            for(int i = 0; i < cmd.size(); i++){
                tokenS token;
                string c = cmd.substr(i, 1);
                std::size_t found = letters.find(c);
                if(found!=std::string::npos){
                    tokenS token;
                    for(int j = i; j < cmd.size() && letters.find(cmd.substr(j, 1))!=std::string::npos; j++, i++){
                        string c = cmd.substr(j , 1);
                        token.first = 1;
                        token.second += string(c);
                    }
                    tokens.push_back(token);
                    i--; continue;
                }
                found = digitals.find(c);
                if(found!=std::string::npos){
                    for(int j = i; j < cmd.size() && digitals.find(cmd.substr(j, 1))!=std::string::npos; j++, i++){
                        string c = cmd.substr(j, 1);
                        token.first = 2;
                        token.second += string(c);
                    }
                    tokens.push_back(token);
                    i--; continue;
                }
                switch(cmd[i]){
                    case '(': token.first = 3; token.second += c; break;
                    case ')': token.first = 4; token.second += c; break;
                    case '|': token.first = 5; token.second += c; break;
                    case '\'': token.first = 6; token.second += c; break;
                    case '`': token.first = 7; token.second += c; break;
                    case '&': token.first = 8; token.second += c; break;
                    case '>': token.first = 9; token.second += c; break;
                    case '<': token.first = 10; token.second += c; break;
                    case '?': token.first = 11; token.second += c; break;
                    case ':': token.first = 12; token.second += c; break;
                    case ' ': token.first = 13; token.second += c; break;
                    default: token.first = -1; token.second += c;
                }
                tokens.push_back(token);
            }
            return tokens;
        }

    static void refCmd(string &refCmd, comm_str &comm){
        string c;
        passFirstWhitespaces(refCmd);
        c = refCmd[0];
        if(c == "("){
            refCmd.erase(0, 1);

            cmds(refCmd, comm);

            passFirstWhitespaces(refCmd);
            c = refCmd[0];
            if(c != ")"){
                throw std::runtime_error("Expected ')' found: '" + c + "' ");
            }else {
                refCmd.erase(0, 1);
            }
        }else{
            cmds(refCmd, comm);
        }
    }

    static void cmds(string &cmd, comm_str &comm){
        string c;
        vector<command_str> & commands = comm.commands;

        command_str command;
        cmd1(cmd, comm, command);
        commands.push_back(command);

        passFirstWhitespaces(cmd);
        c = cmd[0];
        while(c == "|"){
            cmd.erase(0, 1);

            command_str command;
            cmd1(cmd, comm, command);
            commands.push_back(command);

            passFirstWhitespaces(cmd);
            c = cmd[0];
        }
    }

    static void cmd1(string &cmd, comm_str &comm, command_str &command){
        string c;
        passFirstWhitespaces(cmd);

        string name = nameFun(cmd, comm);
        if(name != ""){
            command.name = name;
        }else{
            string specialP = special(cmd, comm);
            if(specialP != ""){
                command.args.push_back(specialP);
            }
            vector<command_str> & commands = comm.commands;
            command_str command;
            cmd1(cmd, comm, command);
            commands.push_back(command);
        }
    }

    static string special(string &cmd, comm_str &comm){
        string c;
        string special;
        passFirstWhitespaces(cmd);
        c = cmd[0];
        if(c == "'"){
            string str;
            do{
                str += c;
                cmd.erase(0, 1);
                c = cmd[0];
                if(cmd.back() == c[0] && c != "'")
                    throw std::runtime_error("Expected ''' found: '" + c + "' ");
            }while(c != "'");
            c = cmd[0];
            str += c;
            special = str;
        }else if(c == "`"){
            string str;
            do{
                str += c;
                cmd.erase(0, 1);
                c = cmd[0];
                if(cmd.back() == c[0] && c != "`")
                    throw std::runtime_error("Expected '`' found: '" + c + "' ");
            }while(c != "'");
            //parse(string(c), comm);  ///TODO
        }else if(c == "" || c == " "){
            return special;
        }else{
            throw std::runtime_error("Expected ' or ` found: '" + c + "' ");
        }
    }

    static string nameFun(string &cmd, comm_str &comm){
        passFirstWhitespaces(cmd);
        char c = cmd[0];
        cmd += " ";
        string name = "";
        string nameAllowChars = "qwertyuiopasdfghjklzxcvbnm1234567890QWERTYUIOPASDFGHJKLZXCVBNM";
        std::size_t found = cmd.find_first_not_of(nameAllowChars);
        if(found != std::string::npos)
        {
            name = cmd.substr(0, found);
            cmd = cmd.substr(found, (cmd.length()-found));
        }else{
            name = cmd;
        }
        return name;
    }

    //tuple<isRead, descriptorN, FileName>
    static tuple<bool, int, string> redirect(string &cmd, comm_str &comm){

    }

    static void error(string msg){
        std::cout.flush();
        std::cout << "->>>> Error: " + msg + " <<<<-" << std::endl;
    }


    static void passFirstWhitespaces(string &text){
        std::size_t found = text.find_first_not_of(' ');
        if(found != std::string::npos)
        {
            string trimmed = text.substr(found, (text.length()-found));
            text = trimmed;
        }
    }













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
