# One-channel-controller-IR-BGA-rework-station
a simple controller for controlling the channel of the upper or the channel of the lower radiator in homemade IR BGA rework station
Этот проект является упрощённой версией полноценного контроллера - 
https://github.com/krepostnoy/brain-of-infrared-rework-station
В создании и тестировании этого контроллера принимали участие те же люди, что и в проекте 
"brain of infrared rework station". 
Идея создания этого контроллера появилась по двум причинам: а) у многих ремонтников уже имелся в наличии автономный термостол для подогрева PCB board; 
б) было желание использовать самые недорогие комплектующие.
В процессе эксплуатации основной версии контроллера, возникла ещё более упрощенная версия, отличающаяся от основной только применением готовых, покупных силовых
элементов. Эти изменения позволили сократить время сборки контроллера, практически до 20 минут! Обе версии - стадартная и упрощенная - представлены в этом репозитории. 
Эта версия самая компактная, специально оптимизируется под использование дешёвых комплектующих: 
плата arduino на основе микроконтроллеров AtMega168p / 328p, (5v / 16MHz) с доступными контактами A6 (к нему подключается клавиатура) и A7; 
дисплей LCD1602a без i2c;
один модуль MAX6675;
самодельная (или покупная) аналоговая (резистивная) клавиатура на 5 кнопок;
самодельная (или покупная) 3х- или 4х-канальная плата реле на 12в (не обязательно);
одно твердотельное реле типа SSR-40DA - SSR-15DA, или его самодельный аналог; 
2 вентилятора на 12в (не обязательно);
самодельная схема контроля перехода синусоиды сетевого напряжения через 0 вольт (ZCC = Zero Crossing Control) желательно, но не обязательно.

Более подробное описание находится в файле informer.h.
