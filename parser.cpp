

typedef pair<int, string> tokenS;
typedef vector<tokenS>::iterator itS;
typedef pair<bool, vector<tokenS>> resultS;

typedef pair<bool, string> nameS; ///<isFound, name>
typedef pair<bool, string> parameterS; ///<isFound, name>
typedef tuple<bool, string, int, bool> redirectionS; ///<isFound, name, what, isInput> what 0 - to file, -1 - pipe, -2 - redirect to another process
typedef pair<bool, command_str> cmdS; ///<isFound, command>

class Parser {
    public:

        static vector<command_str> parse(char line[1024], comm_str &comm){
            vector<command_str> & commands = comm.commands;

            std::cout.flush();
            vector<tokenS> tokens = getTokens(string(line));
            itS it = tokens.begin();
            passWhiteTokens(it);

            try {
                refCmd(it, comm);
            }
            catch (std::exception &e) {
                cout<<"Caught exception: "<<e.what()<<"\n";
            }

            /*
            for(std::vector<tokenS>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
                std::cout.flush();
                int n = ((*it).first);
                string name = (*it).second;
                std::cout << n << " - " + name << endl;
            }*/

            return commands;
        };


    private:

        static void refCmd(itS &it, comm_str &comm){
            tokenS token = *it;
            if(token.first == 3){
                it++;
                cmds(it, comm);
                if(it->first != 4) throw runtime_error("Expected 4 ')' but found: " + it->second);
                comm.isChildShell = true;
            }else{
                cmds(it, comm);
            }
        }

        static void cmds(itS &it, comm_str &comm){
            cmdS cmdSt = cmd(it, comm);
            if(cmdSt.first == true){   //found command, looking for next commands
                passWhiteTokens(it);
                while(it->first == 5){
                    it++;
                    cmdSt = cmd(it, comm);
                    if(cmdSt.first == false) throw runtime_error("Expected command but found: " + it->second);
                    passWhiteTokens(it);
                }
            }
        }

        static cmdS cmd(itS &it, comm_str &comm){
            cmdS cmdSt(false, command_str());
            command_str command;
            nameS nameSt = name(it, comm);
            if(nameSt.first == true){ //found name of command
                command.name = nameSt.second;

                do{ //find parameters or redirections
                    redirectionS redSt = redirection(it, comm);
                    if(get<0>(redSt) == true){
                        if(get<3>(redSt) == true){//input
                            command.we.push_back(pair<int, string>(get<2>(redSt), get<1>(redSt)));
                        }else{//outpu
                            command.wy.push_back(pair<int, string>(get<2>(redSt), get<1>(redSt)));
                        }
                    }else{
                        parameterS parameterSt = parameter(it, comm);
                        if(parameterSt.first == true){
                            command.args.push_back(parameterSt.second);
                        }else{
                            break;
                        }
                    }
                }while(true);
                cmdSt.first = true;
                cmdSt.second = command;
                comm.commands.push_back(command);
            }
            return cmdSt;
        }

        static parameterS parameter(itS &it, comm_str &comm){
            parameterS parameterSt(false, "");
            passWhiteTokens(it); //in case of quotation, name has its own
            nameS nameSt = name(it, comm);
            if(nameSt.first == true){    //found name
                parameterSt = parameterS(true, nameSt.second);
            }else{ //looking for characters
                if(it->first == 6){
                    it++;
                    nameSt = characters(it, comm);
                    if(it->first != 6)  throw runtime_error("Expected 6 (') but found: " + it->second);
                    it++;
                    if(nameSt.first == true){
                        parameterSt.first = true;
                        parameterSt.second = "'";
                        parameterSt.second += nameSt.second;
                        parameterSt.second += "'";
                    }
                }
            }
            return parameterSt;
        }

        static redirectionS redirection(itS &it, comm_str &comm){
            redirectionS redSt(false, "", -1, false);
            itS itBegin = it;
            passWhiteTokens(it);
            if(it->first == 2){ //there is number at the begining
                get<2>(redSt) = stoi(it->second);
                it++;
            }
            if(it->first == 9){ //redirect output
                get<0>(redSt) = true; ///TODO
                get<3>(redSt) = false;
                it++;
                nameS nameSt = name(it, comm);
                if(nameSt.first == true)
                    get<1>(redSt) = nameSt.second;
            }else if(it->first == 10){ // redirect input
                get<0>(redSt) = true; ///TODO
                get<3>(redSt) = true;
                it++;
                nameS nameSt = name(it, comm);
                if(nameSt.first == true)
                    get<1>(redSt) = nameSt.second;
            }else{ //there is not redirection back iterator
                it = itBegin;
            }
            return redSt;
        }

        static nameS name(itS &it, comm_str &comm, bool isPassWhite = true){
            nameS nameSt(false, "");
            if(isPassWhite)
                passWhiteTokens(it);
            while(it->first == 1 || it->first == 2){
                nameSt.first = true;
                nameSt.second += it->second;
                it++;
            }
            return nameSt;
        }

        static nameS characters(itS &it, comm_str &comm){
            nameS nameSt(false, "");
            string str(it->second);
            while(it->first != 6){ //we copy what ever but not ', because of the end; while not ', we copy everything apart from ', ' it's end
                nameSt.first = true;
                nameSt.second += it->second;
                it++;
            }
            return nameSt;
        }


        static void passWhiteTokens(itS &it){
            while(it->first == 13)
                ++it;
        }

        /**
        * end (koniec) - 0 ($)
        * litery - 1,
        * liczby - 2,
        * ( - 3, ) - 4, | - 5, ' - 6, ` - 7, & - 8, > - 9, < - 10, ? - 11, : - 12, space - 13
        */
        static vector<tokenS> getTokens(string cmd){
            cmd.erase(std::remove(cmd.begin(), cmd.end(), '\n'), cmd.end());
            vector<tokenS> tokens;
            string letters = "-qwertyuiopasdfghjklzxcvbnQWERTYUIOPASDFGHJKLZXCVBNM";
            string digitals = "1234567890";
            for(unsigned int i = 0; i < cmd.size(); i++){
                tokenS token;
                string c = cmd.substr(i, 1);
                std::size_t found = letters.find(c);
                if(found!=std::string::npos){
                    tokenS token;
                    for(unsigned int j = i; j < cmd.size() && letters.find(cmd.substr(j, 1))!=std::string::npos; j++, i++){
                        string c = cmd.substr(j , 1);
                        token.first = 1;
                        token.second += string(c);
                    }
                    tokens.push_back(token);
                    i--; continue;
                }
                found = digitals.find(c);
                if(found!=std::string::npos){
                    for(unsigned int j = i; j < cmd.size() && digitals.find(cmd.substr(j, 1))!=std::string::npos; j++, i++){
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
            tokens.push_back(tokenS(0, "$")); //end
            return tokens;
        }
};
