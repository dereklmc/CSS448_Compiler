#ifndef CONSTANT_H
#define CONSTANT_H

#include "variable.h"
#include "constvalue.h"

class Constant : public Symbol
{
	
  private:
       ConstValue *value;
         
  public:
      Constant(std::string, ConstValue *);
        
      ~Constant();
      
        std::string generateCode() const;
	int getEnumType() {return value->getType();}
      void print(std::ostream&) const;
};

//class Constant : public Variable
//{
	
//  private:
//       ConstValue *value;
         
//   public:
//      Constant(std::string, ConstValue *);
        
//      ~Constant();
        
//      std::string generateCode() const;
        
//      void print(std::ostream&) const;
//};

#endif
