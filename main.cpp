#include <iostream>

#include "async.h"
#include "config.h"
#include <chrono>
#include <thread>

#define SLEEP_INTERVAL 100

int main(int, char *[])
{
    std::size_t bulk = 5;
#ifdef BYTE_BY_BYTE
    auto h = async::connect(bulk);
    auto h2 = async::connect(bulk);

    int variant = 1;

    switch (variant) {
    case 0:
    {
        std::string hData("1\n2\n3\n4\n5\n6\n{\na\nb\nc\nd\n}\n89\n");
        for(char& c : hData)
        {
            async::receive(h, &c, 1);
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_INTERVAL));
        }

        std::string h2Data("1\n");
        for(char& c : h2Data)
        {
            async::receive(h2, &c, 1);
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_INTERVAL));
        }
        break;
    }
    case 1:
    {
        async::receive(h,"0",1);
        async::receive(h," ",1);
        async::receive(h,"0",1);
        async::receive(h,"\n",1);
        async::receive(h,"\n",1);
        async::receive(h,"1",1);
        async::receive(h," ",1);
        async::receive(h,"1",1);
        async::receive(h,"\n",1);
        async::receive(h,"2",1);
        async::receive(h,"\n",1);
        break;
    }
    default:
        break;
    }

    async::disconnect(h);
    async::disconnect(h2);

#else
    auto h = async::connect(bulk);
    auto h2 = async::connect(bulk);
    async::receive(h, "1", 1);
    async::receive(h2, "1\n", 2);
    async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    async::receive(h, "b\nc\nd\n}\n89\n", 11);
    async::disconnect(h);
    async::disconnect(h2);
#endif

    return 0;
}
