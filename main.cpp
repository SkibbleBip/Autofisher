/*
This application is designed for holding the right click mouse button automatically for autofishing stations in Minecraft. It implements 2 seperate
signal handlers, one for an Interrupt and one for a Termination request. Currently this application is only designed for Windows systems, a Unix
version may be planned in the future. To toggle the right click mouse button on and off, tapping the right shift key will raise a SIGINT which will
be handled accordingly and used to trigger the mouse button toggle. If the User presses Ctrl-Z, regardless if the command window is in focus or not,
the program will raise a SIGTERM signal and exit accordingly. This can serve as an emergency killswitch should something unexpected occur in the game.
*/
#include <iostream>
#include <Windows.h>
#include <signal.h>



bool g_toggle = false;
volatile bool g_running = true;

void signalHandler(int signal);

void stop(int sig){
    //turns the program into a state where it is no longer looping indefinitely

    g_running = false;
    return;

}


int main()
{
    std::cout<< "Application is running now. To hold right click, tap right shift. To let go of right click, tap right shift again.\nTo quit, Tap Control-Z"  <<std::endl;
    signal(SIGINT, signalHandler);
    signal(SIGTERM , stop);
    //listen for the sigint and sigterm signals, attach the handle functions for these signals


    while(g_running){

        if(GetKeyState(VK_RSHIFT)& 0x8000){
            //if right shift is pressed, then raise an interrupt, restart the sigint watchdog
            raise(SIGINT);
            signal(SIGINT, signalHandler);
        }
        if((GetKeyState(VK_CONTROL)& 0x8000) && (GetKeyState(0x5A) & 0x8000 ))
        //if ctrl-z is pressed, raise a sigterm.
            raise(SIGTERM);

        Sleep(10);
        //sleep for 10 milliseconds, without sleep it eats up all the cpu processing
    }

    std::cout<<"Exiting..."<<std::endl;
    return 0;
}


void signalHandler(int signal){


    if(g_toggle == true){
        std::cout<<"Right Click not holding"<<std::endl;

        if(GetKeyState(VK_RBUTTON) & 0x8000)
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
        //lift the mouse right click ONLY IF it's depressed. If this check isnt performed, certain applications can act unexpectedly
        //to an "un-depressed, non-depressed" click

    }
    else{
       std::cout<<"Right Click now holding"<<std::endl;
       mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
       //depress the right click mouse button
    }

    g_toggle = !g_toggle;
    //toggle the global toggle flag
    std::cout<<'\a';
    //print the alarm sound
    while(GetKeyState(VK_RSHIFT)& 0x8000);
    //right shift debouncer, without this the application may attempt to raise and lower the mouse button repeatedly


}



