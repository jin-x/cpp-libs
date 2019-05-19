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


#if !defined(TIMEMEASURE_H__INCLUDE__7X)
#define TIMEMEASURE_H__INCLUDE__7X


#include <iostream>
#include <chrono>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


namespace the {


  // Time interval type.
  using Duration = std::chrono::high_resolution_clock::duration;

  // Default prefix and suffix text for duration messages.
  const char* DEFAULT_PREFIX_SEC_TEXT = "";
  const char* DEFAULT_SUFFIX_SEC_TEXT = " sec\n";

  // Class for time measurement of code execution.
  class TimeMeasure
  {

  public:
    // Constructor. Run timer.
    explicit TimeMeasure() {
      restart();
    }

    // Get elapsed time for the moment (as 'Duration' type).
    auto get_duration() const {
      return (_running ? std::chrono::high_resolution_clock::now() : _stop_time) - _start_time;
    }

    // Get elapsed time for the moment (using object as 'Duration' type).
    operator Duration() const {
      return get_duration();
    }

    // Get elapsed time for the moment in seconds (as real type).
    double get_sec() const {
      return std::chrono::duration_cast<std::chrono::duration<double>>(get_duration()).count();
    }

    // Get elapsed time for the moment in seconds (using object as real type).
    operator double() const {
      return get_sec();
    }

    // Get elapsed time for the moment in milliseconds (as integer type).
    auto get_millisec() const {
      return std::chrono::duration_cast<std::chrono::milliseconds>(get_duration()).count();
    }

    // Get elapsed time for the moment in microseconds (as integer type).
    auto get_microsec() const {
      return std::chrono::duration_cast<std::chrono::microseconds>(get_duration()).count();
    }

    // Get elapsed time for the moment in nanoseconds (as integer type).
    auto get_nanosec() const {
      return std::chrono::duration_cast<std::chrono::nanoseconds>(get_duration()).count();
    }

    // Restart timer.
    void restart() {
      _running = true;
      _start_time = std::chrono::high_resolution_clock::now();
    }

    // Stop timer and get elapsed time (as 'Duration' type). Set new stop time point if timer is already stopped.
    auto stop() {
      _stop_time = std::chrono::high_resolution_clock::now();
      _running = false;
      return get_duration();
    }

    // Resume timer if it was stopped (else do nothing).
    void resume() {
      if (!_running) {
        _running = true;
        _start_time += std::chrono::high_resolution_clock::now() - _stop_time;
      }
    }

    // Get timer running status.
    bool is_running() {
      return _running;
    }

    // Get timer running status (using object as boolean type).
    explicit operator bool() {
      return _running;
    }

    // Get timer stopped status (using '!object').
    operator!() {
      return !_running;
    }

    // Send specified duration (converted to seconds) to stream (console by default).
    static void show_sec(const Duration duration, const std::string prefix_text = DEFAULT_PREFIX_SEC_TEXT, const std::string suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) {
      stream << prefix_text << std::chrono::duration_cast<std::chrono::duration<double>>(duration).count() << suffix_text;
    }

    // Send elapsed time for the moment (in seconds) to stream (console by default).
    auto show_sec(const std::string prefix_text = DEFAULT_PREFIX_SEC_TEXT, const std::string suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) {
      auto duration = get_duration();
      show_sec(duration, prefix_text, suffix_text, stream);
      return duration;
    }

    // Stop timer and send elapsed time (in seconds) to stream (console by default).
    auto stop_show_sec(const std::string prefix_text = DEFAULT_PREFIX_SEC_TEXT, const std::string suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) {
      stop();
      return show_sec(prefix_text, suffix_text, stream);
    }

    // Call specified function and return elapsed time (as 'Duration' type).
    template<class FuncT>
    static auto call(const FuncT& func) {
      TimeMeasure tm;
      func();
      return tm.get_duration();
    }

    // Call specified function, and then send elapsed time (in seconds) to stream (console by default) and return this time (as 'Duration' type).
    template<class FuncT>
    static auto call_show_sec(const FuncT& func, const std::string prefix_text = DEFAULT_PREFIX_SEC_TEXT, const std::string suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) {
      TimeMeasure tm;
      func();
      return tm.show_sec(prefix_text, suffix_text, stream);
    }

    // Send 'pre_text' string to stream (console by default), flush stream (if specified), then call specified function, then send elapsed time (in seconds) to stream and return this time (as 'Duration' type).
    template<class FuncT>
    static auto call_show_sec(const std::string pre_text, const FuncT& func, const std::string prefix_text = DEFAULT_PREFIX_SEC_TEXT, const std::string suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout, const bool flush_before_call = false) {
      stream << pre_text;
      if (flush_before_call) { stream.flush(); }
      TimeMeasure tm;
      func();
      return tm.show_sec(prefix_text, suffix_text, stream);
    }

  private:
    bool _running;
    std::chrono::high_resolution_clock::time_point _start_time, _stop_time;

  }; // class TimeMeasure


/**********************************************************************************************************************/


  // Class for time measurement of code execution inside a scope.
  class TimeMeasureScoped : public TimeMeasure
  {

  public:
    // Constructor. Run timer and define variable of 'Duration' type to get elapsed time after leaving the current scope (in destructor).
    explicit TimeMeasureScoped(Duration& result) : _result(&result), _show_message(false) {
      restart();
    }

    // Constructor. Run timer and define messages to send to stream (console by default) after leaving the current scope (in destructor).
    explicit TimeMeasureScoped(const std::string prefix_text = DEFAULT_PREFIX_SEC_TEXT, const std::string suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) :
      _result(nullptr), _show_message(true), _prefix_text(prefix_text), _suffix_text(suffix_text), _stream(&stream) {
      restart();
    }

    // Constructor. Run timer, define variable of 'Duration' type to get elapsed time and define messages to send to stream (console by default) after leaving the current scope (in destructor).
    explicit TimeMeasureScoped(Duration& result, const std::string prefix_text, const std::string suffix_text = DEFAULT_SUFFIX_SEC_TEXT, std::ostream& stream = std::cout) :
      _result(&result), _show_message(true), _prefix_text(prefix_text), _suffix_text(suffix_text), _stream(&stream) {
      restart();
    }

    // Destructor. Write elapsed time to variable (if it was defined in constructor) and send elapsed time (in seconds) to stream (if messages were defined in constructor).
    ~TimeMeasureScoped() {
      stop();
      if (_result) { *_result = get_duration(); }
      if (_show_message) { show_sec(_prefix_text, _suffix_text, *_stream); }
    }

  private:
    Duration *_result;
    const bool _show_message;
    const std::string _prefix_text;
    const std::string _suffix_text;
    std::ostream* _stream;

  }; // class TimeMeasureScoped


}; // namespace the


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif // !defined(TIMEMEASURE_H__INCLUDE__7X)
