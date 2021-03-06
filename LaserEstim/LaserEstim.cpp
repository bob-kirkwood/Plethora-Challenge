// LaserEstim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>


#include "..\Geometry\Profile.h"
#include "..\Geometry\Vertices.h"
#include "..\Geometry\Edges.h"

using namespace std;

const string STR_REMEMBER_QUOTES = "Remember: A file name that includes spaces must be wrapped in quotes.";

const string STR_INVALID_CMD_LINE = "Invalid command line.";

const string STR_HELP =
"example: LaserEstim <Filename> verbose test\n"
"verbose (or v) is optional and tells you more about the processing\n"
"test (or t) is optional and makes arg processing ~2500 times slower, thus one arc takes a long as 2500 arcs.\n\n"
"LaserEstim computes an estimate from a json file holding a profile for a laser cut pattern of lines & arcs\n"
"";

void AwaitUserInput() { getchar(); }

void ShowHelp()
    {
    cout << STR_INVALID_CMD_LINE << endl << STR_HELP << endl << STR_REMEMBER_QUOTES;
    AwaitUserInput();
    }

int main(int argc, char *argv[])
    {
    try
        {
        bool bVerbose = false;
        bool bTestScalability = false;

        if (argc <= 1)
            {
            ShowHelp();
            return 0;
            }

        string strFile;
        for (int nx = 1; nx < argc; nx++)
            {
            string strArg(argv[nx]);
            std::transform(strArg.begin(), strArg.end(), strArg.begin(), ::tolower); // per https://stackoverflow.com/questions/313970/how-to-convert-stdstring-to-lower-case

            if (("h" == strArg) || ("help" == strArg))
                {
                ShowHelp();
                return 0;
                }
            if (strFile.empty())
                strFile = strArg;
            else if (("verbose" == strArg) || ("v" == strArg))
                bVerbose = true;
            else if (("test" == strArg) || ("t" == strArg))
                bTestScalability = true;
            else
                {
                ShowHelp();
                return 1;
                }
            }

        ifstream ffTest(strFile);
        if (!ffTest)
            {
            cout << "File not found: " << strFile << endl << STR_REMEMBER_QUOTES;
            AwaitUserInput();
            return 1;
            }

        Profile profile(strFile);

        time_t ttStart;
        time(&ttStart);

        if (bTestScalability)
            Profile::setAccuracy(1e-8); // for checking performance

        double fCost = profile.ComputeCost();

        if (bVerbose)
            {
            double fMatlArea = profile.ComputeMatlArea();
            cout << "Vertices:" << endl;
            for (VertexMap::VtxPtr vtxiter : profile.getVertices())
                cout << vtxiter->ToStr() << endl;
            cout << endl << "Edges:" << endl;
            for (shared_ptr<Edge> sp_edgeIter : profile.getEdges())
                cout << sp_edgeIter->ToStr() << endl;
            cout << endl << "ModelSizeIndicator: " << profile.GetEvalCount() << endl << endl;
            cout << "Material Area: " << fMatlArea << endl;
            cout << "Material Cost: " << fMatlArea * Profile::getMatlCost() << endl;
            cout << "Machine Cost:" << fCost - fMatlArea * Profile::getMatlCost() << endl;
            }

        cout << "Cost: " << fCost << " $/each" << endl << endl;

        time_t ttEnd;
        time(&ttEnd);

        if (bTestScalability)
            {
            cout << "Run time: " << difftime(ttEnd, ttStart) << " seconds" << endl;
            if (bVerbose)
                cout << "Note: Verbose mode causes processing to take twice as long";
            }

        AwaitUserInput();

        return 0;
        }
    catch (exception xx)
        {
        cerr << xx.what();
        AwaitUserInput();
        return 1;
        }
}

