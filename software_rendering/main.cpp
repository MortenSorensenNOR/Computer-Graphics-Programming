#include <stdlib.h>
#include <time.h>

#include "application/core_utils/core_utils.h"
#include "application/application.h"

int main(int argc, char** argv) {
    srand(time(0));

    Application_t app;
    int err = application_init(&app, 1920, 1080, "Computer Graphics Programming");
    if (err) {
        printf("Could not initialize application\n");
        return 1;
    }

    application_run(&app);

    application_free(&app);

    return 0;
}
