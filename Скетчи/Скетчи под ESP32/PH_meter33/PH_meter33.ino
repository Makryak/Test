#define SensorPin        A4                                                                   // pH meter Analog output to Arduino Analog Input 0
#define Offset           -0.00                                                                // Компенсация смещения
#define LED              13                                                                   // Номер вывода светодиода, который является индикатором нормальной работы скетча
#define samplingInterval 20                                                                   // Интервал в мс между измерениями
#define printInterval    800                                                                  // Интервал в мс между выводами показаний
#define ArrayLenth       40                                                                   // Количество выборок
int pHArray[ArrayLenth];                                                                      // Массив для определения среднего показания напряжения считанного с датчика
int pHArrayIndex=0;                                                                           // Индекс элемента массива pHArray значения которого требуется изменить
                                                                                              //
void setup(void){                                                                             //
    pinMode(LED,OUTPUT);                                                                      // Конфигурируем вывод светодиода как выход
    Serial.begin(115200);                                                                       // Инициируем передачу данных в монитор последовательного порта на скорости 9600 бит/с
    Serial.println("pH meter experiment!");                                                   // Выводим надпись "pH meter experiment!"
}                                                                                             //
void loop(void){                                                                              //
    static unsigned long samplingTime = millis();                                             // Определяем переменную samplingTime для хранения времени прошедшего с момента старта (переменная создаётся при первом проходе цикла loop и не теряется по его завершении)
    static unsigned long printTime    = millis();                                             // Определяем переменную printTime    для хранения времени прошедшего с момента старта (переменная создаётся при первом проходе цикла loop и не теряется по его завершении)
    static float pHValue, voltage;                                                            // Объявляем переменные для хранения значений напряжения и pH
//  Проводим измерения:                                                                       //
    if(millis() - samplingTime > samplingInterval){                                           // Выполняем код в теле оператора if через каждые samplingInterval мс
        pHArray[pHArrayIndex++] = analogRead(SensorPin);                                      // Читаем данные в очередной элемент массива pHArray
        if(pHArrayIndex==ArrayLenth) pHArrayIndex=0;                                          // Если достигли последнего элемента массива pHArray, то сбрасываем номер текущего элемента этого массива в 0
        voltage = averagearray(pHArray, ArrayLenth) * 5.0 / 1023;                             // Получаем среднее напряжение в мВ из массива напряжений pHArray
        pHValue = 3.5 * voltage + Offset;                                                     // Преобразуем мВ в pH
        samplingTime = millis();                                                              // Обновляем время для переменной samplingTime
    }                                                                                         //
//  Выводим показания в монитор:                                                              //
    if(millis() - printTime > printInterval){                                                 // Выполняем код в теле оператора if через каждые printInterval мс
        Serial.print("Voltage:");                                                             // Выводим надпись "Voltage:"
        Serial.print(voltage,2);                                                              // Выводим среднее напряжение
        Serial.print("    pH value: ");                                                       // Выводим надпись "    pH value: "
        Serial.println(pHValue,2);                                                            // Выводим значение pH
        digitalWrite(LED,digitalRead(LED)^1);                                                 // Меняем состояние светодиода
        printTime = millis();                                                                 // Обновляем время для переменной printTime
    }                                                                                         //
}                                                                                             //
                                                                                              //
//  Функция определения среднего значения напряжения                                          // Эта функция возвращает среднее арифметическое значение данных массива arr без учёта одного максимального и одного минимального значения массива.
double averagearray(int* arr, int number){                                                    //
    int i,max,min;                                                                            // Объявляем переменные для цикла и экстремумов
    double avg;                                                                               // Объявляем переменную для вывода среднего значения
    long amount=0;                                                                            // Определяем переменную для подсчёта среднего значения
    if(number<=0){ Serial.println("Error number for the array to avraging!/n");  return 0;}   // В массиве arr не может быть 0 и менее элементов
    if(number< 5){ for(i=0; i<number; i++){amount+=arr[i];} avg = amount/number; return avg;  // Если в массиве arr менее 5 элементов, то среднее значение является средним арифметическим значением
    }else{                                                                                    // Если в массиве arr более 5 элементов, то среднее значение считаем иначе ...
        if(arr[0]<arr[1]){ min = arr[0]; max=arr[1];}                                         // Определяем минимальное и максимальное число из первых двух элементов массива
        else             { min = arr[1]; max=arr[0];}                                         // Определяем минимальное и максимальное число из первых двух элементов массива
        for(i=2; i<number; i++){                                                              // Проходим по остальным элементам массива
                 if(arr[i]<min){ amount+=min; min=arr[i]; }                                   // Если значение очередного элемента меньше минимального,  то добавляем к значению amount предыдущее минимальное значение  и обновляем значение min
            else if(arr[i]>max){ amount+=max; max=arr[i]; }                                   // Если значение очередного элемента больше максимального, то добавляем к значению amount предыдущее максимальное значение и обновляем значение max
            else               { amount+=arr[i];          }                                   // Если значение очередного элемента находится в пределах между min и max, то добавляем значение этого элемента к amount
        }                                                                                     //
        avg = (double) amount/(number-2);                                                     // Получаем среднее арифметическое значение (без учета значений первых двух элементов массива arr, т.к. они не добавлялись к amount)
    }                                                                                         //
    return avg;                                                                               // Возвращаем полученное среднее значение
}                       
