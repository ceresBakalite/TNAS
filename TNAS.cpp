// TNAS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*

  The Ratcliff/Obershelp pattern-matching algorithm was developed by John W. Ratcliff and 
  John A. Obershelp in 1983 to address concerns about educational software (Ratcliff, 1988).

  Approximate string matching: takes two strings and returns a similarity score based on
  co-occurrence of sub patterns.

  This is perhaps my fifth language translation since 1989 from the original Assembler, done 
  mostly for fun. Later renditions of the original in a variety of languages are now publicly 
  available and may differ from the code as presented here.

  The algorithm itself has become part of the Python pantheon of comparison functions and can
  be found in The Python Standard Library: class difflib.SequenceMatcher.

  Required:

  1. getGestaltValue(string str1, string str2)
  2. matchPattern(string str1, int strLen1, string str2, int strLen2)

  The getGestaltValue function calls the matchPattern function, which in turn calls itself until 
  all patterns are matched and a percentage value is returned.

  Usage:

  See main()

  Notes:

  1. When the pre processing initialiseComparison() function is invoked and is instructed to
     ignore the character case in the string pair it utilises transform ::tolower which is not
     unicode compliant.  Unicode compliance, although desirable, is unnecessary as ::tolower
     will resolve, or indeed fail to resolve, unicode characters uniformly for both comparison
     strings.

  2. The matchPattern() Ratcliff/Obershelp recursive pattern matching algorithm appears to
     be, at face value, a very simple comparison algorithm.  This would be misleading.

     The algorithm is inherently fuzzy as the probability of two dissimilar strings being,
     in fact, similar can be highly likely when comparing sets of data.

     Depending on the data sets, a name may take a high precedence, or perhaps the combination of 
     a name / country / city subset will hold greater significance. Conversely, noise words and
     phrases can clutter a comparison pair and may detract from an otherwise highly similar match.

     Consider also, as another example, comparing numeric data strings when pattern matching image
     comparison sets, and then consider weighting certain compound angular values over others.

     The Ratcliff/Obershelp algorithm definitely benefits from both pre and post processing of the
     comparison strings especially when adding different weighted values to significant aspects
     inherent in the both the string data and the search criteria.

  3. The primary function getGestaltValue() initialises the string length pattern matching
     variables once only before calling the recursive matchPattern() function.

     To paraphrase the original usage text and the TEXT SEGMENT portion in SIMIL.ASM published on 
     July 01, 1988 by John W. Ratcliff and David E. Metzener:

        November 10, 1987

        Uses the Ratcliff/Obershelp pattern recognition algorithm. This program provides a new
        function to C on an 8086 based machine. The function SIMIL returns a percentage value
        corresponding to how alike any two strings are.  Be certain to upper case the two strings
        passed if you are not concerned about case sensitivity...

        ...the similarity routine begins by computing the total length of both strings passed
        and placing that value in TOTAL.  It then takes the beginning and ending of both strings
        passed and pushes them on the stack.  It then falls into the main line code. The
        original two strings are immediately popped off of the stack and are passed to the
        compare routine.

        Ratcliff, J & Metzener, D 1988, 'Pattern Matching: the Gestalt Approach: The Ratcliff/Obershelp 
        pattern-matching algorithm', Dr Dobb's Journal, pp. 46-51. 

  The following source code attempts to replicate this behaviour

  Alexander Munro - January 29, 2020

*/

#include <iostream>
#include <algorithm>
#include <sstream>

using namespace std;

// the pre process functions
void initialiseComparison(char* argv[]); // capture and pre process the command-line arguments
void commandLineUsage(int argc, char* argv[]); // if necessary display the command-line usage requirements
void requestDefaultComparison(); // request whether the user wishes to use the default comparison strings

// the Ratcliff/Obershelp functions
int getGestaltValue(string str1, string str2); // initialise the pattern matching algorithm
int matchPattern(string str1, int strLen1, string str2, int strLen2); // recursively match the comparison strings

using namespace std;

int main(int argc, char* argv[])
{
    if (argc == 4)
    {
        initialiseComparison(argv);
    }
    else {
        commandLineUsage(argc, argv);
        requestDefaultComparison();
    };

    return 0;
};

