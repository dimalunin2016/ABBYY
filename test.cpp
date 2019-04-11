#include <iostream>
#include "Exeption.h"
#include <string.h>

class Dummy : public BasicExeptionParentClass {
  public:
    int curr = 0;
};

//simple testing
void test0() {

  TRY(
      std::cout <<"I am here 0\n";
      THROW(UsualExeption);
      std::cout <<"end 0\n";
      );
  CATCH(BadFileExeption, std::cout <<"cath Bad file exeption 0\n";);
  CATCH(UsualExeption, std::cout <<"cath usual exeption 0\n";);
}

//testing try in try situation
void test1() {

  Dummy a;
  TRY(
      std::cout <<"I am here 1, 0\n";
      Dummy x;
      TRY(std::cout <<"I am here 1, 1\n";
          Dummy y;
          THROW(BadFileExeption));

      CATCH(BadFileExeption, std::cout <<"cath Bad file exeption 1, 1\n";);

      THROW(UsualExeption);
      std::cout <<"end 1\n";
      );
  CATCH(BadFileExeption, std::cout <<"cath Bad file exeption 1\n";);
  CATCH(UsualExeption, std::cout <<"cath usual exeption 1\n";);
  STOP_CATCH();
}

//testing try in try situation (no exeption raised inside)
void test2() {

  TRY(
      std::cout <<"I am here 2, 0\n";
      Dummy x;
      TRY(std::cout <<"I am here 2, 1\n";
          Dummy y;);
      CATCH(AnyExeption, std::cout <<"cath Bad file exeption 2, 1\n";);

      THROW(UsualExeption);
      std::cout <<"end 2\n";
      );
  CATCH(AnyExeption, std::cout <<"cath all exeptions 2\n";);
}

//testing try in try, where exeption catched by last CATCH
void test3() {

  TRY(
      std::cout <<"I am here 3, 0\n";
      Dummy x;
      TRY(std::cout <<"I am here 3, 1\n";
          Dummy y;
          THROW(UsualExeption););
      CATCH(BadFileExeption, std::cout <<"cath File exeptions 3\n";);
      STOP_CATCH();
      std::cout <<"end 3\n";
      );
  CATCH(AnyExeption, std::cout <<"cath all exeptions 3\n";);
}
int main() {
  std::cout <<"test0============================\n";
  test0();
  std::cout <<"test1============================\n";
  test1();
  std::cout <<"test2============================\n";
  test2();
  std::cout <<"test3============================\n";
  test3();
  return 0;
}
