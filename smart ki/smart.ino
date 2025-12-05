#include <Servo.h>
int Cabinet =0; //расстояние до объекта, измеренное ульразвуковым датчиком
int PIRS = 0; // значение с пир-датчика
int Gass = 0; // значение с газового датчика
int Temps = 0; //температура, расчитанная по аналоговому сигнал
long readUltrasonicDistance(int triggerPin, int echoPin) // функция, которая измеряет расстояние с помощью ультразвукового датчика
{
  pinMode(triggerPin, OUTPUT); //устанавливаем пин триггер как выход
  digitalWrite(triggerPin, LOW); // сначала сбрасываем датчик через параметр LOW
  delayMicroseconds(2); //задержка 2 микросекунды
  digitalWrite(triggerPin, HIGH); // посылаем импульс при помощи параметра HIGH на 10 мкс
  delayMicroseconds(10);//задержка 10 микросекунды
  digitalWrite(triggerPin, LOW); //снова сбрасываем
  pinMode(echoPin, INPUT);//пин эхо как вход
  return pulseIn(echoPin, HIGH);//считываем длительность импульса (время, пока echo = HIGH)
}
Servo servo_5; //создаем объект servo_5 для управления микросервоприводом
void setup() {//основная функция настройки
  Serial.begin(9600); //включаем монитор последовательного интерфейса для оображения значений
  servo_5.attach(5, 500, 2500); //подключаем сервопривод к пину 5, с указанным диапазоном ШИМ
  pinMode(10, INPUT); //инициализируем подключение пироэлектрического датчика движения
  pinMode(9, OUTPUT); //выход
  pinMode(7, OUTPUT); //выход, цепь из лампочек
  pinMode(A1, INPUT);//вход, аналоговый датчик температуры
  pinMode(6, OUTPUT);//выход, двигатель постоянного тока (винтелятор)
  pinMode(A0, INPUT);//вход, газовый датчик
  pinMode(4, OUTPUT);//выход, пироэлектрический датчик
}
void loop() { //основный цикл loop
  Cabinet = 0.01723 * readUltrasonicDistance(3,3); //измеряем расстояние до объекта. 0.01723 - коэффициент для перевода времени в сантиметры(учитывая скорость звука и формулу расстояния)
  Serial.println(Cabinet);//выводим результат расстояния до объекта в монитор последовательного интерфейса
  if (Cabinet < 15) { //если расстояние меньше 15
    servo_5.write(90); //открываем микросервопривод на 90 градусов
    delay(5000); // задержка 5 сек, после чего микросервопривод Поворачивается обратно (закрывается)
  } else {
    servo_5.write(0); //оставляем микросервопривод закрытым, если расстояние до объекта больше 15
  }
  PIRS = digitalRead(10); //считываем значение с датчика движения
  Serial.println(PIRS);//выводим значения с датчика в монитор последовательного интерфейса
  if (PIRS == HIGH) {
    digitalWrite(9, HIGH);
    digitalWrite(7, HIGH);
  } else {
    digitalWrite(9, LOW);
    digitalWrite(7, LOW);
  }
  Temps = (-40 + 0.488155 * (analogRead(A1) - 20));
  Serial.println(Temps);
  if (Temps >= 30) {
    digitalWrite(6, HIGH);
  } else {
    digitalWrite(6, LOW);
  }
  Gass = analogRead(A0);
  Serial.println(Gass);
  if (Gass >= 220) {
    digitalWrite(4, HIGH);
  } else {
    digitalWrite(4, LOW);
  }
}
  
  
