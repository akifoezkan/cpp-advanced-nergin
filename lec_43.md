# Policy-based design uzerine tekrar
> fonksiyonun default arguman almasi ile class
benzer:
- template parametresi ve template parametre nin varsayilan parametre almasi icin onun sagindaki butun parametrelerin hepsi varsayilan arguman almali

benzemeyen ozelikler
i)
  _ex:_
  ```cpp
    void func(int x, int y = x);    // Error
                                    // template da yapilabilir
  ```

- Template parametre varsayilan arguman aldiginda, fonksiyonun parametresi de varsayilan arguman alir
  _ex:_
  ```cpp
    template<typename T, typename U = std::vector<T>>
    void func(T x, U y = U{})
    {
        typeid(U).name() << "\n";       // vector<int, ..>
        cout << y.size() << '\n';       // 0
    }

    int main()
    {
        func(12);
    }
  ```

ayrildim - 5 dk
  _ex:_
  ```cpp
    #include <vector>

    template<template<typename T> typename C>
    class Myclass {
    };

    template<template<typename, typename> typename C>
    class Myclass2 {
    };

    int main()
    {
        Myclass<std::vector> x; // ok
        
        Myclass2<std::vector> x; // ok, vector's econd template parameter is default allocator
    }
  ```

  _ex:_
  ```cpp
    template<typenmae T>
    class Nec {};

    template<template<typenmae> typename C>
    class Myclass {
    public:
        Myclass()
        {
            C<int> i;
            C<double> d;
            cout << typeid(C).name() << "\n";
            cout << typeid(Myclass).name() << "\n";
            cout << typeid(*this).name() << "\n";
            cout << typeid(i).name() << "\n";
            cout << typeid(d).name() << "\n";
        }
    };

    int main()
    {
        Myclass<Nec> m;
    }
  ```

  _ex:_
  ```cpp
    template<typenmae T>
    class Nec {};

    template<typename T, template<typenmae> typename C>
    class Myclass {
    public:
        Myclass()
        {
            C<T> i;
            cout << typeid(C<T>).name() << '\n';    // Nec<int>
        }
    };

    int main()
    {
        Myclass<int, Nec> m;
    }
  ```

-> type dedcution
  _ex:_
  ```cpp
    template<typename T, typename A, template<typename, typename> class Con>
    void func(const Con<T, A>& c)
    {
        cout << typeid(T).name() << '\n';
        cout << typeid(A).name() << '\n';
        cout << typeid(c).name() << '\n';
        cout << typeid(Con).name() << '\n';
    }

    int main()
    {
        using namespace std;

        vector<int> ivec;

        func(ivec);
    }
  ```

variadic parametre>>
  _ex:_
  ```cpp
    #include <iostream>
    #include <tuple>

    template <typename T>
    class A {
    };

    template <typename T>
    class B {
    };

    template<template<typename> typename ...Ts>
    class Myclass {
    public:
        Myclass()
        {
            cout << typeid(Myclass).name() << '\n';
            cout << typeid(*this).name() << '\n';
            cout << typeid(tuple<Ts<int>...>).name() << '\n';
        }
    };

    int main()
    {
        Myclass<A> x;
        
        Myclass<A, B> y;
        
        Myclass<A, B, C> z;
    }
  ```

  _ex:_
  ```cpp
    #include <iostream>
    #include <tuple>

    template <typename T>
    class A {
    };

    template <typename T>
    class B {
    };

    template<typename T, template<typename> typename ...Ts>
    class Myclass {
    public:
        Myclass()
        {
            cout << typeid(Myclass).name() << '\n';
            cout << typeid(*this).name() << '\n';
            cout << typeid(tuple<Ts<T>...>).name() << '\n';
        }
    };

    int main()
    {
        Myclass<int, A, B, C> z;
    }
  ```
