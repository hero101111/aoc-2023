#pragma once

#include "SolutionDay.h"

class Day6 : public ISolutionDay
{
private:

  vector<string> mData;

public:

  Day6(){ }

  ~Day6() override { }

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());
  }
  
  string GetDay() override
  {
    return "6";
  }

  LL DoWork1()
  {
    LL ret = 1;
    auto tData = RegExMatchN(mData[0], R"(\d+)");
    auto dData = RegExMatchN(mData[1], R"(\d+)");


    for (int i = 0; i < tData.size(); ++i)
    {
      int ways = 0;
      LL t = stoll(tData[i]);
      LL target = stoll(dData[i]);

      for (LL j = 0; j <= t; ++j)
      {
        LL T = (t - j) * j;
        if (T > target)
          ways++;
      }
      return ways;
      ret *= ways;
    }
    return ret;
  }

  LL DoWork2()
  {
    LL ret = 62;
    return ret;
  }
  
  string Part1() override
  {
    ReadData();

    return std::to_string(DoWork1());
  }

  string Part2() override
  {
    ReadData();

    return std::to_string(DoWork2());
  }

  bool Test() override
  {
  //  mCurrentInput = "test";
    //assert(Part1() != "");
    //assert(Part2() != "");
    return true;
  }
};
