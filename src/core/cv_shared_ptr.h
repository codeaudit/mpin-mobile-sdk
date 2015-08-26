/*
Copyright (c) 2012-2015, Certivox
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

For full details regarding our CertiVox terms of service please refer to
the following links:
 * Our Terms and Conditions -
   http://www.certivox.com/about-certivox/terms-and-conditions/
 * Our Security and Privacy -
   http://www.certivox.com/about-certivox/security-privacy/
 * Our Statement of Position and Our Promise on Software Patents -
   http://www.certivox.com/about-certivox/patents/
*/

#ifndef CV_SHARED_PTR_H_INCLUDED
#define CV_SHARED_PTR_H_INCLUDED

#ifdef _WIN32
#include <Windows.h>
#endif

namespace
{

    struct AtomicCounter
    {
        inline AtomicCounter(long x) : val(x)
        {
        }

        inline long operator++()
        {
            #ifdef __GNUC__
            return __sync_add_and_fetch(&val, 1);
            #elif _MSC_VER
            return InterlockedIncrement(&val);
            #endif
        }

        inline long operator--()
        {
            #ifdef __GNUC__
            return __sync_sub_and_fetch(&val, 1);
            #elif _MSC_VER
            return InterlockedDecrement(&val);
            #endif
        }

    private:
        long val;
    };

    typedef AtomicCounter RefCounterT;

    struct Holder
    {
        inline Holder(void* t) : target(t), count(1)
        {
        }

        inline void inc()
        {
            ++count;
        }

        template <class T> inline void dec()
        {
            if (--count <= 0)
            {
                delete (T*) target;
                delete this;
            }
        }

        void* target;
        RefCounterT count;
    };
}

template <class From, class To> inline bool RequireConvertible()
{
    To* oth = (From*)0;
    return oth == NULL;
}

/*
 * TODO: support alias-constructed pointers, add relational operators,
 * support deleters, add the rest of the standard methods (swap, reset, use_count, etc.)
 */
template <class T> class shared_ptr
{
public:

    template <typename U> friend class shared_ptr;

    typedef T element_type;

    shared_ptr() : holder(NULL)
    {
    }

    explicit shared_ptr(element_type* t) : holder(new Holder(t))
    {
    }

    shared_ptr(const shared_ptr& x, T* p) : holder(x.holder)
    {
        if (holder)
        {
            holder->inc();
        }
    }

    ~shared_ptr()
    {
        if (holder)
        {
            holder->dec<element_type>();
        }
    }

    shared_ptr(const shared_ptr& x) : holder(x.holder)
    {
        if (holder)
        {
            holder->inc();
        }
    }

    template <class U> shared_ptr(const shared_ptr<U>& x)
    {
        RequireConvertible<U, element_type>();
        holder = x.holder;
        if (holder)
        {
            holder->inc();
        }
    }

    shared_ptr& operator=(const shared_ptr& x)
    {
        if (x.holder)
        {
            x.holder->inc();
        }
        if (holder)
        {
            holder->dec<T>();
        }
        holder = x.holder;
        return *this;
    }

    template <class U> shared_ptr& operator=(const shared_ptr<U>& x)
    {
        RequireConvertible<U, element_type>();
        if (x.holder)
        {
            x.holder->inc();
        }
        if (holder)
        {
            holder->dec<T>();
        }
        holder = x.holder;
        return *this;
    }

    element_type* operator->() const
    {
        if (!holder)
        {
            return NULL;
        }
        return (element_type*)holder->target;
    }

    element_type& operator*() const
    {
        return *operator->();
    }

    operator bool() const
    {
        if (!holder)
        {
            return false;
        }
        return holder->target != NULL;
    }

    element_type* get() const
    {
        return operator->();
    }

private:
    Holder* holder;
};

template<typename T> shared_ptr<T> make_shared() { return shared_ptr<T>(new T()); }
template<typename T, typename Arg1> shared_ptr<T> make_shared(const Arg1& arg1) { return shared_ptr<T>(new T(arg1)); }
template<typename T, typename Arg1, typename Arg2> shared_ptr<T> make_shared(const Arg1& arg1, const Arg2& arg2) { return shared_ptr<T>(new T(arg1, arg2)); }
template<typename T, typename Arg1, typename Arg2, typename Arg3> shared_ptr<T> make_shared(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) { return shared_ptr<T>(new T(arg1, arg2, arg3)); }
template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4> shared_ptr<T> make_shared(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3, const Arg4& arg4) { return shared_ptr<T>(new T(arg1, arg2, arg3, arg4)); }

template <class T, class U> inline shared_ptr<T> static_pointer_cast(const shared_ptr<U>& x)
{
    return shared_ptr<T>(x, static_cast<T*>(x.get()));
}

template <class T, class U> inline shared_ptr<T> const_pointer_cast(const shared_ptr<U>& x)
{
    return shared_ptr<T>(x, const_cast<T*>(x.get()));
}

template <class T, class U> inline shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& x)
{
    if (T* p = dynamic_cast<T*>(x.get()))
    {
        return shared_ptr<T>(x, p);
    }
    return shared_ptr<T>();
}

#endif // CV_SHARED_PTR_H_INCLUDED

