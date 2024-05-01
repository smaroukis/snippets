// Source: Ritchie 1978 Sec. 1.9 Character Arrays p. 26

// Learned
// can't assign a char array to another char array i.e. `saved = currentLine`, instead 
// must use a copy command


// Usage
// Use the provided VSCode task which builds then runs with e.g.  ./main < test_input.txt

#include <stdio.h>
#include <stdint.h>

typedef void (*generalTestFunct)(); // change return type and arguments as nec.

#define MAXLEN 1000

int get_line(char s[], int lim);
void copy(char s1[], char s2[]);

// Reads a set of lines and prints the longest
void IO_longestLine(void)
{
    int len;
    int maxSeen = 0;
    char currentLine[MAXLEN];
    char saved[MAXLEN];

    // While there's another line (get line)
    while ( (len = get_line(currentLine, MAXLEN)) ) { // NB len == 0 for EOF
        // if it's longer than the previous longest
        if (len > maxSeen) 
        {
            // save it and its length
            maxSeen = len; 
            // saved = currentLine; // cannot do this
            copy(currentLine, saved);
        }
    }

    // print the saved longest line 
    if (maxSeen > 0)  // there was a line
    {
        printf("%s", saved);
    }
}

// Get the next line of input, return length of line or EOR signal 
// inputs: variable to hold the line char array and limit
// affects: stores line in char s[] 
// EOF indication is 0 since min line length is 1 (`\n`)
int get_line(char s[], int lim)
{
    int i;
    char c;
    
    // while not EOF and not endline, get the next char and increment i
    for (i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
    {
        s[i] = c;
    }

    if (c == '\n')
    {
        s[i] = c;
        ++i;
    }

    s[i] = '\0'; // terminate string correctly
    return i; // 0-indexed i will index the null terminator which gives the length
}

// Copy s1 to s2
// Assumes s2 is big enough to carry s1 (no sanity check)
void copy(char s1[], char s2[])
{
    int i = 0;

    while ( (s2[i] = s1[i]) != '\0' )
    {
        ++i;
    }
}

int main(void) {

    generalTestFunct test_func = &IO_longestLine;
    
    // test 1:
    test_func();

    return 0;
}
