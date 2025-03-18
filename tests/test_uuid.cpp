#include <uuid/uuid.h>
#include <iostream>

using namespace std;

void assert(const bool condition, const char *title, const char *message)
{
    const auto esc_green = "\033[1;32m";
    const auto esc_red = "\033[1;31m";
    const auto esc_reset = "\033[0m";
    cout << esc_green << esc_red << esc_reset << title << ": ";
    if (!condition)
    {
        cout << esc_red << " failed: " << esc_reset << message << endl;
    }
    else
    {
        cout << esc_green << " passed" << endl;
    }
}

void test_uuid_are_different()
{
    const auto title = "uuids are different";
    uuid_t uuid1, uuid2;

    uuid_generate(uuid1);
    uuid_generate(uuid2);

    const bool result = uuid_compare(uuid1, uuid2) != 0;
    assert(result, title, "uuids are different failed");
}

void test_uuid_are_equal()
{
    const auto title = "uuids are equal";
    uuid_t uuid1, uuid2;

    uuid_generate(uuid1);
    uuid_copy(uuid2, uuid1);

    const bool result = uuid_compare(uuid1, uuid2) == 0;
    assert(result, title, "uuids are equal failed");
}

int main()
{
    test_uuid_are_different();
    test_uuid_are_equal();
    return 0;
}