-> CRTP de variadic template parametre paketinin kullanilmasi
  _ex:_
  ```cpp
    template<typename Der>
    class A {
    public:
        void func();
        {
            static_cast<Der*>(this)->f1();
        }
    };

    template<typename Der>
    class B {
    public:
        void foo();
        {
            static_cast<Der*>(this)->f2();
        }
    };

    class Myclass : public A<Myclass>, public B<Myclass> {
    public:
        void f1()
        {
            cout << "Myclass::f1()\n";
        }

        void f2()
        {
            cout << "Myclass::f1()\n";
        }
    };

    int main()
    {
        Myclasss mx;

        mx.func();
        mx.foo();
    }
  ```

  _ex:_
  ```cpp
    template<typename Der>
    class A {
    public:
        void func();
        {
            static_cast<Der*>(this)->f1();
        }
    };

    template<typename Der>
    class B {
    public:
        void foo();
        {
            static_cast<Der*>(this)->f2();
        }
    };

    template<template<typename> typename ...Ts>
    class Myclass : public Ts<Myclass<Ts...>>...{
    public:
        Myclass {
            cout << typeid(tuple<Myclass<Ts...>>).name() << "\n";
            cout << typeid(tuple<Ts<Myclass<Ts...>>...>).name() << "\n";
                // tuple<class A<class Myclass<class A, class B>>> ??? buraya bak
        }
        void f1()
        {
            cout << "Myclass::f1()\n";
        }

        void f2()
        {
            cout << "Myclass::f1()\n";
        }
    };

    int main()
    {
        Myclass<A> x;
        Myclass<A, B> x;
        Myclass<A, B, C> x;
    }
  ```

  _ex:_
  ```cpp
    template<typename Der>
    class A {
    public:
        void func();
        {
            static_cast<Der*>(this)->f1();
        }
    };

    template<typename Der>
    class B {
    public:
        void foo();
        {
            static_cast<Der*>(this)->f2();
        }
    };

    template<typename Der>
    class C {
    public:
        void bar();
        {
            static_cast<Der*>(this)->f3();
        }
    };

    template<template<typename> typename ...Skills>
    class Myclass : public Skills<Myclass<Skills...>>...{
            // type of Myclass =  Myclass<Skills...> 
            // public Skill1<type(Myclass)>, public Skill2<type(Myclass)>
    public:
        Myclass()
        {
            cout << typeid(Myclass).name << '\n';
            cout << typeid(tuple<Skills <Myclass >>)  ????

            // ?? tuple<class A <class Myclass<class A, class B, class C>>,  > 
        }

        void f1()
        {
            cout << "f1 cagirildi\n";
        }

        void f2()
        {
            cout << "f2 cagirildi\n";
        }

        void f3()
        {
            cout << "f3 cagirildi\n";
        }
    };

    int main()
    {
        Myclass<A> x;       // class Myclass<class A>
        x.func();

        Myclass<A, B> x;    // class Myclass<class A, class B>
        x.func();
        x.foo();

        Myclass<A, B, C> x;    // class Myclass<class A, class B, class C>
        x.func();
        x.foo();
        x.bar();
    }
  ```

read: fluent c++: https://www.fluentcpp.com/2018/06/22/variadic-crtp-opt-in-for-class-features-at-compile-time/
sekle bak
> policy - based design da bu yapi cok kullaniliyor

# Policy-based design

en typical example: smartpointer
  _ex:_
  ```cpp
    //scoped_ptr.hi, unique ptr
    #pragma once
    #include<utility>
    #include<vector>

    template<typename T>
    class ScopedPtr {
    public:
        explicit ScopedPtr(T* p = nullptr) : m_p{p} {}
        ~ScopedPtr()
        {
            delete m_p;
        }

        const T* operator->() const
        {
            return m_p;
        }

        T* release()
        {
            return std::exchange(m_p, nullptr);
        }

        T& operator*()
        {
            return *m_p;
        }

        const T& operator*() const
        {
            return *m_p;
        }

    private:
        ScopedPtr(const ScopedPtr&) = delete;
        ScopedPtr(const ScopedPtr&) = delete;

        T* m_p;
    };

  ```

  ```cpp
    // nec.h
    #include<iosfwd>

    class Nec {
    public:
        Nec(int ival = 0);
        ~Nec();

        void set(int ival)
        {
            m_val = ival;
        }

        int get() const
        {

        }

        friend std::ostream& operator<<(std::ostream&, const Nec&);

    private:
        int m_val;
    };
  ```

  ```cpp
    // Nec.cpp
    #include "Nec.h"
    #include<iostream>

    Nec::Nec(int val) : m_val{val}
    {
        cout << m_val << " degerinde Nec nesnesi olusturuldu";
    }

    Nec::~Nec()
    {
        cout << m_val << "degerindeki Nec nesnesi yok edildi";
    }

    std::ostream& operator<<(std::ostream& os, const Nec& nec)
    {
        return os << '(' << nec.m_val << ')';
    }

  ```

  ```cpp
    // main.cpp
    #include "scoped_ptr.h"

    void func()
    {
        ScopedPtr sptr{new Nec{34}};
        cout << *sptr << "\n";
        sptr->set(459);
        cout < *sptr << "\n";
        auto val = sptr->get();
        cout << "val = " << val << "\n";
    }

    int main()
    {
        cout << "main basladi\n";
        
        func();
                // 34 degerinde nesne olusturuldu
                // ...
                // 34 degerinde nesne destruct edildi

        cout << "main devam ediyor\n";
    }
  ```

// onceden yazilmis kodlar incelendi
1.tekink
    Del_policy private member -> containment yoluyla composition
    policy nin sizeof u yuksek oldugunda, tasima semantikten faydalanilabilir

2. ornek
- tempalte<typename> typenmae DelPolicy = DefaultDelete
    * neden template template parametre secilir?
    - ayni sinif sablonunun farkli specialization lari kullanilacaksa

- DelPolicy<T> m_del_policy

  _ex:_

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
