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

    std::string fileName = "/home/stepan/.postlink/Files/Шевелев Александр Евгеньевич/Data/UMTS/umts_rdm_2020-12-03_07-41-16.dat";
    auto resultsFromFile = parcerFile. ReadFile(fileName);
    std::vector <UmtsRdmAnswer> requeriedGroupAnswer;

    map <uint32_t, std::vector<UmtsRdmAnswer>> answersOrdered;
 //прочитали файл, сортируетм соответственно каждому ключу
    for (auto a : resultsFromFile) {
      answersOrdered[(a.key>>22)].push_back(a);
    }
//пробегаемся по мапе, записываем координаты поста для каждого ответа для конкретной БС
    while (answersOrdered.begin()!=answersOrdered.end()) {

      parcerFile.saveCoordinates( answersOrdered.begin()->second,  answersOrdered.begin()->first);
      parcerFile.savePeakPos(answersOrdered.begin()->second,  answersOrdered.begin()->first);
      answersOrdered.erase(answersOrdered.begin()->first);
    }
    std::cerr << " end of processing "<< std::endl;
    return 0;
}
