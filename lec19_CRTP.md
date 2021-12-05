# CRTP (Curiously Recurring Template Pattern)
  Oyle bir kalitim yapacagiz ki Base class in template parametresi Der class olacak

  _ex_:
  ```cpp
    template<typename Derived>
    class Base {
    public:
        void interface()
        {
            //...
            static_cast<Derived*>(this)   // pointer to Der

            static_cast<Derived&>(*this)  // ref to Der

            // Taban sinif turemis sinifin fonksiyonu cagirabilir
            static_cast<Derived*>(this)->implementation();

            // ->implementation() dependent type oldugu icin derleyici 
            // ilk fazda bu ismi aramayacak
        }
    };

    class Der : public Base<Der> {
    public:
        void implementation()
    };

    class Nec : public Base<Nec> {

    };
  ```

Bu sayede 
- Base class Derived in uye fonksiyonlarini cagirabilir

  _ex_:
  ```cpp
    // Public kalitimda Derived in clint lari Base in interface fonksiyonlarini cagirabilir

    template<typename Derived>
    class Base {
    public:
        void interface()
        {
            // Taban sinif turemis sinifin fonksiyonu cagirir
            static_cast<Derived*>(this)->implementation();
        }
    };

    class Der : public Base<Der> {
    public:
        void implementation()
        {
            std::cout << "Der::implementation()\n";
        }
    };


    int main(){
        Der myder;

        myder.interface();    // Der, Base in interface ine erisebilir
    }
  ```

## CRTP: interface design

Base in interface inde, Der in interface inde olan bir fonksiyon tanimlanmis olabilir.  
  - Bu bir syntax error degildir
  - Der'in interface fonksiyonu Base in interface fonksiyonunu shadow eder.

  _ex_:
  ```cpp
    template<typename Derived>
    class Base {
    public:
        void interface()
        {
            // Taban sinif turemis sinifin fonksiyonu cagirir
            static_cast<Derived*>(this)->implementation();
        }

        // Base::implementation, shadowed by Der::implementation
        void implementation()
        {
            std::cout << "Base::implementatioon\n";
        }
    };

    class Der : public Base<Der> {
    public:
        // Der::implementation
        void implementation()
        {
            std::cout << "Der::implementation()\n";
        }
    };

    int main(){
        Der myder;

        myder.interface();    // Der::implementation cagirilir
    }
  ```

Public inheritance da Der, Base in interface ini kullanir

  _ex_:
  ```cpp
    template<typename Derived>
    class Base {
    public:
        void interface()
        {
            static_cast<Derived*>(this)->implementation();
        }

        void implementation()
        {
            std::cout << "Base::implementatioon\n";
        }
    };

    class Der : public Base<Der> {
    public:
    };


    int main(){
        Der myder;

        myder.interface();    // Base::implementation cagirilir
    }
  ```

  _ex_:
  ```cpp
    template<typename Derived>
    class Base {
    public:
        void interface()
        {
            static_cast<Derived*>(this)->implementation();
        }

        static void static_interface()
        {
            Derived::static_implementation();
        }

        void implementation()
        {
            std::cout << "Base::implementatioon\n";
        }

        static void static_implementation()
        {
            std::cout << "Base::static_implementation()\n";
        }
    };

    class Der : public Base<Der> {
    public:
        void implementation()
        {
            std::cout << "Base::implementatioon\n";
        }

        static void static_implementation()
        {
            std::cout << "Der::static_implementation()\n";
        }
    };


    int main(){
        Der myder;

        myder.interface();    // Base::implementation cagirilir

        myder.static_implementation();  // Der:static_implementation
        
        // Der:static_implementation tanimlanmasaydi, Base::static_impl cagirilirdi
    }
  ```

> Common pitfall  
> CRTP pattern: ```class Der : public Base<Der>```  
> Hata: ```class Der : public Base<Der2>```  (Base in template parametresi Der degil)  
> sonuc: (run-time a yonelik) beklenmeyen sonuclar olusur.  
>
> Bunu (undefined behaviouri) engellemek ve syntax error garantisi vermek icin soyle bir yapi kullanabiliriz:
  
  ```cpp
    template<typename Derived>
    class Base {
    private:
        Base() {};
        friend Derived;

        // Base class i sadece Derived class construct edebilir.
        // Bu sayede Nec: public Base<Der>, syntax error verir
    };

    class Der : public Base<Der>{
    public:

    };

    class Nec : public Base<Der>{
    public:
      // Runtime a yonelik bir hata olusur

      // Nec in Base ini Der acilimindan olusturmaya calisiyor
    };
  ```

