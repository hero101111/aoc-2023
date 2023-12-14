#pragma once

#include "SolutionDay.h"

class Day14 : public ISolutionDay
{
private:
  vector<string> mData;

public:
  Day14() {}

  ~Day14() override {}

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());
  }

  string GetDay() override { return "14"; }

  void spin(DynamicMap<char> & m, Point direction)
  {
    while (true)
    {
      bool work = false;
      for (int y : m.range_y())
      {
        for (int x : m.range_x())
        {
          Point p{ x, y };
          char  c = m[p];
          if (c != 'O')
            continue;
          while (true)
          {
            auto newP = p + direction;

            if (m.isSetAt(newP) && !m.hasAt(newP, 'O') && !m.hasAt(newP, '#'))
            {
              char n = m[newP];

              m[p]    = '.';
              m[newP] = 'O';
              work    = true;

              int i = 0;
            }
            else
              break;
          }
        }
      }
      if (!work)
        break;
    }
  }

  LL DoWork(bool partTwo)
  {
    LL               ret = 0;
    DynamicMap<char> m;
    m.fromlines(mData);

    /*

    Part two manual solution based on output for my input, cycle detection by hand:
    Loop detection code is easy to implement...

    step: value
    541 : 83488
    542 : 83482
    543 : 83477
    544 : 83473
    545 : 83484
    546 : 83491
    547 : 83507
    548 : 83516
    549 : 83516
    550 : 83502
    551 : 83489

    552 : 83488

    startIndex 541 cycle of 11 ... 83488 through 83489

    541 + 11*k = 83488


    541 + 11*k = 1000000000

    11k = 1000000000 - 541 = 999999459
    k = 90.909.041,72


    90.909.041 * 11 = 999999451


    => answer = 999999459 - 999999451 = 8 offset => 83516

    */
    for (int i = 0; i < (partTwo ? 1000000000 : 1); ++i)
    {
      spin(m, Point{}.Up());
      spin(m, Point{}.Left());
      spin(m, Point{}.Down());
      spin(m, Point{}.Right());

      // m.printf(KVERBOSE);

      LL load = 0;
      m.for_each(
        [&](Point p, char c)
        {
          if (c == 'O')
          {
            load += m.max_y - p.y + 1;
          }
        });
      cout << i << " : " << load << endl;
      ret = load;
    }

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
    // assert(Part1() != "");
    // assert(Part2() != "");
    return true;
  }
};
