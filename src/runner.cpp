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

#include <cstdio>
#include <string_view>

#include "mstest/detail/testlist.hpp"
#include "mstest/detail/colors.hpp"
#include "mstest/detail/symbols.hpp"
#include "mstest/detail/testlist.hpp"

namespace mstest
{

int run_tests()
{
    printf ("%s<---    Executing tests    --->%s\n", detail::color::blue, detail::color::reset);
    int passed_tests = 0;
    int executed_tests = 0;
    int return_code = 0;
    const char* suite = "";

    suite = "";
    for (auto& test : mstest::detail::TestList::get())
    {
        if (std::string_view(suite) != std::string_view(test.suite()))
        {
            suite = test.suite();
            printf("%s -> Suite: %s%s\n", detail::color::blue, suite, detail::color::reset);
        }
        mstest::detail::TestList::get().current_test(test.test());
        if(!test.execute())
        {
            ++return_code;

            printf("%s  x  %-50s %s\n", detail::color::red, test.testcase(), detail::color::reset);
        }
        else
        {
            ++passed_tests;
            printf("%s  %s  %-50s %s\n", detail::color::green, detail::symbols::check_mark, test.testcase(), detail::color::reset);

        }
        ++executed_tests;
    }

    printf("%s ----------------------------%s\n", detail::color::blue, detail::color::reset);
    printf("%s|%s Executed tests: %10d%s |%s\n", detail::color::blue, detail::color::reset, executed_tests, detail::color::blue, detail::color::reset);

    const char* color = mstest::detail::color::green;
    if (executed_tests - passed_tests != 0)
    {
        color = mstest::detail::color::red;
    }

    printf("%s|%s Passed tests  : %10d%s |%s\n", detail::color::blue, detail::color::green, passed_tests, mstest::detail::color::blue, mstest::detail::color::reset);
    printf("%s|%s Failed tests  : %10d%s |%s\n", detail::color::blue, color, executed_tests - passed_tests, detail::color::blue, mstest::detail::color::reset);
    printf("%s ----------------------------%s\n", detail::color::blue, detail::color::reset);

    return return_code;
}

} // namespace mstest
