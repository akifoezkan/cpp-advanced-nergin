# ilk yarim saati not etmedim

  _ex:_
  ```cpp
    cout << format("{:x}", 47802) << "\n"
    cout << format("{:X}", 47802) << "\n"
    
    ...
    cout << format("{:f}", 5.) << "\n";     
    cout << format("{:#f}", 5.) << "\n";        
  ```

## nested placeholder
  _ex:_
  ```cpp
    double dval = 3. / 7;
    int width = 12;
    int precision = 5;

    cout << format("|{2:<{0}.{1}f}|", width, precision, dval) << "\n";   // |0.42..   |
  ```

## locale
  _ex:_
  ```cpp
    cout << format(locale{"turkish"}, "{:L}\n", 3.12);          // 3,12
    cout << format(locale{"turkish"}, "{:L}\n", 457'982'123);   // 457.982.123
    cout << format(locale{"turkish"}, "{}\n", 457'982'123);   	// 457982123
    cout << format( "{}\n", 457'982'123) << "\n";   			// 457982123 ???
  ```

  _ex:_
  ```cpp
	locale::global(locale{"turkish"});

	cout << 123.9765 << "\n";
  ```

  _ex:_
  ```cpp
	locale::global(locale{"turkish"});

	format("{:L}\n", 4.56);		// 4,56
  ```

# std::format_to
  - std::format bir string olusturur
  - std::format_to ....

  string olustrumasini istemiyorsak (....), format_to kullanabiliriz.

  _ex:_
  ```cpp
	int x = 47802;
	format_to(ostream_iterator<char>(cout), "|{:16X}|", x);		// |     BABA|
  ```

  _ex:_
  ```cpp
	ofstream ofs{ "output.txt" };
	if(!ofs) {
        cerr << "dosya olusturulamadi\n";
        return 1;
    }

	for(int i = 0 ; i < 10000; ++i)
	{
        format_to(ostream_iterator<char>(ofs), "{:<6} {:>12.4f}\n", i , sqrt(i));
	}
  ```

  _ex:_
  ```cpp
    string name {"Harun Bozaci"};
    int id = 76234;
    double dval{65.7234};

    string str;

    format_to(back_inserter(str), "|{}{}{:.2f}|", id, name, dval);

    cout << str << "\n";    // |76234Harun Bozaci65.7234|
  ```

  _ex:_
  ```cpp
    string name {"Harun Bozaci"};
    int id = 76234;
    double dval{65.7234};

    list<char> clist;

    format_to(back_inserter(clist), "|{}{}{:.2f}|", id, name, dval);

    for(auto c : clist)
        cout << c;

    cout << str << "\n";    // |
  ```

  _ex:_
  ```cpp
    string str;

    for(char c = 'A'; c <= 'Z'; ++c) 
    {
        format_to(back_inserter(str), "{{{0}, {0}}}", c);       // {A, A} {B, B}
        
        format_to(back_inserter(str), "{{{0}, {0:d}}}", c);      // {A, A65} {B, 66}
    }

    cout << str << "\n"; 
  ```

# format_to_n
    - buffer overflow diziyi tasirmayacak
    - return type: format_to_n_result

  format_to_n
    .out    // address char*
    .size   // 

  yazinin sonuna Null char koymaz.
  Bunu yapmak istersen yazinin sonuna kendimiz koymaliyiz.

  _ex:_
  ```cpp
    string name{"necati ergin"};

    char buffer[64];
    auto result = format_to_n(buffer, std::size(buffer) - 1, "{} isminin uzunlugu {}.", name, name.length());

    cout << buffer << "\n"; // undefined behaviour, no NULL char at the end

    cout << result.size << "\n";            // 33
    cout << result.out - buffer << "\n";    // 33

    *result.out = '0';
    // or
    buffer[result.size] = '\0';

    cout << "|" << buffer << "|";   //ok
  ```

  _ex:_
  ```cpp
    std::array<char, 10> ar;

    for (auto c : ar)
  ```

  _ex:_
  ```cpp
    std::array<char, 10> ar{};

    format_to_n(ar.data(), ar.size() - 1, "{} {}", "necati", "ergin");

    cout << "ar.data() : " << ar.data() << "\n";
  ```

## formatted_size
  - ??

  _ex:_
  ```cpp
    string name { "ahmet aksoy" };
    int age{ 25 };

    formatted_size("{} {}", name, age);

    std::cout << "n = " << n << "\n";
  ```

  _ex:_
  ```cpp
    int x{ 65234 };
    double dval = 123.654;

    const char* p{"{:#X} {:.2f}"};
    const auto n = formatted_size(p, x, dval);

    vector<char> buf(n);
    format_to(buf.begin(), p , x, dval);

    for(auto c : buf)
        cout << c << " ";
  ```

  _ex:_
  ```cpp
    // bunun calismamasi lazim ama calisiyor :)
    const char* get_fmt_str()

    int main()
    {
        auto pfm = get_fmt_str();

        try{
            cout << format(pfm, 57) << "\n";
        }
        catch(const std::exception ex)
        {
            std::cout << "exception caught: " << ex.what() << '\n';
        }
    }

    const char* get_fmt_str()
    {
        return "{:16X}";
    }
  ```

??? I missed
  _ex:_
  ```cpp
  ```

