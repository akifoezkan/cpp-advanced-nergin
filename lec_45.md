# ilk 10 dk gec kaldim

#
  _ex:_
  ```cpp
    // shape.h
    #pragma once
    #include <string>

    class Shape {
    public:
        // interface
        virtual ~Shape() = default;
        virtual std::string name() const = 0;

        // .. 
    };
  ```

  ```cpp
    // circle.h
    #include "shape.h"
    class Circle : public Shape {
    public:
        Circle(double r) : m_radius{r} {}
        double get_radius const { return m_radius; }
        virtual std::string name() const override { return "circle"; }
    private:
        double m_radius;
    };
  ```

  ```cpp
    // rectangle.h
    #include "shape.h"
    class Rectangle : public Shape {
    public:
        Rectangle(double e1, doublee2) : m_edge1{e1}, m_edge2{e2} {}
        double get_short_edge() const { return std::min(m_edge1, m_edge2); }
        double get_long_edge() const { return std::max(m_edge1, m_edge2); }
        virtual std::string name() const override { return "rectangle"; }
    private:
        double m_edge1;
        double m_edge2;
    };
  ```

  ```cpp
    // circle.h
    #include "shape.h"
    class Square: public Shape {
    public:
        Square(double e) : m_edge{r} {}
        double get_edge const { return m_edge; }
        virtual std::string name() const override { return "square"; }
    private:
        double m_edge;
    };
  ```

  ```cpp
    // triangle.h
    #include "shape.h"
    class Triangle: public Shape {
    public:
        Triangle(double e1, doublee2) : m_edge1{e1}, m_edge2{e2}, m_edge3{e3} {}
        double get_edge1() const { return m_edge1; }
        double get_edge2() const { return m_edge2; }
        double get_edge3() const { return m_edge3; }

        virtual std::string name() const override { return "triangle"; }

    private:
        double m_edge1;
        double m_edge2;
        double m_edge3;
    };
  ```

  ```cpp
    #include "circle.h"
    #include "rectangle.h"
    #include "square.h"
    #include "triangle.h"
    #include "iostream"

    Shape* create_random_shape()
    {
        // normally use smart pointers

        static std::mt19937 eng{ std::random_device{}() };  // size=5000 byte
        static std::uniform_int_distribution  idist{0, 3};
        static std::uniform_real_distribution rdist{0., 9.};

        switch(idist(eng))
        {
        case 0: return new Circle{ rdist(eng) };
        case 1: return new Square{ rdist(eng) };
        case 2: return new Rectangle{ rdist(eng), rdist(eng) };
        case 3: return new Triangle{ rdist(eng), rdist(eng), rdist(eng) }; // geometrically might not be a triangle
        }

        return nullptr;
    }

    int main()
    {
        using namespace std;
        vector<Shape*> svec;

        for(int i = 0; i < 1000; ++i) {
            svec.push_back(create_random_shape());
        }

        for(auto p : svec)
            p-> name();

        for(auto p : svec)
            delete p;
    }
  ```

  yukaridaki gibi bir kod var.
  Buna yeni bir ozellik eklemek istiryoruz (ex: yukseklik hesaplama)
  Bu kodun kapali hierarchy olacagini bildigimizden sunu yapiyoruz:
  1. shape_visitor class i yaz 
  2. add an accept mmember function to all the classes (template prog. for boilerplate code) 
  3. now Shapevisitor can be inherited for other visitors

  ```cpp
    // shape.h
    #pragma once
    #include <string>

    class ShapeVisitor; // ***

    class Shape {
    public:
        // interface
        virtual ~Shape() = default;
        virtual std::string name() const = 0;
        
        virtual void accept(SahpeVisitor&) = 0;     // ***

        // .. 
    };
  ```

  ```cpp
    // shape_visitor.h ***
    #pragma once;

    // kapali hierarchy
    class Circle;
    class Square;
    class Rectangle
    class Triangle; 

    class ShapeVisitor {
    public:
        virtual void visit(Circle&) = 0;
        virtual void visit(Square&) = 0;
        virtual void visit(Rectangle&) = 0;
        virtual void visit(Triangle&) = 0;
    };
  ```

  ```cpp
    // circle.h
    #include "shape.h"
    #include "shape_visitor.h"  //***
    
    class Circle : public Shape {
    public:
        Circle(double r) : m_radius{r} {}
        double get_radius const { return m_radius; }
        virtual std::string name() const override { return "circle"; }

        // ***
        virtual void accept(ShapeVisitor& visitor) override
        {
            visitor.visit(*this);
        }
    private:
        double m_radius;
    };
  ```

  ```cpp
    // rectangle.h
    #include "shape.h"
    #include "shape_visitor.h"  //***

    class Rectangle : public Shape {
    public:
        Rectangle(double e1, doublee2) : m_edge1{e1}, m_edge2{e2} {}
        double get_short_edge() const { return std::min(m_edge1, m_edge2); }
        double get_long_edge() const { return std::max(m_edge1, m_edge2); }
        virtual std::string name() const override { return "rectangle"; }
        
        // ***
        virtual void accept(ShapeVisitor& visitor) override
        {
            visitor.visit(*this);
        }
    private:
        double m_edge1;
        double m_edge2;
    };
  ```

  ```cpp
    // circle.h
    #include "shape.h"
    #include "shape_visitor.h"  // ***

    class Square: public Shape {
    public:
        Square(double e) : m_edge{r} {}
        double get_edge const { return m_edge; }
        virtual std::string name() const override { return "square"; }
        
        // ***
        virtual void accept(ShapeVisitor& visitor) override
        {
            visitor.visit(*this);
        }
    private:
        double m_edge;
    };
  ```

  ```cpp
    // triangle.h
    #include "shape.h"
    #include "shape_visitor.h"  // ***

    class Triangle: public Shape {
    public:
        Triangle(double e1, doublee2) : m_edge1{e1}, m_edge2{e2}, m_edge3{e3} {}
        double get_edge1() const { return m_edge1; }
        double get_edge2() const { return m_edge2; }
        double get_edge3() const { return m_edge3; }

        virtual std::string name() const override { return "triangle"; }
        
        // ***
        virtual void accept(ShapeVisitor& visitor) override
        {
            visitor.visit(*this);
        }

    private:
        double m_edge1;
        double m_edge2;
        double m_edge3;
    };
  ```

