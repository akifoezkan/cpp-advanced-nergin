

  _ex:_
  ```cpp
    Myclass{
    public:
        Myclass()
        {
            std::cout << "Myclass ctor this: " << this << "\n";
        }

        ~Myclass()
        {
            std::cout << "Myclass dtor this: " << this << "\n";
        }

        void func()
        {
            std::cout << "Myclass func this: " << this << "\n";
        }
    };


    lazy_init<struct gm1_tag, Myclass> gm1;
    lazy_init<struct gm2_tag, Myclass> gm2;

    int main()
    {
        gm1->func();
        gm2->func();
    }
  ```

  tag olmasa 1 obje olusacakti

##
  _ex:_
  ```cpp
    class Usingleton{
    private:
        struct Deleter
        {
            void operator()(USingleton* p);
        }

        inline static std::unique_ptr<USingleton, Deleter> mp_instance;
        USingleton()
        ~USingleton();
    };

    ...
  ```

# Monostate
 singleton in kilik degistrimis hali
 sinifin data memberlari static, dolayisiyla butun nesneler ayni sonucu uretir

  _ex:_
  ```cpp
    class Clock {
    public:
        int get_hour() const;
        int get_min() const;
        int get_sec() const;
        std::string get_current_time() const;
    private:
        static inline int m_hour{};
        static inline int m_min{};
        static inline int m_sec{};
    };
  ```
> sinifin butun uye fonksiyonlarini da static yapabiliriz.

  _ex:_
   chrono::clock siniflari, monostate

## monostate vs singleton

singleton
  - tek nesne
  - istersek lazy init yapabiliriz
  - static instance fonksiyonu gerekli
  - inheritance ve polymorphism yapilabilir

monostate
  - birden fazla nesne olabilir
  - istersek lazy init yapamayiz
  - butun public fonksiyonlar static
  - static fonksiyonlar sanal olamaz


