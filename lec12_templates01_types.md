# Templates
  - derleyiciye kod yazdirir  
  - template => metacode

## cesitleri
  - function template
  - class template
  - alias template        (modern c++)  
    => using anahtar sozucugu ile template support
       (typedef template support etmiyordu)
  - variable template     (modern c++)  
     => template keyword lere bagli kod yazdirma


 >   => standardlara gore "function template" ve "template function"
       diye bir terim farkliligi yok.

 >   => instantiation: user-code un compiler tarafindan yazilmasi   


## examples from STL:
* function template:
    - algorithm headerindaki fonksiyonlar
    - numeric headerindaki algoritma fonksiyonlar

* class template:
    - containerlar
        * sequence (vector, list, forward_list, string, array)
        * assocoative (set, map, multiset, multimap)
        * unordered associative (unordered set/map/multiset/multimap)

    - stack, queue, priority queue  
        => bir container alip bunlarin interface ini adapte eder

    - smart pointers  
        => unique ptr, shared ptr,

    - RAII amacli guard siniflari

    - hash sinif sablonu

    - metafonksiyon olarak kullanilan siniflar
        * type_traits dosyasindaki class lar
            => compile time programlamasi icin veri uretirler

* alias template:
    - sonu "_t" ile biten fonksiyonlar

    _ex:_
    ```cpp
      remove_reference_t<int &>
    ```

    >=> template icinde kullanidiginda typename anahtar sozcugunun
       kullanilma zorunlulugunu ortadan kaldirir

    _ex:_  
    ```cpp
    template<typename T>
    using RemoveReference_t = typename std::remove_reference<T>::type
    ```


* variable template:
    - sonu "_v" ile biten fonksiyonlar

      _ex:_
      ```cpp
        is_rvalue_reference_v<int&&>
      ```

      _implementation:_
      ```cpp
        template<typename T>
        inline constexpr bool is_RvalueReferenc_v = std::is_rvalue_reference<T>::value;
      ```

===========================================================================
## reminder: inline variable
```cpp
int x = 10; // global variable, violates one definition rule if in the header
```

similar for functions

```cpp
// ali.h
inline void func(int x) { ... }
```

linker inline func dan sadece 1 tane gorur, ODR ihlal edilmez

=> token-by-token ayni kod inline edildigi takdirde, birden fazla kez include edilse bile ODR (one definition rule) i ihlal etmez

## c++17 inline for variables
  ayni sekilde optimizasyona yonelik degil

  _ex:_  
  ```cpp
  inline int x = 10;
  ```

  => token by token ayni olmasi durumunda birden fazla dosyada include edilse bile 1 tane degisken olusturulur. ODR ihlal edilmez

===========================================================================
