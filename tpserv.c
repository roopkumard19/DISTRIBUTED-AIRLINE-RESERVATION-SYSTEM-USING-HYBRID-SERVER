#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <mysql/mysql.h>
#include <signal.h>
#include <ctype.h>
#include <pthread.h>
#include <stdint.h>
#include <semaphore.h>


/* Declaraton of variables*/
sem_t mutex;
void sig_int(int);
void * doit (void *);
pthread_t tid;
socklen_t clilen;
int *thread_soc;
struct thread thread1;
char prim_key[1024];
int sock,new_sock;
int join,list;
struct sockaddr_in6 server;
struct sockaddr_in6 client;
int length; 
char buffer[2096],buffer1[5000],data[1024],column[1024];
char dat[2096];
char userid[2096];
char password[2096];
char ad_in[2096]="1";
char pri_up[2096]="2";
char gu_del[2096]="3";
char serv[2096] = "localhost";
char user[2096] = "root";
char pass[2096] = "Abcd@1234"; 
char database[2096] = "airlines";
char table[2096]= "air_lines";
int port;
char *sockt= NULL;
char s_no[2096];
char airlines[2096];
char flight_no[2096];
char depart[2096];
char dis[1024];
char arrive[2096];
char depart_date[2096];
char arrive_date[2096];
char depart_time[2096];
char arrive_time[2096];
char seats[2096];
int seats_remain;
char cost[2096];
char discount[2096]="";
char query[2096];
char rate[2096];
MYSQL *establish;
MYSQL_RES *reply;
MYSQL_ROW rows;
int k,k1;

//-------------------------------------Thread structure-----------------------------------------------------
struct thread{

unsigned int sock_new;

}obj;

//------------------------------------decrypt function declaration-------------------------------------------------
void decrypt()
{
printf("password in dedcrypt: %s\n", password);
establish = mysql_init(NULL);

   if((mysql_real_connect(establish, serv, user, pass, database, port, sockt, 0))<0)

{
      printf("Unable to connect to the database\n");
      exit(1);
   }
sprintf(query,"SELECT * FROM pass WHERE hash='%s'",password);

mysql_query(establish,query);
reply = mysql_store_result(establish);
bzero(buffer,2096);

 while (rows = mysql_fetch_row(reply))
{
sprintf(buffer,"%s",rows[0]);
}
mysql_free_result(reply);
   mysql_close(establish);
}


//------------------------------Zombie Handler Function----------------------------------------------------------------------------------

void 
sig_child(int signo)
{
pid_t pid;
int stat;
while((pid=waitpid(-1,&stat,WNOHANG))>0)
printf("child %d terminated\n",pid);
return;
}

