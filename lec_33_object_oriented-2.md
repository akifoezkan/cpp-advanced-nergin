# Solid principles (Robert C. Martin)
  [S]ingle Responsibility Principle (SRP)  
  [O]pen Closed Principle (OCP)  
  [L]iskov Substitution Principle (LSP)  
  [I]nterface Segregation Principle (ISP)  
  [D]ependancy Inversion Principle (DIP)  

  segregation -> ayristirma

> interface definition:  
> OOP da "interface" abstract class daki sapsanal fonksiyonlar kastedilir.


> uncle bob:
> bir sinif degisecekse tek sebepten dolayi degismeli

## [S]ingle Responsibility Principle (SRP)

Bir modul sadece ve sadece tek bir aktore karsi sorumluluk tasimalidir.
Bu aktorun talebi degisince sinif degismeli.
Ayni ilkeyi farkli sekilde ifade edebiliriz.
<Bir modul sadece ve sadece bir kullaniciya yada paydasa karsi sorumluluk tasimalidir.>

> according to Uncle Bob, this is the most commonly misunderstood principle
>
> "
> Tum solid ilkeleri icerisinde en fazla yanlis anlasilan SRP.
> Bunun nedeni buyuk ihtimalle isminin cok uygun olmamasi.
> 
> Ilkenin ismini duyan programcilar, her modulun tek bir sey yapmasi gerektigi anlamini cikartabiliyorlar.
> 
> Evet boyle bir ilke var: Bir fonksiyon tek ve yanliz tek bir sey yapmali.
> Bu ilkeyi (en alt seviyede - kod seviyesinde) buyuk fonksiyonlari refactoring yaparak daha kucuk fonksiyonlara bolerken kullaniyoruz.
> Faka bu SOLID ilkelerinden biri olan SRP degil.
> "

## [O]pen Closed Principle (OCP)
- modules should be open for extension but closed for modification  
  bir degisiklik icin: var olan kodu degistirmek yerine yeni kod yazmak
 
- siniflar extension a acik, modification a kapali olmali

  _ex_:
  qsort algorithm in C
  karsilastirmanin nasil yapilacagini, input olan function pointer dan alir
  her kasilastirma islemi icin yeni bir sort algoritmasi yazilmaz, yani qsort algoritmasi degistirilmez  


- polymorphic definition by Uncle Bob:  
  Use interfaces instead of super classes to allow different implementations  
  super class -> abstract olmayan class lar  

- var olan kodlar degismedigi icin sistem daha az kirilgan, test etmesi daha kolay

Bu prensibi nerelerde uygulayacagiz?
- degisikligin nerelerden gelebileceginin tasarim sirasinda ongorebilmemiz gerekir
- degisebilecek varyasyonlara, duragan degismeyecek interface saglamamiz gerekir

Bu prensibin uygulanmadigini nasil anlariz?
- C++ runtime type identification yapiliyorsa bu bir isaret olabilir.
  Program calisirken tur sorgulamasi yapiliyor ve bu ture gore bir dallanma yapiliyorsa buyuk ihtimal bu prensib cignenmistir.

  _ex:_ polymorphism yerine if/else, switch kullanilmasi

  _ex_: virtual dispatch yerine, manual dispatch

  _ex:_
  ```cpp
  struct Rectangle { int type;};
  struct Circle {};
  struct Square {};

  // manual dispatch
  // bu kod OCP yi ihlal eder 
  ```

  _ex:_
  ```cpp
  class IShape {
  public:
    virtual double get_area() const = 0;
  }

  add func(std::vector<Shape*> vec)
  {
    for(auto p : vec)
    {
        p->get_area();
    }

    // if/else gibi turu sorgulayan kod
    // OCP ye uygar
    // yeni talepler ekleme yoluyla uygulanabilir
  }
  ```

## [L]iskov Substitution Principle (LSP)  
- Polymorphismin guvenli kullanimina iliskin bir prensip

- Subclasses should be substitutable for their base classes  
  (Taban sinifin kullanildigi yerde turemis sinif kullanildiginda bir problem olmamasi lazim)

  _ex:_ 
  Animal hierarchy sinde Animal base class yerine Cat, Dog hatasiz kulllanilabilmeli

> LSP uyumlulugu:  
> Run-time polymorphism inin inheritance degil composition ile yapilmasini tercih et:  
> Bu ilke taban sinif olarak kullanilacak sinifi veri elemani olarak koy demek degildir!  
> Yani dogrudan containment yolu ile compositiondan bahsedilmiyor.  
> Bunun C++ daki karsiligi sudur:  
> sinifa veri elemani olarak abstract class turunden referans koyuyoruz.
> ornegin shape*, unique_ptr<Shape>


### sozlesmeye bagli tasarim
  interface uzerinden

  _ex:_
  ```cpp
    class Shape {};

    // LSP
    void func(Shape*) {...}
  ```

#### base sinif uzerinden interface sunmak icin, bazi garantiler verilmeli:  
- Bir siniftan turetme yapildiginda, subclass (derived), super class (base) in 
 * behavior ini ve 
 * state conditions ini bozmamali.  
Eger bu kosullar bozulursa, taban sinif interface ine bagli olarak turemis sinif kullanilan yerlerde, buna guvenen kodlar kotu sonuclarla karsilasilabilir. 

