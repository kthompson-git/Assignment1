#include<bits/stdc++.h>  
#include <iostream>
#include <unistd.h>
#include <stdio.h>


// check if character in string is duplicated
bool duplicate(int index, std::string &fileIn)
{
  if (index == 0)
    return false;
  else
  {
    for (int i = 0; i < index; i++)
    {
      if (fileIn[index] == fileIn[i])
        return true;
    }
    return false;
  }
}

// check frequency of characters in a string
int frequency(char symbol, std::string &fileIn)
{
  int cnt = 0;
  for (int i = 0; i < fileIn.length(); i++)
  {
    if (symbol == fileIn[i])
      cnt++;
  }
  return cnt;
}

// checks character occurences in a string and populates vectors
void getStringData(std::string &fileIn, std::vector<char> &sym, std::vector<int> &cnt)
{  
  for (int i = 0; i < fileIn.length(); i++)
  {
    if (!duplicate(i, fileIn))
    {
      sym.push_back(fileIn[i]);
      cnt.push_back(frequency(fileIn[i], fileIn));
    }   
  }  
  return;
}

// swap the position of two elements in vectors
void vectorSwap(int index1, int index2, std::vector<char> &sym, std::vector<int> &cnt)
{
  int intTemp = cnt[index1];
  char charTemp = sym[index1];

  cnt[index1] = cnt[index2];
  sym[index1] = sym[index2];

  cnt[index2] = intTemp;
  sym[index2] = charTemp;

  return;
}

// sorts vectors from most frequent character to least
// sorts equal frequencies by character low to high ACSII value
void sortVectors(std::vector<char> &sym, std::vector<int> &cnt)
{
  int max = 0, maxIndex;

  for (int i = 0; i < sym.size(); i++)
  {
    for (int j = i; j < sym.size(); j++)
    {
      if (cnt[j] > max)
      {
        maxIndex = j;
        max = cnt[j];
      }
      else if (cnt[j] == max && (int)sym[j] < (int)sym[maxIndex])
        maxIndex = j;
    }
    vectorSwap(i, maxIndex, sym, cnt);
    max = 0;
  }
  return;
}

// changes all newline characters to <EOL>
void newlineToEOL(std::string &fileIn)
{
  std::string temp;
  for (int i = 0; i < fileIn.length(); i++)
  {
    if (fileIn[i] == '\n')
      temp = temp + "<EOL>";
    else
      temp.push_back(fileIn[i]);
  }
  fileIn.swap(temp);
  return;
}

// child process function to write compression to file
void writeToFile(char sym, std::string fileIn, std::string fileName)
{
  std::string convertEOL = fileIn;
  std::ofstream outFile;
  outFile.open(fileName);
  newlineToEOL(convertEOL);
  outFile << convertEOL << std::endl;
  for (int i = 0; i < fileIn.length(); i++)
  {
    if (fileIn[i] == sym)
      outFile << 1;
    else
      outFile << 0;
  }
  outFile.close();
  return;
}

// remove specified character from string
void removeChar(char sym, std::string &str)
{
  std::string temp;
  for (int i = 0; i < str.length(); i++)
  {
    if (str[i] != sym)
      temp.push_back(str[i]);
  }
  str.swap(temp);
  return;
}

// compresses symbols to binary
void compression(std::string &fileIn, std::vector<char> &sym)
{
  pid_t pid;
  std::string fileName;
  if (fileIn.empty())
    return;
  for (int i = 0; i < sym.size(); i++)
  {
    fileName = "out" + std::to_string(i);
    // pid = fork();
    writeToFile(sym[i], fileIn, fileName);
    removeChar(sym[i], fileIn);
  }
}

// prints the vectors
void printVectors(std::vector<char> &sym, std::vector<int> &cnt)
{
  for (int i = 0; i < sym.size(); i++)
  {
    if (sym[i] == '\n')
      std::cout << "<EOL> frequency = " << cnt[i] << std::endl;
    else
      std::cout << sym[i] << " frequency = " << cnt[i] << std::endl;
  }
  return;
}

// prints file(s) contents
void printFiles(std::vector<char> &sym)
{
  std::ifstream inFile;
  std::string fileName, message, code;
  for (int i = 0; i < sym.size(); i++)
  {
    fileName = "out" + std::to_string(i);
    inFile.open(fileName);
    std::getline(inFile, message);
    std::getline(inFile, code);
    if (i == 0)
      std::cout << "Original Message:\t" << message << std::endl;
    else
      std::cout << "Remaining Message:\t" << message << std::endl;
    
    if (sym[i] == '\n')
      std::cout << "Symbol <EOL> code:\t" << code << std::endl;
    else
      std::cout << "Symbol " << sym[i] << " code:\t\t" << code << std::endl;

    inFile.close();
  }
  return;
}

// calls printing functions
void printCompression(std::vector<char> &sym, std::vector<int> &cnt)
{
  printVectors(sym, cnt);
  printFiles(sym);
  return;
}

int main() 
{
  // string for file input
  std::string fileIn; 
  std::getline(std::cin, fileIn, '\0');

  // vectors for file info
  std::vector<char> symbol;
  std::vector<int> count;

  // populate vectors
  getStringData(fileIn, symbol, count);

  // sort vectors
  sortVectors(symbol, count);

  // perform symbol compression
  compression(fileIn, symbol);

  // print compression steps
  printCompression(symbol, count);

  return 0;
}
