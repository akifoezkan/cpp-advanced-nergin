
  _ex:_
  ```cpp
    #include <ranges>
    #include <functional>

    template<std::input_iterator Iter, std::sentinel_for<Iter> s, typename T, typename Proj = std::identity>
    Iter Find(Iter beg, S end, const T& val, Proj proj = {})
    {
        while(beg != end) 
        {
            if (std::invoke(proj, *beg) == val)
                return beg;

            ++ beg;

            // proj yerine bir data member variable gelirse, invoke bunu sarmalar
        }

        return beg;
    }

    int main()
    {
        using namespace std;
        vector ivec{10,  5, 6, 400, 4, 6, 20, 9, 2};

        // sayinin karesi 400 e esit olani bul
        if(auto iter = Find(ivec.begin(), ivec.end(), 400, [](int x) { return x * x;}); iter != ivec.end())
        {
            cout << "bulundu " << *iter << "\n";
            cout << "index = " << distance(ivec.begin(), iter) << "\n";
        }
        else
        {
            std::cout << "bulunamadi\n";
        }
    }


    // S ve Iter ayni tur oladabilir olmayadabilir
  ```

  _ex:_
  ```cpp
    int main()
    {
        auto vec = getvec(100, 0, 1000);

        ranges::sort(vec);
    }
  ```

# borrowed range (c++20)
  - range isteyen algoritmalar input 1) L-val, yada 2) borrowed range olmasi gerekir

  _ex:_
  ```cpp
    int main()
    {
        auto vec = getvec(100, 0, 1000);

        auto iter = ranges::min_element(vec);

        std::cout << "min = " << *iter << "index = " << iter - vec.begin() << "\n";
    }
  ```

 yukaridaki algorithmaya R-val gecelim
  _ex:_
  ```cpp
    int main()
    {
        auto iter = ranges::min_element(getvec(100, 0, 1000));

        // getvec ile dondurulen vector PR-val, dolayisiyla
        // iter is a dangling pointer

        std::cout << "min = " << *iter << "\n";     // syntax error
        
        // iter type: rangess::dangling
        // template metaprogramming sayesinde dereference i syntax error yapti
    }
  ```

## borrowed range:
  oyle bir range ki, iteratorunun dangling olmasi dogrudan mumkun degil

  _ex:_
  ```cpp
    auto vec = getvec(10, 0, 100000);

    span(vec);

    auto iter = ranges::min_element(span(vec));
    std::cout << "min = " << *iter;

    // no syntax error
    // min_element fonksiyona gonderilen turun R-val, ancak borrowed range
    // bu sayede syntax error yok
  ```

### borrowed range turleri:
  1)
  2)

  _ex:_
  ```cpp
    template<>
    inline constexpr bool std::ranges::enable_borrowed_range<Myclass> = true;
  ```

#
  bir numeric algoritmasini ranges algoritmasina donusturelim
  _ex:_
  ```cpp
    template<typenmae Iter, typename Init>
    Init Accumulate1(Iter beg, Iter end, Init init)
    {
        while(beg != end)
        {
            init = std::move(init) + *beg;
            ++beg;
        }

        return init;
    }

    int main()
    {
        auto ivec = getvec(100, 0, 2000);
        print(ivec);

        accumulate(ivec.begin(), ivec.end(), 0);
        std::cout << "val = " << val << "\n";
    }
  ```

  _ex:_
  ```cpp
    // SenType iteratur turunden farkli
    // toplama islemi yerine generik op yi kullandik

    template<typenmae Iter, typename SenType, typename Op = std::plus<>>
    Init Accumulate2(Iter beg, SenType end, Init init, Op op = {})
    requires std::input_iterator<Iter> &&
    std::sentinel_for<Iter, SenType>
    {
        while(beg != end)
        {
            init = op(std::move(init), *beg);
            ++beg;
        }

        return init;
    }
  ```

  _ex:_
  ```cpp
    // template constraint parameter 

    template<std::input_iterator Iter, std::sentinel_for<Iter> SenType, typename Op = std::plus<>>
    Init Accumulate3(Iter beg, SenType end, Init init, Op op = {})
    {
        while(beg != end)
        {
            init = op(std::move(init), *beg);
            ++beg;
        }

        return init;
    }
  ```

  _ex:_
  ```cpp
    // use invoke

    template<std::input_iterator Iter, std::sentinel_for<Iter> SenType, typename Op = std::plus<>>
    Init Accumulate4(Iter beg, SenType end, Init init, Op op = {})
    {
        while(beg != end)
        {
            init = std::invoke(op, std::move(init), *beg);
            ++beg;
        }

        return init;
    }
  ```

  _ex:_
  ```cpp
    // add Init as a default template parameter

    template<std::input_iterator Iter, std::sentinel_for<Iter> SenType, typename Init = std::iter_value_t<Iter>, typename Op = std::plus<>>
    Init Accumulate5(Iter beg, SenType end, Init init = Init{}, Op op = {})
    {
        while(beg != end)
        {
            init = std::invoke(op, std::move(init), *beg);
            ++beg;
        }

        return init;
    }
  ```

  _ex:_
  ```cpp
    // ?? burayi kontrol et 

    template<std::input_iterator Iter, std::sentinel_for<Iter> SenType, typename Init = std::iter_value_t<Iter>, typename Op = std::plus<>, typename Proj = std::identity>
    Init Accumulate6(Iter beg, SenType end, Init init = Init{}, Op op = {}, Proj proj = {})
    {
        while(beg != end)
        {
            init = std::invoke(op, std::move(init), std::invoke(proj, *beg));
            ++beg;
        }

        return init;
    }

    template<std::ranges::input_range R, class Init = std::ranges::range_value_t<R>, typename OP = std::plus<>, typename Proj = std::identity>
    Init Accumulate(R &&r, Init init = Init{}, OP op = {}, Proj proj = {})
    {
        return Accumulate(std::ranges::begin(r), std::ranges::end(r), std::move(init), std::move(op), std::move(proj));
    }


    // nicin: const R& r degil R&& r
    // cunku bazi durumlarda const ???
  ```
