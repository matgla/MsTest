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
#include <sstream>
#include <experimental/source_location>

#include "mstest/detail/colors.hpp"

namespace std
{
    using source_location = std::experimental::source_location;

    /* https://stackoverflow.com/questions/5100718/integer-to-hex-string-in-c */
    template <typename I> std::string n2hexstr(I w, size_t hex_len = sizeof(I)<<1) {
        static const char* digits = "0123456789ABCDEF";
        std::string rc(hex_len,'0');
        for (size_t i=0, j=(hex_len-1)*4 ; i<hex_len; ++i,j-=4)
            rc[i] = digits[(w>>j) & 0x0f];
        return rc;
    }
    // TODO: change to custom printers
    template <typename T>
    std::string to_string(T* x)
    {
        return std::string("0x") + n2hexstr(reinterpret_cast<std::size_t>(x));
    }
} // namespace std


namespace mstest
{

inline void print_location(const std::source_location& location)
{
    printf("        Called from: %s:%d\n", location.file_name(), location.line());
}

inline bool generic_matcher(bool passed, const std::source_location& location)
{
    if (!passed)
    {
        print_location(location);
        mstest::detail::TestList::get().current_test()->fail();
    }
    return passed;
}

template <class T>
void expect_true(T x, const std::source_location& location = std::source_location::current())
{
    if (!generic_matcher(x, location))
    {
        printf("    %sAssertion failed:%s expect_true(x), where x = %s\n", detail::color::red, detail::color::reset, std::to_string(x).c_str());
    }
}

template <class T>
constexpr void expect_false(const T& x, const std::source_location& location = std::source_location::current())
{
    if (generic_matcher(x, location))
    {
        printf("    %sAssertion failed:%s expect_false(x), where x = %s\n", detail::color::red, detail::color::reset, std::to_string(x).c_str());
    }
}

template <class A, class B>
constexpr void expect_eq(A a, B b, const std::source_location& location = std::source_location::current())
{
    if (!generic_matcher(a == b, location))
    {
        printf("    %sAssertion failed:%s expect_eq(a, b), where a = %s, b = %s\n", detail::color::red, detail::color::reset, std::to_string(a).c_str(), std::to_string(b).c_str());
    }
}

template <class A, class B>
constexpr void expect_gt(A a, B b, const std::source_location& location = std::source_location::current())
{
    if (!generic_matcher(a > b, location))
    {
        printf("    %sAssertion failed:%s expect_gt(a, b), where a = %s, b = %s\n", detail::color::red, detail::color::reset, std::to_string(a).c_str(), std::to_string(b).c_str());
    }
}

template <class A, class B>
constexpr void expect_lt(A a, B b, const std::source_location& location = std::source_location::current())
{
    if (!generic_matcher(a < b, location))
    {
        printf("    %sAssertion failed:%s expect_lt(a, b), where a = %s, b = %s\n", detail::color::red, detail::color::reset, std::to_string(a).c_str(), std::to_string(b).c_str());
    }
}


template <class A, class B>
constexpr void expect_ge(A a, B b, const std::source_location& location = std::source_location::current())
{
    if (!generic_matcher(a >= b, location))
    {
        printf("    %sAssertion failed:%s expect_ge(a, b), where a = %s, b = %s\n", detail::color::red, detail::color::reset, std::to_string(a).c_str(), std::to_string(b).c_str());
    }
}

template <class A, class B>
constexpr void expect_le(A a, B b, const std::source_location& location = std::source_location::current())
{
    if (!generic_matcher(a <= b, location))
    {
        printf("    %sAssertion failed:%s expect_le(a, b), where a = %s, b = %s\n", detail::color::red, detail::color::reset, std::to_string(a).c_str(), std::to_string(b).c_str());
    }
}


} // namespace mstest
