# std::enable_if ve SFINAE
> => SFINAE sadece function overloading icin gecerli  
> enable_if i kullanarak SFINAE idiomundan faydalanabiliriz 

std::enable_if
  - ici bos bir yapi
    ```cpp
    template< bool B, class T = void >
    struct enable_if;

    template<class T>
    struct enable_if<true, T> { typedef T type; };
    ```

  - ``enable_if<true>`` nun bir type i varken  
    ``enable_if<false>`` in bir type i yok

    ```cpp
    enable_if<true>::type;
    enable_if<false>;       // type i yok, kullanildiginda SFINAE out olur
    ```

  _ex:_
  ```cpp
    template<typename T>
    std::enable_if<true, T>::type void foo(T x);
        // T void foo(T x);
    
    template<typename T>
    std::enable_if<false, T>::type void foo(T x);   // bu durumda gecersiz tur olusturur
        // substitution failure, overload set den duser
  ```

  _ex:_
  ```cpp
    template<typename T>
    std::enable_if<std::is_floating_point_v<T>, T>::type foo(T x);

    // ya da
    template<typename T>
    std::enable_if_t<std::is_floating_point_v<T>, T> foo(T x)
    {
        return x;
    }

    int main()
    {
        foo(12.4);  // enable_if<true, T>::type => float

        foo(12);    // enable_if<false, T>::type => SFINAE out
                    // no matching overloading func
    }
  ```

  _ex:_
  ```cpp
    // SFINAE out 
    template<typename T>
    std::enable_if_t<std::is_floating_point_v<T>, T> foo(T x)
    {
        return x;
    }

    // only function in the overload set for foo(12) 
    foo(double);

    int main()
    {
        foo(12);  // not a syntax error
    }
  ```

  _ex:_
  ```cpp
    template<typename T>
    std::enable_if_t<std::is_floating_point_v<T>, T> foo(T x)
    {
        // 1
        return x;
    }

    template<typename T>
    std::enable_if_t<std::is_integral_v<T>, T> foo(T x)
    {
        // 2
        return x;
    }

    int main()
    {
        foo(12);    // 2 
        foo(4.5);   // 1 
    }
  ```
