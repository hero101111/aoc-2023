#pragma once

#include "SolutionDay.h"
#include "z3++.h"

#define dmap DynamicMap
#define uset unordered_set
#define umap unordered_map

class Day24 : public ISolutionDay
{
private:
  vector<string> mData;

public:
  Day24() {}

  ~Day24() override {}

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());
  }

  string GetDay() override { return "24"; }

  struct hstone
  {
    Point p;
    Point v;
    bool  operator==(const hstone & other) const { return p == other.p && v == other.v; }
  };

  LL DoWork1(bool partTwo)
  {
    LL         ret = 0;
    dmap<char> m;

    vector<hstone> stones;
    for (auto d : mData)
    {
      auto [l, r] = split(d, '@');
      hstone h;
      h.p = Point{ false, l };
      h.v = Point{ false, r };
      stones.push_back(h);
    }

    LL minV = 200000000000000;
    LL maxV = 400000000000000;

    ret = 0;

    for (int i = 0; i < stones.size() - 1; ++i)
    {
      auto h0 = stones[i];
      for (int j = i + 1; j < stones.size(); ++j)
      {
        auto h1 = stones[j];
        if (h0 != h1)
        {
          auto [x0, y0, z0] = make_tuple(h0.p.x, h0.p.y, h0.p.z);
          auto [x1, y1, z1] = make_tuple(h1.p.x, h1.p.y, h1.p.z);

          auto [v0x, v0y, v0z] = make_tuple(h0.v.x, h0.v.y, h0.v.z);
          auto [v1x, v1y, v1z] = make_tuple(h1.v.x, h1.v.y, h1.v.z);

          double delta = v0y * v1x - v0x * v1y;  // v1x * (y1 - y0) - v1y * (x1 - x0);

          double ds = v1x * (y1 - y0) - v1y * (x1 - x0);
          double dt = v0x * (y1 - y0) - v0y * (x1 - x0);
          double s  = ds / delta;
          double t  = dt / delta;

          if (s < 0 || t < 0)
            continue;

          auto [X0, Y0, Z0] =
            make_tuple(h0.p.x + h0.v.x * s, h0.p.y + h0.v.y * s, h0.p.z + h0.v.z * s);
          auto [X1, Y1, Z1] =
            make_tuple(h1.p.x + h1.v.x * t, h1.p.y + h1.v.y * t, h1.p.z + h1.v.z * t);
          // assert(X0 == X1);
          // assert(Y0 == Y1);

          if (X0 >= minV && X0 <= maxV && Y0 >= minV && Y0 <= maxV)
          {
            ret++;
          }

          //  assert(Z0 == Z1);

          //  int vv = 9;
        }
      }
    }

    if (!partTwo)
      return ret;

    //-----------------------
    //
    z3::context c;
    z3::expr    x  = c.int_const("x");
    z3::expr    y  = c.int_const("y");
    z3::expr    z  = c.int_const("z");
    z3::expr    vx = c.int_const("vx");
    z3::expr    vy = c.int_const("vy");
    z3::expr    vz = c.int_const("vz");
    z3::solver  solver(c);

    for (int i = 0; i < stones.size(); ++i)
    {
      auto   d    = stones[i];
      string strT = "t" + to_string(i);

      z3::expr t   = c.int_const(strT.c_str());
      auto     dpx = c.int_val(d.p.x);
      auto     dvx = c.int_val(d.v.x);

      auto dpy = c.int_val(d.p.y);
      auto dvy = c.int_val(d.v.y);

      auto dpz = c.int_val(d.p.z);
      auto dvz = c.int_val(d.v.z);

      solver.add(x + t * vx == dpx + dvx * t);
      solver.add(y + t * vy == dpy + dvy * t);
      solver.add(z + t * vz == dpz + dvz * t);
    }

    solver.check();
    auto solution = solver.get_model();

    ret = stoll(solution.eval(x + y + z).to_string());
    //-----------------------

    return ret;
  }

  string Part1() override
  {
    ReadData();

    return std::to_string(DoWork1(false));
  }

  string Part2() override
  {
    ReadData();

    return std::to_string(DoWork1(true));
  }

  bool Test() override
  {
    mCurrentInput = "test";
    // assert(Part1() != "");
    //  assert(Part2() != "");
    return true;
  }
};
