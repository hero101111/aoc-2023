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

  bool evalP(string p, vector<LL> extra)
  {
    int crtFound = 0;

    if (findQ(p) >= 0)
      return false;

    p += "E";
    for (auto c : p)
    {
      if (c == '#')
      {
        crtFound++;
      }
      if (c == '.' || c == 'E')
      {
        if (crtFound > 0 && extra.empty())
          return false;

        if (crtFound > 0 && extra.front() != crtFound)
          return false;

        if (crtFound > 0 && !extra.empty())
          extra.erase(begin(extra));
        
        crtFound = 0;
      }
    }
    if (extra.size() > 0)
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
      auto [pattern, extra] = split(d, ' ');
      auto extraV = stoll(tok(extra, ','));
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


        if (evalP(crtTop, extraV))
        {
         // cout << "YO   " << crtTop << endl;
          t++;
        }

        crtS.pop();

        auto firstQ = findQ(crtTop);
        if (firstQ >= 0)
        {
          string newPattern1 = crtTop;
          string newPattern2 = crtTop;
          /*  if (crtTop.second < totalQ)
              newPattern[pm[crtTop.second]] = '#';*/

          auto it = crtTop.find('?');
          if (it != string::npos)
          {
            newPattern1[firstQ] = '#';
            newPattern2[firstQ] = '.';
            crtS.push(newPattern1);
            crtS.push(newPattern2);
          }
        }

        if (crtS.empty())
          break;
      }
    }
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
   // assert(Part1() != "");
    //assert(Part2() != "");
    return true;
  }
};
