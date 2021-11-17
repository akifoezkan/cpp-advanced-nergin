soru: TypeTeller

# policy selection - if constexpr
  - Alternative SFNIAE

  ```cpp
    // compile-time code selection for int and others
    template<typenmae T>
    void func(T x)
    {
        // if constexpr(sizeof(T) > 2) 

        if constexpr(stD::is_integral_v<T>)
        {
            std::cout << "true path\n";
        }
        else
        {
            std::cout << "false path\n";
        }
    }

    func(12);   // true path
    func('A');  // false path
  ```

  ```cpp
    // compile-time code selection for int and others
    template<typenmae T>
    void func(T x)
    {
        if constexpr(std::is_integral_v<T>)
        {
            std::cout << "integral types\n";
        }
        else if constexpr (std::is_floating_point_v<T>)
        {
            std::cout << "real numbers\n";
            *x = 12;  // NOT a SYNTAX ERROR
                      // since this is not the selected code
        }
        else
        {
            std::cout << "other types\n";
        }
    }

    func('A');
  ```

## examples 
  -> cpp codes course

  > if constexpr blogunda kodun girmedigi kisimlara cok kisitli bir kontrol yapilir (1.faz)

  ```cpp
    #include <string>
    #include <type_traits>
    
    template <typename T> 
    std::string as_string(T x)
    {
    	if constexpr (std::is_same_v<T, std::string>) {
    		return x;
    	}
    	else if constexpr (std::is_arithmetic_v<T>) {
    		return std::to_string(x);
    	}
    	else {
    		return std::string(x); 
    	}
    }
    
    #include <iostream>
    
    int main()
    {
    	std::cout << as_string(42) << '\n';   // int part
                                            //  -> std::string(int) normalde syntax erros
                                            // ancak constexpr else bloguna girmedigi icin compiler oraya bakmadi

    	std::cout << as_string(4.2) << '\n';  // artihmetic part
                                            // compiler 
    	std::cout << as_string(std::string("hello")) << '\n';
    	std::cout << as_string("hello") << '\n';
    }
  ```

  > static_assert if_constexpr nin secilmeyen kullanilsa bile hata verir.
  > isim arama birinci fazda yapilir: kod oraya girmese de hata verir
    - template parametresine bagli olmayan isimler???
  ```cpp
    #include <type_traits>

    template <typename T>
    void func(T tx)
    {
    	if constexpr (std::is_integral_v<T>) {
    		if (tx != 0) {
    			func(tx--);
    			//...
    		}
    	}
    	else {
    		//undeclared_f(); //syntax error
                          // isim arama ilk fazda yapilir.
                          // bu fonksiyon tanimli olmadigi icin else e girmese de syntax error verir

    		//undeclared(f);  // ok
                          // f template parametresine bagli bir isim

    		//static_assert(false, "not integral type"); //syntax error
            // kod else bloguna girmese de hata verir.
    	}
    }
    
    int main()
    {
    	func<double>(2);
    }
  ```



  ```cpp
    #include <type_traits>

    template <typename T>
    void func(T& tx)
    {
    	if (tx > 0) {
    		if constexpr (std::is_integral_v<T>) {
    			++tx;
    		}
    		else {
    			--tx;
    		}
    	}
    }
    
    #include <iostream>
    
    int main()
    {
    	int ival = 5;
    	double dval = 2.5;

    	func(ival);   // if, ival = 6
    	func(dval);   // else, ival = 1.5
    
    	std::cout << "ival = " << ival << "\n";
    	std::cout << "dval = " << dval << "\n";

    	int ival = - 5;
    	func(ival);     // none, ival = -5 
    }
  ```

  > if constexpr ile farkli return type elde edebiliriz
  ```cpp
    constexpr auto func()
    {
    	if constexpr (sizeof(int) > 4u) {
    		return 1;     // return type is int
    	}
    	else {
    		return 0.5;   // return type is double
    	}
    }
    
    int main()
    {
    	constexpr auto val = func();
    }

  ```

  > if constexpr tag dispatch e bir alternatif olusturur
  ```cpp
    #include <iterator>
    
    template <typename Iter, typename Dist>
    void Advance(Iter& pos, Dist n)
    {
    	using Cat = typename std::iterator_traits<Iter>::iterator_category;
    	Advance_impl(pos, n, Cat{});    // tag dispatch
                                      // iterator category sine bagli olarak operator overloading
    }
    
    //implementation for random access iterators
    template <typename Iter, typename Dist>
    void Advance_impl(Iter& pos, Dist n, std::random_access_iterator_tag)
    {
    	pos += n;
    }
    
    //implementation for random bidirectional iterators
    template <typename Iter, typename Dist>
    void Advance_impl(Iter& pos, Dist n, std::bidirectional_iterator_tag)
    {
    	if (n >= 0) {
    		while (n--)
    			++pos;
    	}
    	else {
    		--pos;
    	}
    }
    
    //implementation for input iterators
    template <typename Iter, typename Dist>
    void Advance_impl(Iter& pos, Dist n, std::input_iterator_tag)
    {
    	while (n--) {
    		++pos;
    	}
    }

  // point of instantiation
    // Note that advance_impl nerede instantiate edilecekse orada gorulecek
    //  ???? -> ama bu kod calismadi

    vector<int> ivec(100);
    auto iter = ivec.begin();

    Advance(iter, 10);
  ```

  > yukaridaki kod asagidaki sekilde yazilarak kolaylastirilabilir

  ```cpp
      #include <iterator>
      #include <type_traits>
      
      template <typename Iter, typename Dist>
      void advance(Iter& pos, Dist n)
      {
      	using cat = typename std::iterator_traits<Iter>::iterator_category;
      
      	if constexpr (std::is_same_v<cat, std::random_access_iterator_tag>) {
      		pos += n;
      	}
      	else if constexpr (std::is_same_v<cat, std::bidirectional_iterator_tag>) {
      		if (n >= 0) {
      			while (n--)
      				++pos;
      		}
      		else {
      			while (n++) {
      				--pos;
      			}
      		}
      	}
      	else { //input iterator tag
      		while (n--)
      			++pos;
      	}
      }
  ```


  > Ortada template a bagli bir kod olmadiginda ``constexpr if`` butun kisimlari syntax kontrole tabi
  ```cpp
      //usage in non template function
      struct Nec {
      
      };

      
      int main()
      {
      	Nec nec;
      
      	if constexpr (sizeof(int) > 4) {
      		++nec;  //syntax error
      	}
      	else {
      		--nec;   //syntax error, template a bagi degil
      	}
      }
  ```



  ```cpp
    // structured binding get fonksionu yazilmazsa calismayacak
    #include <string>

    struct Nec {
        int ival;
        double dval;
        std::string s;
    };
    
    template<size_t n>
    auto& get(Nec& nec);
    
    template <> auto& get<0>(Nec& s) { return s.ival; }
    template <> auto& get<1>(Nec& s) { return s.dval; }
    template <> auto& get<2>(Nec& s) { return s.s; }
    
    Nec foo()
    {
        return { 12, 4.5, "salih"};
    }
    
    #include <iostream>
    
    int main()
    {
        auto [i, d, name] = foo();
    
        std::cout << i << " " << d << " " << name << "\n";
    }
  ```
  yukaridaki fonksiyon constexpr if ile daha kolay yazilabilir

  ```cpp
    #include <string>
    
    struct Nec {
        int ival;
        double dval;
        std::string s;
    };
    
    template<size_t n>
    auto& get(Nec& nec)
    {
        if constexpr (n == 0)
            return nec.ival;
        else if constexpr (n == 1)
            return nec.dval;
        else
            return nec.s;
    }
    
    
    Nec foo()
    {
        return { 12, 4.5, "salih"};
    }
    
    #include <iostream>
    
    int main()
    {
        auto [i, d, name] = foo();
    
        std::cout << i << " " << d << " " << name << "\n";
    }
  ```

  ```cpp
    // constexpr if kod yazimini kolaylastirir
    template<int N>
    constexpr int fibonacci() { return fibonacci<N - 1>() + fibonacci<N - 2>(); }
    
    template<>
    constexpr int fibonacci<1>() { return 1; }
    
    template<>
    constexpr int fibonacci<0>() { return 0; }
    
    
    template<int N>
    constexpr int fibo()
    {
        if constexpr (N >= 2)
            return fibo<N - 1>() + fibo<N - 2>();
        else
            return N;
    }
    
    int main()
    {
        constexpr auto x = fibonacci<8>();
        constexpr auto y = fibo<8>();
    }
  ```

  ```cpp
    auto func(int x)
    {
        if (x > 10)
          return 1;
        else
          return 2;

        // this is redundant else, yazmasak olur
    }

    // ayni sey constexpr if de gecerli degil
    auto func(int x)
    {
        if constexpr(sizeof(int) > 4)
          return 1;

        // burada else yazilmadigi icin fonk geri donus degeri void 
    }

    auto x = func();  // x = int
    
    auto x = func();  // false, x = void, compile time error

  ```

  ```cpp
    #include <iostream>
    #include <memory>
    
    template <typename T>
    auto getval(T t)
    {
        if constexpr (std::is_pointer_v<T>)
            return *t;
        else
            return t;
    }
    
    int main()
    {
        int ival{ 87 };
        double dval{ 4.5 };
        int* iptr{ &ival };
        double* dptr{ &dval };
    
        std::cout << getval(ival) << "\n";
        std::cout << getval(iptr) << "\n";  // ptr
        std::cout << getval(dval) << "\n";
        std::cout << getval(dptr) << "\n";  // ptr
    
    }
  ```

