#pragma once

#include "SolutionDay.h"

int lowHits{ 0 };
int highHits{ 0 };

using RT = vector<tuple<string, string, bool>>;

enum ModuleType
{
  FlipFlop = 0,
  Conjuction,
  Broadcast,
  Button,
  Output
};
struct Module
{
  string             name;
  vector<Module *>   connect;
  virtual RT         Fire(string parentModule, bool highPulse) = 0;
  virtual ModuleType GetType()                                 = 0;
};

struct MFlipFlop : public Module
{
  bool       isOn{ false };
  ModuleType GetType() override { return ModuleType::FlipFlop; };
  RT         Fire(string parentModule, bool highPulse) override
  {
    cout << parentModule << (highPulse ? " -HIGH -> " : " -LOW -> ") << this->name << endl;
    RT ret;

    if (highPulse)
      highHits++;
    else
      lowHits++;

    if (highPulse)
    {
      // do nothing
    }
    else
    {
      bool sendHigh = (isOn ? false : true);
      for (auto m : connect)
      {
        // m->Fire(this->name, sendHigh);
        ret.push_back(make_tuple(this->name, m->name, sendHigh));
      }
      isOn = isOn ? false : true;
    }
    return ret;
  }
};

struct MConjunction : public Module
{
  map<string, bool> isHigh;
  ModuleType        GetType() override { return ModuleType::Conjuction; };

  RT Fire(string parentModule, bool highPulse) override
  {
    cout << parentModule << (highPulse ? " -HIGH -> " : " -LOW -> ") << this->name << endl;
    RT ret;
    if (highPulse)
      highHits++;
    else
      lowHits++;

    isHigh[parentModule] = highPulse;
    bool allHigh         = true;
    for (auto [k, v] : isHigh)
    {
      if (!v)
      {
        allHigh = false;
        break;
      }
    }
    bool sendHigh = (allHigh ? false : true);

    for (auto m : connect)
    {
      // m->Fire(this->name, sendHigh);

      ret.push_back(make_tuple(this->name, m->name, sendHigh));
    }
    return ret;
  }
};

struct MBroadcast : public Module
{
  ModuleType GetType() override { return ModuleType::Broadcast; };
  RT         Fire(string parentModule, bool highPulse) override
  {
    RT ret;
    cout << parentModule << (highPulse ? " -HIGH -> " : " -LOW -> ") << this->name << endl;

    if (highPulse)
      highHits++;
    else
      lowHits++;

    for (auto m : connect)
    {
      // m->Fire(this->name, highPulse);

      ret.push_back(make_tuple(this->name, m->name, highPulse));
    }
    return ret;
  }
};

MBroadcast broadcastSingleton;

struct MButton : public Module
{
  ModuleType GetType() override { return ModuleType::Button; };
  RT         Fire(string, bool) override
  {
    RT ret;

    // highHits++;
    //   cout << name << " : " << endl;

    // roadcastSingleton.Fire(this->name, false);

    ret.push_back(make_tuple("button", broadcastSingleton.name, false));
    return ret;
  }
};

struct MOutput : public Module
{
  ModuleType GetType() override { return ModuleType::Output; };
  RT         Fire(string parentModule, bool highPulse) override
  {
    if (highPulse)
      highHits++;
    else
      lowHits++;

    RT ret;
    cout << parentModule << (highPulse ? " -HIGH -> " : " -LOW -> ") << this->name << endl;
    return ret;
  }
};

class Day20 : public ISolutionDay
{
private:
  vector<string> mData;

public:
  Day20() {}

  ~Day20() override {}

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());
  }

  string GetDay() override { return "20"; }

  LL DoWork1()
  {
    LL ret = 0;

    vector<MFlipFlop> flipFlops;
    flipFlops.reserve(100);
    vector<MConjunction> conjuctions;
    conjuctions.reserve(100);

    unordered_map<string, Module *> m;

    unordered_map<string, vector<string>> toConnect;

    MButton button;
    button.name = "button";

    MOutput output;

    m["button"]      = &button;
    m["broadcaster"] = &broadcastSingleton;

    broadcastSingleton.name = "broadcaster";

    for (auto d : mData)
    {
      auto [l, r] = split(d, '>');
      l.pop_back();  // drop -
      l.pop_back();  // drop space

      auto connections = tok(r, ',');

      string name = "broadcaster";
      if (l.substr(0, 2) == "br")
      {
      }
      else
      {
        char type = l[0];
        l.erase(begin(l));
        name = l;
        if (type == '%')  // flip
        {
          flipFlops.push_back(MFlipFlop{});
          flipFlops.back().name = name;
          m[name]               = &flipFlops.back();
        }
        else if (type == '&')  // conj
        {
          conjuctions.push_back(MConjunction{});
          conjuctions.back().name = name;
          m[name]                 = &conjuctions.back();
        }
      }
      toConnect[name] = connections;
    }

    for (auto [k, v] : toConnect)
    {
      assert(m.contains(trim(k)));
      Module * node = m[trim(k)];
      for (auto n : v)
      {
        n = trim(n);
        // assert(m.contains(n));

        if (m.contains(n))
        {
          Module * toConnect = m[n];
          node->connect.push_back(toConnect);

          if (toConnect->GetType() == ModuleType::Conjuction)
          {
            ((MConjunction *)toConnect)->isHigh[node->name] = false;
          }
        }
        else
        {
          output.name = n;
          m[n]        = &output;
          node->connect.push_back(m[n]);
        }
      }
    }

    ret = 0;
    for (int i = 0; i < 1000; ++i)
    {
      cout << endl;
      RT qu = button.Fire("", false);
      while (true)
      {
        RT newQu;
        for (auto q : qu)
        {
          auto [parent, mod, sendHigh] = q;

          auto newQ = m[mod]->Fire(parent, sendHigh);
          for (auto qq : newQ)
            newQu.push_back(qq);
        }
        qu = newQu;
        if (qu.empty())
          break;
      }
    }
    /*  highHits++;
      lowHits--;*/
    ret = highHits * lowHits;
    return ret;
  }

  LL DoWork2()
  {
    LL ret = 202;
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
    // assert(Part2() != "");
    return true;
  }
};
