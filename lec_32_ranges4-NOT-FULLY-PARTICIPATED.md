# 2 questions
##
## declval

  _ex:_
  ```cpp
    template<typename T>
    typename std::add_rvalue_reference_t<T> Declval() noexcept;


    class Myclass{
    public:
        double func();
        Myclass() = delete;
    };

    int main()
    {
        decltype(Myclass{}.func()) x{};             // Myclass ctor is deleted??
        
        decltype(Declval<Myclass>().func()) x{};    // ok
    }
  ```

=============================== Ranges-3
#
  _ex:_
  ```cpp
    #include <ranges>

    template<std::ranges::input_range Rng, typename Projection = std::identity>
    std::ranges::range_value_t<Rng> gmax(Rng&& rg, Projection proj = {})
    {
        if(std::ranges::empty(rg))
        {
            return std::ranges::range_value_t<Rng>{};
        }

        auto pos = std::ranges::begin(rg);
        auto max_iter{ pos };

        while(++pos != std::ranges::end(rg))
        {
            if(std::invoke(proj, *pos) > std::invoke(proj, *max_iter))
            {
                max_iter = pos;
            }
        }

        return *max_iter;
    }

    int main()
    {
        using namespace std;

        vector ivec{2, -4, 8, 17, -90, 40, 34, 13, 4};
        auto val = gmax(ivec, [](int x) { return abs(x); });

        std::cout << "val = " << val << "\n";
    }
  ```

  _ex:_
  ```cpp
    auto f { [](int x) {return x * 10;}};
    
    //constexpr auto v = ranges::transform_view{ ranges::reverse_view{ views::iota{1, 30} }, f};
    constexpr auto v = views::iota(1, 30) | views::transform(f);

    vector<int> ivec{ivec.begin(), ivec.end()};

    ??
  ```

## compile time palindrome test
  _ex:_
  ```cpp
    #include <string_view>

    // kalas yok kutuk koy salak
    // at sahibi gibi hasta!
    // para hazir ama Riza harap!
    // Ana, ne dedi Naci dede Canan'a
    // A man, a plan a canal: Panama!

    [[nodiscard]] constexpr bool  is_palindrome(const std::string_view sv)
    {
        auto first_part = sv | view::take(sv.size() / 2);
        auto second_part = sv | std::views::reverse | std::views::take(sv.size() / 2);

        return std::ranges::equal(first_part, second_part);
    }

    int main()
    {
        constexpr auto b = is_palindrome("ey edip adanada pide ye");    // compile time: true
    }
  ```

## keys, values
  _ex:_
  ```cpp
    #include<map>
    #include<ranges>
    #include<iostream>

    int main()
    {
        using namespace std;

        map<string, int> simap;
        rfill(simap, 20, []{ return pair{rname() + ' ' + rfname(), Irand{0, 10'000}()}; });
        print(simap, "\n");

        auto v = simap
                 | views::filter([](const auto& s) { return s.first.length() > 12; })
                 | views::take(3)
                 | views::keys;
                 // | views::values;
                 // | views::values;

        for(const auto &elem : v)
            cout << elem << "\n";
    }
  ```

  _ex:_
  ```cpp
    using namespace std;

    vector<pair<int, string>> isvec;
    for(int i = 0; i < 30; ++i)
    {
        isvec.emplace_back(Irand{0, 3000}, rname());
    }

    print(isvec, "\n");
    size_t len;
    cout << "uzunluk: ";
    cin >> len;

    auto pred = [len](const string& s) { return s.length() == len; };
    
    for(const auto& s : isvec | views::values | views::filter(pred))
    {
        cout << s << ' ';
    }
  ```

  _ex:_
  ```cpp
    using namespace std;

    using Person = tuple<int, string, string, double>;
    vector<Person> pvec;

    Irand irand{0, 100'000};
    Drand drand{15., 99.99};

    for(int i = 0; i < 100; ++i)
    {
        pvec.emplace_back(irand(), rname(), rfname(), drand())
    }

    auto vw = views::elements<0>(pvec);
    for(const auto& x : vw)
    {
        cout << x << " ";
    }

    auto vw = views::elements<1>(pvec);
    for(const auto& x : vw)
    {
        cout << x << " ";
    }

    auto vw = views::elements<2>(pvec);
    for(const auto& x : vw)
    {
        cout << format("{:.3f}\n", x);
    }
  ```

