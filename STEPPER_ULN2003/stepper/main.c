//2线四相八拍
#include <STC89C5xRC.H>

main(){

while(1){

unsigned int i,j,k;

 for(k=0;k<255;k++){
//四线八拍
for(i=0;i<75;i++)
for(j=0;j<255;j++);

P2=0x01;//0001
 for(i=0;i<75;i++)
for(j=0;j<255;j++);

P2=0x03;//0011
   for(i=0;i<75;i++)
for(j=0;j<255;j++);

P2=0x02;//0010
  for(i=0;i<75;i++)
for(j=0;j<255;j++);

P2=0x06;//0110
 for(i=0;i<75;i++)
for(j=0;j<255;j++);

P2=0x04;//0100
 for(i=0;i<75;i++)
for(j=0;j<255;j++);

P2=0x0c;//1100
   for(i=0;i<75;i++)
for(j=0;j<255;j++);

P2=0x08;//1000
  for(i=0;i<75;i++)
for(j=0;j<255;j++);

P2=0x09;//1001
 }

  }
}