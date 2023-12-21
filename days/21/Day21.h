#pragma once

#include "SolutionDay.h"

class Day21 : public ISolutionDay
{
private:
  vector<string> mData;

public:
  Day21() {}

  ~Day21() override {}

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());
  }

  string GetDay() override { return "21"; }

  LL DoWork(bool partTwo)
  {
    LL ret = 0;

    DynamicMap<char> m;
    m.fromlines(mData);

    unordered_set<Point> pts;
    m.for_each(
      [&](Point p, char c)
      {
        if (c == 'S')
        {
          pts.insert(p);
        }
      });

    LL f0, f1, f2;

    for (int i = 0; i < 327; ++i)
    {
      unordered_set<Point> newPts;

      for (auto p : pts)
      {
        auto neigh = p.GetDirectNeighbours();
        for (auto n : neigh)
        {
          if ((m.hasAtINF(n, '.') || m.hasAtINF(n, 'S')))
          {
            newPts.insert(n);
          }
        }
      }

      cout << i << "  :  " << newPts.size() << endl;
      pts = newPts;

      if (!partTwo && i == 63)
        return pts.size();

      if (i == 64)
        f0 = pts.size();
      if (i == 195)
        f1 = pts.size();
      if (i == 326)
        f2 = pts.size();
    }

    LL a, b, c;
    c = f0;
    a = (f2 - 2 * f1 + f0) / 2;
    b = f1 - f0 - a;

    LL x = 202300;
    ret  = a * x * x + b * x + c;

    cout << "X : " << ret << endl;
    return ret;
  }

  string Part1() override
  {
    ReadData();

    return std::to_string(DoWork(true));
  }

  string Part2() override
  {
    ReadData();

    return std::to_string(DoWork(false));
  }

  bool Test() override
  {
    mCurrentInput = "test";
    // assert(Part1() != "");
    //   assert(Part2() != "");
    return true;
  }
};
