#include <systemc.h>

SC_MODULE (hello_world) {
  SC_CTOR (hello_world) {
    // Nothing in constructor
  }
  void say_hello() {
    //Print "Hello World" to the console.
    cout << "Hello World.\n";
  }
};

// sc_main in top level function like in C++ main
int sc_main(int argc, char* argv[]) {

  cout << "\n\n=== Begin Program of systemC  ========================================================== \n\n";

  hello_world hello("HELLO");
  // Print the hello world
  hello.say_hello();

  cout << "\n\n=== End Program of systemC    ==========================================================";
  return(0);
}