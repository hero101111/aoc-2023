#pragma once

#include "SolutionDay.h"

class Day7 : public ISolutionDay
{
private:

  vector<string> mData;

public:

  Day7(){ }

  ~Day7() override { }

  void ReadData()
  {
    mData.clear();
    mData = rff(GetInputPath());
  }
  
  string GetDay() override
  {
    return "7";
  }

  LL DoWork(bool part2)
  {
    LL ret = 0;
    map<char, LL> cm;
    // A , K, Q, J, T, 9, 8, 7, 6, 5, 4, 3, or 2
    LL ttt = 15;
    cm['A'] = ttt--;
    cm['K'] = ttt--;
    cm['Q'] = ttt--;
    cm['J'] = ttt--;
    cm['T'] = ttt--;
    cm['9'] = ttt--;
    cm['8'] = ttt--;
    cm['7'] = ttt--;
    cm['6'] = ttt--;
    cm['5'] = ttt--;
    cm['4'] = ttt--;
    cm['3'] = ttt--;
    cm['2'] = ttt--;

    if (part2)
      cm['J'] = ttt--;

    auto getHandValue = [part2](string h) 
      {
        assert(h.size() == 5);
        unordered_map<char, int> freq;
        for (auto c : h)
        {
          if (c != '\0')
            freq[c]++;
        }
        vector<pair<char, int>> freqV;
        for (auto [k, v] : freq)
        {
          freqV.push_back(make_pair(k, v));
        }


        int ret = 0;

        
        if (part2 && freq['J'] == 1)
        {
          if (freqV.size() == 2)
            ret = 7; // five of a kind
          else if (freqV.size() == 3)
          {
            if (freqV[0].second == 3 || freqV[1].second == 3 || freqV[2].second == 3)
              ret = 6; // four of a kind
            else
              ret = 5; // full house
          }
          else if (freqV.size() == 4)
            ret = 4; // three of akind
          else if (freqV.size() == 5)
            ret = 2; // one pair
          else
            assert(!"invalid");
        }
        else if (part2 && freq['J'] == 2)
        {
          if (freqV.size() == 2)
            ret = 7; // five of a kind
          else if (freqV.size() == 3)
            ret = 6; // four of a kind
          else if (freqV.size() == 4)
            ret = 4; // three of akind
          else
            assert(!"invalid");
        }
        else if (part2 && freq['J'] == 3)
        {
          if (freqV.size() == 2)
            ret = 7; // five of a kind
          else if (freqV.size() == 3)
            ret = 6; // four of a kind
          else
            assert(!"invalid");
        }
        else if (part2 && freq['J'] == 4)
        {
          ret = 7; // five of a kind
        }
        else if (part2 && freq['J'] == 5)
        {
          ret = 7; // five of a kind
        }
        else
        {

          //if (h[0] == h[1] == h[2] == h[3] == h[4])
          if (freqV.size() == 1)
            ret = 7; // five of a kind
          if (freqV.size() == 2)
          {
            if (freqV[0].second == 4 || freqV[1].second == 4)
              ret = 6; // four of a kind
            if (freqV[0].second == 3 || freqV[1].second == 3)
              ret = 5; // full house
          }
          if (freqV.size() == 3)
          {
            if (freqV[0].second == 3 || freqV[1].second == 3 || freqV[2].second == 3)
              ret = 4; // three of akind

            if (freqV[0].second == 2 || freqV[1].second == 2 || freqV[2].second == 2)
              ret = 3; // two pair
          }
          if (freqV.size() == 4)
          {
            if (freqV[0].second == 2 || freqV[1].second == 2 || freqV[2].second == 2 || freqV[3].second == 2)
              ret = 2;; // one pair
          }
          if (freqV.size() == 5)
          {
            ret = 1; // high card;
          }
        }

        assert(ret != 0);

        return ret;
      };

    struct hb
    {
      string hand;
      LL bid;
    };

    vector<hb> hands;

    for (auto d : mData)
    {
      auto [hand, value] = split(d, ' ');
      hb hb;
      hb.bid = stoll(value);
      hb.hand = hand;      
      hands.push_back(hb);
    }
    sort(begin(hands), end(hands),
      [&](const hb & hb1, const hb& hb2) {
        auto v1 = getHandValue(hb1.hand);
        auto v2 = getHandValue(hb2.hand);
        if (v1 < v2)
          return true;
        if (v1 == v2)
        {
          for (int i = 0; i < 5; ++i)
          {
            char l1 = hb1.hand[i];
            char l2 = hb2.hand[i];
            if (cm[l1] < cm[l2])
              return true;
            if (cm[l1] > cm[l2])
              return false;
            if (cm[l1] == cm[l2])
              continue;
          }
        }
        return false; 
      });

    LL crtP = 1;
    for (auto v : hands)
    {
      ret += v.bid * crtP++;
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
    aoc_assert(Part1(), "6440"s);
    aoc_assert(Part2(), "5905"s);
    mCurrentInput = "input";
    aoc_assert(Part1(), "250254244"s);
    aoc_assert(Part2(), "250087440"s);
    return true;
  }
};
