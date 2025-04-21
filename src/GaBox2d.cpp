#include <GaBox2dApp.h>
#include <worker/worker.h>

int run_app()
{
    GUI::CGaBox2dApp theApp;
    return theApp.run();
}

int main(const int argc, char *argv[])
{
    if (argc == 2)
    {
        do_work(argv[1]);
        return 0;
    }

    return run_app();
}
