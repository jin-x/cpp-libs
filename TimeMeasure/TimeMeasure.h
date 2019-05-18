/////////////////////////////////////////
//                                     //
//       [ TimeMeasure ] for C++       //
//            version 1.10             //
//                                     //
//   http://xk7.ru/p/c/l/TimeMeasure   //
//                                     //
/////////////////////////////////////////
//                                     //
//  (c) 2019 by Jin X (jin_x@list.ru)  //
//                                     //
/////////////////////////////////////////


/***********************************************************************************************************************

ИСТОРИЯ ИЗМЕНЕНИЙ

v1.10 (18.05.2019):
  [!] Всё содержимое заключено в пространство имён the.
  [+] Добавлен класс (структура) TimeMeasureScoped для замера времени выполнения кода внутри области видимости, т.е.
      выполняющий получение результата или вывод времени в деструкторе.
  [*] Метод show_time переименован в show_sec, а stop_and_show - в stop_show_sec.
  [+] Добавлены статические методы TimeMeasure::show_sec, TimeMeasure::call и TimeMeasure::call_show_sec (2 варианта).
  [+] Добавлено объявление алиаса Duration для типа std::chrono::high_resolution_clock::duration.
  [*] Возвращаемый тип методов TimeMeasure::stop, TimeMeasure::show_sec и TimeMeasure::stop_show_sec заменён с double
      на auto (фактически Duration).
  [+] Добавлены константы DEFAULT_PREFIX_SEC_TEXT и DEFAULT_SUFFIX_SEC_TEXT.
  [*] Ключевое слово class заменено на struct.

v1.00 (11.05.2019):
  [!] Первая версия (содержит только класс TimeMeasure).

***********************************************************************************************************************/


#if !defined(TIMEMEASURE_H__INCLUDE)
#define TIMEMEASURE_H__INCLUDE


#include <iostream>
#include <chrono>
#include <functional>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


namespace the {


  // Тип для хранения интервала времени.
  using Duration = std::chrono::high_resolution_clock::duration;

  // Текст префикса и суффикса для вывода времени по умолчанию.
  const char* DEFAULT_PREFIX_SEC_TEXT = "";
  const char* DEFAULT_SUFFIX_SEC_TEXT = " sec\n";

  // Класс для замера времени выполнения кода.
  struct TimeMeasure
  {

    // Запустить таймер.
    TimeMeasure() {
      restart();
    }

    // Получить прошедшее на текущий момент время (Duration).
    auto get_duration() {
      return (_running ? std::chrono::high_resolution_clock::now() : _stop_time) - _start_time;
    }

    // Получить прошедшее на текущий момент время в секундах.
    double get_sec() {
      return std::chrono::duration_cast<std::chrono::duration<double>>(get_duration()).count();
    }

    // Получить прошедшее на текущий момент время в миллисекундах.
    auto get_millisec() {
      return std::chrono::duration_cast<std::chrono::milliseconds>(get_duration()).count();
    }

    // Получить прошедшее на текущий момент время в микросекундах.
    auto get_microsec() {
      return std::chrono::duration_cast<std::chrono::microseconds>(get_duration()).count();
    }

    // Получить прошедшее на текущий момент время в наносекундах.
    auto get_nanosec() {
      return std::chrono::duration_cast<std::chrono::nanoseconds>(get_duration()).count();
    }

    // Перезапустить таймер заново.
    void restart() {
      _running = true;
      _start_time = std::chrono::high_resolution_clock::now();
    }

    // Остановить таймер (если уже был остановлен, установить новую точку остановки) и получить прошедшее время (Duration).
    auto stop() {
      _stop_time = std::chrono::high_resolution_clock::now();
      _running = false;
      return get_duration();
    }

    // Продолжить (если был остановлен).
    void resume() {
      if (!_running) {
        _running = true;
        _start_time += std::chrono::high_resolution_clock::now() - _stop_time;
      }
    }

    // Вывести в stream (по умолчанию в консоль) интервал времени Duration в секундах.
    static void show_sec(const Duration duration, const char* prefix_text = DEFAULT_PREFIX_SEC_TEXT, const char* suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) {
      stream << prefix_text << std::chrono::duration_cast<std::chrono::duration<double>>(duration).count() << suffix_text;
    }

    // Вывести в stream (по умолчанию в консоль) прошедшее на текущий момент время в секундах (double).
    auto show_sec(const char* prefix_text = DEFAULT_PREFIX_SEC_TEXT, const char* suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) {
      auto duration = get_duration();
      show_sec(duration, prefix_text, suffix_text, stream);
      return duration;
    }

    // Остановить таймер и вывести в stream (по умолчанию в консоль) прошедшее время в секундах (double).
    auto stop_show_sec(const char* prefix_text = DEFAULT_PREFIX_SEC_TEXT, const char* suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) {
      stop();
      return show_sec(prefix_text, suffix_text, stream);
    }

    // Выполнить функцию func и вернуть время её выполнения (Duration).
    static auto call(const std::function<void()> func) {
      TimeMeasure tm;
      func();
      return tm.get_duration();
    }

    // Выполнить функцию func, вывести и вернуть время её выполнения (Duration).
    static auto call_show_sec(const std::function<void()> func, const char* prefix_text = DEFAULT_PREFIX_SEC_TEXT, const char* suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) {
      TimeMeasure tm;
      func();
      return tm.show_sec(prefix_text, suffix_text, stream);
    }

    // Вывести строку pre_text, выполнить функцию func, вывести и вернуть время её выполнения (Duration).
    static auto call_show_sec(const char* pre_text, const std::function<void()> func, const char* prefix_text = DEFAULT_PREFIX_SEC_TEXT, const char* suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) {
      stream << pre_text;
      TimeMeasure tm;
      func();
      return tm.show_sec(prefix_text, suffix_text, stream);
    }

  private:
    bool _running;
    std::chrono::high_resolution_clock::time_point _start_time, _stop_time;

  }; // class TimeMeasure


/**********************************************************************************************************************/


  // Класс для замера времени выполнения кода внутри области видимости.
  struct TimeMeasureScoped : TimeMeasure
  {
    // Запустить таймер и задать переменную для получения результата после выхода из области видимости (в деструкторе).
    TimeMeasureScoped(Duration& result) : _result(&result), _show_message(false) { }

    // Запустить таймер и задать переменную для получения результата и сообщения для вывода после выхода из области видимости (в деструкторе).
    TimeMeasureScoped(Duration& result, const char* prefix_text, const char* suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) :
      _result(&result), _show_message(true), _prefix_text(prefix_text), _suffix_text(suffix_text), _stream(&stream) { }

    // Запустить таймер и задать сообщения для вывода после выхода из области видимости (в деструкторе).
    TimeMeasureScoped(const char* prefix_text = DEFAULT_PREFIX_SEC_TEXT, const char* suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) :
      _result(nullptr), _show_message(true), _prefix_text(prefix_text), _suffix_text(suffix_text), _stream(&stream) { }

    // Деструктор.
    ~TimeMeasureScoped() {
      stop();
      if (_result) { *_result = get_duration(); }
      if (_show_message) { show_sec(_prefix_text, _suffix_text, *_stream); }
    }

  private:
    Duration *_result;
    const bool _show_message;
    const char* _prefix_text;
    const char* _suffix_text;
    std::ostream* _stream;

  }; // class TimeMeasureScoped


}; // namespace the


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif // !defined(TIMEMEASURE_H__INCLUDE)