- improper inheritance: inheritance in yanlis bir sekilde yapilmasi (C++ daki terim)   
  **A subclass should require nothing more and promise nothing less!**   
  In other words, derived class should respect pre-conditions, post-conditions, and invariants of the base class.  
  Of course, the behaviour (also objective) should be the same (if the goal is to calculate salary, member function in the derived class should also calculate salary).  

  * __pre-conditions:__ requirements, islem yapilmadan once saglanmasi gereken conditions   
  Der precondition'i gevsetebilir ama daha siki hale getiremez

  * __post-conditions:__ fonksiyon isini yaptiktan sonra, nesne hangi kosullari saglamali? 

  * __class invariant:__ degismezler (onceden belirlenmis kurallar)  
  _ex:_ last element of a linked list is NULL char



  > _duck test_: It looks like a duck, Quacks like a duck, But needs batteries -- You probably have the wrong abstraction  
  > cunku baska kodlar onun pile ihtitiyac duymadigina guvenerek bir sozlesme imzalarlar.

  > _ex_: her kare bir dikdortgendir.  
  > _ex_: her deve kusu bir kustur  
  > bu ornekler matematiksel olarak dogru (there is also an is-a relationship) ancak **kalitim kullanilmamali!**


  _ex:_
  ```cpp
    class Base {
    public:
        virtual void foo(int x)
        {
            // precondition: input parameter is integer
        }
    };

    class Der : public Base {
    public:
        void foo(int x) override
        {
            if (x < 0) {
                throw ...
            }

            // violates LSP rule:
            // Der requires more, Base.foo does not require x to be negative
            // this is not part of the general agreement (Base's interface)
        }
    }
  ```

#### C++, LSP
__parameters:__
- turemis sinifin override eden fonksiyonlari, taban sinifin virtual fonksiyonlarinin parametrik yapilariyla uyumlu olmali
  * return type -> covariance
  * function parameters -> contravarience

  > Sanal sinif override edildiginde fonksiyonun imzasi ve return type i ayni tur olmali.
  > Eger taban sinifin return type i bir Base type ise, override eden tur bu Base in bir Der type i olabilir.
  >
  > Sebebi:   
  > Derive sinif LSP'yi ihlal etmemesi gerekir, bu durumda davranissal bir sikinti yasanmayacaktir. 

__exception:__
- turemis sinif, taban sinifin exception gonderme kosullarina uymali
- Der sinif Base class dan farkli bir exception gonderirse, Base class in davranisina gore yazilmis kodlar hatali hareket edebilir.


> C++ idiom: non-virtual interface, LSP isterlerini daha iyi kontrol etme olanigi verir.

> burada en zorlayan nokta, invariantlarin korunmus olmasi ilkesi.
> Cunku bu invariantlarin saglanmasi icin Base class in implementationlarinin biliniyor olmasi gerekir

### Telltale signs hinting the violation of LSP
 - Der sinif, Base in bir abstract fonksiyonunu override eder, ancak bu kodun ici bos.   
   Cunku, Der in bu fonksiyon icin yapacak bir isi yok.
 
 - The documentation for an abstract Base class says: do not call this function for the clients  
   LSP violate edildigi icin cagirildiginda sikinti cikacak
 
 - The documentation says: Deve kusu icin fly cagirilmasin
   su tarz cozumler LSP yi ihlal eder: 
   * Deve kusu cagirilirsa exception throw et
   * Deve kusunun fly fonksiyonu baska bir is yapsin

> tipik hata: ortak kodlarin olmasi, programciyi inheritance a tesvik eder.  
> Ancak, inheritance in amacini sadece code reusability olarak gormememiz gerekir.  
> Base beklenen yerde Der kullanilabilmeli.  
> Dolayisiyla, kalitimi kullanirken bagimliligi matematiksel olarak dusunmek yerine __agreement-based bir iliskli__ olarak gormeliyiz.  
> Pre-conditions, Post-conditions a uyuluyor mu? invariantler korunuyor mu? requirments for return types?

_ex:_ Kare-dikdortgen  
Fragility: Dikdortgen icin yazilan fonksiyonlar, 2 kenari esit olan kare icin ayni sonucu vermeyebilir.

## [I]nterface Segregation Principle (ISP)  
  - Client kodlar (Istemciler), kullanmadiklari interface lere bagimli olmaya zorlanmamalidir
  > interface: remember that OOP da "interface" abstract class daki sapsanal fonksiyonlar kastedilir.

  - Make fine-grained interfaces that are client-specific. 
  Clients should not be forced to use interfaces that they won't use

  - Prefer small, cohesive interfaces instead of fat interfaces.
  Tum metodlari kapsayan tek bir interface yerine her biri yari metod gruplarina hizmet veren birden fazla interface olusturun. 

  _ex:_
  ```cpp
    struct IFile {
        virtual ~IFile() = default;
        virtual void write() = 0;
        virtual void read() = 0;
    };

    // bu interface i hem okuma hem yazma islemlerine zorluyoruz
  ```

  bunun yerine sunu yap

  _ex:_
  ```cpp
    struct IRead {
        virtual ~IRead() = default;
        virtual void read() = 0;
    };

    struct IWrite {
        virtual ~IWrite() = default;
        virtual void write() = 0;
    };

    // Ok: Farkli client lar icin farkli interface ler yaz
  ```

  _ex:_
  ```cpp
    class Animal {
    public:
        virtual void eat() = 0;
        virtual void sleep() = 0;
        virtual void walk() = 0;
    };
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