void initialiseComparison(char* argv[])
{
    //preprocess arguments

    string str1 = argv[1];
    string str2 = argv[2];
    string arg3 = argv[3];

    // remove space characters (artificial weighting 1)
    str1.erase(remove(str1.begin(), str1.end(), ' '), str1.end());
    str2.erase(remove(str2.begin(), str2.end(), ' '), str2.end());

    int strLen1 = str1.length();
    int strLen2 = str2.length();

    // pad the strings to equal length (artificial weighting 2)
    if (strLen1 > strLen2) { str2.insert(str2.end(), strLen1 - strLen2, ' '); };
    if (strLen2 > strLen1) { str1.insert(str1.end(), strLen2 - strLen1, ' '); };

    bool bIgnoreCase = (arg3 == "true") ? true : false;

    if (!bIgnoreCase) { stringstream argStream(argv[3]); argStream >> bIgnoreCase; };

    // apply case insensitivity to each string (artificial weighting 3)
    if (bIgnoreCase)
    {
        transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
        transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
    }

    cout << ((bIgnoreCase) ? "\nThe case insensitive gestalt percentage returned: " : "\nThe case sensitive gestalt percentage returned: ");

    // call the principle function
    int test1 = getGestaltValue(str1, str2);

    // weighted converse comparison example (artificial weighting 4)
    reverse(str1.begin(), str1.end());
    int test2 = getGestaltValue(str1, str2) / 2;

    int weightedComparison = ((test1 >= test2) ? test1 : test2);
    
    cout << weightedComparison << endl;
};

void requestDefaultComparison()
{
    string useDefault = "N";

    cout << "Use the default test comparison strings instead?  (Y\\N) ";
    cin >> useDefault;

    transform(useDefault.begin(), useDefault.end(), useDefault.begin(), ::tolower);

    if (useDefault == "y")
    {
        string compareThis = "aabbccdd";
        string withThis = "aabbCCDD";

        cout << "\nDefault comparison test arguments:\n\nCompare this string: \"" << compareThis << "\"\n" << "with this string: \"" << withThis << "\"\n" << endl;

        // call the principle function (Note: there are no weighted arguments in this example)
        cout << "The case sensitive gestalt percentage returned: " << getGestaltValue(compareThis, withThis) << endl;
    };
};

void commandLineUsage(int argc, char* argv[])
{
    cout << ((argc == 1) ? "\nNo arguments were passed to the command-line" : "\nThe command-line requires three arguments. The following were used...\n\nCommand-line arguments: \n") << endl;

    for (int i = 1; i < argc; i++)
    {
        cout << "  arg[" << i << "]   " << argv[i] << "\n";
    };

    cout << "\nUsage: \n" << endl;
    cout << argv[0] << " \"comparison string one\"  \"comparison string two\" ignore case (true or false)\n" << endl;
    cout << "ie: " << argv[0] << " \"compare this string\"  \"with this string\" true\n" << endl;
};

// the two pattern matching functions 

int getGestaltValue(string str1, string str2) 
{
    int strLen1 = str1.length();
    int strLen2 = str2.length();

    if (strLen1 == 0 || strLen2 == 0) return 0;

    return (matchPattern(str1, strLen1, str2, strLen2) * 200) / (strLen1 + strLen2);
};

int matchPattern(string str1, int arLengthLeft, string str2, int arLengthRight)
{
    const char* arLeft = str1.c_str();
    const char* arRight = str2.c_str();

    int i, j, k, m;
    int arLength = 0;
    int arLengthLeftReset = 0;
    int arLengthRightReset = 0;

    for (i = 0; i < arLengthLeft - arLength; i++)
    {
        for (j = 0; j < arLengthRight - arLength; j++)
        {
            if (arLeft[i] == arRight[j] && arLeft[i + arLength] == arRight[j + arLength])
            {
                for (k = i + 1, m = j + 1; arLeft[k] == arRight[m] && k < arLengthLeft && m < arLengthRight; k++, m++);

                if (k - i > arLength)
                {
                    arLengthLeftReset = i;
                    arLengthRightReset = j;
                    arLength = k - i;
                };
            };
        };
    };

    if (arLength == 0) return 0;

    i = (arLengthLeftReset + arLength);
    j = (arLengthRightReset + arLength);

    arLengthLeft -= i;
    arLengthRight -= j;

    int leftMatch = (arLengthLeftReset != 0 && arLengthRightReset != 0) ? matchPattern(arLeft, arLengthLeftReset, arRight, arLengthRightReset) : 0;
    int rightMatch = (arLengthLeft != 0 && arLengthRight != 0) ? matchPattern(arLeft + i, arLengthLeft, arRight + j, arLengthRight) : 0;

    return arLength + leftMatch + rightMatch;
};