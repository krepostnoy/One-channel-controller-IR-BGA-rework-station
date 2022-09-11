//секция ПИД
float integra, e, p, d;             //интегральная составляющая, ошибка регулирования, П-составляющая, Д-составляющая
#define i_min 0.0                   //минимум И составляющей
#define i_max 100.0                 //максимум И составляющей
double  Input1;                     //текущая температура(измеренная max6675) ВИ
byte Output1;                       //выход ПИД регулятора ВИ

//секция алгоритма Брезенхема
int er1 = 1;                        //переменная для расчета переполнения в алгоритме Брезенхема для ВИ
int reg1;                           //промежуточная переменная для алгоритма Брезенхема для ВИ
boolean out1;                       //выход импульса управления ВИ алгоритма Брезенхема
#define SENSOR_SAMPLING_TIME 250    //частота обновления текущей температуры (1000=1 раз в секунду)

//секция профиля
struct pr {                         //основные поля профиля
  byte profileSteps;                //количество шагов профиля
  byte temperatureStep[4];          //заданные температуры для шагов профиля ВИ
  byte rampRateStep[4];             //скорость ростsetpointRampа температуры
  byte dwellTimerStep[4];           //установленное время перехода на следубщий шаг
  byte kp1;                         //пропорциональный коэффициент ПИД для ВИ
  byte ki1;                         //интегральный коэффициент ПИД для ВИ
  byte kd1;                         //дифференциальный коэффициент ПИД для ВИ
  byte pwr_PREHEAT;                 //мощность верхнего нагревателя на этапе преднагрева
  byte min_pwr_TOPStep[4];          // минимальная мощность верхнего нагревателя
  byte max_pwr_TOPStep[4];          //максимальная мощность верхнего нагревателя
};
byte SizeProfile = sizeof(pr);      //длина поля данных
pr profile;
byte currentProfile = 1;            //текущий профиль
byte editStep = 0;                  //номер шага профиля который редактируется в данный момент
byte currentStep = 1;               //текущий шаг профиля
byte Setpoint1;                     //установленная температура(уставка) ВИ

//Секция флагов
boolean TopStart = false;           //флаг включения ВИ
boolean flag = 0;                   //флаг для фиксации стартовой температуры и начала работы нагревателей
byte x = 1;                         //переменная для перехода на нужный шаг при горячей плате
//boolean alarmOn = false;           //признак предупреждения
boolean updateScreen = true;        //признак обновления экрана

//Секция переменных общего назначения
byte startTemp;                     //стартовая температура для профиля, когда НИ вышел на уставку, используется для определения шага профиля с которого начинаем
byte setpointRamp;                  //заданная температура по определённому шагу профиля ВИ, связана с заданной скоростью роста температуры
byte counter;                       //счётчик для рампы и счетчик для времени нахождения в шаге
unsigned long previousMillis;       //это для счетчиков
unsigned long nextRead;             //переменная для обновления текущей температуры и расчёта ПИД
int tc1;                            //переменная для хранения текущей температуры ВИ
int tc2 = 0;                        //для совместимости с софтом на ПК (SerialPortPlotter (и irsp.exe© от @Dmitrysh))
int Output2 = 0;                    //для совместимости с софтом на ПК (SerialPortPlotter (и irsp.exe© от @Dmitrysh))
int i = 0;
int chast, kluch, Count1;           //для вывода в порт и расчета ПИД

typedef enum REFLOW_STATE : byte {  //структура статусов событий ***** TYPE DEFINITIONS *****
  REFLOW_STATE_IDLE,                //экран в режиме простоя, выбор номера профиля
  REFLOW_STATE_MENU_STEPS,          //меню выбора числа шагов редактируемого профиля
  REFLOW_STATE_MENU_STEP_TARGET,    //меню выбора конечной t° на конкретном шаге редактируемого профиля
  REFLOW_STATE_MENU_STEP_RAMP,      //меню выбора скорости роста t° ВИ на конкретном шаге редактируемого профиля
  REFLOW_STATE_MENU_STEP_DWELL,     //меню выбора времени удержания конечной t° на конкретном шаге редактируемого профиля
  REFLOW_STATE_MENU_TOP_P,          //меню выбора коэффициента P для ВИ редактируемого профиля
  REFLOW_STATE_MENU_TOP_I,          //меню выбора коэффициента I для ВИ редактируемого профиля
  REFLOW_STATE_MENU_TOP_D,          //меню выбора коэффициента D для ВИ редактируемого профиля
  REFLOW_STATE_MENU_PREHEAT_PWR,    // меню выбора мощности ВИ на этапе преднагрева в редактируемом профиле
  REFLOW_STATE_MENU_TOP_PWR_MIN,    //меню выбора мин. мощности ВИ в шагах редактируемого профиля
  REFLOW_STATE_MENU_TOP_PWR_MAX,    //меню выбора макс. мощности ВИ в шагах редактируемого профиля
  REFLOW_STATE_PREHEAT,             // состояние преднагрева ВИ в момент запуска пайки
  REFLOW_STATE_STEP_RAMP,           //"процесс пайки", рост температуры с заданной для текущего шага ВИ скоростью
  REFLOW_STATE_STEP,
  REFLOW_STATE_STEP_DWELL,
  REFLOW_STATE_COMPLETE,
  REFLOW_STATE_PROFILE_INIT         //состояние в IDLE "ИНИЦИАЛИЗАЦИЯ ПРЕДУСТАНОВЛЕННОГО ПРОФИЛЯ"
}
reflowState_t;
typedef enum REFLOW_STATUS : byte { //структура статуса пайки, запущен или нет
  REFLOW_STATUS_OFF,
  REFLOW_STATUS_ON
}
reflowStatus_t;
reflowStatus_t reflowStatus;        //переменная статусов событий
reflowState_t reflowState;          //переменная статуса пайки