> visitor classes
  ```cpp
    class HeightVistior : public ShapeVisitor {
    public:
        void visit(Circle&) override;
        void visit(Square&) override;
        void visit(Rectangle&) override;
        void visit(Triangle&) override;
        double get_total_height() const { return m_total_height; }

    private:
        double m_total_height{}; 
    }
  ```

> yeni davranis icin kod ekledim: open-closed principle
> interface i kirletmedik
> ...

  _ex:_
  ```cpp
    #include "height_visitor.h"
    #include "circle.h"
    #include "square.h"
    #include "rectangle.h"
    #include "triangle.h"

    void HeightVisitor::visit(Circle& circle)
    {
        m_total_height += circle.get_radius() * 2;
    }

    void HeightVisitor::visit(Square& square)
    {
        m_total_height += circle.get_edge();
    }

    void HeightVisitor::visit(Rectangle& rect)
    {
        m_total_height += rect.get_long_edge();
    }

    void HeightVisitor::visit(Triangle& triangle)
    {
        m_total_height + = std::max({tr.get_edge_1(),  tr.get_edge_2(), tr.get_edge_3()});
    }
  ```

  ```cpp
    #include "circle.h"
    #include "rectangle.h"
    #include "square.h"
    #include "triangle.h"
    #include "iostream"

    int main()
    {
        using namespace std;
        vector<Shape*> svec;

        for(int i = 0; i < 1000; ++i) {
            svec.push_back(create_random_shape());
        }

        HeightVisitor hv;

        for(auto p : svec)
            p-> accept(hv);

        std::cout << "toplam yukseklik degeri = " << hv->get_total_height() << "\n";

        for(auto p : svec)
            delete p;
    }
  ```

# ara sonrasi
  _ex:_
  ```cpp
    FeedingVisitor : public PetVisitor
    ...
  ```

book: Hands-On Design Patterns with C++: Solve common C++ problems with modern design patterns and build robust applications (English Edition)

# variant, visit, recap

====================================== Adapter
# Adapter pattern 
- Kodundan faydalanmak istedigimiz Adaptee sinifini, ... 
- inheritance yada composition ile olabilir
- STL examples:
   container adapters (stack, queue, priority_queue)

  _ex:_
  ```cpp
   
  ```

# decorater
- bir nesneye saydam ve dinaik olarak yeni sorumluluk/yetenek eklemek. Bunu yaparken diger nesneleri etkilememek.

- kalitimdaki en kotu ozelliklerden biri ...
- her davranis icin exponantial olarak kalitimla elde edilen sinif turu artar.
  Alternatif bu ozellikleri nesnelere dinamik olarak verek

  _ex:_
  ```cpp
    Widget* aWidget = new BorderDecoratot ()'
  ```

# en sik kullanilan patternler

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
