#ifndef PTypes_H
#define PTypes_H

#include <cstdint>
#include <vector>
#include <memory>

//Местоположение и время
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
  // Одиночное измерение
    struct ChannelMeasure {
     ~ChannelMeasure() {
     free(convolution);
     }
     float shift; // Аппроксимация позиции начала SB внутри локального кадра в отсчётах
     unsigned int samplingFreq; // Частота дискретизации
     unsigned int sizeConvolution; // Длина ВКФ
     unsigned int* convolution;
     float convStartTimeLocal; // Время, соответствующее первому отсчёту ВКФ от начала локального кадра (нс)
     float convStartTimePps; // Время, соответствующее первому отсчёту ВКФ от момента старта (нс)
     float convPeakTimeLocal; // Время, соответствующее пику ВКФ от начала локального кадра (нс)
     float convPeakTimePps; // Время, соответствующее пику ВКФ от момента старта (нс)
    };



  // Ответ:
    struct GsmRdmAnswer {
     int idQuery; // ID задания
     unsigned int key; // Ключ для ответа, совпадающий с ответами РЭО и пеленгования для данной БС
     unsigned int freqKhz; // Частота найденной БС в кГц
     GeoData geoData; // Данные о местоположении и времени измерения
     unsigned int numMeasures; // Количество обработанных SB
//     ChannelMeasure channelMeasures[GSM_MAX_MEASURES_COUNT]; // Масссив с данными измерений
    };
    struct LteRdmAnswer {
     int idQuery; // ID задания
     uint32_t key; // Номер скремблирующей последовательности << 22 + частота
     GeoData geoData; // Координаты поста в момент измерения
     float peakPosition; // Аппроксимированная позиция пика корреляции
     uint32_t samplingFreq; // Частота дискретизации
     uint32_t rdmCorrSize = 0; // Размер ВКФ

     //    uint32_t *rdmCorr = new uint32_t [rdmCorrSize]; // Значения ВКФ
     std::vector<uint32_t> rdmCorr;
     float convStartTimeLocal; // Время, соответствующее первому отсчёту ВКФ от начала локального кадра (нс)
     float convStartTimePps; // Время, соответствующее первому отсчёту ВКФ от момента старта (нс)
     float convPeakTimeLocal; // Время, соответствующее пику ВКФ от начала локального кадра (нс)
     float convPeakTimePps; // Время, соответствующее пику ВКФ от момента старта (нс)
    };
    enum Standart{
      Gsm,
      Umts,
      Lte
    };

}
#endif // PTypes_H
