/*
 * MsTest C++ testing framework for embedded development(Cortex-M)
 *
 * Copyright 2020 Mateusz Stadnik
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#include <string>
#include <experimental/source_location>

namespace std
{
using source_location = std::experimental::source_location;
}

namespace mstest
{
namespace detail
{
    class TestCaseNode;

    constexpr const char* color_red = "\u001b[31;1m";
    constexpr const char* color_green = "\u001b[32;1m";
    constexpr const char* color_blue = "\u001b[34;1m";
    constexpr const char* color_reset = "\u001b[0m";
}



    class Test
    {
        public:
            virtual ~Test() = default;
            virtual void setup()
            {
            }

            virtual void teardown()
            {
            }

            virtual void execute(mstest::detail::TestCaseNode*) = 0;
    };
}

namespace mstest::detail
{


class TestCaseNode
{
public:
    TestCaseNode(Test* test, const char* suite, const char* testcase)
        : suite_(suite)
        , testcase_(testcase)
        , test_(test)
        , next_(nullptr)
        , passed_(true)
    {
    }

    void fail()
    {
        passed_ = false;
    }

    bool passed() const
    {
        return passed_;
    }

    bool execute()
    {
        test_->setup();
        test_->execute(this);
        test_->teardown();
        return passed_;
    }

    void next(TestCaseNode* next)
    {
        next_ = next;
    }

    TestCaseNode* next()
    {
        return next_;
    }

    const char* testcase() const
    {
        return testcase_;
    }

    const char* suite() const
    {
        return suite_;
    }

private:
    const char* suite_;
    const char* testcase_;
    Test* test_;
    TestCaseNode* next_;
    bool passed_;
};

class TestList
{
public:
    class TestListIterator
    {
    public:
        TestListIterator(TestCaseNode* node) : node_(node)
        {

        }
        ~TestListIterator()                     = default;
        TestListIterator(const TestListIterator&) = default;
        TestListIterator(TestListIterator&&)       noexcept = default;
        TestListIterator& operator=(TestListIterator&&)  noexcept = default;
        TestListIterator& operator=(const TestListIterator&) = default;

        TestListIterator& operator++()
        {
            if (node_ == nullptr)
            {
                return *this;
            }
            node_ = node_->next();
            return *this;
        }

        bool operator!=(const TestListIterator& it) const
        {
            return node_ != it.node_;
        }

        TestCaseNode& operator*()
        {
            return *node_;
        }

    private:
        TestCaseNode* node_;

    };

    TestListIterator begin()
    {
        return TestListIterator(root_);
    }

    TestListIterator end()
    {
        return TestListIterator(nullptr);
    }


    static TestList& get()
    {
        static TestList list;
        return list;
    }

    static bool register_test(TestCaseNode* next)
    {
        TestList& self = get();
        if (self.root_ == nullptr)
        {
            self.root_ = next;
            self.last_ = next;
            return true;
        }
        self.last_->next(next);
        return true;
    }

    void current_test(TestCaseNode* test)
    {
        current_ = test;
    }

    TestCaseNode* current_test()
    {
        return current_;
    }

    TestCaseNode* root()
    {
        return root_;
    }

private:
    TestList() = default;
    TestCaseNode* root_ = nullptr;
    TestCaseNode* last_ = nullptr;
    // TODO: move to context
    TestCaseNode* current_;
};

template <typename X>
constexpr int true_matcher(const X& x, mstest::detail::TestCaseNode* context)
{
    context->fail();
    return x ? true : false;
}

constexpr int generic_matcher(bool pass, mstest::detail::TestCaseNode* context)
{
    if (!pass)
    {
        context->fail();
    }
    return pass;
}


} // namespace mstest::detail

namespace mstest
{

inline int run_tests()
{
    printf ("Executing tests\n");
    int return_code = 0;
    for (auto& test : mstest::detail::TestList::get())
    {
        printf("[Execute      ] %s::%s\n", test.suite(), test.testcase());
        mstest::detail::TestList::get().current_test(&test);
        if(!test.execute())
        {
            ++return_code;
            printf("%s[Failed     ]%s  %s::%s\n", detail::color_red, detail::color_reset, test.suite(), test.testcase());
        }
    }
    return return_code;
}

template <class T>
constexpr void expect_true(T x, const std::source_location& location = std::source_location::current())
{
    if (!x)
    {
        printf("%sAssertion failed:%s expect_true(x), where x is %s\n", detail::color_red, detail::color_reset, std::to_string(x).c_str());
        printf("Called from: %s:%d\n", location.file_name(), location.line());
        mstest::detail::TestList::get().current_test()->fail();
    }
}

}

#define MSTEST(fixture, testcase) \
    class __mstest_##fixture_##testcase : public mstest::Test \
    { \
        void execute(mstest::detail::TestCaseNode*) override;\
    }; \
    static __mstest_##fixture_##testcase __mstest_##fixture_##testcase_instance; \
    static mstest::detail::TestCaseNode __mstest_test_case_node_##fixture_##testcase(&__mstest_##fixture_##testcase_instance, #fixture, #testcase); \
    static volatile bool __mstest_test_case_node_##fixture_##testcase_ = mstest::detail::TestList::register_test(&__mstest_test_case_node_##fixture_##testcase); \
    void __mstest_##fixture_##testcase::execute(mstest::detail::TestCaseNode* __tc_context)

#define EXPECT_TRUE(x) mstest::detail::generic_matcher(x, __tc_context)
// #define EXPECT_FALSE(x) TEST_ASSERT(!x, std::string("assertion failed: ") + std::string(#x) + " is not false")
// #define EXPECT_EQ(x, y) TEST_ASSERT((x) == (y), create_message(x, y, #x, #y, " == "))
// #define EXPECT_NE(x, y) TEST_ASSERT((x) != (y), create_message(x, y, #x, #y, " != "))
// #define EXPECT_GE(x, y) TEST_ASSERT((x) >= (y), create_message(x, y, #x, #y, " >= "))
// #define EXPECT_GT(x, y) TEST_ASSERT((x) > (y), create_message(x, y, #x, #y, " > "))
// #define EXPECT_LE(x, y) TEST_ASSERT((x) <= (y), create_message(x, y, #x, #y, " <= "))
// #define EXPECT_LT(x, y) TEST_ASSERT((x) < (y), create_message(x, y, #x, #y, " < "))
