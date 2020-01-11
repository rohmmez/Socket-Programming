# Socket-Programming-Project
EE450 Socket Programming Project in 2019 Spring(Used C language)

  In order to run the program, we need to open six terminals. One for running Makefile, one for running client, one for running aws server, one for running server A, one for running server B and one for running monitor.
  
  My code flies include: client.c, aws.c, server A.c, server B.c and monitor.c
  
  cliet.c: In this file, client sends data to aws and receives reslults from aws.

  aws.c: In this file, aws sends data to server A, server B and monitor according to write or compute functions. It also sends results that receives from server B to client. In a word, aws's function is to transfer data.

  serverA.c: It has two functions. It can write data that receives from client to database.txt. And it also can search data in database.txt whatever client wants.

  serverB.c: Server B can compute the data that receives from client and server A. After that, it will send the results to aws.

  monitor.c: monitor's function is to know every steps in this system and represents these steps in the screen.

  In this assignment, I make a client, a monitor and three servers connected. After that, I add two functions into two servers(server A and server B). Firstly, I connect client with aws through sockets. Then the client can send data to aws. Secondly, I connect aws with server A, server B and monitor. That means that aws can send data to server A, server B and monitor separately. Since client, server A, server B and monitor can all connect with aws, client can send data to server A, server B and monitor through aws.
  
   And we also need to achieve two functions in server A and server B. Firstly, we achive write function in server A. Server A in this function acts as a storage. Client sends data to aws and aws sends data to server A. Then server A use fopen(database.txt,"a+") to write data to database.txt. After that, server A has completed this function. Now the database.txt has some data that receive form client. Secondly, we need to achieve compute function. And this function include two steps. The first step is that we need to search the data in database.txt. We do this in server A. After that, server A send the data to aws and then aws send the data to server B. Server B needs to compute 4 numbers that receives from server A and 2 numbers that receives from client. After server B complete this function, it sends results to aws and aws sends results to client. Then all processes are completed. Except these, we need to note that aws also needs to send data to monitor in alomost every steps.
