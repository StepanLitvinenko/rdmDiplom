#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <list>
#include <tuple>

using namespace std;

struct GeoData {
    int latitude; // Широта
    int longitude; // Долгота
    int altitude; // Высота
    unsigned int timeUTC; // Время (UTC)
};

struct UmtsRdmAnswer {

    int idQuery = 0; // ID задания
    uint32_t key = 0; // Номер скремблирующей последовательности << 22 + частота
    GeoData geoData; // Координаты поста в момент измерения
    float peakPosition = 0; // Аппроксимированная позиция пика корреляции
    uint32_t samplingFreq = 0; // Частота дискретизации
    uint32_t rdmCorrSize = 0; // Размер ВКФ

    //    uint32_t *rdmCorr = new uint32_t [rdmCorrSize]; // Значения ВКФ
    std::vector<uint32_t> rdmCorr;

    float convStartTimeLocal = 0; // Время, соответствующее первому отсчёту ВКФ от начала локального кадра (нс)
    float convStartTimePps = 0; // Время, соответствующее первому отсчёту ВКФ от момента старта (нс)
    float convPeakTimeLocal = 0; // Время, соответствующее пику ВКФ от начала локального кадра (нс)
    float convPeakTimePps = 0;
};

std::vector<UmtsRdmAnswer> ReadFile(std::string fileName)
{

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
int main()
{
    std::string fileName = "/home/stepan/.postlink/Files/Шевелев Александр Евгеньевич/umts_rdm.dat";
    auto resultsFromFile = ReadFile(fileName);
    for (auto a : resultsFromFile) {
        //        std::cerr << " scramblingSeq: " << (a.key >> 22) << " freq: " << a.key - (a.key >> 22) << std::endl;4
        auto b = a.key >> 22;
        std::cerr << " idQuery " << a.idQuery << " freq: " << (a.key & b << 22) << " scramblingSeq: " << (a.key >> 22) << std::endl;
    }

    return 0;
}
