#ifdef UNIT_TEST

#include "tests.h"
#include "counter.h"

Counter* counter;

#include <unity.h>
#include <stdio.h>

void setUp(){
  counter = new Counter();
  counter->init();
}

void goAMile(){
  counter->setCount(8000);
}

void goAMinute(){
    counter->setTime(60000);
}

void goAMileAMinute(){
  goAMile();
  goAMinute();
}

void counter_is_init(){
  TEST_ASSERT_NOT_NULL(counter->pointer());
  TEST_ASSERT_NOT_NULL(counter);
  TEST_ASSERT_EQUAL(counter, counter->pointer());

  int staringCount = counter->getPulses();
  float startingDistance = counter->getDistance_miles();
  float startingSpeed = counter->getSpeed_MPH();
  int startingTime = counter->getTime();
  TEST_ASSERT_EQUAL_INT(0, staringCount);
  TEST_ASSERT_EQUAL_FLOAT(0, startingDistance);
  TEST_ASSERT_EQUAL_FLOAT(0, startingSpeed);
  TEST_ASSERT_EQUAL_INT(0, startingTime);
}

void counter_counts(){
  int count = 0;
  counter->setCount();
  count = counter->getPulses();
  TEST_ASSERT_EQUAL_INT(1, count);
  counter->setCount();
  counter->setCount();
  counter->setCount();
  count = counter->getPulses();
  TEST_ASSERT_EQUAL_INT(4, count);

  counter->setCount(4);
  count = counter->getPulses();
  TEST_ASSERT_EQUAL_INT(8, count);
}

void resetDistance_flushes_count(){
  int count;
  counter->setCount();
  count = counter->getPulses();
  counter->resetDistance();
  TEST_ASSERT_EQUAL_INT(1, count);
  count = counter->getPulses();
  TEST_ASSERT_EQUAL_INT(0, count);
}

void getDistance_returns_distance(){
  float distance;
  goAMile();

  distance = counter->getDistance_miles();
  TEST_ASSERT_EQUAL_FLOAT(1.0, distance);
  counter->resetDistance();
  counter->setCount(12000);

  distance = counter->getDistance_miles();
  TEST_ASSERT_EQUAL_FLOAT(1.5, distance);
}

void getSpeed_MPH_returns_mph_when_given_time(){
  goAMile();
  TEST_ASSERT_EQUAL_FLOAT(3600000, counter->getSpeed_MPH(1));
  goAMile();
  TEST_ASSERT_EQUAL_FLOAT(1800000, counter->getSpeed_MPH(2));
  goAMile();
  TEST_ASSERT_EQUAL_FLOAT(60, counter->getSpeed_MPH(60000));
}

void getTime_starts_with_zero(){
  TEST_ASSERT_EQUAL_INT(0,counter->getTime());
  counter->setTime(1);
  TEST_ASSERT_EQUAL_INT(1,counter->getTime());
  counter->setTime(60);
  TEST_ASSERT_EQUAL_INT(61,counter->getTime());
}

void getTime_and_getSpeed_generate_valid_speed(){
  goAMileAMinute();

  TEST_ASSERT_EQUAL_FLOAT(60, counter->getSpeed_MPH()); //60 MPH

  //100ms and 25mph 27*8000/3600000*200 = 12(repeating) pulses/200ms
  counter->setTime(200);
  counter->setCount(12);

  TEST_ASSERT_EQUAL_FLOAT(27, counter->getSpeed_MPH());
}

void resetTime_zeros_time(){
  counter->setTime(1);
  counter->resetTime();
  TEST_ASSERT_EQUAL_INT(0,counter->getTime());
}

void when_time_and_distance_is_zero_speed_is_zero(){
  TEST_ASSERT_EQUAL_FLOAT(0.0, counter->getSpeed_MPH());
}

void getSpeed_and_getDistance_reset_but_dont_break_eachother(){
  goAMileAMinute();
  TEST_ASSERT_EQUAL_FLOAT(1.0,counter->getDistance_miles());
  TEST_ASSERT_EQUAL_FLOAT(60.0, counter->getSpeed_MPH());
  TEST_ASSERT_EQUAL_FLOAT(0.0,counter->getDistance_miles());
  TEST_ASSERT_EQUAL_FLOAT(0.0, counter->getSpeed_MPH());

  goAMileAMinute();

  TEST_ASSERT_EQUAL_FLOAT(1.0,counter->getDistance_miles());
  TEST_ASSERT_EQUAL_FLOAT(0.0,counter->getDistance_miles());
  TEST_ASSERT_EQUAL_FLOAT(60.0, counter->getSpeed_MPH());
  TEST_ASSERT_EQUAL_FLOAT(0.0, counter->getSpeed_MPH());

}

int run_counter_tests(){
  UNITY_BEGIN();
  RUN_TEST(counter_is_init);
  RUN_TEST(counter_counts);
  RUN_TEST(resetDistance_flushes_count);
  RUN_TEST(getDistance_returns_distance);
  RUN_TEST(getSpeed_MPH_returns_mph_when_given_time);
  RUN_TEST(getTime_starts_with_zero);
  RUN_TEST(getTime_and_getSpeed_generate_valid_speed);
  RUN_TEST(resetTime_zeros_time);
  RUN_TEST(when_time_and_distance_is_zero_speed_is_zero);
  RUN_TEST(getSpeed_and_getDistance_reset_but_dont_break_eachother);
  return UNITY_END();
}

#endif
