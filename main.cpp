#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <list>
#include <tuple>

#include <map>
#include "Common/RdmTypes.h"
#include "Common/Parcer.h"
using namespace std;
using namespace RdmTypes;




int main()
{
  Parcer parcerFile;

    std::string fileName = "/home/stepan/RdmDiplom/FilesDataRdm/LTE/lte_rdm_2020-12-03_07-55-46.dat";
    auto resultsFromFile = parcerFile.ParceFile(fileName, Gsm);

    return 0;
}