---
## type traits:: conditional

  ```cpp
    template<typename T>
    void func(T x)
    {
        // type bir kosula bagladin
        // kosul dogruysa int, degilse double
        std::conditional_t<std::is_integral_v<T>, int, double> y{};

        std::cout << typeid(y).name() << "\n";
    }

    func(12);     // int
    func(1.2);    // double

  ```


  ```cpp
    template<bool b, typename T, typenme U>
    struct Conditional {
        using type = U;
    };

    template<typename T, typenme U>
    struct Conditional<true, T, U> {
        using type = T;
    };

    template<bool b, typename T, typename U>
    using Conditional_t = Conditional<b, T, U>::type;

    Conditional<true, int, double>::type x;
    Conditional<false, int, double>::type x;
  ```
---
============================== variadic functions ===============
# variadic function templates
  -> fonksiyona istedigimiz kadar arguman gonderebilelim


## C deki variadic fonksiyonlarin dezavantajlari (C++ da gecerli):
  - type safe degil
  - istedgim sayida arguman gonderebilirim.
    Ancak kac tane arguman gonderdigimi de bir sekilde belirtmeliyim
  
  ex:
  ```c
    void func(int x, ...);  // en az 1 arguman olacak
          // -> void func() gecerli degil

    // variadic parametreden onceki tum argumanlara deger gondermek zorundayiz

    // kac parametre gonderdigimizi nasil anlariz?
    int main()
    {
      func(5, x, y, z, t, a); // 5 tane arguman gonderecegim

      //dolayli belirtmek
      func(x, y, z, t, a, ); // son parametre ozel deger '\0' 

      func("%d %d %d"); // const char i parse edip kac arguman oldugunu anlayacagiz

    }
  ```

