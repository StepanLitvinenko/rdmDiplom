
#include "Parcer.h"
#include "RdmTypes.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
using namespace RdmTypes;
Parcer::Parcer()
{

}


bool Parcer::ParceFile(const std::string& fileName, Standart standart)
{
  switch (standart) {
  case Gsm:{
    std::cerr<< " at the moment Gsm parcing is not suppoting " <<std::endl;
    return false;
  }
  case Umts:{
   std::string postfixPtotocol = "UMTS";
   auto resultsFromFile = ReadFile<UmtsRdmAnswer>(fileName);
   Process(resultsFromFile, postfixPtotocol);
   return true;
  }
  case Lte:{
   std::string postfixPtotocol = "LTE";
   auto resultsFromFile = ReadFile<LteRdmAnswer>(fileName);
   Process(resultsFromFile, postfixPtotocol);
   return true;
  }
  default:{
    std::cerr<< " ERROR unknown standart "<<std::endl;
    return false;
  }

  }











}
