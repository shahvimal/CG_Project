#include <iostream>
#include<graphics.h>
#include<conio.h>
#include<windows.h>
#include<pthread.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
//frog number
int fr=4;
//for placing frogs in the hole
int op1=0,op2=0,op3=0,op4=0;
//for getting the present location of cars on track 1
int xct1,xcb1;
//for getting the present location of cars on track 1
int xct2,xcb2;
//for getting the present location of frog
int xt=50,yt=395,xb=80,yb=425;
//this variable is for collision
int coll=0;
//global variable
int i1;
//variable for collision count
int ccount=0;
//global variables to restart game check function restart
int yes=1,no=0;

void border();
void frog();
void *moveFrog(void *vargp);
void *cars(void *vargp);
void *song(void *vargp);

void goal1();
void goal2();
void goal3();
void goal4();


void collision();
int inBetween(int cord,int xtop,int xbottom){
    if((cord>=xtop)&&(cord<=xbottom))
        return 1;
}
void winning();
void Win(){
    cleardevice();
    //number of frog crushed
    int nFc=ccount;

    //number of frog saved
    int nFs=op1+op2+op3+op4;

    char str[100],str1[100];
    int x = 200, y = 200;

    sprintf(str,"Frog Crushed : %d",nFc);
    sprintf(str1,"Frog Saved : %d",nFs);
    settextstyle(8, 0,3);
    if(nFs==4){
    outtextxy(100, 250, "You Win Because");
    outtextxy(100,300," You Saved All Frogs!!:)");
    }
    else if((nFs>0) && (nFs<4))
    {
        outtextxy(100, 250, str);
        outtextxy(100, 300, str1);
    }
    else {
        outtextxy(100, 250, "You Lose Because");
        outtextxy(100,300," You Crushed All Frogs!! :(");
    }
    delay(2000);
}

void reset(int i){
    if(i==1){xt=50,yt=395,xb=70,yb=425;}
    else if(i==2){xt=150,yt=395,xb=170,yb=425;}
    else if(i==3){xt=250,yt=395,xb=270,yb=425;}
    else if(i==4){xt=350,yt=395,xb=370,yb=425;}
    coll=0;
}
void restart();
void gameInfo();
int main()
{
    pthread_t thread_id1,thread_id2,thread_id3;
    int gd=DETECT,gm;
    int i;int wid1;
    initgraph(&gd,&gm,"C:\\Program Files (x86)\\CodeBlocks\\MinGW\\include");
    cleardevice();
    setcolor(RED);
    wid1=initwindow(640,480,"Frog Jump",0,0,false,true);
    setcurrentwindow(wid1);
    gameInfo();
    while(yes==1)
    {
        for(i=1;i<=fr;i++){
            i1=i;
            setfillstyle(SOLID_FILL,RED);
            reset(i);
            border();
            frog();

            pthread_create(&thread_id3, NULL, song, NULL);//thread for song function
            pthread_create(&thread_id1, NULL, moveFrog, NULL);//thread for moveFrog function
            pthread_create(&thread_id2, NULL, cars, NULL);//thread for cars function

            pthread_join(thread_id3, NULL);//joins song thread with main thread
            pthread_join(thread_id1, NULL);//joins moveFrog thread with main thread
            pthread_join(thread_id2, NULL);//joins cars thread with main thread

            winning();
            cleardevice();
        }
        restart();
    }
    cleardevice();
    exit(0);

    getch();
    return 0;
}

//from both track
void *cars(void *vargp)
{
    int i,j;
    char k;

    for(i=50,j=610;coll!=1&&yt!=95;i+=40,j-=40){
        delay(300-((i1-1)*70));
        collision();
        //changing whole track to black
        setfillstyle(SOLID_FILL,BLACK);

        readimagefile("road.jpg",10,170,630,255);
        frog();
        //cars
        if(i>640){i=20;}
        setfillstyle(SOLID_FILL,YELLOW);
        xct1=i,xcb1=50+i;
        readimagefile("carR.jpg",xct1,190,xcb1,240);
        readimagefile("carR.jpg",xct1+100,190,xcb1+100,240);
        readimagefile("carR.jpg",xct1+200,190,xcb1+200,240);
        readimagefile("carR.jpg",xct1+300,190,xcb1+300,240);
        readimagefile("carR.jpg",xct1+400,190,xcb1+400,240);

        //border overlapping right
        setfillstyle(SOLID_FILL,RED);
        bar(630,170,640,255);


        //for car on track 2
        readimagefile("road.jpg",10,265,630,355);

        frog();
        //cars
        if(j<0){j=640;}
        setfillstyle(SOLID_FILL,YELLOW);
        xct2=j-150; xcb2=j-100;
        readimagefile("carL.jpg",xct2,290,xcb2,340);
        readimagefile("carL.jpg",xct2-100,290,xcb2-100,340);
        readimagefile("carL.jpg",xct2-200,290,xcb2-200,340);
        readimagefile("carL.jpg",xct2-300,290,xcb2-300,340);
        readimagefile("carL.jpg",xct2-400,290,xcb2-400,340);

        //border overlapping left
        setfillstyle(SOLID_FILL,RED);
        bar(0,265,10,355);
        bar(630,265,640,355);
    }
    delay(1000);
    cleardevice();
}

