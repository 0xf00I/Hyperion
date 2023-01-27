# Overview

## The Malware 
First activated by running the compiled bytecode then proceeded to scans the current directory and overwrites all executable files that have not been previously infected with its morphed code, Next the original executable is run from a file it was copied to during the propagation phase to disguise the fact that the actual executable was infected. Finaly establish a connection with C2 & strat enumerating /proc/ files, Send the content to c2 server, Finaly close the connection and deletes itself.

## C2 Server 
The data extraction module (dext) runs on a well defined port and will start an autonomous thread listening for incoming connections by malware instances. Once connected, the module will simply print on the standard output the content of the incoming connection (which are is the information extracted by the client).

Inspired by this Stack Overflow post. https://stackoverflow.com/a/16379982