void
sig_int(int signo)
{
exit(signo);
}
//------------------------------------Main Function----------------------------------------------------
int main(int argc, char *argv[]) 			// The argc and argv are used to get the command line arguments.
{

if (argc<2)
{
printf("port or filename not provided\n");
}


/*Socket creation*/

sock=socket(AF_INET6, SOCK_STREAM, 0);			// creating a socket with arguments such as domain,connection type (TCP), protocol
if(sock==-1)
{
printf("Socket is not created");		 	// Error when socket not created
exit(-1);
}

printf("Socket created\n");

/* Defining the value for the server structure*/

bzero((char *)&server,sizeof(server));
server.sin6_flowinfo= 0;
server.sin6_family= AF_INET6;                             // Assigning the Family name AF_INET 
server.sin6_port=htons(atoi(argv[1]));			// Assigning the port number for the server 
server.sin6_addr=in6addr_any;			// IP address is not assigned to any specific socket.
port=server.sin6_port;

/*--------------------- Binding of socket---------------------*/

length=sizeof(server);
join=bind(sock, (struct sockaddr *)&server, length);// passing arguments such as sd, server struct & length of struct to the bind function 
if (join==-1)
	{
	printf("Bind has not happened");			// error when bind does not happend
	exit(-1);
	}
struct dirent *ent;



/*Placing the socket in listening mode */ 

list=listen(sock,5);					// passing arguments sd, and queue to the listen function
if (list==-1)
{
printf("Listen has not happened");			// Error when listen function fails
exit(-1);
}

printf("listening..\n");

/* Accepting a connection and creation of new socket */

signal(SIGCHLD,sig_child);  

signal(SIGINT,sig_int);  
for(;;)
{
new_sock=accept(sock,(struct sockaddr *)&client, &length);	 // Connection establishment and creation if new socket.
if (new_sock==-1)
{
//printf("Unable to accept the connection");			// Error when connection is not established.
exit(-1);
}

printf("accepted\n");

//---------------------------------Multi-Processing-----------------------------------------
int pid;
pid=fork();
if(pid<0)
{
	close(new_sock);
	exit(-1);
}
if(pid>0)
{
	close(new_sock);
}
if(pid==0)
{

if(write(new_sock,"\nWELCOME TO THE AIRLINE RESERVATION SYSTEM !!\n\nPLEASE SELECT YOUR PREFERENCE\n\n1)Admin   2)Member   3)Guest",sizeof("WELCOME TO THE AIRLINE RESERVATION SYSTEM !!\n\nPLEASE SELECT YOUR PREFERENCE\n\n1)Admin   2)Member   3)Guest\n\n"))<0) 
	{
	printf("Cannot find the client\n");
	}

	if(read(new_sock,buffer,2096)<0)
	{
	printf("Client type cannot be read\n");
	}

}

strcpy(dat,buffer);
if ((strcasecmp(pri_up,dat)==0)||(strcasecmp(gu_del,dat)==0))
{
//---------------------------------Multi-Processing-----------------------------------------
int pid;
pid=fork();
if(pid<0)
{
	close(new_sock);
	exit(-1);
}
if(pid>0)
{
	close(new_sock);
}
if(pid==0)
{
	close(sock);
//-----------------------------------------Customer Part---------------------------------------
int m,m1;
m=strcasecmp(dat,pri_up);

int check2,check3;
if(m==0)
{
do{
m1=0;
	
	if(write(new_sock,"\nPlease enter your Customer credentials\n\nUsername",sizeof("\nPlease enter your Customer credentials\n\nUsername"))<0) 
	{
	printf("Cannot ask for the credential username\n");
	}
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read the username\n");
	}

	strcpy(userid,buffer);
printf("userid: %s\n",userid);
check2=strcasecmp(userid,"Student");
	bzero(buffer,2096);
	if(write(new_sock,"Password",sizeof("Password"))<0) 
	{
	printf("Cannot ask for the credential password \n");
	}


	bzero(buffer,2096);
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read the password\n");
	}
bzero(password,2096);
	sscanf(buffer,"%s",password);
printf("paswrd: %s\n",password);
decrypt();                                 //Calling Decrypt Function---------------

	
	check3=strcmp(buffer,"Network");

	if((check2==0)&&(check3==0))
	break;
	
	if((check2!=0)||(check3!=0))
        {
	if(write(new_sock,"\nInvalid userid and password combination\n\nENTER OK TO CONTINUE->",sizeof("\nInvalid userid and password combination\n\nENTER OK TO CONTINUE->"))<0) 
	{
	printf("Cannot write the invalid combination \n");
	}
	bzero(buffer,2096);
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read the reply\n");
	}
m1 = 1;
	
	}
}while(m1);
	
	if((check2==0)&&(check3==0))
	{
	if(write(new_sock,"\nLogin Successful!!!\n\nENTER OK TO CONTINUE->",sizeof("\nLogin Successful!!!\n\nENTER OK TO CONTINUE->"))<0) 
	{
	printf("Cannot write \n");
	}
	}
	bzero(buffer,2096);
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read the reply\n");
	}


}/


int check4,check5;
int x1;
check4=strcasecmp(dat,gu_del);
check5=strcasecmp(dat,pri_up);

int check6;
int check7;

if((check4==0)||(check5==0))       // checking if the client is member or guest so that trip type can be selected.
	{
do{
x1=0;

	if(write(new_sock,"\nSelect trip\n\n1.One-way\t2.Round-Trip\n",sizeof("\nSelect trip\n\n1.One-way\t2.Round-Trip\n"))<0) 
		{
		printf("Cannot select the trip \n");
		}

	if(read(new_sock,buffer1,2096)<0)
		{
		printf("Cannot read the trip\n");
		}
check6=strcasecmp(buffer1,ad_in);
check7=strcasecmp(buffer1,pri_up);

if(check6!=0 && check7!=0)
{
if(write(new_sock,"\nIncorrect Entry\n\nENTER OK TO CONTINUE->",sizeof("Incorrect Entry\n\nENTER OK TO CONTINUE->"))<0) 
	{
	printf("Cannot write\n");
	}
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read\n");
	}
x1=1;	
} 

}while(x1);
}

/*----------------------------------------------------------------------------------------------*/



int z1;
int a1=1,b1=1;
check6=strcasecmp(buffer1,ad_in);
check7=strcasecmp(buffer1,pri_up);

