


class Help_class {
    public:
        static void print_command_vector(vector<command_str> &commands){

            std::cout.flush();
            int size1 = commands.size();
            std::cout << "Size of vector: " + size1 << std::endl;


            for(std::vector<command_str>::iterator it = commands.begin(); it != commands.end(); ++it) {
                std::cout.flush();
                string name = (*it).name;
                std::cout << name;

                for(std::vector<string>::iterator itargs = (*it).args.begin(); itargs != (*it).args.end(); ++itargs) {
                    std::cout.flush();
                    string value = (*itargs);
                    std::cout <<" -->" + value + "";
                }
                std::cout << "; ";
            }

            std::cout << std::endl;

        }
};
