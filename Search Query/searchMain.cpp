/*
 * Name: Spalding Latham
 * Date Submitted: 11/30/18
 * Lab Section: 3
 * Assignment Name: Lab 11
 */
#include <iostream>
#include <dirent.h>
#include "searchpp.h"
#include <vector>

using namespace std;

// Run the search function in searchpp
int main(int argc, char* argv[]){

   DIR *dp;
   struct dirent *ep;
   vector<string> contentFiles;

   dp = opendir("./content");
   if(dp != NULL){
     while((ep = readdir(dp)) != NULL){
          contentFiles.push_back(ep->d_name);
         }
         closedir(dp);
       } else {
           perror("Couldn't open the directory!");
       }


   int contentSize = contentFiles.size();
   query test(contentFiles);

   //start at index two to skip the . and .. directories
   for(int i = 2; i < contentSize; i++){
      test.fileMap[i - 2] = contentFiles[i];
     }


   cout << "Enter a Search Query: ";

   string queryString;
   getline(cin, queryString);

   test.longest_match(queryString);


}
