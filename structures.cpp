struct command_str {
    string name; // command name
    vector<string> args; // command args
    /* for we and wy
     * int = -1 -> pipe
     * int = -2 -> redirect
     * int > 0  -> filename in string
     */
    vector<pair<int, string>> we;
    vector<pair<int, string>> wy;

    command_str(){};
    command_str(const command_str &a){
        this->name = a.name;
        this->args = a.args;
        this->we = a.we;
        this->wy = a.wy;
    }
};

struct comm_str {
    vector<command_str> commands;
    vector<string> redirected;
    bool isBackground;
    bool isChildShell;
    int retValue;
};
