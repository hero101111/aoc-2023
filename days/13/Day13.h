#pragma once

#include "SolutionDay.h"

class Day13 : public ISolutionDay
{
private:
  vector<string> mData;

public:
  Day13() {}

  ~Day13() override {}

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());
  }

  string GetDay() override { return "13"; }

  LL checkSym(DynamicMap<char> & m, bool isVertical, int ignoreVal = -1)
  {
    for (int x = (isVertical ? m.max_x : m.max_y); x >= 0; --x)
    {
      if (ignoreVal == x)
        continue;
      int count = 0;
      while (true)
      {
        auto c1 = (isVertical ? m.GetColumn(x - count) : m.GetLine(x - count));
        auto c2 = (isVertical ? m.GetColumn(x + 1 + count) : m.GetLine(x + 1 + count));

        if (c1 != c2)
        {
          count = 0;
          break;
        }
        count++;
        auto lim = (isVertical ? m.max_x : m.max_y);
        if (x - count < 0 || x + 1 + count > lim)
          break;
      }
      if (count > 0)
        return x + 1;
    }
    return 0;
  }
  LL DoWork(bool partTwo)
  {
    LL                       ret = 0;
    vector<string>           lines;
    vector<DynamicMap<char>> maps;
    DynamicMap<char>         m;
    for (auto d : mData)
    {
      if (d.empty())
      {
        m = DynamicMap<char>();
        m.fromlines(lines);
        maps.push_back(m);
        lines.clear();
      }
      else
        lines.push_back(d);
    }

    LL TT = 0;
    LL TH = 0;
    for (auto map : maps)
    {
      auto lv = checkSym(map, true);
      auto lh = checkSym(map, false);

      if (partTwo)
      {
        bool found = false;
        auto m2    = map;
        for (auto x = 0; x <= map.max_x; ++x)
        {
          for (auto y = 0; y <= map.max_y; ++y)
          {
            char c       = map[{ x, y }];
            char n       = (c == '.' ? '#' : '.');
            m2[{ x, y }] = n;

            auto lv2 = checkSym(m2, true, lv - 1);
            auto lh2 = checkSym(m2, false, lh - 1);

            if (y == 1 && x == 4)
            {
              int i = 0;
              m2.printf(KVERBOSE);
            }

            if ((lv2 > 0) && (lv2 != lv))
            {
              found = true;
              lv    = lv2;
              lh    = 0;
              break;
            }
            if ((lh2 > 0) && (lh2 != lh))
            {
              found = true;
              lv    = 0;
              lh    = lh2;
              break;
            }
            m2[{ x, y }] = c;
          }
          if (found)
            break;
        }
        assert(found);
      }

      TT += lv;
      TH += lh;
      assert(lv + lh > 0);
      assert(lv * lh == 0);
    }
    ret       = TT + TH * 100;
    auto last = mData.back();

    cout << "T: " << ret << endl;
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
    mCurrentInput = "test";
    aoc_assert(Part1(), "405"s);
    aoc_assert(Part2(), "400"s);
    mCurrentInput = "input";
    aoc_assert(Part1(), "33975"s);
    aoc_assert(Part2(), "29083"s);
    return true;
  }
};
