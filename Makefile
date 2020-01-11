all:
	gcc -o client client.c
	gcc -o awsoutput aws.c
	gcc -o serverAoutput serverA.c
	gcc -o serverBoutput serverB.c -lm
	gcc -o monitoroutput monitor.c
serverA:
	./serverAoutput
serverB:
	./serverBoutput
aws:
	./awsoutput
monitor:
	./monitoroutput


.PHONY: client awsoutput serverAoutput serverBoutput monitoroutput
