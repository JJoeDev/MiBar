#include <iostream>

#include "Application.h"

int main(){
    bar::AppParams params;
    params.AppName = "MiBar";
    
    bar::Application app(params);

    std::cin.get();
    
    return 0;
}
