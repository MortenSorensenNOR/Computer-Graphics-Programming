#include <stdlib.h>
#include <time.h>
#include <filesystem>  

#include "application/application.h"

int main(int argc, char** argv) {
    srand(time(0));
    
    std::filesystem::path root = std::filesystem::current_path();
    std::filesystem::path assets = root.parent_path() / "assets/";

    Application application(1280, 720, "Computer Graphics Programming", assets.string());
    application.run();

    return 0;
}
