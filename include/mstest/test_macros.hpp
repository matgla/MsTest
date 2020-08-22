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

#include "mstest/test.hpp"
#include "mstest/detail/testcase_node.hpp"
#include "mstest/detail/testlist.hpp"

#define MSTEST(fixture, testcase) \
    class __mstest_##fixture##_##testcase : public mstest::Test \
    { \
        void execute() override;\
    }; \
    static __mstest_##fixture##_##testcase __mstest_##fixture##_##testcase##_instance; \
    static mstest::detail::TestCaseNode __mstest_test_case_node_##fixture##_##testcase(&__mstest_##fixture##_##testcase##_instance, #fixture, #testcase); \
    static volatile bool __mstest_test_case_node_##fixture##_##testcase##_ = mstest::detail::TestList::register_test(&__mstest_test_case_node_##fixture##_##testcase); \
    void __mstest_##fixture##_##testcase::execute()

#define MSTEST_F(fixture, testcase) \
    class __mstest_##fixture##_##testcase : public fixture \
    { \
        void execute() override;\
    }; \
    static __mstest_##fixture##_##testcase __mstest_##fixture##_##testcase##_instance; \
    static mstest::detail::TestCaseNode __mstest_test_case_node_##fixture##_##testcase(&__mstest_##fixture##_##testcase##_instance, #fixture, #testcase); \
    static volatile bool __mstest_test_case_node_##fixture##_##testcase##_ = mstest::detail::TestList::register_test(&__mstest_test_case_node_##fixture##_##testcase); \
    void __mstest_##fixture##_##testcase::execute()
