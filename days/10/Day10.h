#pragma once

#include "SolutionDay.h"

class Day10 : public ISolutionDay
{
private:

  vector<string> mData;

  DynamicMap<char> m;
  Point startP;
  vector<Point> dirV = { Point{}.Right(), Point{}.Down(), Point().Left(), Point().Up() };
  vector<Point> loopPts;

public:

  Day10() { }

  ~Day10() override { }

  void ReadData()
  {
    m = DynamicMap<char>();
    m.fromfile(GetInputPath());

    auto fir = m.first_if([](Point, char c) { return c == 'S'; });
    assert(fir);
    startP = fir->first;

    loopPts = { startP };

    int crtDir;
    for (int i : rangeint(0, 3))
    {
      Point nextP = loopPts.back() + dirV[i];
      if (m.isSetAt(nextP))
      {
        char c = m[nextP];
        if (i == 0 && (c == '-' || c == 'J' || c == '7')) // right
        {
          crtDir = 0;
          if (c != '-') crtDir = c == 'J' ? 3 : 1;
          loopPts.push_back(nextP);
          break;
        }
        if (i == 1 && (c == '|' || c == 'L' || c == 'J')) //down
        {
          crtDir = 1;
          if (c != '-') crtDir = c == 'L' ? 0 : 2;
          loopPts.push_back(nextP);
          break;
        }
        if (i == 2 && (c == '-' || c == 'F' || c == 'L')) //left
        {
          crtDir = 2;
          if (c != '-') crtDir = c == '7' ? 1 : 3;
          loopPts.push_back(nextP);
          break;
        }
        if (i == 3 && (c == '|' || c == 'F' || c == '7')) //up
        {
          crtDir = 3;
          if (c != '-') crtDir = c == 'F' ? 0 : 2;
          loopPts.push_back(nextP);
        }
      }
    }
    while (loopPts.back() != startP)
    {
      Point nextP = loopPts.back() + dirV[crtDir];
      char c = m[nextP];

      // cout << c << " ";
      loopPts.push_back(nextP);

      if (crtDir == 0) // right
      {
        if (c != '-')
          crtDir = c == '7' ? 1 : 3;
      }
      else if (crtDir == 1) //down
      {
        if (c != '|')
          crtDir = c == 'J' ? 2 : 0;
      }
      else if (crtDir == 2) // left
      {
        if (c != '-')
          crtDir = c == 'F' ? 1 : 3;
      }
      else if (crtDir == 3) // up
      {
        if (c != '|')
          crtDir = c == '7' ? 2 : 0;
      }
    }
  }

  string GetDay() override
  {
    return "10";
  }

  LL DoWork1()
  {
    LL ret = 0;
    ret = loopPts.size() / 2;
    return ret;
  }

  LL DoWork2()
  {
    DynamicMap<char> m2;
    for (auto i : rangeint(0, m.max_x * 2))
      for (auto j : rangeint(0, m.max_y * 2))
        m2[{i, j}] = '.';

    for (int i : rangeint(1, loopPts.size() - 1))
    {
      m2[loopPts[i - 1] + loopPts[i]] = 'Z';
      m2[loopPts[i] + loopPts[i]] = 'Z';
    }

    //m2.printf(KVERBOSE);

    unordered_set<Point> toFlood;
    for (int i : m2.range_y())
    {
      if (m2[{0, i}] == '.')
        toFlood.insert(Point{ 0, i });
      if (m2[{m2.max_x, i}] == '.')
        toFlood.insert(Point{ m2.max_x, i });
    }

    for (int i : m2.range_x())
    {
      if (m2[{i, 0}] == '.')
        toFlood.insert(Point{ i, 0 });
      if (m2[{i, m2.max_y}] == '.')
        toFlood.insert(Point{ i, m2.max_y });
    }

    while (true)
    {
      unordered_set<Point> shouldFlood;
      for (auto p : toFlood)
      {
        for (auto d : dirV)
        {
          Point cand = p + d;
          if (m2.hasAt(cand, '.'))
          {
            m2[cand] = 'Z';
            shouldFlood.insert(cand);
          }
        }
      }
      toFlood = shouldFlood;

      if (toFlood.empty())
        break;
    }

    LL ret = 0;
    for (int x : rangeint(0, m2.max_x, 2))
    {
      for (int y : rangeint(0, m2.max_y, 2))
        ret += m2[{x, y}] == '.';
    }

    return ret;
  }

  string Part1() override
  {
    ReadData();

    return to_string(DoWork1());
  }

  string Part2() override
  {
    ReadData();

    return to_string(DoWork2());
  }

  bool Test() override
  {
    mCurrentInput = "test";
   // assert(Part1() != "");
   // assert(Part2() != "");
    return true;
  }
};
