


class Help_class {
    public:
        static void print_command_vector(comm_str &comm){
            vector<command_str> & commands = comm.commands;

            std::cout.flush();
            int size1 = commands.size();
            std::cout << " Size of vector: " << size1 << std::endl;


            for(std::vector<command_str>::iterator it = commands.begin(); it != commands.end(); ++it) {
                std::cout.flush();
                string name = (*it).name;
                std::cout << name;

                for(std::vector<string>::iterator itargs = (*it).args.begin(); itargs != (*it).args.end(); ++itargs) {
                    std::cout.flush();
                    string value = (*itargs);
                    std::cout <<" p-->" + value + "";
                }
                for(std::vector<pair<int, string>>::iterator itargs = (*it).we.begin(); itargs != (*it).we.end(); ++itargs) {
                    std::cout.flush();
                    std::cout <<" ri-->" << itargs->first << "<" << itargs->second;
                }
                for(std::vector<pair<int, string>>::iterator itargs = (*it).wy.begin(); itargs != (*it).wy.end(); ++itargs) {
                    std::cout.flush();
                    std::cout <<" ro-->" << itargs->first << ">" << itargs->second;
                }
                std::cout << "; ";
            }

            std::cout << std::endl;

        }
};
