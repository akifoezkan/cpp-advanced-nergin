# facade recap 

  _ex:_
  ```cpp
    class Console
    ...
  ```

# composite pattern 
  nesneleri agac yapilari seklinde ifade etmemizi ve bu nesnelerden her birini tek bir nesne olarak kullanabilmemizi saglar.

  ex: folder - file : hem file lari hem folder lari tek bir nesne gibi kullanmak

  ex: refactoring guru: 
  kutularin icinde hem kutu, hem urunler var
  kutunun toplam fiyati nedir
  add picture

  composite tasarim kalibi ile urunleri ve kutulari ortak bir interface ile isleyebiliriz
  -> bir urun icin metod sadece urunun fiyatini dondurur
  -> bir kutu icin metod kutudaki her kalemi dolasarak toplam fiyati dondurur

  > uygulamanin agac seklinde gosterilmesi gerekir


  advantages
  -> uses the advantages of polymorphism and recursive to work with comlex data structures
  -> open/closed principle: var olan kodu bozmadan yeni eleman turleri eklenebilir
  
  disadvantages:
  -> islevsellikleri birbirinden cok farkli olan siniflar icin ortak bir arayuz olustrumak zor olabilir
  -> component arayuzunun gereginden fazla genellestirilmesi yapinin anlasilmasini zor hale getirir

  _ex:_
  ```cpp
    class Component {
    protected:
        Component* m_parent;    // bunu tutmak zorunda degil

    public:
        virtual ~Component() = default;

        void set_parent(Component* parent)
        
        Component* get_parent(Component* parent)

        // add ve remove leaf tarafinda implement edilmeyecek
        // onlarin boyle bir islevi yok
        // bos olabilir yada exception throw edebilir
        virtual void add(Component*) = 0;
        virtual void remove(Component*) = 0;

        virtual bool is_composite() const
        
        // operations
        virtual std::string is_composite() const = 0;
    };

    class Leaf : public Component {
    public:
        std::string operation() const override
        {
            return "Leaf";
        }


        // add ve remove default edildiginde icin burada override edilmesine gerek kalmamis
        // alternatif olarak add ve remove sapsanal fonksiyon olabilir ve bos implement edilebilir
    };

    class Composite : public Component {
    protected:
        std::list<Component*> m_children_list;

    public:
        void add(Component* pcom) override
        {
            m_children_list.push_back(pcom);
            pcom->set_parent(this);
        }

        void remove(Component* pcomp) override
        {
            m_children_list.remove(pcomp);
            pcomp->set_parent(nullptr);
        }

        bool is_composite() const override
        {
            return true;
        }
       
        std::string operation() const override
        {
            std::string result;
            for(const Component( c : m_children_list)
            {
            
                if(c == m_children_list.back())
                    result + = c->operation();

                ....
            }
        }
    }

    // ...

    int main()
    {
        Component* simple = new Leaf;
        ClientCode(simple);


        Component( tree = new Composite;
        Component( branch1 = new Composite;

        Component* leaf1 = new Leaf;
        // ...
    }
  ```

  _ex:_
  ```cpp
    // component.h
    #pragma once
    #include<vector>

    class Component {
    public:
        virtual ~Component() = default;
        virtual void operation() = 0;
        virtual void add_component(Component* ) = 0;
        virtual void remove_component(Component* ) = 0;
    };
  ```

  ```cpp
    // leaf.h
    #include "component.h"

    class Leaf : public Component {
    public:
        virtual void operation() = 0;

        // bos, cunku leaf
        virtual void add_component(Component*) override {}
        virtual void remove_component(Component*) override {}
    }'
  ```

  ```cpp
    // composite.h
    #pragma once
    #include<component>

    class Composite : public Component {
    public:
        virtual void operation() override ;
        virtual void add_component(Component* ) = 0;
        virtual void remove_component(Component* ) = 0;

    private:
        std::vector<Component*> m_comp_vec;
    };
  ```

  ```cpp
    // leaf.cpp
    #include "leaf.h"

    void Leaf::operation()
    {
        std::cout < "Leaf::operation()\n";
    }
  ```

  ```cpp
    #include <cassert>

    //composite.cpp
    void composite::operation()
    {
        std::cout << "composite operation\n";
        for(auto pcomp : m_comp_vec) {
            pcomp->operation();
            std::cout << "\n";
        }
    }

    void composite::add_component()
    {
        m_comp_vec.push_back(pcomp);
    }

    void composite::remove_component()
    {
        // m_comp_vec.erase(remove(m_comp_vec.begin(), m_comp_vec.end(), pcomp), m_comp_vec.end());

        // c++20
        std::size_t n = erase(m_comp_vec, pcomp);   // ADL, erase <- m_comp_vec
        assert(n != 0);

        // note that erasing pcomp does not end its life
        // this should be handled somewhere else
    }
  ```

  ```cpp
  #include "leaf.h"
  #include "composite.h"

  int main()
  {
    Leaf lf_1, lef_2, lf_3;
    Composite root;

    root.add_component(&lf_1);
    root.add_component(&lf_2);

    Composite branch;
    branch.add_component(&lf_3);

    root.add_component(&branch);
    func(&root);
  }
  ```

# book recommendations??

# Command pattern 
  - bir request, bu istegin gerceklestirilmesine iliskin tum bilgileri iceren bir sinif nesnesi haline donusturulur

  factoring guru:
  - bir metin 'duzenleyici program'
  - Button sinifi, toolbar ve baska yerler

  seperation of concerns:
  - requires layering

  GUI layer:
  - ekrandaki resimlerin/cizimlerin olusturulmasi
  - kullanici girislerinin alinmasi
  - ..

  Bussines logic layer
  - onemli isleri GUI layer Business logic layer a delegate eder


  > command, receiver ve receiver parametrelerini bilir
  > invoker commandi olusturur ve execute fonksiyonunu cagirir
  > isin kendisini receiver gerceklestirir
  > client istege dair butun parametreleri command sinifinin ctor una gecer. Bu 


  - comamnd leri fonk. argumani olarak gecilebilir
  - command ler nesneler icinde saklanabilir
  - Linked komutlar calisma zamani degisebilir

  kullanim alanlari:
  - operasyonlari bir kuyruga almak istedigimizde
  - operasyonlarin yurutulmelerini bir schedule
  - onlarin uzaktan yurutulmesini sitedigimzde

  bir command nesnesi serialize edilebilir
  yani  nesne -> string e donusturulur

> random siniflari inserter/extractor fonk ile state i serialize/deserialize edilebilir 

  redo/undo islemlerinin en sik kullanilan teknik Command tasarim kalibidir


  advantages
  - single responsibility: 
  operasyonlari cagiran ve isi yapana rasinda baglasiklik yok

  - open closed principle: 
  kod degistirmeden
  yeni concrete command eklenebilir
  undo/redo islemleri gerceklestirilebilir
  basit commandler daha karmasik bir Comman i olusturmak icin birlestirilebilir

  _ex:_
  ```cpp
    refactoring guru example
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
