# 1 saat geciktim    
## projection
  _ex:_
  ```cpp
    // Icindeki 1 sayisi cok olandan az olana
    vector<bitset<32>> bvec;

    mt19937 eng{ random_device{}() };

    for(int i{}; i < 10'0000; ++i)
    {
        bvec.emplace_back(eng)
    }

    ranges::sort(bveci, {}, [](const bitset<32>& bs)
    {
        return bs.count();
    });

    ofstream ofs{"out.txt"};
    
    print(bvec, "\n", ofs);
  ```

  _ex:_
  ```cpp
    vector<string> svec;

    ranges::find(svec, "ali");
    
    ranges::find(svec, tu, [](const string &s) {return s.length(); });
  ```
##

  _ex:_
  ```cpp
    vector<Date> dvec;
    rfill(dvec, 100, Date::random);

    ranges::sort(dvec, {}, [](const Date&d) { return d.month_day(); });
    
    ranges::sort(dvec, {}, [](const Date& d) { return d.month_day(); });

    print(dvec, "\n");
  ```

# bu konuyla ilgili en kaliteli talk: christian
  -> Tristan Brindle
 
  _ex:_
  ```cpp
    struct Employee {
        std::string name;
        int id;
    };

    struct Payslip {
        std::string pay_info;
        int id;
    };

   // Her Employee icin bir payslip olacak

    int main()
    {
        vector<Employee> evec(100);
        vector<Payslip> pvec(100);

        sort(evec.begin(), evec.end(), [const Employee &e1, const Employee& e2]) {
            return e1.id < e2.id;
        });

        sort(pvec.begin(), pvec.end(), [const Payslip& p1, const Payslip& p2]) {
            return p1.id < p2.id;
        });

        auto b = equal(evec.begin(), evec.end(), pvec.begin(), pvec.end(), [](const Employee &e, const Payslip& p)) {
            return e.id == p.id;
        }
    }
  ```

=> use ranges library
  _ex:_
  ```cpp
    struct Employee {
        std::string name;
        int id;
    };

    struct Payslip {
        std::string pay_info;
        int id;
    };

   // Her Employee icin bir payslip olacak

    int main()
    {
        vector<Employee> evec(100);
        vector<Payslip> pvec(100);

        ranges::sort(evec, [const Employee &e1, const Employee& e2]) {
            return e1.id < e2.id;
        });

        ranges::sort(pvec, [const Payslip& p1, const Payslip& p2]) {
            return p1.id < p2.id;
        });

        auto b = ranges::equal(evec, pvec, [](const Employee &e, const Payslip& p)) {
            return e.id == p.id;
        }
    }
  ```
=> use projection
  _ex:_
  ```cpp
    struct Employee {
        std::string name;
        int id;
    };

    struct Payslip {
        std::string pay_info;
        int id;
    };

   // Her Employee icin bir payslip olacak

    int main()
    {
        vector<Employee> evec(100);
        vector<Payslip> pvec(100);

        ranges::sort(evec, ranges::less{}, [](const Employee& e) { return e.id; });
        
        ranges::sort(pvec, ranges::less{}, [](const Payslip& p) { return p.id; });

        //auto b = ranges::equal(evec, std::ranges:equal_to{}, [](const Employee &e) {return e.id; }, [](cinst Payslip& p ){ return p.id; });
        
        //  equal_to is default
        auto b = ranges::equal(evec, {}, [](const Employee &e) {return e.id; }, [](cinst Payslip& p ){ return p.id; });
  ```