20.55
# subrange
??
  _ex:_
  ```cpp
    int main()
    {
        auto ivec = getvec(20, 0, 100);

        ranges::subrange sr1{ ivec };   // boylece view olma kosullarini saglayacak

        cout << sizeof(sr1) << "\n";    // 8

        cout << sr1.size() << "\n";
        cout << sr1.data() << "\n";
        cout << sr1.front() << "\n";

        sr1.advance(5);
        cout << sr1.size() << "\n";

        ranges::subrange sr{ivec.begin(), ivec.end()};
    }
  ```

  _ex:_
  ```cpp
    template<typename Iter>
    void func(Iter beg, Iter end)
    {
        subrange sr{beg, end};
    }

    int main()
    {
        auto ivec = getvec(20, 0, 100);

        ranges::subrange sr{ivec.begin(), ivec.end()};

        for(auto x : sr)
            std::cout << x << " ";

        for(auto iter = sr.begin(), ??)

        ranges::for_each(sr, [](int x){ cout << x << " "; });

        auto [beg, end] = sr;
    }
  ```

# view siniflari
  - hafif siklet range
  - kopyalama tasima maliyeti son derece dusuk
  - composable

  _ex:_
  ```cpp
    std::ranges::filter_view 

    std::ranges::views::filter

    std::views::filter
  ```

## view types:
- filter_view
- transform view
- take_view
- take_while_view
- drop_view
- drop_while_view
- reverse_view
- join_view
- split_view
- common_ciew
- basic_istream_view
- keys_view
- values_view

## range factories (view factories)
bunlar dogrudan range in sahibi

- iota_view
- single_view
- empty_view
- basic_istream_view

### filter
 - bir predicate a bagli olarak bir filtre olustururu

  _ex:_
  ```cpp
    auto ivec = getvec(100, 0, 100);

    ranges::filter_view vw{ivec, [](int x) {return x % 5 == 0;}};

    // yada range_adaptor
    views::filter(ivec, [](int x)  {return x % 5 == 0;} );

  ```

gonderile range nesnesisinin kendisi de bir view olabilir
  _ex:_
  ```cpp
    auto ivec = getvec(100, 0, 100);
    auto vw = ivec | views::filter([](int x) {return x %5 == 0;});

    for(auto i : vw) {
        cout << i << " ";
    }
  ```

  _ex:_
  ```cpp
    auto ivec = getvec(100, 0, 100);
    ranges::transform_view(auto vw = ivec | views::filter([](int x) {return x %5 == 0;}); 
    ???
  ```

  _ex:_
  ```cpp
    auto ivec = getvec(100, 0, 100);

    views::transform(views::filter(ivec, [](int x) { return x % 5 == 0;}), [](int x) { return x * x - 1; }));
    ???
  ```

  _ex:_
  ```cpp
    auto vw = views::transform(views::filter(ivec, [](int x) { return x % 5 == 0;}), [](int x) { return x * x - 1; })
    auto iter = ranges::max_element(vw);

    cout << * iter << "\n";
  ```

## take adaptor
  - range adaptor
  - take_view dodurur
  - ilk n element

  _ex:_
  ```cpp
    auto ivec = getvec(40, 0, 100);
    print(ivec);

    for(auto i : views::take(ivec, 10))
        std::cout << i << " ";

    // prints firs 10 elements
  ```

## drop adaptor
  - ilk n tane elemani range den cikarir
  _ex:_
  ```cpp
    auto ivec = getvec(40, 0, 100);
    print(ivec);

    for(auto i : views::drop(ivec, 10))
        std::cout << i << " ";
  ```

## drop_while_view, take_while_view
  - kosul saglanana kadar ki ilk oge alinir veya birakilir

  _ex:_
  ```cpp
    auto ivec = getvec(20, 0, 100);
    print(ivec);

    int ival;

    cout <<  "degeri girin: "
    for(auto i : ivec | views::take_while([ival](int x) { return x > ival; })) 
    {
        cout << i << " ";
    }

    ??
  ```

  _ex:_
  ```cpp
    list<string> slist;
    rfill(slist, 20,rname);
    print(slist);

    size_t len;
    cout << "uzunluk degeri girin: ";
    cin >> len;

    auto v = views::take_while(slist, [len](const string& s){ return s.size() > len; });
    // auto v = views::drop_while(slist, [len](const string& s){ return s.size() > len; });

    ranges::for_each(v, [](const auto& s)) {
        std::cout << quoted(s) << " ";
    }
  ```

