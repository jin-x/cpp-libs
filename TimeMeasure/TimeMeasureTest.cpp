/////////////////////////////////////////
//                                     //
//    [ TimeMeasure Test ] for C++     //
//            version 1.10             //
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
#include "TimeMeasure.h"

using namespace the;  // TimeMeasure.h
using namespace std::literals::chrono_literals;
using std::cout;

int main()
{
  cout << "One second delay time measurement test!\n";
  {
    cout << "Testing TimeMeasure struct...";
    TimeMeasure tm;
    std::this_thread::sleep_for(1s);
    tm.stop_show_sec(" ");
  }

  cout << "Testing TimeMeasure::call_show_sec (1)...";
  TimeMeasure::call_show_sec([]() { std::this_thread::sleep_for(1s); }, " ");

  TimeMeasure::call_show_sec("Testing TimeMeasure::call_show_sec (2)...", []() { std::this_thread::sleep_for(1s); }, " ");

  cout << "Testing TimeMeasure::call + TimeMeasure::show_sec...";
  Duration result = TimeMeasure::call([]() { std::this_thread::sleep_for(1s); });
  TimeMeasure::show_sec(result, " ");

  {
    cout << "Testing TimeMeasureScoped struct (auto display)...";
    TimeMeasureScoped tm(" ");
    std::this_thread::sleep_for(1s);
  }

  {
    cout << "Testing TimeMeasureScoped struct (auto assign) + TimeMeasure::show_sec...";
    TimeMeasureScoped tm(result);
    std::this_thread::sleep_for(1s);
  }
  TimeMeasure::show_sec(result, " ");

  return 0;
}
