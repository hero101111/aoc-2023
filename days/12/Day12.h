#pragma once

#include "SolutionDay.h"

using MT = pair<LL, LL>;

template <>
struct hash<MT>
{
  auto operator()(const MT & k) const -> std::size_t
  {
    string s = to_string(k.first);
    s += "_" + to_string(k.second);

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

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());
  }

  string GetDay() override { return "12"; }

  unordered_map<MT, LL> cache;

  bool IsValidPattern(string pp)
  {
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

  LL SolveRow(string row, const vector<LL> & damaged, LL rowIndex, LL damagedIndex)
  {
    if (damagedIndex >= damaged.size())
      return 0;
    if (cache.contains(make_pair(rowIndex, damagedIndex)))
      return cache[make_pair(rowIndex, damagedIndex)];

    auto crtRowChar = damaged[damagedIndex++];

    vector<int> cand;

    if (row.size() < crtRowChar + 1)
      return 0;

    for (LL i = rowIndex; i < row.size() - crtRowChar - 1; ++i)
    {
      bool done = false;
      for (LL j = rowIndex; j < i; ++j)
        if (row[j] == '#')
        {
          done = true;
          break;
        }
      if (done)
        break;

      string candP = row.substr(i, crtRowChar + 2);
      if (IsValidPattern(candP))
        cand.push_back(i);
    }

    LL ret = 0;
    for (auto i : cand)
    {
      LL toAdd = 0;
      if (damagedIndex >= damaged.size())
      {
        bool isGood = true;
        for (int k = i + crtRowChar + 1; k < row.size(); ++k)
          if (row[k] == '#')
          {
            isGood = false;
            break;
          }

        ret += isGood ? 1 : 0;
      }
      else
      {
        ret += SolveRow(row, damaged, i + crtRowChar + 1, damagedIndex);
      }
    }
    cache[make_pair(rowIndex, damagedIndex - 1)] = ret;
    return ret;
  }

  LL DoWork(bool partTwo)
  {
    LL ret = 0;

    for (auto d : mData)
    {
      cache.clear();

      auto [pattern, extra] = split(d, ' ');

      auto extraV = stoll(tok(extra, ','));

      if (partTwo)
      {
        auto pat2 = pattern;
        auto ex2  = extraV;
        for (auto i : rangeint(1, 4))
        {
          pattern += "?";
          pattern += pat2;
          for (auto e : ex2)
            extraV.push_back(e);
        }
      }
      pattern = '.' + pattern;  // padding begin
      pattern += '.';           // padding end
      ret += SolveRow(pattern, extraV, 0, 0);
      int __ = 0;
    }
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
    aoc_assert(Part1(), "21"s);
    aoc_assert(Part2(), "525152"s);
    mCurrentInput = "input";
    aoc_assert(Part1(), "7260"s);
    aoc_assert(Part2(), "1909291258644"s);
    return true;
  }
};
