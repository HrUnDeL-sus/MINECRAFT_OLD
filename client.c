#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <time.h>
#include "world.h"
#include "camera.h"
SOCKET client_socket;
int size_data=0;
int client_UID=-1;
enum ClientAction {
    RenderingWorld,
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
    MoveForward,
    MoveBack,
    SetBlock,
    RotateLeft,
    RotateRight,
    GetCameraPosition,
    Jump
};
int connect_to_server() {
    WSADATA ws;
    WSAStartup( MAKEWORD(2,2), &ws);
    SOCKET s;
    client_socket=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    SOCKADDR_IN adr;
    memset(&adr,0,sizeof(adr));
    adr.sin_family=AF_INET;
    adr.sin_port=htons(1243);
    adr.sin_addr.S_un.S_addr=inet_addr("88.85.171.249");
    return connect(client_socket,&adr,sizeof(adr));


}
void read_world(){
	  clock_t t_start = clock();
	 int n=0;
        float data1[3];
         float data2[size_data];
      int count2=recv(client_socket,data1,sizeof(data1),0);

        set_camera_position(data1[0],data1[1],data1[2]);
        enable_camera();

		 int count=recv(client_socket,data2,sizeof(data2),0);
        int isNull=1;

        for(int k=0; k<size_data; k+=16) {
            float data3[16];
            for(int t=0; t<=15; t+=1)
                data3[t]=data2[k+t];
            for(int i=0; i<15; i+=1) {
                if(data3[i]!=0)
                    isNull=0;
            }
            if(isNull==1)
                return;
            add_object(data3);
        }
        clock_t t_end = clock();
        if((double)(t_end-t_start)>0)
			printf("\n%f",(double)(t_end-t_start));

}
void send_state(int state) {
    int stat;

    stat=state;
    send(client_socket,&stat,sizeof(stat),0);
    if(stat==0) {
		read_world();
    } else {
        int t=0;
        recv(client_socket,&t,sizeof(t),0);
    }
}