##  variadic templates:parameter pack
  - typename kullanidliginda type, non type parametre icin de kullanilabilir
  - 0 dahil n tane olabilecek parametre

  ```cpp
      template<typename T, typename ...Args>
      template<typename T, typename ...Types>
      template<typename T, typename ...Ts>
  ```


  ```cpp
    template<typename ...Ts>
    void func(Ts ...args)
    {

    }

    // suna benzer calisir
    template<typename T, typename, typename K>
    void func(T p1, U p2, K p3);


    func(1, 2.5, 3L);  // 3 arguman, types: int double long
                    // fonksiyonun parametre paketinde 3 arguman var
                    // her bir parametre icin deduction yapilacak

    func(); // parametre paketinde 0 parametre var (parametre yok)
  ```



  ```cpp
    template<typename ...Ts>
    void func(Ts ...args)

    template<typename ...Ts>
    void func2(Ts &...args)

    template<typename ...Ts>
    void func2(const Ts &...args);  // arguments const ref

    template<typename ...Ts>
    void func4(Ts &&...args); // forwarding reference


    int x{};
    double d{};
    func2(x, d);   // parameter pack: int, double
                  // func degiskenleri int&, double&
  ```


## sizeof can be used to learn number of parameters in a parameter pack
  - compile time
  - both ...Types, or ...args can be used

  ```cpp
    template<typename ...Ts>
    void func(Ts&& ...args)
    {
        constexpr auto n1 = sizeof...(args);
        constexpr auto n2 = sizeof...(Ts);

        std::cout << n1 << " " << n2;
    }


    func(int{}, double{}, 3, 'A');  // 4
  ```

