   ========================= type erasure idiomu ========================
  # Type Erasure Idiom
  Type erasure idiom su ise yarar:  
  Farki turler var. Bu turlerin ortak ozelligine dayanarak ayni tur gibi kullanmaya imkan tanir.

  > Inheritance da ayni isi yapar, ancak
  >  1. taban sinifi olustur
  >  2. turemis sinifi taban siniftan olustur (taban sinifin virtual func. override etmek)
  >
  > => type erasure idiom, **kalitimin olusturudugu bagimliligi olusturmadan**, birbirinden farkli turleri (bu turlerin ortak ozelliklerini kullanarak) ayni tur gibi kullanmaya olanak saglar.

  > C++ type erasure idiomu ile bircok dilde bulunan type erasure kavramini birbirine karistirmayalim.

## Type Erasure in C (Do not mix with type erasure idiom in C++)
  - uses void*
  - this is not the same with type erasure idiom in C++

  _ex_: qsort: bir diziyi turden bagimsiz olarak siralar
  ```c
    // function declaration
    void qsort(void* vpa, size_t size, size_t sz, int (*f)(const void*, const void*));
        // vpa: siralanacak dizinin adresi
        // size: siralanacek dizinin boyutu
        // sz: siralanacak dizinin bir ogesinin sizeof'u
        // iki elemani karsilastiracak function pointer

    // usage
    int mycmp(const void* vp1, const void* vp2)
    {
        // Biz sunu biliyoruz:
        //  vp1 int* elemanlardan olusacak

        // way1: tasma hatasi olabilir
        // return *(const int*)vp1 - *(const void*)vp2;
        
        // way2: preferred
        if(*(const int*)vp1 > *(const void*)vp2)
        {
            return 1
        }
        if(*(const int*)vp1 < *(const void*)vp2)
            return -1;

        return 0;
    }

    int main()
    {
        constexpr int size = 10'000;
        int a[size];

        generate_n(std::begin(a), size, Irand{0, 100000});
        print(begin(a), end(a));

        qsort(a, size, sizeof(*a), )
        // tehlike: herhangi bir dizinin adresini gonderebiliriz. 
        //          yanlis bir dizi bile olsa compile time error gormeyiz
    }
  ```

## Inheritance is also a Type Erasure
  _ex_
  ```cpp
    class Base{};

    void func(Base&)
    {
        // gercek tur run-time da belli olur
        // we get a specific implementation via virtual dispatch
    }
  ```

## Type Erasure idiom in C++
Birbirinden bagimsiz siniflari kalitim kullanmadan, destekleri interface e guvenerek ayni turdenmis gibi turlerini dikkate almadan sunlari yapmak istiyorum:  
  - bir containerda tutmak
  - herhangi bir fonksiyona arguman olarak gecebilmek

> run-time polymorphismi icin kalitim'a gore daha saglikli bir alternatif yaratir.

  _ex_
  ```cpp
    // A, B, C bagimsiz siniflar, hic bir base class a bagli degil
    class A{};
    class B{};
    class C{};

    // Type Erasure idiom ile bu siniflari ayni turmus gibi kullanmak istiyorum
    // Sunlari inheritance kullanmadan legal yapalim
    //ex:
    func(A);
    func(B);
    func(C);

    // ex
    class Nec {A}; 
    class Nec {B}; 
    class Nec {C}; 

  ```

  kullanilan terimler: Concept, Model  
  - concept sinif model sinifa bir interface saglar
  - virtual dispatch i kullanacagiz ancak tamamen implementasyonda kalacak. Yani client siniflar arasinda bir inheritance iliskisi olmayacak. 

> C++20 Concept degil

  _ex_  
  **STEP-1: wrtite a concept and a model class**  
  **Concept class: Interface i gosterir**  
  **Model class: that overrides the interface using ErasedType**  
  -  1: Create a concept class that shows the common interface as an abstrat class
  -  2: Create a model class, which uses virtual dispatch mechanism to implement the concept class interface according to the template parameter Erased type

  ```cpp
    // 1. Concept class: Interface i gosterir
    // Concept olarak kullanilacak sinif model olarak kullanilack siniflara bir interface saglar
    class AnimalConcept {
    public:
        virtual ~AnimalConcept = default;
        virtual void cry() = 0;
    };

    // 2. Model class: overrides the interface using an ErasedType
    template <typename ErasedType>
    class AnimalModel : public AnimalConcept {
    public:
        AnimalModel(const ErasedType& object) : object_(object){}

        void cry() override
        {
            object_.cry();  
            // AnimalModel.cry, ErasedType.cry i cagirir.
        }

    private:
        ErasedType object_;
    };
  ```

