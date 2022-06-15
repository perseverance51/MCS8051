#include <intrins.h>
#include <reg51.h>
#include "main.h"

//Variables
static  data long lvalue;
static  data long rvalue;
static  data char currtoken;
static  data char lasttoken;
static  data char lastpress;
static  xdata char outputbuffer[MAX_DISPLAY_char];

void main (void)
 { 
	 lvalue    = 0;
   rvalue    = 0;
   currtoken = '=';
   lasttoken = '0';
   initialise();  // ���,��ʼ��Һ����ʾ����
   calc_output(OK);
   calc_evaluate();
 }   

void calc_evaluate()
 { 
	 char data key;
   int  data i;
   char xdata number[MAX_DISPLAY_char];
   char xdata *bufferptr;
   
   // Clear the buffer before we start.
   for (i = 0; i <= MAX_DISPLAY_char; i++)
      { number[i] = ' ';
	  }
   bufferptr = number;  
   
   for (;;)
     { key = calc_getkey();
	   if (calc_testkey(key))
       // Key test positive for digit so we read it into the
       // buffer and then write the buffer to the screen/LCD.
	   // Size limit the number of digits - allow for termination
	   // and possible negative results.
          { if (bufferptr != &number[MAX_DISPLAY_char - 2])
               { *bufferptr = key;
                 calc_display(number);
                 bufferptr++;
               }
          }

       else
       // Key is an operator so pass it to the function handlers.
       // If we are just after startup or cancel then assign to lvalue
       // otherwise assign to rvalue.
          { 
		    //Assign the value.
            if (lasttoken == '0')
               { lvalue = calc_asciidec (number);}
            else
               { rvalue = calc_asciidec (number);}

            //Clear the number buffer.
            bufferptr = number;
            for (i = 0;i <= MAX_DISPLAY_char; i++)
               { number[i] = ' '; }
		 
            //Process the Operator.
            currtoken = key;
			if (currtoken == 'C') 
			   { calc_opfunctions(currtoken); }
		 	else
			   { calc_opfunctions(lasttoken); }
		
 		    // Clear the outputbuffer for reuse on next operation.
            for (i = 0;i <= MAX_DISPLAY_char;i++)
               { outputbuffer[i] = ' ';}
		      
      	     bufferptr = number;
			// Handle the equals operation here for brevity.
			// All we need do is preserve the previous operator in
			// lasttoken.
			if (currtoken != 0x3D) lasttoken = currtoken;
            
   		  }
       lastpress = key;
     }
 }

void calc_opfunctions (char token)
// Handle the operations. Lvalue holds the result and we test for
// consecutive operator presses.
 { char data result;
   switch(token)
        // Add.
     {  case '+' : if ((currtoken == '=' ) || ((lastpress >= 0x30) && (lastpress <=0x39)))
      			      { lvalue += rvalue;
        			    result = calc_chkerror(lvalue);
					  }
   				   else
      				  { result =  SLEEP; }		break;
        // Subtract.
		case '-' : if ((currtoken == '=' ) || ((lastpress >= 0x30) && (lastpress <=0x39)))
                      { lvalue -= rvalue;
                        result = calc_chkerror(lvalue);		
					  }
                   else
                      { result = SLEEP;}		break;
        // Multiply.
		case '*' : if ((currtoken == '=' ) || ((lastpress >= 0x30) && (lastpress <=0x39)))
                      { lvalue *= rvalue;
                        result =  calc_chkerror(lvalue);
                      }
                   else
                      { result =  SLEEP;}		break;
		// Divide.			  
		case '/' : if ((currtoken == '=' ) || ((lastpress >= 0x30) && (lastpress <=0x39)))
                      { if (rvalue)
                           { lvalue /= rvalue;
                             result = calc_chkerror(lvalue);
                           }
                        else
                           { result = ERROR;}				 
                      }
                   else
                      { result = SLEEP;}		break;
		// Cancel.
 		case 'C' : lvalue = 0;
                   rvalue = 0;
                   currtoken = '0';
                   lasttoken = '0';
                   result = OK;				  	break;
	
		default :  result = SLEEP;  

      }
   calc_output(result); 
 }

 
/************************************************************************
***** Utility Routines *****
***************************/

int calc_chkerror (long num)
// Check upper and lower bounds for the display.
// i.e. 99999999 and -99999999.
 { if ((num >= -9999999) && (num <= 9999999))
      return OK;
   else
      return ERROR;
 }


void calc_output (int status)
// Output according to the status of the operation.
// *Sleep* is used for the first op press after a full cancel
// or on startup.  
 { switch (status)
      { case OK      : calc_display(calc_decascii(lvalue));    break;
        case SLEEP   :                                         break;
		case ERROR   : calc_display("Exception ");			   break;	
        default      : calc_display("Exception ");	    	   break;
      }
 }


long calc_asciidec (char *buffer)
// Convert the ASCII string into the floating point number.
 { long data value;
   long data digit;
   value = 0;
   while (*buffer != ' ')
      { digit = *buffer - 48;
	    value = value*10 + digit;
        buffer++;
	  }
   return value;
 }

char *calc_decascii (long num)
// A rather messy function to convert a floating
// point number into an ASCII string.
 { long data temp = num;
   char xdata *arrayptr = &outputbuffer[MAX_DISPLAY_char];
   long data divisor = 10;
   long data result;
   char data remainder,asciival;
   int  data i;
   
   // If the result of the calculation is zero 
   // insert a zero in the buffer and finish.
   if (!temp)
      { *arrayptr = 48;
	    goto done;
	  }
   // Handle Negative Numbers.
   if (temp < 0)
      { outputbuffer[0] = '-';
	    temp -= 2*temp;
	  }

   for (i=0 ; i < sizeof(outputbuffer) ; i++)
      { remainder = temp % divisor;   
        result = temp / divisor;
	    
		// If we run off the end of the number insert a space into
	    // the buffer.
	    if ((!remainder) && (!result))
 	       { *arrayptr = ' ';}
	  
	    // We're in business - store the digit offsetting
	    // by 48 decimal to account for the ascii value.
	    else
	       { asciival = remainder + 48;
		     *arrayptr = asciival;
		   } 
 	  
		temp /= 10;
	    // Save a place for a negative sign.
	    if (arrayptr != &outputbuffer[1]) arrayptr--;
	   }
   done: return outputbuffer;
 }


char calc_testkey (char key)
// Test whether the key is a digit or an operator. Return 1 for digit, 0 for op.
 { if ((key >= 0x30) && (key <= 0x39))
      { return 1;}
   else
      { return 0;}
 }

/************************************************************************
***** I/O Routines *****
***********************/

char calc_getkey (void)
// Use the input routine from the *Keypad_Read* assembly file to 
// Scan for a key and return ASCII value of the Key pressed.
{ char data mykey;
  do mykey = input();
     while (mykey == 0);
  return mykey;
 }

void calc_display (char buf[MAX_DISPLAY_char])
// Use the Output and Clearscreen routines from the 
// *LCD_Write* assembly file to output ASCII values to the LCD.
 { int data  i = 0;
   clearscreen();
   for (i ; i <= MAX_DISPLAY_char ; i++)
      { if (buf[i] != ' ')
	     { output(buf[i]); }
	  }	
 }





