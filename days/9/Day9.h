#pragma once

#include "SolutionDay.h"

class Day9 : public ISolutionDay
{
private:

  vector<string> mData;

public:

  Day9(){ }

  ~Day9() override { }

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());
  }
  
  string GetDay() override
  {
    return "9";
  }

  LL DoWork(bool part2)
  {
    LL ret = 0;
    for (auto d : mData)
    {
      auto v = stoll(tok(d, ' '));
      map<int, vector<LL>> diffs;
      auto crtV = v;
      int crtStep = 0;
      diffs[crtStep++] = crtV;
      while (true)
      {
        vector<LL> diff;
        for (int i = 1; i < crtV.size(); ++i)
          diff.push_back(crtV[i] - crtV[i - 1]);
        diffs[crtStep++] = diff;

        crtV = diff;
        bool shouldBreak = true;
        for (auto k : crtV)
          if (k != 0)
            shouldBreak = false;
        if (shouldBreak)
          break;
      }
      crtStep -= 1;
      LL nextValue = 0;
      while (crtStep > 0)
      {
        if (part2)
          nextValue = diffs[crtStep - 1].front() - nextValue;
        else 
          nextValue = nextValue + diffs[crtStep - 1].back();

        crtStep -= 1;
      }
      ret += nextValue;
    }
    return ret;
  }
  
  string Part1() override
  {
    ReadData();

    return std::to_string(DoWork(false));
  }

  string Part2() override
  {
    ReadData();

    return std::to_string(DoWork(true));
  }

  bool Test() override
  {
    mCurrentInput = "input";
    aoc_assert(Part1(), "1743490457"s);
    aoc_assert(Part2(), "1053"s);
    return true;
  }
};
