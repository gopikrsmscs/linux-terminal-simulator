#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

/* Author: Pavuluri Gopikrishna
 */


/* A Function which converts into the chunks of the size 5 */
vector<vector<string> > getVectorContainer(vector<pair<string, long long> > vectorA)
{
    int c = 0;
    vector<vector<string> > myvectorContainer;
    vector<string> fileNameVector;
    for (int i = 0; i < vectorA.size(); i++)
    {
        if (i != 0 && i % 5 == 0)
        {
            fileNameVector.push_back(vectorA[i].first);

            myvectorContainer.push_back(fileNameVector);
            fileNameVector.clear();
        }
        else
        {
            fileNameVector.push_back(vectorA[i].first);
        }
    }
    myvectorContainer.push_back(fileNameVector);
    return myvectorContainer;
}


/* Custom Sort function  to sort based on the size */
bool sortbysec(const pair<string, long long> &a,
               const pair<string, long long> &b)
{
    return (a.second < b.second);
}

int main(void)
{
    
    
    struct dirent *content;
    int i, pageNum = 0;
    char c, k;
    char s[256], cmd[256];
    time_t t;
    DIR *d;
    struct stat st;
    vector<std::string> fileNames;
    vector<std::string> directoryNames;
    vector<vector<string> >  fileNamesContainer;
    vector<vector<string> > directoryNameContainer;
    vector<pair<string, long long> > fileNameAndSizeVector;
    vector<pair<string, long long> > directoryNameAndSizeVector;
    map<string,long long> fileNameToSizeMapping;
    map<string,long long> directoryNameToSizeMapping;


    

    while (1)
    {
        t = time(NULL);
        char *tm = ctime(&t);
        cout << "Time:" << tm << endl;
        cout << "-----------------------------------------------" << endl;

        getcwd(s, 200);
        cout << "Current Directory: " << s << endl;

        // Reading Files into vector
        if(fileNameAndSizeVector.size() == 0){
             d = opendir(".");
            while ((content = readdir(d)))
            {
                if (((content->d_type) & DT_REG))
                {

                    stat(content->d_name, &st);
                    fileNameAndSizeVector.push_back(make_pair(content->d_name, (long long)st.st_size));
                    fileNameToSizeMapping[content->d_name]=(long long)st.st_size;
                }
            }
            closedir(d);
            // Reading Directory into vector
            d = opendir(".");
            c = 0;
            while ((content = readdir(d)))
            {
                if ((content->d_type) & DT_DIR)
                {
                    stat(content->d_name, &st);
                    directoryNameAndSizeVector.push_back(make_pair(content->d_name, (long long)st.st_size));
                    directoryNameToSizeMapping[content->d_name]=(long long)st.st_size;
                }
            }
            closedir(d);
        }
        
        cout << "----------------Operations---------------------" << endl;
        cout << "D: Display" << endl;
        cout << "E: Edit" << endl;
        cout << "R: Run" << endl;
        cout << "C: Change Directory" << endl;
        cout << "S:Sort Directory Listing" << endl;
        cout << "M: Move to Directory" << endl;
        cout << "r:Remove File" << endl;
        cout << "Q: QUIT" << endl;
        cout << "--------------Enter Choice---------------------" << endl;
        cin >> c;
        switch (c)
        {
        case 'D': 
            cout << "--------------------Files----------------------" << endl;

            // Get the Pagination Results for Files
            fileNamesContainer = getVectorContainer(fileNameAndSizeVector);

            /*  Fit the files into window with the list size of 5*/
            pageNum=0;
            while (pageNum >= 0 && pageNum < fileNamesContainer.size())
            {
                for (int y = 0; y < fileNamesContainer[pageNum].size(); y++)
                {
                    cout << ((pageNum * 5) + y + 1) << " " << fileNamesContainer[pageNum][y] << " ,Size: "<< fileNameToSizeMapping[fileNamesContainer[pageNum][y]]<< endl;
                }
                cout << "Hit 'N' for Next File List and 'P' for previous files list" << endl;
                cin >> k;
                if (k == 'N')
                {
                    pageNum++;
                }
                if (k == 'P')
                {
                    pageNum--;
                }
            }
            cout << "-----------------Directories-------------------" << endl;

            //Get the pagination results for the directory

            directoryNameContainer = getVectorContainer(directoryNameAndSizeVector);
            pageNum = 0;
            while (pageNum >= 0 && pageNum < directoryNameContainer.size())
            {
                for (int y = 0; y < directoryNameContainer[pageNum].size(); y++)
                {
                    cout << ((pageNum * 5) + y + 1) << " " << directoryNameContainer[pageNum][y] << ", Size "<< directoryNameToSizeMapping[directoryNameContainer[pageNum][y]]<< endl;
                }
                cout << "Hit 'N' for Next Directory List and 'P' for previous directory list" << endl;
                cin >> k;
                if (k == 'N')
                {
                    pageNum++;
                }
                if (k == 'P')
                {
                    pageNum--;
                }
            }
            cout << "-----------------------------------------" << endl;
            break;
        case 'Q':
            exit(0); 
        case 'E':
            cout << "Enter File Name to Edit." << endl;
            scanf("%s", s);
            strcpy(cmd, "pico ");
            strcat(cmd, s);
            system(cmd);
            break;
        case 'R':
            cout << "Enter Unix Command to Run file" << endl;
            scanf("%s", cmd);
            system(cmd);
            break;
        case 'C':
            cout << "Enter Command to change the directory." << endl;
            scanf("%s", cmd);
            chdir(cmd);
            fileNameAndSizeVector.clear();
            break;

        case 'S':
            sort(fileNameAndSizeVector.begin(), fileNameAndSizeVector.end(), sortbysec); //Sort the files vector by size
            sort(directoryNameAndSizeVector.begin(), directoryNameAndSizeVector.end(), sortbysec);//Sort the dorectory vector by size
            directoryNameContainer.clear();
            fileNamesContainer.clear();
            cout<<"Choose d option to display the sorted directory"<<endl;
            break;

        case 'r':
            char fileName[256];
            char command[256];
            cout << "Enter the file name to remove" << endl;
            scanf("%s", fileName);
            strcpy(command, "rm ");
            strcat(command, fileName);
            system(command);
            for(int i=0;i<fileNameAndSizeVector.size();i++){
                if(fileNameAndSizeVector[i].first == fileName){
                    fileNameAndSizeVector.erase(fileNameAndSizeVector.begin()+i);
                    break;
                }
            }
            break;

        case 'M':
            char source[256];
            char destinaation[256];
            cout << "Enter the source directory" << endl;
            scanf("%s", source);
            cout << "Enter the destination directory" << endl;
            scanf("%s", destinaation);
            strcpy(cmd, "mv ");
            strcat(cmd, source);
            int size = sizeof(cmd);
            cmd[size] = ' ';
            strcat(cmd, destinaation);
            system(cmd);
            break;
        }
    }
}