#

  _ex:_
  ```cpp
    // printer_base.h

    class PrinterBase {
    protected:
        printerBase() = default; 
    
    public:
        virtual ~PrinterBase() = default;

        // Singleton, copy operations are not allowed
        printerBase(const PrinterBase&) = delete;
        printerBase& operator=(const PrinterBase&) = delete;

        virtual void print(const std::string&) = 0;
    };

    using PrinterPtr = std::shared_ptr<PrinterBase>;
  ```

  ```cpp
    // html_printer.h

    #pragma once;
    #include "printerbase.h"

    class HtmlPrinter : public PrinterBase {
    private:
        HtmlPrinter();

    public:
        ~HtmlPrinter();

        static PrinterPtr get_instance();
        virtual void print(const std::string&) override;
    };
  ```

  ```cpp
    // xml_printer.h

    #pragma once;
    #include "printerbase.h"

    class XmlPrinter : public PrinterBase {
    private:
        XmlPrinter();

    public:
        ~XmlPrinter();

        static PrinterPtr get_instance();
        virtual void print(const std::string&) override;
    };
  ```

  ```cpp
    // pdf_printer.h

    #pragma once;
    #include "printerbase.h"

    class PdfPrinter : public PrinterBase {
    private:
        PdfPrinter();

    public:
        ~PdfPrinter();

        static PrinterPtr get_instance();
        virtual void print(const std::string&) override;
    };
  ```

  ```cpp
    //html_printer.cpp

    HtmlPrinter::HtmlPrinter()
    {
        std::cout << "HTML printer olustruldu\n";
    }

    HtmlPrinter::~HtmlPrinter()
    {
        std::cout << "HTML printer yok edildi\n";
    }

    HtmlPrinter::get_instance()
    {
        static PrinterPtr ptr{ new HtmlPrinter{} };
        return ptr;
    }

    HtmlPrinter::print(const std::string& str)
    {
        std::cout << "Html Printer : " << str << '\n';
    }
  ```

  ```cpp
    //pdf_printer.cpp

    PdfPrinter::PdfPrinter()
    {
        std::cout << "Pdf printer olustruldu\n";
    }

    PdfPrinter::~PdfPrinter()
    {
        std::cout << "Pdf printer yok edildi\n";
    }

    PdfPrinter::get_instance()
    {
        return PrinterPtr{ new PdfPrinter{} };
    }

    PdfPrinter::print(const std::string& str)
    {
        std::cout << "Pdf Printer : " << str << '\n';
    }
  ```

  ```cpp
    //xml_printer.cpp

    XmlPrinter::XmlPrinter()
    {
        std::cout << "Xml printer olustruldu\n";
    }

    XmlPrinter::~XmlPrinter()
    {
        std::cout << "Xml printer yok edildi\n";
    }

    XmlPrinter::get_instance()
    {
        return PrinterPtr{ new XmlPrinter{} };
    }

    XmlPrinter::print(const std::string& str)
    {
        std::cout << "Xml Printer : " << str << '\n';
    }
  ```

  ```cpp
    // printer_provider.h
    #pragma once
    #include <functional>

  _ex:_
    using CreatorFunc = std::function<PrinterPtr()>;

    class PrinterProvider {
    private:
        struct InstanceData {
            PrinterPtr m_printer;
            CreatorFunc m_creator;
        };
        inline static std::unordered_map<std::string, InstanceData> mprinter_map{};

    public:
        static void register_creator(const std::string& key, CreatorFunc func);
        static void get_printer(const std::string& key);
    };
  ```

  _ex:_
  ```cpp
    void PrinterProvider::register_creator(const std::string& key, CreatorFunc func)
    {
        if(auto iter = mprinter_map.find(key); iter == mprinter_map.end())
        {
           mprinter_map[key].m_creator = func; 
            std::cout << "singleton is registered!\n";
        {
        else
        {
            std::cout << "already registered!\n";
        }
    }

    void PrinterProvider::get_printer(const std::string& key)
    {
        if(mprinter_map.contains(key))
        {
            if(!mprinter_map[key].m_printer)
            {
                mprinter_map[key].m_printer = mprinter_map[key].m_creator();
            }

            return mprinter_map[key].m_printer;
        }

        std::cout << "wrong key value\n";
        return {};
    }
  ```

  ```cpp
    #include "html_printer.h"
    #include "xml_printer.h"
    #include "pdf_printer.h"
    #include "printer_provider"

    int main()
    {
        PrinterProvide::register_creator("xml", &XmlPrinter::get_instance)
        PrinterProvide::register_creator("html", &HtmlPrinter::get_instance)
        PrinterProvide::register_creator("pdf", &PdfPrinter::get_instance)

        auto p1 = PrinterProvider::get_printer("xml");
        if(p1)
        {
            p1->print("singleton is an antipattern!");
        }

        auto p2 = PrinterProvider::get_printer("xml");
        if(p2)
        {
            p2->print("necati ergin");
        }
    }
  ```

  kayit mekanizmasi sayesinde butun singletonlar bir yerde tutuluyor.
  olusturulan her singleton in tek bir yerden kontrolu saglanir


# Fabrika pattern
  - gof daki fabrika metodu make_shared gibi fabrika fonksiyonlari ile ilgili degil

> named constructor: ctor degil ama ctor gibi kullanilir
bir sinif nesnesini dogrudan ctor ile olusturmakla, bir fonksiyon kullanarak bu sinifi olustrumak arasinda ne gibi farklar olabilir?

named ctor kullanma nedenleri:
- named ctor return value ile basari degeri gondermek (exception handling kullanilamiyorsa)?

- overloading yerine named ctor nesneyi hangi sekilde kurdugunu anlatabilir
    _ex:_
    ```cpp
      Myclass create_cartesian()
      Myclass create_polar()
    ```
- named ctor, parametre sayisini azaltabilir
  _ex:_ policy tag veya bir bool deger yerine ctor name

- function overloading icin imzalarin farkli olmasi gerekir. 
  ancak ayni signature a sahip farkli named ctor tanimlanabilir

