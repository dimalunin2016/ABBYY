#include <iostream>
#include <vector>
#include <setjmp.h>

enum exeptions {
  UsualExeption = 0,
  DivideZeroExeption = 1,
  BadFileExeption = 2,
  AnyExeption = 3, // in case to reveal any exeption
  NoExeption = 4 // must be the last element

};

class BasicExeptionParentClass;

const exeptions no_exeption = NoExeption;
exeptions current_exeption = NoExeption;
std::vector<jmp_buf*> jumping_array;
std::vector<int> at_beginning_try_curr_obj_size;
std::vector<BasicExeptionParentClass*> curr_objects;

// use this class as parent class for all classes in yor try blocks
class BasicExeptionParentClass {
  public:

    BasicExeptionParentClass() {
      std::cout << "class created\n";
      curr_objects.push_back(this);
    }

    ~BasicExeptionParentClass() {
      std::cout << "class deleted\n";
      curr_objects.pop_back();
    }

};

jmp_buf* curr_jump = nullptr;
int jump_exeption  = 0;

void try_prepare() {

  at_beginning_try_curr_obj_size.push_back(curr_objects.size());
  jumping_array.push_back(new jmp_buf[1]);
  curr_jump = jumping_array.back();
}

#define TRY(code)                                     \
  try_prepare();                                      \
  jump_exeption = setjmp(*curr_jump);                 \
  if (!jump_exeption) {                               \
    code;                                             \
    at_beginning_try_curr_obj_size.pop_back();        \
  }                                                   \
  delete[] jumping_array.back();                      \
  jumping_array.pop_back();                           \


void delete_objects() {

  while(curr_objects.size() > at_beginning_try_curr_obj_size.back()) {
    curr_objects.back()->~BasicExeptionParentClass();
  }
  at_beginning_try_curr_obj_size.pop_back();
}

#define THROW(EXCEPTION)                              \
  if (EXCEPTION > no_exeption) {                      \
    std::cout <<                                      \
      "wrong exeption, you can use <UsualExeption> if you have any problems\n"; \
    exit(1);                                          \
  }                                                   \
  delete_objects();                                   \
  current_exeption = EXCEPTION;                       \
  longjmp(*(jumping_array.back()), 1);

#define CATCH(EXEPTION_TYPE, CODE)                    \
  if (EXEPTION_TYPE == current_exeption ||            \
      (EXEPTION_TYPE == AnyExeption &&                \
       current_exeption != NoExeption)) {             \
    CODE                                              \
    current_exeption = no_exeption;                   \
  }

//use it when you need to catch exeption by external try/catch block
// you can use it after all catch blocks in section
#define STOP_CATCH()                                  \
  if (current_exeption != NoExeption) {               \
    THROW(current_exeption);                          \
  }
