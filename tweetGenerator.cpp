#include "twitterClient.h"

#include <iostream>

#include <string>

#include <vector>

#include <map>

#include <bits/stdc++.h>

#include <fstream>

#include <ctime>

#include <streambuf>

#include <stdlib.h>

#include <random>

#include "json.hpp"

#include <unistd.h>

#include <chrono>

#include <thread>

#include <functional>

#include <unistd.h>

using json = nlohmann::json;
using namespace std;


void printUsage() {
  printf("\nUsage:\ntwitterClient -u username -p password\n");
}

bool BothAreSpaces(char lhs, char rhs) {
  return (lhs == rhs) && (lhs == ' ');
}
json toJson(const char * jsonString) {
  json jsonObj;
  std::stringstream(jsonString) >> jsonObj;

  return jsonObj;

}

class marcov {
  map < string, vector < string >> dict;
  vector < string > startWords;
  string outout;
  long int since_Id = 0;
  string user = "";
  int count = 0;
  public:
    void driver(string textString, bool jsonBool) {
      if (jsonBool) {

        printf("\nEnter the User you want to find( without @ if ou write wrong programm might crash(couldnt fix that) just try again than): ");
        cin >> user;
        while (count < 5) { //limted by twitter api 10 are 5*200 tweets
          count++;
          setTwitter(true);
          cout << "xxxxxxx" << endl;
        }

      } else createDict(textString);
      createTweet();
      cout << "Do yo want to post the tweet, input Y " << endl;
      string input;
      cin >> input;
      input[0] = toupper(input[0]);
      if (input[0] == 'Y') {
        setTwitter(false);
      }
      cout << "Do yo want to run the Bot i your name it post Every  5 Seconds, input Y if so " << endl;
      string input2;
      cin >> input2;
      input2[0] = toupper(input2[0]);
      if (input2[0] == 'Y') {
        while (true) {
          outout = "";
          usleep(5000 * 1000); // takes microseconds
          bot();
        }
      }

    }
  private:

    void bot() {
      createTweet();
      setTwitter(false);
    }