- post initialize: constructor cagirildiktan sonra farkli bir takim isler yapilmasi gerekebilir

- duruma gore nesne olusturup/olusturmama karari.
  ctor exception throw etmedigi surece objeyi olusturur.

- [en onemli] nesnenin turu programin calisma zamaninda belli olursa 
  _ex:_  menu den secim yap 

  _ex:_
  ```cpp
    void func(Car* p)
    {
        // mercedes vs ford ...
    }
  ```

> Java gibi dillerde new i hic kullanma, her zaman fabrika fonksiyonlarini kullan diyorlar
> ancak bu C++ icin cok kati bir durum olurdu.

> Bazi dillerde global fonksiyon olmadigi icin, baska classlar i olusturan class kullanimi yaygin

> Fabrika metodu en cok kullanilan patternlerden biri

> Gof patternlerinden, Factory metod ve abstract factory metodlari birlikte anlatilmali.
> bunlar hem ayni hem ayni degil.

## GoF, Factory Method
- **Intent:** define an interface for creating an object, but let subclasses decide which class to instantiate.
   Factory Method lets a class defer instantitation to subclasses.

  **also known as:** virtual Ctor

  resim: Gof daki diagram

Product (Document)
   defines the interface of objects the factory method creates

ConcreteProduct (MyDocument)
  implements the Product interface

Creator (Application)
  declares the factory method, which returns an object of type Product.
  Creator may also define a default implementation of the factory metod that returns a ConcreteProduct object.

ConcreteCreator
  


> Note that Creator class herhangi bir sinif olabilir, 
> Creator in kullandigi nesnenenin turu programin calisma zamaninda anlasilacak.
> ?? 

> template metod, creator metoda cok benzer.
> hangi isin yapilaca ??

  _ex:_
  ```cpp
    #include "creator.h"

    class YCreator : public Creator {
    public:
        void operation();   // one of the operations of Creator
        {
            m_product = create();           // create sanal fonk. kullanilarak obje olusturulacak
            m_product->do_something();

            // creator olarak concrete implementationlarina bagli degiliz
            // dolayisyla concrete_product class larin headerlarini include etmiyoruz
        }

        virtual Product* create() = 0;   // factory method will create Product instances

        virtual ~Creator() { ...}

    private:
        Product* m_product{};
    }

    class Product {
    public:
        ~Product() = default;
        v

    }

    class XProduct : Public Product {

    }

  ```

  ```cpp
    #include "creator.h"
    #include "x_creator.h"

    int main()
    {
       XCreator c;

       c.operation();
    }
  ```

> Creator in tek kullanim nedeni bir nesne olusturma degil

  _ex:_
  ```cpp
    //person.h

    #pragma once
    #include<memory>

    class Car;
    using CarPtr = std::unique_ptr<Car>;

    class Person {
    public:
        virtual ~Person() = default;

        virtual CarPtr create_car() = 0;    // factory method, pure virtual olmak zorunda degil

        void travel();
    };

  ```

  ```cpp
    // person.cpp
    #include "person.h"
    #include "car.h"

    void Person::travel()
    {
        CarPtr ptr = create_car();
        ptr->start();
        ptr->run();
        ptr->stop();

        // Biz sadece car interface ine bagliyiz
    }
  ```

  ```cpp
  // car.h

  class Car{
    public:
        virtual ~Car() = default;
        virtual void start() = 0;
        virtual void run() = 0;
        virtual void stop() = 0;
  }
  ```

  ```cpp
    // student.h
    #include "person.h"

    // Concrete creator, person in factory metod unu override eder
    class Student : public Person {
    public:
        Student();
        ~Student();
        virtual CarPtr create_car() override;
    };
  ```

Factory metod (GoF)
Parametric factory method
  - turden nesne olusturacagini bir string veya enum ile belirlenir
  - fabrika ayri, creator dan kalitim yoluyla bir sinif elde edilmeyecek


  ```cpp
    class FileManager {
    public:
        newFile();
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
