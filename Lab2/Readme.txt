Ibrahim Hayek

Launch.c
-- The program forks a child process				
-- The parent process prints the PID of the child on stderr	
-- The child process executes the supplied command 	
-- The child needs to prepare the new argv structure 	
-- The parent prints the return value of the child on stderr	


Tube.c
-- The program allocates a pipe 	
-- The program forks two children
-- The parent process prints the PID of both children on stderr 
-- The parent process closes access to the pipe and the child processes wires the pipe to allow for inter-process communication
-- The first process executes the first command, and the second process executes the second command  
-- The program prints the return value of the first child and then the second child on stderr