do{
z1=0;
if((check6==0)||(check7==0))

{


bzero(buffer,2096);
	if(write(new_sock,"\nEnter the trip details\n\nDeparture_Airport",sizeof("\nEnter the One-way details\n\nDeparture_Airport"))<0) 
	{
	printf("Cannot write \n");
	}
	bzero(buffer,2096);
	if(read(new_sock,depart,2096)<0)
	{
	printf("Cannot read \n");
	}
	bzero(buffer,2096);
	if(write(new_sock,"\nArrival_Airport",sizeof("\nArrival_Airport"))<0) 
	{
	printf("Cannot write Arrival_Airport \n");
	}
	bzero(buffer,2096);
	if(read(new_sock,arrive,2096)<0)
	{
	printf("Cannot read Arrival_Airport\n");
	}
	bzero(buffer,2096);
	if(write(new_sock,"\nDepart_Date",sizeof("\nDepart_Date"))<0) 
	{
	printf("Cannot write Depart_Date \n");
	}
	bzero(buffer,2096);
	if(read(new_sock,depart_date,2096)<0)
	{
	printf("Cannot read Depart_Date\n");
	}

}

if (check7==0)

{

	bzero(buffer,2096);
	if(write(new_sock,"\nReturn_Date",sizeof("\nReturn_Date"))<0) 
	{
	printf("Cannot write Return_Date \n");
	}
	bzero(buffer,2096);
	if(read(new_sock,arrive_date,2096)<0)
	{
	printf("Cannot read Return_Date\n");
	}
}
/*------------------------------sql query-------------------------------------------------------------------*/
if((check6==0)||(check7==0))
{
//database="airlines";

establish = mysql_init(NULL);

   /* Connect to database */

   if((mysql_real_connect(establish, serv, user, pass, database, port, sockt, 0))<0)

{
      printf("Unable to connect to the database\n");
      exit(1);
}

   /* send SQL query */

if (check6==0)
{

sprintf(query,"SELECT * FROM air_lines WHERE Depart='%s' and Arrive='%s' and Depart_Date='%s'",depart,arrive,depart_date);
}

else if (check7==0)

{

sprintf(query,"SELECT * FROM air_lines WHERE Depart='%s' and Arrive='%s' and Depart_Date='%s' and Arrive_Date='%s' ",depart,arrive,depart_date,arrive_date);
}



  mysql_query(establish, query);


   reply = mysql_store_result(establish);

   /* output table name */ 
 
   while (rows = mysql_fetch_row(reply))

{
	bzero(buffer,2096);
if(check5==0)
{
a1=0;

	sprintf(buffer,"\nMatching Airline Details From the DB:\n\nAirlines:%s\nFlight_No:%s\nDeparture_Airport:%s\nArrival_Airport:%s\nDeparture_Time:%s\nArrival_time:%s\nDeparture_Date:%s\nReturn_Date:%s\nSeats_Remaining:%s\nCost($):%s\nDiscount($):%s\n\nEnter OK to proceed",rows[1],rows[2],rows[3],rows[4],rows[5],rows[6],rows[7],rows[8],rows[9],rows[10],rows[11]);
}
if(check4==0)
{
b1=0;

	sprintf(buffer,"\nMatching Airline Details From the DB:\n\nAirlines:%s\nFlight_No:%s\nDeparture_Airport:%s\nArrival_Airport:%s\nDeparture_Time:%s\nArrival_time:%s\nDeparture_Date:%s\nReturn_Date:%s\nSeats_Remaining:%s\nCost (in dollars):%s\n\nEnter OK to proceed",rows[1],rows[2],rows[3],rows[4],rows[5],rows[6],rows[7],rows[8],rows[9],rows[10]);
}
strcpy(rate, rows[10]);
strcpy(prim_key,rows[2]);
strcpy(dis, rows[11]);
seats_remain=atoi(rows[9]);
	if(write(new_sock,buffer,sizeof(buffer))<0) 
	{
	printf("Error sending the database \n");
	}
    bzero(buffer,2096);
	if(read(new_sock,buffer,sizeof(buffer))<0) 
	{
	printf("Error reading the database \n");
	}


        
}
     
   /* close connection */
   mysql_free_result(reply);
   mysql_close(establish);

}

if(a1!=0)
{
if(b1!=0)
{

if(write(new_sock,"\nNo flights for the data selected. Please try again.\n\nENTER OK TO CONTINUE->",sizeof("No flights for the data selected. Please try again.\n\nENTER OK TO CONTINUE->"))<0) 
	{
	printf("Cannot write\n");
	}
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read\n");
	}
z1=1;
}
}
}while(z1);

/*-----------------------Booking of ticket----------------------------------------------------------------*/
int g, g1;
char n_name[2096], n_age[2096], n_seats[2096], n_email[2096];
if((check6==0)||(check7==0))