## common
  _ex:_
  ```cpp
    template<typename Iter>
    void algo(Iter beg, Iter end)
    {
        while(beg != end)
        {
            std::cout << *beg++ << ' ';
        }
        std::cout << "\n";
    }

    template<typename TBeg, typename TEnd>
    void call_algo(TBeg beg, TEnd end)
    {
        if constexpr (std::same_as<TBeg, TEnd>)
        {
            algo(beg, end);
        }
        else
        {
            using iter_type = std::common_iterator<decltype(beg), decltype(end)>;
            algo(iter_type{beg}, iter_type{end});
            std::cout << "\n";
            auto v = std::views::common(std::ranges::subrange(beg, end));
            algo(v.begin(), v.end());
        }

    }

    int main()
    {
        list ilist{1, 4, 7, 2, 3, 2, 1, 8, 7, 6};
        auto v = views::take(ilist, 6);

        call_algo(v.begin(), v.end());
    }
  ```

## split
  _ex:_
  ```cpp
    using namespace std;
    vector<int> ivec{7, 82, 3, 4, i7, 8, 2, 7, 4, 12, ...., 4};

    auto spv = views::split(ivec, 4);

    for(const auto& split : spv)
    {
        ranges::copy(split, ostream_iterator<int>{cout, " "}>);
        cout << "\n";
    }
  ```

  _ex:_
  ```cpp
    using namespace std;
    vector<int> ivec{7, 82, 3, 4, 7, 8, 2, 7, 4, 12, ...., 4};
    array sep{3, 4, 7};

    auto spv = views::split(ivec, sep);     // ardisik 3, 4, 7 dizisini arar

    for(const auto& split : spv)
    {
        ranges::copy(split, ostream_iterator<int>{cout, " "}>);
        cout << "\n";
    }
  ```

## 
  _ex:_
  ```cpp
    #include <sstream>
    #include<string>
    #include<ranges>

    int main()
    {
        using namespace std;

        istringstream iss{"akif murat kadir harun erkan sercan"};
        
        for (const auto& s : ranges::istream_view<string>(iss)) {
            cout << s << " ";
        }

    }
  ```

  _ex:_
  ```cpp
    #include <fstream>
    #include <iostream>
    #include<format>
    #include "nutility.h"

    int main()
    {
        using namespace std;

        ofstream ofs{ "necati.txt" };
        if(!ofs)
        {
            cerr << "dosya olusturulamadi\n";
            return 1;
        }

        Drand drand{0., 9.99};

        for(int i = 0; i < 1000; ++i)
        {
            if(i && i % 10 == 0)
                ofs << "\n";
            ofs << format("{:.4f} ", drand());
        }
    }
  ```

  _ex:_
  ```cpp
    #include <fstream>
    #include <iostream>
    #include<format>
    #include "nutility.h"

    int main()
    {
        using namespace std;

        ofstream ifs{ "necati.txt" };
        if(!ifs)
        {
            cerr << "dosya olusturulamadi\n";
            return 1;
        }
        
        double sum{};
        ranges::for_each(ranges::istream_view<double>(ifs), [&sum](double d) { sum += d;});

        cout << "sum = " << sum << "\n";
    }
  ```

  _ex:_
  ```cpp
    template<std::ranges::view T>
    void vw_print(T x)
    {
        for(auto iter = std::ranges::begin(x); iter != std::ranges::end(x); ++iter) {
            cout << *iter << ' ';
        }
        cout << "\n\n";
    }

    int main()
    {
        vector<string> svec;
        rfill(svec, 100, rtown);
        print(svec);
        (void)getchar();

        cout << "karakter girin: ";
        cin >> c;
        const auto pred = [c](const string& s) { return s.find(c) != string::npos; };

        vw_print(views::all(svec) || views::filter(pred));
    }
  ```
=======================
# SOLID, GRASP
# design patterns:

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
