## Rule: variadic parametre herkese kaybeder
  _ex:_
  ```cpp
    // cpp_kodlar/ basic02.cpp

    template<typename T, unsigned N>
    std::size_t len(T(&)[N])
    {
    	return N;
    }
    
    template<typename T>
    typename T::size_type len(T const& t)
    {
    	return t.size();
    }
    
    // variadic parametre herkese kaybeder
    std::size_t len(...)
    {
    	return 0;
    }
    
    
    int main()
    {
      int a[10]{};    // selected: 1
                      // 1 viable, 2 not viable, 3 viable

      std::cout << len("tmp");  // 1
                                // 1 viable, 2 not viable (SFINAE out), 3 viable

      std::vector<int> v(5);
      std::cout << len(p);      // 2, (2, 3 viable)

    	int* p{};
      std::cout << len(p);      // 3, (3 viable, 2 SFINAE out)

    	std::allocator<int> x;
      std::cout << len(x);    // syntax error
                              // (2, 3 viable), overload resolution i 2 kazanir
                              // allocator<int>.size() yok, instantiation sirasinda hata verilir
    }
  ```

## Partial Ordering: How compiler selects the most specialized?
> source: https://docs.microsoft.com/en-us/cpp/cpp/partial-ordering-of-function-templates-cpp?view=msvc-160

Use the following process to determine if one function template candidate is more specialized:

1. Consider two function templates, T1 and T2.
2. Replace the parameters in T1 with a hypothetical unique type X.
3. With the parameter list in T1, see if T2 is a valid template for that parameter list. Ignore any implicit conversions.
4. Repeat the same process with T1 and T2 reversed.

5. If one template is a valid template argument list for the other template, but the converse isn't true, then that template is considered to be less specialized than the other template. If by using the previous step, both templates form valid arguments for each other, then they're considered to be equally specialized, and an ambiguous call results when you attempt to use them.

6. Using these rules:  
    * A template specialization for a specific type is more specialized than one taking a generic type argument.  

    * A template taking only ``T*`` is more specialized than one taking only ``T``, because a hypothetical type ``X*`` is a valid argument for a T template argument, but X is not a valid argument for a ``T*`` template argument.

    * ``const T`` is more specialized than ``T``, because ``const X`` is a valid argument for a ``T`` template argument, but ``X`` is not a valid argument for a ``const T`` template argument.

    * ``const T*`` is more specialized than ``T*``, because ``const X*`` is a valid argument for a ``T*`` template argument, but ``X*`` is not a valid argument for a ``const T*`` template argument.

    _ex:_
    ```cpp
        // bunlardan hangisi daha specific?

        template<typename T>
        void func(T x);       // 1, fictional type: A

        template<typename T>
        void func(T*);        // 2, fictional type: B

        // A turu fonksiyon-2 e gonderilemez
        // Ancak B* fonksiyon-1 e gonderilebilir (T = B*)
        // O zaman fonksiyon-2 daha specific
    ```
> Yukarida bahsedilen partial ordering rules ile daha specific fonksiyonu ararken sunlari ignore et:
>  1. default arguman varsa ama kullanilmamissa o parametreyi ignore et
>  2. variadic parametre argumani ignore et 

  _ex:_
  ```cpp
  templates/overloading/partial overlading.cpp

  template<typename T>
  void t(T*, T const* = nullptr, ...);  // 1
     // variadic parametreyi gormezden gel (daha specifik olana bakarken)
     // varsayilan arguman kullanilmis, ignore etme
     // yani su fonksiyonu incele:
     //    template<typename T>
     //    void t(T*, T const*);
     //        fictional type: A => (A*, A const*) ile fonk-2 cagirilamaz
  
  template<typename T>
  void t(T const*, T*, T* = nullptr);  // 2
     // kullanilmayan default argumani ignore et
     // yani su fonksiyonu incele:
     //    template<typename T>
     //    void t(T const*, T*);
     //        fictional type: B => (B const*, B*) ile fonk-1 cagirilamaz

  void example(int* p)
  {
     t(p, p);   // syntax error:
                 // daha sepecific fonk bulunamadi 
     
     // error message:
     // more than one instance of overload function "t" matches the argument list
  }
  ```

  _ex:_
  ```cpp
  // function overloading reminder

  void func(long double);   // 1
  void func(char);          // 2

  int main() {
      func(2.3f);   // synax error: ambiguity
                    // 1: standard conversion
                    // 2: standard conversion

  }
  ```

  _ex:_
  ```cpp
  // function overloading reminder
  
  void func(unsigned int);
  void func(long double);

  int main() {
      func(12);   // synax error: ambiguity
                  // both are standard conversion 
  }
  ```

  _ex:_
  ```cpp
  // function overloading reminder
  
  void func(long double); // 1
  void func(double);      // 2

  int main() {
      func(12.f); // 2 cagirilir (promotion)  
                  // 1: standard conversion 
  }
  ```
        

## reminder: template function in bulundugu yerde exact match (gercek fonksiyon) varsa gercek fonksiyon cagirilir

  _ex:_
  ```cpp
  int Max(int a, int b)   // 1
  {
      return b < a ? a: b;
  }

  template<typename T>
  T Max(T a, T b)         // 2
  {
      return b < a ? a: b;
  }

  
  auto n = Max(12, 45);   // 1 exact match (1, 2 viable)
  
  auto n = Max(12, 4.5);   // 1 (1 viable)
  
  auto n = Max('A', 'B');   // 2
  
  auto n = Max<>(12, 45);   // 2
  
  auto n = Max<int>(12, 4.5);   // 2
  
  auto n = Max<int>(12, 4.5);   // 2
  ```


  _ex:_
  ```cpp
  template<typename T1, typename T2>
  auto max(T1 a, T2 b)
  {
      std::cout << "1\n";
      return b < a ? a : b;
  }

  template<typename RT, typename T1, typename T2>
  RT max(T1 a, T2 b)
  {
      std::cout << "2\n";
      return b < a ? a : b;
  }

  int main() {}
      auto a = ::max(4, 7.2);     // 1, (2 viable degil, geri donus cikarimi yapilamaz)

      auto b = ::max<long double>(7.2, 4);    // 2 is a better match since 7.2 is double
                                              // 1 (viable): auto max(long double, int)
                                              // 2 (viable): long double max(double, int) 

      auto c = ::max<int>(4, 7.2);    // ambiguity, 1 ve 2 arasindan secim yapilamiyor
                                      // 1 (viable): auto max(int, double)
                                      // 2 (viable): int max(int, double) 
  }
  ```
### exact match in partial ordering kurallarina gore template functionlara olan ustunlugunden su sekilde faydalanilabilir:

  _ex:_
  ```cpp
  // bir fonksiyonun sadece berlirli bir turle cagirilmasi
  // diger turlerle yapilan cagrilarda sentax hatasi verilmes 

  template<typename T>       
  void foo(T) = delete;

  void foo(int);

  int main()
  {
      foo(12);
      foo(.12);   // error, deleted
      foo(1.2f);  // error, deleted
  }

  // bu enable_if ile de yapilabilir
  ```
