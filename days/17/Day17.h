#pragma once

#include "SolutionDay.h"

struct DT  // loc, dir, howLong
{
  Point p;
  Point dir;
  int   t;

  // bool operator==(const DT & other) { return p == other.p && dir == other.dir && t == other.t; }

  bool operator<(const DT & other) const
  {
    if (p < other.p)
      return true;
    else if (p == other.p)
    {
      if (dir < other.dir)
        return true;
      else if (dir == other.dir)
      {
        if (t < other.t)
          return true;
      }
    }
    return false;
  }
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

  LL walk(MT pd, DynamicMap<int> & m, vector<MT> crt)
  {
    int  ret = 0;
    auto p   = pd.first;
    auto d   = pd.second;

    ret += m[p];
    crt.push_back(pd);

    if (p == Point{ m.max_x, m.max_y })
    {
      return ret;
    }

    auto canGo = [&](MT where)
    {
      if (!m.isSetAt(where.first))
        return false;

      auto f = find_if(begin(crt), end(crt),
                       [&](auto it)
                       {
                         return it.first == where.first;
                       });
      if (f != end(crt))
        return false;

      int  t  = 0;
      auto it = crt.size();
      // MT   startPD = crt.back();
      while (true)
      {
        if (it == 0)
          break;
        it--;
        int dist = crt.size() - it;
        if (where.second != crt[it].second)
          break;
        if (dist > 2)
          return false;
      }
      return true;
    };

    auto neigh = vector<pair<Point, Point>>{ make_pair(p + d, d) };
    if (d == Point::OriginRight() || d == Point::OriginLeft())
    {
      auto d1 = Point::OriginUp();
      auto d2 = Point::OriginDown();
      neigh.push_back(make_pair(p + d1, d1));
      neigh.push_back(make_pair(p + d2, d2));
    };
    if (d == Point::OriginDown() || d == Point::OriginUp())
    {
      auto d1 = Point::OriginLeft();
      auto d2 = Point::OriginRight();
      neigh.push_back(make_pair(p + d1, d1));
      neigh.push_back(make_pair(p + d2, d2));
    };

    LL minWalk = numeric_limits<int>::max();
    for (auto n : neigh)
    {
      if (!canGo(n))
        continue;

      LL w = walk(n, m, crt);
      if (w < minWalk)
        minWalk = w;
    }
    if (minWalk != numeric_limits<int>::max())
      ret += minWalk;

    return ret;
  }
  LL DoWork1()
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

    Point start;
    Point finish{ m.max_x, m.max_y };

    vector<MT> v;
    // ret = walk(make_pair(Point{}, Point::OriginRight()), m, v);

    Graph<DT> g(mData.size() * mData.size() * 4 * 3 * 3);
    m.for_each(
      [&](Point p, int c)
      {
        cout << p.ToString() << " ";
        auto neigh = p.GetDirectNeighbours();

        for (int t = 0; t < 3; ++t)
        {
          for (auto n : neigh)
          {
            auto dir = n - p;

            DT crt;
            crt.p   = p;
            crt.dir = dir;
            crt.t   = t;

            if (t < 3)
            {
              DT nx;
              nx.p   = n;
              nx.dir = dir;
              nx.t   = t + 1;

              if (m.isSetAt(nx.p))
                g.AddEdge(crt, nx, m[nx.p]);
            }

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

      for (int i = 0; i < 3; ++i)
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
          cout << "A: " << distMap[d] << endl;
          if (distMap[d] < ret)
            ret = distMap[d];
        }
      }
    }

    cout << "T: " << ret << endl;

    return ret;
  }

  LL DoWork2()
  {
    LL ret = 172;
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
