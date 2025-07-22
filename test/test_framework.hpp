#pragma once
#include <iostream>
#include <string>
#include <vector>

struct TestCase {
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
        const auto& all_tests = tests();
        const int total_tests = all_tests.size();

        for (const auto& test : all_tests) {
            std::cout << "[ RUN      ] " << test.name << std::endl;
            try {
                test.test_func();
                std::cout << "[       OK ] " << test.name << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "[  FAILED  ] " << test.name << " - " << e.what() << std::endl;
                failed++;
            }
        }

        // Вывод итоговой статистики
        std::cout << "\n[==========] " << total_tests << " tests ran.\n";
        std::cout << "[  PASSED  ] " << (total_tests - failed) << " tests.\n";
        if (failed > 0) {
            std::cerr << "[  FAILED  ] " << failed << " tests.\n";
        }

        // Возвращаем количество упавших тестов (0 = успех)
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