# misesed the first 1 hour
  - https://refactoring.guru/

# decorator example
  _ex:_
  ```cpp
    class Component {
    public:
        virtual ~Component() = default;
        virtual void do_it() = 0;
    };

    class ConcreteComponent : public Component {
    public:
        ~ConcreteComponent()
        {
            std::cout << "ConcreteComponent dtor\n";
        }

        void do_it() override
        {
            std::cout << "ConcreteComponent::doit()\n";
        }
    }

    class Decorator : public Component {
        Decorator(Component* pcom) : mp_wrappee{pcom} {}
        ~Decorator()
        {
            delete mp_wrappee;
        }

        void do_it() override
        {
            mp_wrappee->do_it(); 
        }

    private:
        Component* mp_wrappee;  // can show a ConcreteCommponent, and decorator
    };


    class XDecorator : public Decorator {
    public:
        XDecorator(Component* pcor) : Decorator{pcor} {}  // can use inherited ctor

        ~XDecorator()
        {
            std::cout << "Xdecorator       ";
        }

        void do_it() override
        {
            std::cout << "XDecorator added behaviour\n";
            Decorator::do_it();
        }
    };

    class YDecorator : public Decorator {
    public:
        YDecorator(Component* pcor) : Decorator{pcor} {}  // can use inherited ctor

        ~YDecorator()
        {
            std::cout << "~Ydecorator()";
        }

        void do_it() override
        {
            std::cout << "XDecorator added behaviour\n";
            Decorator::do_it();
        }
    };

    class ZDecorator : public Decorator {
    public:
        ZDecorator(Component* pcor) : Decorator{pcor} {}  // can use inherited ctor

        ~ZDecorator()
        {
            std::cout << "~Zdecorator()";
        }

        void do_it() override
        {
            std::cout << "XDecorator added behaviour\n";
            Decorator::do_it();
        }
    };

    int main()
    {
        Component* pxy = new YDecorator{new XDecorator{new ConcreteComponent}};
        pyx->do_it();
            //Y added behaviour, X added behaviour, ConcreteComponent::do_it()

        delete pyx;
            // ~YDecorator ~XDecorator ~ConcreteDecorator 
    }
  ```

## cpp-kursu-kodlar/mixin ???
  - mixin de hangi sinifa davranis eklemek istiyorsak, onu taban sinif yapariz
  - static bir yapi

  _ex:_
  ```cpp
     template<typenmae Base>
     class SkillA : public Base
     // ....

     // => Der, Base e skill ekler


     SkillA<SkillB<SkillC<MyClass>>>> x;    // static olarak MyClass a davranis eklendi
  ```

# Proxy Pattern (Structural)
  - proxy: original nesnenin yerine gecen bir nesnenin kullanilmasini saglar
  - vekil (cumhurbaskani yerine basbakan)
  - en cok kullanilan kaliplardan biri

  - Proxy nesne, original nesneye erisimi kontrol eder.bunu neden yapalim:
    * nesneye dogrudan erisim guvenlik riski iceriyor olabilir

    * lazy initialization/loading: bazi nesneler cok kaynak kullaniyor.
      buna ihtiyac olusana kadar (yada belki hic) bu nesne kullanilmaz.
      (virtual proxy)

    * Nesne uzaktadir (remote object). Ona kontrollu, kolaylastirilmis, detaylardan arindirilmis erisim saglanir

    * Nesneye erisim thread-safe degildir. Bu thread-safe hale getirilebilir.

    * Nesneye erisimden once yada sonra ilave islemler yapmak istenebilir. Ornegin loglama.


## nasil yapacagiz:
  - client ile erisimi kontrol edilecek servis nesnesi arasina bir gecis (surrogate) nesne koyulur. 

  - proxy nesnesinin arayuzu saklanann nesne ile ayni olursa, istemci (client) bundan haberdar olmaz. Client original nesne ile aradaki nesneyi ayirt edemez.

  - proxy asil nesneye bagimliligi ortadan kaldirir

> add picture ??
   service interface: proxy ve service ayni interface e sahip
   service:  fayda saglayacak is mantigini gerceklestirecek somut sinif
   proxy:    service nesnesinin hayatini kontrol eder
   client:   service veya proxy i ayni interface ile kullanabilir


## proxy kalibindan farkli sekillerde faydalanabiliriz:
  - lazy initialization/loading, "virtual proxy"
  - protection proxy
  - caching proxy, tekrar eden istekler icin caching
  - 

  _ex:_
  ```cpp
    class Subject {
    public:
        virtual void 
    };

  ```
  
  ```cpp
    #include "subject.h"

    class RealSubject : public Subject
  ```

  ```cpp
  ...
  ```

  _ex:_
  ```cpp
   service: Image

   class BitmapProxy : Image
   // ...
  ```

# Fascade Pattern
 - fascade-> cephe

 - cok fazla alt sistemden kaynaklanan, karmasik bir arayuzu sozkonusu
 bu sistemin daha pratik/intuitive oarak kullanilmasini saglamak

 ex: Windows API lari C ile yazilmis, nesne yonelimli degil
 bunlari coherent, nesne yonelimli hale getirmek

 - yuksek coupling i kaldirarak, client lar icin bir facade (cephe) olusturuyoruz
 client a daha ugun, basit ve daha abstract bir interface


 add resim

## deavantajlar
  - alt sistemin islevselligi azalabilir

  - performans dusebilir

## gerceklestirim
  - fascade, alt sistemin implementationini gizleyerek, client a bagimliligi azaltabilir

  - client kodlarin bu siniflara erisimine olanak saglayabilir

## cestileri
  - opaque fascade
  - transparent fascade

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
