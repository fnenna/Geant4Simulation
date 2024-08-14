#ifndef MarcelloDumper_h
#define MarcelloDumper_h

#include <string>
#include <iostream>
#include <sstream>

namespace Marcello{

  class Dumper{
  public:
    Dumper(){};
    ~Dumper(){};
    std::stringstream& instance(){return outp;}
    void insert(const std::string& s){
      outp<<s<<"\n";
    };
    void insert(const std::string& s, int a){
      outp<<s<<" "<<a<<"\n";
    };
    void insert(const std::string& s1, int a, const std::string& s2){
      outp<<s1<<" "<<a<<" "<<s2<<"\n";
    };
    
    void dump(){
      std::cout<<outp.str()<<std::endl;
    };
    void reset(){
      outp.str("");
      outp.clear();
    };
  private:
  
    std::stringstream outp;
  };
}
#endif