=> use data member pointer, let invoke do the rest
  _ex:_
  ```cpp
    struct Employee {
        std::string name;
        int id;
    };

    struct Payslip {
        std::string pay_info;
        int id;
    };

   // Her Employee icin bir payslip olacak

    int main()
    {
        vector<Employee> evec(100);
        vector<Payslip> pvec(100);

        ranges::sort(evec, ranges::less{}, &Employee::id);
        
        ranges::sort(pvec, ranges::less{}, &Payslip::id);
        
        //  equal_to is default
        auto b = ranges::equal(evec, pvec, {}, &Employee::id, &Payslip::id); ??
  ```

  _ex:_
  ```cpp
    struct Employee {
        Employee() = default;
        Employee(std::string name, std::string surname, int id) : m_name{std::move(name)}, m_surname{std::move(surname)}, m_id{id} {}
        
        auto operator<=>(const Employee&) const = default;
        // note: spaceship siralamayi name e gore yapar, cunku ilk o tanimli


        std::string m_name;
        std string m_surname;
        int m_id{};
    };

    int main()
    {
        vector<Employee> evec;

        for(int i = 0; i < 1000; ++i) 
        {
            evec.emplace_back(rname(), rfname(), Irand{0, 100000}());
        }

        sort(evec.begin(), evec.end());

        for (const auto& e : evec)
        {
            cout << std::format("{:<12} {:<16} {:<16}", e.m_id, e.m_name, e.surname) << "\n";
        }
    }

    // cpp20
    int main()
    {
        vector<Employee> evec;

        for(int i = 0; i < 1000; ++i) 
        {
            evec.emplace_back(rname(), rfname(), Irand{0, 100000}());
        }

       ranges::sort(evec, {}, &Employee::m_name);
       //ranges::sort(evec, {}, &Employee::m_surname);
       //ranges::sort(evec, {}, &Employee::m_id);

       ranges::sort(evec, {}, &[](const Employee e) {
            return e.m_name + e.m_surname;
       });

       for (const auto& e : evec)
       {
           cout << std::format("{:<12} {:<16} {:<16}", e.m_id, e.m_name, e.surname) << "\n";
       }
    }
  ```

  _ex:_
  ```cpp
    struct Employee {
        Employee() = default;
        Employee(std::string name, std::string surname, int id) : m_name{std::move(name)}, m_surname{std::move(surname)}, m_id{id} {}
        friend std::ostream& operator<<(std::ostream&os, const Employee& e)
        {
            return os << '{' << e.m_id << ", " << e.m_name << ", " << e.m_surname;
        }
        
        auto operator<=>(const Employee&) const = default;
        // note: spaceship siralamayi name e gore yapar, cunku ilk o tanimli


        std::string m_name;
        std string m_surname;
        int m_id{};
    };

    // Bir range i standard cikis akimina veren bir fonksiyon sablonu
    template<std::ranges::input_range R, typename Projection = std::identity>
    void Print(R &&r, Projection proj = {})
    {
        std::ranges::for_each(r, [] (const auto& e){std::cout << e << ' '; }, proj);
        std::cout << '\n';
    }

    int main()
    {
        vector<Employee> evec;

        for(int i = 0; i < 1000; ++i) 
        {
            evec.emplace_back(rname(), rfname(), Irand{0, 100000}());
        }

        sort(evec.begin(), evec.end());

        for (const auto& e : evec)
        {
            cout << std::format("{:<12} {:<16} {:<16}", e.m_id, e.m_name, e.surname) << "\n";
        }
    }

    // cpp20
    int main()
    {
        vector<Employee> evec;

        for(int i = 0; i < 1000; ++i) 
        {
            evec.emplace_back(rname(), rfname(), Irand{0, 100000}());
        }

       ranges::sort(evec, {}, &Employee::m_name);
       //ranges::sort(evec, {}, &Employee::m_surname);
       //ranges::sort(evec, {}, &Employee::m_id);

       ranges::sort(evec, {}, &[](const Employee e) {
            return e.m_name + e.m_surname;
       });

       for (const auto& e : evec)
       {
           cout << std::format("{:<12} {:<16} {:<16}", e.m_id, e.m_name, e.surname) << "\n";
       }
    }
  ```

====================================== views ============================
20.01
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
