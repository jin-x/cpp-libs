/////////////////////////////////////////
//                                     //
//       [ TimeMeasure ] for C++       //
//            version 1.11             //
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


  // Default prefix and suffix texts for duration messages.
  #define TIMEMEASURE_DEFAULT_PREFIX_SEC_TEXT ""
  #define TIMEMEASURE_DEFAULT_SUFFIX_SEC_TEXT " sec\n"

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Class for time measurement of code execution.                                                                   //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  class TimeMeasure
  {

  public:

    /** TYPE DECLARATIONS *********************************************************************************************/

    // Clock type.
    using Clock = typename std::conditional<
        std::chrono::high_resolution_clock::is_steady,
        std::chrono::high_resolution_clock,
        std::chrono::steady_clock
      >::type;

    // Time interval type.
    using Duration = Clock::duration;

    /** CONSTRUCTOR ***************************************************************************************************/

    // Constructor. Run timer.
    explicit TimeMeasure() noexcept {
      restart();
    }

    /** REGULAR METHODS ***********************************************************************************************/

    // Get elapsed time for the moment (as 'Duration' type).
    auto get_duration() const noexcept {
      return (_running ? Clock::now() : _stop_time) - _start_time;
    }

    // Get elapsed time for the moment in seconds (as real type).
    double get_sec() const noexcept {
      return std::chrono::duration_cast<std::chrono::duration<double>>(get_duration()).count();
    }

    // Get elapsed time for the moment in milliseconds (as integer type).
    auto get_millisec() const noexcept {
      return std::chrono::duration_cast<std::chrono::milliseconds>(get_duration()).count();
    }

    // Get elapsed time for the moment in microseconds (as integer type).
    auto get_microsec() const noexcept {
      return std::chrono::duration_cast<std::chrono::microseconds>(get_duration()).count();
    }

    // Get elapsed time for the moment in nanoseconds (as integer type).
    auto get_nanosec() const noexcept {
      return std::chrono::duration_cast<std::chrono::nanoseconds>(get_duration()).count();
    }

    // Restart timer.
    void restart() noexcept {
      _start_time = Clock::now();
      _running = true;
    }

    // Stop timer and get elapsed time (as 'Duration' type). Set new stop time point if timer is already stopped.
    auto stop() noexcept {
      _stop_time = Clock::now();
      _running = false;
      return get_duration();
    }

    // Resume timer if it was stopped (else do nothing).
    void resume() noexcept {
      if (!_running) {
        _start_time += Clock::now() - _stop_time;
        _running = true;
      }
    }

    // Get timer running status.
    bool is_running() const noexcept {
      return _running;
    }

    // Send elapsed time for the moment (in seconds) to stream (console by default).
    auto show_sec(const std::string& prefix_text = TIMEMEASURE_DEFAULT_PREFIX_SEC_TEXT,
                  const std::string& suffix_text = TIMEMEASURE_DEFAULT_SUFFIX_SEC_TEXT,
                  std::ostream& stream = std::cout) const {
      const auto duration = get_duration();
      show_sec(duration, prefix_text, suffix_text, stream);
      return duration;
    }

    // Stop timer and send elapsed time (in seconds) to stream (console by default).
    auto stop_show_sec(const std::string& prefix_text = TIMEMEASURE_DEFAULT_PREFIX_SEC_TEXT,
                       const std::string& suffix_text = TIMEMEASURE_DEFAULT_SUFFIX_SEC_TEXT,
                       std::ostream& stream = std::cout) {
      stop();
      return show_sec(prefix_text, suffix_text, stream);
    }

    /** STATIC METHODS ************************************************************************************************/

    // Send specified duration (converted to seconds) to stream (console by default).
    static void show_sec(const Duration duration,
                         const std::string& prefix_text = TIMEMEASURE_DEFAULT_PREFIX_SEC_TEXT,
                         const std::string& suffix_text = TIMEMEASURE_DEFAULT_SUFFIX_SEC_TEXT,
                         std::ostream& stream = std::cout) {
      stream << prefix_text << std::chrono::duration_cast<std::chrono::duration<double>>(duration).count() << suffix_text;
    }

    // Call specified function and return elapsed time (as 'Duration' type).
    template<class FuncT>
    static auto call(const FuncT& func) {
      const TimeMeasure tm;
      func();
      return tm.get_duration();
    }

    // Call specified function, and then send elapsed time (in seconds) to stream (console by default) and return this time (as 'Duration' type).
    template<class FuncT>
    static auto call_show_sec(const FuncT& func,
                              const std::string& prefix_text = TIMEMEASURE_DEFAULT_PREFIX_SEC_TEXT,
                              const std::string& suffix_text = TIMEMEASURE_DEFAULT_SUFFIX_SEC_TEXT,
                              std::ostream& stream = std::cout) {
      TimeMeasure tm;
      func();
      return tm.show_sec(prefix_text, suffix_text, stream);
    }

    // Send 'pre_text' string to stream (console by default), flush stream (if specified), then call specified function, then send elapsed time (in seconds) to stream and return this time (as 'Duration' type).
    template<class FuncT>
    static auto call_show_sec(const std::string& pre_text,
                              const FuncT& func,
                              const std::string& prefix_text = TIMEMEASURE_DEFAULT_PREFIX_SEC_TEXT,
                              const std::string& suffix_text = TIMEMEASURE_DEFAULT_SUFFIX_SEC_TEXT,
                              std::ostream& stream = std::cout, const bool flush_before_call = false) {
      stream << pre_text;
      if (flush_before_call) { stream.flush(); }
      TimeMeasure tm;
      func();
      return tm.show_sec(prefix_text, suffix_text, stream);
    }

    /** OPERATOR OVERLOADS ********************************************************************************************/

    // Get elapsed time for the moment (using object as 'Duration' type).
    operator Duration() const noexcept {
      return get_duration();
    }

    // Get elapsed time for the moment in seconds (using object as real type).
    operator double() const noexcept {
      return get_sec();
    }

    // Get timer running status (using object as boolean type).
    explicit operator bool() const noexcept {
      return _running;
    }

    // Get timer stopped status (using '!object').
    bool operator!() const noexcept {
      return !_running;
    }

  private:

    /** PRIVATE MEMBERS ***********************************************************************************************/

    bool _running;
    Clock::time_point _start_time, _stop_time;

  }; // class TimeMeasure


