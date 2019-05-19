/////////////////////////////////////////
//                                     //
//  [ TimeMeasure Examples ] for C++   //
//            version 1.11             //
//                                     //
//   http://xk7.ru/p/c/l/TimeMeasure   //
//                                     //
/////////////////////////////////////////
//                                     //
//  (c) 2019 by Jin X (jin_x@list.ru)  //
//                                     //
/////////////////////////////////////////


#include <thread>
#include <chrono>
#include <fstream>
#include "..\src\TimeMeasure.h"


int main()
{

  using namespace the;  // TimeMeasure.h
  using std::cout;

  const auto test_code = []() {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    };
  const std::string timer_status[] { "running", "stopped" };

  cout << "One second delay time measurement examples!\n";

  // Example 1
  {
    cout << "TimeMeasure class...";
    TimeMeasure tm;
    test_code();
    tm.stop_show_sec(" ");
    cout << "> Stop: timer is " << timer_status[!tm] << ". Waiting...";
    test_code();
    cout << "\n> Resume:";
    tm.resume();
    cout << " timer is " << timer_status[!tm] << ". Waiting...";
    test_code();
    tm.show_sec("\n> Total time (2 seconds): ");

  // Example 2
    cout << "> Restart...";
    tm.restart();
    test_code();
    cout << " use object as real type: " << tm << " sec\n";
  }

  // Example 3
  cout << "TimeMeasure::call_show_sec...";
  TimeMeasure::call_show_sec(test_code, " ");

  // Example 4
  TimeMeasure::call_show_sec("TimeMeasure::call_show_sec (with pre_text)...", test_code, " ");

  // Example 5
  cout << "TimeMeasure::call + TimeMeasure::show_sec...";
  TimeMeasure::Duration duration = TimeMeasure::call(test_code);
  TimeMeasure::show_sec(duration, " ");

  // Example 6
  {
    cout << "TimeMeasureScoped class (auto display)...";
    const TimeMeasureScoped tm(" ");
    test_code();
  }

  // Example 7
  {
    cout << "TimeMeasureScoped class (auto assign) + TimeMeasure::show_sec...";
    const TimeMeasureScoped tm(duration);
    test_code();
  }
  TimeMeasure::show_sec(duration, " ");

  // Example 8
  cout << "TimeMeasure::call_show_sec (to file)...";
  #define filename "TimeMeasureExamples_result.txt"
  std::ofstream file(filename);
  if (file) {
    duration = TimeMeasure::call_show_sec(test_code, "Elapsed time is ", TIMEMEASURE_DEFAULT_SUFFIX_SEC_TEXT, file);
    file.close();
    the::TimeMeasure::show_sec(duration, " ok (file \"" filename "\" is created, elapsed time: ", " sec)\n");
  } else {
    cout << " file create error!\n";
  }

  return 0;

} // main()
