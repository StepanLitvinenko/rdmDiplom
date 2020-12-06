#ifndef PTypes_H
#define PTypes_H

#include <cstdint>
#include <vector>


namespace RdmTypes {

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

}
#endif // PTypes_H
