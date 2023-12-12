#pragma once

#include "SolutionDay.h"

class Day12 : public ISolutionDay
{
private:

  vector<string> mData;

public:

  Day12(){ }

  ~Day12() override { }


  int findQ(string p)
  {
    for (int i = 0; i < p.size(); ++i)
      if (p[i] == '?') return i;
    return -1;
  }

  bool evalP(string p, const vector<LL> & extra, bool preeval = false)
  {
    int crtFound = 0;

    //if (findQ(p) >= 0)
    //  return false;

    p += "E";
    int extraPos = 0;

    for (auto c : p)
    {
      if (c == '#')
      {
        crtFound++;
      }
      if (preeval && c == '?')
        return true;
      if (c == '.' || c == 'E')
      {
        if (crtFound > 0 && extraPos >= extra.size())
          return false;

        if (crtFound > 0 && extra[extraPos] != crtFound)
          return false;

        if (crtFound > 0 && extraPos < extra.size())
          extraPos += 1;
        
        crtFound = 0;
      }
    }
    if (extraPos < extra.size())
      return false;

    return true;
  }


  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());    
  }
  
  string GetDay() override
  {
    return "12";
  }

  LL DoWork1()
  {
    LL t = 0;

    auto b = evalP(".#.###.#.######"s, vector<LL>{1, 3, 1, 6});

  // auto b2 = evalP(".??..??...?##.", vector<LL>{ 1, 1, 3});
    auto b2 = evalP("..#...#...###.", vector<LL>{ 1, 1, 3});
    for (auto d : mData)
    {
      cout << d << endl;

      auto [pattern, extra] = split(d, ' ');

      auto extraV = stoll(tok(extra, ','));

      auto pat2 = pattern;
      auto ex2 = extraV;

      for (auto i : rangeint(1, 4))
      {
        pattern += "?";
        pattern += pat2;
        for (auto e : ex2)
          extraV.push_back(e);
      }
      map<int, int> pm;
      int totalQ = 0;
      for (int i = 0; i < pattern.size(); ++i)
        if (pattern[i] == '?')
          pm[totalQ++] = i;

      stack<string> crtS;
      crtS.push(pattern);

      while (true)
      {
        auto crtTop = crtS.top();

        crtS.pop();

        auto firstQ = findQ(crtTop);
        if (firstQ >= 0)
        {
          string newPattern1 = crtTop;
          string newPattern2 = crtTop;

          bool pev = evalP(crtTop, extraV, true);
          if (pev)
          {
            auto it = crtTop.find('?');
            if (it != string::npos)
            {
              newPattern1[firstQ] = '#';
              newPattern2[firstQ] = '.';
              crtS.push(newPattern1);
              crtS.push(newPattern2);
            }
          }
        }
        else
        {
          if (evalP(crtTop, extraV, false))
          {
            // cout << "YO   " << crtTop << endl;
            t++;
          }
        }

        if (crtS.empty())
          break;
      }
    }
    cout << "T: " << t << endl;
    return t;
  }

  LL DoWork2()
  {
    LL ret = 122;
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
    //assert(Part2() != "");
    return true;
  }
};
