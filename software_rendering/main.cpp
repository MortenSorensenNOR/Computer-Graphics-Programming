#include <stdlib.h>
#include <time.h>
#include <filesystem>  

#include "application/application.h"

int main(int argc, char** argv) {
    srand(time(0));
    
    std::filesystem::path root = std::filesystem::current_path();
    std::filesystem::path assets = root.parent_path() / "assets/";

    Application_t app;
    int err = application_init(&app, 854, 480, "Computer Graphics Programming", assets.string());
    if (err) {
        printf("Could not initialize application\n");
        return 1;
    }

    application_run(&app);

    application_free(&app);

    return 0;
}
