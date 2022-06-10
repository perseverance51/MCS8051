typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef float FLOAT;
typedef double DOUBLE;

// Define the maximum number of ASCII characters that can fit
// on the display.
#define MAX_DISPLAY_char 9

//Error handling status.
enum ERROR { OK = 0, SLEEP = 1, ERROR = 2};

/************************************************************************
***** FUNCTION PROTOTYPES *****
******************************/

void calc_evaluate();

//Operator indirect function.
void calc_opfunctions (char token);

//Utility functions.
char  calc_testkey  (char ch);
long  calc_asciidec (char *buffer);
char *calc_decascii (long num);
int   calc_chkerror (long num);
void  calc_output   (int status);

// I/O functions.
char calc_getkey (void);
void calc_display (char buf[MAX_DISPLAY_char]);

//Assembly Function prototypes.
void initialise();
char input ();
char output(char ch);
void clearscreen(); 







