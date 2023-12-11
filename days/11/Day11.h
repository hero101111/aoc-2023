#pragma once

#include "SolutionDay.h"

class Day11 : public ISolutionDay
{
private:

  vector<string> mData;

public:

  Day11(){ }

  ~Day11() override { }

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());

  }
  
  string GetDay() override
  {
    return "11";
  }

  LL DoWork1(bool partTwo)
  {
    LL graphSize = 0;

    vector<int> emptyCols, emptyLines;
    for (auto it = mData.begin(); it != end(mData); ++it)
    {
      bool onlySpace = true;
      for (char c : *it)
        if (c != '.')
          onlySpace = false;
      if (onlySpace)
      {
        emptyLines.push_back(it - mData.begin());       
      }
    }

    for (int i = mData[0].size() - 1; i >= 0; --i)
    {
      bool onlySpace = true;
      for (auto d : mData)
        if (d[i] != '.')
          onlySpace = false;
      if (onlySpace)
      {
        emptyCols.push_back(i);
      }
    }

    DynamicMap<char> map;
    map.fromlines(mData);

    vector<Point> galaxies;
    map.for_each([&](Point p, char c) {
      if (c == '#')
      {
        galaxies.push_back(p);
      }
      });

    unordered_map<Point, Point> offsets;
    int N = partTwo ? 1000000 : 2;
    for (auto line : emptyLines)
    {
      for (auto & g : galaxies)
      {
        if (g.y > line)
          offsets[g].y += N - 1;
      }
    }

    for (auto col : emptyCols)
    {
      for (auto & g : galaxies)
      {
        if (g.x > col)
          offsets[g].x += N - 1;
      }
    }

    for (auto & g : galaxies)
    {
      if (offsets.contains(g))
        g = g + offsets[g];
    }
    LL ret = 0;

    //ofstream kVerbose(KVERBOSE);

    for (auto i = 0; i < galaxies.size() - 1;  ++i)
      for (auto q = i + 1; q < galaxies.size(); ++q)
      {       
        auto dd = galaxies[i].ManhattanDist(galaxies[q]);
        ret += dd;
        //kVerbose << i << " " << q << "  :  " << dd << endl;

      }
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
    //assert(Part2() != "");
    return true;
  }
};