  void goThroughTweets(string jsonString) {

    nlohmann::json answer = toJson(jsonString.c_str());
    long int n = 0;

    for (int i = 0; i < answer.size(); i++) {
      std::string temp = answer[i]["full_text"].get < std::string > ();
      //std::cout << temp << '\n';
      int pos = temp.find_last_of(" \t\n");
      temp = temp.substr(0, pos);
      if (pos > -1) createDict(temp);
      n = i;
    }
    string number = answer[n]["id_str"].get < std::string > ();
    since_Id = stol(number.c_str());

  }
  void createDict(string ex) {

    //std::cout << "createDict" << '\n';
    ex.push_back(' ');
    char n = 10; // is /n
    char r = 13; // is /r
    char t = 30; // other newlines
    replace(ex.begin(), ex.end(), n, ' ');
    replace(ex.begin(), ex.end(), t, ' ');
    replace(ex.begin(), ex.end(), r, ' ');
    string::size_type pos = 0; // Must initialize
    while ((pos = ex.find("\r\n", pos)) != string::npos) {
      ex.erase(pos, 2);
    }

    while ((pos = ex.find("\r", pos)) != string::npos) {
      ex.erase(pos, 2);
    }
    while ((pos = ex.find("\n", pos)) != string::npos) {
      ex.erase(pos, 1);
    }

    string::iterator new_end = std::unique(ex.begin(), ex.end(), BothAreSpaces);
    ex.erase(new_end, ex.end());

    int start = 0;
    int found1 = ex.find_first_of(" ");
    if (found1 == 0 && ex.size() > 2) {
      ex.erase(0, 1);
      found1 = ex.find_first_of(" ");
    }
    int found2 = ex.find_first_of(" ", found1 + 1);
    int found3 = ex.find_first_of(" ", found2 + 1);
    if (found3 != std::string::npos && found2 != std::string::npos && found1 != std::string::npos) {
      std::string starters = ex.substr(start, found2 - start);
      startWords.push_back(starters);
    }

    while (found3 != std::string::npos && found2 != std::string::npos && found1 != std::string::npos)

    {

      if (ex[found1 - 1] == '.') {
        std::string upperString = ex.substr(found1 + 1, found3 - found1 - 1);
        if (isupper(upperString[0])) {
          startWords.push_back(upperString);

        }
      }
      std::string key = ex.substr(start, found2 - start);

      std::string value = ex.substr(found2 + 1, found3 - found2 - 1);

      dict[key].push_back(value);
      start = found1 + 1;
      found1 = found2;
      found2 = found3;
      found3 = ex.find_first_of(" ", found3 + 1);
    }
    //printDict();

  }
  void printDict() {
    //std::cout << "printDict" << '\n';
    map < string, vector < string > > ::const_iterator it;
    for (it = dict.begin(); it != dict.end(); ++it)
        {
           cout <<"keys: "<< it->first<<"\n" << endl ;
           vector<string>::const_iterator itVec;
           for (itVec = it->second.begin(); itVec != it->second.end(); ++itVec)
           {
             cout<<"Values: " << *itVec <<";";
                    }
                    cout<<"\n"<<endl;

        }

        vector<string>::const_iterator itVec;
        for (itVec = startWords.begin(); itVec != startWords.end(); ++itVec)
        {

        }

    }
  void createTweet() {

    int maxLengt = 240;
    double lengthsStartVec = startWords.size();
    //std::cout << "length start words: " << lengthsStartVec << '\n';
    std::random_device rd; // only used once to initialise (seed) engine
    std::mt19937 rng(rd()); // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution < int > uni(0, lengthsStartVec - 1); // guaranteed unbiased

    unsigned rand1 = uni(rng);
    //std::cout << "radn: " << rand1 << '\n';
    //std::cout << startWords.at(rand1) << '\n';
    outout.append(startWords.at(rand1));
    /*  std::cout << "start" << "\n\n\n";
      cout << outout << "\n\n\n";
      std::cout << "end" << "\n\n\n";*/
    int start = 0;
    int found1;
    int found2;
    bool stop = false;

    while (outout.size() < maxLengt + 1 && !stop) {
      outout.append(" ");
      found1 = outout.find_first_of(" ", start);
      //std::cout<<"found1: " << found1 << '\n';
      found2 = outout.find_first_of(" ", found1 + 1);
      //std::cout <<"found2: "<< found2 << '\n';
      std::string key = outout.substr(start, found2 - start);
      //  std::cout << "key: " << key << '\n';
      if (dict[key].size()) {
        int lengthsVecKEx = dict[key].size();
        std::uniform_int_distribution < int > uni2(0, lengthsVecKEx - 1); // guaranteed unbiased
        unsigned rand2 = uni2(rng);
        //  std::cout << "length  words: " << lengthsVecKEx << '\n';
        //  std::cout << "radn: " << rand2 << '\n';
        //std::cout << dict[key].at(rand2) << '\n';
        string temp(dict[key].at(rand2));
        outout.append(temp);
        //  std::cout << "start" << "\n\n\n";
        //  cout << outout << "\n\n\n";
        // std::cout << "end" << "\n\n\n";
        start = found1 + 1;
      } else {
        std::cout << "No more follwowing Keys exist" << endl;
        stop = true;
      }

    }

    //cut of string so its under 240
    //cuts string at endpoint so it just doesnt end in random word
    long unsigned endings[7] = {
      outout.rfind(". "),
      outout.rfind(".\t"),
      outout.rfind(". "),
      outout.rfind("! "),
      outout.rfind("? "),
      outout.rfind("“ "),
      outout.rfind("; ")
    };
    long unsigned temp = 0;

    for (int i = 0; i < 7; i++) {
      if (endings[i] > temp && endings[i] != string::npos && endings[i] < 240)
        temp = endings[i];
    }
    if (temp > 0) {
      outout = outout.substr(0, temp + 1);
    } else {
      //cuts last word
      size_t pos3 = outout.find_last_of(" ");

      outout = outout.substr(0, pos3);

    }
    std::cout << "Tweet is in result.html" << endl;
    std::ofstream res("result.html");

    res << outout;
    res.close();

  }

