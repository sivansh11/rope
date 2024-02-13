#include "rope.hpp"

#include <iostream>
#include <unordered_map>
#include <queue>
#include <cassert>
#include <fstream>
#include <sstream>
#include <functional>
#include <chrono>
#include <thread>

// didnt know what to call this, its just an allocater that checks if all the allocations are deallocated
template <typename _Tp>
class checking_allocator {
public:
    using value_type = _Tp;
    checking_allocator() = default;
    ~checking_allocator() noexcept(false) {
        if (_allocations.size()) {
            throw std::runtime_error("not all allocations are cleared");
        }
    }

    value_type *allocate(size_t n) {
        value_type *p = reinterpret_cast<value_type *>(::operator new(n * sizeof(value_type)));
        _allocations.insert({size_t(p), n});
        return p;
    }

    void deallocate(value_type *p, size_t n) {
        auto itr = _allocations.find(size_t(p));
        if (itr == _allocations.end()) {
            throw std::runtime_error("pointer not allocated by allocator");
        }
        if (itr->second != n) {
            throw std::runtime_error("not all elements cleared");
        }
        _allocations.erase(itr);
        ::operator delete(p);
    }

private:
    std::unordered_map<size_t, size_t> _allocations;
};

void test_rope() {
    // set slice end append
    {
        rope::rope_t<7, checking_allocator> r("01234567");
        r.set_slice("8", 1, 8, 0);
        assert(r.to_string() == "012345678");
    }
    {
        rope::rope_t<8, checking_allocator> r("01234567");
        r.set_slice("8", 1, 8, 0);
        assert(r.to_string() == "012345678");
    }
    {
        rope::rope_t<9, checking_allocator> r("01234567");
        r.set_slice("8", 1, 8, 0);
        assert(r.to_string() == "012345678");
    }
    {
        rope::rope_t<7, checking_allocator> r("01234567");
        r.set_slice("89", 2, 8, 0);
        assert(r.to_string() == "0123456789");
    }
    {
        rope::rope_t<8, checking_allocator> r("01234567");
        r.set_slice("89", 2, 8, 0);
        assert(r.to_string() == "0123456789");
    }
    {
        rope::rope_t<9, checking_allocator> r("01234567");
        r.set_slice("89", 2, 8, 0);
        assert(r.to_string() == "0123456789");
    }
    // set slice size == n
    {
        rope::rope_t<1, checking_allocator> r("012345");
        r.set_slice("a", 1, 3, 1);
        assert(r.to_string() == "012a45"); 
    }
    {
        rope::rope_t<2, checking_allocator> r("012345");
        r.set_slice("a", 1, 3, 1);
        assert(r.to_string() == "012a45"); 
    }
    {
        rope::rope_t<3, checking_allocator> r("012345");
        r.set_slice("a", 1, 3, 1);
        assert(r.to_string() == "012a45"); 
    }
    {
        rope::rope_t<1, checking_allocator> r("012345");
        r.set_slice("ab", 2, 3, 2);
        assert(r.to_string() == "012ab5"); 
    }
    {
        rope::rope_t<2, checking_allocator> r("012345");
        r.set_slice("ab", 2, 3, 2);
        assert(r.to_string() == "012ab5"); 
    }
    {
        rope::rope_t<3, checking_allocator> r("012345");
        r.set_slice("ab", 2, 3, 2);
        assert(r.to_string() == "012ab5"); 
    }
    {
        rope::rope_t<4, checking_allocator> r("012345");
        r.set_slice("ab", 2, 3, 2);
        assert(r.to_string() == "012ab5"); 
    }
    {
        rope::rope_t<6, checking_allocator> r("012345");
        r.set_slice("ab", 2, 3, 2);
        assert(r.to_string() == "012ab5"); 
    }

    // set slice size < n case 2
    {
        rope::rope_t<8, checking_allocator> r("01234567");
        r.set_slice("ab", 2, 3, 3);
        assert(r.to_string() == "012ab67");
    }
    // set slice size < n case 4
    {
        rope::rope_t<4, checking_allocator> r("01234567");
        r.set_slice("ab", 2, 2, 3);
        assert(r.to_string() == "01ab567");
    }
    // set slice size < n case 1
    {
        rope::rope_t<4, checking_allocator> r("01234567");
        r.set_slice("abc", 3, 2, 6);
        assert(r.to_string() == "01abc");
    }
    // set slice size < n case 3 
    {
        rope::rope_t<4, checking_allocator> r("01234567");
        r.set_slice("ab", 2, 2, 6);
        assert(r.to_string() == "01ab");
    }
    // set slice size < n no writes 
    {
        rope::rope_t<4, checking_allocator> r("01234567");
        r.set_slice("", 0, 4, 4);
        assert(r.to_string() == "0123");
    }
        // set_slice size > n
    {
        rope::rope_t<1, checking_allocator> rope1("0123456789");
        rope::rope_t<2, checking_allocator> rope2("0123456789");
        rope::rope_t<3, checking_allocator> rope3("0123456789");
        rope::rope_t<4, checking_allocator> rope4("0123456789");
        rope::rope_t<5, checking_allocator> rope5("0123456789");
        rope::rope_t<6, checking_allocator> rope6("0123456789");
        rope::rope_t<7, checking_allocator> rope7("0123456789");
        rope::rope_t<8, checking_allocator> rope8("0123456789");
        rope::rope_t<9, checking_allocator> rope9("0123456789");
        rope::rope_t<10, checking_allocator> rope10("0123456789");
        rope::rope_t<20, checking_allocator> rope20("0123456789");
        rope1.set_slice("abcd", 4, 3, 3);
        rope2.set_slice("abcd", 4, 3, 3);
        rope3.set_slice("abcd", 4, 3, 3);
        rope4.set_slice("abcd", 4, 3, 3);
        rope5.set_slice("abcd", 4, 3, 3);
        rope6.set_slice("abcd", 4, 3, 3);
        rope7.set_slice("abcd", 4, 3, 3);
        rope8.set_slice("abcd", 4, 3, 3);
        rope9.set_slice("abcd", 4, 3, 3);
        rope10.set_slice("abcd", 4, 3, 3);
        rope20.set_slice("abcd", 4, 3, 3);
        assert(rope1.to_string() == "012abcd6789");
        assert(rope2.to_string() == "012abcd6789");
        assert(rope3.to_string() == "012abcd6789");
        assert(rope4.to_string() == "012abcd6789");
        assert(rope5.to_string() == "012abcd6789");
        assert(rope6.to_string() == "012abcd6789");
        assert(rope7.to_string() == "012abcd6789");
        assert(rope8.to_string() == "012abcd6789");
        assert(rope9.to_string() == "012abcd6789");
        assert(rope10.to_string() == "012abcd6789");
        assert(rope20.to_string() == "012abcd6789");
    }
}

int main() {

    rope::rope_t<5, checking_allocator> r("01234567");
    std::string str = "abcd";
    r.set_slice(str.c_str(), str.size(), 2, 3);
    std::cout << r << '\n';

    test_rope();
    return 0;
}