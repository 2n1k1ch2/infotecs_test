#pragma once

#include <string>
#include <vector>
struct TestCase{
    std::string name;
    void (*test_func)();
};
class TestRunner {
public:
    static void register_test(const std::string& name, void (*test_func)()) {
        tests().push_back({name, test_func});
    }

    static int run_all() {
        int failed = 0;
        for (const auto& test : tests()) {
            std::cout << "[ RUN      ] " << test.name << std::endl;
            try {
                test.test_func();
                std::cout << "[       OK ] " << test.name << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "[  FAILED  ] " << test.name << " - " << e.what() << std::endl;
                failed++;
            }
        }
        return failed;
    }

private:
    static std::vector<TestCase>& tests() {
        static std::vector<TestCase> instance;
        return instance;
    }
};

#define TEST(test_name) \
void test_name##_impl(); \
namespace { \
struct test_name##_registrar { \
    test_name##_registrar() { \
        TestRunner::register_test(#test_name, &test_name##_impl); \
    } \
} test_name##_instance; \
} \
void test_name##_impl()