## fonksiyonun parametre paketi tek basina bulunmak zorunda degil
  - baska tempalte parametrelereine bagli da olabilirler

  ```cpp
      template<typenmae T, typenmae ...Args>
      void func(T x, Args ...args);

      int main()
      {
          func(1, 2.3, 6, 7L);  // parameter pack has the last 3 elements
                                // int, double, long
      }
  ```

  ```cpp
      template<typenmae ...Args>
      class TypeTeller;

      template<typenmae ...Args>
      void foo(Args ...)
      {
          TypeTeller<Args...> x;    // pack expansion
      }

      template<typename ...Args>
      void func(Args ...args)
      {
          // parameter expansion: derleyici parameter paci virgul ile ayirilmis bir liste haline donusturur

          foo(args...);
      }

      func(1, 4.2m "mehmet");   // hata mesaju type lari gosterecek

      // derleyici sunu yazar (3 parametre icin):

      void func(T1 param1, T2 param2, T3 param3)
      {
          foo(param1, param2, param3);
      }

  ```

## dil tarafindan garanti altina alinmis 10 a yakin oruntu var


  ```cpp
        ???
      // derleyici sunu yazar (3 parametre icin):

      void func(T1 param1, T2 param2, T3 param3)
      {
          foo(&param1, &param2, &param3);
      }

        ???
      // derleyici sunu yazar (3 parametre icin):

      void func(T1 param1, T2 param2, T3 param3)
      {
          foo(f(param1), &param2, &param3);
      }
  ```
  > ancak pack expansion in kullanilmasi icin belirli kosulllarin saglanmasi gerekir

  ```cpp
      template<typename ...Args>
      void func(Args ...args)
      {

          std::tuple<Args>(args...);
      }

      // derleyici sunu yazar (3 parametre icin):
  ```

  ```cpp
      template<typename ...Args>
      void func(Args ...args)
      {
          foo(args...); 
      }
      
      func(1, 4.2m "mehmet")
  ```


  ```cpp
  ```


## cpp.dcom => pack expansion 
  - function argument lists
    ++args
    &args
    const_cast<const Args*>((&args)...)