# CRTP kullanim amaclari
CRTP farkli amaclar icin kullanilabilir:
  - [esas amaci]: Taban sinifin turemis sinifi kullanarak, truemis sinifa bir takim islevler eklemesi
     * boilerplate kod u azaltir
     * ...
  - runtime polymorphisme (static) alternative olusturur  

Ornek olarak asagidaki seneryolar incelenecek:

## 1. Object counting
> CRTP ile boiler kod yazimi azaltilabilir  

  Normalde asagidaki bir kodun object counting ozelligini isteyen butun class lar icin yazilmasi gerekir:  
  - static counter that increases with copy ctor and decrements with dtor

  ```cpp
      template<typename T>
      class Counter {
      public:
          Counter()
          {
             ++ms_count_created;
             ++ms_count_alive;
          }

          Counter(const Counter&)
          {
             ++ms_count_created;
             ++ms_count_alive;
          }

          ~Counter()
          {
             --ms_count_alive;
          }

          // Der siniflari icin interface

          static std::size_t how_many_created() { return ms_count_created; }
          
          static std::size_t how_many_alive() { return ms_count_alive; }


      private:
          static std::size_t ms_count_created;
          static std::size_t ms_count_alive;

          // Reminder: her farkli T turu icin ayri bir ms_count* olusturulacak
      };

      template<typename T>
      std::size_t Counter<T>::ms_count_alive{};

      template<typename T>
      std::size_t Counter<T>::ms_count_created{};


      // private inheritance to hide the interface of Counter
      // Dolayisiyla Base sinifin public interface i Der in public interface ine eklenmez
      class Neco : Counter<Neco> {
          // interface ...

          // using ile istenilen fonksiyonlari Der in interface ine ekleriz
          using Counter<Neco>::how_many_created;
          using Counter<Neco>::how_many_alive;

          // CRTP sayesinde kod tekrarini azalttik.
          // Object Counting islevini Neco'ya ekledi
      }


      int main()
      {
          Neco n1, n2, n3;
          {
              Neco n4, n5, n6;
          }

          Neco::how_many_created();
          Neco::how_many_alive();
      }
  ```

