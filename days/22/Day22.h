#pragma once

#include "SolutionDay.h"

static int _crtId{ 0 };

class Day22 : public ISolutionDay
{
private:
  vector<string> mData;

public:
  Day22() {}

  ~Day22() override {}

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());
  }

  string GetDay() override { return "22"; }

  struct brick
  {
    int id;
    brick()
    {
      _crtId++;
      id = _crtId;
    }
    Point a, b;

    unordered_set<Point> inner;

    vector<brick *> restOnMe;
    vector<brick *> restsOn;

    void UpdateInner()
    {
      inner.clear();
      for (auto x : rangeint2(a.x, b.x))
        for (auto y : rangeint2(a.y, b.y))
          for (auto z : rangeint2(a.z, b.z))
            inner.insert(Point{ x, y, z });
    }

    void MoveInner(LL oldZ, LL newZ)
    {
      for (auto x : rangeint2(a.x, b.x))
        for (auto y : rangeint2(a.y, b.y))
        {
          inner.erase(Point{ x, y, oldZ });
          inner.insert(Point{ x, y, newZ });
        }
    }

    bool alive{ true };

    bool operator==(const brick & other) const { return a == other.a && b == other.b; }

    bool intersects(const brick & other) const
    {
      unordered_set<Point> myP;

      for (auto p : inner)
        if (other.inner.contains(p))
          return true;

      return false;
    }
    bool fall(vector<brick> & w, bool simulate = false)
    {
      bool hasFallen = false;
      while (a.z > 1 && b.z > 1)
      {
        auto nextB = *this;
        nextB.a.z  = a.z - 1;
        nextB.b.z  = b.z - 1;
        nextB.MoveInner(max(a.z, b.z), min(nextB.a.z, nextB.b.z));
        // nextB.UpdateInner(); // xxx

        bool shouldBreak = false;
        for (auto b : w)
        {
          this->intersects(b);
          if (b.alive && b != *this && nextB.intersects(b))
          {
            shouldBreak = true;
            break;
          }
        }
        if (shouldBreak)
          break;

        hasFallen = true;
        if (!simulate)
        {
          a.z   = nextB.a.z;
          b.z   = nextB.b.z;
          inner = nextB.inner;
        }
        else
          break;
      }
      return hasFallen;
    }
  };

  LL DoWork1()
  {
    LL ret = 0;

    vector<brick> br;

    for (auto d : mData)
    {
      auto [l, r] = split(d, '~');
      brick b;

      b.a = Point{ true, l };
      b.b = Point{ true, r };
      b.UpdateInner();
      br.push_back(b);
    }

    cout << "falling" << endl;

    while (true)
    {
      bool anyFallen = false;
      for (auto & b : br)
        anyFallen |= b.fall(br, false);

      if (!anyFallen)
        break;
    }

    cout << "done falling" << endl;

    ret = 0;

    cout << "fetching boundaries" << endl;
    for (auto & b_ : br)
    {
      auto b = b_;
      if (b.a.z > 1 && b.b.z > 1)
      {
        b.a.z -= 1;
        b.b.z -= 1;
        b.MoveInner(max(b.a.z + 1, b.b.z + 1), min(b.a.z, b.b.z));

        for (auto & b2 : br)
        {
          if (b2 != b_ && b.intersects(b2))
          {
            b2.restOnMe.push_back(&b_);
            b_.restsOn.push_back(&b2);
          }
        }
      }
    }

    // ----------
    // ret   = 0;
    // int k = 0;
    // for (auto & b : br)
    //{
    //  cout << "XXX " << k++ << endl;
    //  b.alive    = false;
    //  bool valid = true;
    //  for (auto & bToFall : br)
    //  {
    //    if (bToFall != b && bToFall.fall(br, true))
    //    {
    //      b.restOnMe.push_back(&bToFall);
    //      valid = false;
    //      // break;
    //    }
    //  }
    //  if (valid)
    //    ret += 1;
    //  b.alive = true;
    //}

    function<LL(brick &)> detect;
    detect = [&](brick & b) -> LL
    {
      b.alive               = false;
      vector<brick *> check = b.restOnMe;
      set<brick *>    brickSet;
      while (!check.empty())
      {
        auto b2 = check.front();
        check.erase(begin(check));

        int  restsOnCount = 0;
        auto restOn       = &b;
        for (auto b2R : b2->restsOn)
        {
          if (b2R->alive)
          {
            restsOnCount++;
            restOn = b2R;
          }
        }
        if (restsOnCount == 0)
        {
          // assert(restOn == &b);
          brickSet.insert(b2);
        }
      }
      LL t = brickSet.size();
      for (auto tt : brickSet)
        t += detect(*tt);

      return t;
    };

    cout << "detecting..." << endl;
    ret     = 0;
    int crt = 0;
    for (auto & b : br)
    {
      cout << crt++ << endl;
      for (auto & b2 : br)
        b2.alive = true;
      ret += detect(b);
    }
    cout << "X: " << ret << endl;
    return ret;
  }

  LL DoWork2()
  {
    LL ret = 222;
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
