#ifndef PARCER_H
#define PARCER_H

#include <cstdint>
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include <map>
#include <cstdint>
#include <iostream>
#include "RdmTypes.h"
class Parcer
{
public:

    Parcer();

    bool ParceFile(const std::string& fileName, RdmTypes::Standart);
      private:
    template <typename T>
        void saveCoordinates(const std::vector <T>& bsData, uint32_t group,const std::string& postfixProtocol){
      std::vector <int32_t> postCoordForReqGroup;
      for(auto &answer: bsData){
        postCoordForReqGroup.push_back(answer.geoData.latitude);
        postCoordForReqGroup.push_back(answer.geoData.longitude);
      }
      std::string fileName = "/home/stepan/RdmDiplom/ParcedData/coordinatesPostForBsWithSn" +postfixProtocol + std::to_string(group) + ".xy";
      std::ofstream file(fileName, std::ofstream::binary);
      file.write(reinterpret_cast<const char*>(postCoordForReqGroup.data()), postCoordForReqGroup.size()*sizeof (int32_t));
      file.close();
    }

    template <typename T>
    void savePeakPos(const std::vector <T>& bsData, uint32_t group, const std::string& postfixProtocol){
      using namespace RdmTypes;
      std::vector <float> postCoordForReqGroup;
      for(auto &answer: bsData){
        postCoordForReqGroup.push_back(answer.peakPosition);
      }
      std::string fileName = "/home/stepan/RdmDiplom/ParcedData/peakPosition" + postfixProtocol+ std::to_string(group) + ".pos";
      std::ofstream file(fileName, std::ofstream::binary);
      file.write(reinterpret_cast<const char*>(postCoordForReqGroup.data()), postCoordForReqGroup.size()*sizeof (float));
      file.close();
    }
  private:
    template <typename T>
    void Process(const std::vector<T>& resultsReading,const std::string& postfixProtocol){
      std::map <uint32_t, std::vector<T>> answersOrdered;
      //прочитали файл, сортируетм соответственно каждому ключу
      for (auto a : resultsReading) {
        answersOrdered[(a.key)].push_back(a);
      }
      //пробегаемся по мапе, записываем координаты поста для каждого ответа для конкретной БС
      while (answersOrdered.begin()!=answersOrdered.end()) {
        saveCoordinates( answersOrdered.begin()->second,  answersOrdered.begin()->first, postfixProtocol);
        savePeakPos(answersOrdered.begin()->second,  answersOrdered.begin()->first, postfixProtocol);
        answersOrdered.erase(answersOrdered.begin()->first);
      }
      std::cerr << " end of processing "<< std::endl;
    }
    private:
    template<typename T>
    std::vector<T> ReadFile(const std::string& fileName){

      std::ifstream in(fileName, std::ifstream::binary);
      std::cerr << " try to open file: "<< fileName<<std::endl;
      assert(in.is_open());
      in.seekg(0, in.end);
      uint32_t actualLength = in.tellg();
      in.seekg(0, in.beg);

      std::vector<T> answers;
      uint32_t filePointer = 0;
      while (filePointer < actualLength) {

        T testAnswer;

        in.read(reinterpret_cast<char*>(&testAnswer.idQuery), sizeof(testAnswer.idQuery));
        in.seekg(filePointer + sizeof(testAnswer.idQuery), in.beg);

        in.read(reinterpret_cast<char*>(&testAnswer.key), sizeof(testAnswer.key));
        in.seekg(filePointer + sizeof(testAnswer.key) + sizeof(testAnswer.idQuery), in.beg);

        in.read(reinterpret_cast<char*>(&testAnswer.geoData), sizeof(testAnswer.geoData));
        in.seekg(filePointer + sizeof(testAnswer.key) + sizeof(testAnswer.idQuery) + sizeof(testAnswer.geoData), in.beg);

        in.read(reinterpret_cast<char*>(&testAnswer.peakPosition), sizeof(testAnswer.peakPosition));
        in.seekg(filePointer + sizeof(testAnswer.key) + sizeof(testAnswer.idQuery) + sizeof(testAnswer.geoData) + sizeof(testAnswer.peakPosition), in.beg);

        in.read(reinterpret_cast<char*>(&testAnswer.samplingFreq), sizeof(testAnswer.samplingFreq));
        in.seekg(filePointer + sizeof(testAnswer.samplingFreq) + sizeof(testAnswer.key) + sizeof(testAnswer.idQuery) + sizeof(testAnswer.geoData) + sizeof(testAnswer.peakPosition), in.beg);

        in.read(reinterpret_cast<char*>(&testAnswer.rdmCorrSize), sizeof(testAnswer.rdmCorrSize));
        in.seekg(filePointer + sizeof(testAnswer.rdmCorrSize) + sizeof(testAnswer.samplingFreq) + sizeof(testAnswer.key) + sizeof(testAnswer.idQuery) + sizeof(testAnswer.geoData) + sizeof(testAnswer.peakPosition), in.beg);

        testAnswer.rdmCorr.resize(testAnswer.rdmCorrSize);

        in.read(reinterpret_cast<char*>(testAnswer.rdmCorr.data()), testAnswer.rdmCorrSize * sizeof(uint32_t));
        in.seekg(filePointer + testAnswer.rdmCorrSize * sizeof(uint32_t) + sizeof(testAnswer.rdmCorrSize) + sizeof(testAnswer.samplingFreq) + sizeof(testAnswer.key) + sizeof(testAnswer.idQuery) + sizeof(testAnswer.geoData) + sizeof(testAnswer.peakPosition), in.beg);

        in.read(reinterpret_cast<char*>(&testAnswer.convStartTimeLocal), sizeof(testAnswer.convStartTimeLocal));
        in.seekg(filePointer + sizeof(testAnswer.convStartTimeLocal) + testAnswer.rdmCorrSize * sizeof(uint32_t) + sizeof(testAnswer.rdmCorrSize) + sizeof(testAnswer.samplingFreq) + sizeof(testAnswer.key) + sizeof(testAnswer.idQuery) + sizeof(testAnswer.geoData) + sizeof(testAnswer.peakPosition), in.beg);

        in.read(reinterpret_cast<char*>(&testAnswer.convStartTimePps), sizeof(testAnswer.convStartTimePps));
        in.seekg(filePointer + sizeof(testAnswer.convStartTimePps) + sizeof(testAnswer.convStartTimeLocal) + testAnswer.rdmCorrSize * sizeof(uint32_t) + sizeof(testAnswer.rdmCorrSize) + sizeof(testAnswer.samplingFreq) + sizeof(testAnswer.key) + sizeof(testAnswer.idQuery) + sizeof(testAnswer.geoData) + sizeof(testAnswer.peakPosition), in.beg);

        in.read(reinterpret_cast<char*>(&testAnswer.convPeakTimeLocal), sizeof(testAnswer.convPeakTimeLocal));
        in.seekg(filePointer + sizeof(testAnswer.convPeakTimeLocal) + sizeof(testAnswer.convStartTimePps) + sizeof(testAnswer.convStartTimeLocal) + testAnswer.rdmCorrSize * sizeof(uint32_t) + sizeof(testAnswer.rdmCorrSize) + sizeof(testAnswer.samplingFreq) + sizeof(testAnswer.key) + sizeof(testAnswer.idQuery) + sizeof(testAnswer.geoData) + sizeof(testAnswer.peakPosition), in.beg);

        in.read(reinterpret_cast<char*>(&testAnswer.convPeakTimePps), sizeof(testAnswer.convPeakTimePps));
        in.seekg(filePointer + sizeof(testAnswer.convPeakTimePps) + sizeof(testAnswer.convPeakTimeLocal) + sizeof(testAnswer.convStartTimePps) + sizeof(testAnswer.convStartTimeLocal) + testAnswer.rdmCorrSize * sizeof(uint32_t) + sizeof(testAnswer.rdmCorrSize) + sizeof(testAnswer.samplingFreq) + sizeof(testAnswer.key) + sizeof(testAnswer.idQuery) + sizeof(testAnswer.geoData) + sizeof(testAnswer.peakPosition), in.beg);
        filePointer += in.tellg();
        answers.push_back(testAnswer);
      }

      in.close();
      return answers;
    }
};

#endif // PARCER_H
