#pragma once

#include "SolutionDay.h"

class Day19 : public ISolutionDay
{
private:
  vector<string> mData;

public:
  Day19() {}

  ~Day19() override {}

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());
  }

  string GetDay() override { return "19"; }

  struct part
  {
    map<string, LL> rs;
  };

  struct ruleT
  {
    string cond;
    string result;
  };

  struct rule
  {
    string        name;
    vector<ruleT> cond;
  };

  string getCondOp(ruleT cond) { return cond.cond.substr(1, 1); }
  string getCondField(ruleT cond) { return cond.cond.substr(0, 1); }
  LL     getCondValue(ruleT cond) { return stoll(cond.cond.substr(2)); }

  string checkCond(part p, vector<ruleT> conds)
  {
    for (auto cond : conds)
    {
      if (cond.cond.empty())
        return cond.result;

      string field = cond.cond.substr(0, 1);
      string op    = cond.cond.substr(1, 1);
      LL     value = stoll(cond.cond.substr(2));

      if (op == "<")
      {
        if (p.rs[field] < value)
          return cond.result;
      }
      else if (op == ">")
      {
        if (p.rs[field] > value)
          return cond.result;
      }
      else if (op.empty())
        return cond.result;
      else
        assert(!"invalid");
    }

    return "";
  }

  LL DoWork1()
  {
    LL ret = 0;

    auto d     = rffv(GetInputPath());
    auto rules = d[0];
    auto parts = d[1];
    assert(d.size() == 2);
    vector<part>      partsV;
    map<string, rule> rulesM;
    vector<rule>      rulesV;

    for (auto r : parts)
    {
      auto tok = RegExMatchN(r, R"(\w=\d+)");
      part np;
      for (auto t : tok)
      {
        auto [l, r] = split(t, '=');
        np.rs[l]    = stoll(r);
      }
      partsV.push_back(np);
    }

    for (auto r : rules)
    {
      auto   it   = r.find('{');
      string name = r.substr(0, it);

      string rest = r.substr(it + 1, r.size() - it - 2);
      rule   nr;
      nr.name = name;

      auto rV = tok(rest, ',');
      for (auto rv : rV)
      {
        auto colon = rv.find(':');
        if (colon != string::npos)
        {
          string cond = rv.substr(0, colon);
          string res  = rv.substr(colon + 1);

          ruleT t;

          t.cond   = cond;
          t.result = res;
          nr.cond.push_back(t);
        }
        else
        {
          ruleT t;
          t.result = rv;
          nr.cond.push_back(t);
        }
      }
      rulesV.push_back(nr);
      rulesM[nr.name] = nr;
    }

    vector<part> accepted;

    /*for (auto p : partsV)
    {
      {
        rule r   = rulesM["in"];
        auto res = checkCond(p, r.cond);

        if (!res.empty())
        {
          rule crtR = r;
          while (true)
          {
            if (res == "R")
              break;
            else if (res == "A")
            {
              accepted.push_back(p);
              break;
            }
            else
            {
              crtR = rulesM[res];
              res  = checkCond(p, crtR.cond);
            }
          }
        }
      }
    }

    for (auto p : accepted)
    {
      for (auto [k, v] : p.rs)
        ret += v;
    }*/

    ret = 0;
    struct interval
    {
      int from{ 1 };
      int to{ 4000 };
      LL  size() { return 1 + to - from; }
    };

    auto getRangeSize = [](map<string, interval> ranges)
    {
      return ranges["x"].size() * ranges["m"].size() * ranges["a"].size() * ranges["s"].size();
    };

    function<LL(string ruleName, map<string, interval> ranges)> split;
    split = [&](string ruleName, map<string, interval> ranges) -> LL
    {
      LL t = 0;

      auto rule = rulesM[ruleName];

      for (auto r : rule.cond)
      {
        if (r.cond.empty())
        {
          if (r.result == "A")
            t += getRangeSize(ranges);
          else if (r.result == "R")
          {
            t += 0;
          }
          else
          {
            t += split(rulesM[r.result].name, ranges);
          }
        }
        else
        {
          auto r0      = r;
          auto opCond  = getCondOp(r0);
          auto opField = getCondField(r0);
          auto opValue = getCondValue(r0);
          if (opCond == ">")
          {
            if (ranges[opField].to > opValue)
            {
              auto newRanges          = ranges;
              newRanges[opField].from = max((LL)ranges[opField].from, opValue + 1);
              // newRanges[opField].to   = ranges[opField].to;

              if (r0.result == "A")
                t += getRangeSize(newRanges);
              else
                t += split(r0.result, newRanges);
            }
            ranges[opField].to = opValue;
          }
          else
          {
            if (ranges[opField].from < opValue)
            {
              auto newRanges        = ranges;
              newRanges[opField].to = min((LL)ranges[opField].to, opValue - 1);
              // newRanges[opField].to   = ranges[opField].to;

              if (r0.result == "A")
                t += getRangeSize(newRanges);
              else
                t += split(r0.result, newRanges);
            }
            ranges[opField].from = opValue;
          }
        }
      }

      return t;
    };

    map<string, interval> ranges;
    for (char c : "xmas"s)
      ranges[c + ""s] = interval{};
    ret = split("in", ranges);
    cout << "X: " << ret << endl;
    return ret;
  }

  LL DoWork2()
  {
    LL ret = 192;
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