## 2. (Mixin-Class) Turemis siniflara belirli bir ozellik kazandirmak
  - Base sinif interface i belirleyecek
  - Derived sinif interfacein implementation inini belirleyecek

  _ex_:
  ```cpp
      template<typename Der>
      class Writer {
      public:
          void write(const char *p) const
          {
              static_cast<const Der*>(this)->write_impl(str);   // calls Der's write impl
          }
      };


      class FileWriter : public Writer<FileWriter> {
      public:
          FileWriter(const char *pfilename) : mp_file {std::fopen(pfilename, "w")};
          {
              using namespace std::literals
              if(!mp_file)
                  throw std::runtime_error{"file "s + pfilename + "cannot be opened!"};
          }

          friend class Writer<FileWriter>;
              // Neden friend?: 
              // private inheritence, write_impl private (global interface de istemiyoruz)

      private:
          void write_impl(const char* str) const
          {
              std::fprintf(mp_file, "%s\n", str);
          }
      };


      class ConsoleWriter : public Writer<ConsoleWriter> {
      public:
          friend class Writer<ConsoleWriter>;

      private:
          void write_impl(const char* str) const
          {
              std::printf("%s\n", str);
          }
      }

      int main()
      {
          FileWriter fw{ "aaaa.txt" };
          ConsoleWriter cw;

          fw.write("Dolar kuru 10.67 oldu");
      }
  ```

  _ex_:
  ```cpp
      // CRTP ile equality compare ozelligini Der siniflarina kazandiralim
      template<typename Der>
      struct Equality {
          const Der& derived() const
          {
              return static_cast<const Der&>(*this);  // her seferinde yazmayalim
          }
      };

      template<typename Der>
      bool operator==(const Equality<Der>& left, const Equality<Der>& right)
      {
          // Der in operator< fonksiyonu oldugunu varsayiyoruz
          return !(left.derived() < right.derived()) && !((right.derived() < left.derived));
      }

      template<typename Der>
      bool operator==(const Equality<Der>& left, const Equality<Der>& right)
      {
          return !(left == right);
      }

      class Nec : public Equality<Nec> {
      public:
          Nec(int val = 0) : m_val{val} {}

          bool operator<(const Nec& other) const
          {
              return m.val < other.m_val;
          }
      private:
          int m_val;
      };

      class Person : public Equality<Person> {
      public:
          Person(std::string name) : m_name{std::move(name)}

          bool operator<(const Nec& other) const
          {
              return m_name < other.m_name;
          }

      private:
          std::string m_name;
      };


      int main()
      {
          Nec x{ 198 }, y{ 546 };

          std::cout << std::boolalpha;
          std::cout << "x == y" << (x == y) << "\n";
          std::cout << "x != y" << (x != y) << "\n";

          Person p1{"Kadir"}, pw{"Akif"};
          std::cout << "p1 == p2" << (p1 == p2) << "\n";
          std::cout << "p1 != p2" << (p1 != p2) << "\n";
      }
  ```

  _ex_:
  ```cpp
      template<typename Der>
      struct Comparison {
          const Der& derived() const
          {
              return static_cast<const Der&>(*this);  // her seferinde yazmayalim
          }

          friend bool operator==(const Comparison<Der>& left, const Comparison<Der>& right)
          {

              return !(left.derived() < right.derived()) && !((right.derived() < left.derived));
          }

          friend bool operator!=(const Comparison<Der>& left, const Comparison<Der>& right)
          {
              return !(left == right);
          }

          friend bool operator>(const Comparison<Der>& left, const Comparison<Der>& right)
          {
              return (right.derived() < left.derived());
          }

          friend bool operator>=(const Comparison<Der>& left, const Comparison<Der>& right)
          {
              return !(left.derived < right.derived());
          }

          friend bool operator<=(const Comparison<Der>& left, const Comparison<Der>& right)
          {
              return !(right.derived() < left.derived());
          }
      };
            
    
    class Nec : public Comparison<Nec> {
      public:
          Nec(int val = 0) : m_val{val} {}

          bool operator<(const Nec& other) const
          {
              return m.val < other.m_val;
          }
      private:
          int m_val;
      };

      class Person : public Comparison<Person> {
      public:
          Person(std::string name) : m_name{std::move(name)}

          bool operator<(const Nec& other) const
          {
              return m_name < other.m_name;
          }

      private:
          std::string m_name;
      };


      int main()
      {
          Nec x{ 198 }, y{ 546 };

          std::cout << std::boolalpha;
          std::cout << "x == y" << (x == y) << "\n";
          std::cout << "x != y" << (x != y) << "\n";

          Person p1{"Kadir"}, pw{"Akif"};
          std::cout << "p1 == p2" << (p1 == p2) << "\n";
          std::cout << "p1 != p2" << (p1 != p2) << "\n";

          ...
      }
  ```


  _ex_:
  ```cpp
      template<typename Derived>
      struct PrintNTimes {
      public:
          void nprint(std::size_t n) const
          {
              while(n--)
              {
                  static_cast<const Derived&>(*this).print();
              }
          }
      };

      class Person : public PrintNTimes<Person>{
      public:
          Person(std::string name, std::string surname) : 
          m_name{std::move(name)}, m_surname{std::move(surname)} {}

          void print() const 
          {
              std::cout << m_name " " << m_surname;
          }

      private:
          std::string m_name;
          std::string m_surname;
      };

      int main()
      {
          Person p{"Murat", "Hepiyiler"};
          ...
      }
  ```

## 3) CRTP ile interface in bir kismini Base e tasimak
  _ex_:
  ```cpp
      template<typename Der>
      class Container {
      public:
          decltype(auto) front()
          {
              return *derived().begin();

              // decltype(auto) since we want to return a reference (when the expresion is L-val)
          }

          decltype(auto) back()
          {
              return std::prev(derived().end());
          }

          decltype(auto) size() const
          {
              return static_cast<std::size_t>(std::distance(derived().begin(), derived().end()));
          }

          decltype(auto) operator[](std::size_t idx)
          {
              return *(std::next(derived().begin(), idx));
          }

      private:
          // Const overloading
          Der& derived() { return static_cast<Der&>(*this); }
          const Der& derived() const { return static_cast<Der&>(*this); }
      };

      template<typename T>
      class Darray : public Container<Darray<T>> {
      public:
          // note that Darray is also a template class

          Darray(std::size_t n) : m_size{n}, m_uptr {std::make_unique<T[]>(n)} {}

          T* begin() { return m_uptr.get(); };
              // T* begin() & {} -> sadece sol taraf degeri nesneleri icin cagirilmasi icin

          const T* begin() { return m_uptr. get(); };

          const T* end() { return m_uptr.get() + m_size; };
              
      private:
          std::size_t m_size;
          std::unique_ptr<T[]> m_uptr;
      };


      int main()
      {
          Darray<int> a {10};

          for(size_t i{}; i < a.size(); ++i)
              a[i] = i;

          std::cout << a.front() << "\n";
          std::cout << a.back() << "\n";
      }
  ```