{
	bzero(buffer,2096);
	if(write(new_sock,"\nEnter the following details\n\nName",sizeof("\nEnter the following details\n\nName"))<0) 
	{
	printf("Cannot enter the name \n");
	}
	bzero(buffer,2096);
	if(read(new_sock,n_name,2096)<0)
	{
	printf("Cannot read name\n");
	}
	printf("name: %s\n",n_name);
	bzero(buffer,2096);
	if(write(new_sock,"\nAge",sizeof("\nAge"))<0) 
	{
	printf("Cannot write Age \n");
	}
	bzero(buffer,2096);
	if(read(new_sock,n_age,2096)<0)
	{
	printf("Cannot read Age\n");
	}
printf("age: %s\n",n_age);
int age;
age=atoi(n_age);
if(isdigit(age)!=0)
{
	bzero(buffer,2096);
	if(write(new_sock,"\nType age in terms of number\n",sizeof("\nType age in terms of number\n"))<0) 
	{
	printf("Cannot write Age \n");
	}
exit(-1);	
}
//---------------------------------------------------------------------------
do
{
g1=0;
	bzero(buffer,2096);
	if(write(new_sock,"\nEnter the Flight Number ",sizeof("Enter the Flight Number "))<0) 
	{
	printf("Cannot write Flight Number\n");
	}
	bzero(flight_no,2096);
	if(read(new_sock,flight_no,2096)<0)
	{
	printf("Cannot read Flight Number\n");
	}
printf("flight: %s\n",flight_no);
if(strcmp(flight_no,prim_key)!=0)
{
if(write(new_sock,"\nIncorrect Flight number\n\nENTER OK TO CONTINUE->",sizeof("Incorrect Flight number\n\nENTER OK TO CONTINUE->"))<0) 
	{
	printf("Cannot write\n");
	}
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read\n");
	}
g1 = 1;
}
} while(g1);

//-----------------------------------------------------------------------------------------------------------
	bzero(buffer,2096);
do
{
g1=0;
	if(write(new_sock,"\nNumber of seats required ",sizeof("\nNumber of seats required "))<0) 
	{
	printf("Cannot write No. of seats required\n");
	}
	
	if(read(new_sock,n_seats,2096)<0)
	{
	printf("Cannot read No. of seats required\n");
	}
printf("seats: %s\n",n_seats);
	if((atoi(n_seats))>seats_remain)
	{
	if(write(new_sock,"\nNumber of seats entered is greater than the seats available\n\nENTER OK TO CONTINUE->",sizeof("\nNo. of seats entered is greater than the seats available\n\nENTER OK TO CONTINUE->"))<0) 
	{
	printf("Cannot write No. of seats required\n");
	}
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read\n");
	}
g1=1;
	}
}while(g1);
//-----------------------------------------------------------------------------------------------------
	bzero(buffer,2096);
	if(write(new_sock,"\nEnter the e-mail ID ",sizeof("\nEnter the e-mail ID "))<0) 
	{
	printf("Cannot write Phone Number\n");
	}
	bzero(buffer,2096);
	if(read(new_sock,n_email,2096)<0)
	{
	printf("Cannot read Phone Number\n");
	}

printf("email: %s\n",n_email);


//---------------------------------------------------------------------------------------------------------

bzero(buffer,2096);
int total;
int r;
printf("ok here\n");
	establish = mysql_init(NULL);
   if((mysql_real_connect(establish, serv, user, pass, database, port, sockt, 0))<0)
{
      printf("Unable to connect to the database");
      exit(1);
}

bzero(query,1024);
sprintf(query,"SELECT Discount FROM air_lines WHERE Flight_No='%s'",flight_no);
mysql_query(establish, query);
reply = mysql_store_result(establish);
rows = mysql_fetch_row(reply);

printf("rate: %s\n",rate);

if(check5==0)
{
strcpy(discount,dis);
}
if(check4==0)
{
bzero(discount,2096);
}
r = atoi(rate)-atoi(discount);

printf("r: %d\n",r);

total=atoi(n_seats)*r;
seats_remain = seats_remain-atoi(n_seats);
sprintf(seats, "%d", seats_remain);
sprintf(buffer, "\nAmount to be paid : %d\n\n Confirm booking(y/n) ",total);
	if(write(new_sock,buffer,sizeof(buffer))<0) 
	{	
	printf("Cannot write Booking successful\n");
	}
	bzero(buffer,2096);
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read the reply\n");
	}
	if(strcmp(buffer,"y")==0)
	{
	bzero(buffer,2096);
establish = mysql_init(NULL);

   if((mysql_real_connect(establish, serv, user, pass, database, port, sockt, 0))<0)

{
      printf("Unable to connect to the database\n");
      exit(1);
}

sprintf(query,"Insert into people values('%s','%s','%s','%s','%d','%s')",n_name, n_age, n_seats, flight_no, total, n_email);

	mysql_query(establish,query);

sprintf(query,"update air_lines set Seats='%s' where Flight_No='%s';",seats, flight_no);
mysql_query(establish,query);

mysql_free_result(reply);
   mysql_close(establish);
	if(write(new_sock,"\nTicket booked successfully!! Enjoy your Trip !!!\n",sizeof("Ticket booked successfully!! Enjoy your Trip !!!\n"))<0) 
	{
	printf("Cannot write Booking successful\n");
	}
	bzero(buffer,2096);
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read the reply\n");
	}
	}
	else
	{
	bzero(buffer,2096);
	if(write(new_sock,"\nPlease Try again later\n\nENTER OK TO CONTINUE-> \n",sizeof("\nPlease Try again later\n\nENTER OK TO CONTINUE-> \n"))<0) 
	{
	printf("Cannot write\n");
	}
	bzero(buffer,2096);
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read the reply\n");
	}
	}
