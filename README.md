# ICT374A2
Assignment 2 - Simple File Transfer Protocol

This project consists of the designing and implementation of a simple network protocol, client program and server program. The client and server programs will communicate using the simple network protocol, which can then be used to download to a remote site, and upload files from a remote site.

Simple Network Protocol Specification
-	Transport Layer Protocol (TCP) should be used for communication
-	Full specification of the protocol should be provided in the Assignment 2 document separate from client and server implementations
-	Program should be able to transfer both small and large files, text files and binary files.
-	Transferred files and original files must be identical in sizes and contents (checked using the diff command in terminal)
-	Able to fulfill certain functions after a connection is established between the server and client, when an argument entered by the user is mapped to the given function, as seen in the table below:

##	Command Name	              Description	Requirements
- 1	pwd	                        Display the current directory of server that is serving the client	

- 2	lpwd	                      Display the current directory of the client	

- 3	dir	                        Display the file names under the current directory of server that is serving the client	

- 4	ldir	                      Display the file names under the current directory of client 	

- 5	cd <directory_pathname>	    Change current directory of server that is serving the client	Support “.” and “..” notations

- 6	lcd <directory_pathname>	  Change the current directory of client	Support “.” and “..” notations

- 7	get <filename>	            Download named file from current directory of remote server and save it in the current directory of client	
  
- 8	put <filename>	            Upload named file from current directory of client to the current directory of remote server	
  
- 9	quit	                      Terminates the myftp session	
