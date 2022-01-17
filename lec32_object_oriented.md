# Summary of Object Oriented Principles for C++ developers
## Terms
  - analysis  -> to know what to do
  - design    -> to know how to do
  - implementation

"
Designing object-oriented programming is hard, and designing reusable object-oriented software is even harder.
  - You must
    * find pertinent objects, 
    * factor them into classes at the right granularity, 
    * define class interfaces and inheritance hierarchies, 
    * and establish key relationships among them.
  - Your design should be specific to the problem at hand but also general enough to address future problems and requirements.
  - You also want to avoid redesign, or at least minimize it.  

Experienced object-oriented designers will tell you that a reusable and flexible design is difficult if not impossible to get "right" the first time.  
Before a design is finished, they usually try to use it several times, modifying it each time.
"

## Symptoms of a bad design
Robert C. Martin (Uncle Bob)
  - Rigidity
  - Fragility
  - Immobility
  - Viscosity
  - Needless complexity
  - Needless repetition
  - Opacity

## Rigidity
  - katilik
  - degisimin kolay yapilamamasi  
  - ripple effect: bir yeri degistirmek icin bir suru farkli yeri de degistirmek gerekir

"Rigidity is the tendency for software to be difficult to change, even in simple wats. 
Every change causes a cascade of subsequent changes in dependant modules". (Design Principles and Design Patterns by Uncle Bob)

- The design is hard to change
  * changes propogate via dependencies to other modules
  * no continuity in the code
- Management reluctance to change if anything becomes the policy

## Fragility
  - kirilganlik
  - closely related to rigidity

- The design is easy to break
  * The code breaks in unexpected places that have no conceptual realationship with the changed area
- Signs:
  * some modules are constantly on the bug list
  * time is used finding bugs, not fixing them

## Immobility
  - The design is hard to reuse
  - Telltale sign:
    * a module could be reused but the effort and risk of separating it from the original environment is too high
  
## Viscosity (akiskanlik):
  - it is easy to do wrong thing, hard to go correct thing 
    * design a uygun cozumleri uygulamak, uygun olmayan cozumlere gore daha zor
  - the development environment is slow and inefficient 
  - telltale signs:
    * when a change is needed, you are tempted to hack rather than to preserve the original design
  
## Needless Repetition
  - kotu tasarimin sonucu olarak programcilar kopyala-yapistir yapar

## Opacity
  * the tendenceny  of a module to become more difficult to undestand
    - a constant effort is needed to keep the code readable
  * telltale sign:
    - you are reluctant to fix somebody else's code - or even your own!

## Needless Complexity
  - too much anticipation of future needs
  - developers try to protect themselves against probable future changes
  - telltale sign: investing in uncertainty
  
Extra complexity is needed only when designing an application framework or customizable component!

===
# cohesion
  - ozyapiskanlik
  - intra module
  - modullerin kendi bilesenleri arasindaki alakanin yogunlugu
    * ex: sinifin sorumluluklari ne kadar birbiriyle alakali
  - internal glue of the module

  => there are metrics and programs that show the cohesion ratio but there is no standard

book: pragmatic programmer


## low cohesion
  - monolitik siniflar
  - siniflarin onlarca farkli sorumlulugu var
  - low maintainability

  _ex_ low cohesion
   - stdlib.h
    * random number generators
    * exit/abort
    * malloc/calloc/realloc/free
    * atoi
    * abs
    * qsort

## higher cohesion
 - higher maintanability
 - higher reusability

# cohesion categories (interview)
  from worse to best:
  - coincidental
  - logical
  - temporal
  - procedural
  - communicational
  - sequence
  - functional

## coincidental cohesion (bad)
  * rastgele bir araya gelmisler
  * birden fazla sorumluluk var
  * farkli isler yapan bilesenler var

  _ex:_ utility headers
  
## logical cohesion (mostly bad)
  * same objective: encryption, math
  * birliktelik temasi var ama sadece kavramsal
    ex: her seyi encrypt ederiz
    ex: okuma islemi (nereden olursa olsun)
    ex: alan hesaplama (ne olursa olsun)

## temporal cohesion (bad)
  * zamana bagli bir birliktelik var

  _ex:_
  initialize (ayni zaman icersinde yapilan islemler)
  ```cpp
    class Init {
        init_printer();
        init_monitor();
        init_disk();
        ...
    }
  ```

## procedural cohesion (bad but not as bad as the former ones)
  * elements of a module have different responsibilities for the same task
  * eylemler arasi baglanti zayif, ayni amacin farkli adimlari
  * generally low reusability
    
  _ex:_
  ```cpp
    class MakeCake {
        addIngredients();
        void mix();
        void bake();
        void cool();
        ...
    }
  ```