void border()
{
    int snum;
    char str[100];
    sprintf(str,"Remaining Frog : %d",fr-i1);
    setcolor(RED);
	bar(0,0,10,480);
	bar(0,0,640,10);
	bar(630,0,640,480);
	bar(0,470,640,480);

	setfillstyle(SOLID_FILL,YELLOW);
	//track line 1
	bar(10,160,630,165);
	//track line 2
	bar(10,260,630,265);
	//track line 3
	bar(10,360,630,365);

	setcolor(YELLOW);
    settextstyle(8, 0,3);
    outtextxy(150,430,str);
	goal1();
	goal2();
	goal3();
	goal4();
}
void goal1()
{
    setfillstyle(SOLID_FILL,YELLOW);
	//arc(110,90,0,180,60);
	//rectangle(50,90,170,130);
	readimagefile("castle.jpg",50,30,170,130);
	if(op1==1){
        setfillstyle(SOLID_FILL,GREEN);
        bar(100,95,120,125);
	}
}
void goal2()
{
    setfillstyle(SOLID_FILL,YELLOW);
	//arc(250,90,0,180,60);
	//rectangle(190,90,310,130);
    readimagefile("castle.jpg",190,30,310,130);
	if(op2==1){
        setfillstyle(SOLID_FILL,GREEN);
        bar(240,95,260,125);
	}
}
void goal3()
{
    setfillstyle(SOLID_FILL,YELLOW);
	//arc(390,90,0,180,60);
	//rectangle(330,90,450,130);
    readimagefile("castle.jpg",330,30,450,130);
	if(op3==1){
        setfillstyle(SOLID_FILL,GREEN);
        bar(380,95,400,125);
	}
}
void goal4()
{
    setfillstyle(SOLID_FILL,YELLOW);
	//arc(530,90,0,180,60);
	//rectangle(470,90,590,130);
    readimagefile("castle.jpg",470,30,590,130);
	if(op4==1){
        setfillstyle(SOLID_FILL,GREEN);
        bar(520,95,540,125);
	}
}

void restart(){
    cleardevice();

    char ch,prech;
    setcolor(RED);
	bar(0,0,10,480);
	bar(0,0,640,10);
	bar(630,0,640,480);
	bar(0,470,640,480);

    setcolor(GREEN);
    settextstyle(8, 0,3);
    outtextxy(200,50,"To select Press \'UP\'or\'DOWN\' ");
    outtextxy(200,100,"Press \'e\' To Confirm ");
    setcolor(YELLOW);
    outtextxy(100,200,"Do You Want To Play Again");
    outtextxy(100,250,"Yes");
    outtextxy(100,300,"No");
    ch=getche();
    while(ch){

    if((int)ch==72){
        setfillstyle(SOLID_FILL,BLACK);
        bar(150,300,200,400);
        outtextxy(150,250,"<-");
        prech=ch;
    }

    else  if((int)ch==80){
        setfillstyle(SOLID_FILL,BLACK);
        bar(150,250,200,300);
        outtextxy(150,300,"<-");
        prech=ch;
    }
    else if(ch=='e')
    {
        if(prech=='w'){
          yes=1;
        }
        else if(prech=='s'){
            yes=0;
        }
        cleardevice();
        break;
    }
    ch=getche();
    }
    //reset all variables for playing again the game
    op1=0,op2=0,op3=0,op4=0;
    ccount=0;i1=0;coll=0;
    xt=50,yt=395,xb=70,yb=425;

}
void gameInfo(){
    cleardevice();
    readimagefile("startPage.jpg",0,0,640,480);
    getche();
    cleardevice();
    setcolor(RED);
	bar(0,0,10,480);
	bar(0,0,640,10);
	bar(630,0,640,480);
	bar(0,470,640,480);

	setcolor(YELLOW);
    settextstyle(8, 0,1.5);
    outtextxy(50,100,"Kindly Note!!!");
    outtextxy(50,125,"Rules And Instruction For GamePlay");
    outtextxy(75,150,"1.Objects Info");
    outtextxy(100,175,"->Rectangle in Green is the Frog");
    outtextxy(100,200,"->Yellow Squares Moving ");
    outtextxy(100,225,"in Both the tracks are Cars ");
    outtextxy(100,250,"Need to be safe from Them ");
    outtextxy(100,275,"->On the top Yellow Holes Are the Destination");
    outtextxy(75,300,"2.Controlles");
    outtextxy(100,325,"->Press UP to move frog up");
    outtextxy(100,350,"->Press Down to move frog down");
    outtextxy(100,375,"->Press Right to move frog right");
    outtextxy(100,400,"->Press left to move frog left");
    settextstyle(8, 0,3);
    setcolor(GREEN);
    outtextxy(75,425,"Press Anykey To continue");
    getche();
    cleardevice();
}