### STL examples:
  - make_shared
  - emplace_functions
  - make_unique


  ex:
  ```cpp
    template<typename T, typename ...Args>
    std::unique_tr<T> MakeUnique(Args&& ...args)
    {
        return std::unique_ptr<T>{ new T(std::forward<Args>(args)...) };
    }
  ```


  ```cpp
    template<typenmae Types>
    void func(Types ...args)
    {
      std::tuple<Types...> x;

      std::cout << typeid(x).name() << "\n";
    }

    func(1, 2.3m 4L);   // tuple<intm double, long>;
  ```

  ```cpp
    template<typenmae Types>
    void func(Types ...args)
    {
      //std::tuple<Types...> x(args...);

      std::tuple<Types...> x(std::forward<Types>(args)...); // perfect forward

      std::cout << typeid(x).name() << "\n";
    }

    func(1, 2.3m 4L);   // tuple<intm double, long>;
  ```

# variadic template ile neler yapabiliriz? 3 tgenel teknik
  - compile-time recursivity 
    -> derleyiciye duruma gore birden fazla fonksiyon yazdirmak
  - C++17: fold expressions
  - comma seperated list
  - if constexpr

  > ayni seyi yapmanin birden fazla yolu var

## compile-time recursivity 
  - 1. argumani parametre paketinden cikarir ve isler
  - fonk. diger argumanlarla (ilk parmetre eksik) yeniden cagir
  - Ta ki en sonda derleyici variadic fonk. sablonundan yeni bir fonksiyonu cagirmak yerine
    (base case olarak) baska bir fonksiyonu cagiracak

  _ex1_
  ```cpp
      // gonderilen argumanlari standard out a yaz
      template<typename T>
      void Print(const T& t)
      {
          std::cout << "\nbase case\n";
          
          std::cout << t << "\n";
      }

      template<typename T, typename ...Args>
      void Print(const T&val, const Args& ...args);
      {
          cout << __FUNCSIG__ << "\n";  // Microsoft: signature print

          std::cout << val << " ";
          Print(args...);           // bir parametre azaltildi

          // base case ihtiyacimiz var: parametre paketi bosken print(args...) syntax error 
      }
  ```

  ```cpp
    ??? baska bir print fonk
  ```

  ```cpp

    #include <ostream>
    
    template<typename T>
    std::ostream& print(std::ostream& os, const T& t)
    {
    	return os << t;		// no separator after the last element in the pack
    }
    
    // this version of print will be called for all but the last element in the pack
    template <typename T, typename... Args>
    std::ostream& print(std::ostream& os, const T& t, const Args& ...rest)
    {
    	os << t << ", "; // print the first argument
    	return print(os, rest...); // recursive call; print the other 	arguments
    }
    
    #include <iostream>
    
    int main()
    {
    	print(std::cout, 45, "ali", 3.87, 'A');
    } 
  ```
  > acilan fonksiyonlar inline olarak expand edilir

### daha kolay daha az bilinen bir yontem

---
background
- 2 types of comma
  - comma seperated list virgul
  - operator comma

  void func(int x, int y);  // seperator

  func(1, 2, 3,); // seperator

  int a[] = {1, ,2 ,3 };  // seperator
  
  int a[] = {i (1, ,2 ,3) };  // operator


  func(x, y);   // seperator
  func((x, y)); // operator

- 2 important features:
  1. sequence point olusturur

      int x = 12, y = 34, z = 50;

      ++x, y += x, z -= y;  // comma operator

      > ilk ifadededen dogan side effectler gerceklestikten sonra sagdaki expression islenir
 
      > yani elimde n tane expression statement varsa
        expr1;
        expr2;
        expr3;

        expr1, expr2, expr3;  // boylelikle tek bir deyim hale getirlebilir
                              // anlamsal acidan hiv bir fark yok

     > c++20 de su kaldirildi
        a[2, 4];  //eskiden legaldi, c++20 de kaldirildi

     > comma operator diger operatorler gibid eger uretir.
       urettigi deger sag tarafindaki deger.

       ex:
        int x = 10, y = 45;
        auto a = (x, y);  // a = 45, comma y ni degerini uretir.

        a = (y, x); // comma x in degerini uretir

     >

        int x = 3, y = 5;
        (x, y) = 9;

        // c: syntax error, her zaman R-value uretir??? 
        // c++