## communicational/ informational cohesion
  - ogeler ayni veri uzerinde islem yaptigi icin bir araya gelmisler
  - farkli islemler, ayni veri uzerinde calisiyor
  
## sequential cohesion (mostly good)
  - modullerden birisinin ciktisi, digerinin girdesi
  
  _ex:_ dosyadan oku, okunan dosyadan islem yap, islem sonucunu yaz

## functional cohesion (ideal situation)
  - modulun elemanlarinin tek bir gorevi var
  - fonksiyonlarin hepsi gerekli
    * refactoring sirasinda bu fonksiyonlardan hic birini ayiramayizz

===
# coupling
  - inter module relationship
  - programi olusturan modullerin birbirleri arasindaki iliski

  it is bad to have coupling, dependency between modules. 
  Yet, it cannot be eliminated since modules should talk with each other.

good: lowly coupled/ loosely coupled
bad: high coupling

### decouple: couplinging ortadan kaldirmak
  - design patterns aim decreasing coupling
  - iliski interface uzerinden yurumeli

# categories of coupling
  from tight (bad) to  loose (good)
  - content level coupling
  - common coupling
  - Control couping
  - Stamp coupling
  - Data coupling

  tight:
    - more interdependency
    - more coordination
    - more information flow
 
 loose:
   - less interdependency
   - less coordination
   - less information flow

## content level coupling
  - bir modul, baska bir modulun implementationina erisir
  - data hiding ihlal edilmis
  - cogunlukla programlama dilleri bunu engelliyor

## common coupling
  - birden fazla modul ayni dataya erisir
  - data nin sorumlulugu kimde belli degil
  - singleton bir common coupling olusturur (maskeli global degisken)
  - fragility (beklenmeyen yan etkiler)

## control coupling
  - bir module diger modulun calisma akisini degistirir 
  - patolojik coupling
  - ok:  moduller birbirinin ic yapilarini bilmiyorlarsa cogunlukla problem yok
  - bad: bir modul diger modulun nasil calistigini biliyorsa cok kotu

## stamp coupling
  - 2 modul arasi bir veri yapisinin kullanimi acisindan bir baglanti var
  - bad: bir veri yapisi buna erisimi olmayan bir modulde kullaniliyorsa ve bu modul bu veri yapisinin nasil manipule edilecegi bilgisine sahipse kotu. 

=> bad but used frequently in C++: because of efficiency 
 - C++ programmers want data-oriented programming, not object oriented

## data coupling (ideal)
  - moduller arasi sadece parametre yoluyla iletisim var

# decreasing coupling
  - modullerin sinirlarini fonksiyonel olarak sinirlandirmak
  - seperation of concerns
    * birbiriyle dogrudan alakasi olmayan concernler ayri ayri gelistirilsin
    * ayni modul icerisindeki ogeler yuksek cohesion icinde olmali

why inheritance is crucial for object oriented programming:
  - coupling i dusuk tutmanin en temel yolu interface ile baglanmak 
  - inheritance demek objelerin birbiriyle interface uzerinden haberlesmesi demek
  - interface ile haberlesmek ayni zamanda maliyet demek, kalitim/runtime polymorphism demek


===
# Terms in Object oriented Programming:
  cogunlukla soyutlama seviyeleri ile ilgili
  stack overflow: bu kavramlarin taxonomy
  gredy butch: object ...

## Taxonomy: From abstract to concrete
-Level-1: Main principles (Level - 1)
  * abstraction
  * encapsulation
  * hierarchy
  * inheritance, aggregation, modularity

- Level-2: Principals that aids and better main principles (Level - 2)
  - Solid principles
  - Grasp principles
  - law of Demeter

- Level-3: Design Patterns (Level - 3)

- Level-4: Heuristics (Level - 4)
   book: object oriented design heuristics

- Level-5: Guidelines (Level - 5)


===
# Solid principles (Robert C. Martin)
  [S]ingle Responsibility Principle (SRP)
  [O]pen Closed Principle (OCP)
  [L]iskov Substitution Principle (LSP)
  [I]nterface Segregation Principle (ISP)
  [D]ependancy Inversion Principle (DIP)

## Single Responsibility Principle:
  - bir sinifin bir sorumlulugu olmali
  - eger bir fonksiyon degisirse diger fonksiyonlarda ayni hedefe yonelik degismeli ???
  - avoid "and" in responsibility

  => suphe cekici noktalar (code smell):
  - testlerde cok fazla case var 
  - siniflarin isimlendirilmesinde "and" var
  - ctor da cok fazla parametresi olmasi
  - fonksiyonlarin kodunun uzun olmasi