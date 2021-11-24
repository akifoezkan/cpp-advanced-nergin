# Question: virtual dispatch - access control (interviews)
  polymorphic bir taban class var. sanal fonksiyon private olabilir mi?  

## Non-virtual idiom (NVI) 
  tabi ki olur, hatta cokca tercih edilen bir metod (template method)
  - implementasyonla interface i daha iyi ayirmak icin
  - taban sinifa ve turemis sinifa class invariantlerini kontrol etme olanigi vermek icin
  sanal fonksiyonu private yapariz.

  ```cpp
    // Turemis sinifin public/private virtual fonk. taban sinifin public/private virtual fonk. override edebilir
    class Base {
    public:
        void foo()
        {
            // kodlar
            func();
            // kodlar
        }
    private:
        virtual void func()
        {
            std::cout << "Bse::func()\n";
        }
    };

    class Der: public Base {
    public:
        void func() override
        {
            std::cout << "Der::func()\n";
        }
    };

    void f(Base& base)
    {
        // base.func();    // error: func is private
        base.foo();
    }

    int main()
    {
        Der myder;
        f(myder);       // ok, Der::func
    }
  ```

## virtual dispatch - access control
  ```cpp
    class Base {
    public:
        virtual void func()
        {
            std::cout << "Bse::func()\n";
        }
    };

    class Der: public Base {
    private:
        void func() override
        {
            std::cout << "Der::func()\n";
        }
    };

    void f(Base& r)
    {
        r.func();
    }

    int main()
    {
        Der myder;

        f(myder);   // ok, Der::func
                    // runtime da Der::func cagirilir. 
                    // access control compile time da static ture (Base) gore yapilir

        myder.func();   // syntax error, der::func() is private
    }
  ```

## virtual dispatch - default arguments
  ```cpp
    class Base {
    public:
        virtual void func(int x = 10)
        {
            std::cout << "Base::func(int) x = " << x << "\n";
        }
    };

    class Der: public Base {
    public:
        void func(int x = 50) override
        {
            std::cout << "Der::func(int) x = " << x << "\n";
        }
    };

    void f(Base& r)
    {
        r.func();
    }

    int main()
    {
        Der myder;

        f(myder);   // ok, calls Der::func, with int x=10
                    // default argument is a compile-time property 
    }
  ```
---