#pragma once

#include "SolutionDay.h"

class Day18 : public ISolutionDay
{
private:
  vector<string> mData;

public:
  Day18() {}

  ~Day18() override {}

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());
  }

  string GetDay() override { return "18"; }

  LL DoWork1()
  {
    LL ret = 0;

    DynamicMap<char> m;
    Point            p;
    m[p] = '#';
    for (auto d : mData)
    {
      auto v   = tok(d, ' ');
      char dir = v[0][0];
      int  len = stoi(v[1]);
      // color

      for (int i = 0; i < len; ++i)
      {
        p    = p.FromDirection(dir);
        m[p] = '#';
      }
    }

    // Point                fillP{ 190, 66 };

    Point fillP;
    m.for_each(
      [&](Point p, char c)
      {
        if (fillP == Point{})
          fillP = p;
      });

    fillP = fillP + Point{ 1, 1 };

    //  m[fillP] = 'A';

    unordered_set<Point> toFlood;
    toFlood.insert(fillP);

    // m.printf(KVERBOSE);

    while (true)
    {
      unordered_set<Point> shouldFlood;
      for (auto p : toFlood)
      {
        for (auto cand : p.GetDirectNeighbours())
        {
          if (!m.isSetAt(cand))
          {
            m[cand] = '#';
            shouldFlood.insert(cand);
          }
        }
      }
      toFlood = shouldFlood;

      //  m.printf(KVERBOSE);
      if (toFlood.empty())
        break;
    }

    //   m.printf(KVERBOSE);
    return m.count_if(
      [](Point p, char c)
      {
        return c == '#';
      });
  }

  LL DoWork2()
  {
    LL ret = 0;

    vector<Point> perimeterPoints;
    LL            perimeterLength = 0;

    DynamicMap<char> m;
    Point            p;
    m[p] = '#';
    for (auto d : mData)
    {
      auto   v   = tok(d, ' ');
      string hex = v[2];
      char   dir;
      hex.pop_back();
      hex.erase(begin(hex));
      switch (hex.back())
      {
      case '0':
        dir = 'R';
        break;
      case '1':
        dir = 'D';
        break;
      case '2':
        dir = 'L';
        break;
      case '3':
        dir = 'U';
        break;
      default:
        assert(!"invalid");
      }
      hex.pop_back();
      hex.erase(begin(hex));
      LL len = stoll(hex, nullptr, 16);

      p = p + Point{}.FromDirection(dir) * len;

      perimeterLength += len;
      perimeterPoints.push_back(p);
    }

    LL area = 0;
    for (int i = 0; i < perimeterPoints.size() - 1; ++i)
    {
      area += (perimeterPoints[i].y + perimeterPoints[i + 1].y) *
              (perimeterPoints[i].x - perimeterPoints[i + 1].x);
    }
    ret = area / 2 + perimeterLength / 2 + 1;
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
    // assert(Part2() != "");
    return true;
  }
};