## 4) polymorphic chain 

Function chaining ozelligini kullanan bir ornek yazalim:

  ```cpp
      class Printer {
      public:
          Printer(std::ostream& os) : m_os{os};

          template<typename T>
          Printer& print(cons T& tval)
          {
              m_os << tval;
              return *this;
          }

          template<typename T>
          Printer& print_line(cons T& tval)
          {
              m_os << tval << "\n";
              return *this;
          }

      private:
          std::ostream m_os;
      };

      int main()
      {
          Printer(std::cout).print("Kaveh Nematipour").print_line(6712.98);
      }
  ```

Kalitim olmasi durumunda yukaridaki chaining calismayabilir.  
Asagidaki kod bu durumu gosterir:

  ```cpp
      class Printer {
      public:
          Printer(std::ostream& os) : m_os{os};

          template<typename T>
          Printer& print(cons T& tval)
          {
              m_os << tval;
              return *this;
          }

          template<typename T>
          Printer& print_line(cons T& tval)
          {
              m_os << tval << "\n";
              return *this;
          }

      private:
          std::ostream m_os;
      };

      #include<iostream>
  
      class ConsolePrinter : public Printer {
      public:
          ConsolePrinter() : Printer{std:cout} {}

          ConsolePrinter& set_color()
          {
              // ... 
              return *this;
          }
      };

      int main()
      {
          ConsolePrinter().print("Kaveh").print_line(67123.12); //ok
                        // ConsolePrinter().print("Kaveh") returns Printer type            
          
          ConsolePrinter().print("Kaveh").set_color(661).print_line(823.48324); //error
                        // ConsolePrinter().print("Kaveh") returns Printer type
                        // Printer does not have a .set_color(661) member function

          // error: set_color is not a member of Printer
      }
  ```

  > yukaridaki problemi CRTP ile cozelim

  ```cpp
      template<typename ConcretePrinter>
      class Printer {
      public:
          Printer(std::ostream& os) : m_os{os};

          template<typename T>
          ConcretePrinter& print(cons T& tval)
          {
              m_os << tval;
              return static_cast<ConcretePrinter&>(*this);
          }

          template<typename T>
          ConcretePrinter& print_line(cons T& tval)
          {
              m_os << tval << "\n";
              return static_cast<ConcretePrinter&>(*this);
          }

      private:
          std::ostream m_os;
      };

      #include<iostream>
  
      class ConsolePrinter : public Printer<ConsolePrinter> {
      public:
          ConsolePrinter() : Printer{std:cout} {}
            // Note: I don't have to write Printer<ConsolePrinter>

          ConsolePrinter& set_color()
          {
              // ... 
              return *this;
          }
      };

      int main()
      {
          ConsolePrinter().print("Kaveh").print_line(67123.12); //ok
          
          ConsolePrinter().print("Kaveh").set_color(661).print_line(823.48324); //ok
      }
  ```

> Yukaridaki problemi covaiant type kullanarak cozemeyiz

---
## reminder: covariant return type (polymorphic classes)
Asagidaki gibi pointer veya referans donduren herhangi bir virtual function icin tanimlanan override function interface return type in bir child turunu dondurebilir.  
Bu durumda child type parent type in covariant i olur.

  ```cpp
      class A{};
      class B{};
      
      class C{} : public A{}; // C is child of A

      class Base{
      public:
          virtual A func();

          virtual *A foo();   // pointer or reference semantic
      }

      class Der : public Base {
      public:
          A func() override;  //ok
          
          B func();  // redeclaration, signature is the same with
                     // error message: .... is not covariant from Base::func

          // C is a (co)variant type of A and interface is a pointer or ref
          C& func() ;  // ok
          C& func() override;  // ok

      };
  ```
---

# 5) static polymorphism
  Virtual dispatch benzeri bir yapiyi compile time da elde etmek

  _ex_:
  ```cpp
      class Animal {
      public:
          virtual void cry() = 0;   // turemis sinif, taban sinifin 
                                    // sapsanal fonksiyonunu override etmek zorunda
      };

      class Cat : public Animal {
      public:
          void cry() override;
      };
  ```