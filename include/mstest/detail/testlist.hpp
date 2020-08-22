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

#include "mstest/detail/testcase_node.hpp"

namespace mstest
{
namespace detail
{

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
        self.last_ = next;
        return true;
    }

    void current_test(Test* test)
    {
        current_ = test;
    }

    Test* current_test()
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
    Test* current_;
};

} // namespace detail
} // namespace mstest
