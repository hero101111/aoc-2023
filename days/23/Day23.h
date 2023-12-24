#pragma once

#include "SolutionDay.h"

class Day23 : public ISolutionDay
{
private:
  vector<string> mData;

public:
  Day23() {}

  ~Day23() override {}

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());
  }

  string GetDay() override { return "23"; }

  LL DoWork1()
  {
    LL               ret = 0;
    DynamicMap<char> m;
    m.fromlines(mData);
    Graph<Point> g(m.max_x * m.max_x);
    Point        startp, endp;

    for (int x : m.range_x())
    {
      if (m[Point{ x, m.min_y }] == '.')
        startp = Point{ x, m.min_y };
      if (m[Point{ x, m.max_y }] == '.')
        endp = Point{ x, m.max_y };
    }
    m.for_each(
      [&](Point p, char c)
      {
        if (c != '.' && c != '#')
          m[p] = '.';
      });

    m.for_each(
      [&](Point p, char c)
      {
        if (c == '.')
        {
          auto neigh = p.GetDirectNeighbours();
          for (auto n : neigh)
          {
            if (m.isSetAt(n) && !m.hasAt(n, '#'))
              g.AddEdge(p, n, 1);
          }
        }
        else if (c != '#')
        {
          auto next = p.FromDirection(c);
          assert(next != p);
          g.AddEdge(p, next, 1);
        }
      });

    auto path = g.GetAllPaths(startp, endp);
    int  maxL = 0;
    for (auto p : path)
      if (p.size() > maxL)
        maxL = p.size();
    ret = maxL - 1;

    // auto path = g.GetShortestPath(startp, endp);
    // ret       = path.size();

    cout << "X: " << ret << endl;

    return ret;
  }

  LL DoWork2()
  {
    LL ret = 232;
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
    // assert(Part1() != "");
    //   assert(Part2() != "");
    return true;
  }
};