/**********************************************************************************************************************/

  // Content for internal use only !!!
  namespace internal {

    // Intermediate base class to initialize protected attributes before 'restart()' call in TimeMeasureScoped constructors.
    class TimeMeasureScoped_base
    {
    public:
      // Constructor. Define variable of 'Duration' type.
      explicit TimeMeasureScoped_base(TimeMeasure::Duration* const result) noexcept :
        _result(result), _stream(nullptr) { }

      // Constructor. Define variable of 'Duration' type and message strings.
      explicit TimeMeasureScoped_base(TimeMeasure::Duration* const result,
                                      const std::string& prefix_text,
                                      const std::string& suffix_text,
                                      std::ostream* const stream) :
        _result(result), _prefix_text(prefix_text), _suffix_text(suffix_text), _stream(stream) { }

    protected:
      TimeMeasure::Duration* const _result;
      const std::string _prefix_text;
      const std::string _suffix_text;
      std::ostream* const _stream;
    }; // class TimeMeasureScoped_base

  } // namespace internal


  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Class for time measurement of code execution inside a scope.                                                    //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  class TimeMeasureScoped : private internal::TimeMeasureScoped_base,
                            public TimeMeasure
  {

  public:
    // Constructor. Define variable of 'Duration' type to get elapsed time on leaving the current scope (in destructor), and then run timer.
    explicit TimeMeasureScoped(Duration& result) noexcept :
      TimeMeasureScoped_base(&result) { }

    // Constructor. Define message strings to send to stream (console by default) on leaving the current scope (in destructor), and then run timer.
    explicit TimeMeasureScoped(const std::string& prefix_text = TIMEMEASURE_DEFAULT_PREFIX_SEC_TEXT,
                               const std::string& suffix_text = TIMEMEASURE_DEFAULT_SUFFIX_SEC_TEXT,
                               std::ostream& stream = std::cout) :
      TimeMeasureScoped_base(nullptr, prefix_text, suffix_text, &stream) { }

    // Constructor. Define variable of 'Duration' type to get elapsed time and message strings to send to stream (console by default) on leaving the current scope (in destructor), and then run timer.
    explicit TimeMeasureScoped(Duration& result,
                               const std::string& prefix_text,
                               const std::string& suffix_text = TIMEMEASURE_DEFAULT_SUFFIX_SEC_TEXT,
                               std::ostream& stream = std::cout) :
      TimeMeasureScoped_base(&result, prefix_text, suffix_text, &stream) { }

    // Destructor. Write elapsed time to variable (if it was defined in constructor) and send elapsed time (in seconds) to stream (if messages were defined in constructor).
    ~TimeMeasureScoped() {
      stop();
      if (_result) { *_result = get_duration(); }
      if (_stream) { show_sec(_prefix_text, _suffix_text, *_stream); }
    }

  }; // class TimeMeasureScoped


} // namespace the


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif // !defined(TIMEMEASURE_H__INCLUDE__7X)
