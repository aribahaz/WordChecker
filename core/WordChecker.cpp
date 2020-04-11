// WordChecker.cpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"
#include <iostream>
#include <algorithm>

WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    std::string temp = word;
    std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
    //std::transform(words.begin(), words.end(), words.begin(), ::toupper);
    //std::cout<< word << std::endl;
    return words.contains(temp);
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
   std::vector<std::string> sug;
   swapIt(word, sug);
   insertIt(word, sug);
   deleteIt(word, sug);
   replaceIt(word, sug);
   splitIt(word, sug);
   return sug;
}


void WordChecker::swapIt(const std::string& word, std::vector<std::string>& sugs) const
{
  int len = word.length()-1;
  for(int i = 0; i < len; ++i)
     {
       std::string temp = word;
       std::swap(temp[i], temp[i+1]);
       if(wordExists(temp) && std::find(sugs.begin(), sugs.end(), temp) == sugs.end())
         {
           sugs.push_back(temp);
         }
     }
}

void WordChecker::insertIt(const std::string& word, std::vector<std::string>& sugs) const
{
  int len = word.length();
  for(int i = 0; i <= len; ++i)
    {
      for(char letter = 'A'; letter <= 'Z'; ++letter)
      {
        std::string temp = word; 
        temp.insert(i, 1, letter);
        if(wordExists(temp) && std::find(sugs.begin(), sugs.end(), temp) == sugs.end())
          {
            sugs.push_back(temp);
          }
      }
    }
}

void WordChecker::deleteIt(const std::string& word, std::vector<std::string>& sugs) const
{
  int len = word.length()-1;
  for(int i = 0; i <= len; ++i)
    {
      std::string temp = word;
      temp.erase(i, 1);
      if(wordExists(temp) && std::find(sugs.begin(), sugs.end(), temp) == sugs.end())
        {
          sugs.push_back(temp);
        }
    }
}

void WordChecker::replaceIt(const std::string& word, std::vector<std::string>& sugs) const
{
  int len = word.length()-1;
  for(int i = 0; i <= len; ++i)
    {
      for(char letter = 'A'; letter <= 'Z'; ++letter)
        {
          std::string temp = word;
          temp[i] = letter;
          if(wordExists(temp) && std::find(sugs.begin(), sugs.end(), temp) == sugs.end())
            {
              sugs.push_back(temp);
            }
        }
    }
}

void WordChecker::splitIt(const std::string& word, std::vector<std::string>& sugs) const
{
  int len = word.length()-1;
  for(int i = 0; i < len; ++i)
    {
      std::string temp = word;
      std::string t1 = temp.substr(0, i);
      std::string t2 = temp.substr(i, len);
      if(wordExists(t1) && wordExists(t2) && std::find(sugs.begin(), sugs.end(), t1) == sugs.end() &&
         std::find(sugs.begin(), sugs.end(), t2) == sugs.end())
        {
          sugs.push_back(t1 + " " + t2);
        }
    }
}

