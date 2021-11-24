 # Mixin where Base class gets features
> Mixin Definition:  
> Bir sinifin kendisine dokunmadan o sinifa ilave ozellikler kazanmdirmak icin bagimsiz siniflar olusturmak

  - onceki orneklerin cogunda CRTP yi Mixin olusturmak icin kullandik
  - burada ilave ozellik kazandirmak istedigimiz sinifi taban sinif olarak kullanacagiz

  _ex_
  ```cpp
    template<typename Base>
    class SkillA : public Base {
    public:
        void afunc()
        {
            std::cout << "SkillA is running\n";
            Base::foo();
            Base::bar();
            std::cout << "-----------------\n";
        }
    };

    template<typename Base>
    class SkillB : public Base {
    public:
        void bfunc()
        {
            std::cout << "SkillB is running\n";
            Base::baz();
            Base::foo();
            std::cout << "-----------------\n";
        }
    };

    template<typename Base>
    class SkillC : public Base {
    public:
        void cfunc()
        {
            std::cout << "SkillC is running\n";
            Base::foo();
            Base::foo();
            Base::foo();
            std::cout << "-----------------\n";
        }
    };

    // Myclass CRTP deki kalitimla elde edilmiyor
    // Myclass taban sinif olarak kullanilacak

    class Myclass {
    public:
        void foo()
        {
            std::cout << "Myclass::foo()\n";
        }

        void bar()
        {
            std::cout << "Myclass::bar()\n";
        }

        void baz()
        {
            std::cout << "Myclass::baz()\n";
        }
    };

    using Neco = SkillA<Myclass>;

    int main()
    {
        SkillA<Myclass>{}.afunc();
        SkillB<Myclass>{}.bfunc();

        SkillA<SkillB<SkillC<Myclass>>> x;  // x has all the skills: afunc, bfunc, cfunc
    }
  ```

## -CRTP ile Mixin olusturma- vs -Base e yetenek kazandirarak Mixin olusturma-

CRTP-Mixin e bir ornek verelim:
  ```cpp
    // Base
    template<typename Printable> 
    struct RepeatPrint {
        void repeat(unsigned n)
        {
            while(n--)
                static_cast<const Printable&>(*this).print();
        }
    };

    // Skill
    class Name : public RepeatPrint<Name>
    {
    public:
        Name(std::string name, std::string surname) : m_name{std::move(name)},
            m_surname{std::move(surname)}{}

        void print() const
        {
            std::cout << m_name << " " << m_surname << '\n';
        }
    
    private:
        std::string m_name;
        std::string m_surname;
    };

    int main()
    {
        Name myname{"Necati", "ergin"};
        myname.repeat(10);
    }

  ```

  ayni ornegi Base e yetenek kazandirarak Mixin olusturma teknigi ile yapalim

  _ex_
  ```cpp
    // Base class
    class Name
    {
    public:
        Name(std::string name, std::string surname) : m_name{std::move(name)},
            m_surname{std::move(surname)}{}

        void print() const
        {
            std::cout << m_name << " " << m_surname << '\n';
        }
    
    private:
        std::string m_name;
        std::string m_surname;
    };

    // Mixin Class (Skill/Feauture)
    template<typename Printable>
    struct RepeatPrint : Printable {
        explicit RepeatPrint(const Printable& printable) : Printable(printable) {}

        using Printable::Printable;     // inherited ctor

        void repeat(unsigned n) const
        {
            while(n--)
            {
                this->print();  // call for Printable::print
                                // use this ptr since print is the function of base class
            }
        }
    };

    using RepeatPrintableName = RepeatPrint<Name>;

    template<typenmae Printable>
    RepeatPrint<Printable> repeat_print(const Printable& pr)
    {
        return RepeatPrint<Printable>(pr);
    }

    int main()
    {
        RepeatPrint<Name> myname{ Name{"Necati", "Ergin"}};

        RepeatPrint<Name> {"ali", "erkoc"}.repeat(5);

        RepeatPrintableName x{"Umut", "Ktan.."};
        x.repeat(10);

        Name myname{"necati", "ergin"};
        repeat_print(myname).repeat(10);
    }
  ```
