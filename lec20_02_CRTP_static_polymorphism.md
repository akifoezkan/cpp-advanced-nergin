 # Question: CRTP pattern where the Derived is a template class 

  _ex_
  ```cpp
    template<typename Der>
    struct MakeDouble {
        Der get_double() const
        {
            auto &d = static_cast<const Der&>(*this);
            return d + d;
        }
    };

    template<typename Der>
    struct MakeTriple {
        Der get_triple() const
        {
            auto &d = static_cast<const Der&>(*this);
            return d + d + d;
        }
    };

    template<typename T>
    class Value : public MakeDouble<Value<T>>, public MakeTriple<Value<T>>
    {
    public:
        Value(const T& val) : mval(val) {};

        Value operator+(const Value& other) const
        {
            return mval + other.mval;
        }

        void print() const
        {
            std::cout << mval << "\n">>;
        }
    private:
        T mval;
    }

    int main()
    {
        Value<int> x{};

        auto val1 = x.get_double();
        auto val2 = x.get_triple();

        std::cout << val1.print() << val2.print();

        Value<std::string> y {"burak"};
        auto val3 = y.get_double();
        auto val4 = y.get_double();

        val3.print():
        val4.print():
    }
  ```

# CRTP, static polymorphism, virtual dispatch e bir alternatif olusturur

> Inheritance cok guclu bir bagimlilik olusturur, bu yuzden cok iyi dusunmek lazim  
> Duruma gore inheritance in alternatifleri var:
> 1. CRTP: static polymorphism
> 2. std::variant kullanmak (olusturulacak sinif sayisi belli ise)  
>    variant daki alternatif turler kalitimda kullanilacak turler olsun  
>    ex: A, B, C bir taban siniftan olusacak ve kapali bir hiyerarsi tanimlanacak (ileride bir D sinifi eklenmeyecek)  
>    bu durumda variant in A, B, C yapisi olusturulabilir
> 3. type erasure idiom: 
>    en kuvvetli alternatif,  
>    avantajlari: inheritance a alternatif olusturur ve bagimliligi azaltir  
>    dezavantajlari: anlasilmasi ve yazilmasi biraz zor ancak bu isi yapan kutuphaneler var

## An example to dynamic polymorphism
  ```cpp
    class Pet {
    public:
        void make sound()
        {
            std::cout << get_sound() << "\n";
        }

    private:
        virtual std::string get_sound() const = 0;  // pure virtual
    };

    class Cat : public Pet {
    public:
        virtual std::string get_sound() const override 
        {
            return "miyav miyav";
        }
    };

    class Dog : public Pet {
    public:
        virtual std::string get_sound() const override 
        {
            return "hav hav hav";
        }
    };

    class Bird : public Pet {
    public:
        virtual std::string get_sound() const override 
        {
            return "cik cik cik";
        }
    };

    void pet_game(Pet& pet)
    {
        pet.make_sound();
    }

    int main()
    {
        Cat mycat;
        Dog mydog;
        Bird Mybird;

        pet_game(mycat);
        pet_game(mydog);
        pet_game(mybird);
    }
  ```

  ---
    Reminder:  
    override: Modern C++ (contextual category)  
    advantage: overide keywrod kullanildiginda, compiler taban sinifta derleyici override edilecek bir interface var mi diye bakar yoksa syntax error verir.
  ---

## An example to static polymorphism
Yukaridaki ornegi static polymorphisme cevirelim (CRTP) 

  ```cpp
    template<typename Der>
    class Pet {
    public:
        void make sound()
        {
            std::cout << static_cast<const Der*>(this)->get_sound(); 
        }

    private:
        const Der& derObject()
        {
            static_cast<const Der&>(*this);
        }
    };

    class Dog : public Pet<Cat> {
    public:
        std::string get_sound() const
        {
            return "miyav miyav miyav";
        }
    }

    class Dog : public Pet<Dog> {
    public:
        std::string get_sound() const
        {
            return "hav hav hav";
        }
    }

    class Dog : public Pet<Bird> {
    public:
        std::string get_sound() const
        {
            return "cik cik cik";
        }
    }

    template<typenmae T>
    void pet_game(Pet<T>& pet)
    {
        pet.make_sound();
    }

    int main()
    {
        Cat mycat;
        Dog mydog;
        Bird Mybird;

        pet_game(mycat);
        pet_game(mydog);
        pet_game(mybird);

        // tabi ki ben bu nesneleri Base Type bir container da tutamam
        // cunku bunlar farkli turler, ayni ture dayanan nesneler degil.

        // dogal olarak asagida bir problem yok
        Cat mycat;
        mycat.make_sound();
    }
  ```
  Avantajar:
  - sanal fonksiyon maliyeti yok
  - nesneler dinamik omulu olmak zorunda degil

  Dezavantajlar (duruma gore)
  - olustrulan derived typelar birbirinden farkli turler. Ayni contanier da tutamam.

## Singleton as an examples to CRTP
  ```cpp
    // thread safe Singleton

    template<typename Derived> 
    class Singleton {
        static std::unique_ptr<Derived> m_instance;
        static std::once_flag           m_once;

    protected:
        Singleton() {}

    public:
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;

        static Derived& get_instance()
        {
            std::call_once(Singleton::m_once,
                [](){ Singleton::m_instance.reset(new Derived()); });

            return *m_instance;
        }
    };

    template<typename Derived>
    std::unique_ptr<Derived> Singleton<Derived>::m_instance;

    template<typename Derived>
    std::once_flag<Derived> Singleton<Derived>::m_once;

    class Nec : public Singleton<Nec> {
    public:
        Nec() {
            std::cout << "Nec ctor\n";
        };

        ~Nec() {
            std::cout << "Nec dtor\n";
        };

        void foo()
        {
            std::cout < "Nec::foo()\n";
        }

        void bar()
        {
            std::cout < "Nec::bar()\n";
        }
    }

    int main()
    {
        Nec::get_instance().foo();
        Nec::get_instance().bar();

        auto& snec = Nec::get_instance();
        snec.bar();

        auto mynec = snec;  // Error, copy ctor is deleted
    }
  ```