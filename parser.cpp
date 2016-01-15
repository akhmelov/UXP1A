

typedef pair<int, string> tokenS;
typedef vector<tokenS>::iterator itS;
typedef pair<bool, vector<tokenS>> resultS;

typedef pair<bool, tokenS> nameS; //<isFound, token>
typedef tuple<bool, tokenS, int> cmdS; //<isFound, token, whatFound>

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
                if(it->first != 4) throw runtime_error("Expected 4 but found: " + it->second);
                comm.isChildShell = true;
            }else{
                cmds(it, comm);
            }
        }

        static void cmds(itS &it, comm_str &comm){
            cmd(it, comm);
        }

        static void cmd(itS &it, comm_str &comm){
            name(it, comm);
        }

        static nameS name(itS &it, comm_str &comm){
            nameS nameSt(false, tokenS());
            if(it->first == 1){
                nameSt = nameS(true, *it);
                it++;
            }
            return nameSt;
        }


        static void passWhiteTokens(itS &it){
            while(it->first == 13)
                ++it;
        }

        /*
        * end (koniec) - 0
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
