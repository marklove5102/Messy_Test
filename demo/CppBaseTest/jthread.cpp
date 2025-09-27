#include <thread>
#include <iostream>
#include <chrono>
#include <format>

// Blog: https://blog.csdn.net/fengbingchun/article/details/152172174

using namespace std::chrono_literals;

int test_jthread_constructor()
{
    auto func1 = [](int val) {
        for (auto i = 0; i < 2; ++i) {
            std::cout << ++val << std::endl;
            std::this_thread::sleep_for(100ms);
        }
    };

    auto func2 = [](int& val) {
        val = 88;
    };

    std::jthread jth1(func1, 5);
    std::jthread jth2{};
    std::cout << std::format("jth1 joinable: {}; jth2 joinable: {}", jth1.joinable(), jth2.joinable()) << std::endl;
    std::cout << "jth1 id: " << jth1.get_id() << "; jth2  id: " << jth2.get_id() << std::endl;

    jth2 = std::move(jth1);
    std::cout << std::format("jth1 joinable: {}; jth2 joinable: {}", jth1.joinable(), jth2.joinable()) << std::endl;
    std::cout << "jth1 id: " << jth1.get_id() << "; jth2  id: " << jth2.get_id() << std::endl;

    int val{ -1 };
    std::jthread jth3(func2, std::ref(val));
    jth3.join();
    std::cout << std::format("jth3 joinable: {}, val: {}", jth3.joinable(), val) << std::endl;
    std::cout << "jth3 id: " << jth3.get_id() << std::endl;

    std::cout << "concurrent threads are supported: " << std::jthread::hardware_concurrency() << std::endl;

    std::jthread jth4([] {
        std::cout << "jth4 running ..." << std::endl;
        std::this_thread::sleep_for(1s);
        std::cout << "jth4 end" << std::endl;
    });
    std::cout << "jth4 joinable: " << jth4.joinable() << ", id: " << jth4.get_id() << std::endl;

    jth4.detach();
    std::this_thread::sleep_for(3s);
    std::cout << "jth4 joinable: " << jth4.joinable() << ", id: " << jth4.get_id() << std::endl;

    jth4.swap(jth2);
    std::cout << std::format("jth2 joinable: {}; jth4 joinable: {}", jth2.joinable(), jth4.joinable()) << std::endl;

    std::swap(jth2, jth4);
    std::cout << std::format("jth2 joinable: {}; jth4 joinable: {}", jth2.joinable(), jth4.joinable()) << std::endl;

    return 0;
}

int test_jthread_request_stop()
{
    std::jthread jth([](std::stop_token token) {
        while (!token.stop_requested()) {
            std::cout << "running ...\n";
            std::this_thread::sleep_for(500ms);
        }
        std::cout << "stopped\n";
    });

    std::this_thread::sleep_for(3s);
    std::cout << "jth joinable: " << jth.joinable() << ", id: " << jth.get_id() << std::endl;
    jth.request_stop();
    std::this_thread::sleep_for(1s);
    std::cout << "jth joinable: " << jth.joinable() << ", id: " << jth.get_id() << std::endl;

	return 0;
}
