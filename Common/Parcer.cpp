
#include "Parcer.h"
#include "RdmTypes.h"
#include <cassert>
#include <fstream>
using namespace RdmTypes;
Parcer::Parcer()
{

}

std::vector<RdmTypes::UmtsRdmAnswer> Parcer::ReadFile(std::string& fileName){

  std::ifstream in(fileName, std::ifstream::binary);

  assert(in.is_open());
  in.seekg(0, in.end);
  uint32_t actualLength = in.tellg();
  in.seekg(0, in.beg);

  std::vector<UmtsRdmAnswer> answers;
  uint32_t filePointer = 0;
  while (filePointer < actualLength) {

    UmtsRdmAnswer testAnswer;

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
