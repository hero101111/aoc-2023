#pragma once

#include "SolutionDay.h"

class Day8 : public ISolutionDay
{
private:

  vector<string> mData;

public:

  Day8(){ }

  ~Day8() override { }

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());
  }
  
  string GetDay() override
  {
    return "8";
  }

  LL DoWork1()
  {
    LL ret = 0;
    struct dd
    {
      string left, right;
    };
    map<string, dd> nodes;

    string firstLine;
    for (auto d : mData)
    {
      if (firstLine.empty())
      {
        firstLine = d;
        continue;
      }
      if (d.empty())
        continue;
      auto [n, l, r] = RegExMatch3(d, R"((\w+) = \((\w+), (\w+)\))");
      int i = 0;
      dd d;
      d.left = l;
      d.right = r;
      nodes[n] = d;
    }

    vector<string> nodeV;
    for (auto [k, v] : nodes)
    {
      if (k.back() == 'A')
        nodeV.push_back(k);
    }

    vector<LL> calc;
    for (int t = 0; t < nodeV.size(); ++t)
    {
      LL steps = 0;
      int crtI = 0;
      while (true)
      {
        bool allOk = true;

        auto& node = nodeV[t];
        //for (auto& node : nodeV)
        {
         // cout << node << " ";
          bool goLeft = firstLine[crtI] == 'L';
          if (goLeft)
            node = nodes[node].left;
          else
            node = nodes[node].right;

          if (node.back() != 'Z')
            allOk = false;
        }
       // cout << endl;

        steps++;

        if (allOk)
        {
          calc.push_back(steps);
          break;
        }

        crtI += 1;
        if (crtI > firstLine.size() - 1)
          crtI = 0;
      }
      ret = steps;
    }

    ret = 1;
    for (int i = 0; i < calc.size(); ++i)
      ret = lcm(ret, calc[i]);

    ofstream f("kek.out");
    f << ret;
    return ret;
  }

  LL DoWork2()
  {
    LL ret = 82;
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
