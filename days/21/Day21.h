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

  LL DoWork1()
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

    auto hasMapAt = [&](DynamicMap<char> & m, Point p, char c)
    {
      Point p2 = p;
      if (p.x < 0)
        p2.x = m.max_x - p.x % m.size_x() - 1;
      if (p.x > m.max_x)
        p2.x = p.x % m.size_x();

      if (p.y < 0)
        p2.y = m.max_y - p.y % m.size_y() - 1;
      if (p.y > m.max_y)
        p2.y = p.y % m.size_y();
      return m.hasAt(p2, c);
    };

    for (int i = 0; i < 500; ++i)
    {
      unordered_set<Point> newPts;

      for (auto p : pts)
      {
        auto neigh = p.GetDirectNeighbours();
        for (auto n : neigh)
        {
          if ((hasMapAt(m, n, '.') || hasMapAt(m, n, 'S')))
          {
            newPts.insert(n);
          }
        }
      }

      if (i % 10 == 0)
        cout << i << endl;
      pts = newPts;
    }

    ret = pts.size();
    cout << "X : " << ret << endl;
    return ret;
  }

  LL DoWork2()
  {
    LL ret = 212;
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
    mCurrentInput = "test";
    assert(Part1() != "");
    //  assert(Part2() != "");
    return true;
  }
};
