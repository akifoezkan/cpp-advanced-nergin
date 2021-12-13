## variant:: .valueless_by_exception
  Normalde vairant her zaman bir degere sahiptir. 
  Variantin bos oldugunu ifade etmek icin ``monostate`` degeri kullanilir.  

  Ancak, nadir olarak, varianta gonderilen deger bir exception throw edilmesi yuzunden gercekten bos kalabilir. Bu ``valueless_by_exception`` member fonksiyonu ile test edilebilir.
  
  _ex:_
  ```cpp
    struct Nec {
      operator int() {
          throw std::runtime_error{"hata hata"};
          return 1;
      }
    };

    int main()
    {
      using namespace std;

      variant<double, int> var{ 3.4 };

      try {
          var.emplace<1>(Nec{});
              // variant var sahip oldugu "3.4" degerini yok edecek ancak
              // yeni deger'i gonderemeden exception throw edilecek, dolayisiyla
              // variant bos kalacak.
      }
      catch(constd std::exception& ex) {
          std::cout << ex.what() << "\n";
          cout << boolalpha << var.valueless_by_exception() << "\n";
      }
    }
  ```