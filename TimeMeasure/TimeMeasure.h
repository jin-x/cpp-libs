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

������� ���������

v1.10 (18.05.2019):
  [!] �� ���������� ��������� � ������������ ��� the.
  [+] �������� ����� (���������) TimeMeasureScoped ��� ������ ������� ���������� ���� ������ ������� ���������, �.�.
      ����������� ��������� ���������� ��� ����� ������� � �����������.
  [*] ����� show_time ������������ � show_sec, � stop_and_show - � stop_show_sec.
  [+] ��������� ����������� ������ TimeMeasure::show_sec, TimeMeasure::call � TimeMeasure::call_show_sec (2 ��������).
  [+] ��������� ���������� ������ Duration ��� ���� std::chrono::high_resolution_clock::duration.
  [*] ������������ ��� ������� TimeMeasure::stop, TimeMeasure::show_sec � TimeMeasure::stop_show_sec ������ � double
      �� auto (���������� Duration).
  [+] ��������� ��������� DEFAULT_PREFIX_SEC_TEXT � DEFAULT_SUFFIX_SEC_TEXT.
  [*] �������� ����� class �������� �� struct.

v1.00 (11.05.2019):
  [!] ������ ������ (�������� ������ ����� TimeMeasure).

***********************************************************************************************************************/


#if !defined(TIMEMEASURE_H__INCLUDE)
#define TIMEMEASURE_H__INCLUDE


#include <iostream>
#include <chrono>
#include <functional>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


namespace the {


  // ��� ��� �������� ��������� �������.
  using Duration = std::chrono::high_resolution_clock::duration;

  // ����� �������� � �������� ��� ������ ������� �� ���������.
  const char* DEFAULT_PREFIX_SEC_TEXT = "";
  const char* DEFAULT_SUFFIX_SEC_TEXT = " sec\n";

  // ����� ��� ������ ������� ���������� ����.
  struct TimeMeasure
  {

    // ��������� ������.
    TimeMeasure() {
      restart();
    }

    // �������� ��������� �� ������� ������ ����� (Duration).
    auto get_duration() {
      return (_running ? std::chrono::high_resolution_clock::now() : _stop_time) - _start_time;
    }

    // �������� ��������� �� ������� ������ ����� � ��������.
    double get_sec() {
      return std::chrono::duration_cast<std::chrono::duration<double>>(get_duration()).count();
    }

    // �������� ��������� �� ������� ������ ����� � �������������.
    auto get_millisec() {
      return std::chrono::duration_cast<std::chrono::milliseconds>(get_duration()).count();
    }

    // �������� ��������� �� ������� ������ ����� � �������������.
    auto get_microsec() {
      return std::chrono::duration_cast<std::chrono::microseconds>(get_duration()).count();
    }

    // �������� ��������� �� ������� ������ ����� � ������������.
    auto get_nanosec() {
      return std::chrono::duration_cast<std::chrono::nanoseconds>(get_duration()).count();
    }

    // ������������� ������ ������.
    void restart() {
      _running = true;
      _start_time = std::chrono::high_resolution_clock::now();
    }

    // ���������� ������ (���� ��� ��� ����������, ���������� ����� ����� ���������) � �������� ��������� ����� (Duration).
    auto stop() {
      _stop_time = std::chrono::high_resolution_clock::now();
      _running = false;
      return get_duration();
    }

    // ���������� (���� ��� ����������).
    void resume() {
      if (!_running) {
        _running = true;
        _start_time += std::chrono::high_resolution_clock::now() - _stop_time;
      }
    }

    // ������� � stream (�� ��������� � �������) �������� ������� Duration � ��������.
    static void show_sec(const Duration duration, const char* prefix_text = DEFAULT_PREFIX_SEC_TEXT, const char* suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) {
      stream << prefix_text << std::chrono::duration_cast<std::chrono::duration<double>>(duration).count() << suffix_text;
    }

    // ������� � stream (�� ��������� � �������) ��������� �� ������� ������ ����� � �������� (double).
    auto show_sec(const char* prefix_text = DEFAULT_PREFIX_SEC_TEXT, const char* suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) {
      auto duration = get_duration();
      show_sec(duration, prefix_text, suffix_text, stream);
      return duration;
    }

    // ���������� ������ � ������� � stream (�� ��������� � �������) ��������� ����� � �������� (double).
    auto stop_show_sec(const char* prefix_text = DEFAULT_PREFIX_SEC_TEXT, const char* suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) {
      stop();
      return show_sec(prefix_text, suffix_text, stream);
    }

    // ��������� ������� func � ������� ����� � ���������� (Duration).
    static auto call(const std::function<void()> func) {
      TimeMeasure tm;
      func();
      return tm.get_duration();
    }

    // ��������� ������� func, ������� � ������� ����� � ���������� (Duration).
    static auto call_show_sec(const std::function<void()> func, const char* prefix_text = DEFAULT_PREFIX_SEC_TEXT, const char* suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) {
      TimeMeasure tm;
      func();
      return tm.show_sec(prefix_text, suffix_text, stream);
    }

    // ������� ������ pre_text, ��������� ������� func, ������� � ������� ����� � ���������� (Duration).
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


  // ����� ��� ������ ������� ���������� ���� ������ ������� ���������.
  struct TimeMeasureScoped : TimeMeasure
  {
    // ��������� ������ � ������ ���������� ��� ��������� ���������� ����� ������ �� ������� ��������� (� �����������).
    TimeMeasureScoped(Duration& result) : _result(&result), _show_message(false) { }

    // ��������� ������ � ������ ���������� ��� ��������� ���������� � ��������� ��� ������ ����� ������ �� ������� ��������� (� �����������).
    TimeMeasureScoped(Duration& result, const char* prefix_text, const char* suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) :
      _result(&result), _show_message(true), _prefix_text(prefix_text), _suffix_text(suffix_text), _stream(&stream) { }

    // ��������� ������ � ������ ��������� ��� ������ ����� ������ �� ������� ��������� (� �����������).
    TimeMeasureScoped(const char* prefix_text = DEFAULT_PREFIX_SEC_TEXT, const char* suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) :
      _result(nullptr), _show_message(true), _prefix_text(prefix_text), _suffix_text(suffix_text), _stream(&stream) { }

    // ����������.
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
