#pragma once

#include "SolutionDay.h"

struct DT  // loc, dir, howLong
{
  Point p;
  Point dir;
  int   t;

  // bool operator==(const DT & other) { return p == other.p && dir == other.dir && t == other.t; }
};

bool operator==(const DT & a, const DT & b)
{
  return a.p == b.p && a.dir == b.dir && a.t == b.t;
}

template <>
struct hash<DT>
{
  auto operator()(const DT & k) const -> std::size_t
  {
    string s = k.p.ToString() + "_";
    s += "_" + k.dir.ToString();
    s += "_" + to_string(k.t);

    return hash<string>()(s);
  }
};

class Day17 : public ISolutionDay
{
private:
  vector<string> mData;

  using MT = pair<Point, Point>;

public:
  Day17() {}

  ~Day17() override {}

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());
  }

  string GetDay() override { return "17"; }

  LL DoWork(bool partTwo)
  {
    LL               ret = 0;
    DynamicMap<char> m_;
    DynamicMap<int>  m;

    m_.fromlines(mData);
    m_.for_each((
      [&](Point p, char c)
      {
        m[p] = c - '0';
      }));

    vector<MT> v;

    int MOVELIM = partTwo ? 10 : 3;
    int MINLIM  = partTwo ? 3 : 0;

    Graph<DT> g(mData.size() * mData.size() * 4 * MOVELIM * MOVELIM);
    m.for_each(
      [&](Point p, int c)
      {
        auto neigh = p.GetDirectNeighbours();

        for (int t = 0; t < MOVELIM; ++t)
        {
          for (auto n : neigh)
          {
            auto dir = n - p;

            DT crt;
            crt.p   = p;
            crt.dir = dir;
            crt.t   = t;

            if (t < MOVELIM)
            {
              DT nx;
              nx.p   = n;
              nx.dir = dir;
              nx.t   = t + 1;

              if (m.isSetAt(nx.p))
                g.AddEdge(crt, nx, m[nx.p]);
            }

            if (t >= MINLIM)
            {
              auto neighV = vector<pair<Point, Point>>{};
              if (dir == Point::OriginRight() || dir == Point::OriginLeft())
              {
                auto d1 = Point::OriginUp();
                auto d2 = Point::OriginDown();
                neighV.push_back(make_pair(p + d1, d1));
                neighV.push_back(make_pair(p + d2, d2));
              };
              if (dir == Point::OriginDown() || dir == Point::OriginUp())
              {
                auto d1 = Point::OriginLeft();
                auto d2 = Point::OriginRight();
                neighV.push_back(make_pair(p + d1, d1));
                neighV.push_back(make_pair(p + d2, d2));
              };
              for (auto ng : neighV)
              {
                DT nx;
                nx.p   = ng.first;
                nx.dir = ng.second;
                nx.t   = 0;
                if (m.isSetAt(nx.p))
                  g.AddEdge(crt, nx, m[nx.p]);
              }
            }
          }
        }
      });
    // cout << "Constructed graph";

    vector<DT> startNodes;
    vector<DT> destNodes;
    ret       = numeric_limits<LL>::max();
    auto dirs = Point{}.GetDirectNeighbours();
    for (auto dir : dirs)
    {
      DT start;
      start.p   = Point{ 0, 0 };
      start.dir = dir;
      start.t   = 0;

      Point nx = start.p + start.dir;
      if (m.isSetAt(nx))
        startNodes.push_back(start);

      for (int i = 0; i < MOVELIM; ++i)
      {
        if (dir != Point::OriginLeft() && dir != Point::OriginUp())
        {
          DT dest;
          dest.p   = Point{ m.max_x, m.max_y };
          dest.dir = dir;
          dest.t   = i;

          destNodes.push_back(dest);
        }
      }
    }

    ret = numeric_limits<LL>::max();
    for (auto s : startNodes)
    {
      for (auto d : destNodes)
      {
        auto distMap = g.GetDistances(s);

        if (distMap.contains(d))
        {
          // cout << "A: " << distMap[d] << endl;
          if (distMap[d] < ret)
            ret = distMap[d];
        }
      }
    }

    // cout << "T: " << ret << endl;

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
    aoc_assert(Part1(), "102"s);
    aoc_assert(Part2(), "94"s);
    mCurrentInput = "input";
    aoc_assert(Part1(), "1008"s);
    aoc_assert(Part2(), "1210"s);
    return true;
  }
};