---

  ```cpp
    void f1() { std::cout << f1() << "\n"; };
    void f2() { std::cout << f1() << "\n"; };
    void f3() { std::cout << f1() << "\n"; };

    int main()
    {
        f1(), 0   // turu: int, degeri 0
                  // once f1 in cagirilacagi garanti altinda
    }

    int main()
    {
        int a[] = {(f1(), 0), (f2(), 0), (f3(), 0)};  

        // a = {0, 0, 0}
        //  bu degerleri uretilirken sirasiyla f1, f2, f3 cagiriliacak
        //  siralama garanti altinda
    }
  ```

  func(x, ++x, ...);  // argumanlarin biri degisiyorsa siralama garanti altinda degil
                      // bir de ???
                      // order of evaluation gelecek ders

  ```cpp
    template<typename ...Types>
    void Print(const Types& ...args)
    {
        (std::cout  << args << " ", 0); // once sol ifade uygulanacak sonra 0

        int a[] = { ((std::cout << args << ""), 0)... };

        // ama hoca bunu tercih etmez 2 sebep
        // - dizi kullanilmayacak: warnings
    }


    Print(1, 2.3, 6L, "mehmet");
  ```

  > bundan daha iyisi initializer list kullnamak

  ```cpp
    template<typename ...Types>
    void Print(const Types& ...args)
    {
        (void)std::initializer_list<int>{ ((std::cout << args << ""), 0)... };
    }


    Print(1, 2.3, 6L, "mehmet");
  ```

  ```cpp
      template<typename T>
      T summer(T v)
      {
      	return v;
      }
      
      template<typename T, typename... Args>
      T summer(const T& first, const Args& ... args)
      {
      	return first + summer(args...);
      }
      
      #include <iostream>
      #include <string>
      
      int main()
      {
      	std::cout << summer(10, 20, 30, 80, 17) << '\n';
      	std::string s1 = "nec", s2 = "ati ", s3 = "Er";
      	std::cout << summer(s1, s2, s3, "gin") << '\n';
      }
  ```

### 3. tekink: if constexpr
  ```cpp
      #include <ostream>
      
      template <typename T, typename... Args>
      std::ostream& print(std::ostream& os, const T& t, const Args& ...rest)
      {
      	os << t;
      
      	if constexpr (sizeof...(rest) > 0)
      		os << ", ";
      	else
      		os << "\n";
      
      	if constexpr(sizeof...(rest) != 0)
      		print(os, rest...); 
      
      	return os;
      }
      
      #include <iostream>
      
      int main()
      {
      	print(std::cout, 45, "ali", 3.87, 'A');
      }
  ```

### fold expressions
  - unary left fold, unary right 

  ```cpp
  // unary left fold:

  template<typenmae ...Args>
  auto Sum (const Args& ...args)
  {
      return (... + args);   // unary left fold: (((pack0 + pack1) + pack2) + ...)

      return (((12 + 5) + 7)  + 3)
  }

  Sum(12, 5, 7, 3);
  ```

  ```cpp
  // unary right fold:

  template<typenmae ...Args>
  auto Sum (const Args& ...args)
  {
      return (args + ...);   // unary left fold: (((pack0 + pack1) + pack2) + ...)

      return p1 + (p2 + (p3 + p4));
  }
  ```

  ```cpp
  template<typenmae ...Args>
  auto Sum_r(const Args& ...args)
  {
      return (args + ...);
  }

  template<typenmae ...Args>
  auto Sum_l(const Args& ...args)
  {
      return (... + args);
  }

  int main()
  {
      string s1{"ali"}, s2{"veli"}, s3{"gul"};

      std::cout << sum_l(s1, s2, s3, "necati", "ergin");  // ok
      std::cout << sum_l(s1, s2, s3, "necati", "ergin");  // error: "necati" + "ergin"
  }
  ```

  ```cpp
  ```

  ```cpp
  ```









