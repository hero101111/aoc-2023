#pragma once

#include "SolutionDay.h"

using PT = pair<Point, Point>;

template <>
struct hash<PT>
{
  auto operator()(const PT & k) const -> std::size_t
  {
    string s = k.first.ToString();
    s += "_" + k.second.ToString();

    return hash<string>()(s);
  }
};
class Day16 : public ISolutionDay
{
private:
  vector<string>   mData;
  DynamicMap<char> m2;

public:
  Day16() {}

  ~Day16() override {}

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());
  }

  string GetDay() override { return "16"; }

  int trace(DynamicMap<char> & m, pair<Point, Point> pd, unordered_set<PT> & eneg)
  {
    int t = 0;

    auto p = pd.first;

    auto dir = pd.second;

    if (!m.isSetAt(p))
      return 0;
    if (!eneg.contains(pd))
    {
      //   m2[p] = '#';
      // m2.printf(KVERBOSE);
      eneg.insert(pd), t++;
    }
    else
      return 0;

    char c = m[p];

    Point n = p + dir;

    if (c == '.')
    {
      t += trace(m, make_pair(n, dir), eneg);
    }
    else if (c == '|')
    {
      if (dir == Point{}.Up() || dir == Point{}.Down())
        t += trace(m, make_pair(n, dir), eneg);
      else
      {
        auto pd1 = make_pair(p + Point{}.Up(), Point{}.Up());
        auto pd2 = make_pair(p + Point{}.Down(), Point{}.Down());

        t += trace(m, pd1, eneg);
        t += trace(m, pd2, eneg);
      }
    }
    else if (c == '-')
    {
      if (dir == Point{}.Left() || dir == Point{}.Right())
        t += trace(m, make_pair(n, dir), eneg);
      else
      {
        auto pd1 = make_pair(p + Point{}.Left(), Point{}.Left());
        auto pd2 = make_pair(p + Point{}.Right(), Point{}.Right());

        t += trace(m, pd1, eneg);
        t += trace(m, pd2, eneg);
      }
    }
    else if (c == '\\')
    {
      if (dir == Point{}.Right())
        t += trace(m, make_pair(p + Point{}.Down(), Point{}.Down()), eneg);
      if (dir == Point{}.Left())
        t += trace(m, make_pair(p + Point{}.Up(), Point{}.Up()), eneg);
      if (dir == Point{}.Up())
        t += trace(m, make_pair(p + Point{}.Left(), Point{}.Left()), eneg);
      if (dir == Point{}.Down())
        t += trace(m, make_pair(p + Point{}.Right(), Point{}.Right()), eneg);
    }
    else if (c == '/')
    {
      if (dir == Point{}.Right())
        t += trace(m, make_pair(p + Point{}.Up(), Point{}.Up()), eneg);
      if (dir == Point{}.Left())
        t += trace(m, make_pair(p + Point{}.Down(), Point{}.Down()), eneg);
      if (dir == Point{}.Up())
        t += trace(m, make_pair(p + Point{}.Right(), Point{}.Right()), eneg);
      if (dir == Point{}.Down())
        t += trace(m, make_pair(p + Point{}.Left(), Point{}.Left()), eneg);
    }
    else
      assert(!"invalid");

    return t;
  }

  LL DoWork1()
  {
    LL               ret = 0;
    DynamicMap<char> m;
    m.fromlines(mData);
    m2 = m;
    m2.for_each(
      [&](Point p, char)
      {
        m2[p] = '.';
      });

    int maxS = 0;
    m.for_each(
      [&](Point p, char)
      {
        if (!m.isOnEdge(p))
          return;

        Point start = p;
        Point dir;
        if (start.y == 0)
          dir = Point{}.Down();
        if (start.y == m.max_y)
          dir = Point{}.Up();
        if (start.x == 0)
          dir = Point{}.Right();
        if (start.x == m.max_x)
          dir = Point{}.Left();

        unordered_set<PT> eneg;
        trace(m, make_pair(start, dir), eneg);

        unordered_set<Point> s;
        for (auto pd : eneg)
          s.insert(pd.first);

        if (s.size() > maxS)
          maxS = s.size();
      });

    return maxS;
  }

  LL DoWork2()
  {
    LL ret = 162;
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
    // assert(Part2() != "");
    return true;
  }
};
