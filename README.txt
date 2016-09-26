Things are done in this assignment:
1. Putting files to server/getting files from server:transfer files between server and client. Implementing reliable transfer with Acknowledgement server/client sent an ACK socket after receiving a data socket. It will take a few minutes to transfer a relatively large file (>100MB). Thank you for your patience.
2. Listing files on server
3. Exit

How to run:
Use following command to compile:
make

Run the server by typing:
./sercer [port_number]

Run the client by typing:
./client [ip_address][port_number]

Following commands can be used on client:
The server transmits the file to client:
get [file_name]
The server receivces the file from client:
put [file_name]
The server list files in local directory:
ls
The server exit:
exit
For other commans, the server repeats the command back to the client.