=================================================================================
# custom format
  sunlar yazilmali:
  1. placeholder i parse edecek parse islevi
  2. format islevi

  _ex:_
  ```cpp
    struct Point {
        int mx, my;
    };

    template<>
    class std::formatter<Point> {
    public:
        //typename std::format_parse_context::iterator; // return type
        // auto parse(auto& pc)

        auto parse(std::format_parse_context& pc)
        {
            auto iter = pc.begin();

            for(; iter != pc.end() && *iter != '}'; ++iter) {
                if (*iter == '#')
                    cb_ = true;
                else
                    throw std::format_error({"invalid formatting character!"};
            }

            return iter;
        }

        typename std::format_context::iterator format(const Point& p, std::format_context)
        {
            std::format_to(fc.out(), "{}{},{}{}", (cb_ ? '{' : '('), p.mx, p.my, (cb_ ? '}' : ')'));
        }

    private:
        bool cb_{};
    };

    int main()
    {
        Point p1{2, 6}, p2{3, 5};

        std::format("{:#} {}", p1, p2) << "\n";
       
        try {
            std::format("{:15#} {}", p1, p2) << "\n";
        } 
        catch (const std::exception& ex) {
            std::cout << ex.what() << "\n";
        }
    }
  ```

---
## template, a new feature introduced in C++20
  - fonksiyon parametrelerinin turu auto ile capture edilir
    normalde bu fonksiyon bir template fonksiyon

  ```cpp
    void func(auto x)
    {
       decltype(x) 
    }
  ```

---

  _ex:_
  ```cpp
    // formatlama ozellligi:

    class Person {
    public:
        Person(std::string name, int id) : m_name(std::move(name)), m_id(id) {}

        std::string get_name() const { return m_id; }
        int get_id() const { return m_id; }

    private:
        std::string m_name;
        int m_id;
    };

    template<>
    class std::formatter<Person> {
    public:
        constexpr auto parse(auto& context)
        {
            auto iter{ context.begin() };
            const auto iter_end{ context.end() };

            if(iter == iter_end || *iter == '}')
            {
                m_ftype = FormatType::All;
                return iter;
            }

            switch(*iter)
            {
                case 'n' : m_ftype = Formattype::Name; break;
                case 'i' : m_ftype = Formattype::Id; break;
                case 'a' : m_ftype = Formattype::All; break;
                default  : throw std::format_error{"Person format error!"} ;
            }
            ++iter;
            if(iter != iter_end && *iter != '}') {
                throw std::format_error{"Person format error!"};
            }

            return iter;
        }

        constexpr auto format(const Person& per, auto& context)
        {
            using enumFormatType;

            switch(m_ftype)
            {
                case Name: return std::format_to(context.out(), "{}", per.get_name());
                case Id:   return std::format_to(context.out(), "{}", per.get_name());
                case All:  return std::format_to(context.out(), "[{}, {}]", per.get_name(), per.get_id());
            }
        }

    private:
        enum class FormatType {Name, Id, All};
        FormatType m_ftype;
    };

    int main()
    {
        Person p1{"necati", 862};
        Person p2{"harun", 8912362};
        Person p3{"murat", 7412};

        cout << std::format("{:n}\n{:id}\n{:all}", p1, p2, p3);

        try{
            cout << std::format("{:n}\n{:id}\n{:all}", p1, p2, p3);
        }
        catch (const std::exception& ex) {
            std::cout << ex.what() << "\n";
        }
    }

  ```

============================ 20 featurs in C++20 ======================
1. aggregate ile ilgili gecmisteki bir hata duzeltildi
  - user declared ctor (including delete and default), bu class hala aggreagate kategorisinde

  bu yuzden aggregate init. yapilabilirdi

  _ex:_
  ```cpp
    /// c++17
    struct Nec{
        Nec() = delete;
    };

    // Bu ctor cagirlirsa hat aol
    int main()
    {
        Nex x{};    // error

        Nec y{};    // legal in c++17
                    // since y is an aggreagate type and aggregate structures
                    // can be init. ...
    }
  ```

  > bu yuzden aggregate struct olma sartlari degisti.
  > legacy code degerlendirildiginde syntax error verecek

  sunlar aggreagate olma ozelliginden cikarildi
  - user declared ???

  _ex:_
  ```cpp
  ```cpp
    struct A {
        A() = delete;
    };

    struct B {
        int x{1}
        B(int) = delete;
    };

    struct C {
        int x;
        C();
    };

    C::C() = default;
    

    // None of these are aggreaate type in C++20


    // Bu ctor cagirlirsa hat aol
    int main()
    {
        A a{};  // C+++20 syntax error, C++17 ok
        B b{ 12 };  // C+++20 syntax error, C++17 ok
    }
  ```

## 2. ?? Aggreagate 

  _ex:_
  ```cpp
    struct Nec {
        int x, y;
    };

    int main()
    {
        Nec nec{10, 20};    // legal
        Nec nec(10, 20);    // legal in c++20, not in c++17
    }

    // oyle yerler var ki () kullanildiginda syntax error olur
    // 1 example: perfect forwarding
    // 2: onislemci komultlari
  ```

> aggreagate type nesnelerin macrolara input yapilmasini saglamak

  _ex:_
  ```cpp
    #include <cassert>

    struct Nec { int x, y; };

    int main()
    {
        assert(Nec{1, 5});
            // , burada macro acilimini bozucuk

        assert(Nec(1, 5));
            // this is ok 
    }

    // bu tarz, macro yazimini support etmek icin aggreagte init. e () ile ilk deger verme eklendi
  ```

buna diziler de dahil

  _ex:_
  ```cpp
    int a[]{1, 2, 3};
    int a2[](1, 2, 3);  // in C++20, this is ok
  ```

  _ex:_
  ```cpp
    struct Nec {
        int x, y;
    };

    int main()
    {
        Nec n(1, 2);
    }
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