if (strcasecmp(buffer,"ok")==0)

exit(1);

}
} 
// closing the if loop started at the mysql section.(check6 & 7)
/*----------------------------------------------------------------------------------------*/

close(new_sock);
}


if (strcasecmp(dat,ad_in)==0)
{
obj.sock_new = new_sock;
uintptr_t *sock1 = malloc(sizeof(uintptr_t));
*sock1 = new_sock;

//--------------------------------------Mutex FUnction-----------------------------------
sem_init(&mutex,0,1);

//---------------------------------------Multi-Threading-----------------------------------

pthread_create(&tid, NULL, doit,sock1);
printf("Calling doit function\n");



//------------------------------------------------------------------------------------//

}
}
printf("No file entered");
return 0;

}
/*Doit Function */
void* doit (void *sock2)
{


int new_sock1 = *((int *) sock2);
struct thread thread1;
char prim_key[1024];
int join,list;
struct sockaddr_in6 server;
struct sockaddr_in6 client;
int length; 
char buffer[2096],buffer1[5000],data[1024],column[1024];
char dat[2096];
char userid[2096];
char password[2096];
char ad_in[2096]="1";
char pri_up[2096]="2";
char gu_del[2096]="3";
char serv[2096] = "localhost";
char user[2096] = "root";
char pass[2096] = "Abcd@1234"; 
char database[2096] = "airlines";
char table[2096]= "air_lines";
int port;
char *sockt= NULL;
char s_no[2096];
char airlines[2096];
char flight_no[2096];
char depart[2096];
char arrive[2096];
char depart_date[2096];
char arrive_date[2096];
char depart_time[2096];
char arrive_time[2096];
char seats[2096];
int seats_remain;
char cost[2096];
char discount[2096]="";
char query[2096];
char rate[2096];
MYSQL *establish;
MYSQL_RES *reply;
MYSQL_ROW rows;

int k,k1;
// -------------------------decrypt-------------------------//

void decrypt()
{

establish = mysql_init(NULL);

   if((mysql_real_connect(establish, serv, user, pass, database, port, sockt, 0))<0)

{
      printf("Unable to connect to the database\n");
      exit(1);
   }
sprintf(query,"SELECT * FROM pass WHERE hash='%s'",password);

mysql_query(establish,query);
reply = mysql_store_result(establish);
bzero(buffer,2096);

 while (rows = mysql_fetch_row(reply))
{
sprintf(buffer,"%s",rows[0]);
}
mysql_free_result(reply);
   mysql_close(establish);
}



//---------------------------show columns------------------------------------------------------------
void show_columns()
{
char a[1024];


     if((mysql_real_connect(establish, serv, user, pass, database, port, sockt, 0))<0)

{
      printf("Unable to connect to the database\n");
      exit(1);
   }

sprintf(query,"show columns from %s;",table);
printf("query= %s\n",query);
mysql_query(establish,query);

reply = mysql_store_result(establish);

bzero(buffer,2096);
bzero(a,1024);

   /* output table name */
 
   for(int i=0;rows = mysql_fetch_row(reply); i++)
{

sprintf(a,"\n %s ",rows[0]);
strcat(a,""); 
strcat(buffer,a);
      
}

return;
}

//----------------------------show tables--------------------------------
void show_tables()
{
char a[1024];

     if((mysql_real_connect(establish, serv, user, pass, database, port, sockt, 0))<0)

{
      printf("Unable to connect to the database\n");
      exit(1);
   }

sprintf(query,"show tables;");
mysql_query(establish,query);


reply = mysql_store_result(establish);

bzero(buffer,2096);

   /* output table name */
 
   for(int i=0;rows = mysql_fetch_row(reply); i++)
{

sprintf(a,"\n %s ",rows[0]);
strcat(a,""); 
strcat(buffer,a);
      
}

return;
}

//-----------------------update table-----------------------------

//-----------------------------------------------------------------


int check,check1;
do{
k1=0;

	
	if(write(new_sock,"\nPlease enter your Admin credentials\n\nUserid",sizeof("\nPlease enter your Admin credentials\nUserid\n\n"))<0) 
	{
	printf("Cannot ask for the credential userid\n");
	}
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read the username\n");
	}

	strcpy(userid,buffer);
check=strcmp(userid,"123456");
printf("userid: %s\n",userid);
	bzero(buffer,2096);
bzero(password,2096);
	if(write(new_sock,"Password",sizeof("Password"))<0) 
	{
	printf("Cannot ask for the credential password \n");
	}


	bzero(buffer,2096);
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read the password\n");
	}

sscanf(buffer,"%s",password);

printf("paswrd: %s\n",password);
decrypt();
	check1=strcmp(buffer,"Cmpepro");

	if((check==0)&&(check1==0))
	break;
	
	if((check!=0)||(check1!=0))
        {
	if(write(new_sock,"Invalid userid and password combination\n\nENTER OK TO CONTINUE->",sizeof("Invalid userid and password combination\n\nENTER OK TO CONTINUE->"))<0) 
	{
	printf("Cannot write the invalid combination \n");
	}
	bzero(buffer,2096);
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read the reply\n");
	}