void frog(){
    char str[100];
    sprintf(str,"Remaining Frog : %d",fr-i1);
    settextstyle(8, 0,3);
    outtextxy(150,430,str);
    setfillstyle(SOLID_FILL,GREEN);
    bar(xt,yt,xb,yb);


}
void *moveFrog(void *vargp){

    char k;
    while(coll!=1&&yt!=95){
        k=getche();
        //for moving frog upwards by step size 100
        if((int)k==72){
            //for limiting the frog from going beyond the frame upwards
            if(yt>130){
            if(yt>365){
            setfillstyle(SOLID_FILL,BLACK);
            bar(10,yt-30,630,yb+35);}
            else
              readimagefile("road.jpg",10,yt-30,630,yb+35);
            yt-=100;
            yb-=100;
            frog();
            }
        }
        //for moving frog downwards by step size 100
        else if((int)k==80){
            //for limiting the frog from going beyond the frame downwards
            if(yb<=325){
            if(yt>365){
            setfillstyle(SOLID_FILL,BLACK);
            bar(10,yt-30,630,yb+35);
            }else readimagefile("road.jpg",10,yt-30,630,yb+35);

            yt+=100;
            yb+=100;
            frog();
            }
            else Beep(150,200);
        }
        //for moving frog left by step size 20
        else if((int)k==75){
            //for limiting the frog from going beyond the frame left direction
            if(xt>10){
            if(yt>365){
            setfillstyle(SOLID_FILL,BLACK);
            bar(10,yt-30,630,yb+35);
            }else readimagefile("road.jpg",10,yt-30,630,yb+35);

            xt-=20;
            xb-=20;
            frog();
            }
        }
        //for moving frog right by step size 20
        else if((int)k==77){
            //for limiting the frog from going beyond the frame right direction
            if(xb<630){
                if(yt>365){
                    setfillstyle(SOLID_FILL,BLACK);
                    bar(10,yt-30,630,yb+35);
                }else readimagefile("road.jpg",10,yt-30,630,yb+35);


            xt+=20;
            xb+=20;
            frog();
            }
        }
    }
}
void winning(){
    if(yt==95&&coll!=1){
        //print win if frog is in the first goal rectangle
        if((xt>=50)&&(xt<170)&&op1==0){
            op1=1;
        }
        //print win if frog is in the second goal rectangle
        else if((xt>=190)&&(xt<310)&&op2==0){
            op2=1;
        }
        //print win if frog is in the third goal rectangle
        else if((xt>=330)&&(xt<450)&&op3==0){
            op3=1;
        }
        //print win if frog is in the four goal rectangle
        else if((xt>=470)&&(xt<590)&&op4==0){
            op4=1;

        }
    }
    if(i1==4)
        Win();
}

void *song(void *vargp){
        while(coll!=1 && yt!=95){
        /*Intro*/
      /*  Beep (330,100);Sleep(100);
        Beep (330,100);Sleep(300);
        Beep (330,100);Sleep(300);
        Beep (262,100);Sleep(100);
        Beep (330,100);Sleep(300);
        Beep (392,100);Sleep(700);
        Beep (196,100);*/
        if(coll==1 || yt==95){break;}
        Beep(440,500);
        Beep(440,500);
        if(coll==1 || yt==95){break;}
        Beep(440,500);
        Beep(523,150);
        if(coll==1 || yt==95){break;}
         Beep(440,500);
         Beep(349,350);
         Beep(523,150);
         Beep(440,1000);
         if(coll==1 || yt==95){break;}
         Beep(659,500);
         Beep(659,500);
         if(coll==1 || yt==95){break;}
         Beep(659,500);
        Beep(698,350);
        Beep(523,150);
         Beep(415,500);
         if(coll==1 || yt==95){break;}
         Beep(349,350);
         if(coll==1 || yt==95){break;}
         Beep(523,150);
         Beep(440,1000);
    }
}

void collision(){
    //for checking collision in track 2
    if(yt==295){
    if(inBetween(xb,xct2,xcb2)==1)
       { coll=1;ccount++;}
    else if(inBetween(xb,xct2-100,xcb2-100)==1)
       { coll=1;ccount++;}
     else if(inBetween(xb,xct2-200,xcb2-200)==1)
        { coll=1;ccount++;}
     else if(inBetween(xb,xct2-300,xcb2-300)==1)
        { coll=1;ccount++;}
     else if(inBetween(xb,xct2-400,xcb2-400)==1)
        {coll=1;ccount++;}
    }
    //for checking collision in track 1
    else if(yt==195){
    if(inBetween(xb,xct1,xcb1)==1)
        {coll=1;ccount++;}
    else if(inBetween(xb,xct1+100,xcb1+100)==1)
        {coll=1;ccount++;}
     else if(inBetween(xb,xct1+200,xcb1+200)==1)
        {coll=1;ccount++;}
     else if(inBetween(xb,xct1+300,xcb1+300)==1)
        {coll=1;ccount++;}
     else if(inBetween(xb,xct1+400,xcb1+400)==1)
        {coll=1;ccount++;}
    }
}
