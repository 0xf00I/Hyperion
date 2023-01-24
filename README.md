# Overview

## The Malware 
The core task of this malware is to retrieve some information of the system the target (OS) is Linux, First activated by running the compiled bytecode then establish a connection with C2 & strat enumerating /proc/ files, Send the content to c2 server, Finaly close the connection and deletes itself.


## C2 Server 
The data extraction module (dext) runs on a well defined port and will start an autonomous thread listening for incoming connections by malware instances. Once connected, the module will simply print on the standard output the content of the incoming connection (which are is the information extracted by the client).

Inspired by this Stack Overflow post. https://stackoverflow.com/a/16379982
