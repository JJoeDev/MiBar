#include <getopt.h>

#include "bar.h"

const char* helpList = "MiBar Help List\n\n-h or --help to show this message again\n-c or --config to change the default config file to a user-defined config file [ -c Monitor2 ]\n";

int main(int argc, char** argv){
    std::string userCfgFile = "";
    int option;

    while((option = getopt(argc, argv, ":hc:")) != -1){
        switch(option){
        case 'h':
            std::cout << helpList;
            return 0;
        case 'c':
            userCfgFile = optarg;
            break;
        case ':':
            std::cout << "Option needs a value\n";
            break;
        case '?':
            std::cout << "Unknown option: " << std::string(optarg) << '\n';
        default:
            return 0;
        }
    }

    MiBar bar(userCfgFile);
    bar.EventLoop();

    return 0;
}