**Step-2: create a common type for erased types**  
**- Common type encapsulates the concept/model implementation in its private part**  
**Therefore, Client codes have nothing to do with the concept/model implementation**
  - 3.1: create the base type for erased types (Animal)
  - 3.2: Move the concept/model classes to the private part of the common type 
  - 4: Create a concept type pointer that can refer to all Erased types as a private member of the common type
  - 5: Add non-virtual interface functions for the Concept class interface
  - 6: Create a template ctor to construct common type from any of the erased types

  ```cpp
    // 3.1: create the base type for erased types (Animal)
    class Animal {
    public:
        
        // 6. Create a template ctor to construct a common type from any of the erased types
        template<typename ErasedType>
        Animal(ErasedType&& x) : uptr(std::make_unique<AnimalModel>>(<std::forward<ErasedType>>(x))) {}
            // note that 
            //  6.1. a model type is created from erased type 
            //  6.2.  pointer is concept type but refers to the newly created model type

        // 5. Add non-virtual interface functions for the Concept class interface.
        void cry()
        {
            // Non-virtual interface functions of the common type use virtual dispatch mechanism (via model type pointer) in their implementation.
            uptr->cry();
        }
    

    private:
        // 3.2: Move the concept/model classes to the private part of the common type 
        //    Note that client codes have nothing to do with the concept/model implementation
        //3 ----------------------
        // 1. Concept Class
        class AnimalConcept {
        public:
            virtual ~AnimalConcept = default;
            virtual void cry() = 0;
        };

        // 2. Model class
        template <typename ErasedType>
        class AnimalModel : public AnimalConcept {
        public:
            AnimalModel(const ErasedType& object) : object_(object){}

            void cry() override
            {
                object_.cry();
            }

        private:
            ErasedType object_;
        };
        // 3 ----------------------

        // 4. Create a concept type pointer that can refer to all Erased types
        std::unique_ptr<AnimalConcept> uptr;
            // private veri elemani
            // Taban sinif pointer, turemis sinifi gosterebilir
            // Dolayisiyla, uptr AnimalModel turunden bir nesneyi de gosterebilir
    }

    class Cat {
    public:
        void cry()
        {
            std::cout << "miyav\n";
        }
    };

    class Dog {
    public:
        void cry()
        {
            std::cout << "hav\n";
        }
    };

    class Bird {
    public:
        void cry()
        {
            std::cout << "cik cik\n";
        }
    };

    int main()
    {
        Animal a{ Bird{} };
        Animal b{ Dog{} };
        Animal c{ Cat{} };

        a.cry();
        b.cry();
        c.cry();

        vector<Animal> vec;

        vec.emplace_back(Cat{})
    }

    // run-time polymorphismi icin kalitim'a gore daha saglikli bir alternatif yaratir.
    // - kalitimin getirdigi bagimlilik yok
    // - yeterki turlerin ortak bir subinterface i olsun
  ```

Unique ptr kullandigimiz icin copy ctor deleted edilmis durumda bunu duzeltelim:  
- Boylelikle, concept nesneleri birbrine kopyalanabilir olsun
- concept class is an abstract class. To support copy, we can write a clone interface

**Step-3**  
**Support copy operation between concept objects**  
  - 7.1: add a copy/clone interface for the concept class, which is an abstract class
  - 7.2: add implementation of the (polymorphic) copy/clone interface within the modell class
  - 7.3: write the copy ctor for the common type

  _ex_
  ```cpp
    class Animal {
    public:
        // 6
        template<typename ErasedType>
        Animal(ErasedType&& x) : uptr(std::make_unique<AnimalModel>>(<std::forward<ErasedType>>(x))) {}

        // 7.3: write the copy ctor for the common type
        Animal(const Animal &other) : uptr(other.uptr->copy()) {}

        // 5
        void cry()
        {
            // non-virtual interface function
            uptr->cry();
                // ????
        }
    
    private: // 3
        // 1
        class AnimalConcept {
        public:
            virtual ~AnimalConcept = default;
            
            // 7.1: add a copy/clone interface for the concept class, which is an abstract class
            virtual std::unique_ptr<AnimalConcept> copy() const = 0;
            
            virtual void cry() = 0;
        };

        // 2
        template <typename ErasedType>
        class AnimalModel : public AnimalConcept {
        public:
            AnimalModel(const ErasedType& object) : object_(object){}

            // 7.2: add implementation of the (polymorphic) copy/clone interface within the modell class
            std::unique_tr<AnimalConcept> copy() const override
            {
                return std::make_unique<AnimalModel>(*this);
            }

            void cry() override
            {
                object_.cry();
            }

        private:
            ErasedType object_;
        };

        // 4
        std::unique_ptr<AnimalConcept> uptr;
    }

    class Cat {
    public:
        void cry()
        {
            std::cout << "miyav\n";
        }
    };

    class Dog {
    public:
        void cry()
        {
            std::cout << "hav\n";
        }
    };

    class Bird {
    public:
        void cry()
        {
            std::cout << "cik cik\n";
        }
    };

    int main()
    {
        vector<Animal> avec;
        
        avec.emplace_back(Dog())
        avec.emplace_back(Cat())
        avec.emplace_back(Bird())

        for(auto&a : vec) {
            a.cry();
        }
    }
  ```

> Bastan beri standard aracin kalitim yerine type erasure olmasi dusunulmus.  
> tek sorun: implementatasyonu karisik  
> ancak bir suru type erasure kutuphanesi var

My summary:  
Ortak bir interface subsetine sahip birbirinden farkli turleri (ErasedTypes) ayni tur gibi kullanmak icin (ayni container da tutma), C deki void pointer yerine ortak bir tur olusturmak istiyoruz. Bu tur Erased type lar arasinda bagimlilik olusturmadan ortak interface i user a sunar.
- Bu amacla ortak interface sahibi bir abstract class (concept) olusturyuyoruz.
- Concepti inherite eden ve ErasedType olarak kullanilacak herhangi bir turu encapsulate eden bir model (template) class olusturuyoruz. Model class, concept class daki abstract interface i, encapsulate ettigi ErasedType i kullanarak implement eder.
- Son olarak bir common Type olusturuyoruz. CommonType, herangi bir erased type tan olusturulabilir. Copyable veya moveable olabilir.
Common Type ortak interface'i kullaniciya sunar.

Sonuc olarak, virtual dispatch mekanizmasi yeni olusturulan common type in private kisminda user dan saklanir.
ErasedType lar arasinda ortak bir interface olmasi yeterli, tamamen birbirinden farkli olabilirler.
