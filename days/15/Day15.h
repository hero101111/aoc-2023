#pragma once

#include "SolutionDay.h"

class Day15 : public ISolutionDay
{
private:
  vector<string> mData;

public:
  Day15() {}

  ~Day15() override {}

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());
  }

  string GetDay() override { return "15"; }

  int hash(string t)
  {
    int s = 0;
    for (char c : t)
    {
      int asc = c;
      s += asc;
      s *= 17;
      s = s % 256;
    }
    return s;
  }

  struct lens
  {
    string label;
    int    focal;
  };

  LL DoWork1()
  {
    LL ret = 0;

    auto d = mData[0];
    // d       = "HASH";
    auto tk = tok(d, ',');
    for (auto t : tk)
    {
      ret += hash(t);
    }

    return ret;
  }

  LL DoWork2()
  {
    LL ret = 0;

    map<int, vector<lens>> m;
    for (auto d : mData)
    {
      // d       = "HASH";
      auto tk = tok(d, ',');
      for (auto t : tk)
      {
        string t1, t2, minus;

        if (t.contains('-'))
          minus = t.substr(0, t.size() - 1);
        else
        {
          auto p = split(t, '=');
          t1     = p.first;
          t2     = p.second;
        }

        if (!t1.empty())
        {
          int box = hash(t1);

          auto & b  = m[box];
          auto   it = find_if(begin(b), end(b),
                              [t1](lens & bx)
                              {
                              return bx.label == t1;
                            });
          if (it != end(b))
            it->focal = stoi(t2);
          else
            b.push_back(lens{ t1, stoi(t2) });
        }
        else if (!minus.empty())
        {
          int    box = hash(minus);
          auto & b   = m[box];
          auto   it  = find_if(begin(b), end(b),
                               [minus](lens & bx)
                               {
                              return bx.label == minus;
                            });
          if (it != end(b))
            b.erase(it);
        }
        else
          assert(!"invalid");
      }
      break;
    }

    for (auto b : m)
    {
      for (int li = 0; li < b.second.size(); ++li)
      {
        int    bn    = b.first + 1;
        int    sn    = li + 1;
        int    fp    = b.second[li].focal;
        string label = b.second[li].label;
        int    r     = bn * sn * fp;
        ret += r;
      }
    }
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
    mCurrentInput = "input";
    aoc_assert(Part1(), "517965"s);
    aoc_assert(Part2(), "267372"s);
    return true;
  }
};