k1 = 1;
	
	}
}while(k1);
	
	if((check==0)&&(check1==0))
	{
	if(write(new_sock,"Login Successful!!!\n\nENTER OK TO CONTINUE->",sizeof("Login Successful!!!\n\nENTER OK TO CONTINUE->"))<0) 
	{
	printf("Cannot write \n");
	}
	}
	bzero(buffer,2096);
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read\n");
	}

int p,p1;
sem_wait(&mutex);
 	if(write(new_sock,"\nEnter 1 to SHOW DATABASES  And Select the database required\n\n ",sizeof("\nEnter 1 to SHOW DATABASES And Select the database required\n\n "))<0) 
	{
	printf("Cannot ask for the task.\n");
	}
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read the task.\n");
	}
 
//------------------------------------------Show Databases----------------------------------------------------	

establish = mysql_init(NULL);

   /* Connect to database */


   if((mysql_real_connect(establish, serv, user, pass, database, port, sockt, 0))<0)

{
      printf("Unable to connect to the database\n");
      exit(1);
   }
sprintf(query,"show databases;");
mysql_query(establish,query);
  reply = mysql_store_result(establish);
bzero(buffer,2096);

   /* output table name */ 
 char a[1024];
   for(int i=0;rows = mysql_fetch_row(reply); i++)
{
strcpy(a,rows[0]);
strcat(a,"\n"); 
strcat(buffer,a);       
}

	if(write(new_sock,buffer,sizeof(buffer))<0) 
	{
	printf("Error sending the database \n");
	}
    bzero(buffer,2096);
	if(read(new_sock,buffer,sizeof(buffer))<0) 
	{
	printf("Error reading the database \n");
	}
printf("buffer database: %s\n",buffer);
strcpy(database,buffer);
//-----------------------------------------show tables-------------------------------------------------------------

	if(write(new_sock,"\nEnter 1 to SHOW TABLES  And Select the table required\n\n ",sizeof("\nEnter 1 to SHOW TABLES  And Select the table required\n\n "))<0) 
	{
	printf("Cannot ask for the task.\n");
	}
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read the task.\n");
	}

show_tables();


	if(write(new_sock,buffer,sizeof(buffer))<0) 
	{
	printf("Error sending the database \n");
	}
    bzero(buffer,2096);
	if(read(new_sock,buffer,sizeof(buffer))<0) 
	{
	printf("Error reading the database \n");
	}
printf("buffer: %s\n",buffer);
sscanf(buffer,"%s",table);

//---------------------------------------------------------------------------------
bzero(buffer,2096);
	if(write(new_sock,"\nEnter 1 to SHOW COLUMNS and Press OK to proceed\n\n ",sizeof("\nEnter 1 to SHOW COLUMNS and Press OK to proceed\n\n "))<0) 
	{
	printf("Cannot ask for the task.\n");
	}
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read the task.\n");
	}

show_columns();

	if(write(new_sock,buffer,sizeof(buffer))<0) 
	{
	printf("Error sending the database \n");
	}
    bzero(buffer,2096);
	if(read(new_sock,buffer,sizeof(buffer))<0) 
	{
	printf("Error reading the database \n");
	}

