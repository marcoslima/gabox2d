#include <GaBox2dApp.h>
#include <worker/worker.h>

int main(const int argc, char *argv[])
{
    GUI::CGaBox2dApp theApp;
    if (argc == 2)
    {
        do_work(argv[1]);
        return 0;
    }
    return theApp.run();
}
