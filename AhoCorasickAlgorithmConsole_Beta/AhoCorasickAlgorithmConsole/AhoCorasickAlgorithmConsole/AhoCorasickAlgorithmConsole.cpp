// AhoCorasickAlgorithmConsole.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <string>
#include<conio.h>
#include "ACAlgorithm.h"

#define NORMAL_STYLE (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED)
#define HIGHLINE_STYLE (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | FOREGROUND_BLUE & FOREGROUND_GREEN & FOREGROUND_RED)

using namespace std;
using namespace ACAlgorithm;
using namespace Tree;

/*
* test 1
*-------------------------------
   ***keywords : "red hr her say she shr shehe he r hehe"
   text 1 : "there have a red apple and a green apple on her hands shehehesheheshe."
   text 2 : "shr"
   =====================
 * test 2
 *-------------------------------
   ***keywords : "i he his she hers"
   text : "ushersheishis"
      =====================
 * test 3
 *-------------------------------
   ***keywords : "she e he hehe"
   text : "shehehe"
*/

int main()
{
    ACStringSearching ac;

    char operKey = 'E';
    do {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        //title
        SetConsoleTextAttribute(hConsole, HIGHLINE_STYLE ^ BACKGROUND_RED);
        cout << "Aho-Corasick Algorithm Multi-keyword Searching";
        SetConsoleTextAttribute(hConsole, NORMAL_STYLE);
        cout << endl << "------------------------------------------------------------------------------------------" << endl;
        string inputWord = "";
        char temp;
        //input keywords
        if (operKey != 't' && operKey != 'T') {
            ac.ClearACTrie();

            cout << "Please input ";
            SetConsoleTextAttribute(hConsole, HIGHLINE_STYLE);
            cout << "Keywords, Pattern = ( keyword1 keyword2 keyword3 ... keywordN )";
            SetConsoleTextAttribute(hConsole, NORMAL_STYLE);
            cout << " : " << "\n";

            SetConsoleTextAttribute(hConsole, HIGHLINE_STYLE);
            //insert words
            inputWord = "";
            temp;
            while (cin.get(temp) && temp != '\n') {
                if ((temp == ' ') && (static_cast<int>(inputWord.length()) != 0)) {
                    ac.AddKeyword(inputWord);
                    inputWord = "";
                }
                else if (ac.IsValidChar(temp))
                    inputWord += temp;
            }
            if (static_cast<int>(inputWord.length()) != 0) {
                ac.AddKeyword(inputWord);
                inputWord = "";
            }

            SetConsoleTextAttribute(hConsole, NORMAL_STYLE);

            ac.BuildACTrie();

            cout << endl;
        }

        cout << "Please input ";
        //SetConsoleTextAttribute(hConsole, HIGHLINE_STYLE);
        cout << "Text";
        //SetConsoleTextAttribute(hConsole, NORMAL_STYLE);
        cout << " : " << "\n";
        //SetConsoleTextAttribute(hConsole, HIGHLINE_STYLE);

        inputWord = "";
        temp = '\0';
        while (cin.get(temp) && temp != '\n') {
            inputWord += temp;
        }

        string text = inputWord;

        int textLength = static_cast<int>(text.length());

        KeywordLocationCollection matchingResult;
        int resultCount = ac.MatchText(text, matchingResult);

        SetConsoleTextAttribute(hConsole, NORMAL_STYLE);

        cout << "------------------------------------------------------------------------------------------" << endl;

        //show the count of matchs
        cout << "Found ";
        SetConsoleTextAttribute(hConsole, NORMAL_STYLE ^ FOREGROUND_BLUE);
        cout << to_string(resultCount);
        SetConsoleTextAttribute(hConsole, NORMAL_STYLE);
        cout << " Results ";
        SetConsoleTextAttribute(hConsole, HIGHLINE_STYLE | FOREGROUND_BLUE);
        cout << "(Start_position - End_position)";
        SetConsoleTextAttribute(hConsole, NORMAL_STYLE);
        cout << " : \n";

        //show where the words are
        if (resultCount > 0) {
            KeywordLocation* location = matchingResult.GetFirstLocation();
            while (location) {
                bool skipBegin;
                int begin = (skipBegin = location->startPosition > 5) ? location->startPosition - 5 : 0;
                bool skipEnd;
                int end = (skipEnd = location->endPosition < (textLength - 6)) ? location->endPosition + 5 : textLength - 1;
                cout << (skipBegin ? "..." : "   ");
                for (int i = begin;
                    i <= end;
                    i++) {
                    if (i == location->startPosition)
                        SetConsoleTextAttribute(hConsole, HIGHLINE_STYLE | FOREGROUND_RED);
                    if (i == location->endPosition + 1)
                        SetConsoleTextAttribute(hConsole, NORMAL_STYLE);
                    cout << text[i];
                }
                SetConsoleTextAttribute(hConsole, NORMAL_STYLE);
                cout << (skipEnd ? "..." : "   ");
                cout << "  ::::  ";
                SetConsoleTextAttribute(hConsole, HIGHLINE_STYLE | FOREGROUND_BLUE);
                cout << location->keyword;
                if (location->startPosition == location->endPosition)
                    cout << " (" + to_string(location->startPosition + 1) << ")";
                else
                    cout << " (" + to_string(location->startPosition + 1) << " - " << to_string(location->endPosition + 1) << ")";
                SetConsoleTextAttribute(hConsole, NORMAL_STYLE);
                cout << ";" << endl;
                cout << "------------------------------------------------------------------------------ \n";
                //
                location = location->next;
            }
        }

        cout << endl << endl;

        //check continue
        cout << "Press Any Key to start from Keyword; \"T\" start from Text; \"E\" for Exit : ";
        operKey = _getch();
        cout << "\n=========================================================================" << endl;
    } while (operKey != 'e' && operKey != 'E');
}

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
