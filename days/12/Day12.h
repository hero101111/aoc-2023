#pragma once

#include "SolutionDay.h"

using MT = pair<string, vector<LL>>;

template <>
struct hash<MT>
{
  auto operator()(const MT & k) const -> std::size_t
  {
    string s = k.first;
    for (auto ss : k.second)
      s += "_" + to_string(ss);

    return hash<string>()(s);
  }
};

class Day12 : public ISolutionDay
{
private:
  vector<string> mData;

public:
  Day12() {}

  ~Day12() override {}

  int findQ(string p)
  {
    for (int i = 0; i < p.size(); ++i)
      if (p[i] == '?')
        return i;
    return -1;
  }

  bool evalP(string p, const vector<LL> & extra, bool * aShortcuit)
  {
    int crtFound = 0;
    if (aShortcuit)
      *aShortcuit = false;

    // if (findQ(p) >= 0)
    //   return false;

    p += "E";
    int extraPos = 0;

    for (auto c : p)
    {
      if (c == '?' && aShortcuit != nullptr)
        return false;

      if (c == '#')
      {
        crtFound++;
      }
      if (c == '.' || c == 'E')
      {
        if (crtFound > 0 && extraPos >= extra.size())
        {
          if (aShortcuit)
            *aShortcuit = true;
          return false;
        }

        if (crtFound > 0 && extra[extraPos] != crtFound)
        {
          if (c != 'E')
            if (aShortcuit)
              *aShortcuit = true;
          return false;
        }

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

  string GetDay() override { return "12"; }

  unordered_map<MT, LL> cache;

  bool isGood(string pp)
  {
    // assert(pp.size() >= 3);
    if (pp.size() < 3)
      return false;

    if (pp[0] != '.' && pp[0] != '?')
      return false;
    if (pp.back() != '.' && pp.back() != '?')
      return false;
    for (auto i = 1; i < pp.size() - 1; ++i)
      if (pp[i] != '#' && pp[i] != '?')
        return false;
    return true;
  }

  LL solve(string p, vector<LL> extra)
  {
    if (extra.empty())
      return 0;
    if (cache.contains(make_pair(p, extra)))
      return cache[make_pair(p, extra)];

    auto f = extra.front();
    extra.erase(begin(extra));

    vector<int> cand;

    if (p.size() < f + 1)
      return 0;

    for (LL i = 0; i < p.size() - f - 1; ++i)
    {
      bool done = false;
      for (LL j = 0; j < i; ++j)
        if (p[j] == '#')
        {
          done = true;
          break;
        }
      if (done)
        break;

      string candP = p.substr(i, f + 2);
      if (isGood(candP))
      {
        cand.push_back(i);

        int __ = 0;
      }
    }

    LL ret = 0;
    for (auto i : cand)
    {
      string candP = p.substr(i, f + 2);

      string restP = p.substr(i + f + 1);
      auto   p2    = p.substr(0, i) + candP + restP.substr(1);
      assert(p2 == p);

      LL toAdd = 0;
      if (extra.empty())
      {
        if (!restP.contains('#'))
          ret += 1;
      }
      else
      {
        ret += solve(restP, extra);
      }
    }

    if (p.size() == f && ret > 0)
    {
      int ___ = 1;
    }
    extra.push_back(f);
    cache[make_pair(p, extra)] = ret;
    return ret;
  }

  LL DoWork1()
  {
    LL t = 0;

    // auto b = evalP(".#.###.#.######"s, vector<LL>{ 1, 3, 1, 6 });

    // auto b2 = evalP(".??..??...?##.", vector<LL>{ 1, 1, 3});
    // auto b2 = evalP("..#...#...###.", vector<LL>{ 1, 1, 3 });
    for (auto d : mData)
    {
      // d = "?###???????? 3,2,1"s;  // 10
      //  d = "????.######..#####. 1,6,5"s;  // 4
      cout << d << endl;
      cache.clear();

      auto [pattern, extra] = split(d, ' ');

      auto extraV = stoll(tok(extra, ','));

      auto pat2 = pattern;
      auto ex2  = extraV;

      for (auto i : rangeint(1, 4))
      {
        pattern += "?";
        pattern += pat2;
        for (auto e : ex2)
          extraV.push_back(e);
      }
      pattern = '.' + pattern;  // padding begin
      pattern += '.';           // padding end
      t += solve(pattern, extraV);
      int __ = 0;
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
    //  assert(Part2() != "");
    return true;
  }
};
