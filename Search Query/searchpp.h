/*
 * Name: Spalding Latham
 * Date Submitted: 11/30/18
 * Description: This is the driver class for the search program 
 */

#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <math.h>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <deque>

using namespace std;

class query{
public:
       query(vector<string> contentFiles);
       void longest_match(string s);
       vector<string> files;
       unordered_map<int, string> fileMap;
       unordered_map<string, pair<string, int>> textMap;
       string toLower(string s);
       int calculateScore(string q, string t);
       bool isPhrase = false;
       bool isFile = false;
       int spaceCounter = 0;
   };


   /*---------------------*/
   /*default constructor*/
   /*-----------------*/
   query::query(vector<string> contentFiles){
     files = contentFiles;
    }

   /*--------------------------------------------*/
   /*function that converts string to lower case*/
   /*------------------------------------------*/
   string query::toLower(string s){
     int length = s.length();
     for(int i = 0; i < length; i++){
          if(s[i] > 64 && s[i] < 91){
             s[i] += 32;
            }

        }
      return(s);
    }

   /*---------------------------------------------------------*/
   /*function that calculates the score for each word/phrase*/
   /*-----------------------------------------------------*/
   int query::calculateScore(string t, string q){
      int qLength = q.length();
      int tLength = t.length();
      int inPlace = 0;
      int inCommon = 0;
      int score = 0;

      //make sure all lowercase for comparison
      string tLower = toLower(t);
      string qLower = toLower(q);


      //compare difference in string lengths, - 1 to account for space
      int distance = -1;
      distance += abs(tLength - qLength);

      unordered_map<int, int> alreadyChecked;
      bool beenChecked = false;
      int alreadyCheckedPos = 0;

      for(int i = 0; i < tLength; i++){
          for(int j = 0; j < qLength; j++){

             //makes sure inCommon doesn't double count same letter
             if(tLower[i] == qLower[j]){
                for(auto x : alreadyChecked){
                   if(j == x.second){
                     beenChecked = true;
              }
            }

              if(beenChecked == false){
                 inCommon++;
              }

              alreadyChecked[alreadyCheckedPos] = j;
              alreadyCheckedPos++;
              beenChecked = false;

              //if they share the same letter in same position the inPlace++
              if(tLower[i] == qLower[i]){
                inPlace++;
              }

            }

      }
    }

   //scoring algorithm: importance-wise: inPlace > distance > inCommon
   score = (pow(inPlace, 2)) + inCommon - (distance * 4);

   //files have higher importance than text
   if(isFile == true){
      score *= 10;
   }
   return(score);


    }


   /*----------------------------------------------------*/
   /* function that finds the best match from the query */
   /*--------------------------------------------------*/

   void query::longest_match(string s){
   int highScore = 0;
   pair<string, bool> bestMatch;
   string bestMatchLocation;
   deque<string> phraseList;

   int sLength = s.length();
   //cout << (pow(sLength, 2) + sLength);
   string queryLower = toLower(s);

   //add .txt to query to check if it's a file
   string queryDotText = queryLower + ".txt";

   //set to true if there's an exact match
   bool isTrue = false;

   /*-------------------------------------*/
   /*calculate scores for all file names*/
   /*----------------------------------*/
   for(auto x : fileMap){

      //store file name lowercase for comparison
      string fileLower = toLower(x.second);
      isFile = true;
      int score = calculateScore(x.second, s);

      if(score > highScore){
         highScore = score;
         bestMatch.first = x.second;
         bestMatch.second = true;
         bestMatchLocation = x.second;
        }


      //print out file if query is exact match
      if((queryDotText == fileLower) || ((queryDotText.substr(0, sLength)) == fileLower)){
        cout << x.second << endl;
        isTrue = true;
        exit(0);

      }

    }

    int length = s.length();
    //if it has a space then it's a phrase
    for(int i = 0; i < length; i++){
       if(s[i] == ' '){
         isPhrase = true;
         spaceCounter++;
       }
     }

     /*-----------------------------------------------------------------------*/
     /*if query isn't a file name and not a phrase, calculate all word scores*/
     /*---------------------------------------------------------------------*/
     isFile = false;

     if(isTrue == false && isPhrase == false){
       for(auto x : fileMap){


          string fileInfo;
          string temp;

          ifstream myFile;
          myFile.open("./content/" + x.second);

          if(!myFile){
            cout << "Unable to open file " << x.second;
            exit(1);
          }

          while(myFile >> temp){

             int score = calculateScore(temp, s);

             //calculate score for single words
             if(score > highScore){
                highScore = score;
                bestMatch.first = temp;
                bestMatch.second = false;
                bestMatchLocation = x.second;
               }

             //if its an exact match print and exit
             if(score == (pow(sLength, 2) + sLength)){
               if(bestMatch.second == false){
                 cout << bestMatchLocation << endl;
               }
               cout << bestMatch.first << endl;
               exit(0);
              }


      }


          myFile.close();


     }

  }


   /*----------------------------------------------------------------*/
   /*if not an exact file match and is a phrase then calculate score*/
   /*--------------------------------------------------------------*/
   if(isTrue == false && isPhrase == true){

     int numWords = spaceCounter + 1;

     //look through phrases in all files
     for(auto x : fileMap){
     string fileInfo;
     string temp;

     ifstream myFile;
     myFile.open("./content/" + x.second);

     if(!myFile){
       cout << "Unable to open file " << x.second;
       exit(1);
     }

     int dSize = 0;
     string phrase;
     bool isDequeFull = false;


     while(myFile >> temp){
          //deque will hold same number of words as query
          if(isDequeFull != true){
             phraseList.push_back(temp);
             dSize = phraseList.size();
             //it's full when the size = number of words in query
             if(dSize == numWords){
                isDequeFull = true;
             }
        }

        if(isDequeFull == true){
           //adds all its contents into a single phrase
           for(int i = 0; i < dSize; i++){
              phrase += phraseList[i];
              phrase += " ";
           }

           //calculate the phrase's score and update highScore
           int score = calculateScore(phrase, s);

           if(score > highScore){
             highScore = score;
             bestMatch.first = phrase;
             bestMatch.second = false;
             bestMatchLocation = x.second;
           }

           //if its an exact match print and exit
           if(score == (pow(sLength, 2) + sLength)){
             cout << bestMatchLocation << endl;
             cout << bestMatch.first << endl;
             exit(0);
           }

           //pops the first element of deque to add the next word
           phraseList.pop_front();
           isDequeFull = false;
           //resetting the phrase is KEY
           phrase = "";
      }



}

    myFile.close();
  }

}


   //if there isn't an exact match, print what has the highest score
   if(isTrue == false){
     if(bestMatch.second == false){
     cout << bestMatchLocation << endl;
    }
      cout << bestMatch.first << endl;

   }

}