int g;
//---------------------------------------Tasks----------------------------------------------------------------------------------------
do
{
g=0;
 if(write(new_sock,"\nPlease enter the task to be performed.\n\n1.Insert Row\t2.Update Row\t3.Delete Row\t4.View Row data",sizeof("\nPlease enter the task to be performed.\n\n1.Insert Row\t2.Update Row\t3.Delete Row\t4.View Row data"))<0) 
	{
	printf("Cannot ask for the task.\n");
	}
	if(read(new_sock,buffer,2096)<0)
	{
	printf("Cannot read the task.\n");
	}

   /* send SQL query */
int n1;
n1=strcasecmp(buffer,ad_in);

if(n1==0)
{

	if(write(new_sock,"\nEnter the following data to insert a new row\n\nRow_no",sizeof("\nEnter the following data to insert a new row\n\nRow_No"))<0) 
	{
	printf("Cannot send\n");
	}
	bzero(buffer,2096);
	if(read(new_sock,s_no,2096)<0)
	{
	printf("Cannot read the reply\n");
	}

	if(write(new_sock,"Airlines",sizeof("Airlines"))<0) 
	{
	printf("Cannot send airlines");
	}
	fflush(stdout);
	bzero(buffer,2096);
	if(read(new_sock,airlines,2096)<0)
	{
	printf("Cannot read the reply\n");
	}
	
	if(write(new_sock,"Flight number",sizeof("Flight number"))<0) 
	{
	printf("Cannot send flight number");
	}
	fflush(stdout);
	bzero(buffer,2096);
	if(read(new_sock,flight_no,2096)<0)
	{
	printf("Cannot read the reply\n");
	}

	if(write(new_sock,"Depart",sizeof("Depart"))<0) 
	{
	printf("Cannot send Depart");
	}
	fflush(stdout);
	bzero(buffer,2096);
	if(read(new_sock,depart,2096)<0)
	{
	printf("Cannot read the reply\n");
	}

	if(write(new_sock,"Arrive",sizeof("Arrive"))<0) 
	{
	printf("Cannot send Arrive");
	}
	fflush(stdout);
	bzero(buffer,2096);
	if(read(new_sock,arrive,2096)<0)
	{
	printf("Cannot read the reply\n");
	}

	if(write(new_sock,"Depart Date",sizeof("Depart Date"))<0) 
	{
	printf("Cannot send Depart Date");
	}
	fflush(stdout);
	bzero(buffer,2096);
	if(read(new_sock,depart_date,2096)<0)
	{
	printf("Cannot read the reply\n");
	}

	if(write(new_sock,"Arrival Date",sizeof("Arrival Date"))<0) 
	{
	printf("Cannot send Arrival Date");
	}
	fflush(stdout);
	bzero(buffer,2096);
	if(read(new_sock,arrive_date,2096)<0)
	{
	printf("Cannot read the reply\n");
	}

	if(write(new_sock,"Depart Time",sizeof("Depart Time"))<0) 
	{
	printf("Cannot send Depart Time");
	}
	fflush(stdin);
	bzero(buffer,2096);
	if(read(new_sock,depart_time,2096)<0)
	{
	printf("Cannot read the reply\n");
	}

	if(write(new_sock,"Arrival Time",sizeof("Arrival Time"))<0) 
	{
	printf("Cannot send Arrival Time");
	}
	fflush(stdin);
	bzero(buffer,2096);
	if(read(new_sock,arrive_time,2096)<0)
	{
	printf("Cannot read the reply\n");
	}

	if(write(new_sock,"Seats",sizeof("Seats"))<0) 
	{
	printf("Cannot send Seats");
	}
	fflush(stdin);
	bzero(buffer,2096);
	if(read(new_sock,seats,2096)<0)
	{
	printf("Cannot read the reply\n");
	}
	if(write(new_sock,"Cost per ticket",sizeof("Cost per ticket"))<0) 
	{
	printf("Cannot send Cost");
	}
	fflush(stdin);
	bzero(buffer,2096);
	if(read(new_sock,cost,2096)<0)
	{
	printf("Cannot read the reply\n");
	}
	if(write(new_sock,"Discount",sizeof("Discount"))<0) 
	{
	printf("Cannot send Discount Rate");
	}
	fflush(stdin);
	bzero(buffer,2096);
	if(read(new_sock,discount,2096)<0)
	{
	printf("Cannot read the reply\n");
	}
	

	
sscanf(buffer, "%s %s %s %s %s %s %s %s %s %s %s %s",s_no,airlines,flight_no,depart,arrive,depart_date,arrive_date,depart_time,arrive_time,seats,cost,discount);
printf("%s\n",depart);
printf("%s\n",arrive);

printf("%s\n",depart_time);
printf("%s\n",depart_date);

printf("%s\n",seats);
printf("%s\n",cost);
//----------------------------------------Semaphore call-------------------
sem_wait(&mutex);
	sprintf(query,"Insert into air_lines values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",s_no,airlines,flight_no,depart,arrive,depart_time,arrive_time,depart_date,arrive_date,seats,cost,discount);
  	
	if(mysql_query(establish,query)==0)
	{
	bzero(buffer,2096);
	if(write(new_sock,"\nInserted New Row\n\nPress OK to continue",sizeof("\nInserted New Row\n\nPress OK to continue"))<0) 
	{
	printf("Error deleting the data in the database \n");
	}
	}
	else
	{
	bzero(buffer,2096);
	if(write(new_sock,"\nDatabase is not inserted with new values\n\nPress OK to continue",sizeof("\nDatabase is not inserted with new values\n\nPress OK to continue"))<0) 
	{
	printf("Error inserting the data in the database \n");
	}
	}
    
	if(read(new_sock,buffer,sizeof(buffer))<0) 
	{
	printf("Error reading the database \n");
	}
sem_post(&mutex);
}
int n2;
n2=strcasecmp(buffer,pri_up);
if(n2==0)
{
sem_wait(&mutex);
	if(write(new_sock,"\nEnter the following details to update the row data\n\nFlight_No",sizeof("\nEnter the following details to update the row data\n\nFlight_No"))<0) 
	{
	printf("Cannot ask for the task.\n");
	}
fflush(stdout);
	bzero(flight_no,2096);
	if(read(new_sock,flight_no,2096)<0)
	{
	printf("Cannot read the task.\n");
	}
printf("flight: %s\n", flight_no);
	if(write(new_sock,"\nColumn Name",sizeof("\nColumn Name"))<0) 
	{
	printf("Cannot ask for the task.\n");
	}
fflush(stdout);
	bzero(column,2096);
	if(read(new_sock,column,2096)<0)
	{
	printf("Cannot read the task.\n");
	}
printf("col: %s\n", column);
	if(write(new_sock,"\nEnter the data to be updated",sizeof("\nEnter the data to be updated"))<0) 
	{
	printf("Cannot ask for the task.\n");
	}
fflush(stdout);
	bzero(data,2096);
	if(read(new_sock,data,2096)<0)
	{
	printf("Cannot read the task.\n");
	}


     if((mysql_real_connect(establish, serv, user, pass, database, port, sockt, 0))<0)

{
      printf("Unable to connect to the database\n");
      exit(1);
   }

sprintf(query,"update %s set seats='%s' where Flight_No='%s';",table,data,flight_no);
mysql_query(establish,query);
sem_post(&mutex);
}

int n3;
n3=strcasecmp(buffer,gu_del);
if(n3==0)
{
sem_wait(&mutex);	//----------------------------------------Semaphore call-------------------
	if(write(new_sock,"\n\nEnter the following detail to perform DELETE\n\nFlight Number",sizeof("\nEnter the following detail to perform DELETE\n\nFlight Number"))<0) 
	{
	printf("Cannot enter the update field\n  \n");
	}
	
	bzero(buffer,2096);
	if(read(new_sock,flight_no,2096)<0)
	{
	printf("Cannot read the reply\n");
	}
	
	sprintf(query,"Delete from air_lines where Flight_No='%s'",flight_no);
	if(mysql_query(establish,query)==0)
	{
	bzero(buffer,2096);
	if(write(new_sock,"\nData is deleted from the database",sizeof("\nData is deleted from the database"))<0) 
	{
	printf("Error deleting the data in the database \n");
	}
	}
	else
	{
	bzero(buffer,2096);
	if(write(new_sock,"\nData is not deleted from the database",sizeof("\nData is not deleted from the database"))<0) 
	{
	printf("Error updating the database \n");
	}
	}
    
	if(read(new_sock,buffer,sizeof(buffer))<0) 
	{
	printf("Error reading the database \n");
	}
sem_post(&mutex);
}

int n4;
n4=strcasecmp(buffer,"4");
if(n4==0)
{
sem_wait(&mutex); //----------------------------------------Semaphore call-------------------
bzero(buffer,2096);
if(write(new_sock,"\n\nEnter the following detail to View the corresponding Row data\n\nFlight Number",sizeof("\nEnter the following detail to View the corresponding Row data\n\nFlight Number"))<0) 
	{
	printf("Cannot enter the update field\n  \n");
	}
	
	bzero(buffer,2096);
	if(read(new_sock,flight_no,2096)<0)
	{
	printf("Cannot read the reply\n");
	}
	

     if((mysql_real_connect(establish, serv, user, pass, database, port, sockt, 0))<0)

	{
      printf("Unable to connect to the database\n");
      exit(1);
  	 }

	sprintf(query,"SELECT * FROM %s WHERE flight_no='%s'",table,flight_no);
	 mysql_query(establish, query);

 	  reply = mysql_store_result(establish);

   /* output table name */ 
 
   while (rows = mysql_fetch_row(reply))
{

	sprintf(buffer,"\nMatching Airline Details From the DB:\n\nRow_No:%s\nAirlines:%s\nFlight_No:%s\nDeparture_Airport:%s\nArrival_Airport:%s\nDeparture_Time:%s\nArrival_time:%s\nDeparture_Date:%s\nReturn_Date:%s\nSeats_Remaining:%s\nCost($):%s\nDiscount($):%s\n\nEnter OK to proceed",rows[0],rows[1],rows[2],rows[3],rows[4],rows[5],rows[6],rows[7],rows[8],rows[9],rows[10],rows[11]);

}

if(write(new_sock,buffer,sizeof(buffer))<0) 
	{
	printf("Error sending the database \n");
	}
    bzero(buffer,2096);
	if(read(new_sock,buffer,sizeof(buffer))<0) 
	{
	printf("Error reading the database \n");
	}

}
 

	bzero(buffer,2096);
	if(write(new_sock,"\nTask Completed.\n\nDo you want to perform any other task?\n1.Yes\n2.No",sizeof("\nTask Completed.\n\nDo you want to perform any other task?\n1.Yes\n2.No"))<0) 
	{
	printf("Error in writing \n");
	}

    
	if(read(new_sock,buffer,sizeof(buffer))<0) 
	{
	printf("Error in reading \n");
	}

	if(strcmp(buffer,ad_in)==0)
	{
	g=1;
	}

		

}while(g);
sem_post(&mutex); //----------------------------------Semaphore call-------------------
  /* close connection */
   mysql_free_result(reply);
   mysql_close(establish);



close((int) sock2);

}










