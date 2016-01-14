

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
