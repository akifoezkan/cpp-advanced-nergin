## SFINAE - (Template Function Overloading):
> Substitution failure is not an Error  

  Substitution sirasinda hata olursa, derleyici syntax error vermez.  
  Sadece bu fonksiyonu overload setten cikarir

  _ex:_
  ```cpp
    template<typename T>
    typename T::nec func(T x);

    int main()
    {
        int x = 10;

        func(x);
    }

    // T = int, substitution sirasinda error olacak (int::nec yok)
    // Normalde bu syntax error olacakti, ancak hata vermek yerine 
    // derleyici bu fonksiyonu overload setten cikarir ve compilation a devam eder
  ```


  _ex:_
  ```cpp
   // 1
   template<typename T, unsigned N>
   std::size_t len(T(&)[N])
   {
       return N;
   }

   // 2
   template<typename T>
   typename T::size_type len(T const& t)   
   {
       // Note: template e bagli dependent type elde edildiginde, bunlarin type 
       // olarak kullanilmasi icin "typename" keyword unun kullanilmasi zorunlu
       return t.size();
   }

   int main()
   {
       int a[10]{};
       std::cout << len(a);    // T = int, N = 10
                               // derleyici 
                               // - 1. fonk. sunu yazar: 
                               //    size_t len(int(&)[10]) {...}
                               // - 2. fonksiyon icin sunu yazar 
                               //    typename int::size_typename ....
      // Substituion sirasinda 2. fonksiyon icin syntax error verilmesi yerine 
      // SFINAE out edildi (no compiler error)

      std::cout << len("tmp");  // 1) T = const char, N = 4
                                // 2) SFINAE out, no compiler error

      int* p{};
      std::cout << len(p); // syntax error:
                           // no matching overloaded function found

      std::allocator<int> x;
      std;:cout << len(x);     // syntax error
                               // 1) viable degil
                               // 2) T = allocator<int>, viable (argument deduction is ok)
      
      // Yani derleyici 2. fonksiyonu overload function olarak secti
      //    function overload resolution bitti, substitution oncesi hata yok
      //    instantiation error: 
      //        allocator<int>.size() is not a member 
   }
  ```

> c++20 ile conceptler SFINAE nin kullanim alani azaldi.
