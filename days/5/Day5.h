#pragma once

#include "SolutionDay.h"

class Day5 : public ISolutionDay
{
private:

  vector<string> mData;

public:

  Day5(){ }

  ~Day5() override { }

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());
  }
  
  string GetDay() override
  {
    return "5";
  }

  enum DataType
  {
    Seed = 0,
    Soil,
    Fertilizer,
    Water,
    Light,
    Temperature,
    Humidity,
    Location
  };

  LL DoWork1()
  {
    using LLX = long long;

    LL ret = 0;

    struct mapping
    {
      LL dest;
      LL source;
      LL length;
    };

    unordered_map<DataType, list<LLX>> world;

    bool firstLine = true;
    bool mapLine = false;
    DataType crtType = DataType::Seed;

    vector<mapping> mappings;

    for (auto d : mData)
    {
      if (firstLine)
      {
        cout << "Reading data" << endl;
        auto seedData = RegExMatchN(d, R"(\d+)");
        list<LLX> vec;
        for (int i = 0; i < seedData.size() - 1; i += 2)
        {
          cout << i << " ";
          LL p = stoll(seedData[i]);
          LL len = stoll(seedData[i + 1]);
          for (LL j = p; j < p + len; ++j)
            vec.push_back(j);
        }
        world[crtType] = vec;
        firstLine = false;
        cout << "Done reading data" << endl;
        continue;
      }
      if (d.empty())
      {
        if (crtType > DataType::Seed)
        {
          // apply transformation

          DataType prevType = (DataType)(crtType - 1);
          //list<LLX> vec;
          //vec.reserve(world[prevType].size());
          auto & prevData = world[Seed];
          for (auto & s : prevData)
          {
            auto s2 = s;
            for (auto m : mappings)
            {
              if (s >= m.source && s <= m.source + m.length)
              {
                s2 = m.dest + (s - m.source);
                break;
              }
            }
            s = s2;
          }
          //world[crtType] = vec;
          //world[prevType] = {};
        }
        mappings.clear();
        if (crtType == DataType::Location)
          break;
        crtType = (DataType)(crtType + 1);
        mapLine = true;
        continue;
      }
      if (mapLine)
      {
        cout << d << endl;
        mapLine = false;
        continue;
      }

      auto mapLine = stoll(tok(d, ' '));

      mapping mp;
      mp.dest = mapLine[0];
      mp.source = mapLine[1];
      mp.length = mapLine[2];
      mappings.push_back(mp);

     // cout << d << endl;
    }

    auto lastData = world[DataType::Seed];
    ret = *min_element(begin(lastData), end(lastData));

    return ret;
  }

  LL DoWork2()
  {
    LL ret = 52;
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
    //assert(Part1() != "");
    //assert(Part2() != "");
    return true;
  }
};
