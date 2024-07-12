#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

#include "bar.h"
#include "configManager.h"

void CliHelp(const std::string&){
    std::cout << "MiBar argument help\n\n" \
    "\t-h shows the current help menu\n" \
    "\t-c changes the target config file name (not format). EXAMPLE: ./MiBar -c secondBar\n";
}

bool Cli(int argc, char** argv){
    if(argc < 2) return true; // only arg was ./mibar

    std::unordered_map<std::string, std::function<void(const std::string&)>> argMap = {
        {"-h", CliHelp},
        {"-c", [](const std::string& subArg){std::cout << "Target config file > $HOME/.config/MiBar/" << subArg << '\n';}},
    };

    std::vector<std::string> args(argv + 1, argc + argv);

    for(int i = 0; i < args.size(); ++i){
        const auto& arg = args[i];
        auto iter = argMap.find(arg);

        if(iter != argMap.end()){
            if(arg == "-c" && i + 1 < args.size()) iter->second(args[++i]);
            else iter->second("");
        }
        else{
            std::cerr << "Invalid argument: " << arg << std::endl;
            return false;
        }
    }

    return true;
}

int main(int argc, char** argv){
    if(!Cli(argc, argv)) return 1;

    configManager cfg;

    mibar bar;

    bar.EventLoop();

    return 0;
}