## views are lazy
  _ex:_
  ```cpp
    auto odd = [](int x)
    {
       cout << "odd => x = "  << x << "\n";

       return x % 2 != 0;
    };

    auto cube = [](int y) {
        std::cout << "cube => y = " << y << "\n";
        return y * y * y;
    };

    auto vec = getvec(20, 0, 30);

    auto v = vec | views::filter(odd) | views::transform(cube);

    // bu noktaya kadar herhangi bir execution yok
    // ne zaman ki biz bu range i dolasiriz, o zaman is yapilir

    // pull method / lazy evaluation
    for(auti i : v) {
        std::cout << "i = " << i << "\n";
    }
  ```

---
filter
transform
take_while
drop_while
---

## reverse
  _ex:_
  ```cpp
    list<string> slist;
    rfill(slist, 10, rname);
    print(slist);

    auto v = views::reverse(slist) | views::take(5);

    for(auto x : v)
        std::cout << x << " ";
  ```

# range factories
## iota
  ```cpp
    ranges::iota_view   // name of the calss
    views::iota()       // name of hte range
  ```

  _ex:_
  ```cpp
    auto v = views::iota(10);

    for(auto x : v)
    {
        cout << x << " ";
        getchar();
    }


    // 10 dan baslayarak degerler??
  ```

  _ex:_
  ```cpp
    auto v = views::iota(10, 20);

    for(auto x : v)
    {
        cout << x << " ";
        getchar();
    }

    // 10 ile 20 arasi degerler ???
  ```

  _ex:_
  ```cpp
    auto v = views::iota(10);

    for(auto x : views::take(20))
    {
        std::cout << x << " ";
    }
  ```

  _ex:_
  ```cpp
    for(auto x : views::iota(10) |  views::transform([](int x) { return x * 2;}) | views::take(20))
    {
        std::cout << x << " ";
    }
  ```

  _ex:_
  ```cpp
    std::string trim(std::string s)
    {
       auto v = s | views::drop_while(std::isspace) 
                  | views::reverse
                  | views::drop_while(std::isspace)
                  | views::reverse;

       return{v.begin(), v.end()};
       string str {" \t\nmustafa aksoy\t\n"};

       cout << "|" << trim(str) << "|";
    }
  ```

  _ex:_
  ```cpp
    #include <Date>

    int main()
    {
        auto v = views::iota(Date{25, 12, 2021}, Date{5, 4, 2022}) | 
                  views::transform([](const Date& date) { return date + 5; });

        //views::filter([](const Date& date){ return date.week_day() > 3});

        for(const auto &d : v)
            cout << d " ";
    }
  ```

## common range
  elimizde bir range var
  a) bu range i kalsik bir algorithmaya gonderecegiz (eski alg. 2 iterator ister)
   
   range: iterator, sentinel turleri farkli
   eski algoritmakar: begin, end iterator ayni tur

  ne yapacagiz?

### common range concept
  _ex:_
  ```cpp
    list mylist{1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto v = views::take(mylist, 5);

    constexpr auto b = ranges::common_range<decltype(mylist)>;  // true?
    
    constexpr auto b = ranges::common_range<decltype(v)>;   // false

    same_as<decltype(v.begin()), decltype(v.end())> // false

    // vector<int> ivec{v.begin(), v.end()};   // syntax error, different types
  ```

bir range i kullanarak bir vrector olusturalim
  _ex:_
  ```cpp
    list mylist{1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto v = views::take(mylist, 5);

    auto v2 = views::common(v);
    vector<int> ivec{v2.begin(), v2.end()};
  ```

  _ex:_
  ```cpp
    list mylist{1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto v = views::take(mylist, 5);

    //std::find(v.begin(), v.end(), 3);   // syntax error

    auto v2 = views::take(mylist, 5) | views::common;

    //std::find(v.begin(), v.end(), 3);   // ok 
  ```

## counted
  _ex:_
  ```cpp
    auto ivec = getvec(10, 0, 1000);
    print(ivec);

    for(auto val : views::counted(ivec.begin(), 4))
        std::cout << val << " ";
  ```

  _ex:_
  ```cpp
    auto ivec = getvec(10, 0, 1000);
    print(ivec);
    
    for(counted_iterator iter{ivec.begin(), 5}); iter.count() > 0; ++iter)
        cout << *iter << " ";

    for(counted_iterator iter{ivec.begin(), 5}); iter != default_sentinel; ++iter)
        cout << *iter << " ";
  ```

  _ex:_
  ```cpp
    auto v = views::iota(10) | 
             views::take(20) | 
             views::reverse |
             views::transform([](int x) {return x * 10; }) | 
             views::common;

    vector<int> ivec {v.begin(), v.end()};

    print(ivec);
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
