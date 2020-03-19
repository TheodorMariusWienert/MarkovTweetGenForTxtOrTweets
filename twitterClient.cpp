#include "twitterClient.h"

#include <iostream>

#include <string>

#include <vector>

#include <map>

#include <bits/stdc++.h>

#include <fstream>

#include <streambuf>



#include <stdlib.h>


using namespace std;

 #include <jsoncpp/json/json.h>




void printUsage() {
  printf("\nUsage:\ntwitterClient -u username -p password\n");
}

bool BothAreSpaces(char lhs, char rhs) {
  return (lhs == rhs) && (lhs == ' ');
}

class marcov {
  map < string, vector < string >> dict;
  vector < string > startWords;
  public:
    void goThroughTweets(vector < string > text) {
      std::ofstream out1("tt.txt");

      for (size_t n = 0; n < text.size(); ++n) {

        int pos = text[n].find_last_of(" \t\n");
        text[n] = text[n].substr(0, pos);


        out1<<"||"<<text[n]<<"||\n";
        if (pos>-1) createDict(text[n]);
      }
      out1.close();

    }
  void createDict(string ex) {

    //std::cout << "createDict" << '\n';
    ex.push_back(' ');
    replace(ex.begin(), ex.end(), '\n', ' ');
    string::size_type pos = 0;

    while ( ( pos = ex.find ("\r\n",pos) ) != string::npos )
{
ex.erase ( pos, 2 );
}
    string::iterator new_end = std::unique(ex.begin(), ex.end(), BothAreSpaces);
    ex.erase(new_end, ex.end());


        int start = 0;
    int found1 = ex.find_first_of(" ");
    if (found1==0&&ex.size()>2){
     ex.erase (0,1);
     found1 = ex.find_first_of(" ");
    }
    int found2 = ex.find_first_of(" ", found1 + 1);
    int found3 = ex.find_first_of(" ", found2 + 1);
    if (found3 != std::string::npos &&found2!= std::string::npos&&found1!= std::string::npos){
      std::string starters = ex.substr(start, found2 - start);
    startWords.push_back(starters);
  }
    while (found3 != std::string::npos&&found2!= std::string::npos &&found1!= std::string::npos)

    {

      if (ex[found1-1]=='.')
        {
          std::string upperString = ex.substr(found1+1, found3 - found1-1);
           if (isupper(upperString[0])) {
              startWords.push_back(upperString);

           }
        }
      std::string key = ex.substr(start, found2 - start);
      //cout<<"key|"<<key<<"|\n";
      std::string value = ex.substr(found2 + 1, found3 - found2 - 1);
      //cout<<"value|"<<value<<"|\n";
      dict[key].push_back(value);
      start = found1 + 1;
      found1 = found2;
      found2 = found3;
      found3 = ex.find_first_of(" ", found3 + 1);
    }

  printDict();
  }
  void printDict() {
    //std::cout << "printDict" << '\n';
    map <string, vector<string> >::const_iterator it;

    for (it = dict.begin(); it != dict.end(); ++it)
    {
       //cout <<"keys: "<< it->first<<"\n" << endl ;

       vector<string>::const_iterator itVec;
       for (itVec = it->second.begin(); itVec != it->second.end(); ++itVec)
       {
           //cout<<"Values: " << *itVec <<";";
       }
       //cout<<"\n"<<endl;
    }
    vector<string>::const_iterator itVec;
    std::ofstream out("startWord.txt");
    for (itVec = startWords.begin(); itVec != startWords.end(); ++itVec)
    {
        //cout<<"startWords: " << *itVec <<"\n";

        out << *itVec << "\n\n";


    }
    out.close();

  }


};
static void getTweets( string userForTweets, marcov m) {
      twitCurl twitterObj;
      std::string tmpStr, tmpStr2;
      std::string replyMsg;
      char tmpBuf[1024];

      /* Set twitter username and password
      twitterObj.setTwitterUsername(userName);
      twitterObj.setTwitterPassword(passWord);

      /* Set proxy server usename, password, IP and port (if present)
      memset(tmpBuf, 0, 1024);
      printf("\nDo you have a proxy server configured (0 for no; 1 for yes): ");
      fgets(tmpBuf, sizeof(tmpBuf), stdin);
      tmpStr = tmpBuf;
      if (std::string::npos != tmpStr.find("1")) {
        memset(tmpBuf, 0, 1024);
        printf("\nEnter proxy server IP: ");
        fgets(tmpBuf, sizeof(tmpBuf), stdin);
        tmpStr = tmpBuf;
        twitterObj.setProxyServerIp(tmpStr);

        memset(tmpBuf, 0, 1024);
        printf("\nEnter proxy server port: ");
        fgets(tmpBuf, sizeof(tmpBuf), stdin);
        tmpStr = tmpBuf;
        twitterObj.setProxyServerPort(tmpStr);

        memset(tmpBuf, 0, 1024);
        printf("\nEnter proxy server username: ");
        fgets(tmpBuf, sizeof(tmpBuf), stdin);
        tmpStr = tmpBuf;
        twitterObj.setProxyUserName(tmpStr);

        memset(tmpBuf, 0, 1024);
        printf("\nEnter proxy server password: ");
        fgets(tmpBuf, sizeof(tmpBuf), stdin);
        tmpStr = tmpBuf;
        twitterObj.setProxyPassword(tmpStr);
      }

      /* OAuth flow begins */
      /* Step 0: Set OAuth related params. These are got by registering your app at twitter.com */
      twitterObj.getOAuth().setConsumerKey(std::string("jR7hyQ1HZimZHG4ecPavl2FbT"));
      twitterObj.getOAuth().setConsumerSecret(std::string("5vFHELVODVYhLuDMG0XtlrmgV9YFpY5RQKL12vRPltX8c7byo7"));

      /* Step 1: Check if we alredy have OAuth access token from a previous run */
      std::string myOAuthAccessTokenKey("");
      std::string myOAuthAccessTokenSecret("");
      std::ifstream oAuthTokenKeyIn;
      std::ifstream oAuthTokenSecretIn;

      oAuthTokenKeyIn.open("twitterClient_token_key.txt");
      oAuthTokenSecretIn.open("twitterClient_token_secret.txt");

      memset(tmpBuf, 0, 1024);
      oAuthTokenKeyIn >> tmpBuf;
      myOAuthAccessTokenKey = tmpBuf;

      memset(tmpBuf, 0, 1024);
      oAuthTokenSecretIn >> tmpBuf;
      myOAuthAccessTokenSecret = tmpBuf;

      oAuthTokenKeyIn.close();
      oAuthTokenSecretIn.close();


      if (myOAuthAccessTokenKey.size() && myOAuthAccessTokenSecret.size()) {
        /* If we already have these keys, then no need to go through auth again */
        printf("\nUsing:\nKey: %s\nSecret: %s\n\n", myOAuthAccessTokenKey.c_str(), myOAuthAccessTokenSecret.c_str());

        twitterObj.getOAuth().setOAuthTokenKey(myOAuthAccessTokenKey);
        twitterObj.getOAuth().setOAuthTokenSecret(myOAuthAccessTokenSecret);
      } else {
        /*Doesnt Work something in libary is broken or twitter api change, if i delete the one line of code below I  get so Segmentation faault weird */
        /* Step 2: Get request token key and secret */
        cout<<"Heyyyyyyyyyyyy";
     std::ofstream oAuthTokenSecretOut;


      }
      /* OAuth flow ends */

      /* Account credentials verification */
      if (twitterObj.accountVerifyCredGet()) {
      //  twitterObj.getLastWebResponse(replyMsg);
        //printf( "\ntwitterClient:: twitCurl::accountVerifyCredGet web response:\n%s\n", replyMsg.c_str() );
      } else {
        twitterObj.getLastCurlError(replyMsg);
        printf("\ntwitterClient:: twitCurl::accountVerifyCredGet error:\n%s\n", replyMsg.c_str());
      }

      /* Post a new status message
      memset( tmpBuf, 0, 1024 );
      printf( "\nEnter a new status message: " );
      fgets( tmpBuf, sizeof( tmpBuf ), stdin );
      tmpStr = tmpBuf;
      replyMsg = "";
      if( twitterObj.statusUpdate( tmpStr ) )
      {
          twitterObj.getLastWebResponse( replyMsg );
          printf( "\ntwitterClient:: twitCurl::statusUpdate web response:\n%s\n", replyMsg.c_str() );
      }
      else
      {
          twitterObj.getLastCurlError( replyMsg );
          printf( "\ntwitterClient:: twitCurl::statusUpdate error:\n%s\n", replyMsg.c_str() );
      }
      /* Get user timeline*/

      std::string my;
      std::cout << "Getting " << userForTweets << "'s timeline.";
      userForTweets = userForTweets.replace(0, 1, "");
      int i =200;
      if (twitterObj.timelineUserGet(true, false, i, userForTweets, false)) {
      twitterObj.getLastWebResponse(my);
       //printf( "\ntwitterClient:: twitCurl::timelineUserGet web response:\n%s\n", replyMsg.c_str() );
        std::string  mystring= my.c_str();
        Json::Reader reader;
        Json::Value answer;
        reader.parse(mystring, answer);


        //json answer=json::parse(mystring);
        vector < string > tweets;

        for (int i = 1; i < answer.size(); i++) {
          std::string temp = answer[i]["full_text"].asString();
          tweets.push_back(temp);
        }
      m.goThroughTweets(tweets);



        std::cout << "Succes in retriving \n";

      } else {
        twitterObj.getLastCurlError(replyMsg);
        printf("\ntwitterClient:: twitCurl::timelineUserGet error:\n%s\n", replyMsg.c_str());
      }
    }

int main(int argc, char * argv[]) {

  string word, t, q, filename;
  char at = '@';
  string txtadd = ".txt";
  marcov m;
  while (true) {
    cout << "Please enter txt File or a Twitter username with @before: ";
    cin >> filename;
    size_t found = filename.find(at);
    if (found == 0) {
      cout << "The twitter username you entered is:  " << filename << endl;
      getTweets(filename, m);
      break;
    }
    if (filename.find(txtadd) != std::string::npos) {
      cout << "The file you want to use ist:  " << filename << endl;
      ifstream file(filename.c_str());
      if (file.fail()) {
        cout << "There was a problem opening the input file, please check if the name was correct" << endl;
      } else {
        cout << "Success in opening the file" << endl;
        std::string content((std::istreambuf_iterator < char > (file)),
          (std::istreambuf_iterator < char > ()));
          //std::cout << content << '\n';
        m.createDict(content);


        break;

      }
    }
  }

  return 0;
}
