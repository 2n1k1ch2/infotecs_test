#pragma once

#include <string>
#include <vector>
struct TestCase{
    std::string name;
    void (*test_func)();
};
class TestRunner {
    private:
    static std::vector<TestCase> cases;
    public:
    static void add_test(const std::string& name, void (*test_func)() ) {
        cases.push_back({name,test_func});
    }
};