  void setTwitter(bool flagGet) {
    twitCurl twitterObj;
    std::string tmpStr;
    std::string replyMsg;
    char tmpBuf[1024];

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
      /* Step 2: Get request token key and secret */
      std::string authUrl;
      twitterObj.oAuthRequestToken(authUrl);

      /* Step 3: Get PIN  */

      /* Ask user to visit twitter.com auth page and get PIN */
      memset(tmpBuf, 0, 1024);
      printf("\nPlease visit this link in web browser and authorize this application:\n%s", authUrl.c_str());
      printf("\nEnter the PIN provided by twitter: ");
      cin >> tmpStr;
      twitterObj.getOAuth().setOAuthPin(tmpStr);

      /* Step 4: Exchange request token with access token */
      twitterObj.oAuthAccessToken();

      /* Step 5: Now, save this access token key and secret for future use without PIN */
      twitterObj.getOAuth().getOAuthTokenKey(myOAuthAccessTokenKey);
      twitterObj.getOAuth().getOAuthTokenSecret(myOAuthAccessTokenSecret);

      /* Step 6: Save these keys in a file or wherever */
      std::ofstream oAuthTokenKeyOut;
      std::ofstream oAuthTokenSecretOut;

      oAuthTokenKeyOut.open("twitterClient_token_key.txt");
      oAuthTokenSecretOut.open("twitterClient_token_secret.txt");

      oAuthTokenKeyOut.clear();
      oAuthTokenSecretOut.clear();

      oAuthTokenKeyOut << myOAuthAccessTokenKey.c_str();
      oAuthTokenSecretOut << myOAuthAccessTokenSecret.c_str();

      oAuthTokenKeyOut.close();
      oAuthTokenSecretOut.close();
    }
    /* OAuth flow ends */

    /* Account credentials verification */
    if (twitterObj.accountVerifyCredGet()) {
      twitterObj.getLastWebResponse(replyMsg);
      //printf( "\ntwitterClient:: twitCurl::accountVerifyCredGet web response:\n%s\n", replyMsg.c_str() );
    } else {
      twitterObj.getLastCurlError(replyMsg);
      //printf( "\ntwitterClient:: twitCurl::accountVerifyCredGet error:\n%s\n", replyMsg.c_str() );
    }
    if (flagGet) {
      /* Get public timeline */
      replyMsg = "";

      string userForTweets = user;

      if (twitterObj.timelineUserGet(0, true, false, 200, userForTweets, false)) {

        twitterObj.getLastWebResponse(replyMsg);
        //printf( "\ntwitterClient:: twitCurl::timelinePublicGet web response:\n%s\n", replyMsg.c_str() );
        goThroughTweets(replyMsg);

      } else {
        twitterObj.getLastCurlError(replyMsg);
        printf("\ntwitterClient:: twitCurl::timelinePublicGet error:\n%s\n", replyMsg.c_str());
      }

    }

    if (!flagGet) {
      replyMsg = "";
      string userForTweets = "Hey"; //otherwise invalid free because of libabry maybe
      if (twitterObj.statusUpdate(outout)) {
        twitterObj.getLastWebResponse(replyMsg);
        //printf( "\ntwitterClient:: twitCurl::timelinePublicGet web response:\n%s\n", replyMsg.c_str() );
        cout << "Success in posting the tweet" << endl;
      } else {
        twitterObj.getLastCurlError(replyMsg);
        //  printf( "\ntwitterClient:: twitCurl::timelinePublicGet error:\n%s\n", replyMsg.c_str() );
        cout << "Failed in posting the tweet" << endl;
      }

    }

  }

};
int main(int argc, char * argv[]) {

  string word, t, q, filename;
  string twit = "twitter";
  string txtadd = ".txt";
  marcov m;
  while (true) {
    cout << "Please enter txt File or if you want to use Twitter say twitter" << endl;
    cin >> filename;

    if (filename.find(twit) != std::string::npos) {
      cout << "You want to use twiiter " << filename << endl;
      m.driver("hey", true);
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
        m.driver(content, false);

        break;

      }
    }
  }

  return 0;
}
