#ifndef PARCER_H
#define PARCER_H

#include <cstdint>
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include "RdmTypes.h"
class Parcer
{
public:

    Parcer();
    std::vector<RdmTypes::UmtsRdmAnswer> ReadFile(std::string& fileName);

    template <typename T>
    void saveCoordinates(const std::vector <T>& bsData, uint32_t group){
      std::vector <int32_t> postCoordForReqGroup;
      for(auto &answer: bsData){
        postCoordForReqGroup.push_back(answer.geoData.latitude);
        postCoordForReqGroup.push_back(answer.geoData.longitude);
      }
      std::string fileName = "/home/stepan/RdmDiplom/ParcedData/coordinatesPostForBsWithSn" + std::to_string(group) + ".xy";
      std::ofstream file(fileName, std::ofstream::binary);
      file.write(reinterpret_cast<const char*>(postCoordForReqGroup.data()), postCoordForReqGroup.size()*sizeof (int32_t));
      file.close();
    }

    template <typename T>
    void savePeakPos(const std::vector <T>& bsData, uint32_t group){
      using namespace RdmTypes;
      std::vector <float> postCoordForReqGroup;
      for(auto &answer: bsData){
        postCoordForReqGroup.push_back(answer.peakPosition);
      }
      std::string fileName = "/home/stepan/RdmDiplom/ParcedData/peakPosition" + std::to_string(group) + ".pos";
      std::ofstream file(fileName, std::ofstream::binary);
      file.write(reinterpret_cast<const char*>(postCoordForReqGroup.data()), postCoordForReqGroup.size()*sizeof (float));
      file.close();
    }
};

#endif // PARCER_H
