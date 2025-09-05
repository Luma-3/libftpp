#include <thread>
#include <vector>

#include "thread_safe_iostream.hpp"

int main(int argc, char* argv[])
{
    ThreadSafeIO.setPrefix("");
    ThreadSafeIO << "No prefix message" << std::endl;
    ThreadSafeIO.flush();

    return 0;
}
